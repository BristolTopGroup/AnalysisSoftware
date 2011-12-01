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
filetype = '*.root'


mc_path = '/storage/TopQuarkGroup/mc/'


mcFolders = [
           'TTjets_TuneZ2_matchingdown_7TeV-madgraph-tauola/nTuple_v4_Summer11-PU_S4_START42_V11-v2_LeptonSkim',
           'TTjets_TuneZ2_matchingup_7TeV-madgraph-tauola/nTuple_v4_Summer11-PU_S4_START42_V11-v2_LeptonSkim',
           'TTjets_TuneZ2_scaledown_7TeV-madgraph-tauola/nTuple_v4_Summer11-PU_S4_START42_V11-v2_LeptonSkim',
           'TTjets_TuneZ2_scaledown_7TeV-madgraph-tauola/nTuple_v4_Summer11-PU_S4_START42_V11-v2_LeptonSkim',
           ]

mcFolders = [mc_path + path + '/' + filetype for path in mcFolders]

inputFiles.extend(mcFolders)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 4692.363