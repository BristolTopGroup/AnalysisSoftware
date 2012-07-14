#Step 1: get histograms of the distribution you are interested in
#Step 2: Pass the distributions with starting values to TMinuit
#Step 3: ???
#Step 4: Profit
from __future__ import division
from ROOT import *
import ROOT
import FILES
import tools.ROOTFileReader as FileReader
from array import array
from tools import Styles
import tools.PlottingUtilities as plotting
import QCDRateEstimation
from copy import deepcopy
from decimal import *
import numpy
savePath = "/storage/results/plots/DiffMETMeasurement/"

availableSamples = FILES.samplesToLoad
files = FILES.files
lumi = FILES.luminosity

normalisation = None
qcdError = 1 
vectors = None
vector_errors = None
DEBUG = False
unbinnedTTbarEvents = 0
unbinnedZJetsEvents = 0
unbinnedWJetsEvents = 0

unbinnedTTbarEvents_matchingDown = 0
unbinnedTTbarEvents_matchingUp = 0
unbinnedTTbarEvents_scaleDown = 0
unbinnedTTbarEvents_scaleUp = 0

unbinnedWJetsEvents_matchingDown = 0
unbinnedWJetsEvents_matchingUp = 0
unbinnedWJetsEvents_scaleDown = 0
unbinnedWJetsEvents_scaleUp = 0

unbinnedZJetsEvents_matchingDown = 0
unbinnedZJetsEvents_matchingUp = 0
unbinnedZJetsEvents_scaleDown = 0
unbinnedZJetsEvents_scaleUp = 0

lumiScaleFactor = 1
singleTopScaleFactor = 1
ttbarScaleFactor = 1
wjetsScaleFactor = 1
zjetsScaleFactor = 1

fit_index = 0

use_fit_errors_only = False
measure_normalised_crossection = False

qcd_samples = [ 'QCD_Pt-20to30_BCtoE',
                 'QCD_Pt-30to80_BCtoE',
                 'QCD_Pt-80to170_BCtoE',
                 'QCD_Pt-20to30_EMEnriched',
                 'QCD_Pt-30to80_EMEnriched',
                 'QCD_Pt-80to170_EMEnriched',
                 'GJets_HT-40To100',
                 'GJets_HT-100To200',
                 'GJets_HT-200']
singleTop_samples = [ 'T_tW-channel',
                 'T_t-channel',
                 'T_s-channel',
                 'Tbar_tW-channel',
                 'Tbar_t-channel',
                 'Tbar_s-channel']
wplusjets_samples = [ 'W1Jet', 'W2Jets', 'W3Jets', 'W4Jets']
diboson_samples = [ 'WWtoAnything', 'WZtoAnything', 'ZZtoAnything']
signal_samples = [ 'TTJet', 'SingleTop']
allMC_samples = [ 'TTJet', 'DYJetsToLL', 'QCD', 'Di-Boson', 'W+Jets', 'SingleTop']

theoryXsection = 157.5

#DO NOT USE theory x-section but rather just the total luminosity*selection efficiency
def getTtbarCrossSection(vectors_={}, normalisation_={}):
    
    #at the moment store the values in the global variables. Not good but the only way it works with current setup
    global normalisation, vectors, theoryXsection
    normalisation = normalisation_
    vectors = vectors_
        
    #setup minuit
    numberOfParameters = 5
    gMinuit = TMinuit(numberOfParameters)
    gMinuit.SetFCN(fitFunction)
    
    gMinuit.SetPrintLevel(-1)
    #Error definition: 1 for chi-squared, 0.5 for negative log likelihood
    gMinuit.SetErrorDef(1)
    #error flag for functions passed as reference.set to as 0 is no error
    errorFlag = ROOT.Long(2)
    
    N_total = normalisation['ElectronHad']
#    N_total = 10000000
    if DEBUG:
        print "Total number of data events before the fit: ", N_total
    N_signal = normalisation['TTJet'] + normalisation['SingleTop']
    #how stupid is this? void function but uses a reference to one of the input parameters to 'return' an int!!!!
#    mnparm(paramNumber, paramName, startingValue, startingStepSizeOrUncertainty,physicalLowerLimit, physicalUpperLimit, errorFlagOutput)  
    gMinuit.mnparm(0, "N_signal(ttbar+single_top)", N_signal, 10.0, 0, N_total, errorFlag)
    gMinuit.mnparm(1, "W+Jets", normalisation['W+Jets'], 10.0, 0, N_total, errorFlag)
    gMinuit.mnparm(2, "Z+Jets", normalisation['DYJetsToLL'], 10.0, 0, N_total, errorFlag)
    gMinuit.mnparm(3, "QCD", normalisation['QCD'], 10.0, 0, N_total, errorFlag)
    gMinuit.mnparm(4, "Di-Boson", normalisation['Di-Boson'], 10.0, 0, N_total, errorFlag)

    
    
    arglist = array('d', 10 * [0.])
    
    #minimisation strategy: 1 standard, 2 try to improve minimum (a bit slower)
    arglist[0] = 2
    
    #minimisation itself
    gMinuit.mnexcm("SET STR", arglist, 1, errorFlag)
    gMinuit.Migrad()
    
    #get the results
    fit = None
    value, error = 0, 0
    fitvalues = []
    fitErrors = []
    for i in range(numberOfParameters):
        temp_par = Double(0)
        temp_err = Double(0)
        gMinuit.GetParameter(i, temp_par, temp_err)
        fitvalues.append(temp_par)
        fitErrors.append(temp_err)

    signalFit = fitvalues[0]
    signalFitError = fitErrors[0]
    ttbarFit = signalFit - normalisation['SingleTop']
    normfactor = unbinnedTTbarEvents / theoryXsection
    
    value = ttbarFit / normfactor
    value_plusError = value + (signalFitError / normfactor)
    value_minusError = value - (signalFitError / normfactor)
    error = (abs(value_plusError - value) + abs(value - value_minusError)) / 2
    fit = makeFit(fitvalues, vectors)
    return fit, value, error, fitvalues, fitErrors

def makeFit(fitvalues, templates):
    global fit_index
    fit = TH1F('fit_' + str(fit_index), 'fit', len(templates['Signal']), -3, 3)
    fit_index += 1
    bin = 1
    input = zip(vectors['Signal'],
                 vectors['W+Jets'], vectors['DYJetsToLL'],
                vectors['QCDFromData'], vectors['Di-Boson'])
    for signal, wjets, zjets, qcd, diBoson in input:
        value = fitvalues[0] * signal + fitvalues[1] * wjets + fitvalues[2] * zjets + fitvalues[3] * qcd + fitvalues[4] * diBoson
        fit.SetBinContent(bin, value)
        bin += 1
    return fit.Clone()
    
# fcn returns back f = - 2*ln(L), the function to be minimised
def fitFunction(nParameters, gin, f, par, iflag):
    global normalisation, vectors, qcdError
    lnL = 0.0
    input = zip(vectors['ElectronHad'], vectors['Signal'],
                 vectors['W+Jets'], vectors['DYJetsToLL'],
                vectors['QCDFromData'], vectors['Di-Boson'])
    
    for data, signal, wjets, zjets, qcd, diBoson in input:
        #signal = ttjet + singleTop
        x_i = par[0] * signal + par[1] * wjets + par[2] * zjets + par[3] * qcd + par[4] * diBoson#expected number of events in each bin
        data = data * normalisation['ElectronHad']
        if (data != 0) and (x_i != 0):
            L = TMath.Poisson(data, x_i)
            lnL += log(L)
    f[0] = -2.0 * lnL
    
    #constrains
    ratio_Z_W = normalisation['DYJetsToLL'] / normalisation['W+Jets']
#    ratio of Z/W to 5%
    if ratio_Z_W == 0 or par[1] == 0:
        f[0] += 1
    else:
        f[0] += ((par[2] / par[1] - ratio_Z_W) / (0.05 * ratio_Z_W)) ** 2
    #constrain W/Z+jets normalisation to 30%
