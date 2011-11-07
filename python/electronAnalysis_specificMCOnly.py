#File for pile-up re-weighting
PUFile = "../PileUp2011_v2.root"
#number of events to be processed
maxEvents = 0
#use HitFit for analysis
useHitFit = False

inputFiles = []
filetype = '*.root'

mc_path = '/storage/TopQuarkGroup/mc/'


mcFolders = [
           'QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v4c_Summer11-PU_S3_START42_V11-v2/320e12f5112fc77647c2bcc27037229f',
           'QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia/nTuple_v4c_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
           'QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia/nTuple_v4a_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
           'QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v4a_Summer11-PU_S4_START42_V11-v1/320e12f5112fc77647c2bcc27037229f',
           ]

mcFolders = [mc_path + path + '/' + filetype for path in mcFolders]

inputFiles.extend(mcFolders)

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = "../../"

#integrated luminosity the MC simulation will be scaled to
lumi = 4692.363