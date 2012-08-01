'''
Created on Aug 1, 2012

@author: Lukasz Kreczko

Email: Lukasz.Kreczko@cern.ch
'''

import FILES
import tools.ROOTFileReader as FileReader

samples = [
         'TTJet',
         'POWHEG',
         'PYTHIA6',
         'MCatNLO'
         ]

metbins = [
           '0-25',
               '25-45',
               '45-70',
               '70-100',
               '100-inf'
               ]


if __name__ == "__main__":
    base = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/BinnedMETAnalysis/'
    
    bjetbin = '2orMoreBtags'
    metType = 'PFMET'
    metType = 'patType1CorrectedPFMet'
#    metType = 'patType1p2CorrectedPFMet'
    for sample in samples:
        print 'Sample = ', sample
        for metbin in metbins:
            genMET = base + 'Electron_%s_bin_%s/electron_eta_%s' % ('GenMET', metbin, bjetbin)
            PFMET = base + 'Electron_%s_bin_%s/electron_eta_%s' % (metType, metbin, bjetbin)
            genMETs = FileReader.getHistogramFromFile(genMET, FILES.files[sample])
            PFMETs = FileReader.getHistogramFromFile(PFMET, FILES.files[sample])
            N_gen = genMETs.Integral()
            N_reco = PFMETs.Integral()
            purity = (N_gen + N_reco)/N_reco
            stability = (N_gen + N_reco)/N_gen
            correctionFactor = N_gen/N_reco
            print 'MET bin =', metbin
            print 'N_gen = ', N_gen
            print 'N_reco = ', N_reco
            print 'p_i =', 1/purity
            print 's_i =', 1/stability
            print 'f_C,i =', correctionFactor
    