#    f[0] += ((par[1] - normalisation['W+Jets']) / (0.3 * normalisation['W+Jets'])) ** 2
#    f[0] += ((par[2] - normalisation['DYJetsToLL']) / (0.3 * normalisation['DYJetsToLL'])) ** 2
#    f[0] += ((par[4] - normalisation['Di-Boson']) / (0.3 * normalisation['Di-Boson'])) ** 2
    #constrain QCD to the error 
    f[0] += ((par[3] - normalisation['QCD']) / normalisation['QCD']) ** 2
#    f[0] += ((par[3] - normalisation['QCD']) / qcdError) ** 2

def prepareHistograms(histograms):
    return histograms

def getNormalisation(histograms):
    normalisation = {}
    for sample in histograms.keys():
        normalisation[sample] = histograms[sample].Integral()
        if not sample == 'ElectronHad':
           normalisation[sample] = normalisation[sample] * lumiScaleFactor
        if sample == 'SingleTop':
            normalisation[sample] = normalisation[sample] * singleTopScaleFactor
        if sample == 'TTJet':
            normalisation[sample] = normalisation[sample] * ttbarScaleFactor
        if sample == 'W+Jets':
            normalisation[sample] = normalisation[sample] * wjetsScaleFactor
        if sample == 'DYJetsToLL':
            normalisation[sample] = normalisation[sample] * zjetsScaleFactor
    return normalisation

def getTemplates(histograms):
    templates = {}
    for sample in histograms.keys():
        hist = histograms[sample].Clone()
        hist.Sumw2()
        templates[sample] = plotting.normalise(hist)
    return templates 

def vectorise(histograms):
    values = {}
    errors = {}
    for sample in histograms.keys():
        hist = histograms[sample]
        nBins = hist.GetNbinsX()
#        hist.Rebin(int(hist.GetSize()/200))
        for bin in range(1, nBins + 1):
            if not values.has_key(sample):
                values[sample] = []
            if not errors.has_key(sample):
                errors[sample] = []
            values[sample].append(hist.GetBinContent(bin))
            errors[sample].append(hist.GetBinError(bin))
    return values, errors
    

def sumSamples(hists):
    global signal_samples, allMC_samples, qcd_samples, singleTop_samples, diboson_samples, wplusjets_samples
    hists['QCD'] = plotting.sumSamples(hists, qcd_samples)
    hists['SingleTop'] = plotting.sumSamples(hists, singleTop_samples)
    hists['Di-Boson'] = plotting.sumSamples(hists, diboson_samples)
    hists['W+Jets'] = plotting.sumSamples(hists, wplusjets_samples)
    hists['SumMC'] = plotting.sumSamples(hists, allMC_samples)
    hists['Signal'] = plotting.sumSamples(hists, signal_samples)
    return hists

def rescaleSamples(hists):
    return hists    

def performMeasurement(distribution, qcdDistribution, listOfFiles, rebin, data_distribution='', data_qcdDistribution=''):
    if data_distribution == '':
        data_distribution = distribution
    if data_qcdDistribution == '':
        data_qcdDistribution = qcdDistribution
        
    if DEBUG:
        print 'Performing measurement for:'
        print distribution
        print 'QCD shape from data:', qcdDistribution
    #create MC files and data only file
    mcFiles = deepcopy(listOfFiles)
    mcFiles.pop('ElectronHad')#removes data
    hists = FileReader.getHistogramDictionary(distribution, mcFiles)
    qcdHists = FileReader.getHistogramDictionary(qcdDistribution, mcFiles)
    data_hist = FileReader.getHistogramFromFile(data_distribution, listOfFiles['ElectronHad'])
    data_qcdhist = FileReader.getHistogramFromFile(data_qcdDistribution, listOfFiles['ElectronHad'])
    
    hists['ElectronHad'] = data_hist
    qcdHists['ElectronHad'] = data_qcdhist
    
    hists = plotting.rebin(hists, rebin)#rebin to 200 bins
    hists = plotting.setYTitle(hists, title="events/5 GeV")
    qcdHists = plotting.rebin(qcdHists, rebin)#rebin to 200 bins
    qcdHists = plotting.setYTitle(qcdHists, title="events/5 GeV")
    #rescale hists if needed
    hists = rescaleSamples(hists)
    qcdHists = rescaleSamples(qcdHists)
    #add samples together: QCD, SumMC, singleTOP, W+Jets
    hists = sumSamples(hists)
    qcdHists = sumSamples(qcdHists)
    #TODO: this needs to be replaced and done properly
    
    normalisation = getNormalisation(hists)
    templates = getTemplates(hists)
    qcdTemplates = getTemplates(qcdHists)
    
    vectors, vector_errors = vectorise(templates)
    qcdVectors, qcdVector_errors = vectorise(qcdTemplates)
    vectors['QCDFromData'] = qcdVectors['ElectronHad']
    qcdHistForEstimation = QCDRateEstimation.defaultHistogram
    #replace QCD bin
    qcdHistForEstimation = qcdHistForEstimation.replace(qcdHistForEstimation.split('_')[-1], distribution.split('_')[-1])
    global qcdError
    qcdResult = QCDRateEstimation.estimateQCDWithRelIso(FILES.files, qcdHistForEstimation)
    qcdNormalisation, qcdError = qcdResult['estimate'], qcdResult['absoluteError']
    
    sumMC = normalisation['SumMC'] - normalisation['QCD'] + qcdNormalisation
    if DEBUG:
        print '*' * 120
        print "Input parameters:"
        print 'signal (ttbar+single top):', normalisation['TTJet'] + normalisation['SingleTop']
        print 'W+Jets:', normalisation['WJetsToLNu']
        print 'Z+Jets:', normalisation['DYJetsToLL'] 
        print 'QCD (from data):', qcdNormalisation, '+-', qcdError, '(', qcdError / qcdNormalisation * 100, '%)'
        print 'SingleTop:', normalisation['SingleTop'] 
        print 'TTJet:', normalisation['TTJet']  
        print 'Di-Boson:', normalisation['Di-Boson']
        print 'SumMC:', sumMC
        print 'Total data', normalisation['ElectronHad']
        print '(data-SumMC)/data', (normalisation['ElectronHad'] - sumMC) / normalisation['ElectronHad']
        print '*' * 120
    normalisation['QCD'] = qcdNormalisation
    
    fit, value, error, fitvalues, fitErrors = getTtbarCrossSection(vectors, normalisation)
    
    sumMC = fitvalues[0] + fitvalues[1] + fitvalues[2] + fitvalues[3] + fitvalues[4]
    if DEBUG:
        print '*' * 120
        print "Fit values:"
        print 'signal (ttbar+single top):', fitvalues[0], '+-', fitErrors[0]
        print 'W+Jets:', fitvalues[1], '+-', fitErrors[1]
        print 'Z+Jets:', fitvalues[2], '+-', fitErrors[2]
        print 'QCD:', fitvalues[3], '+-', fitErrors[3]
        print 'SingleTop (no fit):', normalisation['SingleTop'] 
        print 'TTJet (signal fit - SingleTop):', fitvalues[0] - normalisation['SingleTop'] 
        print 'Di-Boson:', fitvalues[4], '+-', fitErrors[4]
        print 'SumMC:', sumMC
        print 'Total data', normalisation['ElectronHad']
        print '(data-SumMC)/data', (normalisation['ElectronHad'] - sumMC) / normalisation['ElectronHad']
        print '*' * 120
    fitvalues_dict = {'Signal': fitvalues[0],
                      'W+Jets': fitvalues[1],
                      'DYJetsToLL': fitvalues[2],
                      'QCDFromData': fitvalues[3],
                      'Di-Boson': fitvalues[4],
                      'TTJet': fitvalues[0] - normalisation['SingleTop'],
                      'ElectronHad': normalisation['ElectronHad'],
                      'SingleTop': normalisation['SingleTop']
                      }
    return value, error, fit, fitvalues_dict, fitErrors
        
