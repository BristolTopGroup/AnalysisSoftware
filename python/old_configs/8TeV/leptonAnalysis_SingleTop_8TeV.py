toolsFolder = 'BristolAnalysis/Tools/'
import os
if os.environ.has_key('toolsFolder'):
    toolsFolder = os.environ['toolsFolder']
#File for pile-up re-weighting
PUFile = toolsFolder + "data/PileUp_2012_69300_truth_finebin.root"
#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = toolsFolder + "data/bJetReso.root"
lightJetResoFile = toolsFolder + "data/lightJetReso.root"
#number of events to be processed
maxEvents = 0
#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False

applyMetSysShiftCorr = True
applyMetType0Corr = True
nTuple_version = 9

inputFiles = []
filetype = '*.root'


mc_path = '/storage/TopQuarkGroup/mc/8TeV/LeptonPlus3JetsSkim/'


mcFolders = [
#           #single top samples
           'Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
           'Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
           'T_t-channel_TuneZ2star_8TeV-powheg-tauola/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
           'T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
           'T_s-channel_TuneZ2star_8TeV-powheg-tauola/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
           'Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets'
             ]

mcFolders = [mc_path + path + '/' + filetype for path in mcFolders]

inputFiles.extend(mcFolders)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 10000.#pb-1

#center of mass energy: 7TeV for 2010/2011 data/MC, 8TeV for 2012 data
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 8

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""
if centerOfMassEnergy == 7:
    datasetInfoFile = toolsFolder + "python/DataSetInfo.py"
elif centerOfMassEnergy == 8:
    datasetInfoFile = toolsFolder + "python/DataSetInfo_8TeV.py"
    
