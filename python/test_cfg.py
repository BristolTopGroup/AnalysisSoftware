toolsFolder = 'BristolAnalysis/Tools/'
toolsFolder = ''
#toolsFolder = ''
#File for pile-up re-weighting
PUFile = toolsFolder + "data/PileUp_2011_truth_finebin_68000microbarn.root"
#+5%
PUFile = toolsFolder + "data/PileUp_2011_truth_finebin_71400microbarn.root"
#-5%
PUFile = toolsFolder + "data/PileUp_2011_truth_finebin_64600microbarn.root"
#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = toolsFolder + "data/bJetReso.root"
lightJetResoFile = toolsFolder + "data/lightJetReso.root"
#JES Systematic, the +/- number of uncertainties to vary the jets with
JESsystematic = 0
#number of events to be processed
maxEvents = 50000
#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False
inputFiles = [
              '/storage/TopQuarkGroup/mc/7TeV/W4Jets_TuneZ2_7TeV-madgraph-tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets/*.root'
#              '/storage/TopQuarkGroup/mc/7TeV/TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets/*.root'
#'/storage/TopQuarkGroup/data/2012/SingleElectron/nTuple_v7c_Run2012B-PromptReco-v1_GoldenJSON_LeptonPlus3Jets/SingleElectron_nTuple_52X_data_merged_001.root'
#              '/storage/TopQuarkGroup/data/2012/SingleElectron/nTuple_v7b_Run2012B-PromptReco-v1_GoldenJSON_LeptonPlus3Jets/*.root',
              ]

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 5050#pb-1

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

custom_file_suffix = "TESTING"