def makeMeasurementPlot(metbins, results, btagBin):
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 1001;')
   
    plotting.setStyle()
    arglist = array('d', [0, 25, 45, 70, 100, 150])
    c = TCanvas("test", "Differential cross section", 1600, 1200)
    plot = TH1F("measurement_" + btagBin, 'Differential cross section; MET [GeV];#frac{#partial#sigma}{#partial MET}', len(arglist) - 1, arglist)
    plotMC = TH1F("measurement_MC_" + btagBin, 'Differential cross section; MET [GeV];#frac{#partial#sigma}{#partial MET}', len(arglist) - 1, arglist)
    plot.SetMinimum(0)
    plot.SetMaximum(60)
    plotMC.SetFillColor(kRed + 1)
    legend = plotting.create_legend()
    legend.AddEntry(plot, 'measured', 'LEP')
    legend.AddEntry(plotMC, 't#bar{t} (MADGRAPH)', 'f')
    bin = 1
    for metbin in metbins:
        result = results[metbin]
        totalUncertainty, totalUncertainty_minus, totalUncertainty_plus = calculateTotalUncertainty(result)
        error = sqrt(result['statError'] ** 2 + totalUncertainty ** 2)
        plot.SetBinContent(bin, result['centralValue'])
        plot.SetBinError(bin, error)
        plotMC.SetBinContent(bin, result['theoryXsection'])
        bin += 1    
    plot.Draw('e1')
    gStyle.SetErrorX(0.4)
    plotMC.Draw('hist same')
    plot.Draw('error same')
    legend.Draw()
    c.SaveAs(savePath + 'EPlusJets_diff_MET_xsection_%s.png' % btagBin)
    c.SaveAs(savePath + 'EPlusJets_diff_MET_xsection_%s.pdf' % btagBin)
    
def printMetSystematics(metbins, results, btagBin):
    printout = ''
    prefix = 'patType1p2CorrectedPFMet'
    header = 'Source'
    rows = {}
    total = {}
    for metbin in metbins:
        header += ' & ' + 'MET %s~\GeV' % metbin 
        result = results[metbin]
        centralvalue = result['centralValue']
        metsystematics = result['systematics']
        errors_on_systematics = result['errors_on_systematics']
        totalUncertainty, totalUncertainty_minus, totalUncertainty_plus = calculateTotalUncertainty(result)
        values = result['values']
        statErrors = result['statErrors']
        pdfweight_minus, pdfweight_plus = calculatePDFErrors(values, statErrors)
        metsystematics['PDFWeightsPlus'] = pdfweight_plus
        metsystematics['PDFWeightsMinus'] = pdfweight_minus
        
        errors_on_systematics['PDFWeightsPlus'] = 0
        errors_on_systematics['PDFWeightsMinus'] = 0
        
        for source in sorted(metsystematics.keys()):
            systematic = metsystematics[source]
            error_on_systematic = errors_on_systematics[source]
            source = source.replace(prefix, '')
            if 'PDFWeights_' in source:
                continue

            if not rows.has_key(source):
                rows[source] = '(%.2f \pm %.2f)' % (systematic, error_on_systematic)
            else:
                rows[source] += '& (%.2f \pm %.2f)' % (systematic, error_on_systematic)
        
        if not rows.has_key('PDFWeights'):
            rows['PDFWeights'] = '(%.2f \pm %.2f)' % (totalPDF, totalPDFError*100) 
        else:
            rows['PDFWeights'] += '(%.2f \pm %.2f)' % (totalPDF*100, totalPDFError*100)
            
        if not rows.has_key('TotalUp'):
            rows['TotalUp'] = '%.2f' % (totalUncertainty_plus) 
        else:
            rows['TotalUp'] += '& %.2f' % (totalUncertainty_plus)
        if not rows.has_key('TotalDown'):
            rows['TotalDown'] = '-%.2f' % (totalUncertainty_minus)
        else:
            rows['TotalDown'] += '&-%.2f' % (totalUncertainty_minus)
    
    printout += header + '\\\\\n'
    printout += '\hline\n'
    for source in sorted(rows.keys()):
        value = rows[source]
        if 'Total' in source:
            continue
        printout += source + ' & ' + value + '\\\\\n'
    printout += 'PDFWeights' + ' & ' + rows['PDFWeights'] + '\\\\\n'
    printout += 'TotalUp' + ' & ' + rows['TotalUp'] + '\\\\\n'
    printout += 'TotalDown' + ' & ' + rows['TotalDown'] + '\\\\\n'
    systematics_file = open(savePath + 'systematics_%s.tex' % btagBin, 'w')
    systematics_file.write(printout)
    systematics_file.close()
        
def doBinnedAnalysis(metBins, btagBin):
    print "Performing measurement in", btagBin
    global lumiScaleFactor, singleTopScaleFactor, ttbarScaleFactor, unbinnedTTbarEvents, vectors, theoryXsection
    results = {}
    base = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/BinnedMETAnalysis/'
    
    rebin = 10
    fit, fitvalues, fiterrors, templates = (None, None, None, None)
    for metbin in metBins:
        values = {}
        statErrors = {}
        fits = {}
        fitvalues_dict = {}
        fiterrors_dict = {}
        templates_dict = {}
        systematics = {}
        errors_on_systematics = {}
        
        
        print "Performing central measurement in", btagBin, 'MET:', metbin
        #central measurement
        distribution = base + 'Electron_patType1CorrectedPFMet_bin_%s/electron_eta_%s' % (metbin, btagBin)
       #should always use 0-btag region!!                  
        qcdDistribution = base + 'QCDConversions/Electron_patType1CorrectedPFMet_bin_%s/electron_eta_0btag' % metbin
        centralValue, statError, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files, rebin)
        values['central'] = centralValue
        statErrors['central'] = statError
        fits['central'] = fit
        fitvalues_dict['central'] = fitvalues
        fiterrors_dict['central'] = fiterrors
        templates_dict['central'] = vectors
        centralRelativeError = getRelativeError(centralValue, statError)
        templates = vectors
        #get x-section from simulation only
        distribution = base + 'Electron_patType1CorrectedPFMet_bin_%s/electron_eta_%s' % (metbin, btagBin)
        ttbarHist = FileReader.getHistogramFromFile(distribution, FILES.files['TTJet'])
        theoryXsection_inBin = ttbarHist.Integral() / unbinnedTTbarEvents * theoryXsection
        print "Performing measurements for systematics in", btagBin, 'MET:', metbin
        #electron efficiency, flat 3% on ratem re-use lumiScaleFactor
        lumiScaleFactor = 1. + 0.03
        value, error, fit, fitvalues, fiterrors  = performMeasurement(distribution, qcdDistribution, FILES.files, rebin)
        values['electronEfficiencyPlus'] = value
        statErrors['electronEfficiencyPlus'] = error
        fits['electronEfficiencyPlus'] = fit
        fitvalues_dict['electronEfficiencyPlus'] = fitvalues
        fiterrors_dict['electronEfficiencyPlus'] = fiterrors
        templates_dict['electronEfficiencyPlus'] = vectors
        
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics['electronEfficiencyPlus'] = value - centralValue
        errors_on_systematics['electronEfficiencyPlus'] = systematics['electronEfficiencyPlus'] * relativeError
        lumiScaleFactor = 1. - 0.03
        
        value, error, fit, fitvalues, fiterrors  = performMeasurement(distribution, qcdDistribution, FILES.files, rebin)
        values['electronEfficiencyMinus'] = value
        statErrors['electronEfficiencyMinus'] = error
        fits['electronEfficiencyMinus'] = fit
        fitvalues_dict['electronEfficiencyMinus'] = fitvalues
        fiterrors_dict['electronEfficiencyMinus'] = fiterrors
        templates_dict['electronEfficiencyMinus'] = vectors
        
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics['electronEfficiencyMinus'] = value - centralValue
        errors_on_systematics['electronEfficiencyMinus'] = systematics['electronEfficiencyMinus'] * relativeError
        lumiScaleFactor = 1.
        #luminosity systematic 2.2%
        lumiScaleFactor = 1. + 0.022 
        value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files, rebin)
        values['lumiPlus'] = value
        statErrors['lumiPlus'] = error
        fits['lumiPlus'] = fit
        fitvalues_dict['lumiPlus'] = fitvalues
        fiterrors_dict['lumiPlus'] = fiterrors
        templates_dict['lumiPlus'] = vectors
        
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics['lumiPlus'] = value - centralValue
        errors_on_systematics['lumiPlus'] = systematics['lumiPlus'] * relativeError
        lumiScaleFactor = 1. - 0.022
        value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files, rebin)
        values['lumiMinus'] = value
        statErrors['lumiMinus'] = error
        fits['lumiMinus'] = fit
        fitvalues_dict['lumiMinus'] = fitvalues
        fiterrors_dict['lumiMinus'] = fiterrors
        templates_dict['lumiMinus'] = vectors
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics['lumiMinus'] = value - centralValue
        errors_on_systematics['lumiMinus'] = systematics['lumiMinus'] * relativeError
        lumiScaleFactor = 1.
        #singleTop x-section 30%
        singleTopScaleFactor = 1. + 0.3
        value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files, rebin)
        values['singleTopPlus'] = value
        statErrors['singleTopPlus'] = error
        fits['singleTopPlus'] = fit
        fitvalues_dict['singleTopPlus'] = fitvalues
        fiterrors_dict['singleTopPlus'] = fiterrors
        templates_dict['singleTopPlus'] = vectors
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics['singleTopPlus'] = value - centralValue
        errors_on_systematics['singleTopPlus'] = systematics['singleTopPlus'] * relativeError
        singleTopScaleFactor = 1. - 0.3
        value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files, rebin)
        values['singleTopMinus'] = value
        statErrors['singleTopMinus'] = error
        fits['singleTopMinus'] = fit
        fitvalues_dict['singleTopMinus'] = fitvalues
        fiterrors_dict['singleTopMinus'] = fiterrors
        templates_dict['singleTopMinus'] = vectors
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics['singleTopMinus'] = value - centralValue
        errors_on_systematics['singleTopMinus'] = systematics['singleTopMinus'] * relativeError
        singleTopScaleFactor = 1
        #ttbar x-section 15%
        #TODO: this has to be changed, not working as intended!
