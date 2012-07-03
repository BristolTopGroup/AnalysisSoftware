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

availableSamples = FILES.samplesToLoad
files = FILES.files
lumi = FILES.luminosity

normalisation = None
qcdError = 1 
vectors = None
vector_errors = None
DEBUG = False
unbinnedTTbarEvents = 0

#DO NOT USE theory x-section but rather just the total luminosity*selection efficiency
def getTtbarCrossSection(vectors_={}, normalisation_={}, theoryXsection=157.5):
    #at the moment store the values in the global variables. Not good but the only way it works with current setup
    global normalisation, vectors
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

    
    
    arglist = array('d', 10*[0.])
    
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
    
    return fit, value, error, fitvalues, fitErrors

# fcn returns back f = - 2*ln(L), the function to be minimised
def fitFunction(nParameters, gin, f, par, iflag):
    global normalisation, vectors, qcdError
    lnL = 0.0
    input = zip(vectors['ElectronHad'], vectors['Signal'],
                 vectors['W+Jets'], vectors['DYJetsToLL'], 
                vectors['QCDFromData'],vectors['Di-Boson'])
    
    for data, signal, wjets, zjets, qcd, diBoson in input:
        #signal = ttjet + singleTop
        x_i = par[0] * signal + par[1] * wjets + par[2] * zjets + par[3] * qcd + par[4]*diBoson#expected number of events in each bin
        data = data * normalisation['ElectronHad']
        if (data != 0) and (x_i != 0):
            L = TMath.Poisson(data, x_i)
            lnL += log(L)
    f[0] = -2.0 * lnL
    
    #constrains
    ratio_Z_W = normalisation['DYJetsToLL'] / normalisation['W+Jets']
#    ratio of Z/W to 5%
    f[0] += ((par[2] / par[1] - ratio_Z_W) / (0.05 * ratio_Z_W)) ** 2
    #constrain W/Z+jets normalisation to 30%
    f[0] += ((par[1] - normalisation['W+Jets']) / (0.3 * normalisation['W+Jets'])) ** 2
    f[0] += ((par[2] - normalisation['DYJetsToLL']) / (0.3 * normalisation['DYJetsToLL'])) ** 2
    f[0] += ((par[4] - normalisation['Di-Boson']) / (0.3 * normalisation['Di-Boson'])) ** 2
    #constrain QCD to the error 
    f[0] += ((par[3] - normalisation['QCD']) / qcdError) ** 2

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
    hists['Signal'] = plotting.sumSamples(hists, [ 'TTJet', 'SingleTop'])
    return hists

def rescaleSamples(hists):
    #renormalise W+N jet MC due to bug
#    totalWPlusJets = 47896878 + 71828418 + 25400440 + 7685939 + 10814233 # = 163625908
#    hists['WJetsToLNu'].Scale(totalWPlusJets/47896878)
#    hists['W1Jet'].Scale(totalWPlusJets / 76048786.*4480.0 / 31314.)
#    hists['W2Jets'].Scale(totalWPlusJets / 25400440.*1674. / 31314.)
#    hists['W3Jets'].Scale(totalWPlusJets / 7685939.*484.7 / 31314.)
#    hists['W4Jets'].Scale(totalWPlusJets / 12998049.*211.7 / 31314.)    
    return hists    

