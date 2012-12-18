toolsFolder = './'
import os
if os.environ.has_key('toolsFolder'):
    toolsFolder = os.environ['toolsFolder']
    
#center of mass energy: 7TeV for 2010/2011 data/MC, 8TeV for 2012 data
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 8    
#number of events to be processed
maxEvents = 10000
#integrated luminosity the MC simulation will be scaled to
lumi = 5050#pb-1


if centerOfMassEnergy == 8:
    #File for pile-up re-weighting
    nTuple_version = 9
    PUFile = toolsFolder + "data/PileUp_2012_69300_truth_finebin.root"
    #+5%
#    PUFile = toolsFolder + "data/PileUp_2012_72765_truth_finebin.root"
    #-5%
#    PUFile = toolsFolder + "data/PileUp_2012_65835_truth_finebin.root"
else:
    #File for pile-up re-weighting
    nTuple_version = 8
    PUFile = toolsFolder + "data/PileUp_2011_truth_finebin_68000microbarn.root"
    #+5%
#    PUFile = toolsFolder + "data/PileUp_2011_truth_finebin_71400microbarn.root"
    #-5%
#    PUFile = toolsFolder + "data/PileUp_2011_truth_finebin_64600microbarn.root"



#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = toolsFolder + "data/bJetReso.root"
lightJetResoFile = toolsFolder + "data/lightJetReso.root"

#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False
inputFiles = [
               #'/storage/TopQuarkGroup/mc/8TeV/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/nTuple_v9a_Summer12-PU_S10_START53_V7C-v1_LeptonPlus3Jets/*.root'
               toolsFolder + 'data/TTJet_nTuple_53X_mc.root' 
              ]

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""

if centerOfMassEnergy == 7:
    if nTuple_version == 7:
        datasetInfoFile = toolsFolder + "python/DataSetInfo_V7.py"
    else:
        datasetInfoFile = toolsFolder + "python/DataSetInfo.py"
elif centerOfMassEnergy == 8:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_8TeV.py"

custom_file_suffix = "SYNCH"
