mc_path = '/storage/TopQuarkGroup/mc/7TeV/'
data_path = '/storage/TopQuarkGroup/data/2011/'
datasets = {'ElectronHad' : [data_path + 'ElectronHad/nTuple_v8c_Run2011A-08Nov2011-v1_GoldenJSON_LeptonPlus3Jets',
                            data_path + 'ElectronHad/nTuple_v8c_Run2011B-19Nov2011-v1_GoldenJSON_LeptonPlus3Jets'],
                 'SingleMu' : [data_path + 'SingleMu/nTuple_v8b_Run2011A-08Nov2011-v1_GoldenJSON_LeptonPlus3Jets',
                            data_path + 'SingleMu/nTuple_v8b_Run2011B-19Nov2011-v1_GoldenJSON_LeptonPlus3Jets'],
                 'TTJet' : [mc_path + 'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'DYJetsToLL' : [mc_path + 'DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'QCD_Mu' : [mc_path + 'QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/nTuple_v8b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'QCD_20to30_BCtoE' : [mc_path + 'QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'QCD_30to80_BCtoE' : [mc_path + 'QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'QCD_80to170_BCtoE' : [mc_path + 'QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'QCD_20to30_EMEnriched' : [mc_path + 'QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'QCD_30to80_EMEnriched' : [mc_path + 'QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'QCD_80to170_EMEnriched' : [mc_path + 'QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'GJets_40To100' : [mc_path + 'GJets_TuneZ2_40_HT_100_7TeV-madgraph/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'GJets_100To200' : [mc_path + 'GJets_TuneZ2_100_HT_200_7TeV-madgraph/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'GJets_200' : [mc_path + 'GJets_TuneZ2_200_HT_inf_7TeV-madgraph/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'WW' : [mc_path + 'WW_TuneZ2_7TeV_pythia6_tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'WZ' : [mc_path + 'WZ_TuneZ2_7TeV_pythia6_tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'ZZ' : [mc_path + 'ZZ_TuneZ2_7TeV_pythia6_tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'T_s' : [mc_path + 'T_TuneZ2_s-channel_7TeV-powheg-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'T_t' : [mc_path + 'T_TuneZ2_t-channel_7TeV-powheg-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'T_tW' : [mc_path + 'T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'Tbar_s' : [mc_path + 'Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'Tbar_t' : [mc_path + 'Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'Tbar_tW' : [mc_path + 'Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'TTbarZIncl' : [mc_path + 'TTbarZIncl_TuneZ2_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'TTbarInclWIncl' : [mc_path + 'TTbarInclWIncl_TuneZ2_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'W1Jet' : [mc_path + 'W1Jet_TuneZ2_7TeV-madgraph-tauola/nTuple_v8b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'W2Jets' : [mc_path + 'W2Jets_TuneZ2_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'W3Jets' : [mc_path + 'W3Jets_TuneZ2_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'W4Jets' : [mc_path + 'W4Jets_TuneZ2_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'TTJets-matchingdown' : [mc_path + 'TTjets_TuneZ2_matchingdown_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'TTJets-matchingup' : [mc_path + 'TTjets_TuneZ2_matchingup_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'TTJets-scaledown' : [mc_path + 'TTjets_TuneZ2_scaledown_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'TTJets-scaleup' : [mc_path + 'TTjets_TuneZ2_scaleup_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'WJets-matchingdown' : [mc_path + 'WJetsToLNu_TuneZ2_matchingdown_7TeV-madgraph-tauola/nTuple_v8b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'WJets-matchingup' : [mc_path + 'WJetsToLNu_TuneZ2_matchingup_7TeV-madgraph-tauola/nTuple_v8b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'WJets-scaledown' : [mc_path + 'WJetsToLNu_TuneZ2_scaledown_7TeV-madgraph-tauola/nTuple_v8b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'WJets-scaleup' : [mc_path + 'WJetsToLNu_TuneZ2_scaleup_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'ZJets-matchingdown' : [mc_path + 'ZJetsToLL_TuneZ2_matchingdown_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'ZJets-matchingup' : [mc_path + 'ZJetsToLL_TuneZ2_matchingup_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'ZJets-scaledown' : [mc_path + 'ZJetsToLL_TuneZ2_scaledown_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'ZJets-scaleup' : [mc_path + 'ZJetsToLL_TuneZ2_scaleup_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'TTJets-mcatnlo' : [mc_path + 'TT_TuneZ2_7TeV-mcatnlo/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'TTJets-pythia' : [mc_path + 'TT_TuneZ2_7TeV-powheg-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                 'TTJets-powheg' : [mc_path + 'TT_TuneZ2_7TeV-pythia6-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                     }

analysisModes = ['central',
                 'BJet_down',
                 'BJet_up',
                 'JES_down',
                 'JES_up',
                 'LightJet_down',
                 'LightJet_up',
                 'PU_down',
                 'PU_up',
                 'Test']

analysisModes.extend(['PDFWeights_%d' % weight for weight in range(1, 45)])

available_settings = ['JESsystematic', 'PUsystematic', 'BTagSystematic', 'LightTagSystematic', 'custom_file_suffix']
default_settings = {
            'JESsystematic':0,
            'PUFile':'PileUp_2011_truth_finebin_68000microbarn.root',
            'BTagSystematic':0,
            'LightTagSystematic':0,
            'custom_file_suffix':'',
            'pdfWeightNumber' : 0,
            }

analysis_settings = {
                     'BJet_down':{'BTagSystematic':-1},
                 'BJet_up':{'BTagSystematic':1},
                 'JES_down':{'JESsystematic':-1},
                 'JES_up':{'JESsystematic':1},
                 'LightJet_down':{'LightTagSystematic':-1},
                 'LightJet_up':{'LightTagSystematic':1},
                 'PU_down':{'PUFile':'PileUp_2011_truth_finebin_64600microbarn.root', 
                            'custom_file_suffix':'PU_64600mb'
                            },
                 'PU_up':{'PUFile':'PileUp_2011_truth_finebin_71400microbarn.root', 
                            'custom_file_suffix':'PU_71400mb'},
                'Test': {'custom_file_suffix':'TESTING'}
                     }
for weight in range(1, 45):
    weight_txt = 'PDFWeights_%d' % weight 
    analysis_settings[weight_txt] = {'pdfWeightNumber' : weight,'custom_file_suffix': weight_txt}
from copy import deepcopy
#helperfunctions
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
if sample in ['TTJets-mcatnlo','TTJets-pythia','TTJets-powheg']:
    suffixes = {'TTJets-mcatnlo':'MCatNLO', 'TTJets-pythia':'PYTHIA6','TTJets-powheg':'POWHEG'}
    if settings['custom_file_suffix'] == "":
        settings['custom_file_suffix'] = suffixes[sample]
    else:
        settings['custom_file_suffix'] = suffixes[sample] + '_' + settings['custom_file_suffix']    
    
#File for pile-up re-weighting
PUFile = toolsFolder + "data/" + settings['PUFile']
#JES Systematic, the +/- number of uncertainties to vary the jets with
JESsystematic = settings['JESsystematic']
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
#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 5050.#pb-1

#center of mass energy: 7TeV for 2010/2011 data/MC, 8TeV for 2012 data
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 7

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""
if centerOfMassEnergy == 7:
    datasetInfoFile = toolsFolder + "python/DataSetInfo.py"
elif centerOfMassEnergy == 8:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_8TeV.py"
nTuple_version = 8


