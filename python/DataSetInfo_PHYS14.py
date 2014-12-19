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
#if using the designated subset:
datasetInfo['TTJet'] = {"cross-section": 831.76, "NumberOfProcessedEvents":25446993}
datasetInfo['TTJets_Madgraph_PU30bx50'] = {"cross-section": 831.76, "NumberOfProcessedEvents":25484471}
datasetInfo['TTJets_Madgraph_PU4bx50'] = {"cross-section": 831.76, "NumberOfProcessedEvents":24547557}
datasetInfo['TT_Pythia8'] = {"cross-section": 831.76, "NumberOfProcessedEvents":2991609}
datasetInfo['TT_Pythia8_PU30bx50'] = {"cross-section": 831.76, "NumberOfProcessedEvents":2991609}
datasetInfo['TT_Pythia8_PU40bx25'] = {"cross-section": 831.76, "NumberOfProcessedEvents":2991609}

# Single top
# Cross sections from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SingleTopSigma
datasetInfo['TToLeptons_s'] = {"cross-section": 7.20, "NumberOfProcessedEvents":500000}
datasetInfo['TToLeptons_t'] = {"cross-section": 136.05, "NumberOfProcessedEvents":3991000}
datasetInfo['T_tw'] = {"cross-section": 35.6, "NumberOfProcessedEvents":986100}

datasetInfo['TBarToLeptons_s'] = {"cross-section": 4.16, "NumberOfProcessedEvents":250000}
datasetInfo['TBarToLeptons_t'] = {"cross-section": 80.97, "NumberOfProcessedEvents":1999800}
datasetInfo['Tbar_tw'] = {"cross-section": 35.6, "NumberOfProcessedEvents":971800}

# V+Jets
# Cross sections from https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat13TeV
datasetInfo['DYJetsToLL'] = {"cross-section": 9616.8, "NumberOfProcessedEvents":2829164}
datasetInfo['WJets'] = {"cross-section": 61526.7, "NumberOfProcessedEvents":2829164}


#
# CROSS SECTIONS FOR QCD CRAP FROM 8 TEV
#

# G+Jets
# Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=GJets_HT*13*&page=0&shown=262173
datasetInfo['GJets_100to200'] = {"cross-section": 1534, "NumberOfProcessedEvents":4734234}
datasetInfo['GJets_200to400'] = {"cross-section": 489.9, "NumberOfProcessedEvents":4533420}
datasetInfo['GJets_400to600'] = {"cross-section": 62.05, "NumberOfProcessedEvents":4560801}
datasetInfo['GJets_600toInf'] = {"cross-section": 20.87, "NumberOfProcessedEvents":4341179}

# QCD BCToE
# Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=QCD_*_bcToE_Tune4C_13TeV_pythia8&page=0&shown=134479933
datasetInfo['QCD_bcToE_20to30'] = {"cross-section": 675900000 * 0.00075, "NumberOfProcessedEvents":999926}
datasetInfo['QCD_bcToE_30to80'] = {"cross-section": 185900000 * 0.00272, "NumberOfProcessedEvents":1852166}
datasetInfo['QCD_bcToE_80to170'] = {"cross-section": 3495000 * 0.01225, "NumberOfProcessedEvents":1000671}
datasetInfo['QCD_bcToE_170toInf'] = {"cross-section": 128500 * 0.0406, "NumberOfProcessedEvents":1000221}

