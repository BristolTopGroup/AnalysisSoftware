#Step 1: get histograms of the distribution you are interested in
#Step 2: Pass the distributions with starting values to RooFit
#Step 3: ???
#Step 4: Profit

from ROOT import *
import FILES
import tools.ROOTFileReader as FileReader
from array import array
#from tdrStyle import *

availableSamples = FILES.samplesToLoad
files= FILES.files
lumi= FILES.luminosity

theorXsect = 157.5
QCD_scale_factor = 1.0
xmin = 0
n_bins = 200
n_par = 4 #number of parameters in Minuit
N_total = 0 #Total number of events in data

data_vector = [] #input data
signal_template = [] #ttbar+single top template from mc
wjets_template = [] #wjets template from mc
zjets_template = [] #zjets template from mc
qcd_template = [] #qcd template from data-driven method

signal_err = []
wjets_err = []
zjets_err = []
qcd_err = []

histname ='TTbarEplusJetsPlusMetAnalysis/Ref + AsymJets selection/MET/patType1CorrectedPFMet/MET_2orMoreBtags'
histnameQCD = 'TTbarEplusJetsPlusMetAnalysis/Ref + AsymJets selection/QCDConversions/MET/patType1CorrectedPFMet/MET_2orMoreBtags'
hists = FileReader.getHistogramDictionary(histname, files)
histsQCD = FileReader.getHistogramDictionary(histnameQCD, files)

histData = hists['ElectronHad']
histSingleTop = hists['T_s-channel'] + hists['T_t-channel'] + hists['T_tW-channel'] + hists['Tbar_s-channel'] + hists['Tbar_t-channel'] + hists['Tbar_tW-channel']
histSignal = histSingleTop + hists['TTJet']
histWjets = hists['WJetsToLNu']
histZjets = hists['DYJetsToLL']
histQCD = histsQCD['ElectronHad']
histTTbar = hists['TTJet']

Ndata = histData.Integral()
Nsignal = histSignal.Integral()
Nwjets = histWjets.Integral()
Nzjets = histZjets.Integral()
Nqcd = QCD_scale_factor * histQCD.Integral()
Nsingle_t = histSingleTop.Integral()
Nttbar = histTTbar.Integral() 

#for sample, hist in hists.iteritems():
#    print sample, ' = ', hist 

def getDataFromHistograms():
    global N_total
    xmax = xmin + n_bins*(histData.GetBinWidth(1))
    print "Number of bins: ", n_bins, " xmax: ", xmax
    for ibin in range(0, n_bins):
        nn = histData.GetBinContent(ibin+1)
        data_vector.append(nn)
        N_total += nn
    #print "Total number of data events before the fit: ", N_total

#def getSignalFromHistograms():
#    histSignal.Scale(1./histSignal.Integral())
#    for ibin in range(0, n_bins):
#        signal_template.append(histSignal.GetBinContent(ibin+1))
#        signal_err.append(histSignal.GetBinError(ibin+1))

def getTemplatesFromHistograms(hist, template, err):
    #print "Number of bins before scaling and rebinning: ", hist, " = ", hist.GetSize()
    hist.Rebin(5)
    hist.Scale(1./hist.Integral())
    #print "Number of bins after scaling and rebinning: ", hist, " = ", hist.GetSize()
    for ibin in range(0, n_bins):
        template.append(hist.GetBinContent(ibin+1))
        err.append(hist.GetBinError(ibin+1))

# fcn passes back f = - 2*ln(L), the function to be minimised. not sure if N_{single top} is obtained from fitting
def fcn( npar, gin, f, par, iflag ):
    lnL = 0.0
    for i in range (0, n_bins):
        data_i = data_vector[i] #observed number of events in each bin
        x_i = par[0]*signal_template[i] + par[1]*wjets_template[i] + par[2]*zjets_template[i] + par[3]*qcd_template[i] #expected number of events in each bin
        #print "{", i, ",", data_i, ",",  x_i, "}, parameters: {", par[0], "*", signal_template[i], "+", par[1], "*", wjets_template[i], "+", par[2], "*", zjets_template[i], "+", par[3], "*", qcd_template[i], "}"
        
        if (data_i != 0) and (x_i != 0):
            lnL += log(TMath.Poisson(data_i, x_i))
            #print i, " { data_i = ", data_i, ", x_i = ", x_i, ", Poisson =  ", TMath.Poisson(data_i, x_i), ", log = ", log(TMath.Poisson(data_i, x_i)), ", lnL = ", lnL, "}"
    
    f = -2.0 * lnL
    
    Nqcd_err = Nqcd * 1.0
    
    f += ( (par[2]/par[1] - Nzjets/Nwjets) / (0.05 * Nzjets/Nwjets) ) * ( (par[2]/par[1] - Nzjets/Nwjets) / (0.05 * Nzjets/Nwjets) )
    f += ( par[3] - Nqcd) * ( par[3] - Nqcd) / ( Nqcd_err * Nqcd_err )
    
def tempFit():

    print "Input parameters: signal: ", Nsignal, " wjets: ", Nwjets, " zjets: ", Nzjets, " QCD: ", Nqcd, " single_t: ", Nsingle_t,  " ttbar: ", Nttbar
    getDataFromHistograms()
    getTemplatesFromHistograms(histSignal, signal_template, signal_err)
    getTemplatesFromHistograms(histWjets, wjets_template, wjets_err)
    getTemplatesFromHistograms(histZjets, zjets_template, zjets_err)
    getTemplatesFromHistograms(histQCD, qcd_template, qcd_err)
    
    gMinuit = TMinuit(n_par)
    gMinuit.SetFCN( fcn )
    
    gMinuit.SetPrintLevel(1)
    gMinuit.SetErrorDef(1.0)
    
    ierflg = Long(0)
    
    #parameters_list = ["ttbar+single_top", "wjets", "zjets", "qcd"] #background parameters
    #par = [Nsignal, Nwjets, Nzjets, Nqcd] #using the MC estimation as starting values
    
    par_value = array ("d", (Nsignal, Nwjets, Nzjets, Nqcd) ) #using the MC estimation as starting values
    
    print "Total number of data events before the fit: ", N_total
    
    gMinuit.mnparm(0, "ttbar+single_top", par_value[0], 1.0, 0, N_total, ierflg)
    gMinuit.mnparm(1, "wjets", par_value[1], 1.0, 0, N_total, ierflg)
    gMinuit.mnparm(2, "zjets", par_value[2], 1.0, 0, N_total, ierflg)
    gMinuit.mnparm(3, "qcd", par_value[3], 1.0, 0, N_total, ierflg)
    
#    for i in range[0, n_par]:
#        gMinuit.mnparm(i, par_name[i], par_value[i], 10.0, 0, N_total, ierflg)
    
    #Fabian's code to improve minimum: optional
    arglist = array( 'd', 10*[0.] )
    arglist[0] = 2
    gMinuit.mnexcm("SET STR", arglist, 1, ierflg)
    gMinuit.Migrad()
    
    outpar = array ("d", n_par*[0.0] )
    err = array ("d", n_par*[0.0])
    
    for i in range(n_par):
        temp_par = Double(0)
        temp_err = Double(0)
        gMinuit.GetParameter(i, temp_par, temp_err)
        outpar[i] = temp_par
        err[i] = temp_err
        print "{", temp_par, ", ", temp_err, "}"
    
    xs_fit = (outpar[0] - Nsingle_t) / (Nttbar/theorXsect)
    
    print "Cross-section is: ", xs_fit
    
tempFit()
