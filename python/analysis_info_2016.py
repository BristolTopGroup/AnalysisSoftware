import os
mc_path_13TeV = '/hdfs/TopQuarkGroup/phxlk/ntuple/v0.0.8/Spring16/'
data_path_13TeV = '/hdfs/TopQuarkGroup/phxlk/ntuple/v0.0.8/Spring16/'

datasets_13TeV = {

    'SingleMuon' : [ 
                     mc_path_13TeV + 'SingleMuon_Run2016B_PromptReco_v2/tmp/',
                     mc_path_13TeV + 'SingleMuon_Run2016C_PromptReco_v2/tmp/',
                     mc_path_13TeV + 'SingleMuon_Run2016D_PromptReco_v2/tmp/',
                     mc_path_13TeV + 'SingleMuon_Run2016E_PromptReco_v2/tmp/',
                     # mc_path_13TeV + 'SingleMuon_Run2016F_PromptReco_v1/tmp/',
                    ],

    'SingleElectron' : [ 
                         mc_path_13TeV + 'SingleElectron_Run2016B_PromptReco_v2/tmp/',
                         mc_path_13TeV + 'SingleElectron_Run2016C_PromptReco_v2/tmp/',
                         mc_path_13TeV + 'SingleElectron_Run2016D_PromptReco_v2/tmp/',
                         mc_path_13TeV + 'SingleElectron_Run2016E_PromptReco_v2/tmp/',
                         # mc_path_13TeV + 'SingleElectron_Run2016F_PromptReco_v1/tmp/',
                        ],
    'TTJets_PowhegPythia8' : [ mc_path_13TeV + 'TTJets_PowhegPythia8/tmp/' ],
    'TTJets_PowhegPythia8_scaledown' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_scaledown/tmp/' ],
    'TTJets_PowhegPythia8_scaleup' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_scaleup/tmp/' ],
    'TTJets_PowhegPythia8_mtop1695' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_mtop1695/tmp/' ],
    'TTJets_PowhegPythia8_mtop1755' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_mtop1755/tmp/' ],

    'TTJets_amcatnloFXFX' : [ mc_path_13TeV + 'TTJets_amcatnloFXFX/tmp/' ],

    'TTJets_madgraphMLM' : [ mc_path_13TeV + 'TTJets_madgraphMLM/tmp/'],
    'TTJets_PowhegHerwigpp' : [ mc_path_13TeV + 'TTJets_PowhegHerwigpp/tmp/'],

    # 'TTJets_synch' : [ mc_path_13TeV + 'TTJets_synch/tmp/'],

    'DYJetsToLL_M50' : [ mc_path_13TeV + 'DYJetsToLL_M50/tmp/' ],
    'WJetsToLNu' : [ mc_path_13TeV + 'WJetsToLNu/tmp/' ],

    'STbar_t' : [mc_path_13TeV + 'STbar_t/tmp/'],
    'ST_s' : [mc_path_13TeV + 'ST_s/tmp/'],
    'ST_tW' : [mc_path_13TeV + 'ST_tw/tmp/'],
    'STbar_tW' : [mc_path_13TeV + 'STbar_tw/tmp/'],

    'QCD_EMEnriched_15to20' : [mc_path_13TeV + 'QCD_EMEnriched_15to20/tmp/'],
    'QCD_EMEnriched_20to30' : [mc_path_13TeV + 'QCD_EMEnriched_20to30/tmp/'],
    'QCD_EMEnriched_30to50' : [mc_path_13TeV + 'QCD_EMEnriched_30to50/tmp/'],
    'QCD_EMEnriched_50to80' : [mc_path_13TeV + 'QCD_EMEnriched_50to80/tmp/'],
    'QCD_EMEnriched_80to120' : [mc_path_13TeV + 'QCD_EMEnriched_80to120/tmp/'],
    'QCD_EMEnriched_120to170' : [mc_path_13TeV + 'QCD_EMEnriched_120to170/tmp/'],
    'QCD_EMEnriched_170to300' : [mc_path_13TeV + 'QCD_EMEnriched_170to300/tmp/'],
    'QCD_EMEnriched_300toInf' : [mc_path_13TeV + 'QCD_EMEnriched_300toInf/tmp/'],

    # 'QCD_bcToE_15to20' : [mc_path_13TeV + 'QCD_bcToE_15to20/tmp/'],
    # 'QCD_bcToE_20to30' : [mc_path_13TeV + 'QCD_bcToE_20to30/tmp/'],
    'QCD_bcToE_30to80' : [mc_path_13TeV + 'QCD_bcToE_30to80/tmp/'],
    'QCD_bcToE_80to170' : [mc_path_13TeV + 'QCD_bcToE_80to170/tmp/'],
    # 'QCD_bcToE_170to250' : [mc_path_13TeV + 'QCD_bcToE_170to250/tmp/'],
    'QCD_bcToE_250toInf' : [mc_path_13TeV + 'QCD_bcToE_250toInf/tmp/'],

    'QCD_MuEnriched_15to20': [mc_path_13TeV + 'QCD_MuEnriched_15to20/tmp/'],
    'QCD_MuEnriched_20to30': [mc_path_13TeV + 'QCD_MuEnriched_20to30/tmp/'],
    'QCD_MuEnriched_30to50': [mc_path_13TeV + 'QCD_MuEnriched_30to50/tmp/'],
    'QCD_MuEnriched_50to80': [mc_path_13TeV + 'QCD_MuEnriched_50to80/tmp/'],
    'QCD_MuEnriched_80to120': [mc_path_13TeV + 'QCD_MuEnriched_80to120/tmp/'],
    'QCD_MuEnriched_120to170': [mc_path_13TeV + 'QCD_MuEnriched_120to170/tmp/'],
    'QCD_MuEnriched_170to300': [mc_path_13TeV + 'QCD_MuEnriched_170to300/tmp/'],
    'QCD_MuEnriched_300to470': [mc_path_13TeV + 'QCD_MuEnriched_300to470/tmp/'],
    'QCD_MuEnriched_470to600': [mc_path_13TeV + 'QCD_MuEnriched_470to600/tmp/'],
    'QCD_MuEnriched_600to800': [mc_path_13TeV + 'QCD_MuEnriched_600to800/tmp/'],
    'QCD_MuEnriched_800to1000': [mc_path_13TeV + 'QCD_MuEnriched_800to1000/tmp/'],
    'QCD_MuEnriched_1000toInf': [mc_path_13TeV + 'QCD_MuEnriched_1000toInf/tmp/'],
}

analysis_modes_13TeV = [
    'central',
    'JES_down',
    'JES_up',
    'JetSmearing_down',
    'JetSmearing_up',
]

analysisModes = [
    'central',
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
    'Test'
]