#        theoryXsection_temp = theoryXsection
#        theoryXsection = theoryXsection_temp * (1.0 + 0.15)
#        ttbarScaleFactor = 1. + 0.15
#        value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files, rebin)
#        values['ttbarPlus'] = value
#        statErrors['ttbarPlus'] = error
#        fits['ttbarPlus'] = fit
#        fitvalues_dict['ttbarPlus'] = fitvalues
#        fiterrors_dict['ttbarPlus'] = fiterrors
#        templates_dict['ttbarPlus'] = vectors
#        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
#        systematics['ttbarPlus'] = value - centralValue
#        errors_on_systematics['ttbarPlus'] = systematics['ttbarPlus'] * relativeError
#        ttbarScaleFactor = 1.0 - 0.15
##        theoryXsection = theoryXsection_temp * (1.0 - 0.15)
#        value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files, rebin)
#        values['ttbarMinus'] = value
#        statErrors['ttbarMinus'] = error
#        fits['ttbarMinus'] = fit
#        fitvalues_dict['ttbarMinus'] = fitvalues
#        fiterrors_dict['ttbarMinus'] = fiterrors
#        templates_dict['ttbarMinus'] = vectors
#        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
#        systematics['ttbarMinus'] = value - centralValue
#        errors_on_systematics['ttbarMinus'] = systematics['ttbarMinus'] * relativeError
#        theoryXsection = theoryXsection_temp
#        ttbarScaleFactor = 1
        #QCD shape systematic
        qcdDistribution = base + 'QCD non iso e+jets/Electron_patType1CorrectedPFMet_bin_%s/electron_eta_0btag' % metbin
        value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files, rebin)
        values['QCDShape'] = value
        statErrors['QCDShape'] = error
        fits['QCDShape'] = fit
        fitvalues_dict['QCDShape'] = fitvalues
        fiterrors_dict['QCDShape'] = fiterrors
        templates_dict['QCDShape'] = vectors
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics['QCDShape'] = value - centralValue
        errors_on_systematics['QCDShape'] = systematics['QCDShape'] * relativeError
        qcdDistribution = base + 'QCDConversions/Electron_patType1CorrectedPFMet_bin_%s/electron_eta_0btag' % metbin
        #JES down
        value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files_JES_down, rebin)
        values['JES_down'] = value
        statErrors['JES_down'] = error
        fits['JES_down'] = fit
        fitvalues_dict['JES_down'] = fitvalues
        fiterrors_dict['JES_down'] = fiterrors
        templates_dict['JES_down'] = vectors
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics['JES_down'] = value - centralValue
        errors_on_systematics['JES_down'] = systematics['JES_down'] * relativeError
        #JES up
        value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files_JES_up, rebin)
        values['JES_up'] = value
        statErrors['JES_up'] = error
        fits['JES_up'] = fit
        fitvalues_dict['JES_up'] = fitvalues
        fiterrors_dict['JES_up'] = fiterrors
        templates_dict['JES_up'] = vectors
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics['JES_up'] = value - centralValue
        errors_on_systematics['JES_up'] = systematics['JES_up'] * relativeError
        #PU down
        value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files_PU_down, rebin)
        values['PU_down'] = value
        statErrors['PU_down'] = error
        fits['PU_down'] = fit
        fitvalues_dict['PU_down'] = fitvalues
        fiterrors_dict['PU_down'] = fiterrors
        templates_dict['PU_down'] = vectors
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics['PU_down'] = value - centralValue
        errors_on_systematics['PU_down'] = systematics['PU_down'] * relativeError
        #PU up
        value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files_PU_up, rebin)
        values['PU_up'] = value
        statErrors['PU_up'] = error
        fits['PU_up'] = fit
        fitvalues_dict['PU_up'] = fitvalues
        fiterrors_dict['PU_up'] = fiterrors
        templates_dict['PU_up'] = vectors
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics['PU_up'] = value - centralValue
        errors_on_systematics['PU_up'] = systematics['PU_up'] * relativeError
        
        print "Q^2 and matching systematics", btagBin, 'MET:', metbin
        #Q^2 scale and matching threshold systematics
        vars = doTTbarSystematics(centralValue, centralRelativeError, metbin, btagBin, rebin)
        for source in vars[0].keys():
            systematics[source] = vars[0][source]
            errors_on_systematics[source] = vars[1]
            values[source] = vars[2][source]
            statErrors[source] = vars[3][source]
            fits[source] = vars[4][source]
            fitvalues_dict[source] = vars[5][source]
            fiterrors_dict[source] = vars[6][source]
            templates_dict[source] = vars[7][source]
        
        vars = doWJetsSystematics(centralValue, centralRelativeError, metbin, btagBin, rebin)
        for source in vars[0].keys():
            systematics[source] = vars[0][source]
            errors_on_systematics[source] = vars[1]
            values[source] = vars[2][source]
            statErrors[source] = vars[3][source]
            fits[source] = vars[4][source]
            fitvalues_dict[source] = vars[5][source]
            fiterrors_dict[source] = vars[6][source]
            templates_dict[source] = vars[7][source]
        
        vars = doZJetsSystematics(centralValue, centralRelativeError, metbin, btagBin, rebin)
        for source in vars[0].keys():
            systematics[source] = vars[0][source]
            errors_on_systematics[source] = vars[1]
            values[source] = vars[2][source]
            statErrors[source] = vars[3][source]
            fits[source] = vars[4][source]
            fitvalues_dict[source] = vars[5][source]
            fiterrors_dict[source] = vars[6][source]
            templates_dict[source] = vars[7][source]
        
        print "MET systematics", btagBin, 'MET:', metbin
        vars = doMETSystematics(centralValue, centralRelativeError, metbin, btagBin, rebin)
        for source in vars[0].keys():
            systematics[source] = vars[0][source]
            errors_on_systematics[source] = vars[1]
            values[source] = vars[2][source]
            statErrors[source] = vars[3][source]
            fits[source] = vars[4][source]
            fitvalues_dict[source] = vars[5][source]
            fiterrors_dict[source] = vars[6][source]
            templates_dict[source] = vars[7][source]
        
        print "PDF weight systematics", btagBin, 'MET:', metbin
        vars = doPDFSystematics(centralValue, centralRelativeError, metbin, btagBin, rebin)
        for source in vars[0].keys():
            systematics[source] = vars[0][source]
            errors_on_systematics[source] = vars[1]
            values[source] = vars[2][source]
            statErrors[source] = vars[3][source]
            fits[source] = vars[4][source]
            fitvalues_dict[source] = vars[5][source]
            fiterrors_dict[source] = vars[6][source]
            templates_dict[source] = vars[7][source]
        #add systematics in squares
        totalSystematic = 0
        for source, syst in systematics.iteritems():    
            totalSystematic += syst ** 2
        totalSystematic = sqrt(totalSystematic)
        
        results[metbin] = {'centralValue':values['central'],
                           'statError':statErrors['central'],
                           'totalSystematic':totalSystematic,
                           'theoryXsection':theoryXsection_inBin,
                           'systematics':systematics,
                           'errors_on_systematics':errors_on_systematics,
                           'fit':fits['central'],
                           'fitvalues':fitvalues_dict['central'],
                           'fiterrors': fiterrors_dict['central'],
                           'templates': templates_dict['central'],
                           'values':values,
                           'statErrors':statErrors,
                           'fits': fits,
                           'allFitvalues':fitvalues_dict,
                           'allFiterrors':fiterrors_dict,
                           'allTemplates':templates_dict }
    return results

