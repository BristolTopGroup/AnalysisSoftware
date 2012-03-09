#File for pile-up re-weighting
PUFile = "BristolAnalysis/Tools/PileUp2011_v2.root"
#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = "BristolAnalysis/Tools/bJetReso.root"
lightJetResoFile = "BristolAnalysis/Tools/lightJetReso.root"
#number of events to be processed
maxEvents = 0
#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False

inputFiles = []
data_path = '/storage/TopQuarkGroup/data/MuHad/'
filetype = '*.root'

dataFiles = ['nTuple_v6_Run2011A-08Nov2011-v1_GoldenJSON_LeptonPlus3Jets',
              'nTuple_v6_Run2011B-19Nov2011-v1_GoldenJSON_LeptonPlus3Jets']
dataFiles = [data_path + path + '/' + filetype for path in dataFiles]




inputFiles.extend(dataFiles)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 5.028#pb-1