def performMeasurement(listOfDistributions, listOfQCDDistributions, listOfFiles, rebin):
    for distribution,qcdDistribution in zip(listOfDistributions, listOfQCDDistributions):
        print 'Performing measurement for:'
        print distribution
        print 'QCD shape from data:', qcdDistribution
        hists = FileReader.getHistogramDictionary(distribution, listOfFiles)
        qcdHists = FileReader.getHistogramDictionary(qcdDistribution, listOfFiles)
        
        hists = plotting.rebin(hists, 5)#rebin to 200 bins
        hists = plotting.setYTitle(hists, title="events/5 GeV")
        qcdHists = plotting.rebin(qcdHists, 5)#rebin to 200 bins
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
        qcdNormalisation, qcdError = QCDRateEstimation.estimateQCDWithRelIso(FILES.files['ElectronHad'], qcdHistForEstimation)
        
        sumMC = normalisation['SumMC'] - normalisation['QCD'] + qcdNormalisation
        if DEBUG:
            print '*' * 120
            print "Input parameters:"
            print 'signal (ttbar+single top):', normalisation['TTJet'] + normalisation['SingleTop']
            print 'W+Jets:', normalisation['WJetsToLNu']
            print 'Z+Jets:', normalisation['DYJetsToLL'] 
            print 'QCD (from data):', qcdNormalisation, '+-',   qcdError, '(', qcdError/qcdNormalisation*100, '%)'
            print 'SingleTop:', normalisation['SingleTop'] 
            print 'TTJet:', normalisation['TTJet']  
            print 'Di-Boson:', normalisation['Di-Boson']
            print 'SumMC:', sumMC
            print 'Total data', normalisation['ElectronHad']
            print '(data-SumMC)/data', (normalisation['ElectronHad'] - sumMC)/normalisation['ElectronHad']
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
            print '(data-SumMC)/data', (normalisation['ElectronHad'] - sumMC)/normalisation['ElectronHad']
            print '*' * 120
        return value, error
        
        
            

if __name__ == "__main__":
    DEBUG = False
    btagBins = [
#                '0orMoreBtag',
#                '1btag',
                '2orMoreBtags'
                ]
    metBins = ['0-30',
               '30-50',
               '50-75',
               '75-inf'
               ]
    
    distribution = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/MET/patType1CorrectedPFMet/MET_2orMoreBtags'
    unbinnedTtbarHist = FileReader.getHistogramFromFile(distribution, FILES.files['TTJet']) 
    unbinnedTTbarEvents = unbinnedTtbarHist.Integral()
    results = {}
    
    for metbin in metBins:
        metbin = str(metbin)
        distribution = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/Electron_METbin_%s/electron_eta' %metbin
       #should always use 0-btag region!!                  
        qcdDistribution = 'TTbarEplusJetsPlusMetAnalysis/Ref selection/QCDConversions/Electron_METbin_%s/electron_eta_0btag'  %metbin
        histograms = [distribution + '_' + btagBin for btagBin in btagBins]
        qcdHistograms = [qcdDistribution]*len(btagBins)
#        print '=' * 60
#        print 'Starting Top-pair cross section measurement'
#        print '=' * 60
        value, error = performMeasurement(histograms,qcdHistograms, FILES.files, 10)
#        print "\n TTbar cross-section = %.2f \pm %.2f (fit) pb \n" %( value, error)
#        
#        print '=' * 60
#        print 'Starting Top-pair cross section measurement - Systematic Error, QCD Shape'
#        print '=' * 60
        qcdDistribution ='TTbarEplusJetsPlusMetAnalysis/Ref selection/QCD non iso e+jets/Electron_METbin_%s/electron_eta_0btag' %metbin
        qcdHistograms = [qcdDistribution]*len(btagBins)
        value2, error2 = performMeasurement(histograms,qcdHistograms, FILES.files, 10)
#        print "\n TTbar cross-section = %.2f \pm %.2f (fit) pb \n" %( value2, error2)
        
#        print '=' * 60
#        print 'Starting Top-pair cross section measurement - Final result'
#        print '=' * 60
        systematic = abs(value2 - value)
#        print "\n TTbar cross-section = %.2f \pm %.2f (stat) \pm %.2f (syst) pb \n" %( value, error, systematic)
        
        results[metbin] = ( value, error, systematic)
    print '=' * 60
    print 'Result for \geq 2 b-tag region'
    print '=' * 60
    print '\met bin & cross-section \\\\'
    print '\hline'
    total = 0
    totalStatError = 0
    totalSystError = 0
    for metbin, result in results.iteritems():
        total += result[0]
        totalStatError += result[1]**2
        totalSystError += result[2]**2 
        print metbin + '~\GeV & %.2f $\pm$ %.2f (stat) $\pm$ %.2f (syst) pb \\\\' % result
    totalStatError = sqrt(totalStatError)
    totalSystError = sqrt(totalSystError)
    print '\hline'
    print 'Total & %.2f $\pm$ %.2f (stat) $\pm$ %.2f (syst) pb\\\\' % (total, totalStatError, totalSystError)
        
    
    
    
