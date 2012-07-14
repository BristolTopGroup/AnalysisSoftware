
placeholder = 'SAMPLE'
pathToFiles = '/storage/results/histogramfiles/2012/7TeV'
luminosity = 5050#pb-1
suffix = 'PFElectron_PFMuon_PF2PATJets_PFMET.root'
JES_down_suffix = 'PFElectron_PFMuon_PF2PATJets_PFMET_minusJES.root'
JES_up_suffix = 'PFElectron_PFMuon_PF2PATJets_PFMET_plusJES.root'
PU_down_suffix = 'PFElectron_PFMuon_PF2PATJets_PFMET_PU_64600mb.root'
PU_up_suffix = 'PFElectron_PFMuon_PF2PATJets_PFMET_PU_71400mb.root'
PDFWeights_suffix = 'PFElectron_PFMuon_PF2PATJets_PFMET_PDF_%d.root'

template = '%(path)s/%(placeholder)s_%(lumi)dpb_%(suffix)s' % {'path':pathToFiles, 'lumi':luminosity, 'suffix':suffix, 'placeholder':placeholder}
template_JES_down = '%(path)s/%(placeholder)s_%(lumi)dpb_%(suffix)s' % {'path':pathToFiles + '/JES_down', 'lumi':luminosity, 'suffix':JES_down_suffix, 'placeholder':placeholder}
template_JES_up = '%(path)s/%(placeholder)s_%(lumi)dpb_%(suffix)s' % {'path':pathToFiles + '/JES_up', 'lumi':luminosity, 'suffix':JES_up_suffix, 'placeholder':placeholder}
template_PU_down = '%(path)s/%(placeholder)s_%(lumi)dpb_%(suffix)s' % {'path':pathToFiles + '/PU_down', 'lumi':luminosity, 'suffix':PU_down_suffix, 'placeholder':placeholder}
template_PU_up = '%(path)s/%(placeholder)s_%(lumi)dpb_%(suffix)s' % {'path':pathToFiles+ '/PU_up', 'lumi':luminosity, 'suffix':PU_up_suffix, 'placeholder':placeholder}
template_PDFWeights = '%(path)s/%(placeholder)s_%(lumi)dpb_%(suffix)s' % {'path':pathToFiles+ '/PDFWeights', 'lumi':luminosity, 'suffix':PDFWeights_suffix, 'placeholder':'TTJet'}

samplesToLoad = ['ElectronHad',
                 'TTJet', 
#                 'WJetsToLNu', 
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
                 'TTbarZIncl',
                 'TTbarInclWIncl',
                 'W1Jet',
                 'W2Jets',
                 'W3Jets',
                 'W4Jets'                 
                 ]
additionalSamples = [
                 'TTJets-matchingdown',
                 'TTJets-matchingup',
                 'TTJets-scaledown',
                 'TTJets-scaleup',
                 'WJets-matchingdown',
                 'WJets-matchingup',
                 'WJets-scaledown',
                 'WJets-scaleup',
                 'ZJets-matchingdown',
                 'ZJets-matchingup',
                 'ZJets-scaledown',
                 'ZJets-scaleup'
                     ]

files = {}
files_JES_down = {}
files_JES_up = {}
files_PU_down = {}
files_PU_up = {}
files_PDF_weights = {}
rpl = template.replace
rpl_JES_down = template_JES_down.replace
rpl_JES_up = template_JES_up.replace
rpl_PU_down = template_PU_down.replace
rpl_PU_up = template_PU_up.replace

for sample in samplesToLoad:
    files[sample] = rpl(placeholder, sample)
    files_JES_down[sample] = rpl_JES_down(placeholder, sample)
    files_JES_up[sample] = rpl_JES_up(placeholder, sample)
    files_PU_down[sample] = rpl_PU_down(placeholder, sample)
    files_PU_up[sample] = rpl_PU_up(placeholder, sample)
    
for sample in additionalSamples:
    files[sample] = rpl(placeholder, sample)
    
    
for index in range(1,45):
    files_PDF_weights['TTJet_%d' % index] = template_PDFWeights % index

