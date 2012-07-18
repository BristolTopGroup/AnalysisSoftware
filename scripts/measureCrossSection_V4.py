from __future__ import division
from ROOT import *
import ROOT
import FILES
import tools.ROOTFileReader as FileReader
from array import array
from tools import Styles
import tools.PlottingUtilities as plotting
#import QCDRateEstimation
from copy import deepcopy
from decimal import *
import numpy
from tools.Timer import Timer
from tools.Table import Table
import QCDRateEstimation

savePath = "/storage/results/plots/DiffMETMeasurement/"
outputFormat_tables = 'latex' #other option: twiki
outputFormat_plots = ['png', 'pdf']

rebin = 10
qcdLabel = 'QCDFromData'
normalisation = None
vectors = None
N_Events = {}
N_ttbar_by_source = {}
DEBUG = False
constrains = {
              qcdLabel: {'enabled':True, 'value': 1},
              'ratio_Z_W': {'enabled':True, 'value': 0.05},
              'W+Jets': {'enabled':False, 'value': 0.3},
              'DYJetsToLL': {'enabled':False, 'value': 0.3},
#              'Di-Boson': {'enabled':False, 'value': 0.3},
              }
fit_index = 0

scale_factors = {
                 'luminosity':1,
                 'singleTop':1,
                 'TTJet':1,
                 'W+Jets':1,
                 'DYJetsToLL':1,
                 'POWHEG':7490162/4262961,
                 'PYTHIA6':7490162/1089625,
                 'MCatNLO':7490162/10504532,
                 }

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

metbins = [
           '0-25',
               '25-45',
               '45-70',
               '70-100',
               '100-inf'
               ]
metbin_widths = {
           '0-25':25,
               '25-45':20,
               '45-70':25,
               '70-100':30,
               '100-inf':50
               }  
#metbin_widths = {
#           '0-25':1,
#               '25-45':1,
#               '45-70':1,
#               '70-100':1,
#               '100-inf':1
#               }      
    
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
#steering variables:
use_fit_errors_only = False
measure_normalised_crossection = False

N_QCD = 14856
bjetbin = '0orMoreBtag'
metbin = metbins[0]
current_source = 'None'

def MinuitFitFunction(nParameters, gin, f, par, iflag):
    global normalisation, vectors, qcdLabel
    lnL = 0.0
    input = zip(vectors['ElectronHad'],
                vectors['Signal'],
                vectors['W+Jets'],
                vectors['DYJetsToLL'],
                vectors[qcdLabel],
#                vectors['Di-Boson']
                )
    
#    for data, signal, wjets, zjets, qcd, diBoson in input:
    for data, signal, wjets, zjets, qcd in input:
        #signal = ttjet + singleTop
        x_i = par[0] * signal + par[1] * wjets + par[2] * zjets + par[3] * qcd# + par[4] * diBoson#expected number of events in each bin
        data = data * normalisation['ElectronHad']
        if (data != 0) and (x_i != 0):
            L = TMath.Poisson(data, x_i)
            lnL += log(L)
    f[0] = -2.0 * lnL
    
    #constrains
    ratio_Z_W = normalisation['DYJetsToLL'] / normalisation['W+Jets']
#    ratio of Z/W to 5%
    if constrains['ratio_Z_W']['enabled']:
        if ratio_Z_W == 0 or par[1] == 0:
            f[0] += 1
        else:
            f[0] += ((par[2] / par[1] - ratio_Z_W) / (constrains['ratio_Z_W']['value'] * ratio_Z_W)) ** 2
    if constrains['W+Jets']['enabled']:
        f[0] += ((par[1] - normalisation['W+Jets']) / (constrains['W+Jets']['value'] * normalisation['W+Jets'])) ** 2
    if constrains['DYJetsToLL']['enabled']:
        f[0] += ((par[2] - normalisation['DYJetsToLL']) / (constrains['DYJetsToLL']['value'] * normalisation['DYJetsToLL'])) ** 2
    if constrains[qcdLabel]['enabled']:
        f[0] += ((par[3] - N_QCD) / (constrains[qcdLabel]['value'] * N_QCD)) ** 2
#    if constrains['Di-Boson']['enabled']:
#        f[0] += ((par[4] - normalisation['Di-Boson']) / (constrains['Di-Boson']['value'] * normalisation['Di-Boson'])) ** 2
        
def createFitHistogram(fitvalues, templates):
    global fit_index
    fit = TH1F('fit_' + str(fit_index), 'fit', len(templates['Signal']), -3, 3)
    fit_index += 1
    bin = 1
    input = zip(vectors['Signal'],
                vectors['W+Jets'],
                vectors['DYJetsToLL'],
                vectors[qcdLabel],
#                vectors['Di-Boson']
                )
#    for signal, wjets, zjets, qcd, diBoson in input:
    for signal, wjets, zjets, qcd in input:
        value = fitvalues[0] * signal + fitvalues[1] * wjets + fitvalues[2] * zjets + fitvalues[3] * qcd #+ fitvalues[4] * diBoson
        fit.SetBinContent(bin, value)
        bin += 1
    return fit.Clone()

def getFittedNormalisation(vectors_={}, normalisation_={}):
    global normalisation, vectors, bjetbin
    normalisation = normalisation_
    vectors = vectors_
        
    #setup minuit
    numberOfParameters = 4#5
    gMinuit = TMinuit(numberOfParameters)
    gMinuit.SetFCN(MinuitFitFunction)
    gMinuit.SetPrintLevel(-1)
    #Error definition: 1 for chi-squared, 0.5 for negative log likelihood
    gMinuit.SetErrorDef(1)
    #error flag for functions passed as reference.set to as 0 is no error
    errorFlag = ROOT.Long(2)
    
    N_total = normalisation['ElectronHad']
#    N_total = 1e6
    N_min = 0
    
    N_QCD = normalisation[qcdLabel]
#    N_QCD = normalisation['QCD']
    if DEBUG:
        print len(vectors['ElectronHad']), len(vectors['Signal']), len(vectors['W+Jets']), len(vectors['DYJetsToLL']), len(vectors['QCDFromData'])
        print "Total number of data events before the fit: ", N_total
    N_signal = normalisation['TTJet'] + normalisation['SingleTop']
    gMinuit.mnparm(0, "N_signal(ttbar+single_top)", N_signal, 10.0, N_min, N_total, errorFlag)
    gMinuit.mnparm(1, "W+Jets", normalisation['W+Jets'], 10.0, N_min, N_total, errorFlag)
    gMinuit.mnparm(2, "DYJetsToLL", normalisation['DYJetsToLL'], 10.0, N_min, N_total, errorFlag)
    gMinuit.mnparm(3, "QCD", N_QCD, 10.0, N_min, N_total, errorFlag)
#    gMinuit.mnparm(4, "Di-Boson", normalisation['Di-Boson'], 10.0, 0, N_total, errorFlag)
    
    arglist = array('d', 10 * [0.])
    #minimisation strategy: 1 standard, 2 try to improve minimum (a bit slower)
    arglist[0] = 2
    #minimisation itself
    gMinuit.mnexcm("SET STR", arglist, 1, errorFlag)
    gMinuit.Migrad()
    
    fitvalues, fiterrors = [], []
    for index in range(numberOfParameters):
        temp_par = Double(0)
        temp_err = Double(0)
        gMinuit.GetParameter(index, temp_par, temp_err)
        fitvalues.append(temp_par)
        fiterrors.append(temp_err)
    
    N_ttbar = fitvalues[0] - normalisation['SingleTop'] 
    N_ttbar_err = (fitvalues[0] - normalisation['SingleTop']) * fiterrors[0] / fitvalues[0]
    result = {'Signal': {'value': fitvalues[0], 'error':fiterrors[0]},
              'W+Jets': {'value': fitvalues[1], 'error':fiterrors[1]},
              'DYJetsToLL': {'value': fitvalues[2], 'error':fiterrors[2]},
              qcdLabel: {'value': fitvalues[3], 'error':fiterrors[3]},
#              'Di-Boson': {'value': fitvalues[4], 'error':fiterrors[4]},
              'TTJet': {'value': N_ttbar, 'error' : N_ttbar_err},
              'ElectronHad': {'value': normalisation['ElectronHad'], 'error':0},
              'SingleTop': {'value': normalisation['SingleTop'], 'error':0},
              'TTJet Before Fit': {'value': normalisation['TTJet'], 'error':0},
              'QCD Before Fit': {'value': N_QCD, 'error':0},
              'W+Jets BeforeFit': {'value': normalisation['W+Jets'], 'error':0},
              'DYJetsToLL Before Fit': {'value': normalisation['DYJetsToLL'], 'error':0},
              'fit': createFitHistogram(fitvalues, vectors),
              'vectors':vectors,
              #other generators
              'POWHEG': {'value': normalisation['POWHEG']*scale_factors['POWHEG'], 'error':0},
              'PYTHIA6': {'value': normalisation['PYTHIA6']*scale_factors['PYTHIA6'], 'error':0},
              'MCatNLO': {'value': normalisation['MCatNLO']*scale_factors['MCatNLO'], 'error':0},
              }
    return result

