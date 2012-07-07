toolsFolder = 'BristolAnalysis/Tools/'
#File for pile-up re-weighting
PUFile = toolsFolder + "data/PileUp_2011_truth_finebin_71400microbarn.root"
#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = toolsFolder + "data/bJetReso.root"
lightJetResoFile = toolsFolder + "data/lightJetReso.root"
#JES Systematic, the +/- number of uncertainties to vary the jets with
JESsystematic = 0
#number of events to be processed
maxEvents = 0
#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False

inputFiles = []
filetype = '*.root'


mc_path = '/storage/TopQuarkGroup/mc/7TeV/'


mcFolders = [
           #single top samples
           'Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'T_TuneZ2_s-channel_7TeV-powheg-tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'T_TuneZ2_t-channel_7TeV-powheg-tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           #di-boson samples
           'WW_TuneZ2_7TeV_pythia6_tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'WZ_TuneZ2_7TeV_pythia6_tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'ZZ_TuneZ2_7TeV_pythia6_tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           #rare processes
           'TTbarZIncl_TuneZ2_7TeV-madgraph-tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'TTbarInclWIncl_TuneZ2_7TeV-madgraph-tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'
           ]

mcFolders = [mc_path + path + '/' + filetype for path in mcFolders]

inputFiles.extend(mcFolders)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 5050.#pb-1

#center of mass energy: 7TeV for 2010/2011 data/MC, 8TeV for 2012 data
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 7

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""
if centerOfMassEnergy == 7:
    datasetInfoFile = toolsFolder + "python/DataSetInfo.py"
elif centerOfMassEnergy == 8:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_8TeV.py"
nTuple_version = 7

custom_file_suffix = "_PU_71400mb"