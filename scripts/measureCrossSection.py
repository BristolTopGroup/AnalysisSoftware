#Step 1: get histograms of the distribution you are interested in
#Step 2: Pass the distributions with starting values to TMinuit
#Step 3: ???
#Step 4: Profit

from ROOT import *
import FILES
import tools.ROOTFileReader as FileReader
from array import array
from tools import Styles

availableSamples = FILES.samplesToLoad
files= FILES.files
lumi= FILES.luminosity

theorXsect = 157.5
QCD_scale_factor = 1.0
xmin = 0
xmax = 0
n_bins = 200
n_par = 4 #number of parameters in Minuit
N_total = 0 #Total number of events in data

data_vector = [] #input data vector
signal_template = [] #ttbar+single top template from mc
wjets_template = [] #wjets template from mc
zjets_template = [] #zjets template from mc
qcd_template = [] #qcd template from data-driven method

#corresponding errors
signal_err = []
wjets_err = []
zjets_err = []
qcd_err = []

outpar = array ("d", n_par*[0.0] ) #output fit parameters
err = array ("d", n_par*[0.0]) #errors for them

histname ='TTbarEplusJetsPlusMetAnalysis/Ref + AsymJets selection/MET/patMETsPFlow/MET_2orMoreBtags'
histnameQCD = 'TTbarEplusJetsPlusMetAnalysis/Ref + AsymJets selection/QCDConversions/MET/patMETsPFlow/MET_2orMoreBtags'

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
    global N_total, xmax
    histData.Rebin(histData.GetSize()/n_bins)
    xmax = xmin + n_bins*(histData.GetBinWidth(1))
    for ibin in range(0, n_bins):
        nn = histData.GetBinContent(ibin+1)
        data_vector.append(nn)
        N_total += nn

def getTemplatesFromHistograms(hist, template, err):
    #print "Number of bins before scaling and rebinning: ", hist, " = ", hist.GetSize()
    hist.Rebin(hist.GetSize()/n_bins)
    hist.Scale(1./hist.Integral())
    #print "Number of bins after scaling and rebinning: ", hist, " = ", hist.GetSize()
    for ibin in range(0, n_bins):
        template.append(hist.GetBinContent(ibin+1))
        err.append(hist.GetBinError(ibin+1))

# fcn returns back f = - 2*ln(L), the function to be minimised
def fcn( npar, gin, f, par, iflag ):
    lnL = 0.0
    for i in range (0, n_bins):
        data_i = data_vector[i] #observed number of events in each bin
        x_i = par[0]*signal_template[i] + par[1]*wjets_template[i] + par[2]*zjets_template[i] + par[3]*qcd_template[i] #expected number of events in each bin
        
        if (data_i != 0) and (x_i != 0):
            lnL += log(TMath.Poisson(data_i, x_i))
    
    f[0] = -2.0 * lnL
    
    Nqcd_err = Nqcd * 1.0
    
    f[0] += ( (par[2]/par[1] - Nzjets/Nwjets) / (0.05 * Nzjets/Nwjets) ) * ( (par[2]/par[1] - Nzjets/Nwjets) / (0.05 * Nzjets/Nwjets) )
    #f[0] += ( par[3] - Nqcd) * ( par[3] - Nqcd) / ( Nqcd_err * Nqcd_err ) #QCD constraint
    