def measureNormalisationIn(histogram):
    
    normalisation = getNormalisation(histogram)
    templates = getTemplates(histogram)
    vectors = vectorise(templates)
    qcdHistForEstimation = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/BinnedMETAnalysis/QCD e+jets PFRelIso/Electron_patType1CorrectedPFMet_bin_%s/electron_pfIsolation_03_%s'
    qcdHistForEstimation = qcdHistForEstimation % (metbin, bjetbin)
    qcdResult = QCDRateEstimation.estimateQCDWithRelIso(FILES.files, qcdHistForEstimation)
    normalisation[qcdLabel] = qcdResult['estimate']
    
    if DEBUG:
        printNormalisation(normalisation)
    fitted_result = getFittedNormalisation(vectors, normalisation)
    if DEBUG:
        printFittedResult(fitted_result)
    return fitted_result

def measureNormalisationIncludingSystematics(histograms):
    global current_source
    fitted_results = {}
    print 'Performing central measurement'
    timer = Timer()
    histogram = histograms['central']
    histogram['QCDFromData'] = histogram['QCDFromData_Conversions']
    fitted_results['central'] = measureNormalisationIn(histogram)
    print '>' * 80, 'completed in %.2fs' % timer.elapsedTime()
    timer.restart()
    print 'Performing measurement of systematic uncertainties (lumi, electron efficiency, single top cross-section)'
    #electron efficiency += 3%
    current_source = 'Electron Efficiency'
    scale_factors['luminosity'] = 1. + 0.03
    fitted_results['Electron Efficiency +'] = measureNormalisationIn(histogram)
    scale_factors['luminosity'] = 1. - 0.03
    fitted_results['Electron Efficiency -'] = measureNormalisationIn(histogram)
    #luminosity uncertainty +- 2.2%
    current_source = 'luminosity'
    scale_factors['luminosity'] = 1. + 0.022
    fitted_results['Luminosity +'] = measureNormalisationIn(histogram)
    scale_factors['luminosity'] = 1. - 0.022
    fitted_results['Luminosity -'] = measureNormalisationIn(histogram)
    scale_factors['luminosity'] = 1.#reset
    #single top cross-section: +-30%
    current_source = 'singleTop'
    scale_factors['singleTop'] = 1. + 0.3
    fitted_results['SingleTop +'] = measureNormalisationIn(histogram)
    scale_factors['singleTop'] = 1. - 0.3
    fitted_results['SingleTop -'] = measureNormalisationIn(histogram)
    scale_factors['singleTop'] = 1.#reset
    print '>' * 80, 'completed in %.2fs' % timer.elapsedTime()
    timer.restart()
    print 'Performing measurement of QCD shape uncertainty, JES and PU uncertainties'
    #QCD shape
    current_source = 'QCD shape'
    histogram['QCDFromData'] = histogram['QCDFromData_AntiIsolated']
    fitted_results['QCD shape'] = measureNormalisationIn(histogram)
    timer.restart()
    #jet energy scale
    current_source = 'JES'
    histogram = histograms['JES+']
    histogram['QCDFromData'] = histogram['QCDFromData_Conversions']
    fitted_results['JES+'] = measureNormalisationIn(histogram)
    histogram = histograms['JES-']
    histogram['QCDFromData'] = histogram['QCDFromData_Conversions']
    fitted_results['JES-'] = measureNormalisationIn(histogram)
    #inelastic cross-section for pile-up calculation +- 5%
    current_source = 'PileUp'
    histogram = histograms['PileUp+']
    histogram['QCDFromData'] = histogram['QCDFromData_Conversions']
    fitted_results['PileUp+'] = measureNormalisationIn(histogram)
    histogram = histograms['PileUp-']
    histogram['QCDFromData'] = histogram['QCDFromData_Conversions']
    fitted_results['PileUp-'] = measureNormalisationIn(histogram)
    print '>' * 60, 'completed in %.2fs' % timer.elapsedTime()
    timer.restart()
    
    print 'Performing measurement of matching and scale systematic uncertainties'
    #matching threshold ttbar: 20 GeV -> 10 GeV & 40GeV
    current_source = 'TTJet matching'
    histogram = histograms['central']
    histogram['QCDFromData'] = histogram['QCDFromData_Conversions']
    ttjet_temp = deepcopy(histogram['TTJet'])
    histogram['TTJet'] = histogram['TTJets-matchingup']
    scale_factors['TTJet'] = N_Events['TTJet'] / N_Events['TTJets-matchingup']
    fitted_results['TTJet matching +'] = measureNormalisationIn(histogram)
    histogram['TTJet'] = histogram['TTJets-matchingdown']
    scale_factors['TTJet'] = N_Events['TTJet'] / N_Events['TTJets-matchingdown']
    fitted_results['TTJet matching -'] = measureNormalisationIn(histogram)
    #Q^2 scale ttbar
    current_source = 'TTJet scale'
    histogram['TTJet'] = histogram['TTJets-scaleup']
    scale_factors['TTJet'] = N_Events['TTJet'] / N_Events['TTJets-scaleup']
    fitted_results['TTJet scale +'] = measureNormalisationIn(histogram)
    histogram['TTJet'] = histogram['TTJets-scaledown']
    scale_factors['TTJet'] = N_Events['TTJet'] / N_Events['TTJets-scaledown']
    fitted_results['TTJet scale -'] = measureNormalisationIn(histogram)
    #reset
    scale_factors['TTJet'] = 1
    histogram['TTJet'] = ttjet_temp
    #matching threshold W+Jets
    current_source = 'W+Jets matching'
    wjets_temp = deepcopy(histogram['W+Jets'])
    histogram['W+Jets'] = histogram['WJets-matchingup']
    scale_factors['W+Jets'] = N_Events['W+Jets'] / N_Events['WJets-matchingup']
    fitted_results['W+Jets matching +'] = measureNormalisationIn(histogram)
    histogram['W+Jets'] = histogram['WJets-matchingdown']
    scale_factors['W+Jets'] = N_Events['W+Jets'] / N_Events['WJets-matchingdown']
    fitted_results['W+Jets matching -'] = measureNormalisationIn(histogram)
    #Q^2 scale W+Jets
    current_source = 'W+Jets scale'
    histogram['W+Jets'] = histogram['WJets-scaleup']
    scale_factors['W+Jets'] = N_Events['W+Jets'] / N_Events['WJets-scaleup']
    fitted_results['W+Jets scale +'] = measureNormalisationIn(histogram)
    histogram['W+Jets'] = histogram['WJets-scaledown']
    scale_factors['W+Jets'] = N_Events['W+Jets'] / N_Events['WJets-scaledown']
    fitted_results['W+Jets scale -'] = measureNormalisationIn(histogram)
    #reset
    scale_factors['W+Jets'] = 1
    histogram['W+Jets'] = wjets_temp
    #matching threshold Z+Jets
    current_source = 'Z+Jets matching'
    zjets_temp = deepcopy(histogram['DYJetsToLL'])
    histogram['DYJetsToLL'] = histogram['ZJets-matchingup']
    scale_factors['DYJetsToLL'] = N_Events['DYJetsToLL'] / N_Events['ZJets-matchingup']
    fitted_results['Z+Jets matching +'] = measureNormalisationIn(histogram)
    histogram['DYJetsToLL'] = histogram['ZJets-matchingdown']
    scale_factors['DYJetsToLL'] = N_Events['DYJetsToLL'] / N_Events['ZJets-matchingdown']
    fitted_results['Z+Jets matching -'] = measureNormalisationIn(histogram)
    #Q^2 scale Z+Jets
    current_source = 'Z+Jets scale'
    histogram['DYJetsToLL'] = histogram['ZJets-scaleup']
    scale_factors['DYJetsToLL'] = N_Events['DYJetsToLL'] / N_Events['ZJets-scaleup']
    fitted_results['Z+Jets scale +'] = measureNormalisationIn(histogram)
    histogram['DYJetsToLL'] = histogram['ZJets-scaledown']
    scale_factors['DYJetsToLL'] = N_Events['DYJetsToLL'] / N_Events['ZJets-scaledown']
    fitted_results['Z+Jets scale -'] = measureNormalisationIn(histogram)
    #reset
    scale_factors['DYJetsToLL'] = 1
    histogram['DYJetsToLL'] = zjets_temp
    print '>' * 60, 'completed in %.2fs' % timer.elapsedTime()
    timer.restart()
    print 'Performing measurement of MET systematic uncertainties'
    for source in metsystematics_sources:
        current_source = source
        histogram = histograms[source]
        histogram['QCDFromData'] = histogram['QCDFromData_Conversions']
        if 'JetRes' in source:
            histogram['QCDFromData'] = histograms['central']['QCDFromData_Conversions']
            histogram['ElectronHad'] = histograms['central']['ElectronHad']
        fitted_results[source] = measureNormalisationIn(histogram)
    print '>' * 60, 'completed in %.2fs' % timer.elapsedTime()
    timer.restart()
        
    print 'Performing measurement of PDF uncertainties'
    histogram_pdf = histograms['PDFWeights']
    histogram = histograms['central']
    ttjet_temp = deepcopy(histogram['TTJet'])
    scale_factors['TTJet'] = 7490162 / 6093274
    for index in range(1, 45):
        pdf = 'TTJet_%d' % index
        current_source = pdf
        histogram['TTJet'] = histogram_pdf[pdf]
        fitted_results['PDFWeights_%d' % index] = measureNormalisationIn(histogram)
    scale_factors['TTJet'] = 1.   
    histogram['TTJet'] = ttjet_temp
    print '>' * 60, 'completed in %.2fs' % timer.elapsedTime()
    return fitted_results


