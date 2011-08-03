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
    histogram = histogram.Clone('fitting')
    numberOfFreeParameters = -1
    fit = None
    
    histogram.Fit(function, "Q0", "ah", fitRange[0], fitRange[1])
    fit = histogram.GetFunction(function)
    if fit:
        return fit.Clone()
    else:
        return None

#def getEstimate(fit, signalRegion=(0., 0.1), binWidthOfOriginalHIstoram=0.1, rebinOfOriginalHistogram=10):
#    estimate = fit.Integral(signalRegion[0], signalRegion[1]) / (binWidthOfOriginalHIstoram * rebinOfOriginalHistogram)
#    return estimate

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

def estimateQCDFor(bjetBin, datafile, histogramForEstimation='QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts', function='expo',
                   fitRanges=[(0.2, 1.6), (0.3, 1.6), (0.4, 1.6)]):
    files = {'data': datafile}
#    fitRanges = [(0.2, 1.1), (0.3, 1.1)]
    
    if bjetBin:
        histogramForEstimation = histogramForEstimation + '_' + bjetBin
        
    hists = [histogramForEstimation]
    hists = getHistsFromFiles(hists, files)
    histogramForEstimation = hists['data'][histogramForEstimation]
    estimate, error = estimateQCDFrom(histogramForEstimation, function, fitRanges)
    del histogramForEstimation 
    del hists
    del function
    return estimate, error


def estimateQCDFrom(histogramForEstimation, function='expo',
                   fitRanges=[(0.2, 1.6), (0.3, 1.6), (0.4, 1.6)]):
    
    histogramForEstimation = histogramForEstimation.Clone('tmp')
    
    binWidthOfOriginalHIstoram = 0.01
    rebinOfOriginalHistogram = 1
    signalRegion = (0., 0.1)
    estimate = 0
    estimate2 = 0
    relFitError = 0
    histogramForEstimation.Rebin(rebinOfOriginalHistogram)
    for fitrange in fitRanges:
        fit = None
        fit = doFit(histogramForEstimation, function, fitrange)
        if fit:
            est = fit.Integral(signalRegion[0], signalRegion[1]) / (binWidthOfOriginalHIstoram * rebinOfOriginalHistogram)
            for parErr in range(0, fit.GetNumberFreeParameters()):
                par = fit.GetParameter(parErr)
                err = fit.GetParError(parErr)
                if not par == 0:
                    relFitError += (err / par) ** 2
#            print par, err
            estimate += est
            estimate2 += est * est
    

    mean = estimate / len(fitRanges)
    mean2 = estimate2 / len(fitRanges)
    error = 0
    if not (mean2 - mean * mean) == 0:
        error = sqrt((mean2 - mean * mean) / 2)
    if not mean == 0:
        return (mean, sqrt(relFitError + (error / mean) ** 2) * mean)
    else:
        return (0,0) 
 
def getQCDEstimate(datafile, histogramForEstimation='QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts', bjetBin = '', function='expo', 
                   fitRange = (0.3, 1.6), additionFitRanges=[(0.2, 1.6), (0.4, 1.6)]):    
    estimate, absoluteError = estimateQCDFor(bjetBin, datafile, histogramForEstimation, function, fitRanges=[fitRange])
    relativeError = 0
    if not estimate == 0:
        relativeError = absoluteError/estimate
    relativeErrorSquared = relativeError**2
    
    systematicErrorFromOtherFitRanges = 0
#    print 'Abs err', absoluteError
    for currentRange in additionFitRanges:
        est, err = estimateQCDFor(bjetBin, datafile, histogramForEstimation, function, fitRanges=[currentRange])
        deviation = est - estimate
        if not estimate == 0:
            relativeErrorSquared += (deviation/estimate)**2
#        print 'Est', est
    statisticalErrorSquared = 0
    if not estimate == 0:
        statisticalErrorSquared = 1/estimate
    relativeErrorSquared += statisticalErrorSquared
    
    relativeError = sqrt(relativeErrorSquared)
    absoluteError = relativeError*estimate
    
    
    
    return estimate, absoluteError
    
    
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

def createErrorHistogram(mcHistograms, qcdHistogram,  relativeQCDEstimationError, shapeErrorHistogram):
    errorHist = qcdHistogram.Clone("ErrorHist")
    
    for bin in range(1, errorHist.GetNbinsX()):
        nQCD = qcdHistogram.GetBinContent(bin)
        nMC = 0
        for hist in mcHistograms:
            nMC += hist.GetBinContent(bin)
        err = relativeQCDEstimationError
        if shapeErrorHistogram:
            shapeErr= shapeErrorHistogram.GetBinContent(bin)
            shapeErrStat = shapeErrorHistogram.GetBinError(bin)
            shapeErr = fabs(shapeErr) + shapeErrStat
            err = sqrt(err*err + shapeErr*shapeErr)
        errorHist.SetBinContent(bin, nMC)
        errorHist.SetBinError(bin, err*nQCD)
    return errorHist
    
