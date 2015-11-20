from optparse import OptionParser
import sys
import os
import os.path
import subprocess
import glob

jobTemplates = [
			[ 'TTJets_PowhegPythia8_tree.root', ['tree_TTJets_PowhegPythia8_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*.root'] ],
			[ 'TTJets_PowhegPythia8_scaleup_tree.root', ['tree_TTJets_PowhegPythia8_scaleup_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*.root'], 'C' ],
			[ 'TTJets_PowhegPythia8_scaledown_tree.root', ['tree_TTJets_PowhegPythia8_scaledown_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*.root'], 'C' ],
			[ 'TTJets_PowhegPythia8_mtop1695_tree.root', ['tree_TTJets_PowhegPythia8_mtop1695_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*.root'], 'C' ],
			[ 'TTJets_PowhegPythia8_mtop1755_tree.root', ['tree_TTJets_PowhegPythia8_mtop1755_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*.root'], 'C' ],
			['TTJets_amc_tree.root', ['tree_TTJets_amcatnloFXFX_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*.root'], 'C'],
			['TTJets_madgraph_tree.root', ['tree_TTJets_madgraphMLM_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*.root'], 'C'],
			['TTJets_amcatnloHerwigpp_tree.root', ['tree_TTJets_amcatnloHerwigpp_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*.root'], 'C'],

			['data_muon_tree.root', ['tree_SingleMuon*_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET.root'], 'C' ],
			['data_electron_tree.root', ['tree_SingleElectron*_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET.root'], 'C' ],

			['SingleTop_tree.root', ['tree_T_tw_1280.23pb_PFElectron_PFMuon_PF2PATJets*.root', 'tree_Tbar_tw_1280.23pb_PFElectron_PFMuon_PF2PATJets*.root', 'tree_TToLeptons_t_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*.root'] ],
			['VJets_tree.root', ['tree_DYJetsToLL_M50_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*', 'tree_WJetsToLNu_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*'] ],
			['QCD_Electron_tree.root', ['tree_QCD_bcToE_*_1280.23pb_PFElectron_PFMuon_PF2PATJets*', 'tree_QCD_EMEnriched_*_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*'] ],
			['QCD_Muon_tree.root', ['tree_QCD_MuEnriched_*_1280.23pb_PFElectron_PFMuon_PF2PATJets_MET*.root'] ],

		]
jobs = []
for job in jobTemplates:
	jobs.append([job[0],job[1],'central'])
	if len(job) == 2:
		jobs.append([job[0].replace('tree','plusJER_tree'),job[1],'plusJER'])
		jobs.append([job[0].replace('tree','minusJER_tree'),job[1],'minusJER'])
		jobs.append([job[0].replace('tree','plusJES_tree'),job[1],'plusJES'])
		jobs.append([job[0].replace('tree','minusJES_tree'),job[1],'minusJES'])

inputDir = '/hdfs/TopQuarkGroup/run2/atOutput/13TeV/raw_25ns/'
outputDir = '/hdfs/TopQuarkGroup/run2/atOutput/13TeV/25ns/'

def main():
	parser = OptionParser("Merge ntuples on DICE")
	parser.add_option("-n", dest="jobNumber", default=-1,
	                  type='int',
	                  help="Specify which job number to run")
	parser.add_option( '--return_nJobs', dest = 'return_nJobs', action = "store_true",
	                   help = 'Return the total number of jobs'  )

	(options, _) = parser.parse_args()

	if options.return_nJobs:
		print 'N jobs : ',len(jobs)
		return

	jobNumber = options.jobNumber

	if jobNumber < 0 :
		print 'Job number %i too stupid' % jobNumber
		sys.exit()
	if jobNumber > len(jobs) :
		print 'Job number %i too large' % jobNumber
		sys.exit()

	jobToRun = jobs[jobNumber]
	outputName = jobToRun[0]
	inputFileTemplates = jobToRun[1]
	variation = jobToRun[2]

	print "Running merge for :"
	print outputName
	print inputFileTemplates
	print variation

	inputFileString = ''
	for inputFileTemplate in inputFileTemplates:
		print inputDir+'/'+inputFileTemplate
		for f in glob.glob(inputDir+'/'+inputFileTemplate):
			if variation == 'plusJER' and variation in f:
				inputFileString += ' '+f
			elif variation == 'minusJER' and variation in f:
				inputFileString += ' '+f
			elif variation == 'plusJES' and variation in f:
				inputFileString += ' '+f
			elif variation == 'minusJES' and variation in f:
				inputFileString += ' '+f
			elif variation == 'central' and not 'plus' in f and not 'minus' in f:
				inputFileString += ' '+f

	command = 'hadd -f7 ' + outputName + inputFileString
	print command
	p = subprocess.Popen(command, shell=True)
	p.wait()

	if os.path.isfile(outputDir+'/'+outputName):
		os.system('chmod a+w '+outputDir+'/'+outputName)
		os.system('rm '+outputDir+'/'+outputName)
	os.system('mv '+outputName+' '+outputDir+'/'+outputName)
	os.system('chmod a+w '+outputDir+'/'+outputName)

if __name__ == '__main__':
    print main()
