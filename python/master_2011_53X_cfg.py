import os
import sys
from copy import deepcopy
from imp import load_source
dirname, _ = os.path.split(os.path.abspath(__file__))
analysis_info = load_source( 'analysis_info', dirname + '/analysis_info.py' )

mc_path = analysis_info.mc_path_7TeV
data_path = analysis_info.data_path_7TeV
datasets = analysis_info.datasets_7TeV

analysisModes = analysis_info.analysisModes

analysisModes.extend(['PDFWeights_%d' % weight for weight in range(1, 45)])

available_settings = ['ElectronScaleFactorSystematic', 'MuonScaleFactorSystematic', 'JESsystematic', 'JetSmearingSystematic', 'PUsystematic', 'BTagSystematic', 'LightTagSystematic', 'custom_file_suffix']
default_settings = {
            'ElectronScaleFactorSystematic':0,
            'MuonScaleFactorSystematic':0,
            'JESsystematic':0,
            'JetSmearingSystematic':0,
            'PUFile':'Data_PUDist_2011Full_central_68000mb_June2014.root',
            'MuonScaleFactorsFile':'MuonEfficiencies_SF_2011_53X_DataMC.root',
            'ElectronIdIsoScaleFactorsFile': 'scaleFactors_electron_id_iso.root',
            'ElectronTriggerScaleFactorsFile':'scaleFactors_electron_trigger.root',
            'hadronTriggerFile':'hadronLegEfficiencies_electron.root',
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
                     'PU_down':{'PUFile':'Data_PUDist_2011Full_64600mb_June2014.root', 
                            'custom_file_suffix':'PU_64600mb'
                            },
                     'PU_up':{'PUFile':'Data_PUDist_2011Full_71400mb_June2014.root', 
                            'custom_file_suffix':'PU_71400mb'},
                     'Test': {'custom_file_suffix':'TESTING'}
                     }

for weight in range(1, 45):
    weight_txt = 'PDFWeights_%d' % weight 
    analysis_settings[weight_txt] = {'pdfWeightNumber' : weight,'custom_file_suffix': weight_txt}
    
#helperfunctions
def getAnalysisSettings(analysisMode):
    if not analysisMode in analysis_settings.keys():
        return default_settings
    
    settings = deepcopy(default_settings)
    settings.update(analysis_settings[analysisMode])
    return settings


#config start
toolsFolder = 'BristolAnalysis/Tools/'
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
getMuonScaleFactorsFromFile = True
MuonScaleFactorsFile = toolsFolder + "data/" + settings['MuonScaleFactorsFile']
getElectronScaleFactorsFromFile = True
ElectronIdIsoScaleFactorsFile = toolsFolder + "data/" + settings['ElectronIdIsoScaleFactorsFile']
ElectronTriggerScaleFactorsFile = toolsFolder + "data/" + settings['ElectronTriggerScaleFactorsFile']
getHadronTriggerFromFile = True
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
lumi = 5050.#pb-1

#center of mass energy: 7TeV for 2010/2011 data/MC, 8TeV for 2012 data
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 7

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""
if centerOfMassEnergy == 7:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_7TeV.py"
elif centerOfMassEnergy == 8:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_8TeV.py"
nTuple_version = 11
