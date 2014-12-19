mc_path = '/hdfs/TopQuarkGroup/run2/ntuples/v7/'
data_path = ''
datasets = {
            'TTJet' : [ mc_path + 'TTJets_Madgraph_PU20bx25.root' ],
            'TTJets_Madgraph_PU30bx50' : [ mc_path + 'TTJets_Madgraph_PU30bx50.root'],
            'TTJets_Madgraph_PU4bx50' : [ mc_path + 'TTJets_Madgraph_PU4bx50.root'],
            'TT_Pythia8' : [ mc_path + 'TT_Pythia8_PU20bx25.root'],
            'TT_Pythia8_PU30bx50' : [ mc_path + 'TT_Pythia8_PU30bx50.root'],
            'TT_Pythia8_PU40bx25' : [ mc_path + 'TT_Pythia8_PU40bx25.root'],

            'DYJetsToLL' : [ mc_path + 'DYJetsToLL_M50_PU20bx25.root' ],
            'WJets' : [ mc_path + 'WJets_PU20bx25.root' ],

            'TToLeptons_s' : [mc_path + 'TToLeptons_s_PU20bx25.root'],
            'TToLeptons_t' : [mc_path + 'TToLeptons_t_PU20bx25.root'],
            'T_tw' : [mc_path + 'T_tw_PU20bx25.root'],
            'TBarToLeptons_s' : [mc_path + 'TBarToLeptons_s_PU20bx25.root'],
            'TBarToLeptons_t' : [mc_path + 'TBarToLeptons_t_PU20bx25.root'],
            'Tbar_tw' : [mc_path + 'Tbar_tw_PU20bx25.root'],

            'GJets_100to200' : [mc_path + 'GJets_100to200_PU20bx25.root'],
            'GJets_200to400' : [mc_path + 'GJets_200to400_PU20bx25.root'],
            'GJets_400to600' : [mc_path + 'GJets_400to600_PU20bx25.root'],
            'GJets_600toInf' : [mc_path + 'GJets_600toInf_PU20bx25.root'],

            'QCD_bcToE_20to30' : [mc_path + 'QCD_bcToE_20to30_PU20bx25.root'],
            'QCD_bcToE_30to80' : [mc_path + 'QCD_bcToE_30to80_PU20bx25.root'],
            'QCD_bcToE_80to170' : [mc_path + 'QCD_bcToE_80to170_PU20bx25.root'],
            'QCD_bcToE_170toInf' : [mc_path + 'QCD_bcToE_170toInf_PU20bx25.root'],
            
            'QCD_EMEnriched_20to30' : [mc_path + 'QCD_EMEnriched_20to30_PU20bx25.root'],
            'QCD_EMEnriched_30to80' : [mc_path + 'QCD_EMEnriched_30to80_PU20bx25.root'],
            'QCD_EMEnriched_80to170' : [mc_path + 'QCD_EMEnriched_80to170_PU20bx25.root'],

            'QCD_MuEnriched_20toInf' : [mc_path + 'QCD_MuEnriched_20toInf_PU20bx25.root'],
            'QCD_MuEnriched_30to50' : [mc_path + 'QCD_MuEnriched_30to50_PU20bx25.root'],
            'QCD_MuEnriched_50to80' : [mc_path + 'QCD_MuEnriched_50to80_PU20bx25.root'],
            'QCD_MuEnriched_80to120' : [mc_path + 'QCD_MuEnriched_80to120_PU20bx25.root'],
            }

analysisModes = ['central',
                 ]

available_settings = ['ElectronScaleFactorSystematic', 'MuonScaleFactorSystematic', 'JESsystematic', 'JetSmearingSystematic', 'PUsystematic', 'BTagSystematic', 'LightTagSystematic', 'custom_file_suffix']
default_settings = {
            'ElectronScaleFactorSystematic':0,
            'MuonScaleFactorSystematic':0,
            'JESsystematic':0,
            'JetSmearingSystematic':0,
            'PUFile':'PileUp_2012_69300_truth_finebin.root',
            'MuonScaleFactors':'nofile.root',
            'PUFile':'noFile.root',
            'BTagSystematic':0,
            'LightTagSystematic':0,
            'custom_file_suffix':'',
            'pdfWeightNumber' : 0,
            }

