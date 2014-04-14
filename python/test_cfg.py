toolsFolder = 'BristolAnalysis/Tools/'
import os
if os.environ.has_key('toolsFolder'):
    toolsFolder = os.environ['toolsFolder']
    
#center of mass energy: 7TeV for 2010/2011 data/MC, 8TeV for 2012 data
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 8    
nTuple_version = 10
#number of events to be processed
maxEvents = 10000
verbose = True

if centerOfMassEnergy == 8:
    #File for pile-up re-weighting
    #integrated luminosity the MC simulation will be scaled to
    lumi = 19584#pb-1
    PUFile = toolsFolder + "data/PileUp_2012_69300_truth_finebin.root"
    #+5%
#    PUFile = toolsFolder + "data/PileUp_2012_72765_truth_finebin.root"
    #-5%
#    PUFile = toolsFolder + "data/PileUp_2012_65835_truth_finebin.root"

    #apply the met corrections
    applyMetSysShiftCorr = True
    applyMetType0Corr = True

    #Apply Jet Smearing
    applyJetSmearing = True

    #Apply Top Pt Reweighting
    applyTopPtReweighting = True

elif centerOfMassEnergy == 7:
    #File for pile-up re-weighting
    if nTuple_version == 8 or nTuple_version == 10:
        #integrated luminosity the MC simulation will be scaled to
        lumi = 5050#pb-1
        PUFile = toolsFolder + "data/PileUp_2011_truth_finebin_68000microbarn.root"
    #+5%
#        PUFile = toolsFolder + "data/PileUp_2011_truth_finebin_71400microbarn.root"
    #-5%
#        PUFile = toolsFolder + "data/PileUp_2011_truth_finebin_64600microbarn.root"

        #apply the met corrections
        applyMetSysShiftCorr = False
        applyMetType0Corr = False

        #Apply Jet Smearing
        applyJetSmearing = True
    
        #Apply Top Pt Reweighting
        applyTopPtReweighting = False

#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = toolsFolder + "data/bJetReso.root"
lightJetResoFile = toolsFolder + "data/lightJetReso.root"

#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False
inputFiles = [
    '/storage/TopQuarkGroup/data/2012/SingleElectron/nTuple_v10_Run2012D-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets/*.root' # 53X 8TeV data
#    '/storage/TopQuarkGroup/mc/8TeV/LeptonPlus3JetsSkim/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/nTuple_v10_Summer12_DR53X-PU_S10_START53_V7C-v1_LeptonPlus3Jets/*.root' #53X 8TeV MC
#    '/storage/TopQuarkGroup/data/2011/ElectronHad/nTuple_v10_Run2011A-12Oct2013-v1_53X_GoldenJSON_LeptonPlus3Jets/*.root' # 53X 7TeV data
#    ''53X 7TeV MC not available yet
#     '/storage/TopQuarkGroup/data/2011/ElectronHad/nTuple_v10_Run2011B-19Nov2011-v1_44X_GoldenJSON_LeptonPlus3Jets/*.root' #44X 7TeV data
#    '/storage/TopQuarkGroup/mc/7TeV/v10/LeptonPlus3JetsSkim/TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v10_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets/*.root' #44X 7TeV MC
              ]

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""

if centerOfMassEnergy == 7:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_7TeV.py"
elif centerOfMassEnergy == 8:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_8TeV.py"

custom_file_suffix = "TEST" 