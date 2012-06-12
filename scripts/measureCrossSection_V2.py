#Step 1: get histograms of the distribution you are interested in
#Step 2: Pass the distributions with starting values to TMinuit
#Step 3: ???
#Step 4: Profit
from __future__ import division
from ROOT import *
import FILES
import tools.ROOTFileReader as FileReader
from array import array
from tools import Styles
import tools.PlottingUtilities as plotting
import QCDRateEstimation

availableSamples = FILES.samplesToLoad
files = FILES.files
lumi = FILES.luminosity

normalisation = None
vectors = None

def getTtbarCrossSection(vectors_={}, normalisation_={}, theoryXsection=157.5):
    fit = None
    value, error = 0, 0
    
    #at the moment store the values in the global variables. Not good but the only way it works with current setup
    global normalisation, vectors
    normalisation = normalisation_
    vectors = vectors_
        
    #setup minuit
    numberOfParameters = 4
    gMinuit = TMinuit(numberOfParameters)
    gMinuit.SetFCN(fitFunction)
    
    gMinuit.SetPrintLevel(-1)
    #Error definition: 1 for chi-squared, 0.5 for negative log likelihood
    gMinuit.SetErrorDef(1)
    
    ierflg = Long(0)
    
    par_value = array ("d", (normalisation['TTJet'] + normalisation['SingleTop'] , normalisation['W+Jets'], normalisation['DYJetsToLL'], normalisation['QCD'])) #using the MC estimation as starting values
    N_total = normalisation['ElectronHad']
    print "Total number of data events before the fit: ", N_total
    
    gMinuit.mnparm(0, "ttbar+single_top", par_value[0], 10.0, 0, N_total, ierflg)
    gMinuit.mnparm(1, "wjets", par_value[1], 10.0, 0, N_total, ierflg)
    gMinuit.mnparm(2, "zjets", par_value[2], 10.0, 0, N_total, ierflg)
    gMinuit.mnparm(3, "qcd", par_value[3], 10.0, 0, N_total, ierflg)
      
    arglist = array('d', 10 * [0.])
    
    #minimisation strategy: 1 standard, 2 try to improve minimum (a bit slower)
    arglist[0] = 2
    
    #minimisation itself
    gMinuit.mnexcm("SET STR", arglist, 1, ierflg)
    gMinuit.Migrad()
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
    ttbarFit = signalFit -  normalisation['SingleTop']
    normfactor = normalisation['TTJet'] / theoryXsection
    
    value = ttbarFit/normfactor
    value_plusError = value + signalFitError/normfactor
    value_minusError = value - signalFitError/normfactor
    error = (abs(value_plusError - value) + abs(value - value_minusError)) / 2
    
    return fit, value, error, fitvalues, fitErrors

# fcn returns back f = - 2*ln(L), the function to be minimised
def fitFunction(nParameters, gin, f, par, iflag):
    global normalisation, vectors
    lnL = 0.0
    for data, ttjet, singleTop, wjets, zjets, qcd in zip(vectors['ElectronHad'], vectors['TTJet'], vectors['SingleTop'], vectors['W+Jets'], vectors['DYJetsToLL'], vectors['QCD']):
        x_i = par[0] * (ttjet + singleTop) + par[1] * wjets + par[2] * zjets + par[3] * qcd #expected number of events in each bin
        
        if (data != 0) and (x_i != 0):
            L = TMath.Poisson(data, x_i)
            lnL += log(L)
    f = -2.0 * lnL
    
    Nqcd_err = normalisation['QCD'] * 1.0
    
    ratio_Z_W = normalisation['DYJetsToLL'] / normalisation['W+Jets']
    f += ((par[2] / par[1] - ratio_Z_W) / (0.05 * ratio_Z_W)) ** 2

def prepareHistograms(histograms):
    return histograms

def getNormalisation(histograms):
    normalisation = {}
    for sample in histograms.keys():
        normalisation[sample] = histograms[sample].Integral()
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
        for bin in range(1, hist.GetNbinsX()):
            if not values.has_key(sample):
                values[sample] = []
            if not errors.has_key(sample):
                errors[sample] = []
            values[sample].append(hist.GetBinContent(bin))
            errors[sample].append(hist.GetBinError(bin))
    return values, errors
    

