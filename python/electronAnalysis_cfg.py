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
data_path = '/storage/TopQuarkGroup/data/ElectronHad/'
filetype = '*.root'

dataFiles = ['nTuple_v6_Run2011A-08Nov2011-v1_GoldenJSON_LeptonPlus3Jets',
              'nTuple_v6_Run2011B-19Nov2011-v1_GoldenJSON_LeptonPlus3Jets']
dataFiles = [data_path + path + '/' + filetype for path in dataFiles]


mc_path = '/storage/TopQuarkGroup/mc/'


mcFolders = ['DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v6a_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'T_TuneZ2_s-channel_7TeV-powheg-tauola/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'T_TuneZ2_t-channel_7TeV-powheg-tauola/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'WW_TuneZ2_7TeV_pythia6_tauola/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'WZ_TuneZ2_7TeV_pythia6_tauola/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'ZZ_TuneZ2_7TeV_pythia6_tauola/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           #b-quark -> X e nu enriched samples
           'QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           #EM enriched samples
           'QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           #photon + jets
           'GJets_TuneZ2_40_HT_100_7TeV-madgraph/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'GJets_TuneZ2_100_HT_200_7TeV-madgraph/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'GJets_TuneZ2_200_HT_inf_7TeV-madgraph/nTuple_v6_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets']

mcFolders = [mc_path + path + '/' + filetype for path in mcFolders]

inputFiles.extend(dataFiles)
inputFiles.extend(mcFolders)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 4692.363