def NormalisationAnalysis():
    global metbins, metsystematics_sources, N_Events, metbin
    analysisTimer = Timer()
    base = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/BinnedMETAnalysis/'
    
    setNEvents(bjetbin)
    setNTtbar(bjetbin)
    
    result = {}
    for metbin in metbins:
        metbinTimer = Timer()
        #loadfiles
        histogramCollection = getHistograms(bjetbin, metbin)
        #sum samples
        histogramCollection['central'] = sumSamples(histogramCollection['central'])
        histogramCollection['JES-'] = sumSamples(histogramCollection['JES-'])
        histogramCollection['JES+'] = sumSamples(histogramCollection['JES+'])
        histogramCollection['PileUp-'] = sumSamples(histogramCollection['PileUp-'])
        histogramCollection['PileUp+'] = sumSamples(histogramCollection['PileUp+'])
        for source in metsystematics_sources:
            histogramCollection[source] = sumSamples(histogramCollection[source])
        print 'Getting fitted normalisation for metbin=', metbin
        result[metbin] = measureNormalisationIncludingSystematics(histogramCollection)
        print 'Result for metbin=', metbin, 'completed in %.2fs' % metbinTimer.elapsedTime()
    print 'Analysis in bjetbin=', bjetbin, 'finished in %.2fs' % analysisTimer.elapsedTime()
    return result


def CrossSectionAnalysis(input):
    global N_ttbar_by_source
    result = {}
    theoryXsection = 157.5
    
    for metbin in metbins:
        result[metbin] = {}
        for measurement in input[metbin].keys():
            result_ttbar = input[metbin][measurement]['TTJet']
            madgraph_ttbar = input[metbin][measurement]['TTJet Before Fit']['value']
            value, error = result_ttbar['value'], result_ttbar['error']
            n_ttbar = N_ttbar_by_source[measurement]
            scale = theoryXsection / n_ttbar
            result[metbin][measurement] = {'value': value * scale, 
                                           'error':error * scale, 
                                           'MADGRAPH':madgraph_ttbar * scale,
                                           'POWHEG':input[metbin][measurement]['POWHEG']['value'] * scale,
                                           'PYTHIA6':input[metbin][measurement]['PYTHIA6']['value'] * scale,
                                           'MCatNLO':input[metbin][measurement]['POWHEG']['value'] * scale}
    return result
        

def NormalisedCrossSectionAnalysis(input):
    global N_ttbar_by_source
    result = {}
    theoryXsection = 157.5
    sums = {'central':{},'MADGRAPH':{},'POWHEG':{},'PYTHIA6':{},'MCatNLO':{} }
    for metbin in metbins:
        result[metbin] = {}
        for measurement in input[metbin].keys():
            if not sums['central'].has_key(measurement):
                sums['central'][measurement] = input[metbin][measurement]['TTJet']['value']
                sums['MADGRAPH'][measurement] = input[metbin][measurement]['TTJet Before Fit']['value']
                sums['POWHEG'][measurement] = input[metbin][measurement]['POWHEG']['value']
                sums['PYTHIA6'][measurement] = input[metbin][measurement]['PYTHIA6']['value']
                sums['MCatNLO'][measurement] = input[metbin][measurement]['MCatNLO']['value']
            else:
                sums['central'][measurement] += input[metbin][measurement]['TTJet']['value']
                sums['MADGRAPH'][measurement] += input[metbin][measurement]['TTJet Before Fit']['value']
                sums['POWHEG'][measurement] += input[metbin][measurement]['POWHEG']['value']
                sums['PYTHIA6'][measurement] += input[metbin][measurement]['PYTHIA6']['value']
                sums['MCatNLO'][measurement] += input[metbin][measurement]['MCatNLO']['value']
    
    for metbin in metbins:
        result[metbin] = {}
        for measurement in input[metbin].keys():            
            result_ttbar = input[metbin][measurement]['TTJet']
            madgraph_ttbar = input[metbin][measurement]['TTJet Before Fit']['value']
            value, error = result_ttbar['value'], result_ttbar['error']
            
            result[metbin][measurement] = {'value': value/sums['central'][measurement], 
                                           'error':error/sums['central'][measurement], 
                                           'MADGRAPH':input[metbin][measurement]['TTJet Before Fit']['value']/sums['MADGRAPH'][measurement],
                                           'POWHEG':input[metbin][measurement]['POWHEG']['value']/sums['POWHEG'][measurement],
                                           'PYTHIA6':input[metbin][measurement]['PYTHIA6']['value']/sums['PYTHIA6'][measurement],
                                           'MCatNLO':input[metbin][measurement]['MCatNLO']['value']/sums['MCatNLO'][measurement]}
    return result

def getHistograms(bjetbin, metbin):
    print 'Getting histograms for bjetbin =', bjetbin, 'and metbin=', metbin
    global metsystematics_sources, rebin
    base = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/BinnedMETAnalysis/'
    distribution = base + 'Electron_patType1CorrectedPFMet_bin_%s/electron_eta_%s' % (metbin, bjetbin)
    qcdDistribution = base + 'QCDConversions/Electron_patType1CorrectedPFMet_bin_%s/electron_eta_0btag' % metbin
    qcdDistribution2 = base + 'QCD non iso e+jets/Electron_patType1CorrectedPFMet_bin_%s/electron_eta_0btag' % metbin

    histogramCollection = {}
    histogramCollection['central'] = FileReader.getHistogramDictionary(distribution, FILES.files)
    histogramCollection['central']['QCDFromData_Conversions'] = FileReader.getHistogramFromFile(qcdDistribution, FILES.files['ElectronHad'])
    histogramCollection['central']['QCDFromData_AntiIsolated'] = FileReader.getHistogramFromFile(qcdDistribution2, FILES.files['ElectronHad'])
    histogramCollection['JES-'] = FileReader.getHistogramDictionary(distribution, FILES.files_JES_down)
    histogramCollection['JES-']['QCDFromData_Conversions'] = FileReader.getHistogramFromFile(qcdDistribution, FILES.files_JES_down['ElectronHad'])
    histogramCollection['JES+'] = FileReader.getHistogramDictionary(distribution, FILES.files_JES_up)
    histogramCollection['JES+']['QCDFromData_Conversions'] = FileReader.getHistogramFromFile(qcdDistribution, FILES.files_JES_up['ElectronHad'])
    histogramCollection['PileUp-'] = FileReader.getHistogramDictionary(distribution, FILES.files_PU_down)
    histogramCollection['PileUp-']['QCDFromData_Conversions'] = deepcopy(histogramCollection['central']['QCDFromData_Conversions'])
    histogramCollection['PileUp+'] = FileReader.getHistogramDictionary(distribution, FILES.files_PU_up)
    histogramCollection['PileUp+']['QCDFromData_Conversions'] = deepcopy(histogramCollection['central']['QCDFromData_Conversions'])
    histogramCollection['PDFWeights'] = FileReader.getHistogramDictionary(distribution, FILES.files_PDF_weights)  
    
    for source in metsystematics_sources:
        distribution = base + 'Electron_%s_bin_%s/electron_eta_%s' % (source, metbin, bjetbin)
        qcdDistribution = base + 'QCDConversions/Electron_%s_bin_%s/electron_eta_0btag' % (source, metbin)
        if not 'JER' in source:
            histogramCollection[source] = FileReader.getHistogramDictionary(distribution, FILES.files)
            histogramCollection[source]['QCDFromData_Conversions'] = FileReader.getHistogramFromFile(qcdDistribution, FILES.files['ElectronHad'])
        else:
            mcFiles = deepcopy(FILES.files)
            mcFiles.pop('ElectronHad')#removes data
            histogramCollection[source] = FileReader.getHistogramDictionary(distribution, FILES.files)
            histogramCollection[source]['QCDFromData_Conversions'] = deepcopy(histogramCollection['central']['QCDFromData_Conversions'])
    
    for source in histogramCollection.keys():
        hists = histogramCollection[source]
        hists = plotting.rebin(hists, rebin)#rebin to 200 bins
        hists = plotting.setYTitle(hists, title="Events/%.2f" % (0.02 * rebin))
    return histogramCollection

