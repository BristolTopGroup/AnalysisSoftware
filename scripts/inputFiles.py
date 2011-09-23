lumi = 1959.75
folder = '/storage/results/histogramFiles/PAS3'
folder = '/storage/results/histogramFiles/NewCode'


files = {
    'data':"%s/data_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
#    'data': '/storage/results/histogramFiles/CiCElectron ID/data_1611.95pb_PFElectron_PF2PATJets_PFMET.root',
#'data': '/storage/results/histogramFiles/CiCElectron ID/data_1959.75pb_PFElectron_PF2PATJets_PFMET.root',
    'ttbar' : "%s/TTJet_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'wjets' : "%s/WJetsToLNu_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'zjets' : "%s/DYJetsToLL_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'bce1' : "%s/QCD_Pt-20to30_BCtoE_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'bce2' : "%s/QCD_Pt-30to80_BCtoE_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'bce3' : "%s/QCD_Pt-80to170_BCtoE_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'enri1' : "%s/QCD_Pt-20to30_EMEnriched_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'enri2' : "%s/QCD_Pt-30to80_EMEnriched_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'enri3' : "%s/QCD_Pt-80to170_EMEnriched_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'pj1' : "%s/GJets_TuneD6T_HT-40To100_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'pj2' : "%s/GJets_TuneD6T_HT-100To200_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'pj3' : "%s/GJets_TuneD6T_HT-200_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'ww' : "%s/WWtoAnything_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'wz' : "%s/WZtoAnything_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'zz' : "%s/ZZtoAnything_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'T_TuneZ2_tW-channel' : "%s/T_TuneZ2_tW-channel_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'T_TuneZ2_t-channel' : "%s/T_TuneZ2_t-channel_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'T_TuneZ2_s-channel' : "%s/T_TuneZ2_s-channel_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'Tbar_TuneZ2_tW-channel' : "%s/Tbar_TuneZ2_tW-channel_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'Tbar_TuneZ2_t-channel' : "%s/Tbar_TuneZ2_t-channel_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
    'Tbar_TuneZ2_s-channel' : "%s/Tbar_TuneZ2_s-channel_%.2fpb_PFElectron_PF2PATJets_PFMET.root" %(folder, lumi),
             }