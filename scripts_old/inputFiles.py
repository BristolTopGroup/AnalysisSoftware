lumi = 4692.36
folder = '/storage/results/histogramfiles'
#folder = '/scratch/results/histogramFiles/NewCode'
identifier = 'PFElectron_PFMuon_PF2PATJets_PFMET'

files = {
    'data':"%s/MuHad_%.2fpb_%s.root" % (folder, lumi, identifier),
#    'data':"%s/ElectronHad_%.2fpb_%s.root" % (folder, lumi, identifier),
    'ttbar' : "%s/TTJet_%.2fpb_%s.root" %(folder, lumi, identifier),
    'wjets' : "%s/WJetsToLNu_%.2fpb_%s.root" %(folder, lumi, identifier),
    'zjets' : "%s/DYJetsToLL_%.2fpb_%s.root" %(folder, lumi, identifier),
    'bce1' : "%s/QCD_Pt-20to30_BCtoE_%.2fpb_%s.root" %(folder, lumi, identifier),
    'bce2' : "%s/QCD_Pt-30to80_BCtoE_%.2fpb_%s.root" %(folder, lumi, identifier),
    'bce3' : "%s/QCD_Pt-80to170_BCtoE_%.2fpb_%s.root" %(folder, lumi, identifier),
    'muQCD' : "%s/QCD_Pt-20_MuEnrichedPt-15_%.2fpb_%s.root" %(folder, lumi, identifier),
    
#    'enri1' : "%s/QCD_Pt-20to30_EMEnriched_%.2fpb_%s.root" %(folder, lumi, identifier),
    'enri2' : "%s/QCD_Pt-30to80_EMEnriched_%.2fpb_%s.root" %(folder, lumi, identifier),
    'enri3' : "%s/QCD_Pt-80to170_EMEnriched_%.2fpb_%s.root" %(folder, lumi, identifier),
    'pj1' : "%s/GJets_TuneD6T_HT-40To100_%.2fpb_%s.root" %(folder, lumi, identifier),
    'pj2' : "%s/GJets_TuneD6T_HT-100To200_%.2fpb_%s.root" %(folder, lumi, identifier),
    'pj3' : "%s/GJets_TuneD6T_HT-200_%.2fpb_%s.root" %(folder, lumi, identifier),
    'ww' : "%s/WWtoAnything_%.2fpb_%s.root" %(folder, lumi, identifier),
    'wz' : "%s/WZtoAnything_%.2fpb_%s.root" %(folder, lumi, identifier),
    'zz' : "%s/ZZtoAnything_%.2fpb_%s.root" %(folder, lumi, identifier),
    'T_TuneZ2_tW-channel' : "%s/T_TuneZ2_tW-channel_%.2fpb_%s.root" %(folder, lumi, identifier),
    'T_TuneZ2_t-channel' : "%s/T_TuneZ2_t-channel_%.2fpb_%s.root" %(folder, lumi, identifier),
    'T_TuneZ2_s-channel' : "%s/T_TuneZ2_s-channel_%.2fpb_%s.root" %(folder, lumi, identifier),
    'Tbar_TuneZ2_tW-channel' : "%s/Tbar_TuneZ2_tW-channel_%.2fpb_%s.root" %(folder, lumi, identifier),
    'Tbar_TuneZ2_t-channel' : "%s/Tbar_TuneZ2_t-channel_%.2fpb_%s.root" %(folder, lumi, identifier),
    'Tbar_TuneZ2_s-channel' : "%s/Tbar_TuneZ2_s-channel_%.2fpb_%s.root" %(folder, lumi, identifier),
             }
