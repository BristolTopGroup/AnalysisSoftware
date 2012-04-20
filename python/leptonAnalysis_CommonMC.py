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


mcFolders = ['DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
             'WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
             #           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           #only a subset (first 3 files of complete samples. Remember to change the weight as well (DataSetInfo.py)!
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets_subset',
           #single top samples
           'Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'T_TuneZ2_s-channel_7TeV-powheg-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'T_TuneZ2_t-channel_7TeV-powheg-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           #di-boson samples
           'WW_TuneZ2_7TeV_pythia6_tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'WZ_TuneZ2_7TeV_pythia6_tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'ZZ_TuneZ2_7TeV_pythia6_tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           #rare processes
           'TTbarZIncl_TuneZ2_7TeV-madgraph-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'TTbarInclWIncl_TuneZ2_7TeV-madgraph-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets']

mcFolders = [mc_path + path + '/' + filetype for path in mcFolders]

inputFiles.extend(mcFolders)

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
    