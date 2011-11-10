#File for pile-up re-weighting
PUFile = "BristolAnalysis/Tools/PileUp2011_v2.root"
#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = "BristolAnalysis/Tools/bJetReso.root"
lightJetResoFile = "BristolAnalysis/Tools/lightJetReso.root"
#number of events to be processed
maxEvents = 0
#use HitFit for analysis
useHitFit = False

inputFiles = []
data_path = '/storage/TopQuarkGroup/data/MuHad/'
filetype = '*.root'

dataFiles = ['nTuple_v4c_Run2011A-05Aug2011-v1_GoldenJSON_muonSkim',
              'nTuple_v4c_Run2011A-May10ReReco-v1_GoldenJSON_muonSkim',
              'nTuple_v4d_Run2011A-PromptReco-v4_GoldenJSON_muonSkim',
              'nTuple_v4d_Run2011A-PromptReco-v6_GoldenJSON_muonSkim',
              'nTuple_v4d_Run2011B-PromptReco-v1_GoldenJSON_muonSkim',
              'nTuple_v4d_Run2011B-PromptReco-v1_GoldenJSON_muonSkim_21.10.11-04.11.11']
dataFiles = [data_path + path + '/' + filetype for path in dataFiles]




inputFiles.extend(dataFiles)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 4692.363