def setNEvents(bjetbin):
    global N_Events, wplusjets_samples
    unbinnedHist = FileReader.getHistogramDictionary('TTbarEplusJetsPlusMetAnalysis/Ref selection/MET/patType1CorrectedPFMet/MET_' + bjetbin,
                                                     FILES.files)
    unbinnedHist['W+Jets'] = plotting.sumSamples(unbinnedHist, wplusjets_samples)
    N_Events['TTJet'] = unbinnedHist['TTJet'].Integral()
    N_Events['W+Jets'] = unbinnedHist['W+Jets'].Integral()
    N_Events['DYJetsToLL'] = unbinnedHist['DYJetsToLL'].Integral()
    
    N_Events['TTJets-matchingup'] = unbinnedHist['TTJets-matchingup'].Integral()
    N_Events['TTJets-matchingdown'] = unbinnedHist['TTJets-matchingdown'].Integral()
    N_Events['TTJets-scaleup'] = unbinnedHist['TTJets-scaleup'].Integral()
    N_Events['TTJets-scaledown'] = unbinnedHist['TTJets-scaledown'].Integral()
    
    N_Events['WJets-matchingup'] = unbinnedHist['WJets-matchingup'].Integral()
    N_Events['WJets-matchingdown'] = unbinnedHist['WJets-matchingdown'].Integral()
    N_Events['WJets-scaleup'] = unbinnedHist['WJets-scaleup'].Integral()
    N_Events['WJets-scaledown'] = unbinnedHist['WJets-scaledown'].Integral()
    
    N_Events['ZJets-matchingup'] = unbinnedHist['ZJets-matchingup'].Integral()
    N_Events['ZJets-matchingdown'] = unbinnedHist['ZJets-matchingdown'].Integral()
    N_Events['ZJets-scaleup'] = unbinnedHist['ZJets-scaleup'].Integral()
    N_Events['ZJets-scaledown'] = unbinnedHist['ZJets-scaledown'].Integral()
    
def setNTtbar(bjetbin):
    global N_ttbar_by_source
    histname = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/MET/patType1CorrectedPFMet/MET_' + bjetbin
    getHist = FileReader.getHistogramFromFile
    central = getHist(histname, FILES.files['TTJet']).Integral()
    sameAsCentral = ['central', 'SingleTop +', 'SingleTop -', 'QCD shape', 'TTJet matching +', 'TTJet matching -',
                     'TTJet scale +', 'TTJet scale -', 'W+Jets matching +', 'W+Jets matching -', 'W+Jets scale +',
                     'W+Jets scale -', 'Z+Jets matching +', 'Z+Jets matching -', 'Z+Jets scale +', 'Z+Jets scale -']
    for source in sameAsCentral:
        N_ttbar_by_source[source] = central
    
    N_ttbar_by_source['Electron Efficiency +'] = central * (1. + 0.03)
    N_ttbar_by_source['Electron Efficiency -'] = central * (1. - 0.03)
    N_ttbar_by_source['Luminosity +'] = central * (1. + 0.022)
    N_ttbar_by_source['Luminosity -'] = central * (1. - 0.03)
    N_ttbar_by_source['JES-'] = getHist(histname, FILES.files_JES_down['TTJet']).Integral()
    N_ttbar_by_source['JES+'] = getHist(histname, FILES.files_JES_up['TTJet']).Integral()
    N_ttbar_by_source['PileUp-'] = getHist(histname, FILES.files_PU_down['TTJet']).Integral()
    N_ttbar_by_source['PileUp+'] = getHist(histname, FILES.files_PU_up['TTJet']).Integral()
    
    
    for index in range(1, 45):
        file = 'TTJet_%d' % index
        pdf = 'PDFWeights_%d' % index
        N_ttbar_by_source[pdf] = getHist(histname, FILES.files_PDF_weights[file]).Integral()
    
    
    for source in metsystematics_sources:
        histname = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/MET/%s/MET_%s' % (source, bjetbin)
        N_ttbar_by_source[source] = getHist(histname, FILES.files['TTJet']).Integral()
    
def prepareHistogramCollections(histogramCollection):
    global metsystematics_sources
    
def printNormalisation(normalisation_):
    global qcdLabel, metbin, current_source
    sumMC = normalisation_['SumMC'] - normalisation_['QCD'] + normalisation_[qcdLabel]
    print '*' * 120
    print 'MET bin: ', metbin
    print 'source:', current_source
    print "Input parameters:"
    print 'signal (ttbar+single top):', normalisation_['TTJet'] + normalisation_['SingleTop']
    print 'W+Jets:', normalisation_['W+Jets']
    print 'Z+Jets:', normalisation_['DYJetsToLL'] 
    print qcdLabel, ':', normalisation_[qcdLabel]
    print 'SingleTop :', normalisation_['SingleTop'] 
    print 'TTJet :', normalisation_['TTJet']  
#    print 'Di-Boson:', normalisation_['Di-Boson']
    print 'SumMC:', sumMC
    print 'Total data', normalisation_['ElectronHad']
    if not normalisation_['ElectronHad'] == 0:
        print '(N_{data} - N_{SumMC})/N_{data}', (normalisation_['ElectronHad'] - sumMC) / normalisation_['ElectronHad']
    print '*' * 120
    
def printFittedResult(fitted_result):
    global current_source
    sumMC = sum([fitted_result['Signal']['value'],
                 fitted_result['W+Jets']['value'],
                 fitted_result['DYJetsToLL']['value'],
                 fitted_result[qcdLabel]['value'],
#                 fitted_result['Di-Boson']
                ]
                )
    print '*' * 120
    print 'MET bin: ', metbin
    print 'source:', current_source
    print "Fit values:"
    print 'signal (ttbar+single top):', fitted_result['Signal']['value'], '+-', fitted_result['Signal']['error']
    print 'W+Jets:', fitted_result['W+Jets']['value'], '+-', fitted_result['W+Jets']['error']
    print 'Z+Jets:', fitted_result['DYJetsToLL']['value'], '+-', fitted_result['DYJetsToLL']['error']
    print 'QCD:', fitted_result[qcdLabel]['value'], '+-', fitted_result[qcdLabel]['error']
    print 'SingleTop (no fit):', fitted_result['SingleTop']['value'], '+-', fitted_result['SingleTop']['error']
    print 'TTJet (signal fit - SingleTop):', fitted_result['TTJet']['value'], '+-', fitted_result['TTJet']['error']
#    print 'Di-Boson:', fitted_result['Di-Boson']['value'], '+-', fitted_result['Di-Boson']['error']
    print 'SumMC:', sumMC
    N_data = fitted_result['ElectronHad']['value']
    print 'Total data:', N_data
    print '(N_{data} - N_{SumMC})/N_{data}:', (N_data - sumMC) / N_data
    print '*' * 120
    
