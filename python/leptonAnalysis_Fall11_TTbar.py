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
#filetype = '*.root'
filetype = ''

mc_path = '/storage/TopQuarkGroup/mc/'


mcFolders = [
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v5_Fall11-PU_S6_START42_V14B-v2_LeptonPlus3Jets/TTJetsFall11_nTuple_42x_mc_merged_1.root',
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v5_Fall11-PU_S6_START42_V14B-v2_LeptonPlus3Jets/TTJetsFall11_nTuple_42x_mc_merged_2.root',
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v5_Fall11-PU_S6_START42_V14B-v2_LeptonPlus3Jets/TTJetsFall11_nTuple_42x_mc_merged_3.root',
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v5_Fall11-PU_S6_START42_V14B-v2_LeptonPlus3Jets/TTJetsFall11_nTuple_42x_mc_merged_4.root',
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v5_Fall11-PU_S6_START42_V14B-v2_LeptonPlus3Jets/TTJetsFall11_nTuple_42x_mc_merged_5.root',
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v5_Fall11-PU_S6_START42_V14B-v2_LeptonPlus3Jets/TTJetsFall11_nTuple_42x_mc_merged_6.root',
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v5_Fall11-PU_S6_START42_V14B-v2_LeptonPlus3Jets/TTJetsFall11_nTuple_42x_mc_merged_7.root',
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v5_Fall11-PU_S6_START42_V14B-v2_LeptonPlus3Jets/TTJetsFall11_nTuple_42x_mc_merged_8.root',
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v5_Fall11-PU_S6_START42_V14B-v2_LeptonPlus3Jets/TTJetsFall11_nTuple_42x_mc_merged_9.root',
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v5_Fall11-PU_S6_START42_V14B-v2_LeptonPlus3Jets/TTJetsFall11_nTuple_42x_mc_merged_10.root',
           ]

mcFolders = [mc_path + path + '/' + filetype for path in mcFolders]

inputFiles.extend(mcFolders)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 4692.363