#File for pile-up re-weighting
PUFile = "../PileUp2011_v2.root"
#number of events to be processed
maxEvents = 0
#use HitFit for analysis
useHitFit = False

inputFiles = []
data_path = '/storage/TopQuarkGroup/data/MuHad/'
filetype = '*.root'

dataFiles = ['nTuple_v4c_Run2011A-05Aug2011-v1_GoldenJSON_muonSkim',
              'nTuple_v4c_Run2011A-May10ReReco-v1_GoldenJSON_muonSkim',
              'nTuple_v4d_Run2011A-PromptReco-v4_GoldenJSON_muonSkim',
              'nTuple_v4d_Run2011A-PromptReco-v6_GoldenJSON_muonSkim',
              'nTuple_v4d_Run2011B-PromptReco-v1_GoldenJSON_muonSkim',
              'nTuple_v4d_Run2011B-PromptReco-v1_GoldenJSON_muonSkim_21.10.11-04.11.11']
dataFiles = [data_path + path + '/' + filetype for path in dataFiles]


mc_path = '/storage/TopQuarkGroup/mc/'


mcFolders = ['DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/nTuple_v4d_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
             #insert QCD MC here
           'Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/nTuple_v4a_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
           'Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/nTuple_v4c_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
           'Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/nTuple_v4a_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v4a_Summer11-PU_S4_START42_V11-v2_LeptonSkim/320e12f5112fc77647c2bcc27037229f',
           'T_TuneZ2_s-channel_7TeV-powheg-tauola/nTuple_v4a_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
           'T_TuneZ2_t-channel_7TeV-powheg-tauola/nTuple_v4d_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
           'T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/nTuple_v4a_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
           'WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/nTuple_v4c_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
           'WW_TuneZ2_7TeV_pythia6_tauola/nTuple_v4d_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
           'WZ_TuneZ2_7TeV_pythia6_tauola/nTuple_v4d_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
           'ZZ_TuneZ2_7TeV_pythia6_tauola/nTuple_v4c_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f']

mcFolders = [mc_path + path + '/' + filetype for path in mcFolders]

inputFiles.extend(dataFiles)
inputFiles.extend(mcFolders)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = "../../"

#integrated luminosity the MC simulation will be scaled to
lumi = 4692.363