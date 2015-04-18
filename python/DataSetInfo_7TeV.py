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
#https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO
datasetInfo['TTJet'] = {"cross-section": 177.31, "NumberOfProcessedEvents":17100187}
#cross-section from twiki (https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections)
datasetInfo['WJetsToLNu'] = {"cross-section": 31314, "NumberOfProcessedEvents":76726296}
#cross-sections from PREP (search for: W*Jet*7TeV*madgraph-tauola)
datasetInfo['W1Jet'] = {"cross-section": 4480.0, "NumberOfProcessedEvents":70430949}
datasetInfo['W2Jets'] = {"cross-section": 1435.0, "NumberOfProcessedEvents":25069566}
datasetInfo['W3Jets'] = {"cross-section": 304.2, "NumberOfProcessedEvents":6291772}
datasetInfo['W4Jets'] = {"cross-section": 172.6, "NumberOfProcessedEvents":13240209}

#cross-sections from twiki (https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections)
datasetInfo['DYJetsToLL'] = {"cross-section": 3048., "NumberOfProcessedEvents":32846945}

#cross-sections from PREP (search for: GJets*7TeV*)
datasetInfo['GJets_HT-40To100'] = {"cross-section": 25690.0, "NumberOfProcessedEvents":9882860}
datasetInfo['GJets_HT-100To200'] = {"cross-section": 5213.0, "NumberOfProcessedEvents":1514347}
datasetInfo['GJets_HT-200ToInf'] = {"cross-section": 798.3, "NumberOfProcessedEvents":9275592}

#cross-sections from PREP (search for: QCD*BCtoE*7TeV*)
datasetInfo['QCD_Pt-20to30_BCtoE'] = {"cross-section": 2.361e8 * 5.9e-4, "NumberOfProcessedEvents":1927944}
datasetInfo['QCD_Pt-30to80_BCtoE'] = {"cross-section": 5.944e7 * 0.00242, "NumberOfProcessedEvents":1946505}
datasetInfo['QCD_Pt-80to170_BCtoE'] = {"cross-section": 898200.0 * 0.0105, "NumberOfProcessedEvents":1002427}

#cross-sections from PREP (search for: QCD*EMEnriched*7TeV*)
datasetInfo['QCD_Pt-20to30_EMEnriched'] = {"cross-section": 2.361e8 * 0.0106, "NumberOfProcessedEvents":32976415}
datasetInfo['QCD_Pt-30to80_EMEnriched'] = {"cross-section": 5.944e7 * 0.061, "NumberOfProcessedEvents":71775065}
datasetInfo['QCD_Pt-80to170_EMEnriched'] = {"cross-section": 898200 * 0.159, "NumberOfProcessedEvents":7650319}
datasetInfo['QCD_Pt-170to250_EMEnriched'] = {"cross-section": 22140 * 0.1474, "NumberOfProcessedEvents":2968842}
datasetInfo['QCD_Pt-250to350_EMEnriched'] = {"cross-section": 2900 * 0.1269, "NumberOfProcessedEvents":2952960}
datasetInfo['QCD_Pt-350_EMEnriched'] = {"cross-section": 520 * 0.1058, "NumberOfProcessedEvents":2957326}

#cross-sections from PREP (search for: QCD*MuPt5Enriched*7TeV*)
datasetInfo['QCD_Pt-15to20_MuEnrichedPt5'] = {"cross-section": 5.792e8 * 0.00288, "NumberOfProcessedEvents":1901684}
datasetInfo['QCD_Pt-20to30_MuEnrichedPt5'] = {"cross-section": 2.363e8 * 0.00568, "NumberOfProcessedEvents":10173300}
datasetInfo['QCD_Pt-30to50_MuEnrichedPt5'] = {"cross-section": 5.307e7 * 0.01124, "NumberOfProcessedEvents":11610111}
datasetInfo['QCD_Pt-50to80_MuEnrichedPt5'] = {"cross-section": 6351000.0 * 0.02205, "NumberOfProcessedEvents":9870031}
datasetInfo['QCD_Pt-80to120_MuEnrichedPt5'] = {"cross-section": 785100.0 * 0.03636, "NumberOfProcessedEvents":9769136}
#cross-sections taken from 8TeV values and multiplied by ratio of 7TeV inclusive sample (QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6, 2.966e8 * 2.855e-4) 
#to 8TeV inclusive sample (QCD_Pt_20_MuEnrichedPt_15_TuneZ2star_8TeV_pythia6, 3.64e8 * 3.7e-4) 
datasetInfo['QCD_Pt-120to170_MuEnrichedPt5'] = {"cross-section": 4692.91, "NumberOfProcessedEvents":7818474} 
datasetInfo['QCD_Pt-170to300_MuEnrichedPt5'] = {"cross-section": 1445.96, "NumberOfProcessedEvents":8116409}
datasetInfo['QCD_Pt-300to470_MuEnrichedPt5'] = {"cross-section": 95.4464, "NumberOfProcessedEvents":7870002}
datasetInfo['QCD_Pt-470to600_MuEnrichedPt5'] = {"cross-section": 7.41697, "NumberOfProcessedEvents":3812529}
datasetInfo['QCD_Pt-600to800_MuEnrichedPt5'] = {"cross-section": 1.69145, "NumberOfProcessedEvents":4149911}
datasetInfo['QCD_Pt-800to1000_MuEnrichedPt5'] = {"cross-section": 0.231869, "NumberOfProcessedEvents":4036867}
datasetInfo['QCD_Pt-1000_MuEnrichedPt5'] = {"cross-section": 0.053385, "NumberOfProcessedEvents":4133897}

