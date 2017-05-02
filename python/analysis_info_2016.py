import os
mc_path_13TeV = '/hdfs/TopQuarkGroup/ec6821/1.0.7/ntuple/v1.0.7/Moriond17/'
data_path_13TeV = '/hdfs/TopQuarkGroup/ec6821/1.0.7/ntuple/v1.0.7/Moriond17/'

datasets_13TeV = {

    'SingleMuon_RunB' : [ mc_path_13TeV + 'SingleMuon_RunB_Rereco_v3/tmp/'],
    'SingleMuon_RunC' : [ mc_path_13TeV + 'SingleMuon_RunC_Rereco_v1/tmp/'],
    'SingleMuon_RunD' : [ mc_path_13TeV + 'SingleMuon_RunD_Rereco_v1/tmp/'],
    'SingleMuon_RunE' : [ mc_path_13TeV + 'SingleMuon_RunE_Rereco_v1/tmp/'],
    'SingleMuon_RunF' : [ mc_path_13TeV + 'SingleMuon_RunF_Rereco_v1/tmp/'],
    'SingleMuon_RunG' : [ mc_path_13TeV + 'SingleMuon_RunG_Rereco_v1/tmp/'],
    'SingleMuon_RunH_Prompt_v2' : [ mc_path_13TeV + 'SingleMuon_RunH_Prompt_v2/tmp/'],
    'SingleMuon_RunH_Prompt_v3' : [ mc_path_13TeV + 'SingleMuon_RunH_Prompt_v3/tmp/'],


    'SingleElectron_RunB' : [ mc_path_13TeV + 'SingleElectron_RunB_Rereco_v3/tmp/'],
    'SingleElectron_RunC' : [ mc_path_13TeV + 'SingleElectron_RunC_Rereco_v1/tmp/'],
    'SingleElectron_RunD' : [ mc_path_13TeV + 'SingleElectron_RunD_Rereco_v1/tmp/'],
    'SingleElectron_RunE' : [ mc_path_13TeV + 'SingleElectron_RunE_Rereco_v1/tmp/'],
    'SingleElectron_RunF' : [ mc_path_13TeV + 'SingleElectron_RunF_Rereco_v1/tmp/'],
    'SingleElectron_RunG' : [ mc_path_13TeV + 'SingleElectron_RunG_Rereco_v1/tmp/'],
    'SingleElectron_RunH_Prompt_v2' : [ mc_path_13TeV + 'SingleElectron_RunH_Prompt_v2/tmp/'],
    'SingleElectron_RunH_Prompt_v3' : [ mc_path_13TeV + 'SingleElectron_RunH_Prompt_v3/tmp/'],

    'TTJets_PowhegPythia8' : [ mc_path_13TeV  + 'TTJets_PowhegPythia8/tmp/' ],
    'TTJets_PowhegPythia8_fsrdown' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_fsrdown*/tmp/' ],
    'TTJets_PowhegPythia8_fsrup' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_fsrup*/tmp/' ],
    'TTJets_PowhegPythia8_isrdown' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_isrdown*/tmp/' ],
    'TTJets_PowhegPythia8_isrup' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_isrup*/tmp/' ],
    'TTJets_PowhegPythia8_down' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_down*/tmp/' ],
    'TTJets_PowhegPythia8_up' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_up*/tmp/' ],
    'TTJets_PowhegPythia8_mtop1695' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_mtop1695*/tmp/' ],
    'TTJets_PowhegPythia8_mtop1755' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_mtop1755*/tmp/' ],
    'TTJets_PowhegPythia8_hdampdown' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_hdampdown*/tmp/' ],
    'TTJets_PowhegPythia8_hdampup' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_hdampup*/tmp/' ],
    'TTJets_PowhegPythia8_erdOn' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_erdOn*/tmp/' ],

    'TTJets_amcatnloFXFX' : [ mc_path_13TeV + 'TTJets_amcatnloFXFX/tmp/' ],
    'TTJets_madgraphMLM' : [ mc_path_13TeV + 'TTJets_madgraphMLM/tmp/'],
    'TTJets_PowhegHerwigpp' : [ mc_path_13TeV + 'TTJets_PowhegHerwigpp*/tmp/'],

    # # # 'TTJets_synch' : [ mc_path_13TeV + 'TTJets_synch/tmp/'],

    # 'DYJetsToLL_M50' : [ mc_path_13TeV + 'DYJetsToLL_M50/tmp/' ],
    'DY1JetsToLL_M50' : [ mc_path_13TeV + 'DY1JetsToLL_M50/tmp/' ],
    'DY2JetsToLL_M50' : [ mc_path_13TeV + 'DY2JetsToLL_M50/tmp/' ],
    'DY3JetsToLL_M50' : [ mc_path_13TeV + 'DY3JetsToLL_M50/tmp/' ],
    'DY4JetsToLL_M50' : [ mc_path_13TeV + 'DY4JetsToLL_M50/tmp/' ],

    # 'WJetsToLNu' : [ mc_path_13TeV + 'WJetsToLNu/tmp/' ],
    'W1JetsToLNu' : [ mc_path_13TeV + 'W1JetsToLNu/tmp/' ],
    'W2JetsToLNu' : [ mc_path_13TeV + 'W2JetsToLNu/tmp/' ],
    'W3JetsToLNu' : [ mc_path_13TeV + 'W3JetsToLNu/tmp/' ],
    'W4JetsToLNu' : [ mc_path_13TeV + 'W4JetsToLNu/tmp/' ],

    'ST_t' : [mc_path_13TeV + 'ST_t/tmp/'],
    'STbar_t' : [mc_path_13TeV + 'STbar_t/tmp/'],
    'ST_s' : [mc_path_13TeV + 'ST_s/tmp/'],
    'ST_tW' : [mc_path_13TeV + 'ST_tW/tmp/'],
    'STbar_tW' : [mc_path_13TeV + 'STbar_tW/tmp/'],

    'QCD_EMEnriched_20to30' : [mc_path_13TeV + 'QCD_EMEnriched_20to30/tmp/'],
    'QCD_EMEnriched_30to50' : [mc_path_13TeV + 'QCD_EMEnriched_30to50/tmp/'],
    'QCD_EMEnriched_50to80' : [mc_path_13TeV + 'QCD_EMEnriched_50to80/tmp/'],
    'QCD_EMEnriched_80to120' : [mc_path_13TeV + 'QCD_EMEnriched_80to120/tmp/'],
    'QCD_EMEnriched_120to170' : [mc_path_13TeV + 'QCD_EMEnriched_120to170/tmp/'],
    'QCD_EMEnriched_170to300' : [mc_path_13TeV + 'QCD_EMEnriched_170to300/tmp/'],
    'QCD_EMEnriched_300toInf' : [mc_path_13TeV + 'QCD_EMEnriched_300toInf/tmp/'],

    'QCD_bcToE_30to80' : [mc_path_13TeV + 'QCD_bcToE_30to80/tmp/'],
    'QCD_bcToE_80to170' : [mc_path_13TeV + 'QCD_bcToE_80to170/tmp/'],
    'QCD_bcToE_170to250' : [mc_path_13TeV + 'QCD_bcToE_170to250/tmp/'],
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