def doTTbarSystematics(centralValue, centralRelativeError, metbin, btagBin, rebin):
    base = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/BinnedMETAnalysis/'
    distribution = base + 'Electron_patType1CorrectedPFMet_bin_%s/electron_eta_%s' % (metbin, btagBin)
       #should always use 0-btag region!!                  
    qcdDistribution = base + 'QCDConversions/Electron_patType1CorrectedPFMet_bin_%s/electron_eta_0btag' % metbin
    values = {}
    statErrors = {}
    fits = {}
    fitvalues_dict = {}
    fiterrors_dict = {}
    templates_dict = {}
    systematics = {}
    errors_on_systematics = {}
    #matching threshold scale down (ttbar)
    temp_files = deepcopy(FILES.files)
    temp_files['TTJet'] = FILES.files['TTJets-matchingdown']#replace distribution
    ttbarScaleFactor = unbinnedTTbarEvents / unbinnedTTbarEvents_matchingDown#normalise to same number of events
    value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
    values['TTJets_matchingDown'] = value
    statErrors['TTJets_matchingDown'] = error
    fits['TTJets_matchingDown'] = fit
    fitvalues_dict['TTJets_matchingDown'] = fitvalues
    fiterrors_dict['TTJets_matchingDown'] = fiterrors
    templates_dict['TTJets_matchingDown'] = vectors

    relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
    systematics['TTJets_matchingDown'] = value - centralValue
    errors_on_systematics['TTJets_matchingDown'] = systematics['TTJets_matchingDown'] * relativeError
    ttbarScaleFactor = 1
    #matching threshold scale up
    temp_files = deepcopy(FILES.files)
    temp_files['TTJet'] = FILES.files['TTJets-matchingup']#replace distribution
    ttbarScaleFactor = unbinnedTTbarEvents / unbinnedTTbarEvents_matchingUp#normalise to same number of events
    ttbarScaleFactor = unbinnedTTbarEvents / unbinnedTTbarEvents_matchingDown#normalise to same number of events
    value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
    values['TTJets_matchingUp'] = value
    statErrors['TTJets_matchingUp'] = error
    fits['TTJets_matchingUp'] = fit
    fitvalues_dict['TTJets_matchingUp'] = fitvalues
    fiterrors_dict['TTJets_matchingUp'] = fiterrors
    templates_dict['TTJets_matchingUp'] = vectors
    relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
    systematics['TTJets_matchingUp'] = value - centralValue
    errors_on_systematics['TTJets_matchingUp'] = systematics['TTJets_matchingUp'] * relativeError
    ttbarScaleFactor = 1
    #Q2 (ttbar) scale up
    temp_files = deepcopy(FILES.files)
    temp_files['TTJet'] = FILES.files['TTJets-scaledown']#replace distribution
    ttbarScaleFactor = unbinnedTTbarEvents / unbinnedTTbarEvents_scaleDown#normalise to same number of events
    value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
    values['TTJets_scaleDown'] = value
    statErrors['TTJets_scaleDown'] = error
    fits['TTJets_scaleDown'] = fit
    fitvalues_dict['TTJets_scaleDown'] = fitvalues
    fiterrors_dict['TTJets_scaleDown'] = fiterrors
    templates_dict['TTJets_scaleDown'] = vectors
    relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
    systematics['TTJets_scaleDown'] = value - centralValue
    errors_on_systematics['TTJets_scaleDown'] = systematics['TTJets_scaleDown'] * relativeError
    ttbarScaleFactor = 1
    #Q2 (ttbar) scale up
    temp_files = deepcopy(FILES.files)
    temp_files['TTJet'] = FILES.files['TTJets-scaleup']#replace distribution
    ttbarScaleFactor = unbinnedTTbarEvents / unbinnedTTbarEvents_scaleUp#normalise to same number of events
    value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
    values['TTJets_scaleUp'] = value
    statErrors['TTJets_scaleUp'] = error
    fits['TTJets_scaleUp'] = fit
    fitvalues_dict['TTJets_scaleUp'] = fitvalues
    fiterrors_dict['TTJets_scaleUp'] = fiterrors
    templates_dict['TTJets_scaleUp'] = vectors
    relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
    systematics['TTJets_scaleUp'] = value - centralValue
    errors_on_systematics['TTJets_scaleUp'] = systematics['TTJets_scaleUp'] * relativeError
    ttbarScaleFactor = 1
    
    return systematics, errors_on_systematics, values, statErrors, fits, fitvalues_dict, fiterrors_dict, templates_dict

