from ROOT import *
from HistGetter import *
from HistPlotter import *

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


def getQCDEstimate(histname):
    files = {'data':"/storage/results/2011/new/data_349.007pb_PFElectron_PF2PATJets_PFMET.root"}
    hist = 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts'
    
    suffixes = ["0orMoreBtag",'0btag',
        "1orMoreBtag",
        "2orMoreBtags",
        "3orMoreBtags",'4orMoreBtags' ]
    
    binWidth = 0.01
    rebin = 10
    function = 'gaus'
    signalRegion = (0.,0.1)
    estimate = 0
    
    for suffix in suffixes:
        if suffix in histname:
            hist = hist + '_' + suffix
            hists = [hist]
            hists = getHistsFromFiles( hists, files )
            hist = hists['data'][hist]
            hist.Rebin(rebin)
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
    return estimate
    
    
    
if __name__ == '__main__':
    gROOT.SetBatch( True )
    gROOT.ProcessLine( 'gErrorIgnoreLevel = 1001;' )
    files = {'data':"/storage/results/2011/new/data_349.007pb_PFElectron_PF2PATJets_PFMET.root"}
    hists = ['QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts']
    
    suffixes = HistPlotter.inclusiveBjetBins
    hists = [hist + '_' + suffix for hist in hists for suffix in suffixes]
    
    hists = getHistsFromFiles( hists, files )
    binWidth = 0.01
    rebin = 10
    function = 'gaus'
    signalRegion = (0.,0.1)
    
    
    
    for name,hist in hists['data'].iteritems():
        print name, hist.GetEntries()
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