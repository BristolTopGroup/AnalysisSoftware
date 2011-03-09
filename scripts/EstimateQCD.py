from HistGetter import *
from tdrStyle import *
from ROOT import *
from math import pow, exp
YMAX_all = 350;
normMode = 1
nj = 'allJets'
lumi = 36.145;
oldLumi = 36.145;
scale = lumi / oldLumi;
    
def setStyle():
    tdrstyle = setTDRStyle();
    tdrstyle.SetPadRightMargin(0.05)
    tdrStyle.SetStatH(0.2);
    tdrStyle.SetOptStat(0);#off title
    tdrStyle.SetOptFit(0);#off title
    tdrStyle.cd();
    gROOT.ForceStyle();
    
def getHisto():
    
    path = '/storage/workspace/BristolAnalysisTools/outputfiles/Fall10_NovRereco/'
    files = {'data': path + "data_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'ttbar' :  path + "ttjet_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'wjets' :  path + "wj_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'zjets' :  path + "zj_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'bce1' :  path + "bce1_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'bce2' :  path + "bce2_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'bce3' :  path + "bce3_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'enri1' : path + "enri1_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'enri2' :  path + "enri2_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'enri3' :  path + "enri3_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'pj1' :  path + "pj1_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'pj2' :  path + "pj2_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'pj3' :  path + "pj3_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'tW' :  path + "tW_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'tchan' :  path + "tchan_36.145pb_PFElectron_PF2PATJets_PFMET.root"}
    HistGetter.hists = ['QCDest_CombRelIso_allJets', 'QCDest_CombRelIso_1orMoreJets', 'QCDest_CombRelIso_2orMoreJets', 
                        'QCDest_CombRelIso_3orMoreJets', 'QCDest_CombRelIso_4orMoreJets']
    qcdSamples = ['bce1', 'bce2', 'bce3', 'enri1', 'enri2','enri3', 'pj1', 'pj2', 'pj3']
    allMCSamples = ['ttbar', 'wjets', 'zjets','tW', 'tchan', 'bce1', 'bce2', 'bce3', 'enri1', 
                        'enri2','enri3', 'pj1', 'pj2', 'pj3']
    singleTopSamples = ['tW', 'tchan']
    
    samplesOfInterest = ['data','qcd','zjets', 'wjets', 'singleTop','ttbar']
    colors = {'ttbar' :  kRed + 1,
    'wjets' :  kGreen - 3,
    'zjets' :  kAzure - 2,
    'qcd' :  kYellow,
    'singleTop' :  kMagenta}
    HistGetter.samplefiles = files
    HG = HistGetter()
    HG.setStyle()
    hists = HG.getHistsFromFiles()
    hists = HG.addSampleSum(hists)
    this_rebin = 10;
    mcStack = {}
    for sample in samplesOfInterest:#sample
        for histname in hists[sample].keys():
            hists[sample][histname].Rebin(this_rebin)
            if not sample == 'data':
                hists[sample][histname].Scale(scale)
                hists[sample][histname].SetFillStyle(1001)
                if(not sample in qcdSamples and not sample in singleTopSamples and not sample == 'allMC'):
                    hists[sample][histname].SetFillColor(colors[sample])
                    if mcStack.has_key(histname):
                        mcStack[histname].Add(hists[sample][histname])
                    else:
                        mcStack[histname] = THStack("MC_" + histname, "MC_" + histname);
                        mcStack[histname].Add(hists[sample][histname])
            else:
                hists[sample][histname].SetMarkerStyle(8);
                
    
    hists['MCStack'] = mcStack#THStack("MC", "MC");
    
    return hists

def calculate_gaus_unc(a, b, c):

    # This formula gives wrong results. ??
    print "a: " , a
    print "b: " , b
    print "c: " , c
    x1 = 0.1;
    exp1 = exp(-0.5 * pow((x1 - b) / c, 2));
    exp2 = exp(-0.5 * pow(b / c, 2));
    est = -a * c * c / x1 * (exp1 / (x1 - b) + exp2 / b);
    #print , "est: ", est , 
    