if __name__ == '__main__':
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 1001;')
    
    files = inputFiles.files
    function = 'gaus'
#    btag = '0btags'
#    fitRanges = [
##                 (0.1, 1.1),
##                 (0.2, 1.1),
##                 (0.3, 1.1),
#                 (0.4, 1.1)
#                 ]

    print 'QCD estimation in relative isolation using', function, 'function'
##    est1 = estimateQCDFor('', files['data'], 'QCDStudy/QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts_3jets', function, [(0.2, 1.6)])[0]
##    est, err = estimateQCDFor('', files['data'], 'QCDStudy/QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts_3jets', function, [(0.3, 1.6)])
##    est2 = estimateQCDFor('', files['data'], 'QCDStudy/QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts_3jets' , function, [(0.4, 1.6)])[0]
##    print 'Err', err
##    print 'Est1', est1
##    print 'Est2', est2
##    err = err / est
##    err2 = err*err + ((est - est1)/est)**2 + ((est - est2)/est)**2 +  1/ est
##    print 'Final QCD estimate (==3jet, %s): %.1f +- %.1f' % ('1 b-tag', est, sqrt(err2)*est)
#    est, err = getQCDEstimate(files['data'], histogramForEstimation='QCDStudy/QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts_3jets', function=function, 
#                   fitRange = (0.3, 1.6), additionFitRanges=[(0.2, 1.6), (0.4, 1.6)])
#    print 'Final QCD estimate (==3jet, %s): %.1f +- %.1f' % ('1 b-tag', est, err)
#    for btag in ['0btag', '1btag', '2btags']:
#        
##        est1 = estimateQCDFor('', files['data'], 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_%s' % btag, function, [(0.2, 1.6)])[0]
##        est, err = estimateQCDFor('', files['data'], 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_%s' % btag, function, [(0.3, 1.6)])
##        est2 = estimateQCDFor('', files['data'], 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_%s' % btag, function, [(0.4, 1.6)])[0]
##        print 'Err', err
##        print 'Est1', est1
##        print 'Est2', est2
##        err = err / est
##        err2 = err*err + ((est - est1)/est)**2 + ((est - est2)/est)**2 +  1/ est
##        print 'Final QCD estimate (>=4jet, %s): %.1f +- %.1f' % (btag, est, sqrt(err2)*est)
#        est, err = getQCDEstimate(files['data'], histogramForEstimation='QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_%s' % btag, function=function, 
#                   fitRange = (0.3, 1.6), additionFitRanges=[(0.2, 1.6), (0.4, 1.6)])
#        print 'Final QCD estimate (>=4jet, %s): %.1f +- %.1f' % (btag, est, err)
        
    for i in range(0,10):
        print i, estimateQCDFor('', files['data'], 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_%s' % '1btag', function, [(0.2, 1.6)])
        
#    print 'performing closure test'
#    est1 = doClosureTestFor(files, 'QCDStudy/QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts_3jets', function, [(0.2, 1.6)])
#    estimate = doClosureTestFor(files, 'QCDStudy/QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts_3jets', function, [(0.3, 1.6)])
#    est2 = doClosureTestFor(files, 'QCDStudy/QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts_3jets' , function, [(0.4, 1.6)])
#    est =1
#    err = estimate
#    err = err / est
#    print '%.3f, %.3f, %.3f' % (err, est1, est2)
##    err2 = err*err + ((est - est1)/est)**2 + ((est - est2)/est)**2 +  1/ est
##    print 'Final QCD estimate (>=4jet, %s): %.2f +- %.2f' % (btag, est, sqrt(err2)*est)
##    print 'Final QCD estimate (3jet, 1btag)', (est, err * est + est / sqrt(est))
##    estimate= estimateQCDFor('', files['data'], 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_0btag')
#    for btag in ['0btag', '1btag', '2btags']:
#        est1 = doClosureTestFor(files, 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_%s' % btag, function, [(0.2, 1.6)])
#        estimate = doClosureTestFor(files, 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_%s' % btag, function, [(0.3, 1.6)])
#        est2 = doClosureTestFor(files, 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_%s' % btag, function, [(0.4, 1.6)])
#        est =1
#        err = estimate
#        print '%.3f, %.3f, %.3f' % (err, est1, est2)
#        est, err = estimate
#        err = err / est
#        err2 = err*err + ((est - est1)/est)**2 + ((est - est2)/est)**2 +  1/ est
#        print 'Final QCD estimate (>=4jet, %s): %.2f +- %.2f' % (btag, est, sqrt(err2)*est)
#        print 'Final QCD estimate (>=4jet, %s)' %btag, (est, err * est + est / sqrt(est))
        
    
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
