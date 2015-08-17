mc_path_7TeV = '/hdfs/TopQuarkGroup/mc/7TeV/v11/LeptonPlus3JetsSkim/'
data_path_7TeV = '/hdfs/TopQuarkGroup/data/2011/'
datasets_7TeV = {
                  'ElectronHad' : [data_path_7TeV + 'ElectronHad/nTuple_v11_Run2011A-12Oct2013-v1_GoldenJSON_LeptonPlus3Jets',
                             data_path_7TeV + 'ElectronHad/nTuple_v11_Run2011B-12Oct2013-v1_GoldenJSON_LeptonPlus3Jets'],
                  'SingleMu' : [data_path_7TeV + 'SingleMu/nTuple_v11_Run2011A-12Oct2013-v1_GoldenJSON_LeptonPlus3Jets',
                             data_path_7TeV + 'SingleMu/nTuple_v11_Run2011B-12Oct2013-v1_GoldenJSON_LeptonPlus3Jets'],
                  'TTJet' : [mc_path_7TeV + 'TTJets_MSDecays_central_TuneZ2_7TeV-madgraph-tauola'],
                  'DYJetsToLL' : [mc_path_7TeV + 'DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola'],
                  'QCD_15to20_Mu' : [mc_path_7TeV + 'QCD_Pt-15to20_MuEnrichedPt5_TuneZ2_7TeV_pythia6'],
                  'QCD_20to30_Mu' : [mc_path_7TeV + 'QCD_Pt-20to30_MuEnrichedPt5_TuneZ2_7TeV_pythia6'],
                  'QCD_30to50_Mu' : [mc_path_7TeV + 'QCD_Pt-30to50_MuEnrichedPt5_TuneZ2_7TeV_pythia6'],
                  'QCD_50to80_Mu' : [mc_path_7TeV + 'QCD_Pt-50to80_MuEnrichedPt5_TuneZ2_7TeV_pythia6'],
                  'QCD_80to120_Mu' : [mc_path_7TeV + 'QCD_Pt-80to120_MuEnrichedPt5_TuneZ2_7TeV_pythia6'],
                  'QCD_120to170_Mu' : [mc_path_7TeV + 'QCD_Pt-120to170_MuEnrichedPt5_TuneZ2_7TeV_pythia6'],
                  'QCD_170to300_Mu' : [mc_path_7TeV + 'QCD_Pt-170to300_MuEnrichedPt5_TuneZ2_7TeV_pythia6'],
                  'QCD_300to470_Mu' : [mc_path_7TeV + 'QCD_Pt-300to470_MuEnrichedPt5_TuneZ2_7TeV_pythia6'],
                  'QCD_470to600_Mu' : [mc_path_7TeV + 'QCD_Pt-470to600_MuEnrichedPt5_TuneZ2_7TeV_pythia6'],
                  'QCD_600to800_Mu' : [mc_path_7TeV + 'QCD_Pt-600to800_MuEnrichedPt5_TuneZ2_7TeV_pythia6'],
                  'QCD_800to1000_Mu' : [mc_path_7TeV + 'QCD_Pt-800to1000_MuEnrichedPt5_TuneZ2_7TeV_pythia6'],
                  'QCD_1000_Mu' : [mc_path_7TeV + 'QCD_Pt-1000_MuEnrichedPt5_TuneZ2_7TeV_pythia6'],
                  'QCD_20to30_BCtoE' : [mc_path_7TeV + 'QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6'],
                  'QCD_30to80_BCtoE' : [mc_path_7TeV + 'QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6'],
                  'QCD_80to170_BCtoE' : [mc_path_7TeV + 'QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia'],
                  'QCD_20to30_EMEnriched' : [mc_path_7TeV + 'QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6'],
                  'QCD_30to80_EMEnriched' : [mc_path_7TeV + 'QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia'],
                  'QCD_80to170_EMEnriched' : [mc_path_7TeV + 'QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6'],
                  'QCD_170to250_EMEnriched' : [mc_path_7TeV + 'QCD_Pt-170to250_EMEnriched_TuneZ2_7TeV-pythia6'],
                  'QCD_250to350_EMEnriched' : [mc_path_7TeV + 'QCD_Pt-250to350_EMEnriched_TuneZ2_7TeV-pythia6'],
                  'QCD_350_EMEnriched' : [mc_path_7TeV + 'QCD_Pt-350_EMEnriched_TuneZ2_7TeV-pythia6'],
                  'GJets_40To100' : [mc_path_7TeV + 'GJets_TuneZ2_40_HT_100_7TeV-madgraph'],
                  'GJets_100To200' : [mc_path_7TeV + 'GJets_TuneZ2_100_HT_200_7TeV-madgraph'],
                  'GJets_200' : [mc_path_7TeV + 'GJets_TuneZ2_200_HT_inf_7TeV-madgraph'],
                  'T_s' : [mc_path_7TeV + 'T_TuneZ2_s-channel_7TeV-powheg-tauola'],
                  'T_t' : [mc_path_7TeV + 'T_TuneZ2_t-channel_7TeV-powheg-tauola'],
                  'T_tW' : [mc_path_7TeV + 'T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola'],
                  'Tbar_s' : [mc_path_7TeV + 'Tbar_TuneZ2_s-channel_7TeV-powheg-tauola'],
                  'Tbar_t' : [mc_path_7TeV + 'Tbar_TuneZ2_t-channel_7TeV-powheg-tauola'],
                  'Tbar_tW' : [mc_path_7TeV + 'Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola'],
                  #'WJets' : [mc_path_7TeV + 'WJetsToLNu_TuneZ2_7TeV-madgraph-tauola'],
                  'W1Jet' : [mc_path_7TeV + 'W1Jet_TuneZ2_7TeV-madgraph-tauola'],
                  'W2Jets' : [mc_path_7TeV + 'W2Jets_TuneZ2_7TeV-madgraph-tauola'],
                  'W3Jets' : [mc_path_7TeV + 'W3Jets_TuneZ2_7TeV-madgraph-tauola'],
                  'W4Jets' : [mc_path_7TeV + 'W4Jets_TuneZ2_7TeV-madgraph-tauola'],
                  #'WJets-matchingdown' : [mc_path_7TeV + 'WJetsToLNu_TuneZ2_matchingdown_7TeV-madgraph-tauola/nTuple_v8b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                  #'WJets-matchingup' : [mc_path_7TeV + 'WJetsToLNu_TuneZ2_matchingup_7TeV-madgraph-tauola/nTuple_v8b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                  #'WJets-scaledown' : [mc_path_7TeV + 'WJetsToLNu_TuneZ2_scaledown_7TeV-madgraph-tauola/nTuple_v8b_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                  #'WJets-scaleup' : [mc_path_7TeV + 'WJetsToLNu_TuneZ2_scaleup_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                  #'ZJets-matchingdown' : [mc_path_7TeV + 'ZJetsToLL_TuneZ2_matchingdown_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                  #'ZJets-matchingup' : [mc_path_7TeV + 'ZJetsToLL_TuneZ2_matchingup_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                  #'ZJets-scaledown' : [mc_path_7TeV + 'ZJetsToLL_TuneZ2_scaledown_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                  #'ZJets-scaleup' : [mc_path_7TeV + 'ZJetsToLL_TuneZ2_scaleup_7TeV-madgraph-tauola/nTuple_v8c_Fall11-PU_S6_START44_V9B-v1_LeptonPlus3Jets'],
                     }

