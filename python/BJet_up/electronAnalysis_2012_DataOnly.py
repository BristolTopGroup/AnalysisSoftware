toolsFolder = 'BristolAnalysis/Tools/'
#File for pile-up re-weighting
PUFile = toolsFolder + "data/PileUp_2011_truth_finebin_71400microbarn.root"
#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = toolsFolder + "data/bJetReso.root"
lightJetResoFile = toolsFolder + "data/lightJetReso.root"
#JES Systematic, the +/- number of uncertainties to vary the jets with
JESsystematic = 0
#number of events to be processed
maxEvents = 0
#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False

inputFiles = [
'/storage/TopQuarkGroup/data/2012/ElectronHad/nTuple_v7c_Run2012A-PromptReco-v1_GoldenJSON_LeptonPlus3Jets/*.root'
]

data_path = '/storage/TopQuarkGroup/data/2012/SingleElectron/'
filetype = '*.root'

dataFiles = ['nTuple_v7c_Run2012B-PromptReco-v1_GoldenJSON_LeptonPlus3Jets',
             'nTuple_v7c_Run2012B-PromptReco-v1_GoldenJSON_Runs_195016-195396_LeptonPlus3Jets'
              ]

dataFiles = [data_path + path + '/' + filetype for path in dataFiles]
inputFiles.extend(dataFiles)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 2752.4#pb-1

#center of mass energy: 7TeV for 2010/2011 data/MC, 8TeV for 2012 data
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 8

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""
if centerOfMassEnergy == 7:
    datasetInfoFile = toolsFolder + "python/DataSetInfo.py"
elif centerOfMassEnergy == 8:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_8TeV.py"
nTuple_version = 7

BTagSystematic = 1