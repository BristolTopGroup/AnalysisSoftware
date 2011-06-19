from ROOT import *

from HistPlotter import inclusiveBjetBins, exclusiveBjetBins, allBjetBins
from HistGetter import getHistsFromFiles,addSampleSum

def doFit(histogram, function, fitRange):#, constrainFit = False):
    numberOfFreeParameters = -1
    fit = None
    
    histogram.Fit( function, "Q0", "ah", fitRange[0], fitRange[1] )
    #fit = histogram.GetFunction( function )
        
    #else:
     #   ff = TF1( function, function, 0, 1 );
      #  numberOfFreeParameters = ff.GetNumberFreeParameters();
    return histogram.GetFunction( function )
    #return {'fit': fit, 'freeParameters':numberOfFreeParameters}

def getEstimate(fit, signalRegion = (0.,0.1), binWidthOfOriginalHIstoram = 0.1, rebinOfOriginalHistogram = 10):
    estimate = fit.Integral( signalRegion[0], signalRegion[1] ) / ( binWidthOfOriginalHIstoram * rebinOfOriginalHistogram )
    return estimate


def getQCDEstimate(histname, datafile):
    files = {'data': datafile}
    histogramForEstimation = 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts'
    
    suffixes = allBjetBins
    
    binWidth = 0.01
    rebin = 10
    function = 'gaus'
    signalRegion = (0.,0.1)
    estimate = -1
    
    for suffix in suffixes:
        if suffix in histname:
            histogramForEstimation = histogramForEstimation + '_' + suffix
            hists = [histogramForEstimation]
            hists = getHistsFromFiles( hists, files )
            histogramForEstimation = hists['data'][histogramForEstimation]
            histogramForEstimation.Rebin(rebin)
            fit1 = doFit(histogramForEstimation, function, ( 0.1, 1.1 ))
            if fit1:
                fit1 = fit1.Clone()
            fit2 = doFit(histogramForEstimation, function, ( 0.2, 1.1 ))
            if fit2:
                fit2 = fit2.Clone()
            
            if not fit1 or not fit2:
                print 'no fit result'
                continue
        
            est1 = getEstimate(fit1, signalRegion, binWidth, rebin)
            est2 = getEstimate(fit2, signalRegion, binWidth, rebin)
        
            estimate = (est1 + est2)/2
    return estimate
    
    
    
if __name__ == '__main__':
    gROOT.SetBatch( True )
    gROOT.ProcessLine( 'gErrorIgnoreLevel = 1001;' )
    files = {
    'data':"/storage/results/histogramFiles/ElectronHad_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'bce1' :  "/storage/results/histogramFiles/QCD_Pt-20to30_BCtoE_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'bce2' :  "/storage/results/histogramFiles/QCD_Pt-30to80_BCtoE_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'bce3' :  "/storage/results/histogramFiles/QCD_Pt-80to170_BCtoE_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'enri1' : "/storage/results/histogramFiles/QCD_Pt-20to30_EMEnriched_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'enri2' :  "/storage/results/histogramFiles/QCD_Pt-30to80_EMEnriched_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'enri3' :  "/storage/results/histogramFiles/QCD_Pt-80to170_EMEnriched_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'pj1' :  "/storage/results/histogramFiles/GJets_TuneD6T_HT-100To200_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'pj2' :  "/storage/results/histogramFiles/GJets_TuneD6T_HT-100To200_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'pj3' :  "/storage/results/histogramFiles/GJets_TuneD6T_HT-200_715.09pb_PFElectron_PF2PATJets_PFMET.root",
             }

    hists = ['QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts']
    
    suffixes = allBjetBins
    hists = [hist + '_' + suffix for hist in hists for suffix in suffixes]
    
    hists = getHistsFromFiles( hists, files )
    hists = addSampleSum(hists)
    binWidth = 0.01
    rebin = 10
    function = 'pol1'
    signalRegion = (0.,0.1)
    
    print 'QCD'
    for name,hist in hists['qcd'].iteritems():
        print name
        hist.Rebin(rebin)
        print 'signal bin', hist.GetBinContent(1)
    
    print 'DATA', function
    for name,hist in hists['data'].iteritems():
        print name
        hist.Rebin(rebin)
        print 'signal bin', hist.GetBinContent(1)
        fit1 = doFit(hist, function, ( 0.1, 1.1 ))
        if fit1:
            fit1 = fit1.Clone()
        fit2 = doFit(hist, function, ( 0.2, 1.1 ))
        if fit2:
            fit2 = fit2.Clone()
            
        if not fit1 or not fit2:
            print 'no fit result'
            continue
        
        est1 = getEstimate(fit1, signalRegion, binWidth, rebin)
        est2 = getEstimate(fit2, signalRegion, binWidth, rebin)
        
        estimate = (est1 + est2)/2
        print 'QCD estimate', round(estimate,2)