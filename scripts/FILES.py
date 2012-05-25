
placeholder = 'SAMPLE'
pathToFiles = '/storage/results/histogramfiles/2012/7TeV'
luminosity = 5028#pb-1
suffix = 'PFElectron_PFMuon_PF2PATJets_PFMET.root'

template = '%(path)s/%(placeholder)s_%(lumi)dpb_%(suffix)s' % {'path':pathToFiles, 'lumi':luminosity, 'suffix':suffix, 'placeholder':placeholder}

samplesToLoad = ['ElectronHad',
                 'TTJet', 
                 'WJetsToLNu', 
                 'DYJetsToLL',
                 'QCD_Pt-20to30_BCtoE',
                 'QCD_Pt-30to80_BCtoE',
                 'QCD_Pt-80to170_BCtoE',
                 'QCD_Pt-20to30_EMEnriched',
                 'QCD_Pt-30to80_EMEnriched',
                 'QCD_Pt-80to170_EMEnriched',
                 'GJets_HT-40To100',
                 'GJets_HT-100To200',
                 'GJets_HT-200',
                 'WWtoAnything',
                 'WZtoAnything',
                 'ZZtoAnything',
                 'T_tW-channel',
                 'T_t-channel',
                 'T_s-channel',
                 'Tbar_tW-channel',
                 'Tbar_t-channel',
                 'Tbar_s-channel',
#                 'TTbarZIncl',
#                 'TTbarInclWIncl'
                 ]

files = {}

rpl = template.replace

for sample in samplesToLoad:
    files[sample] = rpl(placeholder, sample)
