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
datasetInfo['MuHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleElectron'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleMu'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

# # Cross sections from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO
datasetInfo['TTJets_PowhegPythia8'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" : 19665194 }
datasetInfo['TTJets_PowhegPythia8_scaleDown'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" : 1999197 }
datasetInfo['TTJets_PowhegPythia8_scaleUp'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" : 1997611 }


datasetInfo['TTJets_amcatnloFXFX'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" : 4994250 }
datasetInfo['TTJets_amcatnloFXFX_scaleDown'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" : 4986572 }
datasetInfo['TTJets_amcatnloFXFX_scaleUp'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" : 4998339 }
datasetInfo['TTJets_amcatnloFXFX_mtop1695'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" : 4921728 }
datasetInfo['TTJets_amcatnloFXFX_mtop1755'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" : 4877056 }

datasetInfo['TTJets_madgraphMLM'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" : 4992231 }
datasetInfo['TTJets_powhegPythia6'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" : 1913779 }

datasetInfo['TTJets_synch'] = { "cross-section" : 831.76, "NumberOfProcessedEvents" : 47000 }

# # Cross sections from https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat13TeV
datasetInfo['DYJetsToLL_M50'] = { "cross-section" : 6025.2, "NumberOfProcessedEvents" : 19925500 }
datasetInfo['WJetsToLNu'] = { "cross-section" : 61526.7, "NumberOfProcessedEvents" : 24089991 }

# # Cross sections from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SingleTopSigma
# datasetInfo['TToLeptons_s'] = { "cross-section" : 1, "NumberOfProcessedEvents" : 1 }
# datasetInfo['TBarToLeptons_s'] = { "cross-section" : 1, "NumberOfProcessedEvents" : 1 }
# datasetInfo['TBarToLeptons_t'] = { "cross-section" : 1, "NumberOfProcessedEvents" : 1 }
datasetInfo['TToLeptons_t'] = { "cross-section" : 136.02, "NumberOfProcessedEvents" : 2985000 }
datasetInfo['T_tw'] = { "cross-section" : 35.6, "NumberOfProcessedEvents" : 998400 }
datasetInfo['Tbar_tw'] = { "cross-section" : 35.6, "NumberOfProcessedEvents" : 1000000 }

# # G+Jet
datasetInfo['GJets_15to6000'] = { "cross-section" : 365896, "NumberOfProcessedEvents" : 9799030 }

# # QCD EM-enriched
# # Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=QCD_*EMEnriched*TuneCUETP8M1*13*&page=0&shown=262173&limit=100
# datasetInfo['QCD_EMEnriched_15to20'] = { "cross-section" : 1273000000 * 0.0002, "NumberOfProcessedEvents" : 2121805 }
# datasetInfo['QCD_EMEnriched_20to30'] = { "cross-section" : 557600000 * 0.0096, "NumberOfProcessedEvents" : 9175903 }
# datasetInfo['QCD_EMEnriched_30to50'] = { "cross-section" : 136000000 * 0.073, "NumberOfProcessedEvents" : 4695884 }
# datasetInfo['QCD_EMEnriched_50to80'] = { "cross-section" : 19800000 * 0.146, "NumberOfProcessedEvents" : 5226884 }
# datasetInfo['QCD_EMEnriched_80to120'] = { "cross-section" : 2800000 * 0.125, "NumberOfProcessedEvents" : 8130424 }
# datasetInfo['QCD_EMEnriched_120to170'] = { "cross-section" : 477000 * 0.132, "NumberOfProcessedEvents" : 8499194 }
# datasetInfo['QCD_EMEnriched_170to300'] = { "cross-section" : 114000 * 0.165, "NumberOfProcessedEvents" : 5735584 }
# # QCD BCToE
# # Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=QCD_Pt_*_bcToE_TuneCUETP8M1_*13*&page=0&shown=262173&limit=100
# datasetInfo['QCD_bcToE_15to20'] = { "cross-section" : 1272980000 * 0.0002, "NumberOfProcessedEvents" : 907678 }
# datasetInfo['QCD_bcToE_20to30'] = { "cross-section" : 557627000 * 0.00059, "NumberOfProcessedEvents" : 3250066 }
# datasetInfo['QCD_bcToE_80to170'] = { "cross-section" : 3221000 * 0.01183, "NumberOfProcessedEvents" : 5141746 }
# datasetInfo['QCD_bcToE_170to250'] = { "cross-section" : 105771 * 0.02492, "NumberOfProcessedEvents" : 2882785 }
# datasetInfo['QCD_bcToE_250toInf'] = { "cross-section" : 21094.1 * 0.03375, "NumberOfProcessedEvents" : 2951115 }
# # QCD Mu-enriched
# # Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=QCD_Pt*_MuEnrichedPt5_TuneCUETP8M1_*13*&page=0&shown=262173&limit=100
datasetInfo['QCD_MuEnriched_15to20'] = { "cross-section" : 1273190000 * 0.003, "NumberOfProcessedEvents" : 2323775 }
datasetInfo['QCD_MuEnriched_20to30'] = { "cross-section" : 558528000 * 0.0053, "NumberOfProcessedEvents" : 5280099 }
datasetInfo['QCD_MuEnriched_30to50'] = { "cross-section" : 139803000 * 0.01182, "NumberOfProcessedEvents" : 4963639 }
datasetInfo['QCD_MuEnriched_50to80'] = { "cross-section" : 19222500 * 0.02276, "NumberOfProcessedEvents" : 5065826 }
datasetInfo['QCD_MuEnriched_80to120'] = { "cross-section" : 2758420 * 0.03844, "NumberOfProcessedEvents" : 3883236 }
datasetInfo['QCD_MuEnriched_120to170'] = { "cross-section" : 469797 * 0.05362, "NumberOfProcessedEvents" : 4020757 }
datasetInfo['QCD_MuEnriched_170to300'] = { "cross-section" : 117989 * 0.07335, "NumberOfProcessedEvents" : 3948326 }
datasetInfo['QCD_MuEnriched_300to470'] = { "cross-section" : 7820.25 * 0.10196, "NumberOfProcessedEvents" : 3927883 }
datasetInfo['QCD_MuEnriched_470to600'] = { "cross-section" : 645.528 * 0.12242, "NumberOfProcessedEvents" : 1990045 }
datasetInfo['QCD_MuEnriched_600to800'] = { "cross-section" : 187.109 * 0.13412, "NumberOfProcessedEvents" : 1982667 }
datasetInfo['QCD_MuEnriched_800to1000'] = { "cross-section" : 32.3486 * 0.14552, "NumberOfProcessedEvents" : 1990869 }
datasetInfo['QCD_MuEnriched_1000toInf'] = { "cross-section" : 10.4305 * 0.15544, "NumberOfProcessedEvents" : 1963183 }



# PHYS 14
# #if using the designated subset:
# datasetInfo['TTJet'] = {"cross-section": 831.76, "NumberOfProcessedEvents":25446993}
# datasetInfo['TTJets_Madgraph_PU30bx50'] = {"cross-section": 831.76, "NumberOfProcessedEvents":25484471}
# datasetInfo['TTJets_Madgraph_PU4bx50'] = {"cross-section": 831.76, "NumberOfProcessedEvents":24547557}
# datasetInfo['TT_Pythia8'] = {"cross-section": 831.76, "NumberOfProcessedEvents":2991609}
# datasetInfo['TT_Pythia8_PU30bx50'] = {"cross-section": 831.76, "NumberOfProcessedEvents":2991609}
# datasetInfo['TT_Pythia8_PU40bx25'] = {"cross-section": 831.76, "NumberOfProcessedEvents":2991609}

# # Single top
# # Cross sections from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SingleTopSigma
# datasetInfo['TToLeptons_s'] = {"cross-section": 7.20, "NumberOfProcessedEvents":500000}
# datasetInfo['TToLeptons_t'] = {"cross-section": 136.05, "NumberOfProcessedEvents":3991000}
# datasetInfo['T_tw'] = {"cross-section": 35.6, "NumberOfProcessedEvents":986100}

# datasetInfo['TBarToLeptons_s'] = {"cross-section": 4.16, "NumberOfProcessedEvents":250000}
# datasetInfo['TBarToLeptons_t'] = {"cross-section": 80.97, "NumberOfProcessedEvents":1999800}
# datasetInfo['Tbar_tw'] = {"cross-section": 35.6, "NumberOfProcessedEvents":971800}

# # V+Jets
# # Cross sections from https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat13TeV
# datasetInfo['DYJetsToLL'] = {"cross-section": 9616.8, "NumberOfProcessedEvents":2829164}
# datasetInfo['WJets'] = {"cross-section": 61526.7, "NumberOfProcessedEvents":2829164}



# #
# # CROSS SECTIONS FOR QCD CRAP FROM 8 TEV
# #

# # G+Jets
# # Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=GJets_HT*13*&page=0&shown=262173
# datasetInfo['GJets_100to200'] = {"cross-section": 1534, "NumberOfProcessedEvents":4734234}
# datasetInfo['GJets_200to400'] = {"cross-section": 489.9, "NumberOfProcessedEvents":4533420}
# datasetInfo['GJets_400to600'] = {"cross-section": 62.05, "NumberOfProcessedEvents":4560801}
# datasetInfo['GJets_600toInf'] = {"cross-section": 20.87, "NumberOfProcessedEvents":4341179}

# # QCD BCToE
# # Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=QCD_*_bcToE_Tune4C_13TeV_pythia8&page=0&shown=134479933
# datasetInfo['QCD_bcToE_20to30'] = {"cross-section": 675900000 * 0.00075, "NumberOfProcessedEvents":999926}
# datasetInfo['QCD_bcToE_30to80'] = {"cross-section": 185900000 * 0.00272, "NumberOfProcessedEvents":1852166}
# datasetInfo['QCD_bcToE_80to170'] = {"cross-section": 3495000 * 0.01225, "NumberOfProcessedEvents":1000671}
# datasetInfo['QCD_bcToE_170toInf'] = {"cross-section": 128500 * 0.0406, "NumberOfProcessedEvents":1000221}

# # QCD EM-enriched
# # Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=QCD_*EMEnriched*13*&page=0&shown=262173
# datasetInfo['QCD_EMEnriched_20to30'] = {"cross-section": 677300000 * 0.007, "NumberOfProcessedEvents":1987127}
# datasetInfo['QCD_EMEnriched_30to80'] = {"cross-section": 185900000 * 0.056, "NumberOfProcessedEvents":2000838}
# datasetInfo['QCD_EMEnriched_80to170'] = {"cross-section": 3529000 * 0.158, "NumberOfProcessedEvents":1959507}
# datasetInfo['QCD_EMEnriched_170to250'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
# datasetInfo['QCD_EMEnriched_250to350'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
# datasetInfo['QCD_EMEnriched_350'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

# # QCD Mu-enriched
# # Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=QCD_*MuEnriched*13*&page=0&shown=262173
# datasetInfo['QCD_MuEnriched_20toInf'] = {"cross-section": 866600000 * 0.00044, "NumberOfProcessedEvents":4777926}
# datasetInfo['QCD_MuEnriched_30to50'] = {"cross-section": 66090000 * 0.0122, "NumberOfProcessedEvents":3745711}
# datasetInfo['QCD_MuEnriched_50to80'] = {"cross-section": 8082000 * 0.0218, "NumberOfProcessedEvents":3896725}
# datasetInfo['QCD_MuEnriched_80to120'] = {"cross-section": 1024000 * 0.0395, "NumberOfProcessedEvents":3470979}

