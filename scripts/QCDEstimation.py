from ROOT import *
from math import fsum

from HistPlotter import inclusiveBjetBins, exclusiveBjetBins, allBjetBins, rebin
from HistGetter import getHistsFromFiles, addSampleSum
import inputFiles

ufloatEnabled = True
try:
    from uncertainties import ufloat
    from uncertainties import umath
except:
    print "Could not find uncertainties package, please install for full functionality"
    print 'http://packages.python.org/uncertainties/'
    ufloatEnabled = False
    

fitRangesClosureTest = [ (0.1, 0.9), (0.1, 1.0), (0.1, 1.1),
                  (0.2, 0.9), (0.2, 1.0), (0.2, 1.1),
                  (0.3, 0.9), (0.3, 1.0), (0.3, 1.1)]

def doFit(histogram, function, fitRange):#, constrainFit = False):
    numberOfFreeParameters = -1
    fit = None
    
    histogram.Fit(function, "Q0", "ah", fitRange[0], fitRange[1])
    #fit = histogram.GetFunction( function )
        
    #else:
     #   ff = TF1( function, function, 0, 1 );
      #  numberOfFreeParameters = ff.GetNumberFreeParameters();
    return histogram.GetFunction(function)
    #return {'fit': fit, 'freeParameters':numberOfFreeParameters}

def getEstimate(fit, signalRegion=(0., 0.1), binWidthOfOriginalHIstoram=0.1, rebinOfOriginalHistogram=10):
    estimate = fit.Integral(signalRegion[0], signalRegion[1]) / (binWidthOfOriginalHIstoram * rebinOfOriginalHistogram)
    return estimate

def getError(estimates):
    numberOfEstimates = len(estimates)
    if numberOfEstimates == 0:
        return 0
    
    estimate = fsum(estimates)
    estimateSquared = fsum([est * est for est in estimates])
    mean = estimate / numberOfEstimates
    meanSquared = estimateSquared / numberOfEstimates
    error = sqrt((meanSquared - mean * mean) / numberOfEstimates)
    return error

def estimateQCDFor(bjetBin, datafile, histogramForEstimation='QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts', function='pol1',
                   fitRanges=[(0.2, 1.1), (0.3, 1.1)]):
    files = {'data': datafile}
#    fitRanges = [(0.2, 1.1), (0.3, 1.1)]
    
    if bjetBin:
        histogramForEstimation = histogramForEstimation + '_' + bjetBin
        
    hists = [histogramForEstimation]
    hists = getHistsFromFiles(hists, files)
    histogramForEstimation = hists['data'][histogramForEstimation]
#    histogramForEstimation.Rebin(rebin)
    return estimateQCDFrom(histogramForEstimation, function, fitRanges)

def estimateQCDFrom(histogramForEstimation, function='pol1', fitRanges=[(0.2, 1.1), (0.3, 1.1)]):
    histogramForEstimation = histogramForEstimation.Clone('tmp')
#    fitRanges = [(0.2, 1.1), (0.3, 1.1)]
    
    binWidth = 0.01
    rebin = 10
    signalRegion = (0., 0.1)
    estimate = 0
    estimate2 = 0
    relFitError = 0
    histogramForEstimation.Rebin(rebin)
    for fitrange in fitRanges:
        fit = None
        fit = doFit(histogramForEstimation, function, fitrange)
        est = getEstimate(fit, signalRegion, binWidth, rebin)
#        err = fit.GetParErrors()[0]
#        fit.Print()
#        print 'Lenght', len(fit.GetParErrors())
        
        for parErr in range(0, fit.GetNumberFreeParameters() - 1):
            par = fit.GetParameter(parErr)
            err = fit.GetParError(parErr)
#            print par, '+-', err
#            err2 = ufloat((fit.GetParameter(parErr), fit.GetParError(parErr)))
            relFitError += (err / par) ** 2
