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
#datasetInfo['TTJet'] = {"cross-section": 225.197, "NumberOfProcessedEvents":59414270}
#if using the designated subset:
datasetInfo['TTJet'] = {"cross-section": 225.197, "NumberOfProcessedEvents":6920475}
datasetInfo['WJetsToLNu'] = {"cross-section": 36257.2, "NumberOfProcessedEvents":57708550}
datasetInfo['W1Jet'] = {"cross-section": 5400.0, "NumberOfProcessedEvents":23140779 }
datasetInfo['W2Jets'] = {"cross-section": 1750.0, "NumberOfProcessedEvents":34041404}
datasetInfo['W3Jets'] = {"cross-section": 519.0, "NumberOfProcessedEvents":15536443}
datasetInfo['W4Jets'] = {"cross-section": 214.0, "NumberOfProcessedEvents":13370904}

datasetInfo['DYJetsToLL'] = {"cross-section": 3503.71, "NumberOfProcessedEvents":30457954}
datasetInfo['DY1JetsToLL'] = {"cross-section": 561.0, "NumberOfProcessedEvents":24042904}
datasetInfo['DY2JetsToLL'] = {"cross-section": 181.0, "NumberOfProcessedEvents":21835749}
datasetInfo['DY3JetsToLL'] = {"cross-section": 51.1, "NumberOfProcessedEvents":11010628}
datasetInfo['DY4JetsToLL'] = {"cross-section": 23.04, "NumberOfProcessedEvents":6391785}

datasetInfo['GJets_HT-200To400'] = {"cross-section": 960.5, "NumberOfProcessedEvents":10484464}
datasetInfo['GJets_HT-400ToInf'] = {"cross-section": 107.5, "NumberOfProcessedEvents":1606586}

datasetInfo['QCD_Pt_20_30_BCtoE'] = {"cross-section": 2.886e8 * 5.8e-4, "NumberOfProcessedEvents":1740223}
datasetInfo['QCD_Pt_30_80_BCtoE'] = {"cross-section": 7.424e7 * 0.00225, "NumberOfProcessedEvents":2048147}
datasetInfo['QCD_Pt_80_170_BCtoE'] = {"cross-section": 1191000.0 * 0.0109, "NumberOfProcessedEvents":1937602}
datasetInfo['QCD_Pt_170_250_BCtoE'] = {"cross-section": 30980.0 * 0.0204, "NumberOfProcessedEvents":1945895}
datasetInfo['QCD_Pt_250_350_BCtoE'] = {"cross-section": 4250.0 * 0.0243, "NumberOfProcessedEvents":2019922}
datasetInfo['QCD_Pt_350_BCtoE'] = {"cross-section": 811.0 * 0.0295, "NumberOfProcessedEvents":1934813}

datasetInfo['QCD_Pt_20_30_EMEnriched'] = {"cross-section": 2.886e8 * 0.0101, "NumberOfProcessedEvents":35005597}
datasetInfo['QCD_Pt_30_80_EMEnriched'] = {"cross-section": 7.433e7 * 0.0621, "NumberOfProcessedEvents":28753832}
datasetInfo['QCD_Pt_80_170_EMEnriched'] = {"cross-section": 1191000.0 * 0.1539, "NumberOfProcessedEvents":34436039}
datasetInfo['QCD_Pt_170_250_EMEnriched'] = {"cross-section": 30990.0 * 0.148, "NumberOfProcessedEvents":31581299}
datasetInfo['QCD_Pt_250_350_EMEnriched'] = {"cross-section": 4250.0 * 0.131, "NumberOfProcessedEvents":34502346}
datasetInfo['QCD_Pt_350_EMEnriched'] = {"cross-section": 810.0 * 0.11, "NumberOfProcessedEvents":33478691}

datasetInfo['QCD_Pt_20_MuEnrichedPt_15'] = {"cross-section": 3.64e8 * 3.7e-4, "NumberOfProcessedEvents":21484326}
datasetInfo['QCD_Pt-15to20_MuEnrichedPt5'] = {"cross-section": 7.022e8 * 0.0039, "NumberOfProcessedEvents":1722678}
datasetInfo['QCD_Pt-20to30_MuEnrichedPt5'] = {"cross-section": 2.87e8 * 0.0065, "NumberOfProcessedEvents":8486893}
datasetInfo['QCD_Pt-30to50_MuEnrichedPt5'] = {"cross-section": 6.609e7 * 0.0122, "NumberOfProcessedEvents":8928999}
datasetInfo['QCD_Pt-50to80_MuEnrichedPt5'] = {"cross-section": 8082000.0 * 0.0218, "NumberOfProcessedEvents":9981739}
datasetInfo['QCD_Pt-80to120_MuEnrichedPt5'] = {"cross-section": 1024000.0 * 0.0395, "NumberOfProcessedEvents":9030624}
datasetInfo['QCD_Pt-120to170_MuEnrichedPt5'] = {"cross-section": 157800.0 * 0.0473, "NumberOfProcessedEvents":8500505}
datasetInfo['QCD_Pt-170to300_MuEnrichedPt5'] = {"cross-section": 34020.0 * 0.0676, "NumberOfProcessedEvents":7662483}
datasetInfo['QCD_Pt-300to470_MuEnrichedPt5'] = {"cross-section": 1757.0 * 0.0864, "NumberOfProcessedEvents":7797481}
datasetInfo['QCD_Pt-470to600_MuEnrichedPt5'] = {"cross-section": 115.2 * 0.1024, "NumberOfProcessedEvents":3749539}
datasetInfo['QCD_Pt-600to800_MuEnrichedPt5'] = {"cross-section": 27.01 * 0.0996, "NumberOfProcessedEvents":0}
datasetInfo['QCD_Pt-800to1000_MuEnrichedPt5'] = {"cross-section": 3.57 * 0.1033, "NumberOfProcessedEvents":4047142}
datasetInfo['QCD_Pt-1000_MuEnrichedPt5'] = {"cross-section": 0.774 * 0.1097, "NumberOfProcessedEvents":3807263}