def doWJetsSystematics(centralValue, centralRelativeError, metbin, btagBin, rebin):
    global wplusjets_samples, unbinnedWJetsEvents_matchingDown, unbinnedWJetsEvents_matchingUp
    global unbinnedWJetsEvents_scaleDown, unbinnedWJetsEvents_scaleUp, unbinnedWJetsEvents
    base = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/BinnedMETAnalysis/'
    distribution = base + 'Electron_patType1CorrectedPFMet_bin_%s/electron_eta_%s' % (metbin, btagBin)
    #should always use 0-btag region!!                  
    qcdDistribution = base + 'QCDConversions/Electron_patType1CorrectedPFMet_bin_%s/electron_eta_0btag' % metbin
    values = {}
    statErrors = {}
    fits = {}
    fitvalues_dict = {}
    fiterrors_dict = {}
    templates_dict = {}
    systematics = {}
    errors_on_systematics = {}
    temp_wjets_sum = deepcopy(wplusjets_samples)
    wplusjets_samples = ['WJetsToLNu']
    #matching threshold scale down (ttbar)
    temp_files = deepcopy(FILES.files)
    temp_files['WJetsToLNu'] = FILES.files['WJets-matchingdown']#replace distribution
    
    wjetsScaleFactor = unbinnedWJetsEvents / unbinnedWJetsEvents_matchingDown#normalise to same number of events
    value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
    values['WJets_matchingDown'] = value
    statErrors['WJets_matchingDown'] = error
    fits['WJets_matchingDown'] = fit
    fitvalues_dict['WJets_matchingDown'] = fitvalues
    fiterrors_dict['WJets_matchingDown'] = fiterrors
    templates_dict['WJets_matchingDown'] = vectors
    relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
    systematics['WJets_matchingDown'] = value - centralValue
    errors_on_systematics['WJets_matchingDown'] = systematics['WJets_matchingDown'] * relativeError
    wjetsScaleFactor = 1
    #matching threshold scale up
    temp_files = deepcopy(FILES.files)
    temp_files['WJetsToLNu'] = FILES.files['WJets-matchingup']#replace distribution
    wjetsScaleFactor = unbinnedWJetsEvents / unbinnedWJetsEvents_matchingUp#normalise to same number of events
    value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
    values['WJets_matchingUp'] = value
    statErrors['WJets_matchingUp'] = error
    fits['WJets_matchingUp'] = fit
    fitvalues_dict['WJets_matchingUp'] = fitvalues
    fiterrors_dict['WJets_matchingUp'] = fiterrors
    templates_dict['WJets_matchingUp'] = vectors
    relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
    systematics['WJets_matchingUp'] = value - centralValue
    errors_on_systematics['WJets_matchingUp'] = systematics['WJets_matchingUp'] * relativeError
    wjetsScaleFactor = 1
    #Q2 (ttbar) scale up
    temp_files = deepcopy(FILES.files)
    temp_files['WJetsToLNu'] = FILES.files['WJets-scaledown']#replace distribution
    wjetsScaleFactor = unbinnedWJetsEvents / unbinnedWJetsEvents_scaleDown#normalise to same number of events
    value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
    values['WJets_scaleDown'] = value
    statErrors['WJets_scaleDown'] = error
    fits['WJets_scaleDown'] = fit
    fitvalues_dict['WJets_scaleDown'] = fitvalues
    fiterrors_dict['WJets_scaleDown'] = fiterrors
    templates_dict['WJets_scaleDown'] = vectors
    relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
    systematics['WJets_scaleDown'] = value - centralValue
    errors_on_systematics['WJets_scaleDown'] = systematics['WJets_scaleDown'] * relativeError
    wjetsScaleFactor = 1
    #Q2 (ttbar) scale up
    temp_files = deepcopy(FILES.files)
    temp_files['WJetsToLNu'] = FILES.files['WJets-scaleup']#replace distribution
    wjetsScaleFactor = unbinnedWJetsEvents / unbinnedWJetsEvents_scaleUp#normalise to same number of events
    value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
    values['WJets_scaleUp'] = value
    statErrors['WJets_scaleUp'] = error
    fits['WJets_scaleUp'] = fit
    fitvalues_dict['WJets_scaleUp'] = fitvalues
    fiterrors_dict['WJets_scaleUp'] = fiterrors
    templates_dict['WJets_scaleUp'] = vectors
    relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
    systematics['WJets_scaleUp'] = value - centralValue
    errors_on_systematics['WJets_scaleUp'] = systematics['WJets_scaleUp'] * relativeError
    wjetsScaleFactor = 1
    #restore sum definition
    wplusjets_samples = temp_wjets_sum
    
    return systematics, errors_on_systematics, values, statErrors, fits, fitvalues_dict, fiterrors_dict, templates_dict

def doZJetsSystematics(centralValue, centralRelativeError, metbin, btagBin, rebin):
    global unbinnedZJetsEvents_matchingDown, unbinnedZJetsEvents_matchingUp
    global unbinnedZJetsEvents_scaleDown, unbinnedZJetsEvents_scaleUp, unbinnedZJetsEvents
    base = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/BinnedMETAnalysis/'
    distribution = base + 'Electron_patType1CorrectedPFMet_bin_%s/electron_eta_%s' % (metbin, btagBin)
    #should always use 0-btag region!!                  
    qcdDistribution = base + 'QCDConversions/Electron_patType1CorrectedPFMet_bin_%s/electron_eta_0btag' % metbin
    values = {}
    statErrors = {}
    fits = {}
    fitvalues_dict = {}
    fiterrors_dict = {}
    templates_dict = {}
    systematics = {}
    errors_on_systematics = {}
    #matching threshold scale down (ttbar)
    temp_files = deepcopy(FILES.files)
    temp_files['DYJetsToLL'] = FILES.files['ZJets-matchingdown']#replace distribution
    
    zjetsScaleFactor = unbinnedZJetsEvents / unbinnedZJetsEvents_matchingDown#normalise to same number of events
    value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
    values['ZJets_matchingDown'] = value
    statErrors['ZJets_matchingDown'] = error
    fits['ZJets_matchingDown'] = fit
    fitvalues_dict['ZJets_matchingDown'] = fitvalues
    fiterrors_dict['ZJets_matchingDown'] = fiterrors
    templates_dict['ZJets_matchingDown'] = vectors
    
    relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
    systematics['ZJets_matchingDown'] = value - centralValue
    errors_on_systematics['ZJets_matchingDown'] = systematics['ZJets_matchingDown'] * relativeError
    zjetsScaleFactor = 1
    #matching threshold scale up
    temp_files = deepcopy(FILES.files)
    temp_files['DYJetsToLL'] = FILES.files['ZJets-matchingup']#replace distribution
    zjetsScaleFactor = unbinnedZJetsEvents / unbinnedZJetsEvents_matchingUp#normalise to same number of events
    value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
    values['ZJets_matchingUp'] = value
    statErrors['ZJets_matchingUp'] = error
    fits['ZJets_matchingUp'] = fit
    fitvalues_dict['ZJets_matchingUp'] = fitvalues
    fiterrors_dict['ZJets_matchingUp'] = fiterrors
    templates_dict['ZJets_matchingUp'] = vectors
    relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
    systematics['ZJets_matchingUp'] = value - centralValue
    errors_on_systematics['ZJets_matchingUp'] = systematics['ZJets_matchingUp'] * relativeError
    zjetsScaleFactor = 1
    #Q2 (ttbar) scale up
    temp_files = deepcopy(FILES.files)
    temp_files['DYJetsToLL'] = FILES.files['ZJets-scaledown']#replace distribution
    zjetsScaleFactor = unbinnedZJetsEvents / unbinnedZJetsEvents_scaleDown#normalise to same number of events
    value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
    values['ZJets_scaleDown'] = value
    statErrors['ZJets_scaleDown'] = error
    fits['ZJets_scaleDown'] = fit
    fitvalues_dict['ZJets_scaleDown'] = fitvalues
    fiterrors_dict['ZJets_scaleDown'] = fiterrors
    templates_dict['ZJets_scaleDown'] = vectors
    relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
    systematics['ZJets_scaleDown'] = value - centralValue
    errors_on_systematics['ZJets_scaleDown'] = systematics['ZJets_scaleDown'] * relativeError
    zjetsScaleFactor = 1
    #Q2 (ttbar) scale up
    temp_files = deepcopy(FILES.files)
    temp_files['DYJetsToLL'] = FILES.files['ZJets-scaleup']#replace distribution
    zjetsScaleFactor = unbinnedZJetsEvents / unbinnedZJetsEvents_scaleUp#normalise to same number of events
    value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
    values['ZJets_scaleUp'] = value
    statErrors['ZJets_scaleUp'] = error
    fits['ZJets_scaleUp'] = fit
    fitvalues_dict['ZJets_scaleUp'] = fitvalues
    fiterrors_dict['ZJets_scaleUp'] = fiterrors
    templates_dict['ZJets_scaleUp'] = vectors
    relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
    systematics['ZJets_scaleUp'] = value - centralValue
    errors_on_systematics['ZJets_scaleUp'] = systematics['ZJets_scaleUp'] * relativeError
    zjetsScaleFactor = 1
    
    return systematics, errors_on_systematics, values, statErrors, fits, fitvalues_dict, fiterrors_dict, templates_dict