#cross-sections from twiki (https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections)
datasetInfo['T_s-channel'] = {"cross-section": 2.72, "NumberOfProcessedEvents":229786}
datasetInfo['T_t-channel'] = {"cross-section": 42.6, "NumberOfProcessedEvents":3249530}
datasetInfo['T_tW-channel'] = {"cross-section": 5.3, "NumberOfProcessedEvents":744859}

datasetInfo['Tbar_s-channel'] = {"cross-section": 1.49, "NumberOfProcessedEvents":138187}
datasetInfo['Tbar_t-channel'] = {"cross-section": 22.0, "NumberOfProcessedEvents":1813615}
datasetInfo['Tbar_tW-channel'] = {"cross-section": 5.3, "NumberOfProcessedEvents":801626}

#systematic samples (same cross-sections respective central samples)
datasetInfo['TTJets-matchingdown'] = {"cross-section": 172, "NumberOfProcessedEvents":1577671} #to update once sample is available
datasetInfo['TTJets-matchingup'] = {"cross-section": 172, "NumberOfProcessedEvents":5306710} #to update once sample is available
datasetInfo['TTJets-scaledown'] = {"cross-section": 172, "NumberOfProcessedEvents":9426377}
datasetInfo['TTJets-scaleup'] = {"cross-section": 172, "NumberOfProcessedEvents":10095984}

datasetInfo['WJets-matchingdown'] = {"cross-section": 31314, "NumberOfProcessedEvents":9954470} #to update once sample is available
datasetInfo['WJets-matchingup'] = {"cross-section": 31314, "NumberOfProcessedEvents":10458997} #to update once sample is available
datasetInfo['WJets-scaledown'] = {"cross-section": 31314, "NumberOfProcessedEvents":10039083} #to update once sample is available
datasetInfo['WJets-scaleup'] = {"cross-section": 31314, "NumberOfProcessedEvents":9782022} #to update once sample is available

datasetInfo['ZJets-matchingdown'] = {"cross-section": 3048., "NumberOfProcessedEvents":1614734} #to update once sample is available
datasetInfo['ZJets-matchingup'] = {"cross-section": 3048., "NumberOfProcessedEvents":1596487} #to update once sample is available
datasetInfo['ZJets-scaledown'] = {"cross-section": 3048., "NumberOfProcessedEvents":1658757} #to update once sample is available
datasetInfo['ZJets-scaleup'] = {"cross-section": 3048., "NumberOfProcessedEvents":1592684} #to update once sample is available

#Data and not used samples
datasetInfo['ElectronHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['MuHad'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleElectron'] = {"cross-section": 0, "NumberOfProcessedEvents":0}
datasetInfo['SingleMu'] = {"cross-section": 0, "NumberOfProcessedEvents":0}

#TTJets different generators (succeeded by BLT)
#(cross-sections same as central samples)
datasetInfo['TTJets-mcatnlo'] = {"cross-section": 172, "NumberOfProcessedEvents":20713658} # not used
datasetInfo['TTJets-powheg'] = {"cross-section": 172, "NumberOfProcessedEvents":16370364} # not used

#Older/newer samples (kept to avoid dictionary errors)

datasetInfo['DY1JetsToLL'] = {"cross-section": 561.0, "NumberOfProcessedEvents":24045248} #not available in Summer11Leg
datasetInfo['DY2JetsToLL'] = {"cross-section": 181.0, "NumberOfProcessedEvents":21852156} #not available in Summer11Leg
datasetInfo['DY3JetsToLL'] = {"cross-section": 51.1, "NumberOfProcessedEvents":11015445} #not available in Summer11Leg
datasetInfo['DY4JetsToLL'] = {"cross-section": 23.04, "NumberOfProcessedEvents":6402827} #not available in Summer11Leg

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
datasetInfo['QCD_Pt-20_MuEnrichedPt-15'] = {"cross-section": 3.64e8 * 3.7e-4, "NumberOfProcessedEvents":25080199}

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

#Higgs
datasetInfo['TTH_Inclusive_M-125'] = {"cross-section": 0.1293, "NumberOfProcessedEvents":993656}
