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
filetype = '*.root'


mc_path = '/storage/TopQuarkGroup/mc/'


mcFolders = [
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v4a_Summer11-PU_S4_START42_V11-v2_LeptonSkim/320e12f5112fc77647c2bcc27037229f',
           ]

mcFolders = [mc_path + path + '/' + filetype for path in mcFolders]

inputFiles.extend(mcFolders)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 4692.363