def add_legend(hists, func = "pol1"):
    histname = 'QCDest_CombRelIso_%s' % nj
    tt = hists['ttbar'][histname]
    wj = hists['wjets'][histname]
    zj = hists['zjets'][histname]
    data = hists['data'][histname]
    QCD = hists['qcd'][histname]
    stop = hists['singleTop'][histname]
    leg = TLegend(0.64, 0.4, 0.9, 0.9);

    leg.SetFillStyle(0);
    leg.SetBorderSize(0);
    leg.SetTextFont(42);

    # Here I define coloured lines for use in the legend
    blue = TF1("blue", "pol0", 0, 1);
    red = TF1("red", "pol0", 0, 1);

    blue.SetLineColor(kBlue);
    red.SetLineColor(kRed);

    red.SetLineWidth(2);
    blue.SetLineWidth(2);

    blue.SetLineStyle(kDashed);

    # Add entry to legend
    leg.AddEntry(data, "Data", "LP");
    if func == "pol1":
        leg.AddEntry(red, "Linear Fit", "l");
    elif func == "expo":
        leg.AddEntry(red, "Exponenetial Fit", "l");
    elif func == "gaus":
        leg.AddEntry(red, "Gaussian Fit", "l");
    leg.AddEntry(blue, "Extrapolation", "l");
    leg.AddEntry(tt, "t#bar{t}", "F");
    leg.AddEntry(stop,  "Single-Top", "F");
    leg.AddEntry(wj, "W#rightarrowl#nu", "F");
    leg.AddEntry(zj, "Z/#gamma*#rightarrowl^{+}l^{-}", "F");
    leg.AddEntry(QCD, "QCD & #gamma+jets", "F");
    return leg

def add_cms_label(intlumi, njet = ""):

    mytext = TPaveText(0.3, 0.8, 0.6, 0.93, "NDC");
    mytext.AddText("CMS Preliminary");
    mytext.AddText("%.1f pb^{-1} at  #sqrt{s} = 7 TeV"% lumi);
    if njet != "":
        mytext.AddText("e+jets, N_{jets} = %s"% njet)
    mytext.SetFillStyle(0);
    mytext.SetBorderSize(0);
    mytext.SetTextFont(42);
    mytext.SetTextAlign(13);
    return mytext

def fit_njet(hists, function = "pol1", Fit_From_user = 0.1):
    histname = 'QCDest_CombRelIso_%s' % nj
    mcStack = hists['MCStack'][histname]
    data = hists['data'][histname]
    QCD = hists['qcd'][histname]
    tt = hists['ttbar'][histname]
    wj = hists['wjets'][histname]
    zj = hists['zjets'][histname]
    stop = hists['singleTop'][histname]
    TheEstimates = open('MyEstimates.txt', 'a')
    print "\nSwitched from chi2 fit to likelihood fit (better with low stats)\n" 

    Fit_From = Fit_From_user;
    print "-----------------------" 
    print " FIT:  ", function, "   range ", Fit_From, "--1.6" 
    print "-----------------------" 

    lw = 2; #line width (function)


    c1 = TCanvas("c1", "RelIso fit", 600, 400);#,"",500,500);




    data.GetXaxis().SetRangeUser(0, 1.6 - 0.01);
    data.Draw();
    max = 0;
    if mcStack.GetMaximum() > data.GetMaximum():
        max = mcStack.GetMaximum()*1.1
    else:
       max =  data.GetMaximum()*1.1
    data.GetYaxis().SetRangeUser(0, max);

    # draw mc
    mcStack.Draw("hist same");
    data.Draw("ae same");
#    data.Draw("axis same");

    print "Fit Range: ", Fit_From, "-1.6" 

    # Fit options used:
    #  L : likelihood method
    #  S : store fit results
    #  0 : do not draw
    myFitResult = data.Fit(function, "0SL", "ah", Fit_From, 1.6); # <----Fit Range
    print "func ", function 
    # Fit line in red
    #  myf = data.GetFunction("pol1");
    myf = data.GetFunction(function);
    myf.SetLineColor(kRed);
    myf.SetLineWidth(lw);

    # Extrapolation in dashed blue
    myf2 = myf.Clone(); #range 0-0.1
    myf2.SetLineColor(kBlue);
    myf2.SetLineStyle(kDashed);
    myf2.SetLineWidth(lw);
    myf2.SetRange(0, Fit_From);

    myf.Draw("same");
    myf2.Draw("same");
    # Add "CMS Preliminary", integrated luminosity and sqrt(s), and legend
