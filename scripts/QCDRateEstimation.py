'''
Created on Nov 23, 2011

@author: Lukasz Kreczko

Email: Lukasz.Kreczko@cern.ch

Different methods are available:
- ABCD method (takes 2D histogram)
- Matrix method (takes 1D histogram)
- RelISo method (takes 1D histogram)

Output consists of an estimated number of events and error
'''
from __future__ import division
from math import sqrt
import tools.ROOTFileReader as FileReader
import tools.PlottingUtilities as plotting
import FILES
try:
    from uncertainties import ufloat
    from uncertainties import umath
except:
    print "Could not find uncertainties package, please install for full functionality"
    print 'http://packages.python.org/uncertainties/'
    ufloatEnabled = False
from ROOT import Double

#relIso method is overestimating by 45%
defaultHistogram = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/QCD e+jets PFRelIso/Electron/electron_pfIsolation_03_0orMoreBtag'
relIsoBias = 0

def estimateQCDWithRelIso(inputFile, histogramForEstimation=defaultHistogram, function='expo',
                   fitRange=(0.3, 1.6), fitRangesForSystematics=[(0.2, 1.6), (0.4, 1.6)]):
    print '*' * 120
    print "Estimating QCD using a fit to RelIso"
    print 'Input file = ', inputFile
    print 'Histogram = ', histogramForEstimation
    print 'Fit function = ', function
    print 'Fit range = ', fitRange
    print 'Fit ranges for systematics = ', fitRangesForSystematics
    print '*' * 120
    histogramForEstimation = FileReader.getHistogramFromFile(histogramForEstimation, inputFile)
    estimate, absoluteError = relIsoMethodWithSystematics(histogramForEstimation, function, fitRange=fitRange, fitRangesForSystematics=fitRangesForSystematics)
    
    estimate = estimate * (1 - relIsoBias)
    absoluteError = absoluteError * (1 - relIsoBias)
    absoluteError = sqrt(absoluteError ** 2 + (estimate * relIsoBias) ** 2)
    return estimate, absoluteError

def relIsoMethodWithSystematics(histogramForEstimation=defaultHistogram, function='expo',
                   fitRange=(0.3, 1.6), fitRangesForSystematics=[(0.2, 1.6), (0.4, 1.6)], applyBiasCorrection = True):
     
    
    centralEstimate, centralAbsoluteError = relIsoMethod(histogramForEstimation, function, fitRange=fitRange)
    
    centralRelativeError = 0
    if not centralEstimate == 0:
        centralRelativeError = centralAbsoluteError / centralEstimate
    centralRelativeErrorSquared = centralRelativeError ** 2
    
    systematicErrorFromOtherFitRangesSquared = 0
    for currentRange in fitRangesForSystematics:
        currentEstimate, err = relIsoMethod(histogramForEstimation, function, fitRange=currentRange)
        deviation = currentEstimate - centralEstimate
        if not centralEstimate == 0:
            systematicErrorFromOtherFitRangesSquared += (deviation / centralEstimate) ** 2
            
    centralRelativeErrorSquared += systematicErrorFromOtherFitRangesSquared

    centralRelativeError = sqrt(centralRelativeErrorSquared)
    centralAbsoluteError = centralRelativeError * centralEstimate
    
    if applyBiasCorrection:
        bias = 0
        reductionFromBias = 1 - bias
        centralEstimate *= reductionFromBias
        centralAbsoluteError = sqrt(centralAbsoluteError ** 2 + (centralEstimate * bias) ** 2)
    
    return centralEstimate, centralAbsoluteError

def relIsoMethod(histogramForEstimation, function='expo',
                   fitRange=(0.3, 1.6), signalRegion=(0., 0.1)):
    
    histogramForEstimation = histogramForEstimation.Clone('tmp')
    
    #investigate them
    binWidthOfOriginalHistoram = 0.01
    rebinOfOriginalHistogram = 10

    estimate = 0
    relativeErrorSquared = 0
    histogramForEstimation.Rebin(rebinOfOriginalHistogram)

    fit = None
    fit = performFit(histogramForEstimation, function, fitRange)
    if fit:
        estimate = fit.Integral(signalRegion[0], signalRegion[1]) / (binWidthOfOriginalHistoram * rebinOfOriginalHistogram)
        for parErr in range(0, fit.GetNumberFreeParameters()):
            par = fit.GetParameter(parErr)
            err = fit.GetParError(parErr)
            if not par == 0:
                relativeErrorSquared += (err / par) ** 2
                
    return estimate, sqrt(relativeErrorSquared) * estimate
    
def performFit(histogram, function, fitRange):
    histogram = histogram.Clone('fitting')
    numberOfFreeParameters = -1
    fit = None
    
    histogram.Fit(function, "Q0", "ah", fitRange[0], fitRange[1])
    fit = histogram.GetFunction(function)
    if fit:
        return fit.Clone()
    else:
        return None
    
#Estimate the bias on MC only
def getRelIsoCalibrationCurve(inputFiles, histogramForEstimation=defaultHistogram, function='expo',
                   fitRanges=[(0.2, 1.6), (0.3, 1.6), (0.4, 1.6)]):
    print '*' * 120
    print "Estimating QCD using a fit to RelIso"
    print 'Input files = ', inputFiles
    print 'Histogram = ', histogramForEstimation
    print 'Fit function = ', function
    print 'Fit ranges = ', fitRanges
    print '*' * 120
    #get histograms
    #instead of data use sum MC
    
