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
    
    estimate = estimate * (1-relIsoBias)
    absoluteError = absoluteError * (1-relIsoBias)
    absoluteError = sqrt(absoluteError ** 2 + (estimate * relIsoBias) ** 2)
    return estimate, absoluteError

def relIsoMethodWithSystematics(histogramForEstimation=defaultHistogram, function='expo',
                   fitRange=(0.3, 1.6), fitRangesForSystematics=[(0.2, 1.6), (0.4, 1.6)]):
    
    
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
    
    return centralEstimate, centralAbsoluteError

def relIsoMethod(histogramForEstimation, function='expo',
                   fitRange=(0.3, 1.6), signalRegion = (0., 0.1)):
    
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
                relativeErrorSquared += (err / par) **2
                
    return estimate, sqrt(relativeErrorSquared)*estimate
    
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
    
def doPerformanceStudyOnMCOnly(inputFiles, histogramForEstimation=defaultHistogram, functions=['expo', 'pol1', 'landau', 'gaus'],
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
    for function in functions:
        getRelIsoCalibrationCurve(inputFiles, histogramForEstimation, function, fitRanges)
        for fitRange in fitRanges:
            #take all other fit ranges as systematics
            fitRangesForSystematics = fitRanges
            fitRangesForSystematics.remove(fitRange)
            value, error = relIsoMethodWithSystematics(histogramForEstimation, function, fitRange, fitRangesForSystematics)
    
if __name__ == '__main__':
    print estimateQCDWithRelIso(FILES.files['ElectronHad'])
#    doPerformanceStudyOnMCOnly(inputFiles, histogramForEstimation, functions, fitRanges)
    pass
