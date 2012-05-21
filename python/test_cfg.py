#File for pile-up re-weighting
PUFile = "BristolAnalysis/Tools/data/PileUp2011_v2.root"
#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = "BristolAnalysis/Tools/data/bJetReso.root"
lightJetResoFile = "BristolAnalysis/Tools/data/lightJetReso.root"
#number of events to be processed
maxEvents = 5000
#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False
inputFiles = [
#              '/storage/TopQuarkGroup/data/ElectronHad/nTuple_v6_Run2011A-08Nov2011-v1_GoldenJSON_LeptonPlus3Jets/*.root',
#              '/storage/TopQuarkGroup/data/MuHad/nTuple_v6_Run2011B-19Nov2011-v1_GoldenJSON_LeptonPlus3Jets/*.root',
              '/storage/TopQuarkGroup/mc/7TeV/TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v7a_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets_subset/*.root'
#              '/storage/TopQuarkGroup/mc/TTbarZIncl_TuneZ2_7TeV-madgraph-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets/*.root'
#             '/storage/TopQuarkGroup/mc/TTbarInclWIncl_TuneZ2_7TeV-madgraph-tauola/nTuple_v6b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets/*.root'
              ]

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 50000#pb-1

#center of mass energy: 7TeV for 2010/2011 data/MC, 8TeV for 2012 data
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 7

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""
if centerOfMassEnergy == 7:
    datasetInfoFile = "BristolAnalysis/Tools/python/DataSetInfo.py"
elif centerOfMassEnergy == 8:
    datasetInfoFile = "BristolAnalysis/Tools/python/DataSetInfo_8TeV.py"
nTuple_version = 6