def doPerformanceStudyOnMCOnly(inputFiles,
                               histogramForEstimation=defaultHistogram,
                               function='expo',
                   fitRanges=[(0.2, 1.6), (0.3, 1.6), (0.4, 1.6)]):
#    print '*' * 120
#    print "Estimating QCD using a fit to RelIso"
##    print 'Input files = ', inputFiles
#    print 'Histogram = ', histogramForEstimation
#    print 'Fit functions = ', function
#    print 'Fit ranges = ', fitRanges
#    print '*' * 120
    #get histograms
    histograms = FileReader.getHistogramDictionary(histogramForEstimation, inputFiles)
    allMC = ['TTJet', 'DYJetsToLL', 'QCD_Pt-20to30_BCtoE', 'QCD_Pt-30to80_BCtoE',
                 'QCD_Pt-80to170_BCtoE', 'QCD_Pt-20to30_EMEnriched', 'QCD_Pt-30to80_EMEnriched',
                 'QCD_Pt-80to170_EMEnriched', 'GJets_HT-40To100', 'GJets_HT-100To200',
                 'GJets_HT-200', 'WWtoAnything', 'WZtoAnything', 'ZZtoAnything', 'T_tW-channel',
                 'T_t-channel', 'T_s-channel', 'Tbar_tW-channel', 'Tbar_t-channel',
                 'Tbar_s-channel', 'W1Jet', 'W2Jets', 'W3Jets', 'W4Jets'
                 ]
    qcd = ['QCD_Pt-20to30_BCtoE', 'QCD_Pt-30to80_BCtoE',
                 'QCD_Pt-80to170_BCtoE', 'QCD_Pt-20to30_EMEnriched', 'QCD_Pt-30to80_EMEnriched',
                 'QCD_Pt-80to170_EMEnriched', 'GJets_HT-40To100', 'GJets_HT-100To200',
                 'GJets_HT-200']
    histograms['SumMC'] = plotting.sumSamples(histograms, allMC)
    
    histograms['QCD'] = plotting.sumSamples(histograms, qcd)
    
    qcdInSignalRegion = histograms['QCD'].Integral()
    qcdError = 0
    if not qcdInSignalRegion == 0:
        qcdError = qcdInSignalRegion / sqrt(qcdInSignalRegion) 
    import copy
    results = {}
    qcdInSignalRegion, qcdError = getIntegral(histograms['QCD'], (0, 0.1))
#        getRelIsoCalibrationCurve(inputFiles, histogramForEstimation, function, fitRanges)
    for fitRange in fitRanges:
        #take all other fit ranges as systematics
        fitRangesForSystematics = copy.copy(fitRanges)
        fitRangesForSystematics.remove(fitRange)
        #instead of data use sum MC
        estimate, absoluteError = relIsoMethodWithSystematics(histograms['SumMC'], function, fitRange, fitRangesForSystematics, False)
        N_est = ufloat((estimate, absoluteError))
        N_qcd = ufloat((qcdInSignalRegion, qcdError))
        relativeDeviation = N_est / N_qcd

        result = {}
        result['performance'] = (relativeDeviation.nominal_value, relativeDeviation.std_dev())
        result['estimate'] = (estimate, absoluteError)
        result['qcdInSignalRegion'] = (qcdInSignalRegion, qcdError)
        result['fitfunction'] = function
        result['fitRange'] = fitRange
        result['fitRangesForSystematics'] = fitRangesForSystematics
        results[str(fitRange)] = result
    return results
        
def printPerformanceResults(results):     
    print '| *function, range* | *N<sub>QCD, true</sub>* | *N<sub>est</sub>* | *(N<sub>est</sub> - N<sub>QCD, true</sub>)/N<sub>QCD, true</sub>* |'       
    for fitrange, result in results.iteritems():
        N_qcd, N_qcd_error = result['qcdInSignalRegion']
        est, err = result['estimate']
        performance, performanceError = result['performance']
        format = (result['fitfunction'], str(result['fitRange']), N_qcd, N_qcd_error, est, err, performance, performanceError)
        print '| %s, %s |  %.0f +- %.0f |  %.0f +- %.0f |  %.3f +- %.3f |' % format
        
def getIntegral(histogram, integralRange=(0, 0.1)):
    firstBin = histogram.GetXaxis().FindBin(integralRange[0])
    lastBin = histogram.GetXaxis().FindBin(integralRange[1])
    
    integral = 0
    absoluteError = Double(0)
    integral = histogram.IntegralAndError(firstBin, lastBin, absoluteError)
    
    return integral, absoluteError        
        
        
    
if __name__ == '__main__':
#    est, err =  estimateQCDWithRelIso(FILES.files['ElectronHad'])
#    print 'QCD estimate %.0f +- %.0f' % (est,err)
    hist = defaultHistogram
#    hist = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/QCD e+jets PFRelIso/Electron/electron_pfIsolation_03_0btag'
#    hist = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/QCD e+jets PFRelIso/Electron/electron_pfIsolation_03_1btag'
#    hist = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/QCD e+jets PFRelIso/Electron/electron_pfIsolation_03_2orMoreBtags'
    results = doPerformanceStudyOnMCOnly(FILES.files, hist, function='expo')
    printPerformanceResults(results)