def getNormalisation(histograms):
    global scale_factors
    normalisation_ = {}
    for sample in histograms.keys():
        normalisation_[sample] = histograms[sample].Integral()
        if not sample == 'ElectronHad' or sample == 'QCDFromData':
           normalisation_[sample] = normalisation_[sample] * scale_factors['luminosity']
        if sample == 'SingleTop':
            normalisation_[sample] = normalisation_[sample] * scale_factors['singleTop']
        if sample == 'TTJet':
            normalisation_[sample] = normalisation_[sample] * scale_factors['TTJet']
        if sample == 'W+Jets':
            normalisation_[sample] = normalisation_[sample] * scale_factors['W+Jets']
        if sample == 'DYJetsToLL':
            normalisation_[sample] = normalisation_[sample] * scale_factors['DYJetsToLL']
    return normalisation_

def getTemplates(histograms):
    templates = {}
    for sample in histograms.keys():
        hist = deepcopy(histograms[sample].Clone())
        hist.Sumw2()
        templates[sample] = plotting.normalise(hist)
    return templates 

def vectorise(histograms):
    values = {}
#    errors = {}
    for sample in histograms.keys():
        hist = histograms[sample]
        nBins = hist.GetNbinsX()
        for bin in range(1, nBins + 1):
            if not values.has_key(sample):
                values[sample] = []
            values[sample].append(hist.GetBinContent(bin))
    return values

def printNormalisationResult(result, toFile=True):
    global metbins
    printout = '\n'
    printout += '=' * 60
    printout = '\n'
    printout += 'Results for %s region\n' % bjetbin
    printout += '=' * 60
    printout += '\n'
    rows = {}
    header = 'Sample'
    for metbin in metbins:
        header += '& N_{events}^(fit) \met bin %s~\GeV' % metbin
        for source in result[metbin].keys():
            results = result[metbin][source]
            if not rows.has_key(source):
                rows[source] = {}
            
            for sample in results.keys():
                if sample == 'fit' or sample == 'vectors':
                    continue
                fitresult = results[sample]
                row = rows[source]
                
                text = ' $%(value).2f \pm %(error).2f$' % fitresult + '(%.2f' % (getRelativeError(fitresult['value'], fitresult['error']) * 100) + '\%)'
                if row.has_key(sample): 
                    row[sample].append(text)
                else:
                    row[sample] = [sample, text]
    header += '\\\\ \n'
    printout += 'Central measurement \n\n'
    printout += header
    printout += '\hline\n'
    for sample in sorted(rows['central'].keys()):
        results = rows['central'][sample]
        for result in results:
            printout += result + '&'
        printout = printout.rstrip('&')
        printout += '\\\\ \n'
    printout += '\hline\n\n'
    
    for source in sorted(rows.keys()):
        if source == 'central':
            continue
        printout += source + ' measurement \n\n'
        printout += header
        printout += '\hline \n'
        for sample in sorted(rows[source].keys()):
            results = rows[source][sample]
            for result in results:
                printout += result + '&'
            printout = printout.rstrip('&')
            printout += '\\\\ \n'
        printout += '\hline \n\n'
        
    if toFile:
        file = open(savePath + 'normalisation_result_' + bjetbin + '.tex', 'w')
        file.write(printout)
        file.close()
    else:
        print printout

def getRelativeError(value, error):
    relativeError = 0
    if not value == 0:
        relativeError = error / value
    return relativeError
        
def sumSamples(hists):
    global signal_samples, allMC_samples, qcd_samples, singleTop_samples, diboson_samples, wplusjets_samples
    hists['QCD'] = plotting.sumSamples(hists, qcd_samples)
    hists['SingleTop'] = plotting.sumSamples(hists, singleTop_samples)
    hists['Di-Boson'] = plotting.sumSamples(hists, diboson_samples)
    hists['W+Jets'] = plotting.sumSamples(hists, wplusjets_samples)
    hists['SumMC'] = plotting.sumSamples(hists, allMC_samples)
    hists['Signal'] = plotting.sumSamples(hists, signal_samples)
    return hists

#change to calculate uncertainties
def calculateTotalUncertainty(results, ommitTTJetsSystematics = False):
    pdf_min, pdf_max = calculatePDFErrors(results)
    centralResult = results['central']
    if centralResult.has_key('TTJet'):
        centralResult = results['central']['TTJet']
    centralvalue, centralerror = centralResult['value'], centralResult['error']
    totalMinus, totalPlus = pdf_min ** 2 , pdf_max ** 2
    totalMinus_err, totalPlus_err = 0, 0
    totalMETMinus, totalMETPlus = 0,0
    totalMETMinus_err, totalMETPlus_err = 0,0
    uncertainty = {}
    for source in results.keys():
        if source == 'central' or 'PDFWeights_' in source:
            continue
        if ommitTTJetsSystematics and source in ['TTJet scale -', 'TTJet scale +', 'TTJet matching -', 'TTJet matching +']:
            continue
        result = results[source]
        if result.has_key('TTJet'):
            result = results['central']['TTJet']
        value, error = result['value'], result['error']
        diff = value - centralvalue
        diff_error = sqrt((centralerror / centralvalue) ** 2 + (error / value) ** 2) * abs(diff)
        uncertainty[source] = {'value':diff, 'error':diff_error}
        if diff > 0:
            totalPlus += diff ** 2
            totalPlus_err += diff_error ** 2
        else:
            totalMinus += diff ** 2
            totalMinus_err += diff_error ** 2
            
        if source in metsystematics_sources:
            if diff > 0:
                totalMETPlus += diff ** 2
                totalMETPlus_err += diff_error ** 2
            else:
                totalMETMinus += diff ** 2
                totalMETMinus_err += diff_error ** 2
        
    total = sqrt(totalPlus + totalMinus)
    total_error = sqrt(totalPlus_err + totalMinus_err)
    totalPlus, totalMinus, totalPlus_err,  totalMinus_err = (sqrt(totalPlus), sqrt(totalMinus), 
                                                             sqrt(totalPlus_err), sqrt(totalMinus_err))
    
    totalMETPlus, totalMETMinus, totalMETPlus_err,  totalMETMinus_err = (sqrt(totalMETPlus), sqrt(totalMETMinus), 
                                                             sqrt(totalMETPlus_err), sqrt(totalMETMinus_err))
    uncertainty['Total+'] = {'value':totalPlus, 'error':totalPlus_err}
    uncertainty['Total-'] = {'value':totalMinus, 'error':totalMinus_err}
    uncertainty['Total'] = {'value':total, 'error':total_error}
    uncertainty['TotalMETUnc+'] = {'value':totalMETPlus, 'error':totalMETPlus_err}
    uncertainty['TotalMETUnc-'] = {'value':totalMETMinus, 'error':totalMETMinus_err}
    uncertainty['PDFWeights+'] = {'value':pdf_max, 'error':0}
    uncertainty['PDFWeights-'] = {'value':pdf_min, 'error':0}
    
    return uncertainty
    #uncertainty = {total, totalMinu, totalPlus, PDFs ....}
    #keys = sources + total, totalMinu, totalPlus, PDFs - central
#    return total, totalMinus, totalPlus
    
def printNormalisationResultsForTTJetWithUncertanties(result, toFile=True):
    global metbins
    printout = '\n'
    printout += '=' * 60
    printout = '\n'
    printout += 'Results for %s region\n' % bjetbin
    printout += '=' * 60
    printout += '\n'
    rows = {}
    printout += '\met bin & N_{t\\bar{t}}^{fit} \\\\ \n'
    printout += '\hline\n'
    uncertainties = {}
    for metbin in metbins:
        centralresult = result[metbin]['central']['TTJet']
        uncertainty = calculateTotalUncertainty(result[metbin])
        formatting = (metbin, centralresult['value'], centralresult['error'], uncertainty['Total+']['value'], uncertainty['Total-']['error'])
        text = '%s~\GeV & $%.2f \pm %.2f (fit)^{+%.2f}_{-%.2f} (sys)$ \\\\ \n' % formatting
        printout += text
        
        for source, value in uncertainty.iteritems():
            unc_result = value
            if not uncertainties.has_key(source):
                uncertainties[source] = '\n'
                uncertainties[source] += '=' * 60
                uncertainties[source] = '\n'
                uncertainties[source] += 'Results for %s region, source = %s\n' % (bjetbin, source)
                uncertainties[source] += '=' * 60
                uncertainties[source] += '\n'
            formatting = (metbin, 
                          unc_result['value'],   
                          unc_result['error'], 
                          ('%.2f' % (unc_result['value']/centralresult['value']*100))+ '\%')
            text = '%s~\GeV & $%.2f \pm %.2f (fit) (%s of central result)$ \\\\ \n' % formatting
            uncertainties[source] += text
        
    if toFile:
        file = open(savePath + 'normalisation_TTJet_result_' + bjetbin + '.tex', 'w')
        file.write(printout)
        for source, value in uncertainties.iteritems():
            file.write(value)
        file.close()
    else:
        print printout