def tempFit():
    global data_vector, signal_template, wjets_template, zjets_template, qcd_template, signal_err, wjets_err, zjets_err, qcd_err

    print "Input parameters: signal (ttbar+single top): ", Nsignal, " wjets: ", Nwjets, " zjets: ", Nzjets, " QCD: ", Nqcd, " single_t: ", Nsingle_t,  " ttbar: ", Nttbar
    getDataFromHistograms()
    getTemplatesFromHistograms(histSignal, signal_template, signal_err)
    getTemplatesFromHistograms(histWjets, wjets_template, wjets_err)
    getTemplatesFromHistograms(histZjets, zjets_template, zjets_err)
    getTemplatesFromHistograms(histQCD, qcd_template, qcd_err)
    
    #setting up Minuit
    gMinuit = TMinuit(n_par)
    gMinuit.SetFCN( fcn )
    
    gMinuit.SetPrintLevel(1)
    
    #Error definition: 1 for chi-squared, 0.5 for negative log likelihood
    gMinuit.SetErrorDef(1)
    
    ierflg = Long(0)
    
    par_value = array ("d", (Nsignal, Nwjets, Nzjets, Nqcd) ) #using the MC estimation as starting values
    
    print "Total number of data events before the fit: ", N_total
    
    gMinuit.mnparm(0, "ttbar+single_top", par_value[0], 10.0, 0, N_total, ierflg)
    gMinuit.mnparm(1, "wjets", par_value[1], 10.0, 0, N_total, ierflg)
    gMinuit.mnparm(2, "zjets", par_value[2], 10.0, 0, N_total, ierflg)
    gMinuit.mnparm(3, "qcd", par_value[3], 10.0, 0, N_total, ierflg)
      
    arglist = array( 'd', 10*[0.] )
    
    #minimisation strategy: 1 standard, 2 try to improve minimum (a bit slower)
    arglist[0] = 2
    
    #minimisation itself
    gMinuit.mnexcm("SET STR", arglist, 1, ierflg)
    gMinuit.Migrad()
    
    
    #getting the output fit parameters back
    global outpar, err
   
    for i in range(n_par):
        temp_par = Double(0)
        temp_err = Double(0)
        gMinuit.GetParameter(i, temp_par, temp_err)
        outpar[i] = temp_par
        err[i] = temp_err
    
    #calculate cross-section and error
    xs_fit = (outpar[0] - Nsingle_t) / (Nttbar/theorXsect)
    xs_fit_plus = (outpar[0]+err[0] - Nsingle_t) / (Nttbar/theorXsect)
    xs_fit_minus = (outpar[0]-err[0] - Nsingle_t) / (Nttbar/theorXsect)
    xs_fit_error = (abs(xs_fit_plus-xs_fit) + abs(xs_fit-xs_fit_minus))/2
    
    print "\n TTbar cross-section = ", xs_fit, " +/- ", xs_fit_error, "(fit) pb \n"
    

def plotResult():
    Styles.tdrStyle().getStyle()
    
    result = TH1F("result", "", n_bins, xmin, xmax)
    data = TH1F("data", "", n_bins, xmin, xmax)
    
    #contribution histograms for the stack plot
    signal_contribution = TH1F("signal_con", "", n_bins, xmin, xmax)
    wjets_contribution = TH1F("wjets_con", "", n_bins, xmin, xmax)
    zjets_contribution = TH1F("zjets_con", "", n_bins, xmin, xmax)
    qcd_contribution = TH1F("qcd_con", "", n_bins, xmin, xmax)
    
    #templates histograms (scaling is irrelevant, shape matters)
    signal_temp = TH1F("signal_temp", "", n_bins, xmin, xmax)
    wjets_temp = TH1F("wjets_temp", "", n_bins, xmin, xmax)
    zjets_temp = TH1F("zjets_temp", "", n_bins, xmin, xmax)
    qcd_temp = TH1F("qcd_temp", "", n_bins, xmin, xmax)
    
    h_stack  = THStack("h_stack", "stacked histograms")
    
    for i in range(len(data_vector)):
        data.SetBinContent(i+1, data_vector[i])
        
        signal_con = outpar[0]*signal_template[i]
        wjets_con = outpar[1]*wjets_template[i]
        zjets_con = outpar[2]*zjets_template[i]
        qcd_con = outpar[3]*qcd_template[i]
        
        mean = signal_con + wjets_con + zjets_con + qcd_con
        
        result.SetBinContent(i+1, mean)
        
        signal_contribution.SetBinContent(i+1, signal_con)
        wjets_contribution.SetBinContent(i+1,wjets_con)
        zjets_contribution.SetBinContent(i+1,zjets_con)
        qcd_contribution.SetBinContent(i+1,qcd_con)
        
        signal_temp.SetBinContent(i+1, signal_template[i])
        wjets_temp.SetBinContent(i+1,wjets_template[i])
        zjets_temp.SetBinContent(i+1,zjets_template[i])
        qcd_temp.SetBinContent(i+1,qcd_template[i])
    
    print "Total number of events after the fit:"
    print "TTbar+single top & w+jets & zjets & qcd"
    print outpar[0], "+-", err[0], " & ",  outpar[1], "+-", err[1], " & ",  outpar[2], "+-", err[2], " & ",  outpar[3], "+-", err[3] 
        
    canvasTemp = TCanvas("canvasTemp", "canvasTemp", 700, 500)
    canvasTemp.cd()
    signal_temp.SetLineColor(kRed+1)
    wjets_temp.SetLineColor(kGreen-3)
    zjets_temp.SetLineColor(kAzure-2)
    qcd_temp.SetLineColor(kYellow)
    
    signal_temp.SetLineWidth(5)
    wjets_temp.SetLineWidth(5)
    zjets_temp.SetLineWidth(5)
    qcd_temp.SetLineWidth(5)
    
    signal_temp.SetAxisRange(0.,300.)
    signal_temp.SetTitle("")
    signal_temp.Draw()
    signal_temp.SetMaximum(0.25)
    signal_temp.GetXaxis().SetTitle("MET")
    signal_temp.GetYaxis().SetTitle("Number of Events")
    
    wjets_temp.Draw("same")
    zjets_temp.Draw("same")
    qcd_temp.Draw("same")
    
    legend_temp = TLegend(0.66,0.6,0.96,0.92)
    legend_temp.SetBorderSize(0)
    legend_temp.SetTextFont(42)
    legend_temp.SetFillColor(0)
    
    legend_temp.AddEntry(signal_temp, " t#bar{t}+single top", "L") 
    legend_temp.AddEntry(wjets_temp,  " W#rightarrowl#nu", "L")
    legend_temp.AddEntry(zjets_temp, " Z/#gamma*#rightarrowl^{+}l^{-}", "L")
    legend_temp.AddEntry(qcd_temp, " QCD", "L")
    legend_temp.SetTextSize(0.045)
    legend_temp.Draw("same")
    
    canvasTemp.SaveAs("templates.pdf")
    
    canvasFit = TCanvas("canvasFit", "canvasFit", 700, 500)
    canvasFit.cd()
    
    signal_contribution.SetFillColor(kRed+1)
    wjets_contribution.SetFillColor(kGreen-3)
    zjets_contribution.SetFillColor(kAzure-2)
    qcd_contribution.SetFillColor(kYellow)
    
    h_stack.Add(qcd_contribution)
    h_stack.Add(zjets_contribution)
    h_stack.Add(wjets_contribution)
    h_stack.Add(signal_contribution)
    
    result.SetLineStyle(1)
    result.SetLineColor(kViolet-3)
    result.SetLineWidth(3)
    
    data.Sumw2();
    data.SetMarkerStyle(20);
    data.SetMarkerSize(0.8);
    data.SetLineWidth(1);

    h_stack.SetTitle("")
    h_max = data.GetMaximum()
    h_stack.Draw()
    h_stack.SetMaximum(1.5*h_max)
    h_stack.GetXaxis().SetTitle("MET")
    h_stack.GetYaxis().SetTitle("Number of Events")
    h_stack.GetXaxis().SetRange(0,66)
    
    h_stack.Draw()
    result.Draw("same")
    data.Draw("same")
    
    legend_fit = TLegend(0.66, 0.6, 0.96, 0.92)
    legend_fit.SetBorderSize(0)
    legend_fit.SetTextFont(42)
    legend_fit.SetFillColor(0)
    
    legend_fit.AddEntry(data     , " Data 2011", "LPE")
    legend_fit.AddEntry(result   , " Fit", "L")
    legend_fit.AddEntry(signal_contribution, " t#bar{t}+single top", "F")
    legend_fit.AddEntry(wjets_contribution, " W#rightarrowl#nu", "F")
    legend_fit.AddEntry(zjets_contribution, " Z/#gamma*#rightarrowl^{+}l^{-}", "F")
    legend_fit.AddEntry(qcd_contribution , " QCD", "F")
    legend_fit.SetTextSize(0.04)
    legend_fit.Draw("same")
    
    cms_label = add_cms_label(lumi, "4orMoreJets", "2orMoreBtags")
    cms_label.Draw()
    gPad.RedrawAxis()
    
    canvasFit.SaveAs("Fit.pdf")

