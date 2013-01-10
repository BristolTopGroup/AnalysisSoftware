toolsFolder = 'BristolAnalysis/Tools/'
import os
if os.environ.has_key('toolsFolder'):
    toolsFolder = os.environ['toolsFolder']
#File for pile-up re-weighting
PUFile = toolsFolder + "data/PileUp_2012_69300_truth_finebin.root"
#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = toolsFolder + "data/bJetReso.root"
lightJetResoFile = toolsFolder + "data/lightJetReso.root"
#number of events to be processed
maxEvents = 0
#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False

applyMetSysShiftCorr = False
applyMetType0Corr = True
nTuple_version = 9

inputFiles = []

data_path = '/storage/TopQuarkGroup/data/2012/SingleElectron/'
filetype = '*.root'

dataFiles = ['nTuple_v9a_Run2012A-13Jul2012-v1_AOD_GoldenJSON_LeptonPlus3Jets',
             'nTuple_v9a_Run2012A-recover-06Aug2012-v1_AOD_GoldenJSON_LeptonPlus3Jets',
             'nTuple_v9a_Run2012B-13Jul2012-v1_AOD_GoldenJSON_LeptonPlus3Jets',
             'nTuple_v9a_Run2012C-24Aug2012-v1_AOD_GoldenJSON_LeptonPlus3Jets'
              ]

dataFiles = [data_path + path + '/' + filetype for path in dataFiles]
inputFiles.extend(dataFiles)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 10000.0#pb-1

#center of mass energy: 7TeV for 2010/2011 data/MC, 8TeV for 2012 data
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 8

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""
if centerOfMassEnergy == 7:
    datasetInfoFile = toolsFolder + "python/DataSetInfo.py"
elif centerOfMassEnergy == 8:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_8TeV.py"

custom_file_suffix = "noMETphiCorr"
