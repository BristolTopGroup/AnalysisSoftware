from ROOT import gROOT, TFile, TH1F

class scaleFactorsForPeriod:
	def __init__(self, fileName, scaleFactorName, lumi):
		self.fileName = fileName
		self.scaleFactorName = scaleFactorName
		self.lumi = lumi

doIsolationSF = True
doMuTriggerSF = False
doETriggerSF = False

if __name__ == '__main__':

	outputFileName = None
	scaleFactorsToAverage = []
	scaleFactorNames = []
	if doIsolationSF:
		scaleFactorNames.append('TightISO_TightID_pt_eta/abseta_pt_ratio')
		scaleFactorNames.append('TightISO_TightID_eta/eta_ratio')
		scaleFactorNames.append('TightISO_TightID_pt/pt_ratio')
		scaleFactorsToAverage = [
			scaleFactorsForPeriod( '../data/EfficienciesAndSF_Isolation_BCDEF.root', 'TightISO_TightID_pt_eta/abseta_pt_ratio', 20.1 ),
			scaleFactorsForPeriod( '../data/EfficienciesAndSF_Isolation_GH.root', 'TightISO_TightID_pt_eta/abseta_pt_ratio', 16.3 )
		]
		outputFileName = '../data/EfficienciesAndSF_Isolation_COMBINED.root'
	elif doMuTriggerSF:
		scaleFactorNames.append('IsoMu24_OR_IsoTkMu24_PtEtaBins/abseta_pt_ratio')
		scaleFactorNames.append('IsoMu24_OR_IsoTkMu24_EtaBins/eta_ratio')
		scaleFactorNames.append('IsoMu24_OR_IsoTkMu24_PtBins/pt_ratio')

		scaleFactorsToAverage = [
			scaleFactorsForPeriod( '../data/EfficienciesAndSF_RunBtoF.root', 'IsoMu24_OR_IsoTkMu24_PtEtaBins/abseta_pt_ratio', 20.1 ),
			scaleFactorsForPeriod( '../data/EfficienciesAndSF_Period4.root', 'IsoMu24_OR_IsoTkMu24_PtEtaBins/abseta_pt_ratio', 16.3 )
		]
		outputFileName = '../data/EfficienciesAndSF_Trigger_COMBINED.root'
	elif doETriggerSF:
		scaleFactorNames.append('Ele32_eta2p1_WPTight_Gsf')
		scaleFactorsToAverage = [
			scaleFactorsForPeriod( '../data/TriggerSF_Run2016BCDEF_v2.root', 'Ele32_eta2p1_WPTight_Gsf', 20.1 ),
			scaleFactorsForPeriod( '../data/TriggerSF_Run2016GH_v2.root', 'Ele32_eta2p1_WPTight_Gsf', 16.3 )
		]
		outputFileName = '../data/EfficienciesAndSF_ElectronTrigger_COMBINED.root'
	else:
		scaleFactorNames.append('MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/abseta_pt_ratio')
		scaleFactorNames.append('MC_NUM_TightID_DEN_genTracks_PAR_eta/eta_ratio')
		scaleFactorNames.append('MC_NUM_TightID_DEN_genTracks_PAR_pt/pt_ratio')
		scaleFactorsToAverage = [
			scaleFactorsForPeriod( '../data/EfficienciesAndSF_BCDEF.root', 'MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/abseta_pt_ratio', 20.1 ),
			scaleFactorsForPeriod( '../data/EfficienciesAndSF_GH.root', 'MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/abseta_pt_ratio', 16.3 )
		]
		outputFileName = '../data/EfficienciesAndSF_COMBINED.root'


outputFile = TFile(outputFileName, 'RECREATE')

for scaleFactorName in scaleFactorNames:
	print scaleFactorName

	firstScaleFactor = True
	averagedScalefactors = None
	sumLumi = 0
	sf_hist = None
	for fileAndLumi in scaleFactorsToAverage:
		inputFile = TFile(fileAndLumi.fileName)
		lumi = fileAndLumi.lumi

	 	gROOT.cd()

		sf_hist = inputFile.Get(scaleFactorName).Clone()
		sumLumi += lumi

		if firstScaleFactor:
			firstScaleFactor = False
			averagedScalefactors = sf_hist
			averagedScalefactors.Scale( lumi )
		else:
			averagedScalefactors.Add( sf_hist, lumi )

		inputFile.Close()

	averagedScalefactors.Scale(1/sumLumi)
	outputFile.cd()
	averagedScalefactors.Write(scaleFactorName.split('/')[-1])

outputFile.Close()

	# for scaleFactors in scaleFactorsToAverage:

	# 	inputFile = TFile(scaleFactors.fileName)

	# 	gROOT.cd()
	# 	scaleFactors.sf_hist = inputFile.Get(scaleFactors.scaleFactorName).Clone()
	# 	sumLumi += scaleFactors.lumi
	# 	if firstScaleFactor:
	# 		firstScaleFactor = False
	# 		averagedScalefactors = scaleFactors.sf_hist.Clone()
	# 		averagedScalefactors.Scale( scaleFactors.lumi )
	# 	else:
	# 		averagedScalefactors.Add( scaleFactors.sf_hist, scaleFactors.lumi )
	# 	inputFile.Close()

	# averagedScalefactors.Scale(1/sumLumi)

	# outputFile = TFile(outputFileName, 'RECREATE')
	# averagedScalefactors.Write('abseta_pt_ratio')
	# outputFile.Close()