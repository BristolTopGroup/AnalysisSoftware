import glob
import sys
#File for pile-up re-weighting
PUFile = "BristolAnalysis/Tools/PileUp2011_v2.root"
#Jet Energy Resolutions files (L7 corrections)                                                                                                                                                                                     
bJetResoFile = "BristolAnalysis/Tools/bJetReso.root"
lightJetResoFile = "BristolAnalysis/Tools/lightJetReso.root"
#number of events to be processed
maxEvents = 0
#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False

inputFiles = []
filetype = '*.root'

mc_path = '/storage/TopQuarkGroup/mc/'


mcFolders = [
           'TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v5a_Fall11-PU_S6_START42_V14B-v2_LooseLeptonSkim_FAT',
           ]

mcFolders = [mc_path + path for path in mcFolders]

for folder in mcFolders:
    files = glob.glob(folder + '/' + filetype)
    inputFiles.extend(files)

#sort files
inputFiles = sorted(inputFiles)
#use subset
#usage 
#[0:4] will use files with index 0,1,2,3
#[4:8] will use files with index 4,5,6,7
#inputFiles = inputFiles[0:4]

print "Using following files as input:"
if len(inputFiles) == 0:
    print "No input files found! Aborting..."
    sys.exit(1)
for file in inputFiles:
    print file

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 4692.363