#            print 'ParErrors (',fitrange, ',', parErr, ')', fit.GetParErrors()[parErr]
#            print err2
#        est = ufloat((est, err))
        estimate += est
        estimate2 += est * est

    mean = estimate / len(fitRanges)
    mean2 = estimate2 / len(fitRanges)
    error = 0
    if not (mean2 - mean * mean) == 0:
        error = sqrt((mean2 - mean * mean) / 2)
#        error = sqrt(error*error + mean.std_dev()*mean.std_dev())
    
#    if ufloatEnabled:
#        return ufloat((mean, error))
#    else:
#    mean = float(mean.nominal_value)
#    print 'Fit error:', fitError.std_dev()/fitError.nominal_value
    print 'Rel. fit error:', sqrt(relFitError)
    print 'Root mean square: ', error/mean
    print 'TotalError', sqrt(relFitError + (error/mean)**2 )
    return (mean, sqrt(relFitError + (error / mean) ** 2) * mean) 
    
def getQCDEstimate(histname, datafile, histogramForEstimation='QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts'):
    files = {'data': datafile}
    
    
    binWidth = 0.01
    rebin = 10
    function = 'gaus'
    signalRegion = (0., 0.1)
    estimate = -1
    
    for suffix in allBjetBins:
        if suffix in histname:
            histogramForEstimation = histogramForEstimation + '_' + suffix
            hists = [histogramForEstimation]
            hists = getHistsFromFiles(hists, files)
            histogramForEstimation = hists['data'][histogramForEstimation]
            histogramForEstimation.Rebin(rebin)
            fit1 = None
            fit2 = None
            if histogramForEstimation.Integral() > 0:
                fit1 = doFit(histogramForEstimation, function, (0.2, 1.1))
                if fit1:
                    fit1 = fit1.Clone()
                fit2 = doFit(histogramForEstimation, function, (0.3, 1.1))
                if fit2:
                    fit2 = fit2.Clone()
            
                if not fit1 or not fit2:
                    print 'No fit result, returning 0'
                    return 0
            
            est1 = getEstimate(fit1, signalRegion, binWidth, rebin)
            est2 = getEstimate(fit2, signalRegion, binWidth, rebin)
        
            estimate = (est1 + est2) / 2
            del hists
            del fit1, fit2, histogramForEstimation
    return estimate
    
def doClosureTestFor(files, histogram, function='pol1', fitRanges=[(0.2, 1.1), (0.3, 1.1)]):
    hists = [histogram]
    hists = getHistsFromFiles(hists, files)
    hists = addSampleSum(hists)
    estimate = estimateQCDFrom(hists['allMC'][histogram], function, fitRanges)[0]
    hists = rebin(hists, 10, histogram)
#    print 'Estimate:', estimate
    signalBin = hists['qcd'][histogram].GetXaxis().FindBin(0.1)
#    print signalBin
#    print 'Integral', hists['qcd'][histogram].Integral(0, 1)
    
    totalQCD = 0
#    values = []
    for bin in range(1, signalBin):
#        error = hists['qcd'][histogram].GetBinError(bin)
        value = hists['qcd'][histogram].GetBinContent(bin)
        totalQCD += value
#        values.append(ufloat((value, error)))
    
#    total = sum(values)
    total = totalQCD
#    print 'total:', total
#    print 'total QCD:', totalQCD
    
    
    syst = (total - estimate) / total
#    print estimate*syst
#    print 'difference', (total-estimate)/total*100
    return (total - estimate) / total
    
    
