lumi = 5050
folder = '/storage/results/histogramfiles/AN-12-241/central2'
#folder = '/storage/results/histogramfiles/AN-12-241/PU_down'
#folder = '/storage/results/histogramfiles/AN-12-241/PU_up'
identifier = 'PFElectron_PFMuon_PF2PATJets_PFMET'
#identifier = 'PFElectron_PFMuon_PF2PATJets_PFMET_PU_64600mb'
#identifier = 'PFElectron_PFMuon_PF2PATJets_PFMET_PU_71400mb'

files = {
    'data':"%s/ElectronHad_%dpb_%s.root" % (folder, lumi, identifier),
#    'data':"%s/SingleMu_%dpb_%s.root" % (folder, lumi, identifier),
    'ttbar' : "%s/TTJet_%dpb_%s.root" %(folder, lumi, identifier),
#    'wjets' : "%s/WJetsToLNu_%dpb_%s.root" %(folder, lumi, identifier),
    'W1Jet' : "%s/W1Jet_%dpb_%s.root" %(folder, lumi, identifier),
    'W2Jets' : "%s/W2Jets_%dpb_%s.root" %(folder, lumi, identifier),
    'W3Jets' : "%s/W3Jets_%dpb_%s.root" %(folder, lumi, identifier),
    'W4Jets' : "%s/W4Jets_%dpb_%s.root" %(folder, lumi, identifier),
    'zjets' : "%s/DYJetsToLL_%dpb_%s.root" %(folder, lumi, identifier),
    'bce1' : "%s/QCD_Pt-20to30_BCtoE_%dpb_%s.root" %(folder, lumi, identifier),
    'bce2' : "%s/QCD_Pt-30to80_BCtoE_%dpb_%s.root" %(folder, lumi, identifier),
    'bce3' : "%s/QCD_Pt-80to170_BCtoE_%dpb_%s.root" %(folder, lumi, identifier),
    'muQCD' : "%s/QCD_Pt-20_MuEnrichedPt-15_%dpb_%s.root" %(folder, lumi, identifier),
    
    'enri1' : "%s/QCD_Pt-20to30_EMEnriched_%dpb_%s.root" %(folder, lumi, identifier),
    'enri2' : "%s/QCD_Pt-30to80_EMEnriched_%dpb_%s.root" %(folder, lumi, identifier),
    'enri3' : "%s/QCD_Pt-80to170_EMEnriched_%dpb_%s.root" %(folder, lumi, identifier),
    'pj1' : "%s/GJets_HT-40To100_%dpb_%s.root" %(folder, lumi, identifier),
    'pj2' : "%s/GJets_HT-100To200_%dpb_%s.root" %(folder, lumi, identifier),
    'pj3' : "%s/GJets_HT-200_%dpb_%s.root" %(folder, lumi, identifier),
    'ww' : "%s/WWtoAnything_%dpb_%s.root" %(folder, lumi, identifier),
    'wz' : "%s/WZtoAnything_%dpb_%s.root" %(folder, lumi, identifier),
    'zz' : "%s/ZZtoAnything_%dpb_%s.root" %(folder, lumi, identifier),
    'T_TuneZ2_tW-channel' : "%s/T_tW-channel_%dpb_%s.root" %(folder, lumi, identifier),
    'T_TuneZ2_t-channel' : "%s/T_t-channel_%dpb_%s.root" %(folder, lumi, identifier),
    'T_TuneZ2_s-channel' : "%s/T_s-channel_%dpb_%s.root" %(folder, lumi, identifier),
    'Tbar_TuneZ2_tW-channel' : "%s/Tbar_tW-channel_%dpb_%s.root" %(folder, lumi, identifier),
    'Tbar_TuneZ2_t-channel' : "%s/Tbar_t-channel_%dpb_%s.root" %(folder, lumi, identifier),
    'Tbar_TuneZ2_s-channel' : "%s/Tbar_s-channel_%dpb_%s.root" %(folder, lumi, identifier),
#    'TTbarZIncl' : "%s/TTbarZIncl_%dpb_%s.root" %(folder, lumi, identifier),
#    'TTbarInclWIncl' : "%s/TTbarInclWIncl_%dpb_%s.root" %(folder, lumi, identifier),
             }