def add_cms_label(lumi, njet="4orMoreJets", nbjet="0orMoreBtag"):
    jetBinsLatex = {'0jet':'0 jet', '0orMoreJets':'#geq 0 jets', '1jet':'1 jet', '1orMoreJets':'#geq 1 jet',
                    '2jets':'2 jets', '2orMoreJets':'#geq 2 jets', '3jets':'3 jets', '3orMoreJets':'#geq 3 jets',
                    '4orMoreJets':'#geq 4 jets'}
    
    BjetBinsLatex = {'0btag':'0 b-tags', '0orMoreBtag':'#geq 0 b-tags', '1btag':'1 b-tags',
                    '1orMoreBtag':'#geq 1 b-tags',
                    '2btags':'2 b-tags', '2orMoreBtags':'#geq 2 b-tags',
                    '3btags':'3 b-tags', '3orMoreBtags':'#geq 3 b-tags',
                    '4orMoreBtags':'#geq 4 b-tags'}

    mytext = TPaveText(0.35, 0.8, 0.6, 0.93, "NDC")
    mytext.AddText("CMS Preliminary")
    mytext.AddText("%.1f pb^{-1} at  #sqrt{s} = 7 TeV" % lumi)
    if njet != "":
        mytext.AddText("e, %s, %s" % (jetBinsLatex[njet], BjetBinsLatex[nbjet]))   
             
    mytext.SetFillStyle(0)
    mytext.SetBorderSize(0)
    mytext.SetTextFont(42)
    mytext.SetTextSize(0.04)
    mytext.SetTextAlign(13)
    return mytext


#Running the fit and plotting the results
tempFit()
plotResult()
