mc_path = '/hdfs/TopQuarkGroup/mc/8TeV/v11/LeptonPlus3JetsSkim/'
data_path = '/hdfs/TopQuarkGroup/data/2012/'
datasets = {'SingleElectron' : [data_path + 'SingleElectron/nTuple_v11_Run2012A-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets',
                                data_path + 'SingleElectron/nTuple_v11_Run2012B-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets',
                                data_path + 'SingleElectron/nTuple_v11_Run2012C-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets',
                                data_path + 'SingleElectron/nTuple_v11_Run2012D-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets'],
            'SingleMu' : [data_path + 'SingleMu/nTuple_v11_Run2012A-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets',
                          data_path + 'SingleMu/nTuple_v11_Run2012B-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets',
                          data_path + 'SingleMu/nTuple_v11_Run2012C-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets',
                          data_path + 'SingleMu/nTuple_v11_Run2012D-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets'],
            'TTJet' : [mc_path + 'TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola'],
            'DY1JetsToLL' : [mc_path + 'DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph'],
            'DY2JetsToLL' : [mc_path + 'DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph'],
            'DY3JetsToLL' : [mc_path + 'DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph'],
            'DY4JetsToLL' : [mc_path + 'DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph'],
            'QCD_Pt-15to20_Mu' : [mc_path + 'QCD_Pt-15to20_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
            'QCD_Pt-20to30_Mu' : [mc_path + 'QCD_Pt-20to30_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
            'QCD_Pt-30to50_Mu' : [mc_path + 'QCD_Pt-30to50_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
            'QCD_Pt-50to80_Mu' : [mc_path + 'QCD_Pt-50to80_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
            'QCD_Pt-80to120_Mu' : [mc_path + 'QCD_Pt-80to120_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
            'QCD_Pt-120to170_Mu' : [mc_path + 'QCD_Pt-120to170_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
            'QCD_Pt-170to300_Mu' : [mc_path + 'QCD_Pt-170to300_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
            'QCD_Pt-300to470_Mu' : [mc_path + 'QCD_Pt-300to470_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
            'QCD_Pt-470to600_Mu' : [mc_path + 'QCD_Pt-470to600_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
            'QCD_Pt-600to800_Mu' : [mc_path + 'QCD_Pt-600to800_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
            'QCD_Pt-800to1000_Mu' : [mc_path + 'QCD_Pt-800to1000_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
            'QCD_Pt-1000_Mu' : [mc_path + 'QCD_Pt-1000_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
            'QCD_20to30_BCtoE' : [mc_path + 'QCD_Pt_20_30_BCtoE_TuneZ2star_8TeV_pythia6'],
            'QCD_30to80_BCtoE' : [mc_path + 'QCD_Pt_30_80_BCtoE_TuneZ2star_8TeV_pythia6'],
            'QCD_80to170_BCtoE' : [mc_path + 'QCD_Pt_80_170_BCtoE_TuneZ2star_8TeV_pythia6'],
            'QCD_170to250_BCtoE' : [mc_path + 'QCD_Pt_170_250_BCtoE_TuneZ2star_8TeV_pythia6'],
            'QCD_250to350_BCtoE' : [mc_path + 'QCD_Pt_250_350_BCtoE_TuneZ2star_8TeV_pythia6'],
            'QCD_350_BCtoE' : [mc_path + 'QCD_Pt_350_BCtoE_TuneZ2star_8TeV_pythia6'],
            'QCD_20to30_EMEnriched' : [mc_path + 'QCD_Pt_20_30_EMEnriched_TuneZ2star_8TeV_pythia6'],
            'QCD_30to80_EMEnriched' : [mc_path + 'QCD_Pt_30_80_EMEnriched_TuneZ2star_8TeV_pythia6'],
            'QCD_80to170_EMEnriched' : [mc_path + 'QCD_Pt_80_170_EMEnriched_TuneZ2star_8TeV_pythia6'],
            'QCD_170to250_EMEnriched' : [mc_path + 'QCD_Pt_170_250_EMEnriched_TuneZ2star_8TeV_pythia6'],
            'QCD_250to350_EMEnriched' : [mc_path + 'QCD_Pt_250_350_EMEnriched_TuneZ2star_8TeV_pythia6'],
            'QCD_350_EMEnriched' : [mc_path + 'QCD_Pt_350_EMEnriched_TuneZ2star_8TeV_pythia6'],
            'GJets_200To400' : [mc_path + 'GJets_HT-200To400_8TeV-madgraph_v2'],
            'GJets_400' : [mc_path + 'GJets_HT-400ToInf_8TeV-madgraph_v2'],
            'T_s' : [mc_path + 'T_s-channel_TuneZ2star_8TeV-powheg-tauola'],
            'T_t' : [mc_path + 'T_t-channel_TuneZ2star_8TeV-powheg-tauola'],
            'T_tW' : [mc_path + 'T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola'],
            'Tbar_s' : [mc_path + 'Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola'],
            'Tbar_t' : [mc_path + 'Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola'],
            'Tbar_tW' : [mc_path + 'Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola'],
            'W1Jet' : [mc_path + 'W1JetsToLNu_TuneZ2Star_8TeV-madgraph'],
            'W2Jets' : [mc_path + 'W2JetsToLNu_TuneZ2Star_8TeV-madgraph'],
            'W3Jets' : [mc_path + 'W3JetsToLNu_TuneZ2Star_8TeV-madgraph'],
            'W4Jets' : [mc_path + 'W4JetsToLNu_TuneZ2Star_8TeV-madgraph'],
            'TTJets-matchingdown' : [mc_path + 'TTJets_matchingdown_TuneZ2star_8TeV-madgraph-tauola'],
            'TTJets-matchingup' : [mc_path + 'TTJets_matchingup_TuneZ2star_8TeV-madgraph-tauola'],
            'TTJets-scaledown' : [mc_path + 'TTJets_scaledown_TuneZ2star_8TeV-madgraph-tauola'],
            'TTJets-scaleup' : [mc_path + 'TTJets_scaleup_TuneZ2star_8TeV-madgraph-tauola'],
            'WJets-matchingdown' : [mc_path + 'WJetsToLNu_matchingdown_8TeV-madgraph-tauola'],
            'WJets-matchingup' : [mc_path + 'WJetsToLNu_matchingup_8TeV-madgraph-tauola'],
            'WJets-scaledown' : [mc_path + 'WJetsToLNu_scaledown_8TeV-madgraph-tauola'],
            'WJets-scaleup' : [mc_path + 'WJetsToLNu_scaleup_8TeV-madgraph-tauola'],
            'ZJets-matchingdown' : [mc_path + 'DYJetsToLL_M-50_matchingdown_8TeV-madgraph-tauola'],
            'ZJets-matchingup' : [mc_path + 'DYJetsToLL_M-50_matchingup_8TeV-madgraph-tauola'],
            'ZJets-scaledown' : [mc_path + 'DYJetsToLL_M-50_scaledown_8TeV-madgraph-tauola'],
            'ZJets-scaleup' : [mc_path + 'DYJetsToLL_M-50_scaleup_8TeV-madgraph-tauola'],
#             'TTJets-mcatnlo' : [mc_path + 'TT_8TeV-mcatnlo'],
#             'TTJets-powheg' : [mc_path + 'TT_CT10_TuneZ2star_8TeV-powheg-tauola'],
#             'TTH_Inclusive_M-125' : [mc_path + 'TTH_Inclusive_M-125_8TeV_pythia6'],
            }

analysisModes = ['central',
                 'Electron_down',
                 'Electron_up',
                 'Muon_down',
                 'Muon_up',
                 'BJet_down',
                 'BJet_up',
                 'JES_down',
                 'JES_up',
                 'LightJet_down',
                 'LightJet_up',
                 'PU_down',
                 'PU_up',
                 'JetSmearing_up',
                 'JetSmearing_down',
                 'Test']

analysisModes.extend(['PDFWeights_%d' % weight for weight in range(1, 45)])

available_settings = ['ElectronScaleFactorSystematic', 'MuonScaleFactorSystematic', 'JESsystematic', 'JetSmearingSystematic', 'PUsystematic', 'BTagSystematic', 'LightTagSystematic', 'custom_file_suffix']
default_settings = {
            'ElectronScaleFactorSystematic':0,
            'MuonScaleFactorSystematic':0,
            'JESsystematic':0,
            'JetSmearingSystematic':0,
            'PUFile':'PileUp_2012_69300_truth_finebin.root',
            'MuonScaleFactorsFile':'nofile.root',
            'ElectronIdIsoScaleFactorsFile': 'nofile.root',
            'ElectronTriggerScaleFactorsFile':'nofile.root',
            'hadronTriggerFile':'nofile.root',
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

for weight in range(1, 45):
    weight_txt = 'PDFWeights_%d' % weight 
    analysis_settings[weight_txt] = {'pdfWeightNumber' : weight,'custom_file_suffix': weight_txt}

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
MuonScaleFactorsFile = toolsFolder + "data/" + settings['MuonScaleFactorsFile']
getElectronScaleFactorsFromFile = False
ElectronScaleFactorsFile = toolsFolder + "data/" + settings["ElectronIdIsoScaleFactorsFile"]
ElectronScaleFactorsFile = toolsFolder + "data/" + settings["ElectronTriggerScaleFactorsFile"]
getHadronTriggerFromFile = False
hadronTriggerFile = toolsFolder + "data/" + settings['hadronTriggerFile']
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
input_folders = datasets[sample]
filetype = '*.root'
inputFiles = [path + '/' + filetype for path in input_folders]

#number of events to be processed
maxEvents = 0# 0 == all

#Jet Energy Resolutions files (L7 corrections)
bJetResoFile = toolsFolder + "data/bJetReso.root"
lightJetResoFile = toolsFolder + "data/lightJetReso.root"

#Jet Smearing application
applyJetSmearing = True

#Apply Top Pt reweighting
applyTopPtReweighting = False

#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False

#MET corrections application
applyMetSysShiftCorr = True
applyMetType0Corr = True

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 19584.#pb-1

#center of mass energy: 7TeV for 2010/2011 data/MC, 8TeV for 2012 data
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 8

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""
if centerOfMassEnergy == 7:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_7TeV.py"
elif centerOfMassEnergy == 8:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_8TeV.py"
nTuple_version = 11
