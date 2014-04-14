####################################################################################################
#MCInfo V6
####################################################################################################
#cross-section: the cross-section of the MC process in pb-1, 
#twiki: https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
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
datasetInfo['TTJet'] = {"cross-section": 157.5, "NumberOfProcessedEvents":7458920}
datasetInfo['WJetsToLNu'] = {"cross-section": 31314., "NumberOfProcessedEvents":57708550} #not needed for 7TeV
datasetInfo['W1Jet'] = {"cross-section": 4480., "NumberOfProcessedEvents":75710577 }
datasetInfo['W2Jets'] = {"cross-section": 1674., "NumberOfProcessedEvents":25397957}
datasetInfo['W3Jets'] = {"cross-section": 484.7, "NumberOfProcessedEvents":7404688}
datasetInfo['W4Jets'] = {"cross-section": 211.7, "NumberOfProcessedEvents":12921933}

datasetInfo['DYJetsToLL'] = {"cross-section": 3048., "NumberOfProcessedEvents":36257353}

datasetInfo['GJets_HT-40To100'] = {"cross-section": 23620., "NumberOfProcessedEvents":12730169}
datasetInfo['GJets_HT-100To200'] = {"cross-section": 3476., "NumberOfProcessedEvents":1535665}
datasetInfo['GJets_HT-200ToInf'] = {"cross-section": 485., "NumberOfProcessedEvents":9366381}

datasetInfo['QCD_Pt-20to30_BCtoE'] = {"cross-section": 0.2355e9 * 0.00046, "NumberOfProcessedEvents":2081553}
datasetInfo['QCD_Pt-30to80_BCtoE'] = {"cross-section": 0.0593e9 * 0.00234, "NumberOfProcessedEvents":2030029}
datasetInfo['QCD_Pt-80to170_BCtoE'] = {"cross-section": 0.906e6 * 0.0104, "NumberOfProcessedEvents":1082622}

datasetInfo['QCD_Pt-20to30_EMEnriched'] = {"cross-section": 0.2355e9 * 0.0073, "NumberOfProcessedEvents":35725318}
datasetInfo['QCD_Pt-30to80_EMEnriched'] = {"cross-section": 0.0593e9 * 0.059, "NumberOfProcessedEvents":70375877}
datasetInfo['QCD_Pt-80to170_EMEnriched'] = {"cross-section": 0.906e6 * 0.148, "NumberOfProcessedEvents":8150065}

datasetInfo['QCD_Pt-20_MuEnrichedPt-15'] = {"cross-section": 84679.3, "NumberOfProcessedEvents":25079944}

datasetInfo['T_s-channel'] = {"cross-section": 2.72, "NumberOfProcessedEvents":259931}
datasetInfo['T_t-channel'] = {"cross-section": 42.6, "NumberOfProcessedEvents":3899754}
datasetInfo['T_tW-channel'] = {"cross-section": 5.3, "NumberOfProcessedEvents":814059}

datasetInfo['Tbar_s-channel'] = {"cross-section": 1.49, "NumberOfProcessedEvents":137959}
datasetInfo['Tbar_t-channel'] = {"cross-section": 22.0, "NumberOfProcessedEvents":1944620}
datasetInfo['Tbar_tW-channel'] = {"cross-section": 5.3, "NumberOfProcessedEvents":809642}

datasetInfo['WWtoAnything'] = {"cross-section": 43., "NumberOfProcessedEvents":4225341}
datasetInfo['WZtoAnything'] = {"cross-section": 18.2, "NumberOfProcessedEvents":4264676}
datasetInfo['ZZtoAnything'] = {"cross-section": 5.9, "NumberOfProcessedEvents":4190463}

#Ttbar + Z/W from http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2011_288_v14.pdf
datasetInfo['TTbarZIncl'] = {"cross-section": 0.14, "NumberOfProcessedEvents":196072}
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
datasetInfo['TTJets-matchingdown'] = {"cross-section": 764., "NumberOfProcessedEvents":1577671}
datasetInfo['TTJets-matchingup'] = {"cross-section": 172., "NumberOfProcessedEvents":5306710} #to be updated
datasetInfo['TTJets-scaledown'] = {"cross-section": 552., "NumberOfProcessedEvents":5387181} #to be updated
datasetInfo['TTJets-scaleup'] = {"cross-section": 200., "NumberOfProcessedEvents":5009488} #to be updated

datasetInfo['WJets-matchingdown'] = {"cross-section": 42352, "NumberOfProcessedEvents":9954470}
datasetInfo['WJets-matchingup'] = {"cross-section": 11439, "NumberOfProcessedEvents":10458997}
datasetInfo['WJets-scaledown'] = {"cross-section": 20137, "NumberOfProcessedEvents":10039083}
datasetInfo['WJets-scaleup'] = {"cross-section": 17859, "NumberOfProcessedEvents":9782022}

datasetInfo['ZJets-matchingdown'] = {"cross-section": 3048., "NumberOfProcessedEvents":1614734}
datasetInfo['ZJets-matchingup'] = {"cross-section": 3048., "NumberOfProcessedEvents":1596487}
datasetInfo['ZJets-scaledown'] = {"cross-section": 3048., "NumberOfProcessedEvents":1658757}
datasetInfo['ZJets-scaleup'] = {"cross-section": 3048., "NumberOfProcessedEvents":1592684}

#other generators
datasetInfo['TTJets-mcatnlo'] = {"cross-section": 157.5, "NumberOfProcessedEvents":20713658}
datasetInfo['TTJets-powheg'] = {"cross-section": 157.5, "NumberOfProcessedEvents":16370364}

