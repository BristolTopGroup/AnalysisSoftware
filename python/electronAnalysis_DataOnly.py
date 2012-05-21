#File for pile-up re-weighting
PUFile = "BristolAnalysis/Tools/data/PileUp_2011_truth_finebin.root"
#Jet Energy Resolutions files (L7 corrections)
bJetResoFile = "BristolAnalysis/Tools/data/bJetReso.root"
lightJetResoFile = "BristolAnalysis/Tools/data/lightJetReso.root"
#number of events to be processed
maxEvents = 0
#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False

inputFiles = []
data_path = '/storage/TopQuarkGroup/data/2011/ElectronHad/'
filetype = '*.root'

dataFiles = ['nTuple_v7a_Run2011A-08Nov2011-v1_GoldenJSON_LeptonPlus3Jets',
              'nTuple_v7a_Run2011B-19Nov2011-v1_GoldenJSON_LeptonPlus3Jets']

dataFiles = [data_path + path + '/' + filetype for path in dataFiles]




inputFiles.extend(dataFiles)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 5028.#pb-1

#center of mass energy: 7TeV for 2010/2011 data/MC, 8TeV for 2012 data
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 7

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""
if centerOfMassEnergy == 7:
    datasetInfoFile = "BristolAnalysis/Tools/python/DataSetInfo.py"
elif centerOfMassEnergy == 8:
    datasetInfoFile = "BristolAnalysis/Tools/python/DataSetInfo_8TeV.py"
nTuple_version = 6
