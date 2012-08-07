toolsFolder = 'BristolAnalysis/Tools/'
import os
if os.environ.has_key('toolsFolder'):
    toolsFolder = os.environ['toolsFolder']
#File for pile-up re-weighting
PUFile = toolsFolder + "data/PileUp_2011_truth_finebin_68000microbarn.root"
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
             #b-quark -> X e nu enriched samples
           'QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           #EM enriched samples
           'QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           #photon + jets
           'GJets_TuneZ2_40_HT_100_7TeV-madgraph/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'GJets_TuneZ2_100_HT_200_7TeV-madgraph/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets',
           'GJets_TuneZ2_200_HT_inf_7TeV-madgraph/nTuple_v7b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'
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

BTagSystematic = -1