#    add_cms_label(lumi, nj).Draw('same');
    mytext = TPaveText(0.3, 0.8, 0.6, 0.93, "NDC");
    mytext.AddText("CMS Preliminary");
    mytext.AddText("%.1f pb^{-1} at  #sqrt{s} = 7 TeV"% lumi);
    if nj != "":
        mytext.AddText("e+jets, N_{jets} = %s"% nj)
    mytext.SetFillStyle(0);
    mytext.SetBorderSize(0);
    mytext.SetTextFont(42);
    mytext.SetTextAlign(13);
    mytext.Draw()
    #  add_fit_res( Fit_From, chi2, ndf );
#    leg = add_legend(hists, function)
    leg = TLegend(0.64, 0.4, 0.9, 0.9);

    leg.SetFillStyle(0);
    leg.SetBorderSize(0);
    leg.SetTextFont(42);

    # Here I define coloured lines for use in the legend
    blue = TF1("blue", "pol0", 0, 1);
    red = TF1("red", "pol0", 0, 1);

    blue.SetLineColor(kBlue);
    red.SetLineColor(kRed);

    red.SetLineWidth(2);
    blue.SetLineWidth(2);

    blue.SetLineStyle(kDashed);

    # Add entry to legend
    leg.AddEntry(data, "Data", "LP");
    if function == "pol1":
        leg.AddEntry(red, "Linear Fit", "l");
    elif function == "expo":
        leg.AddEntry(red, "Exponenetial Fit", "l");
    elif function == "gaus":
        leg.AddEntry(red, "Gaussian Fit", "l");
    leg.AddEntry(blue, "Extrapolation", "l");
    leg.AddEntry(tt, "t#bar{t}", "F");
    leg.AddEntry(stop,  "Single-Top", "F");
    leg.AddEntry(wj, "W#rightarrowl#nu", "F");
    leg.AddEntry(zj, "Z/#gamma*#rightarrowl^{+}l^{-}", "F");
    leg.AddEntry(QCD, "QCD & #gamma+jets", "F");
    leg.Draw()
    # Get estimate from extrapolation
    n_extrap = myf2.Integral(0, 0.1) / 0.1; #note divided by bin width=0.1

    p0 = myf.GetParameter(0);
    p1 = myf.GetParameter(1);
    e0 = myf.GetParError(0);
    e1 = myf.GetParError(1);

    chi2 = myFitResult.Chi2();
    ndf = myFitResult.Ndf();

    print "------------" 
    print "Fit results" 
    print "------------" 
    print "n extrap = ", n_extrap 
    print "p0 = ", p0, " +/- ", e0 
    print "p1 = ", p1, " +/- ", e1 
    print "chi2/ndf =  ", chi2, " / ", ndf 

    # Construct functions to see how estimate varies within fit parameter
    # uncertainties, ie error of p0 and p1

    # vary p0 (normalization)
    #  newF1_up = TF1("pol1","pol1",0,1.6);
    newF1_up = TF1(function, function, 0, 1.6);
    newF1_up.SetParameters(p0 + e0, p1);
    newF1_up.SetLineColor(kGreen);
    newF1_up.SetLineWidth(lw);

    newF1_down = TF1(function, function, 0, 1.6);
    newF1_down.SetParameters(p0 - e0, p1);
    newF1_down.SetLineColor(kGreen);
    newF1_down.SetLineWidth(lw);

    # Get the estimates
    est_1 = newF1_up.Integral(0, 0.1) / 0.1;
    est_2 = newF1_down.Integral(0, 0.1) / 0.1;
    #est_3 = newF2_up.Integral(0,0.1)/0.1;
    #est_4 = newF2_down.Integral(0,0.1)/0.1;
    print "n extrap (p0 + e0,   p1     ) = ", est_1 
    print "n extrap (p0 - e0,   p1     ) = ", est_2 
    #print "n extrap (p0     ,   p1 + e1) = ", est_3 
    #print "n extrap (p0     ,   p1 - e1) = ", est_4 

    # take the maximum deviation as the uncertainty
    est_unc = 0;
    if fabs(est_1 - n_extrap) > est_unc:
        est_unc = fabs(est_1 - n_extrap);
    if fabs(est_2 - n_extrap) > est_unc:
        est_unc = fabs(est_2 - n_extrap);
    #if( fabs(est_3 - n_extrap) > est_unc ) est_unc = fabs(est_3 - n_extrap);
    #if( fabs(est_4 - n_extrap) > est_unc ) est_unc = fabs(est_4 - n_extrap);

    print "Take the maximum deviation as the uncertainty of the QCD estimate." 
    print "--------------------------------------------" 
    print "  RESULT: ", function, " ", Fit_From, "-1.6" 
    print "--------------------------------------------" 
    # print out MC prediction for QCD + photon+jets
    if normMode == 1:
        print "\n  QCD predicted =  ", QCD.Integral( 0, 1 ), " (Lumi)"
        TheEstimates.write( "\n  QCD predicted =  " + str( QCD.Integral( 0, 1 ) ) + " (Lumi)" )

    print "\n  QCD estimate  =  ", n_extrap, "  +/-  ", est_unc, " (vary p0)"
    TheEstimates.write( "  QCD estimate  =  " + str( n_extrap ) + "  +/-  " + str( est_unc ) + " (vary p0)" )

    #----------------------

    # error propagation
    #----------------------
    N = 0;
    sigmaN = 0;

    if function == "pol1":
        c = p0;
        m = p1;

        eC = e0;
        eM = e1;

        N = 0.05 * m + c;
        sigmaN = TMath.Hypot(0.05 * eM, eC);

    elif function == "expo":
        a = p0;
        b = p1;
        eA = e0;
        eB = e1;

        x2 = 0.1;

        exp_ab = exp(a + 0.1 * b);
        exp_a = exp(a);

        N = 10 / b * (exp_ab - exp_a);

        # del(N)/del(a/b)
        dNda = 1 / (b * x2) * (exp_ab - exp_a);
        dNdb = -1 / (b * b * x2) * (exp_ab - exp_a) + 1 / b * exp_ab;

        # Note: sigma(N) = sqrt( dNda^2*eA^2 + dBdb^2*eB^2  )
        sigmaN = TMath.Hypot(dNda * eA, dNdb * eB);
    elif function == "gaus":
        p2 = myf.GetParameter(2);
        e2 = myf.GetParError(2);
        # Not finished. Calculated estimate is wrong.
        print "\n-. Error propagation for Gaussian not yet implemented!" 
        #calculate_gaus_unc(p0,p1,p2);

    print "\n  QCD estimate  =  ", N;
    print "  +/-  ", sigmaN, " (error propagation)\n\n";
    print "--------------------------------------------" 


    # save as eps, then convert to pdf
    print "Fit_From=", Fit_From 
    out = '';
    if function == "pol1":
        out = "ele_reliso_fit_%s_linear_from_0%.0f"% (nj, Fit_From * 10.0);
    else:
        out = "ele_reliso_fit_%s_%s_from_0%.0f" % ( function, nj, Fit_From * 10.0);

    if normMode == 1:
        out += "_normToLumi";
    else:
        out += "_normToData";

    print "out: ", out 
    c1.SaveAs(out + '.eps');
    gROOT.ProcessLine(".!ps2pdf -dEPSCrop %s.eps" % out);
    gROOT.ProcessLine(".!rm -f %s.eps" % out);

    c1.Close(); #crucial!
    TheEstimates.close()
    
if __name__ == '__main__':
    global nj
    hists = getHisto()
    nj = 'allJets'
    fit_njet(hists, 'gaus', 0.1)
    fit_njet(hists, 'gaus', 0.2)
    fit_njet(hists, 'gaus', 0.3)
    nj = '4orMoreJets'
    fit_njet(hists, 'gaus', 0.1)
    fit_njet(hists, 'gaus', 0.2)
    fit_njet(hists, 'gaus', 0.3)
#    make_QCDnormToEstimate_plot()
    