mc_path_8TeV = '/hdfs/TopQuarkGroup/mc/8TeV/v11/LeptonPlus3JetsSkim/'
data_path_8TeV = '/hdfs/TopQuarkGroup/data/2012/'
datasets_8TeV = {
 'SingleElectron' : [data_path_8TeV + 'SingleElectron/nTuple_v11_Run2012A-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets',
                                 data_path_8TeV + 'SingleElectron/nTuple_v11_Run2012B-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets',
                                 data_path_8TeV + 'SingleElectron/nTuple_v11_Run2012C-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets',
                                 data_path_8TeV + 'SingleElectron/nTuple_v11_Run2012D-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets'],
             'SingleMu' : [data_path_8TeV + 'SingleMu/nTuple_v11_Run2012A-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets',
                           data_path_8TeV + 'SingleMu/nTuple_v11_Run2012B-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets',
                           data_path_8TeV + 'SingleMu/nTuple_v11_Run2012C-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets',
                           data_path_8TeV + 'SingleMu/nTuple_v11_Run2012D-22Jan2013-v1_AOD_GoldenJSON_LeptonPlus3Jets'],
             'TTJet' : [mc_path_8TeV + 'TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola'],
             'DY1JetsToLL' : [mc_path_8TeV + 'DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph'],
             'DY2JetsToLL' : [mc_path_8TeV + 'DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph'],
             'DY3JetsToLL' : [mc_path_8TeV + 'DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph'],
             'DY4JetsToLL' : [mc_path_8TeV + 'DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph'],
             'QCD_Pt-15to20_Mu' : [mc_path_8TeV + 'QCD_Pt-15to20_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
             'QCD_Pt-20to30_Mu' : [mc_path_8TeV + 'QCD_Pt-20to30_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
             'QCD_Pt-30to50_Mu' : [mc_path_8TeV + 'QCD_Pt-30to50_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
             'QCD_Pt-50to80_Mu' : [mc_path_8TeV + 'QCD_Pt-50to80_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
             'QCD_Pt-80to120_Mu' : [mc_path_8TeV + 'QCD_Pt-80to120_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
             'QCD_Pt-120to170_Mu' : [mc_path_8TeV + 'QCD_Pt-120to170_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
             'QCD_Pt-170to300_Mu' : [mc_path_8TeV + 'QCD_Pt-170to300_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
             'QCD_Pt-300to470_Mu' : [mc_path_8TeV + 'QCD_Pt-300to470_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
             'QCD_Pt-470to600_Mu' : [mc_path_8TeV + 'QCD_Pt-470to600_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
             'QCD_Pt-600to800_Mu' : [mc_path_8TeV + 'QCD_Pt-600to800_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
             'QCD_Pt-800to1000_Mu' : [mc_path_8TeV + 'QCD_Pt-800to1000_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
             'QCD_Pt-1000_Mu' : [mc_path_8TeV + 'QCD_Pt-1000_MuEnrichedPt5_TuneZ2star_8TeV_pythia6'],
             'QCD_20to30_BCtoE' : [mc_path_8TeV + 'QCD_Pt_20_30_BCtoE_TuneZ2star_8TeV_pythia6'],
             'QCD_30to80_BCtoE' : [mc_path_8TeV + 'QCD_Pt_30_80_BCtoE_TuneZ2star_8TeV_pythia6'],
             'QCD_80to170_BCtoE' : [mc_path_8TeV + 'QCD_Pt_80_170_BCtoE_TuneZ2star_8TeV_pythia6'],
             'QCD_170to250_BCtoE' : [mc_path_8TeV + 'QCD_Pt_170_250_BCtoE_TuneZ2star_8TeV_pythia6'],
             'QCD_250to350_BCtoE' : [mc_path_8TeV + 'QCD_Pt_250_350_BCtoE_TuneZ2star_8TeV_pythia6'],
             'QCD_350_BCtoE' : [mc_path_8TeV + 'QCD_Pt_350_BCtoE_TuneZ2star_8TeV_pythia6'],
             'QCD_20to30_EMEnriched' : [mc_path_8TeV + 'QCD_Pt_20_30_EMEnriched_TuneZ2star_8TeV_pythia6'],
             'QCD_30to80_EMEnriched' : [mc_path_8TeV + 'QCD_Pt_30_80_EMEnriched_TuneZ2star_8TeV_pythia6'],
             'QCD_80to170_EMEnriched' : [mc_path_8TeV + 'QCD_Pt_80_170_EMEnriched_TuneZ2star_8TeV_pythia6'],
             'QCD_170to250_EMEnriched' : [mc_path_8TeV + 'QCD_Pt_170_250_EMEnriched_TuneZ2star_8TeV_pythia6'],
             'QCD_250to350_EMEnriched' : [mc_path_8TeV + 'QCD_Pt_250_350_EMEnriched_TuneZ2star_8TeV_pythia6'],
             'QCD_350_EMEnriched' : [mc_path_8TeV + 'QCD_Pt_350_EMEnriched_TuneZ2star_8TeV_pythia6'],
             'GJets_200To400' : [mc_path_8TeV + 'GJets_HT-200To400_8TeV-madgraph_v2'],
             'GJets_400' : [mc_path_8TeV + 'GJets_HT-400ToInf_8TeV-madgraph_v2'],
             'T_s' : [mc_path_8TeV + 'T_s-channel_TuneZ2star_8TeV-powheg-tauola'],
             'T_t' : [mc_path_8TeV + 'T_t-channel_TuneZ2star_8TeV-powheg-tauola'],
             'T_tW' : [mc_path_8TeV + 'T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola'],
             'Tbar_s' : [mc_path_8TeV + 'Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola'],
             'Tbar_t' : [mc_path_8TeV + 'Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola'],
             'Tbar_tW' : [mc_path_8TeV + 'Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola'],
             'W1Jet' : [mc_path_8TeV + 'W1JetsToLNu_TuneZ2Star_8TeV-madgraph'],
             'W2Jets' : [mc_path_8TeV + 'W2JetsToLNu_TuneZ2Star_8TeV-madgraph'],
             'W3Jets' : [mc_path_8TeV + 'W3JetsToLNu_TuneZ2Star_8TeV-madgraph'],
             'W4Jets' : [mc_path_8TeV + 'W4JetsToLNu_TuneZ2Star_8TeV-madgraph'],
             'WJets-matchingdown' : [mc_path_8TeV + 'WJetsToLNu_matchingdown_8TeV-madgraph-tauola'],
             'WJets-matchingup' : [mc_path_8TeV + 'WJetsToLNu_matchingup_8TeV-madgraph-tauola'],
             'WJets-scaledown' : [mc_path_8TeV + 'WJetsToLNu_scaledown_8TeV-madgraph-tauola'],
             'WJets-scaleup' : [mc_path_8TeV + 'WJetsToLNu_scaleup_8TeV-madgraph-tauola'],
             'ZJets-matchingdown' : [mc_path_8TeV + 'DYJetsToLL_M-50_matchingdown_8TeV-madgraph-tauola'],
             'ZJets-matchingup' : [mc_path_8TeV + 'DYJetsToLL_M-50_matchingup_8TeV-madgraph-tauola'],
             'ZJets-scaledown' : [mc_path_8TeV + 'DYJetsToLL_M-50_scaledown_8TeV-madgraph-tauola'],
             'ZJets-scaleup' : [mc_path_8TeV + 'DYJetsToLL_M-50_scaleup_8TeV-madgraph-tauola'],
 #             'TTH_Inclusive_M-125' : [mc_path_8TeV + 'TTH_Inclusive_M-125_8TeV_pythia6'],
            }

mc_path_13TeV = '/hdfs/TopQuarkGroup/run2/ntuples/v25/'
data_path_13TeV = ''
datasets_13TeV = {
            'SingleMuon_PromptReco' : [ mc_path_13TeV + 'SingleMuon_PromptReco' ],
            'SingleMuon_Rereco' : [ mc_path_13TeV + 'SingleMuon_Rereco' ],
            'SingleElectron_PromptReco' : [ mc_path_13TeV + 'SingleElectron_PromptReco' ],
            'SingleElectron_Rereco' : [ mc_path_13TeV + 'SingleElectron_Rereco' ],

            'TTJets_PowhegPythia8' : [ mc_path_13TeV + 'TTJets_PowhegPythia8' ],
            'TTJets_PowhegPythia8_scaledown' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_scaledown' ],
            'TTJets_PowhegPythia8_scaleup' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_scaleup' ],
            'TTJets_PowhegPythia8_mtop1695' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_mtop1695' ],
            'TTJets_PowhegPythia8_mtop1755' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_mtop1755' ],
            
            'TTJets_amcatnloFXFX' : [ mc_path_13TeV + 'TTJets_amcatnloFXFX' ],
            # 'TTJets_amcatnloFXFX_scaledown' : [ mc_path_13TeV + 'TTJets_amcatnloFXFX_scaledown' ],
            # 'TTJets_amcatnloFXFX_scaleup' : [ mc_path_13TeV + 'TTJets_amcatnloFXFX_scaleup' ],
            # 'TTJets_amcatnloFXFX_mtop1695' : [ mc_path_13TeV + 'TTJets_amcatnloFXFX_mtop1695' ],
            # 'TTJets_amcatnloFXFX_mtop1755' : [ mc_path_13TeV + 'TTJets_amcatnloFXFX_mtop1755' ],
            
            # 'TTJets_PowhegPythia6' : [ mc_path_13TeV + 'TTJets_PowhegPythia6' ],
            'TTJets_madgraphMLM' : [ mc_path_13TeV + 'TTJets_madgraphMLM'],
            'TTJets_PowhegHerwigpp' : [ mc_path_13TeV + 'TTJets_PowhegHerwigpp'],
            # 'TTJets_synch' : [ mc_path_13TeV + 'TTJets_synch'],

            'DYJetsToLL_M50' : [ mc_path_13TeV + 'DYJetsToLL_M50' ],
            'WJetsToLNu' : [ mc_path_13TeV + 'WJetsToLNu' ],

            # 'TToLeptons_s' : [mc_path_13TeV + 'TToLeptons_s_PU20bx25'],
            # 'TBarToLeptons_s' : [mc_path_13TeV + 'TBarToLeptons_s_PU20bx25'],
            # 'TBarToLeptons_t' : [mc_path_13TeV + 'TBarToLeptons_t_PU20bx25'],

            'TToLeptons_t' : [mc_path_13TeV + 'TToLeptons_t'],
            'T_tw' : [mc_path_13TeV + 'T_tw'],
            'Tbar_tw' : [mc_path_13TeV + 'Tbar_tw'],

            # 'GJets_15to6000' : [mc_path_13TeV + 'GJets_15to6000'],

            'QCD_EMEnriched_15to20' : [mc_path_13TeV + 'QCD_EMEnriched_15to20'],
            'QCD_EMEnriched_20to30' : [mc_path_13TeV + 'QCD_EMEnriched_20to30'],
            'QCD_EMEnriched_30to50' : [mc_path_13TeV + 'QCD_EMEnriched_30to50'],
            'QCD_EMEnriched_50to80' : [mc_path_13TeV + 'QCD_EMEnriched_50to80'],
            'QCD_EMEnriched_80to120' : [mc_path_13TeV + 'QCD_EMEnriched_80to120'],
            'QCD_EMEnriched_120to170' : [mc_path_13TeV + 'QCD_EMEnriched_120to170'],
            'QCD_EMEnriched_170to300' : [mc_path_13TeV + 'QCD_EMEnriched_170to300'],
            'QCD_EMEnriched_300toInf' : [mc_path_13TeV + 'QCD_EMEnriched_300toInf'],

            'QCD_bcToE_15to20' : [mc_path_13TeV + 'QCD_bcToE_15to20'],
            'QCD_bcToE_20to30' : [mc_path_13TeV + 'QCD_bcToE_20to30'],
            'QCD_bcToE_30to80' : [mc_path_13TeV + 'QCD_bcToE_30to80'],
            'QCD_bcToE_80to170' : [mc_path_13TeV + 'QCD_bcToE_80to170'],
            'QCD_bcToE_170to250' : [mc_path_13TeV + 'QCD_bcToE_170to250'],
            'QCD_bcToE_250toInf' : [mc_path_13TeV + 'QCD_bcToE_250toInf'],

            'QCD_MuEnriched_15to20': [mc_path_13TeV + 'QCD_MuEnriched_15to20'],
            'QCD_MuEnriched_20to30': [mc_path_13TeV + 'QCD_MuEnriched_20to30'],
            'QCD_MuEnriched_30to50': [mc_path_13TeV + 'QCD_MuEnriched_30to50'],
            'QCD_MuEnriched_50to80': [mc_path_13TeV + 'QCD_MuEnriched_50to80'],
            'QCD_MuEnriched_80to120': [mc_path_13TeV + 'QCD_MuEnriched_80to120'],
            'QCD_MuEnriched_120to170': [mc_path_13TeV + 'QCD_MuEnriched_120to170'],
            'QCD_MuEnriched_170to300': [mc_path_13TeV + 'QCD_MuEnriched_170to300'],
            'QCD_MuEnriched_300to470': [mc_path_13TeV + 'QCD_MuEnriched_300to470'],
            'QCD_MuEnriched_470to600': [mc_path_13TeV + 'QCD_MuEnriched_470to600'],
            'QCD_MuEnriched_600to800': [mc_path_13TeV + 'QCD_MuEnriched_600to800'],
            'QCD_MuEnriched_800to1000': [mc_path_13TeV + 'QCD_MuEnriched_800to1000'],
            'QCD_MuEnriched_1000toInf': [mc_path_13TeV + 'QCD_MuEnriched_1000toInf'],


            # 'GJets_100to200' : [mc_path_13TeV + 'GJets_100to200_PU20bx25'],
            # 'GJets_200to400' : [mc_path_13TeV + 'GJets_200to400_PU20bx25'],
            # 'GJets_400to600' : [mc_path_13TeV + 'GJets_400to600_PU20bx25'],
            # 'GJets_600toInf' : [mc_path_13TeV + 'GJets_600toInf_PU20bx25'],

            # 'QCD_MuEnriched_20toInf' : [mc_path_13TeV + 'QCD_MuEnriched_20toInf_PU20bx25'],
            # 'QCD_MuEnriched_30to50' : [mc_path_13TeV + 'QCD_MuEnriched_30to50_PU20bx25'],
            # 'QCD_MuEnriched_50to80' : [mc_path_13TeV + 'QCD_MuEnriched_50to80_PU20bx25'],
            # 'QCD_MuEnriched_80to120' : [mc_path_13TeV + 'QCD_MuEnriched_80to120_PU20bx25'],

            # 'QCD_EMEnriched_20to30' : [mc_path_13TeV + 'QCD_EMEnriched_20to30_PU20bx25'],
            # 'QCD_EMEnriched_30to80' : [mc_path_13TeV + 'QCD_EMEnriched_30to80_PU20bx25'],
            # 'QCD_EMEnriched_80to170' : [mc_path_13TeV + 'QCD_EMEnriched_80to170_PU20bx25'],

            # 'QCD_bcToE_20to30' : [mc_path_13TeV + 'QCD_bcToE_20to30_PU20bx25'],
            # 'QCD_bcToE_30to80' : [mc_path_13TeV + 'QCD_bcToE_30to80_PU20bx25'],
            # 'QCD_bcToE_80to170' : [mc_path_13TeV + 'QCD_bcToE_80to170_PU20bx25'],
            # 'QCD_bcToE_170toInf' : [mc_path_13TeV + 'QCD_bcToE_170toInf_PU20bx25'],

            }

analysis_modes_13TeV = ['central',
                 'JES_down',
                 'JES_up',
                 'JetSmearing_down',
                 'JetSmearing_up',
                 ]

analysisModes = ['central',
                 'Electron_down',
                 'Electron_up',
                 'Muon_down',
                 'Muon_up',
                 'BJet_down',
                 'BJet_up',
                 'JES_down',
                 'JES_up',
                 'LightJet_down',
                 'LightJet_up',
                 'PU_down',
                 'PU_up',
                 'JetSmearing_up',
                 'JetSmearing_down',
                 'Test']