analysis_settings = {
                     'Electron_down':{'ElectronScaleFactorSystematic':-1},
                     'Electron_up':{'ElectronScaleFactorSystematic':1},
                     'Muon_down':{'MuonScaleFactorSystematic':-1},
                     'Muon_up':{'MuonScaleFactorSystematic':1},
                     'BJet_down':{'BTagSystematic':-1},
                     'BJet_up':{'BTagSystematic':1},
                     'JES_down':{'JESsystematic':-1},
                     'JES_up':{'JESsystematic':1},
                     'JetSmearing_up':{'JetSmearingSystematic':1},
                     'JetSmearing_down':{'JetSmearingSystematic':-1},
                     'LightJet_down':{'LightTagSystematic':-1},
                     'LightJet_up':{'LightTagSystematic':1},
                     'PU_down':{'PUFile':'PileUp_2012_65835_truth_finebin.root',
                                'custom_file_suffix':'PU_65835mb'
                                },
                     'PU_up':{'PUFile':'PileUp_2012_72765_truth_finebin.root',
                              'custom_file_suffix':'PU_72765mb'},
                     'Test': {'custom_file_suffix':'TESTING'}
                     }

from copy import deepcopy
#helper functions
def getAnalysisSettings(analysisMode):
    if not analysisMode in analysis_settings.keys():
        return default_settings
    
    settings = deepcopy(default_settings)
    settings.update(analysis_settings[analysisMode])
    return settings


#config start
toolsFolder = 'BristolAnalysis/Tools/'
import os
import sys
if os.environ.has_key('toolsFolder'):
    toolsFolder = os.environ['toolsFolder']

sample = ''
analysisMode = 'central'

if os.environ.has_key('analysisMode') and os.environ.has_key('sample'):
    sample = os.environ['sample']
    analysisMode = os.environ['analysisMode']
    if not analysisMode in analysisModes:
        print 'Unknown analysisMode=', analysisMode
        sys.exit()
    if not sample in datasets.keys():
        print 'Unknown sample=', sample
        sys.exit()
else:
    print 'Environmental variables "analysisMode" and "sample" not set'
    sys.exit()
    
settings = getAnalysisSettings(analysisMode)
if sample in ['TTJets-mcatnlo','TTJets-powheg']:
    suffixes = {'TTJets-mcatnlo':'MCatNLO','TTJets-powheg':'POWHEG'}
    if settings['custom_file_suffix'] == "":
        settings['custom_file_suffix'] = suffixes[sample]
    else:
        settings['custom_file_suffix'] = suffixes[sample] + '_' + settings['custom_file_suffix']    
    
#File for pile-up re-weighting
PUFile = toolsFolder + "data/" + settings['PUFile']
getMuonScaleFactorsFromFile = False
MuonScaleFactorsFile = toolsFolder + "data/" + settings['MuonScaleFactors']
getHadronTriggerFromFile = True
hadronTriggerFile = ''
ElectronScaleFactorSystematic = settings['ElectronScaleFactorSystematic']
MuonScaleFactorSystematic = settings['MuonScaleFactorSystematic']
#JES Systematic, the +/- number of uncertainties to vary the jets with
JESsystematic = settings['JESsystematic']
JetSmearingSystematic = settings['JetSmearingSystematic']
BTagSystematic = settings['BTagSystematic']
LightTagSystematic = settings['LightTagSystematic']
custom_file_suffix = settings['custom_file_suffix']
print 'Parsed config settings:'
for setting,value in settings.iteritems():
    print setting, '=', value
# input_folders = datasets[sample]
# filetype = '*.root'
# inputFiles = [path + '/' + filetype for path in input_folders]
inputFiles = datasets[sample]

#number of events to be processed
maxEvents = 0# 0 == all

#Jet Energy Resolutions files (L7 corrections)
bJetResoFile = toolsFolder + "data/bJetReso.root"
lightJetResoFile = toolsFolder + "data/lightJetReso.root"

#Jet Smearing application
applyJetSmearing = False

#Apply Top Pt reweighting
applyTopPtReweighting = False

#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False

#MET corrections application
applyMetSysShiftCorr = False
applyMetType0Corr = False

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 5000.#pb-1

#center of mass energy: 7TeV for 2010/2011 data/MC, 8TeV for 2012 data
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 13

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""
if centerOfMassEnergy == 13:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_PHYS14.py"


nTuple_version = 0
