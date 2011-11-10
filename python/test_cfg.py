#File for pile-up re-weighting
PUFile = "BristolAnalysis/Tools/PileUp2011_v2.root"
#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = "BristolAnalysis/Tools/bJetReso.root"
lightJetResoFile = "BristolAnalysis/Tools/lightJetReso.root"
#number of events to be processed
maxEvents = 5000
#use HitFit for analysis
useHitFit = False
inputFiles = [
#              '/scratch/TopQuarkGroup/data/ElectronHad/nTuple_v4a_Run2011A-05Aug2011-v1_GoldenJSON_electronSkim/919e07a072adba25d339f168e288c1b2/*.root',
              '/storage/TopQuarkGroup/data/MuHad/nTuple_v4c_Run2011A-05Aug2011-v1_GoldenJSON_muonSkim/*.root',
#              '/storage/TopQuarkGroup/mc/TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v4a_Summer11-PU_S4_START42_V11-v2_LeptonSkim/320e12f5112fc77647c2bcc27037229f/*.root'
              ]

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 5000