def doMETSystematics(centralValue, centralRelativeError, metbin, btagBin, rebin):
    base = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/BinnedMETAnalysis/'
    systematics = {}
    errors_on_systematics = {}
    values = {}
    statErrors = {}
    fits = {}
    fitvalues_dict = {}
    fiterrors_dict = {}
    templates_dict = {}
    for source in metsystematics_sources:
        value, error, fit, fitvalues, fiterrors, relativeError = 0,0,0,0,0,0
        if not 'JetRes' in source:
            distribution = base + 'Electron_%s_bin_%s/electron_eta_%s' % (source, metbin, btagBin)
            qcdDistribution = base + 'QCDConversions/Electron_%s_bin_%s/electron_eta_0btag' % (source, metbin)
            value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files, rebin)
        else:
            distribution = base + 'Electron_%s_bin_%s/electron_eta_%s' % (source, metbin, btagBin)
            qcdDistribution = base + 'QCDConversions/Electron_%s_bin_%s/electron_eta_0btag' % (source, metbin)
            data_distribution = base + 'Electron_patType1CorrectedPFMet_bin_%s/electron_eta_%s' % (metbin, btagBin)
            #should always use 0-btag region!!                  
            data_qcdDistribution = base + 'QCDConversions/Electron_patType1CorrectedPFMet_bin_%s/electron_eta_0btag' % metbin
            #JET change for MC only, not for data. 
            value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, FILES.files, 
                                              rebin, data_distribution, data_qcdDistribution)
            
        values[source] = value
        statErrors[source] = error
        fits[source] = fit
        fitvalues_dict[source] = fitvalues
        fiterrors_dict[source] = fiterrors
        templates_dict[source] = vectors
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics[source] = value - centralValue
        errors_on_systematics[source] = systematics[source] * relativeError
    
    return systematics, errors_on_systematics, values, statErrors, fits, fitvalues_dict, fiterrors_dict, templates_dict

def doPDFSystematics(centralValue, centralRelativeError, metbin, btagBin, rebin):
    base = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/BinnedMETAnalysis/'
    distribution = base + 'Electron_patType1CorrectedPFMet_bin_%s/electron_eta_%s' % (metbin, btagBin)
    #should always use 0-btag region!!                  
    qcdDistribution = base + 'QCDConversions/Electron_patType1CorrectedPFMet_bin_%s/electron_eta_0btag' % metbin
        
    systematics = {}
    errors_on_systematics = {}
    values = {}
    statErrors = {}
    fits = {}
    fitvalues_dict = {}
    fiterrors_dict = {}
    templates_dict = {}
    for index in range(1,45):
        temp_files = deepcopy(FILES.files)
        temp_files['TTJet'] = FILES.files_PDF_weights['TTJet_%d' % index]#replace distribution
        ttbarScaleFactor = 7490162/6093274
        value, error, fit, fitvalues, fiterrors = performMeasurement(distribution, qcdDistribution, temp_files, rebin)
        
        source = 'PDFWeights_%d' % index    
        values[source] = value
        statErrors[source] = error
        fits[source] = fit
        fitvalues_dict[source] = fitvalues
        fiterrors_dict[source] = fiterrors
        templates_dict[source] = vectors
        relativeError = sqrt(getRelativeError(value, error) ** 2 + centralRelativeError ** 2)
        systematics[source] = value - centralValue
        errors_on_systematics[source] = systematics[source] * relativeError
    
    return systematics, errors_on_systematics, values, statErrors, fits, fitvalues_dict, fiterrors_dict, templates_dict
         
def printResults(metBins, results, btagBin):
    printout = '\n'
    printout += '=' * 60
    printout += 'Result for %s region\n' % btagBin
    printout += '=' * 60
    printout += '\n\met bin & cross-section \\\\\n'
    printout += '\hline\n'
    total = 0
    totalStatError = 0
    totalSystError = 0
    for metbin in metBins:#, result in results.iteritems():
        result = results[metbin]
        totalUncertainty, totalUncertainty_minus, totalUncertainty_plus = calculateTotalUncertainty(result)
        total += result['centralValue']
        totalStatError += result['statError'] ** 2
        totalSystError += totalUncertainty ** 2 
        formatting = (result['centralValue'], result['statError'], totalUncertainty)
        printout += metbin + '~\GeV & %.2f $\pm$ %.2f (stat) $\pm$ %.2f (syst) pb \\\\\n' % formatting
    totalStatError = sqrt(totalStatError)
    totalSystError = sqrt(totalSystError)
    printout += '\hline'
    printout += 'Total & %.2f $\pm$ %.2f (stat) $\pm$ %.2f (syst) pb\\\\\n' % (total, totalStatError, totalSystError)
    result_file = open(savePath + 'results_%s.tex' % btagBin, 'w')
    result_file.write(printout)
    result_file.close()
    
def plotFits(metBins, results, btagBin):
    global rebin
    for metbin in metBins:
        result = results[metbin]
        fit = result['fit']
        fitvalues = result['fitvalues']
        templates = result['templates']
        plots = {}
        
        samples = [
                   'ElectronHad',
                   'W+Jets',
                   'DYJetsToLL',
                   'QCDFromData',
                   'Di-Boson',
                   'TTJet',
                   'SingleTop'
                   ]
        colors = {
                  'ElectronHad':0,
                   'W+Jets':kGreen - 3,
                   'DYJetsToLL':kAzure - 2,
                   'QCDFromData':kYellow,
                   'Di-Boson':kWhite,
                   'TTJet':kRed + 1,
                   'SingleTop': kMagenta
                  }
        for sample in samples:
            template = templates[sample]
            plot = TH1F(sample + metbin + btagBin, sample, len(template), -3, 3)
            bin = 1
            for value in template:
                plot.SetBinContent(bin, value)
                bin += 1
            plot.Scale(fitvalues[sample])
#            plot.Rebin(rebin)
            plot.SetYTitle('Events/0.2')
            plot.SetXTitle('#eta(e)')
            if not sample == 'ElectronHad':
                plot.SetFillColor(colors[sample])
            plots[sample] = plot
#        fit.Rebin(10)
        fit.SetLineColor(kViolet - 3)
        fit.SetLineWidth(5)
        c = TCanvas("Fit_" + metbin, "Differential cross section", 1600, 1200)
        max = plots['ElectronHad'].GetMaximum()
        plots['ElectronHad'].SetMaximum(max * 1.5)
        plots['ElectronHad'].Draw('error')
        mcStack = THStack("MC", "MC")
        mcStack.Add(plots['Di-Boson']);
        mcStack.Add(plots['QCDFromData']);
        mcStack.Add(plots['DYJetsToLL']);
        mcStack.Add(plots['W+Jets']);
        mcStack.Add(plots['SingleTop']);
        mcStack.Add(plots['TTJet']);
        mcStack.Draw('hist same')
        fit.Draw('same')
        plots['ElectronHad'].Draw('error same')
        
        legend = plotting.create_legend()
        legend.AddEntry(plots['ElectronHad'], "data `11", 'P')
        legend.AddEntry(plots['TTJet'], 't#bar{t}', 'F')
        legend.AddEntry(plots['W+Jets'], 'W#rightarrowl#nu', 'F')
        legend.AddEntry(plots['DYJetsToLL'], 'Z/#gamma*#rightarrowl^{+}l^{-}', 'F')
        legend.AddEntry(plots['QCDFromData'], 'QCD/#gamma + jets', 'F')
        legend.AddEntry(plots['SingleTop'], 'Single-Top', 'F')
        legend.AddEntry(plots['Di-Boson'], 'VV + X', 'F')
        legend.Draw()
        c.SaveAs(savePath + 'EPlusJets_electron_eta_fit_' + metbin + '_' + btagBin + '.png')
        c.SaveAs(savePath + 'EPlusJets_electron_eta_fit_' + metbin + '_' + btagBin + '.pdf')

