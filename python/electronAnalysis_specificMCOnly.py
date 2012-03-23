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
filetype = '*.root'

mc_path = '/storage/TopQuarkGroup/mc/'


mcFolders = [
             #b-quark -> X e nu enriched samples
           'QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           #EM enriched samples
           'QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           #photon + jets
           'GJets_TuneZ2_40_HT_100_7TeV-madgraph/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'GJets_TuneZ2_100_HT_200_7TeV-madgraph/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'GJets_TuneZ2_200_HT_inf_7TeV-madgraph/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'
           ]

mcFolders = [mc_path + path + '/' + filetype for path in mcFolders]

inputFiles.extend(mcFolders)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 5028.#pb-1

#file with information (cross-section, number of processed events) for event weight calculation 
datasetInfoFile = "BristolAnalysis/Tools/python/DataSetInfo.py"