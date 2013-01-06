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
    'QCD_Pt-15to20_MuEnrichedPt5_TuneZ2star_8TeV_pythia6/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v2_LeptonPlus3Jets',
    'QCD_Pt-20to30_MuEnrichedPt5_TuneZ2star_8TeV_pythia6/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
    'QCD_Pt-30to50_MuEnrichedPt5_TuneZ2star_8TeV_pythia6/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
    'QCD_Pt-50to80_MuEnrichedPt5_TuneZ2star_8TeV_pythia6/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
    'QCD_Pt-80to120_MuEnrichedPt5_TuneZ2star_8TeV_pythia6/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
    'QCD_Pt-120to170_MuEnrichedPt5_TuneZ2star_8TeV_pythia6/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
    'QCD_Pt-170to300_MuEnrichedPt5_TuneZ2star_8TeV_pythia6/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
    'QCD_Pt-300to470_MuEnrichedPt5_TuneZ2star_8TeV_pythia6/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
    'QCD_Pt-470to600_MuEnrichedPt5_TuneZ2star_8TeV_pythia6/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
    #QCD_Pt-600to800_MuEnrichedPt5_TuneZ2star_8TeV_pythia6/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
    'QCD_Pt-800to1000_MuEnrichedPt5_TuneZ2star_8TeV_pythia6/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
    'QCD_Pt-1000_MuEnrichedPt5_TuneZ2star_8TeV_pythia6/nTuple_v9a_Summer12_DR53X-PU_S10_START53_V7A-v1_LeptonPlus3Jets',
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
    