def printCrossSectionResult(result, toFile=True):    
    global metbins
    printout = '\n'
    printout += '=' * 60
    printout = '\n'
    printout += 'Results for %s region\n' % bjetbin
    printout += '=' * 60
    printout += '\n'
    rows = {}
    header = 'Measurement'
    for metbin in metbins:
        width = metbin_widths[metbin]
        scale = 1/width
        header += '& $\sigma_{meas}$ \met bin %s~\GeV' % metbin
        for source in result[metbin].keys():
            fitresult = result[metbin][source]
            relativeError = getRelativeError(fitresult['value'], fitresult['error'])
            text = ' $%.2f \pm %.2f$  pb' % (fitresult['value']*scale, fitresult['error']*scale) + '(%.2f' % (relativeError * 100) + '\%)'
            if rows.has_key(source):
                rows[source].append(text)
            else:
                rows[source] = [source, text]
            
    header += '\\\\ \n'
    printout += header
    printout += '\hline\n'
    for item in rows['central']:
        printout += item + '&'
    printout = printout.rstrip('&')
    printout += '\\\\ \n'
    
    for source in sorted(rows.keys()):
        if source == 'central':
            continue
        for item in rows[source]:
            printout += item + '&'
        printout = printout.rstrip('&')
        printout += '\\\\ \n'
    printout += '\hline \n\n'
        
    if toFile:
        file = open(savePath + 'crosssection_result_' + bjetbin + '.tex', 'w')
        file.write(printout)
        file.close()
    else:
        print printout

def printCrossSectionResultsForTTJetWithUncertanties(result, toFile=True):
    global metbins
    printout = '\n'
    printout += '=' * 60
    printout = '\n'
    printout += 'Results for %s region\n' % bjetbin
    printout += '=' * 60
    printout += '\n'
    rows = {}
    printout += '\met bin & $\sigma{meas}$ \\\\ \n'
    printout += '\hline\n'
    uncertainties = {}
    for metbin in metbins:
        width = metbin_widths[metbin]
        scale = 1/width
        centralresult = result[metbin]['central']
        uncertainty = calculateTotalUncertainty(result[metbin])
        formatting = (metbin, centralresult['value']*scale, centralresult['error']*scale, 
                      uncertainty['Total+']['value']*scale, uncertainty['Total-']['value']*scale)
        text = '%s~\GeV & $%.2f \pm %.2f (fit)^{+%.2f}_{-%.2f} (sys)$ pb \\\\ \n' % formatting
        printout += text
        
        for source, value in uncertainty.iteritems():
            unc_result = value
            if not uncertainties.has_key(source):
                uncertainties[source] = '\n'
                uncertainties[source] += '=' * 60
                uncertainties[source] = '\n'
                uncertainties[source] += 'Results for %s region, source = %s\n' % (bjetbin, source)
                uncertainties[source] += '=' * 60
                uncertainties[source] += '\n'
            formatting = (metbin, 
                          unc_result['value']*scale,   
                          unc_result['error']*scale, 
                          ('%.2f' % (unc_result['value']/centralresult['value']*100))+ '\%')
            text = '%s~\GeV & $%.2f \pm %.2f (fit) (%s of central result)$ \\\\ \n' % formatting
            uncertainties[source] += text
        
    if toFile:
        file = open(savePath + 'crosssection_main_result_' + bjetbin + '.tex', 'w')
        file.write(printout)
        for source, value in uncertainties.iteritems():
            file.write(value)
        file.close()
    else:
        print printout
        
def calculatePDFErrors(results):
    centralResult = results['central']
    if centralResult.has_key('TTJet'):
        centralResult = results['central']['TTJet']
    centralvalue, centralerror = centralResult['value'], centralResult['error']
    negative = []
    positive = []
    
    for index in range(1, 45):
        weight = 'PDFWeights_%d' % index
        result = results[weight]
        if result.has_key('TTJet'):
            result = results[weight]['TTJet']
        value, error = result['value'], result['error']
        if index % 2 == 0: #even == negative
            negative.append(value)
        else:
            positive.append(value)
    pdf_max = numpy.sqrt(sum(max(x - centralvalue, y - centralvalue, 0) for x, y in zip(negative, positive)))
    pdf_min = numpy.sqrt(sum(max(centralvalue - x, centralvalue - y, 0) for x, y in zip(negative, positive)))
    return pdf_min, pdf_max   
    
    
def plotNormalisationResults(results):
    global metbins
    for metbin in metbins:
        for measurement, result in results[metbin].iteritems():
            if not measurement == 'central' and not measurement == 'QCD shape':
                continue
#        result = results[metbin]['central']
            fit = result['fit']
    #        fitvalues = result['fitvalues']
            templates = result['vectors']
            plots = {}
            plot_templates = {}
            
            samples = [
                       'ElectronHad',
                       'W+Jets',
                       'DYJetsToLL',
                       qcdLabel,
    #                   'Di-Boson',
                       'TTJet',
                       'SingleTop'
                       ]
            colors = {
                      'ElectronHad':0,
                       'W+Jets':kGreen - 3,
                       'DYJetsToLL':kAzure - 2,
                       qcdLabel:kYellow,
    #                   'Di-Boson':kWhite,
                       'TTJet':kRed + 1,
                       'SingleTop': kMagenta
                      }
            for sample in samples:
                template = templates[sample]
                plot = TH1F(sample + metbin + bjetbin + measurement, sample, len(template), -3, 3)
                plot_template = TH1F(sample + metbin + bjetbin + measurement + '_template', sample, len(template), -3, 3)
                bin = 1
                for value in template:
                    plot.SetBinContent(bin, value)
                    plot_template.SetBinContent(bin, value)
                    bin += 1
                plot.Scale(result[sample]['value'])
    #            plot.Rebin(rebin)
                plot.SetYTitle('Events/%.2f' % (0.02 * rebin))
                plot_template.SetYTitle('normalised to unit area/%.2f' % (0.02 * rebin))
                plot.SetXTitle('#eta(e)')
                plot_template.SetXTitle('#eta(e)')
                if not sample == 'ElectronHad':
                    plot.SetFillColor(colors[sample])
                    plot_template.SetLineColor(colors[sample])
                    plot_template.SetLineWidth(3)
                plots[sample] = plot
                plot_templates[sample] = plot_template
    #        fit.Rebin(10)
            fit.SetLineColor(kViolet - 3)
            fit.SetLineWidth(5)
            c = TCanvas("Fit_" + metbin + bjetbin + measurement, "Differential cross section", 1600, 1200)
            max = plots['ElectronHad'].GetMaximum()
            plots['ElectronHad'].SetMaximum(max * 1.5)
            plots['ElectronHad'].Draw('error')
            mcStack = THStack("MC", "MC")
    #        mcStack.Add(plots['Di-Boson']);
            mcStack.Add(plots[qcdLabel]);
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
            legend.AddEntry(plots[qcdLabel], 'QCD/#gamma + jets', 'F')
            legend.AddEntry(plots['SingleTop'], 'Single-Top', 'F')
    #        legend.AddEntry(plots['Di-Boson'], 'VV + X', 'F')
            legend.Draw()
            plotting.saveAs(c, 'EPlusJets_electron_eta_fit_' + measurement + '_' + metbin + '_' + bjetbin,
                            outputFormat_plots,
                            savePath + measurement)
            c = TCanvas("EPlusJets_electron_eta_templates_" + metbin + bjetbin + measurement, "Differential cross section", 1600, 1200)
            max = 1
            
            plot_templates[qcdLabel].Draw('hist')
            plot_templates['DYJetsToLL'].Draw('hist same')
            plot_templates['W+Jets'].Draw('hist same')
            plot_templates['SingleTop'].Draw('hist same')
            plot_templates['TTJet'].Draw('hist same')
            
            legend = plotting.create_legend()
            legend.AddEntry(plot_templates['TTJet'], 't#bar{t}', 'L')
            legend.AddEntry(plot_templates['W+Jets'], 'W#rightarrowl#nu', 'L')
            legend.AddEntry(plot_templates['DYJetsToLL'], 'Z/#gamma*#rightarrowl^{+}l^{-}', 'L')
            legend.AddEntry(plot_templates[qcdLabel], 'QCD/#gamma + jets', 'L')
            legend.AddEntry(plot_templates['SingleTop'], 'Single-Top', 'L')
    #        legend.AddEntry(plots['Di-Boson'], 'VV + X', 'F')
            legend.Draw()
            plotting.saveAs(c, 'EPlusJets_electron_eta_templates_' + measurement + '_' + metbin + '_' + bjetbin,
                            outputFormat_plots,
                            savePath + measurement)