def getShapeErrorHistogram(histname, files):
    files = {'data': files['data']}
    histogramForShape = 'topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets'
    histogramForComparison = 'topReconstruction/backgroundShape/mttbar_antiIsolated_withMETAndAsymJets'
    suffixes = allBjetBins
    rebin = 50

    errors = None
        
    for suffix in suffixes:
        if suffix in histname:
            histogramForShape = histogramForShape + '_' + suffix
            histogramForComparison = histogramForComparison + '_' + suffix
            hists = [histogramForShape, histogramForComparison]
            hists = getHistsFromFiles(hists, files)
            histogramForShape = hists['data'][histogramForShape]
            histogramForComparison = hists['data'][histogramForComparison]
            histogramForShape.Sumw2()
            histogramForComparison.Sumw2()
            
            histogramForShape.Rebin(rebin)
            histogramForComparison.Rebin(rebin)
            
            nShape = histogramForShape.Integral()
            nCompare = histogramForComparison.Integral()
            
            if nShape > 0 and nCompare > 0:
                histogramForShape.Scale(1 / nShape)
                histogramForComparison.Scale(1 / nCompare)
            
            errors = histogramForShape.Clone('ShapeErrors')
            errors.Add(histogramForComparison, -1)#subtraction
            for bin in range(1, errors.GetNbinsX()):
                errors.SetBinContent(bin, fabs(errors.GetBinContent(bin)))
            errors.Divide(histogramForShape)
            
    return errors   
    
def combineErrorsFromHistogramList():
    pass
if __name__ == '__main__':
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 1001;')
    
    files = inputFiles.files
    function = 'gaus'
    btag = '0btags'
    fitRanges = [
#                 (0.1, 1.1),
#                 (0.2, 1.1),
                 (0.3, 1.1)
                 ]
    estimate= estimateQCDFor('', files['data'], 'QCDStudy/QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts_3jets', function, fitRanges)
    est, err = estimate
    err = err / est
    print 'Final QCD estimate (3jet, 1btag)', (est, err * est + est / sqrt(est))
#    estimate= estimateQCDFor('', files['data'], 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_0btag')
    for btag in ['0btag', '1btag', '2btags']:
        estimate = estimateQCDFor('', files['data'], 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_%s' % btag, function, fitRanges)
        est, err = estimate
        err = err / est
        print 'Final QCD estimate (>=4jet, %s)' %btag, (est, err * est + est / sqrt(est))
#    estimate= estimateQCDFor('', files['data'], 'QCDStudy/QCDest_PFIsolation_WithMETCutAndAsymJetCuts_4orMoreJets')
#    print 'QCD estimate:', estimate
    
#    print 'performing closure test'
#    sysErr = doClosureTestFor(files, 'QCDStudy/QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts_3jets', function, fitRanges)
#    sysErr = doClosureTestFor(files, 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_%s' % btag, function, fitRanges)
#    sysErr = 0
#    
#    err = err / est
#    err = sqrt(err * err + sysErr * sysErr)
#    print 'Final QCD estimate', (est, err * est + est / sqrt(est))

#    hists = ['QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts']
#    
#    suffixes = allBjetBins
#    hists = [hist + '_' + suffix for hist in hists for suffix in suffixes]
#    
#    hists = getHistsFromFiles(hists, files)
#    hists = addSampleSum(hists)
#    binWidth = 0.01
#    rebin = 10
#    function = 'gaus'
#    signalRegion = (0., 0.1)
#    
#    print 'QCD'
#    for name, hist in hists['qcd'].iteritems():
#        print name
#        hist.Rebin(rebin)
#        print 'signal bin', hist.GetBinContent(1)
#    
#    print 'DATA', function
#    for name, hist in hists['data'].iteritems():
#        print name
#        hist.Rebin(rebin)
#        print 'signal bin', hist.GetBinContent(1)
#        fit1 = doFit(hist, function, (0.1, 1.1))
#        if fit1:
#            fit1 = fit1.Clone()
#        fit2 = doFit(hist, function, (0.2, 1.1))
#        if fit2:
#            fit2 = fit2.Clone()
#            
#        if not fit1 or not fit2:
#            print 'no fit result'
#            continue
#        
#        est1 = getEstimate(fit1, signalRegion, binWidth, rebin)
#        est2 = getEstimate(fit2, signalRegion, binWidth, rebin)
#        
#        estimate = (est1 + est2) / 2
#        print 'QCD estimate', round(estimate, 2), '+-', round(getError([est1, est2]), 2)