# QCD EM-enriched
# Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=QCD_*EMEnriched*13*&page=0&shown=262173
datasetInfo['QCD_EMEnriched_20to30'] = {"cross-section": 677300000 * 0.007, "NumberOfProcessedEvents":1987127}
datasetInfo['QCD_EMEnriched_30to80'] = {"cross-section": 185900000 * 0.056, "NumberOfProcessedEvents":2000838}
datasetInfo['QCD_EMEnriched_80to170'] = {"cross-section": 3529000 * 0.158, "NumberOfProcessedEvents":1959507}
datasetInfo['QCD_EMEnriched_170to250'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_EMEnriched_250to350'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_EMEnriched_350'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

# QCD Mu-enriched
# Cross sections from https://cms-pdmv.cern.ch/mcm/requests?dataset_name=QCD_*MuEnriched*13*&page=0&shown=262173
datasetInfo['QCD_MuEnriched_20toInf'] = {"cross-section": 866600000 * 0.00044, "NumberOfProcessedEvents":4777926}
datasetInfo['QCD_MuEnriched_30to50'] = {"cross-section": 66090000 * 0.0122, "NumberOfProcessedEvents":3745711}
datasetInfo['QCD_MuEnriched_50to80'] = {"cross-section": 8082000 * 0.0218, "NumberOfProcessedEvents":3896725}
datasetInfo['QCD_MuEnriched_80to120'] = {"cross-section": 1024000 * 0.0395, "NumberOfProcessedEvents":3470979}

# FROM 8 TEV SO CODE ACTUALLY RUNS

#Data and not used samples
datasetInfo['ElectronHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['MuHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleElectron'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleMu'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

#cross-sections from ??
#cross-sections from PREP (search for: W*Jet*8TeV*madgraph)
datasetInfo['W1Jet'] = {"cross-section":  0, "NumberOfProcessedEvents":0 }
datasetInfo['W2Jets'] = {"cross-section": 0, "NumberOfProcessedEvents":0 }
datasetInfo['W3Jets'] = {"cross-section": 0, "NumberOfProcessedEvents":0 }
datasetInfo['W4Jets'] = {"cross-section": 0, "NumberOfProcessedEvents":0 }

#cross-sections from ??
datasetInfo['DY1JetsToLL'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['DY2JetsToLL'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['DY3JetsToLL'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['DY4JetsToLL'] = {"cross-section": 0, "NumberOfProcessedEvents":0}







#cross-sections from PREP (search for: QCD*Mu*8TeV*pythia6)
datasetInfo['QCD_Pt_20_MuEnrichedPt_15'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-15to20_MuEnrichedPt5'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-20to30_MuEnrichedPt5'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-30to50_MuEnrichedPt5'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-50to80_MuEnrichedPt5'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-80to120_MuEnrichedPt5'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-120to170_MuEnrichedPt5'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-170to300_MuEnrichedPt5'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-300to470_MuEnrichedPt5'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-470to600_MuEnrichedPt5'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-600to800_MuEnrichedPt5'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-800to1000_MuEnrichedPt5'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-1000_MuEnrichedPt5'] = {"cross-section": 0, "NumberOfProcessedEvents":0}



#systematic samples (same cross-sections respective central samples)
datasetInfo['TTJets-matchingdown'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['TTJets-matchingup'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['TTJets-scaledown'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['TTJets-scaleup'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

datasetInfo['WJets-matchingdown'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['WJets-matchingup'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['WJets-scaledown'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['WJets-scaleup'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

datasetInfo['ZJets-matchingdown'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['ZJets-matchingup'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['ZJets-scaledown'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['ZJets-scaleup'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

#TTJets different generators (succeeded by BLT)
#other generators (cross-sections same as central samples)
datasetInfo['TTJets-mcatnlo'] = {"cross-section": 0, "NumberOfProcessedEvents":0} #not used
datasetInfo['TTJets-powheg'] = {"cross-section": 0, "NumberOfProcessedEvents":0} #not used


datasetInfo['WWtoAnything'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['WZtoAnything'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['ZZtoAnything'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
#Ttbar + Z/W from http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2011_288_v14.pdf
datasetInfo['TTbarZIncl'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['TTbarInclWIncl'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
#heavy flavour sample
datasetInfo['VqqJets'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

#Z' samples
datasetInfo['Zprime_M500GeV_W5GeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M500GeV_W50GeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M750GeV_W7500MeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M1000GeV_W10GeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M1000GeV_W100GeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M1250GeV_W12500MeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M1500GeV_W15GeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M1500GeV_W150GeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M2000GeV_W20GeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M2000GeV_W200GeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M3000GeV_W30GeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M3000GeV_W300GeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M4000GeV_W40GeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M4000GeV_W400GeV'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

#top samples with different top mass
datasetInfo['TTJets161'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['TTJets163'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['TTJets166'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['TTJets169'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['TTJets175'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['TTJets178'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['TTJets181'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['TTJets184'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

datasetInfo['QCD_Pt-20_MuEnrichedPt-15'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

datasetInfo['QCD_Pt-20to30_BCtoE'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-30to80_BCtoE'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-80to170_BCtoE'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

datasetInfo['QCD_Pt-20to30_EMEnriched'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-30to80_EMEnriched'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-80to170_EMEnriched'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-170to250_EMEnriched'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-250to350_EMEnriched'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-350_EMEnriched'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
#Higgs
datasetInfo['TTH_Inclusive_M-125'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