#            c.SaveAs(savePath + measurement + 'EPlusJets_electron_eta_fit_' + measurement + '_' + metbin + '_' + bjetbin + '.png')
#            c.SaveAs(savePath + 'EPlusJets_electron_eta_fit_' + 'central' + '_' + metbin + '_' + bjetbin + '.pdf')
def plotCrossSectionResults(result, compareToSystematic = False):
    
    arglist = array('d', [0, 25, 45, 70, 100, 150])
    c = TCanvas("test", "Differential cross section", 1600, 1200)
    plot = TH1F("measurement_" + bjetbin, 'Differential cross section; MET [GeV];#frac{#partial#sigma}{#partial MET}', len(arglist) - 1, arglist)
    plotMADGRAPH = TH1F("measurement_MC_MADGRAPH" + bjetbin, 'Differential cross section; MET [GeV];#frac{#partial#sigma}{#partial MET}', len(arglist) - 1, arglist)
    plotPOWHEG = TH1F("measurement_MC_POWHEG" + bjetbin, 'Differential cross section; MET [GeV];#frac{#partial#sigma}{#partial MET}', len(arglist) - 1, arglist)
    plotPYTHIA6 = TH1F("measurement_MC_PYTHIA6" + bjetbin, 'Differential cross section; MET [GeV];#frac{#partial#sigma}{#partial MET}', len(arglist) - 1, arglist)
    plotnoCorr_mcatnlo = TH1F("measurement_MC_MCatNLO" + bjetbin, 'Differential cross section; MET [GeV];#frac{#partial#sigma}{#partial MET}', len(arglist) - 1, arglist)
    plot.SetMinimum(0)
    plot.SetMaximum(3)
    plotMADGRAPH.SetLineColor(kRed + 1)
    plotMADGRAPH.SetLineWidth(2)
    plotPOWHEG.SetLineColor(kBlue)
    plotPOWHEG.SetLineWidth(2)
    plotPYTHIA6.SetLineColor(kGreen + 4)
    plotPYTHIA6.SetLineWidth(2)
    plotnoCorr_mcatnlo.SetLineColor(kMagenta + 3)
    plotnoCorr_mcatnlo.SetLineWidth(2)
    
    legend = plotting.create_legend()
    legend.AddEntry(plot, 'measured', 'LEP')
    if compareToSystematic:
        legend.AddEntry(plotMADGRAPH, 't#bar{t} (Q^{2} down)', 'l')
        legend.AddEntry(plotPOWHEG, 't#bar{t} (Q^{2} up)', 'l')
        legend.AddEntry(plotPYTHIA6, 't#bar{t} (matching threshold 10 GeV)', 'l')
        legend.AddEntry(plotnoCorr_mcatnlo, 't#bar{t} (matching threshold 40 GeV)', 'l')
    else:
        legend.AddEntry(plotMADGRAPH, 't#bar{t} (MADGRAPH)', 'l')
        legend.AddEntry(plotPOWHEG, 't#bar{t} (POWHEG)', 'l')
        legend.AddEntry(plotPYTHIA6, 't#bar{t} (PYTHIA6)', 'l')
        legend.AddEntry(plotnoCorr_mcatnlo, 't#bar{t} (MC@NLO)', 'l')
    
    bin = 1
    for metbin in metbins:
        width = metbin_widths[metbin]
        scale = 1/width
        centralresult = result[metbin]['central']
        plot.SetBinContent(bin, centralresult['value']*scale)
        if compareToSystematic:
            plotMADGRAPH.SetBinContent(bin, result[metbin]['TTJet scale -']['MADGRAPH']*scale)
            plotPOWHEG.SetBinContent(bin, result[metbin]['TTJet scale +']['MADGRAPH']*scale)
            plotPYTHIA6.SetBinContent(bin, result[metbin]['TTJet matching -']['MADGRAPH']*scale)
            plotnoCorr_mcatnlo.SetBinContent(bin, result[metbin]['TTJet matching +']['MADGRAPH']*scale)
        else:
            plotMADGRAPH.SetBinContent(bin, centralresult['MADGRAPH']*scale)
            plotPOWHEG.SetBinContent(bin, centralresult['POWHEG']*scale)
            plotPYTHIA6.SetBinContent(bin, centralresult['PYTHIA6']*scale)
            plotnoCorr_mcatnlo.SetBinContent(bin, centralresult['MCatNLO']*scale)
        bin += 1
    plotAsym = TGraphAsymmErrors(plot)    
    bin = 0
    for metbin in metbins:
        width = metbin_widths[metbin]
        scale = 1/width
        centralresult = result[metbin]['central']
        uncertainty = calculateTotalUncertainty(result[metbin], compareToSystematic)
        error_up = sqrt(centralresult['error'] ** 2 + uncertainty['Total+']['value'] ** 2)*scale
        error_down = sqrt(centralresult['error'] ** 2 + uncertainty['Total-']['value'] ** 2)*scale
        plotAsym.SetPointEYhigh(bin, error_up)
        plotAsym.SetPointEYlow(bin, error_down)
        bin += 1
    plot.Draw('P')
    gStyle.SetErrorX(0.4)
    plotMADGRAPH.Draw('hist same')
    plotPOWHEG.Draw('hist same')
    plotPYTHIA6.Draw('hist same')
    plotnoCorr_mcatnlo.Draw('hist same')
    plotAsym.Draw('same P')
    legend.Draw()
    if compareToSystematic:
        plotting.saveAs(c, 'EPlusJets_diff_MET_xsection_compareSystematics_' + bjetbin, outputFormat_plots, savePath)
    else:
        plotting.saveAs(c, 'EPlusJets_diff_MET_xsection_' + bjetbin, outputFormat_plots, savePath)
        
def printNormalisedCrossSectionResult(result,toFile = True):
    global metbins
    printout = '\n'
    printout += '=' * 60
    printout = '\n'
    printout += 'Results for %s region\n' % bjetbin
    printout += '=' * 60
    printout += '\n'
    rows = {}
    header = 'Measurement'
    for metbin in metbins:
        header += '& $\sigma_{meas}$ \met bin %s~\GeV' % metbin
        width = metbin_widths[metbin]
        for source in result[metbin].keys():
            fitresult = result[metbin][source]
            scale = 100/width
            relativeError = getRelativeError(fitresult['value'], fitresult['error'])
            text = ' $(%.2f \pm %.2f) \cdot 10^{-2}$ ' % (fitresult['value']*scale,fitresult['error']*scale) + '(%.2f' % (relativeError * 100) + '\%)'
            text = ' $%.2f \pm %.2f $ ' % (fitresult['value']*scale,fitresult['error']*scale) + '(%.2f' % (relativeError * 100) + '\%)'
            if rows.has_key(source):
                rows[source].append(text)
            else:
                rows[source] = [source, text]
            
    header += '\\\\ \n'
    printout += header
    printout += '\hline\n'
    for item in rows['central']:
        printout += item + '&'
    printout = printout.rstrip('&')
    printout += '\\\\ \n'
    
    for source in sorted(rows.keys()):
        if source == 'central':
            continue
        for item in rows[source]:
            printout += item + '&'
        printout = printout.rstrip('&')
        printout += '\\\\ \n'
    printout += '\hline \n\n'
        
    if toFile:
        file = open(savePath + 'normalised_crosssection_result_' + bjetbin + '.tex', 'w')
        file.write(printout)
        file.close()
    else:
        print printout
        
def printNormalisedCrossSectionResultsForTTJetWithUncertanties(result, toFile = True):
    global metbins
    printout = '\n'
    printout += '=' * 60
    printout = '\n'
    printout += 'Results for %s region\n' % bjetbin
    printout += '=' * 60
    printout += '\n'
    rows = {}
    printout += '\met bin & $\sigma{meas}$ \\\\ \n'
    printout += '\hline\n'
    uncertainties = {}
    header = 'Uncertainty'
    for metbin in metbins:
        header += '&\met bin %s~\GeV' % metbin
        width = metbin_widths[metbin]
        centralresult = result[metbin]['central']
        uncertainty = calculateTotalUncertainty(result[metbin])
        scale = 100/width
        formatting = (metbin, centralresult['value']*scale, 
                      centralresult['error']*scale, uncertainty['Total+']['value']*scale, 
                      uncertainty['Total-']['value']*scale)
        text = '%s~\GeV & $%.2f \pm %.2f (fit)^{+%.2f}_{-%.2f} (sys) \cdot 10^{-2}$\\\\ \n' % formatting
