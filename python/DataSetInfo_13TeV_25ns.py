
####################################################################################################
#MCInfo
####################################################################################################
#cross-section: the cross-section of the MC process in pb-1, 
#twiki: https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat13TeV
####################################################################################################
#Number of processed events: Number of events processed by CRAB. Not identical to number of produced events if not all grid jobs succeed!
####################################################################################################
#Number of produced events: Number of total events in the parent sample, i.e. 
#https://cmsweb.cern.ch/das/request?view=list&limit=10&instance=cms_dbs_prod_global&input=dataset%3D%2FElectronHad%2FRun2011A-08Nov2011-v1%2FAOD
####################################################################################################
#Number of selected events: Number of events passing the pre-selection at the nTuple process
####################################################################################################
#Naming of the samples:
#Please use the same naming as in (names array)
#https://svnweb.cern.ch/trac/bat/browser/trunk/AnalysisTools/interface/DataTypes.h
####################################################################################################

datasetInfo = {}

#Data samples
datasetInfo['ElectronHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
# datasetInfo['MuHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

datasetInfo['SingleElectron_RunB'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleElectron_RunC'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleElectron_RunD'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleElectron_RunE'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleElectron_RunF'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleElectron_RunG'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleElectron_RunH_v2'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleElectron_RunH_v3'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

datasetInfo['SingleMuon_RunB'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleMuon_RunC'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleMuon_RunD'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleMuon_RunE'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleMuon_RunF'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleMuon_RunG'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleMuon_RunH_v2'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleMuon_RunH_v3'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

# # Cross sections from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO
datasetInfo['TTJets_PowhegPythia8'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :154948894}
datasetInfo['TTJets_PowhegPythia8_fsrdown'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :59306906}
datasetInfo['TTJets_PowhegPythia8_fsrup'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :59230899}
datasetInfo['TTJets_PowhegPythia8_isrdown'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :58792211}
datasetInfo['TTJets_PowhegPythia8_isrup'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :59033604}
datasetInfo['TTJets_PowhegPythia8_down'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :58338240}
datasetInfo['TTJets_PowhegPythia8_up'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :58953660}
datasetInfo['TTJets_PowhegPythia8_mtop1695'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :58542590}
datasetInfo['TTJets_PowhegPythia8_mtop1755'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :59384660}
datasetInfo['TTJets_PowhegPythia8_hdampdown'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :58163976}
datasetInfo['TTJets_PowhegPythia8_hdampup'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :58858606}
datasetInfo['TTJets_PowhegPythia8_erdOn'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :59882210}
datasetInfo['TTJets_PowhegPythia8_QCDbased_erdOn'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :59620206}
datasetInfo['TTJets_PowhegPythia8_GluonMove'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :59037234}

datasetInfo['TTJets_amcatnloFXFX'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :15065716}
datasetInfo['TTJets_madgraphMLM'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :10139950}
datasetInfo['TTJets_PowhegHerwigpp'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" :59174465}

# # Cross sections from https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat13TeV
# DY New XSec 1921.8 for Z->mumu m(ll)>50

datasetInfo['DYJetsToLL_M50'] = { "cross-section" : 5765.4, "NumberOfProcessedEvents" :49144274}
datasetInfo['DY1JetsToLL_M50'] = { "cross-section" : 1016, "NumberOfProcessedEvents" :62627174}
datasetInfo['DY2JetsToLL_M50'] = { "cross-section" : 331.4, "NumberOfProcessedEvents" :19970551}
datasetInfo['DY3JetsToLL_M50'] = { "cross-section" : 96.36, "NumberOfProcessedEvents" :5856110}
datasetInfo['DY4JetsToLL_M50'] = { "cross-section" : 51.4, "NumberOfProcessedEvents" :4197868}

datasetInfo['WJetsToLNu'] = { "cross-section" : 61526.7, "NumberOfProcessedEvents" :29705748}
datasetInfo['W1JetsToLNu'] = { "cross-section" : 9493, "NumberOfProcessedEvents" :45367044}
datasetInfo['W2JetsToLNu'] = { "cross-section" : 3120, "NumberOfProcessedEvents" :60197766}
datasetInfo['W3JetsToLNu'] = { "cross-section" : 942.3, "NumberOfProcessedEvents" :59067548}
datasetInfo['W4JetsToLNu'] = { "cross-section" : 524.2, "NumberOfProcessedEvents" :29995313}

# # Cross sections from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SingleTopSigma
# datasetInfo['TToLeptons_s'] = { "cross-section" : 1, "NumberOfProcessedEvents" :0 }
# datasetInfo['TBarToLeptons_s'] = { "cross-section" : 1, "NumberOfProcessedEvents" :0 }
# datasetInfo['TBarToLeptons_t'] = { "cross-section" : 1, "NumberOfProcessedEvents" :0 }

datasetInfo['ST_t'] = { "cross-section" : 136.02, "NumberOfProcessedEvents" :66928232}
datasetInfo['STbar_t'] = { "cross-section" : 80.95, "NumberOfProcessedEvents" :38811017}
datasetInfo['ST_s'] = { "cross-section" : 6.35, "NumberOfProcessedEvents" :622990}
datasetInfo['ST_tW'] = { "cross-section" : 35.6, "NumberOfProcessedEvents" :7944854}
datasetInfo['STbar_tW'] = { "cross-section" : 35.6, "NumberOfProcessedEvents" :7931370}

# # # GJet
# datasetInfo['GJets_100to200'] = { "cross-section" : 9226, "NumberOfProcessedEvents" :0} 
# datasetInfo['GJets_200to400'] = { "cross-section" : 2300, "NumberOfProcessedEvents" :0} 
# datasetInfo['GJets_400to600'] = { "cross-section" : 277.4, "NumberOfProcessedEvents" :0} 
# datasetInfo['GJets_600toInf'] = { "cross-section" : 93.38, "NumberOfProcessedEvents" :0} 


# # QCD EM-enriched
# # Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=QCD_*EMEnriched*TuneCUETP8M1*13*&page=0&shown=262173&limit=100
datasetInfo['QCD_EMEnriched_15to20'] = { "cross-section" : 1273000000 * 0.0002, "NumberOfProcessedEvents" :0 }
datasetInfo['QCD_EMEnriched_20to30'] = { "cross-section" : 557600000 * 0.0096, "NumberOfProcessedEvents" :9218954}
datasetInfo['QCD_EMEnriched_30to50'] = { "cross-section" : 136000000 * 0.073, "NumberOfProcessedEvents" :6768384}
datasetInfo['QCD_EMEnriched_50to80'] = { "cross-section" : 19800000 * 0.146, "NumberOfProcessedEvents" :45156163}
datasetInfo['QCD_EMEnriched_80to120'] = { "cross-section" : 2800000 * 0.125, "NumberOfProcessedEvents" :76489397}
datasetInfo['QCD_EMEnriched_120to170'] = { "cross-section" : 477000 * 0.132, "NumberOfProcessedEvents" :77771316}
datasetInfo['QCD_EMEnriched_170to300'] = { "cross-section" : 114000 * 0.165, "NumberOfProcessedEvents" :11540163}
datasetInfo['QCD_EMEnriched_300toInf'] = { "cross-section" : 9000 * 0.15, "NumberOfProcessedEvents" :7373633}
# # QCD BCToE
# # Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=QCD_Pt_*_bcToE_TuneCUETP8M1_*13*&page=0&shown=262173&limit=100
datasetInfo['QCD_bcToE_15to20'] = { "cross-section" : 1272980000 * 0.0002, "NumberOfProcessedEvents" :0} 
datasetInfo['QCD_bcToE_20to30'] = { "cross-section" : 557627000 * 0.00059, "NumberOfProcessedEvents" :0} 
datasetInfo['QCD_bcToE_30to80'] = { "cross-section" : 159068000 * 0.00255, "NumberOfProcessedEvents" :15328096}
datasetInfo['QCD_bcToE_80to170'] = { "cross-section" : 3221000 * 0.01183, "NumberOfProcessedEvents" :14976689}
datasetInfo['QCD_bcToE_170to250'] = { "cross-section" : 105771 * 0.02492, "NumberOfProcessedEvents" :9720760}
datasetInfo['QCD_bcToE_250toInf'] = { "cross-section" : 21094.1 * 0.03375, "NumberOfProcessedEvents" :9773617}
# # QCD Mu-enriched
# # Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=QCD_Pt*_MuEnrichedPt5_TuneCUETP8M1_*13*&page=0&shown=262173&limit=100
datasetInfo['QCD_MuEnriched_15to20'] = { "cross-section" : 1273190000 * 0.003, "NumberOfProcessedEvents" :4141251}
datasetInfo['QCD_MuEnriched_20to30'] = { "cross-section" : 558528000 * 0.0053, "NumberOfProcessedEvents" :30613738}
datasetInfo['QCD_MuEnriched_30to50'] = { "cross-section" : 139803000 * 0.01182, "NumberOfProcessedEvents" :29954815}
datasetInfo['QCD_MuEnriched_50to80'] = { "cross-section" : 19222500 * 0.02276, "NumberOfProcessedEvents" :19806915}
datasetInfo['QCD_MuEnriched_80to120'] = { "cross-section" : 2758420 * 0.03844, "NumberOfProcessedEvents" :23584215}
datasetInfo['QCD_MuEnriched_120to170'] = { "cross-section" : 469797 * 0.05362, "NumberOfProcessedEvents" :8042721}
datasetInfo['QCD_MuEnriched_170to300'] = { "cross-section" : 117989 * 0.07335, "NumberOfProcessedEvents" :17350231}
datasetInfo['QCD_MuEnriched_300to470'] = { "cross-section" : 7820.25 * 0.10196, "NumberOfProcessedEvents" :48995686}
datasetInfo['QCD_MuEnriched_470to600'] = { "cross-section" : 645.528 * 0.12242, "NumberOfProcessedEvents" :18976018}
datasetInfo['QCD_MuEnriched_600to800'] = { "cross-section" : 187.109 * 0.13412, "NumberOfProcessedEvents" :9981311}
datasetInfo['QCD_MuEnriched_800to1000'] = { "cross-section" : 32.3486 * 0.14552, "NumberOfProcessedEvents" :19767439}
datasetInfo['QCD_MuEnriched_1000toInf'] = { "cross-section" : 10.4305 * 0.15544, "NumberOfProcessedEvents" :13400031}

datasetInfo['QCD_50to80'] = { "cross-section" : 19204300, "NumberOfProcessedEvents" :9954370}
datasetInfo['QCD_80to120'] = { "cross-section" : 2762530, "NumberOfProcessedEvents" :7608830}
datasetInfo['QCD_120to170'] = { "cross-section" : 471100, "NumberOfProcessedEvents" :5748736}
datasetInfo['QCD_170to300'] = { "cross-section" : 117276, "NumberOfProcessedEvents" :7838066}
datasetInfo['QCD_300to470'] = { "cross-section" : 7823, "NumberOfProcessedEvents" :18253032}
datasetInfo['QCD_470to600'] = { "cross-section" : 648.2, "NumberOfProcessedEvents" :15458074}
datasetInfo['QCD_600to800'] = { "cross-section" : 186.9, "NumberOfProcessedEvents" :9622896}
datasetInfo['QCD_800to1000'] = { "cross-section" : 32.293, "NumberOfProcessedEvents" :15704980}
datasetInfo['QCD_1000to1400'] = { "cross-section" : 9.4183, "NumberOfProcessedEvents" :6982586}
datasetInfo['QCD_1400to1800'] = { "cross-section" : 0.84265, "NumberOfProcessedEvents" :2477018}
datasetInfo['QCD_1800to2400'] = { "cross-section" : 0.114943, "NumberOfProcessedEvents" :1552064}
datasetInfo['QCD_2400to3200'] = { "cross-section" : 0.00682981, "NumberOfProcessedEvents" :596904}
datasetInfo['QCD_3200toInf'] = { "cross-section" : 0.000165445, "NumberOfProcessedEvents" :391735}

