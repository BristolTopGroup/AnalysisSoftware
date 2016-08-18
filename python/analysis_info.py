mc_path_13TeV = '/phxlk/ntuple/v0.0.8/Spring16/'
# data_path_13TeV = '/hdfs/TopQuarkGroup/run2/ntuples/25ns/v0.0.3/'
data_path_13TeV = '/phxlk/ntuple/v0.0.8/Spring16/'
datasets_13TeV = {

    'SingleMuon' : [ mc_path_13TeV + 'SingleElectron_Run2016B_PromptReco_v1',
                     mc_path_13TeV + 'SingleElectron_Run2016B_PromptReco_v2'
                    ],

    'SingleElectron' : [ mc_path_13TeV + 'SingleElectron_Run2016B_PromptReco_v1',
                         mc_path_13TeV + 'SingleElectron_Run2016B_PromptReco_v2'
                        ],

    'TTJets_PowhegPythia8' : [ mc_path_13TeV + 'TTJets_PowhegPythia8' ],
    'TTJets_PowhegPythia8_scaledown' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_scaledown' ],
    'TTJets_PowhegPythia8_scaleup' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_scaleup' ],
    'TTJets_PowhegPythia8_mtop1695' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_mtop1695' ],
    'TTJets_PowhegPythia8_mtop1755' : [ mc_path_13TeV + 'TTJets_PowhegPythia8_mtop1755' ],
    
    'TTJets_amcatnloFXFX' : [ mc_path_13TeV + 'TTJets_amcatnloFXFX' ],
    'TTJets_madgraphMLM' : [ mc_path_13TeV + 'TTJets_madgraphMLM'],
    'TTJets_PowhegHerwigpp' : [ mc_path_13TeV + 'TTJets_PowhegHerwigpp'],
    'TTJets_amcatnloHerwigpp' : [ mc_path_13TeV + 'TTJets_amcatnloHerwigpp'],

    # 'TTJets_synch' : [ mc_path_13TeV + 'TTJets_synch'],

    'DYJetsToLL_M50' : [ mc_path_13TeV + 'DYJetsToLL_M50' ],
    'WJetsToLNu' : [ mc_path_13TeV + 'WJetsToLNu' ],

    'STbar_t' : [mc_path_13TeV + 'STbar_t'],
    'ST_s' : [mc_path_13TeV + 'ST_s'],
    'ST_tW' : [mc_path_13TeV + 'ST_tw'],
    'STbar_tW' : [mc_path_13TeV + 'STbar_tw'],

    # 'GJets_100to200' : [mc_path_13TeV + 'GJets_100to200'],
    # 'GJets_200to400' : [mc_path_13TeV + 'GJets_200to400'],
    # 'GJets_400to600' : [mc_path_13TeV + 'GJets_400to600'],
    # 'GJets_600toInf' : [mc_path_13TeV + 'GJets_600toInf'],

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
