#File for pile-up re-weighting
PUFile = "../PileUp2011_v2.root"
#number of events to be processed
maxEvents = 0
#use HitFit for analysis
useHitFit = False

inputFiles = []
filetype = '*.root'

mc_path = '/storage/TopQuarkGroup/mc/'


mcFolders = [
                     #insert QCD MC here
             ]

mcFolders = [mc_path + path + '/' + filetype for path in mcFolders]

inputFiles.extend(mcFolders)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = "../../"

#integrated luminosity the MC simulation will be scaled to
lumi = 4692.363