#        text = '%s~\GeV & $%.2f \pm %.2f (fit)^{+%.2f}_{-%.2f} (sys)$\\\\ \n' % formatting
        printout += text
        for source in uncertainty.keys():
            unc_result = uncertainty[source]
            if not uncertainties.has_key(source):
                uncertainties[source] = source + ' & '
            relativeError = getRelativeError(centralresult['value'], unc_result['value'])
#            text = ' $(%.2f \pm %.2f) \cdot 10^{-2} $ ' % (unc_result['value']*scale,unc_result['error']*scale) + '(%.2f' % (relativeError * 100) + '\%) &'
            text = '%.2f' % (relativeError * 100) + '\% &'
#            text = ' $%.2f \pm %.2f $ ' % (unc_result['value']*scale,unc_result['error']*scale) + '(%.2f' % (relativeError * 100) + '\%) &'
            uncertainties[source] += text
        
    if toFile:
        file = open(savePath + 'normalised_crosssection_main_result_' + bjetbin + '.tex', 'w')
        file.write(printout)
        header += '\\\\ \n'
        file.write(header)
        for source in sorted(uncertainties.keys()):
            value = uncertainties[source]
            value = value.rstrip('&')
            value += '\\\\ \n'
            file.write(value)
        file.close()
    else:
        print printout
        
def plotNormalisedCrossSectionResults(result, compareToSystematic = False):
    
    arglist = array('d', [0, 25, 45, 70, 100, 150])
    c = TCanvas("test", "Differential cross section", 1600, 1200)
    plot = TH1F("measurement_" + bjetbin, 'Differential cross section; MET [GeV];#frac{1}{#sigma_{meas}} * #frac{#partial#sigma}{#partial MET}', len(arglist) - 1, arglist)
    plotMADGRAPH = TH1F("measurement_MC_MADGRAPH" + bjetbin, 'Differential cross section; MET [GeV];#frac{#partial#sigma}{#partial MET}', len(arglist) - 1, arglist)
    plotPOWHEG = TH1F("measurement_MC_POWHEG" + bjetbin, 'Differential cross section; MET [GeV];#frac{#partial#sigma}{#partial MET}', len(arglist) - 1, arglist)
    plotPYTHIA6 = TH1F("measurement_MC_PYTHIA6" + bjetbin, 'Differential cross section; MET [GeV];#frac{#partial#sigma}{#partial MET}', len(arglist) - 1, arglist)
    plotnoCorr_mcatnlo = TH1F("measurement_MC_MCatNLO" + bjetbin, 'Differential cross section; MET [GeV];#frac{#partial#sigma}{#partial MET}', len(arglist) - 1, arglist)
    plot.SetMinimum(0)
    plot.SetMaximum(0.03)
    plotMADGRAPH.SetLineColor(kRed + 1)
    plotMADGRAPH.SetLineWidth(2)
    plotPOWHEG.SetLineColor(kBlue)
    plotPOWHEG.SetLineWidth(2)
    plotPYTHIA6.SetLineColor(kGreen + 4)
    plotPYTHIA6.SetLineWidth(2)
    plotnoCorr_mcatnlo.SetLineColor(kMagenta + 3)
    plotnoCorr_mcatnlo.SetLineWidth(2)
    legend = plotting.create_legend()
    legend.AddEntry(plot, 'measured', 'LEP')
    if compareToSystematic:
        legend.AddEntry(plotMADGRAPH, 't#bar{t} (Q^{2} down)', 'l')
        legend.AddEntry(plotPOWHEG, 't#bar{t} (Q^{2} up)', 'l')
        legend.AddEntry(plotPYTHIA6, 't#bar{t} (matching threshold 10 GeV)', 'l')
        legend.AddEntry(plotnoCorr_mcatnlo, 't#bar{t} (matching threshold 40 GeV)', 'l')
    else:
        legend.AddEntry(plotMADGRAPH, 't#bar{t} (MADGRAPH)', 'l')
        legend.AddEntry(plotPOWHEG, 't#bar{t} (POWHEG)', 'l')
        legend.AddEntry(plotPYTHIA6, 't#bar{t} (PYTHIA6)', 'l')
        legend.AddEntry(plotnoCorr_mcatnlo, 't#bar{t} (MC@NLO)', 'l')
    bin = 1
    for metbin in metbins:
        width = metbin_widths[metbin]
        scale = 1/width
        centralresult = result[metbin]['central']
        plot.SetBinContent(bin, centralresult['value']*scale)
        if compareToSystematic:
            plotMADGRAPH.SetBinContent(bin, result[metbin]['TTJet scale -']['MADGRAPH']*scale)
            plotPOWHEG.SetBinContent(bin, result[metbin]['TTJet scale +']['MADGRAPH']*scale)
            plotPYTHIA6.SetBinContent(bin, result[metbin]['TTJet matching -']['MADGRAPH']*scale)
            plotnoCorr_mcatnlo.SetBinContent(bin, result[metbin]['TTJet matching +']['MADGRAPH']*scale)
        else:
            plotMADGRAPH.SetBinContent(bin, centralresult['MADGRAPH']*scale)
            plotPOWHEG.SetBinContent(bin, centralresult['POWHEG']*scale)
            plotPYTHIA6.SetBinContent(bin, centralresult['PYTHIA6']*scale)
            plotnoCorr_mcatnlo.SetBinContent(bin, centralresult['MCatNLO']*scale)
        bin += 1    
    plotAsym = TGraphAsymmErrors(plot)
    bin = 0
    for metbin in metbins:
        width = metbin_widths[metbin]
        scale = 1/width
        centralresult = result[metbin]['central']
        uncertainty = calculateTotalUncertainty(result[metbin],compareToSystematic)
        error_up = sqrt(centralresult['error'] ** 2 + uncertainty['Total+']['value'] ** 2)*scale
        error_down = sqrt(centralresult['error'] ** 2 + uncertainty['Total-']['value'] ** 2)*scale
        if DEBUG:
            print centralresult['error'], uncertainty['Total+']['value'], error_up
            print centralresult['error'], uncertainty['Total-']['value'], error_down
        plotAsym.SetPointEYhigh(bin, error_up)
        plotAsym.SetPointEYlow(bin, error_down)
        bin += 1 
    plot.Draw('P')
    gStyle.SetErrorX(0.4)
    plotMADGRAPH.Draw('hist same')
    plotPOWHEG.Draw('hist same')
    plotPYTHIA6.Draw('hist same')
    plotnoCorr_mcatnlo.Draw('hist same')
    plotAsym.Draw('same P')
    legend.Draw()
    if compareToSystematic:
        plotting.saveAs(c, 'EPlusJets_diff_MET_norm_xsection_compareSystematics_' + bjetbin, outputFormat_plots, savePath)
    else:
        plotting.saveAs(c, 'EPlusJets_diff_MET_norm_xsection_' + bjetbin, outputFormat_plots, savePath)
    
if __name__ == '__main__':
#    DEBUG = True
    completeAnalysisTimer = Timer()
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 1001;')
   
    plotting.setStyle()
    
    bjetbin = '0orMoreBtag'
#    bjetbin = '0btag'
#    bjetbin = '1btag'
#    bjetbin = '2orMoreBtags'
    
    normalisation_result = NormalisationAnalysis()
    printNormalisationResult(normalisation_result)
    printNormalisationResultsForTTJetWithUncertanties(normalisation_result)
    plotNormalisationResults(normalisation_result)
    
    crosssection_result = CrossSectionAnalysis(normalisation_result)
    printCrossSectionResult(crosssection_result)
    printCrossSectionResultsForTTJetWithUncertanties(crosssection_result)
    plotCrossSectionResults(crosssection_result)
    plotCrossSectionResults(crosssection_result, compareToSystematic=True)
#    
    normalised_crosssection_result = NormalisedCrossSectionAnalysis(normalisation_result)
    printNormalisedCrossSectionResult(normalised_crosssection_result)
    printNormalisedCrossSectionResultsForTTJetWithUncertanties(normalised_crosssection_result)
    plotNormalisedCrossSectionResults(normalised_crosssection_result)
    plotNormalisedCrossSectionResults(result=normalised_crosssection_result, compareToSystematic=True)
    print 'Complete Analysis in bjetbin=', bjetbin, 'finished in %.2fs' % completeAnalysisTimer.elapsedTime()
    
    