#Data and not used samples
datasetInfo['ElectronHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['MuHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleElectron'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleMu'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

#Older/newer QCD samples (kept to avoid dictionary errors)
datasetInfo['DY1JetsToLL'] = {"cross-section": 561.0, "NumberOfProcessedEvents":24045248}
datasetInfo['DY2JetsToLL'] = {"cross-section": 181.0, "NumberOfProcessedEvents":21852156}
datasetInfo['DY3JetsToLL'] = {"cross-section": 51.1, "NumberOfProcessedEvents":11015445}
datasetInfo['DY4JetsToLL'] = {"cross-section": 23.04, "NumberOfProcessedEvents":6402827}

datasetInfo['GJets_HT-200To400'] = {"cross-section": 960.5, "NumberOfProcessedEvents":10479625}
datasetInfo['GJets_HT-400ToInf'] = {"cross-section": 107.5, "NumberOfProcessedEvents":1611963}

datasetInfo['QCD_Pt_20_30_BCtoE'] = {"cross-section": 2.886e8 * 5.8e-4, "NumberOfProcessedEvents":1740229}
datasetInfo['QCD_Pt_30_80_BCtoE'] = {"cross-section": 7.424e7 * 0.00225, "NumberOfProcessedEvents":2048152}
datasetInfo['QCD_Pt_80_170_BCtoE'] = {"cross-section": 1191000.0 * 0.0109, "NumberOfProcessedEvents":1945525}
datasetInfo['QCD_Pt_170_250_BCtoE'] = {"cross-section": 30980.0 * 0.0204, "NumberOfProcessedEvents":1948112}
datasetInfo['QCD_Pt_250_350_BCtoE'] = {"cross-section": 4250.0 * 0.0243, "NumberOfProcessedEvents":2026521}
datasetInfo['QCD_Pt_350_BCtoE'] = {"cross-section": 811.0 * 0.0295, "NumberOfProcessedEvents":1948532}

datasetInfo['QCD_Pt_20_30_EMEnriched'] = {"cross-section": 2.886e8 * 0.0101, "NumberOfProcessedEvents":34339883}
datasetInfo['QCD_Pt_30_80_EMEnriched'] = {"cross-section": 7.433e7 * 0.0621, "NumberOfProcessedEvents":32537408}
datasetInfo['QCD_Pt_80_170_EMEnriched'] = {"cross-section": 1191000.0 * 0.1539, "NumberOfProcessedEvents":34542763}
datasetInfo['QCD_Pt_170_250_EMEnriched'] = {"cross-section": 30990.0 * 0.148, "NumberOfProcessedEvents":22862259}
datasetInfo['QCD_Pt_250_350_EMEnriched'] = {"cross-section": 4250.0 * 0.131, "NumberOfProcessedEvents":32505856}
datasetInfo['QCD_Pt_350_EMEnriched'] = {"cross-section": 810.0 * 0.11, "NumberOfProcessedEvents":33981105}

datasetInfo['QCD_Pt_20_MuEnrichedPt_15'] = {"cross-section": 3.64e8 * 3.7e-4, "NumberOfProcessedEvents":21484326}
datasetInfo['QCD_Pt-15to20_MuEnrichedPt5'] = {"cross-section": 7.022e8 * 0.0039, "NumberOfProcessedEvents":1722681}
datasetInfo['QCD_Pt-20to30_MuEnrichedPt5'] = {"cross-section": 2.87e8 * 0.0065, "NumberOfProcessedEvents":8486904}
datasetInfo['QCD_Pt-30to50_MuEnrichedPt5'] = {"cross-section": 6.609e7 * 0.0122, "NumberOfProcessedEvents":9560265}
datasetInfo['QCD_Pt-50to80_MuEnrichedPt5'] = {"cross-section": 8082000.0 * 0.0218, "NumberOfProcessedEvents":10365230}
datasetInfo['QCD_Pt-80to120_MuEnrichedPt5'] = {"cross-section": 1024000.0 * 0.0395, "NumberOfProcessedEvents":9238642}
datasetInfo['QCD_Pt-120to170_MuEnrichedPt5'] = {"cross-section": 157800.0 * 0.0473, "NumberOfProcessedEvents":8501935}
datasetInfo['QCD_Pt-170to300_MuEnrichedPt5'] = {"cross-section": 34020.0 * 0.0676, "NumberOfProcessedEvents":7669947}
datasetInfo['QCD_Pt-300to470_MuEnrichedPt5'] = {"cross-section": 1757.0 * 0.0864, "NumberOfProcessedEvents":7832261}
datasetInfo['QCD_Pt-470to600_MuEnrichedPt5'] = {"cross-section": 115.2 * 0.1024, "NumberOfProcessedEvents":3783069}
datasetInfo['QCD_Pt-600to800_MuEnrichedPt5'] = {"cross-section": 27.01 * 0.0996, "NumberOfProcessedEvents":4119000}
datasetInfo['QCD_Pt-800to1000_MuEnrichedPt5'] = {"cross-section": 3.57 * 0.1033, "NumberOfProcessedEvents":4107853}
datasetInfo['QCD_Pt-1000_MuEnrichedPt5'] = {"cross-section": 0.774 * 0.1097, "NumberOfProcessedEvents":3873970}

#Higgs
datasetInfo['TTH_Inclusive_M-125'] = {"cross-section": 0.1293, "NumberOfProcessedEvents":993656}