def sumSamples(hists):
    hists['QCD'] = plotting.sumSamples(hists, [ 'QCD_Pt-20to30_BCtoE',
                 'QCD_Pt-30to80_BCtoE',
                 'QCD_Pt-80to170_BCtoE',
                 'QCD_Pt-20to30_EMEnriched',
                 'QCD_Pt-30to80_EMEnriched',
                 'QCD_Pt-80to170_EMEnriched',
                 'GJets_HT-40To100',
                 'GJets_HT-100To200',
                 'GJets_HT-200'])
    hists['SingleTop'] = plotting.sumSamples(hists, [ 'T_tW-channel',
                 'T_t-channel',
                 'T_s-channel',
                 'Tbar_tW-channel',
                 'Tbar_t-channel',
                 'Tbar_s-channel'])
    hists['Di-Boson'] = plotting.sumSamples(hists, [ 'WWtoAnything', 'WZtoAnything', 'ZZtoAnything'])
    hists['W+Jets'] = plotting.sumSamples(hists, [ 'W1Jet', 'W2Jets', 'W3Jets', 'W4Jets'])
    hists['SumMC'] = plotting.sumSamples(hists, [ 'TTJet', 'DYJetsToLL', 'QCD', 'Di-Boson', 'W+Jets', 'SingleTop'])
    return hists

def rescaleSamples(hists):
    totalWPlusJets = 47896878 + 71828418 + 25400440 + 7685939 + 10814233 # = 163625908
    hists['WJetsToLNu'].Scale(0)
    hists['W1Jet'].Scale(totalWPlusJets / 71828418.*4480.0 / 31314.)
    hists['W2Jets'].Scale(totalWPlusJets / 25400440.*1674. / 31314.)
    hists['W3Jets'].Scale(totalWPlusJets / 7685939.*484.7 / 31314.)
    hists['W4Jets'].Scale(totalWPlusJets / 10814233.*211.7 / 31314.)    
    return hists    

def performMeasurement(listOfDistributions, listOfFiles, rebin):
    for distribution in listOfDistributions:
        print 'Performing measurement for:'
        print distribution
        hists = FileReader.getHistogramDictionary(distribution, listOfFiles)
        hists = plotting.rebin(hists, 5)
        hists = plotting.setYTitle(hists, title="events/5 GeV")
        #rescale hists if needed
        hists = rescaleSamples(hists)
        #add samples together: QCD, SumMC, singleTOP, W+Jets
        hists = sumSamples(hists)
        #TODO: this needs to be replaced and done properly
        
        normalisation = getNormalisation(hists)
        templates = getTemplates(hists)
        vectors, vector_errors = vectorise(templates)
        print '*'*120
        print "Input parameters:"
        print 'signal (ttbar+single top):', normalisation['TTJet'] + normalisation['SingleTop']
        print 'W+Jets:', normalisation['W+Jets']
        print 'Z+Jets:', normalisation['DYJetsToLL'] 
        print 'QCD:', normalisation['QCD'] 
        print 'SingleTop:', normalisation['SingleTop'] 
        print 'TTJet:', normalisation['TTJet']  
        print '*'*120
#        for sample, value in normalisation.iteritems():
#            print sample, '=', value
#        for sample, vector in vectors.iteritems():
#            print sample, '=', vector
        
        fit, value, error,fitvalues, fitErrors = getTtbarCrossSection(vectors, normalisation)
        print '*'*120
        print "Fit values:"
        print 'signal (ttbar+single top):', fitvalues[0], '+-', fitErrors[0]
        print 'W+Jets:', fitvalues[1], '+-', fitErrors[1]
        print 'Z+Jets:', fitvalues[2], '+-', fitErrors[2]
        print 'QCD:', fitvalues[3], '+-', fitErrors[3]
        print 'SingleTop (no fit):', normalisation['SingleTop'] 
        print 'TTJet (signal fit - SingleTop):', fitvalues[0] - normalisation['SingleTop']   
        print '*'*120
        print "\n TTbar cross-section = ", value, " +/- ", error, "(fit) pb \n"
        
            

if __name__ == "__main__":
    btagBins = [
                '0btag',
#                '1btag',
#                '2orMoreBtags'
                ]
    mets = ['patMETsPFlow',
            
            ]
    histograms = [
                  'TTbarEplusJetsPlusMetAnalysis/Ref + AsymJets selection/MET/patMETsPFlow/MET',
                  ]
    histograms = [hist + '_' + btagBin for hist in histograms for btagBin in btagBins]
#    hists = FileReader.getHistogramsFromFiles(histograms, FILES.files)
    #initial binning = 1GeV for MET
    #rebin histograms
    
    print '=' * 60
    print 'Starting Top-pair cross section measurement'
    print '=' * 60
    performMeasurement(histograms, FILES.files, 5)
    
    
    