def getRelativeError(value, error):
    relativeError = 0
    if not value == 0:
        relativeError = error / value
    return relativeError

def calculateTotalUncertainty(result):
    values = result['values']
    statErrors = result['statErrors']
    pdfweight_minus, pdfweight_plus = calculatePDFErrors(values, statErrors)
#    values['PDFWeightsPlus'] = pdfweight_plus
#    values['PDFWeightsMinus'] = pdfweight_minus
    centralvalue = values['central']
    centralerror = statErrors['central']
    
    totalUp = 0
    totalDown = 0
    total = 0
    
    for source, value in values.iteritems():
        if 'PDFWeights' in source:
            continue
        diff = centralvalue - value
        if diff > 0:
            totalUp += diff**2
        else:
            totalDown += diff**2
        total += diff**2
    totalUp = sqrt(totalUp + pdfweight_plus**2)
    totalDown = sqrt(totalDown + pdfweight_minus**2)
    total = sqrt(total + pdfweight_plus**2 + pdfweight_minus**2)
    
    return total, totalDown, totalUp
        
    
def calculatePDFErrors(values, errors):
    centralvalue = values['central']
    centralerror = errors['central']
    negative = []
    positive = []
    
    for index in range(1, 45):
        value = values['PDFWeights_%d' % index]
        if index % 2 == 0: #even == negative
            negative.append(value)
        else:
            positive.append(value)
    pdf_max = numpy.sqrt(sum(max(x-centralvalue, y-centralvalue, 0) for x,y in zip(negative, positive)))
    pdf_min = numpy.sqrt(sum(max(centralvalue - x, centralvalue -y, 0) for x,y in zip(negative, positive)))
    return pdf_min, pdf_max
    
            
def doNormalisedMeasurement(metBins, btagBin, results):
    results = {}
    totalFitValue = 0
    totalFitError = 0
    for metbin in metBins:
        fitvalues = results[metbin]['fitvalues']
        fiterrors = results[metbin]['fiterrors']
        nTTbar = fitvalues['TTJet']
        totalFitValue += nTTbar
        totalFitError += fiterrors['TTJet']**2
    totalFitError = sqrt(totalFitError)
    
    for metbin in metBins:
        fitvalues = results[metbin]['fitvalues']
        fiterrors = results[metbin]['fiterrors']
        
        nTTbar = fitvalues['TTJet']
        results[metbin]['centralValue_normalised'] = nTTbar/totalFitValue
        

if __name__ == "__main__":
    #TODO: implemet switchex (program options for
    #b-jet bin
    #do systematics or not
    DEBUG = False
    btagBins = [
#                '0orMoreBtag',
#                '0btag',
#                '1btag',
                '2orMoreBtags'
                ]
    metBins = ['0-25',
               '25-45',
               '45-70',
               '70-100',
               '100-inf'
               ]
    
    
    metsystematics_sources = [
 "patType1p2CorrectedPFMetElectronEnUp",
        "patType1p2CorrectedPFMetElectronEnDown",
        "patType1p2CorrectedPFMetMuonEnUp",
        "patType1p2CorrectedPFMetMuonEnDown",
        "patType1p2CorrectedPFMetTauEnUp",
        "patType1p2CorrectedPFMetTauEnDown",
        "patType1p2CorrectedPFMetJetResUp",
        "patType1p2CorrectedPFMetJetResDown",
        "patType1p2CorrectedPFMetJetEnUp",
        "patType1p2CorrectedPFMetJetEnDown",
        "patType1p2CorrectedPFMetUnclusteredEnUp",
        "patType1p2CorrectedPFMetUnclusteredEnDown"
                      ]
    
    
    for btagBin in btagBins:
        #normalisation
        distribution = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/MET/patType1CorrectedPFMet/MET_' + btagBin
#        temp_hist = FileReader.getHistogramFromFile(distribution, FILES.files['TTJet']) 
        unbinnedTTbarEvents = FileReader.getHistogramFromFile(distribution, FILES.files['TTJet']).Integral()
        unbinnedTTbarEvents_matchingDown = FileReader.getHistogramFromFile(distribution, FILES.files['TTJets-matchingdown']).Integral()
        unbinnedTTbarEvents_matchingUp = FileReader.getHistogramFromFile(distribution, FILES.files['TTJets-matchingup']).Integral()
        unbinnedTTbarEvents_scaleDown = FileReader.getHistogramFromFile(distribution, FILES.files['TTJets-scaledown']).Integral()
        unbinnedTTbarEvents_scaleUp = FileReader.getHistogramFromFile(distribution, FILES.files['TTJets-scaleup']).Integral()
        
        w1jet = FileReader.getHistogramFromFile(distribution, FILES.files['W1Jet']).Integral()
        w2jets = FileReader.getHistogramFromFile(distribution, FILES.files['W2Jets']).Integral()
        w3jets = FileReader.getHistogramFromFile(distribution, FILES.files['W3Jets']).Integral()
        w4jets = FileReader.getHistogramFromFile(distribution, FILES.files['W4Jets']).Integral()
        unbinnedWJetsEvents = w1jet + w2jets + w3jets + w4jets
        unbinnedWJetsEvents_matchingDown = FileReader.getHistogramFromFile(distribution, FILES.files['WJets-matchingdown']).Integral()
        unbinnedWJetsEvents_matchingUp = FileReader.getHistogramFromFile(distribution, FILES.files['WJets-matchingup']).Integral()
        unbinnedWJetsEvents_scaleDown = FileReader.getHistogramFromFile(distribution, FILES.files['WJets-scaledown']).Integral()
        unbinnedWJetsEvents_scaleUp = FileReader.getHistogramFromFile(distribution, FILES.files['WJets-scaleup']).Integral()
        
        unbinnedZJetsEvents = FileReader.getHistogramFromFile(distribution, FILES.files['DYJetsToLL']).Integral()
        unbinnedZJetsEvents_matchingDown = FileReader.getHistogramFromFile(distribution, FILES.files['ZJets-matchingdown']).Integral()
        unbinnedZJetsEvents_matchingUp = FileReader.getHistogramFromFile(distribution, FILES.files['ZJets-matchingup']).Integral()
        unbinnedZJetsEvents_scaleDown = FileReader.getHistogramFromFile(distribution, FILES.files['ZJets-scaledown']).Integral()
        unbinnedZJetsEvents_scaleUp = FileReader.getHistogramFromFile(distribution, FILES.files['ZJets-scaleup']).Integral()
        
#        print "Stats:"
#        print "TTbar Central:", unbinnedTTbarEvents
#        print "TTbar Q^2 up/down:", unbinnedTTbarEvents_scaleUp, '/', unbinnedTTbarEvents_scaleDown
#        print "TTbar matching up/down:", unbinnedTTbarEvents_matchingUp, '/', unbinnedTTbarEvents_matchingDown
#        print "WJets Central:", unbinnedWJetsEvents
#        print "WJets Q^2 up/down:", unbinnedWJetsEvents_scaleUp, '/', unbinnedWJetsEvents_scaleDown
#        print "WJets matching up/down:", unbinnedWJetsEvents_matchingUp, '/', unbinnedWJetsEvents_matchingDown
#        print "ZJets Central:", unbinnedZJetsEvents
#        print "ZJets Q^2 up/down:", unbinnedZJetsEvents_scaleUp, '/', unbinnedZJetsEvents_scaleDown
#        print "ZJets matching up/down:", unbinnedZJetsEvents_matchingUp, '/', unbinnedZJetsEvents_matchingDown
        
        results = doBinnedAnalysis(metBins, btagBin)
#        results_normalised = doNormalisedMeasurement(metBins, btagBin, results)
    
        printResults(metBins, results, btagBin)
        printMetSystematics(metBins, results, btagBin)
        
        makeMeasurementPlot(metBins, results, btagBin)
        plotFits(metBins, results, btagBin)
