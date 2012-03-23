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

#input files for analysis
inputFiles = ['/storage/TopQuarkGroup/data/ElectronHad/nTuple_v6b_Run2011A-08Nov2011-v1_GoldenJSON_LeptonPlus3Jets/ElectronHad_*001.root']

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 3934.085

#file with information (cross-section, number of processed events) for event weight calculation 
datasetInfoFile = "BristolAnalysis/Tools/python/DataSetInfo.py"
