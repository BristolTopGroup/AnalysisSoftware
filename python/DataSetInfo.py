####################################################################################################
#MCInfo V6
####################################################################################################
#cross-section: the cross-section of the MC process in pb-1, 
#twiki: https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
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

#TODO: read this variable in the config
nTuple_version = 6

datasetInfo = {}
datasetInfo['TTJet'] = {"cross-section": 157.5, "NumberOfProcessedEvents":59414270}
#if using the designated subset:
#datasetInfo['TTJet'] = {"cross-section": 157.5, "NumberOfProcessedEvents":3636129}
datasetInfo['WJetsToLNu'] = {"cross-section": 31314., "NumberOfProcessedEvents":75476429}
datasetInfo['DYJetsToLL'] = {"cross-section": 3048., "NumberOfProcessedEvents":32112858}

datasetInfo['GJets_TuneD6T_HT-40To100'] = {"cross-section": 23620., "NumberOfProcessedEvents":12659371}
datasetInfo['GJets_TuneD6T_HT-100To200'] = {"cross-section": 3476., "NumberOfProcessedEvents":1536287}
datasetInfo['GJets_TuneD6T_HT-200'] = {"cross-section": 485., "NumberOfProcessedEvents":9377170}

datasetInfo['QCD_Pt-20to30_BCtoE'] = {"cross-section": 0.2355e9 * 0.00046, "NumberOfProcessedEvents":2081560}
datasetInfo['QCD_Pt-30to80_BCtoE'] = {"cross-section": 0.0593e9 * 0.00234, "NumberOfProcessedEvents":2013126}
datasetInfo['QCD_Pt-80to170_BCtoE'] = {"cross-section": 0.906e6 * 0.0104, "NumberOfProcessedEvents":1044013}

datasetInfo['QCD_Pt-20to30_EMEnriched'] = {"cross-section": 0.2355e9 * 0.0073, "NumberOfProcessedEvents":34607077}
datasetInfo['QCD_Pt-30to80_EMEnriched'] = {"cross-section": 0.0593e9 * 0.059, "NumberOfProcessedEvents":70376046}
datasetInfo['QCD_Pt-80to170_EMEnriched'] = {"cross-section": 0.906e6 * 0.148, "NumberOfProcessedEvents":8150672}

datasetInfo['QCD_Pt-20_MuEnrichedPt-15'] = {"cross-section": 84679.3, "NumberOfProcessedEvents":25080241}

datasetInfo['T_TuneZ2_s-channel'] = {"cross-section": 2.72, "NumberOfProcessedEvents":259971}
datasetInfo['T_TuneZ2_t-channel'] = {"cross-section": 42.6, "NumberOfProcessedEvents":3340750}
datasetInfo['T_TuneZ2_tW-channel'] = {"cross-section": 5.3, "NumberOfProcessedEvents":814390}

datasetInfo['Tbar_TuneZ2_s-channel'] = {"cross-section": 1.49, "NumberOfProcessedEvents":137980}
datasetInfo['Tbar_TuneZ2_t-channel'] = {"cross-section": 22.0, "NumberOfProcessedEvents":1944826}
datasetInfo['Tbar_TuneZ2_tW-channel'] = {"cross-section": 5.3, "NumberOfProcessedEvents":724439}

datasetInfo['WWtoAnything'] = {"cross-section": 43., "NumberOfProcessedEvents":4191740}
datasetInfo['WZtoAnything'] = {"cross-section": 18.2, "NumberOfProcessedEvents":4136765}
datasetInfo['ZZtoAnything'] = {"cross-section": 5.9, "NumberOfProcessedEvents":2408805}
#Ttbar + Z/W from http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2011_288_v14.pdf
datasetInfo['TTJetsZIncl'] = {"cross-section": 0.14, "NumberOfProcessedEvents":196277}
datasetInfo['TTbarInclWIncl'] = {"cross-section": 0.16, "NumberOfProcessedEvents":349038}
#heavy flavour sample
datasetInfo['VqqJets'] = {"cross-section": 35.3, "NumberOfProcessedEvents":720613}

#Z' samples
datasetInfo['Zprime_M500GeV_W5GeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M500GeV_W50GeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M750GeV_W7500MeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M1000GeV_W10GeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M1000GeV_W100GeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M1250GeV_W12500MeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M1500GeV_W15GeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M1500GeV_W150GeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M2000GeV_W20GeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M2000GeV_W200GeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M3000GeV_W30GeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M3000GeV_W300GeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M4000GeV_W40GeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}
datasetInfo['Zprime_M4000GeV_W400GeV'] = {"cross-section": 50, "NumberOfProcessedEvents":0}

#top samples with different top mass
datasetInfo['TTJets161'] = {"cross-section": 157.5, "NumberOfProcessedEvents":1620072}
datasetInfo['TTJets163'] = {"cross-section": 157.5, "NumberOfProcessedEvents":1633197}
datasetInfo['TTJets166'] = {"cross-section": 157.5, "NumberOfProcessedEvents":1669034}
datasetInfo['TTJets169'] = {"cross-section": 157.5, "NumberOfProcessedEvents":1606570}
datasetInfo['TTJets175'] = {"cross-section": 157.5, "NumberOfProcessedEvents":1538301}
datasetInfo['TTJets178'] = {"cross-section": 157.5, "NumberOfProcessedEvents":1648519}
datasetInfo['TTJets181'] = {"cross-section": 157.5, "NumberOfProcessedEvents":1665350}
datasetInfo['TTJets184'] = {"cross-section": 157.5, "NumberOfProcessedEvents":1671859}

#systematic samples
datasetInfo['TTJets-matchingdown'] = {"cross-section": 764., "NumberOfProcessedEvents":1065323}
datasetInfo['TTJets-matchingup'] = {"cross-section": 172., "NumberOfProcessedEvents":1062792}
datasetInfo['TTJets-scaledown'] = {"cross-section": 552., "NumberOfProcessedEvents":967055}
datasetInfo['TTJets-scaleup'] = {"cross-section": 200., "NumberOfProcessedEvents":930483}

datasetInfo['WJetsToLNu-matchingdown'] = {"cross-section": 42352, "NumberOfProcessedEvents":9956679}
datasetInfo['WJetsToLNu-matchingup'] = {"cross-section": 11439, "NumberOfProcessedEvents":10461655}
datasetInfo['WJetsToLNu-scaledown'] = {"cross-section": 20137, "NumberOfProcessedEvents":10092532}
datasetInfo['WJetsToLNu-scaleup'] = {"cross-section": 17859, "NumberOfProcessedEvents":9756359}

#Data and not used samples
datasetInfo['ElectronHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['MuHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['TTJetsFall11'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