datasetInfo['T_s-channel'] = {"cross-section": 3.89394, "NumberOfProcessedEvents":259911}
datasetInfo['T_t-channel'] = {"cross-section": 55.531, "NumberOfProcessedEvents":3757707}
datasetInfo['T_tW-channel'] = {"cross-section": 11.1773, "NumberOfProcessedEvents":497395}

datasetInfo['Tbar_s-channel'] = {"cross-section": 1.75776, "NumberOfProcessedEvents":139948}
datasetInfo['Tbar_t-channel'] = {"cross-section": 30.0042, "NumberOfProcessedEvents":1934817}
datasetInfo['Tbar_tW-channel'] = {"cross-section": 11.1773, "NumberOfProcessedEvents":493239}

#systematic samples
datasetInfo['TTJets-matchingdown'] = {"cross-section": 130., "NumberOfProcessedEvents":5458456}
datasetInfo['TTJets-matchingup'] = {"cross-section": 138., "NumberOfProcessedEvents":5412642}
datasetInfo['TTJets-scaledown'] = {"cross-section": 228.0, "NumberOfProcessedEvents":5384596}
datasetInfo['TTJets-scaleup'] = {"cross-section": 97.7, "NumberOfProcessedEvents":5007277}

datasetInfo['WJets-matchingdown'] = {"cross-section": 29690.0, "NumberOfProcessedEvents":21364189}
datasetInfo['WJets-matchingup'] = {"cross-section": 30290.0, "NumberOfProcessedEvents":20126095}
datasetInfo['WJets-scaledown'] = {"cross-section": 33300.0, "NumberOfProcessedEvents":20760315}
datasetInfo['WJets-scaleup'] = {"cross-section": 32000.0, "NumberOfProcessedEvents":19402929}

datasetInfo['ZJets-matchingdown'] = {"cross-section": 2888.0, "NumberOfProcessedEvents":2091131}
datasetInfo['ZJets-matchingup'] = {"cross-section": 2915.0, "NumberOfProcessedEvents":1985440}
datasetInfo['ZJets-scaledown'] = {"cross-section": 3312.0, "NumberOfProcessedEvents":1934768}
datasetInfo['ZJets-scaleup'] = {"cross-section": 2954.0, "NumberOfProcessedEvents":2126758}

#Data and not used samples
datasetInfo['ElectronHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['MuHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleElectron'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleMu'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

#Old QCD samples (kept to avoid dictionary errors)
datasetInfo['GJets_HT-40To100'] = {"cross-section": 23620., "NumberOfProcessedEvents":12659371}
datasetInfo['GJets_HT-100To200'] = {"cross-section": 3476., "NumberOfProcessedEvents":1536287}
datasetInfo['GJets_HT-200ToInf'] = {"cross-section": 485., "NumberOfProcessedEvents":9377170}

datasetInfo['WWtoAnything'] = {"cross-section": 57.1097, "NumberOfProcessedEvents":4191740}
datasetInfo['WZtoAnything'] = {"cross-section": 32.3161, "NumberOfProcessedEvents":4136765}
datasetInfo['ZZtoAnything'] = {"cross-section": 8.25561, "NumberOfProcessedEvents":2408805}
#Ttbar + Z/W from http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2011_288_v14.pdf
datasetInfo['TTbarZIncl'] = {"cross-section": 0.14, "NumberOfProcessedEvents":196277}
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

datasetInfo['QCD_Pt-20_MuEnrichedPt-15'] = {"cross-section": 3.64e8 * 3.7e-4, "NumberOfProcessedEvents":25080199}

datasetInfo['QCD_Pt-20to30_BCtoE'] = {"cross-section": 0.2355e9 * 0.00046, "NumberOfProcessedEvents":2081560}
datasetInfo['QCD_Pt-30to80_BCtoE'] = {"cross-section": 0.0593e9 * 0.00234, "NumberOfProcessedEvents":2013126}
datasetInfo['QCD_Pt-80to170_BCtoE'] = {"cross-section": 0.906e6 * 0.0104, "NumberOfProcessedEvents":1044013}

datasetInfo['QCD_Pt-20to30_EMEnriched'] = {"cross-section": 0.2355e9 * 0.0073, "NumberOfProcessedEvents":34607077}
datasetInfo['QCD_Pt-30to80_EMEnriched'] = {"cross-section": 0.0593e9 * 0.059, "NumberOfProcessedEvents":70376046}
datasetInfo['QCD_Pt-80to170_EMEnriched'] = {"cross-section": 0.906e6 * 0.148, "NumberOfProcessedEvents":8150672}
