from __future__ import division
from numpy import arange
from tdrStyle import *
from ROOT import *
from QCDEstimation import getQCDEstimate, estimateQCDFor
import QCDEstimation

import HistPlotter
import HistGetter
import inputFiles

from optparse import OptionParser
canvases = []
scanvases = []
setLogY = False

def plotMttbar():
    saveAs = HistPlotter.saveAs
    
    performRescale = False
#    tdrstyle = setTDRStyle();
#    gStyle.SetHatchesSpacing(1.0);
    HistPlotter.setStyle()
    lumi = 1091.45
    oldLumi = 1091.45
    scale = lumi / oldLumi;
    prefix = 'CiCID_'
    outputFormats = ['pdf', 'png']
    reverseMCOrder = True
    qcdFromData = 'topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets_0orMoreBtag'
    wjetScale = 1.
    topScale = 1.
    savePath = "/storage/results/plots/ElectronHad/"
    print 'Top scale', topScale
    print 'W+jets scale', wjetScale
    hists = [];
#    hists.append('topReconstruction/backgroundShape/mttbar_QCDEnriched')
#    hists.append('topReconstruction/backgroundShape/mttbar_antiID_withMETAndAsymJets')
#    hists.append('topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets')
#    hists.append('topReconstruction/backgroundShape/mttbar_controlRegion_withMETAndAsymJets')
#    hists.append('topReconstruction/backgroundShape/mttbar_antiIsolated_withMETAndAsymJets')
    
    
    hists.append("topReconstruction/mttbar_withMETAndAsymJets");
#    hists.append("topReconstruction/mttbar_2ndSolution_withMETAndAsymJets");
#    hists.append("topReconstruction/mttbar_3rdSolution_withMETAndAsymJets");
#    hists.append("topReconstruction/mttbar_allSolutions_withMETAndAsymJets");
#    
#    hists.append("topReconstruction/ttbar_pt_withMETAndAsymJets");
#    hists.append("topReconstruction/ttbar_pt_2ndSolution_withMETAndAsymJets");
#    hists.append("topReconstruction/ttbar_pt_3rdSolution_withMETAndAsymJets");
#    hists.append("topReconstruction/ttbar_pt_allSolutions_withMETAndAsymJets");
#    
#    hists.append("topReconstruction/angleTops_withMETAndAsymJets");
    
#    hists.append("topReconstruction/pt_leadingTop");
#    hists.append("topReconstruction/pt_leadingTop_withMETAndAsymJets");
    
#    hists.append("topReconstruction/pt_NextToLeadingTop");
#    hists.append("topReconstruction/pt_NextToLeadingTop_withMETAndAsymJets");
##    
#    hists.append("topReconstruction/mLeptonicTop_withMETAndAsymJets");
#    hists.append("topReconstruction/mHadronicTop_withMETAndAsymJets");
#    hists.append("topReconstruction/mAllTop_withMETAndAsymJets");
    
#    hists.append("topReconstruction/ttbar_px");
#    hists.append("topReconstruction/ttbar_py");
#    hists.append("topReconstruction/ttbar_pz");
#    hists.append("topReconstruction/ttbar_px_withMETAndAsymJets");
#    hists.append("topReconstruction/ttbar_py_withMETAndAsymJets");
#    hists.append("topReconstruction/ttbar_pz_withMETAndAsymJets");
    
#    hists.append("topReconstruction/m3_withMETAndAsymJets");
#    hists.append("topReconstruction/HT_withMETAndAsymJets");
#    hists.append("topReconstruction/MET");
    hists.append("topReconstruction/MET_withMETAndAsymJets");
#    hists.append("jetStudy/AllJetMass");
#    hists.append("jetStudy/AllGoodJetMass");
#    hists.append("jetStudy/GoodJetMass_atLeastOneJets");
#    hists.append("jetStudy/GoodJetMass_atLeastTwoJets");
#    hists.append("jetStudy/GoodJetMass_atLeastThreeJets");
#    hists.append("jetStudy/GoodJetMass_atLeastFourJets");
#    hists.append("topReconstruction/mtW_withMETAndAsymJets");
#    hists.append("topReconstruction/neutrino_pz");
#    hists.append("QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts")
#    hists.append("QCDStudy/PFIsolation_controlRegion")

    files = inputFiles.files
    hists = HistGetter.getHistsFromFiles(hists, files, bJetBins=HistPlotter.allBjetBins)
    suffixes = HistPlotter.allBjetBins
#    suffixes = ["0orMoreBtag", '0btag',
#        "1orMoreBtag",
#        "2orMoreBtags",
#        "3orMoreBtags", '4orMoreBtags' ]
#    hists = [hist + '_' + suffix for hist in hists for suffix in suffixes]
#    
    jetBinned = HistPlotter.allJetBins
    jetBinnedhists = []
#    jetBinnedhists.append('diElectronAnalysis/diElectronMass')
#    jetBinnedhists.append('diElectronAnalysis/diElectronMass_iso')
#    jetBinnedhists.append("QCDStudy/QCDest_PFIsolation_WithMETCutAndAsymJetCuts")
#    jetBinnedhists.append("QCDStudy/QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts")
#    
    
    jetBinnedhists = HistGetter.getHistsFromFiles(jetBinnedhists, files, jetBins=HistPlotter.allJetBins)
    
    otherHists = []
#    otherHists.append('pileupStudy/nVertex')
#    otherHists.append('pileupStudy/nVertex_reweighted')
    otherHists = HistGetter.getHistsFromFiles(otherHists, files)
    hists = HistGetter.joinHistogramDictionaries([hists, jetBinnedhists, otherHists])
    gcd = gROOT.cd

    for histname in hists[hists.keys()[0]]:
        print '='*70
        print 'Plotting:', histname
        gcd()
        
        hist_data = hists['data'][histname]

        hist_ttbar = hists['ttbar'][histname]
        hist_wjets = hists['wjets'][histname]
        hist_zjets = hists['zjets'][histname]

        hist_bce1 = hists['bce1'][histname]
        hist_bce2 = hists['bce2'][histname]
        hist_bce3 = hists['bce3'][histname]
        hist_enri1 = hists['enri1'][histname]
        hist_enri2 = hists['enri2'][histname]
        hist_enri3 = hists['enri3'][histname]
        hist_pj1 = hists['pj1'][histname]
        hist_pj2 = hists['pj2'][histname]
        hist_pj3 = hists['pj3'][histname]
        hist_singleTop = hists['tW'][histname]
        hist_singleTop.Add(hists['tchan'][histname])
        
        hist_diboson = hists['ww'][histname]
        hist_diboson.Add(hists['wz'][histname])
        hist_diboson.Add(hists['zz'][histname])

        hist_ttbar.Scale(scale);
        hist_ttbar.Scale(topScale);
        hist_wjets.Scale(scale);
        hist_wjets.Scale(wjetScale);
        hist_zjets.Scale(scale);
        hist_singleTop.Scale(scale)
        hist_diboson.Scale(scale)
        hist_bce1.Scale(scale);
        hist_bce2.Scale(scale);
        hist_bce3.Scale(scale);

        hist_enri1.Scale(scale);
        hist_enri2.Scale(scale);
        hist_enri3.Scale(scale);

        hist_pj1.Scale(scale);
        hist_pj2.Scale(scale);
        hist_pj3.Scale(scale);

#        hist_Zprime500.Scale(scale);
#        hist_Zprime750.Scale(scale);
#        hist_Zprime1000.Scale(scale);
#        hist_Zprime1250.Scale(scale);
#        hist_Zprime4000.Scale(scale);

        hist_qcd = hist_bce1.Clone("qcd")
        hist_qcd.Add(hist_bce2);
        hist_qcd.Add(hist_bce3);
        hist_qcd.Add(hist_enri1);
        hist_qcd.Add(hist_enri2);
        hist_qcd.Add(hist_enri3);
        hist_qcd.Add(hist_pj1);
        hist_qcd.Add(hist_pj2);
        hist_qcd.Add(hist_pj3);
        nqcd = hist_qcd.Integral();
        mttbars = ['topReconstruction/mttbar_' + suffix for suffix in suffixes]
        mttbars2 = ['topReconstruction/mttbar_withMETAndAsymJets_' + suffix for suffix in suffixes]
        shapeErrorHist = None 
        relativeQCDEstimationError = 0
        if histname in mttbars or histname in mttbars2:
#            print "Taking QCD shape from DATA (%s)" % qcdFromData
            qcdHists = HistGetter.getHistsFromFiles([qcdFromData], {'data':files['data']})
            hist_qcd = qcdHists['data'][qcdFromData]
            shapeErrorHist = QCDEstimation.getShapeErrorHistogram(qcdFromData, files)
                
        if not 'background' in histname and not 'QCDStudy' in histname and 'withMETAndAsymJets' in histname:  
            currentBin = HistPlotter.getBjetBin(histname)
              
            estimate, err = getQCDEstimate(files['data'], bjetBin=currentBin)
            if not estimate == 0:
                relativeQCDEstimationError = err / estimate
                
            print 'Estimated QCD background: %.1f +- %.1f' % (estimate, err)
            if(hist_qcd.Integral() > 0 and estimate >= 0):
                hist_qcd.Scale(estimate / hist_qcd.Integral())
                
                
        if performRescale:        
            print '=' * 100
            print '"best" rescaling for', histname
            rescales = rescaleMC(hist_data, hist_ttbar, hist_wjets, hist_qcd)
            print rescales
            print '=' * 100
            topScale = rescales[1]['ttbar']
            wjetScale = rescales[1]['wjets']
            qcdScale2 = rescales[1]['qcd']

            hist_ttbar.Scale(topScale)
            hist_wjets.Scale(wjetScale)
            hist_qcd.Scale(qcdScale2) 
        
        

        rebin = 1;
        Urange = (0, 5000)
        if ("mttbar" in histname):
            hist_data.SetXTitle("m(t#bar{t})/GeV");
            hist_data.SetYTitle("Events/(50 GeV)");
            rebin = 50;
            Urange = (250, 3000)
        elif ("m3" in histname):
            hist_data.SetXTitle("M3/GeV");
            hist_data.SetYTitle("Events/(50 GeV)");
            rebin = 50;
            Urange = (0, 2000)
        elif (histname == "electron_et"):
            hist_data.SetXTitle("electron p_{T}/GeV");
            hist_data.SetYTitle("Events/(5 GeV)");
            rebin = 5;
        elif ("ttbar_pt" in histname):
            hist_data.SetXTitle("p_{T} of t#bar{t} system/GeV");
            hist_data.SetYTitle("Events/(10 GeV)");
            rebin = 10;
            Urange = (0, 1000)
        elif ("ttbar_px" in histname):
            hist_data.SetXTitle("p_{x} of t#bar{t} system/GeV");
            hist_data.SetYTitle("Events/(10 GeV)");
            rebin = 10;
            Urange = (0, 1000)
        elif ("ttbar_py" in histname):
            hist_data.SetXTitle("p_{y} of t#bar{t} system/GeV");
            hist_data.SetYTitle("Events/(10 GeV)");
            rebin = 10;
            Urange = (0, 1000)
        elif ("ttbar_pz" in histname):
            hist_data.SetXTitle("p_{z} of t#bar{t} system/GeV");
            hist_data.SetYTitle("Events/(50 GeV)");
            rebin = 50;
            Urange = (0, 2500)
        elif ("HT" in histname):
            hist_data.SetXTitle("#Sigma p_{T}/GeV");
            hist_data.SetYTitle("Events/(50 GeV)");
            rebin = 50;
            Urange = (0, 2500)
        elif (histname == "numberOfJets"):
            hist_data.SetXTitle("number of jets");
            hist_data.SetYTitle("Events");
        elif (histname == "numberOfBJets"):
            hist_data.SetXTitle("number of b-tagged jets (SSVHE medium)");
            hist_data.SetYTitle("Events");
        elif ('MET_' in histname):
            hist_data.SetXTitle("MET/GeV");
            hist_data.SetYTitle("Events/(20 GeV)");
            rebin = 4;
            Urange = (0, 600)
        elif ("mtW" in histname):
            hist_data.SetXTitle("transverse W-boson mass/GeV");
            hist_data.SetYTitle("Events/(10 GeV)");
            rebin = 10;
            Urange = (0, 500)
        elif ("electronD0" in histname):
            hist_data.SetXTitle("electron d_{0} / cm");
            hist_data.SetYTitle("Events/(0.001 cm)");
            rebin = 10;
        elif ("angleTops" in histname):
            hist_data.SetXTitle("angle between top quarks");
            hist_data.SetYTitle("Events/(0.1 rad)");
            rebin = 20;
        elif ("neutrino_pz" in histname):
            hist_data.SetXTitle("neutrino p_{Z} /GeV");
            hist_data.SetYTitle("Events/(10 GeV)");
            rebin = 10;
            Urange = (-500, 500)
        elif ('mHadronicTop' in histname or 'mLeptonicTop' in histname or 'mAllTop' in histname):
            hist_data.SetXTitle("top mass /GeV");
            hist_data.SetYTitle("Events/(20 GeV)");
            rebin = 20;
            Urange = (0, 1500)
        elif ('pt_leadingTop' in histname or 'pt_NextToLeadingTop' in histname):
            hist_data.SetXTitle("top p_{T} /GeV");
            hist_data.SetYTitle("Events/(20 GeV)");
            rebin = 20;
        elif('QCDest_CombRelIso' in histname):
            hist_data.SetXTitle("relative isolation");
            hist_data.SetYTitle("Events/(0.1)");
            rebin = 10;
            Urange = (0, 2)
        elif('PFIsolation' in histname):
            hist_data.SetXTitle("Relative isolation");
            hist_data.SetYTitle("Events/(0.1)");
            rebin = 10
            Urange = (0, 1.6)
        elif('diElectron' in histname):
            hist_data.SetXTitle("m(ee)");
            hist_data.SetYTitle("Events/(10 GeV)");
            rebin = 10
            Urange = (50, 2000)
        elif 'JetMass' in histname:
            hist_data.SetXTitle("m(jet)");
            hist_data.SetYTitle("Events/(5 GeV)");
            rebin = 5
            Urange = (0, 200)
        
        hist_data.Rebin(rebin);
        hist_ttbar.Rebin(rebin);
        hist_wjets.Rebin(rebin);
        hist_zjets.Rebin(rebin);
        hist_qcd.Rebin(rebin);
        hist_singleTop.Rebin(rebin)
        hist_diboson.Rebin(rebin)
        hist_data.SetAxisRange(Urange[0], Urange[1]);
        hist_ttbar.SetAxisRange(Urange[0], Urange[1]);
        hist_wjets.SetAxisRange(Urange[0], Urange[1]);
        hist_zjets.SetAxisRange(Urange[0], Urange[1]);
        hist_qcd.SetAxisRange(Urange[0], Urange[1]);
        hist_singleTop.SetAxisRange(Urange[0], Urange[1]);
        hist_diboson.SetAxisRange(Urange[0], Urange[1]);

        hist_data.SetMarkerStyle(8);
        
        hist_ttbar.SetFillStyle(1001);
        hist_ttbar.SetFillColor(kRed + 1);
        hist_wjets.SetFillStyle(1001);
        hist_wjets.SetFillColor(kGreen - 3);
        hist_zjets.SetFillStyle(1001);
        hist_zjets.SetFillColor(kAzure - 2);
        hist_qcd.SetFillStyle(1001);
        hist_qcd.SetFillColor(kYellow);
        hist_singleTop.SetFillStyle(1001);
        hist_singleTop.SetFillColor(kMagenta)
        
        hist_diboson.SetFillStyle(1001);
        hist_diboson.SetFillColor(kWhite)

#        hist_Zprime500.SetLineColor(kTeal - 9);
#        hist_Zprime500.SetFillStyle(0);
#        hist_Zprime500.SetFillColor(kWhite);
#
#        hist_Zprime750.SetLineColor(kBlue - 6);
#        hist_Zprime750.SetFillStyle(0);
#        hist_Zprime750.SetFillColor(kWhite);
#
#        hist_Zprime1000.SetLineColor(28);
#        hist_Zprime1000.SetFillStyle(0);
#        hist_Zprime1000.SetFillColor(kWhite);
#
#        hist_Zprime1250.SetLineColor(kCyan - 5);
#        hist_Zprime1250.SetFillStyle(0);
#        hist_Zprime1250.SetFillColor(kWhite);
#
#        hist_Zprime4000.SetLineColor(kOrange + 1);
#        hist_Zprime4000.SetFillStyle(0);
#        hist_Zprime4000.SetFillColor(kWhite);

        leg = TLegend(0.696, 0.95, 0.94, 0.55);
        leg.SetBorderSize(0);
        leg.SetLineStyle(0);
        leg.SetTextFont(42);
        leg.SetFillStyle(0);

        leg.AddEntry(hist_data, "data '11", "P");
        #        leg.AddEntry(hist_data2, "data(no HLT)", "P");
        leg.AddEntry(hist_ttbar, "t#bar{t}", "f");
        leg.AddEntry(hist_wjets, "W#rightarrowl#nu", "f");
        leg.AddEntry(hist_zjets, "Z/#gamma*#rightarrowl^{+}l^{-}", "f");
        leg.AddEntry(hist_qcd, "QCD/#gamma + jets");
        leg.AddEntry(hist_singleTop, "Single-Top")
        leg.AddEntry(hist_diboson, "VV + X")
        
#        leg.AddEntry(hist_Zprime500, "Z' 0.5TeV (50pb)");
#        leg.AddEntry(hist_Zprime750, "Z' 0.75TeV (50pb)");
#        leg.AddEntry(hist_Zprime1000, "Z' 1TeV (50pb)");
#        leg.AddEntry(hist_Zprime1250, "Z' 1.25TeV (50pb)");
#        leg.AddEntry(hist_Zprime4000, "Z' 4TeV (50pb)");

        
        canvases.append(TCanvas("cname" + histname, histname, 1200, 900))
        canvases[-1].cd().SetRightMargin(0.04);
        
        errorHist = QCDEstimation.createErrorHistogram([hist_ttbar, hist_wjets, hist_zjets, hist_singleTop, hist_diboson, hist_qcd],
                                                       hist_qcd, relativeQCDEstimationError, shapeErrorHist)
        
        
        hs = THStack("MC", "MC");
        if reverseMCOrder:
            hs.Add(hist_ttbar);
            hs.Add(hist_wjets);
            hs.Add(hist_zjets);
            hs.Add(hist_singleTop);
            hs.Add(hist_qcd);
            hs.Add(hist_diboson);
        
        else:
            hs.Add(hist_diboson);
            hs.Add(hist_qcd);
            hs.Add(hist_zjets);
            hs.Add(hist_wjets);
            hs.Add(hist_singleTop);
            hs.Add(hist_ttbar);
            
            
            
            
        
        
        
        #hs.SetMinimum(1)
        #hist_data.SetMinimum(1)
        max = 0
        if hs.GetMaximum() > hist_data.GetMaximum():
            max = hs.GetMaximum()*1.4
        else:
            max = hist_data.GetMaximum()*1.4
        hist_data.GetYaxis().SetRangeUser(0, max);
        if setLogY:
            hist_data.GetYaxis().SetRangeUser(0.1, max);
        hist_data.Draw('error');
        hs.Draw("hist same");
#        hist_Zprime500.Draw("same");
#        hist_Zprime750.Draw("same");
#        hist_Zprime1000.Draw("same");
#        hist_Zprime1250.Draw("same");
#        hist_Zprime4000.Draw("same");
        if errorHist:
            gStyle.SetErrorX(0.5);
            errorHist.SetFillColor(kGray + 3)
            errorHist.SetMarkerStyle(0)
            errorHist.SetFillStyle(3001);
            leg.AddEntry(errorHist, "QCD Error")
            errorHist.Draw('E2 same')
            
        hist_data.Draw("error same");
        leg.Draw();
        text1 = HistPlotter.get_cms_label(lumi, HistPlotter.getJetBin(histname),
                                          HistPlotter.getBjetBin(histname))
        text1.Draw();
#
        postfix = ''
        if setLogY:
            canvases[-1].SetLogy(1)
            postfix = '_log'
        name = ''.join(histname[:histname.rfind('/') + 1]) + prefix + ''.join(histname[histname.rfind('/') + 1:])
        saveAs(canvas=canvases[-1], name=name + postfix, outputFormats=outputFormats, outputFolder=savePath)
        del canvases[-1]

        cu_hist_data = getCumulativePlot(hist_data, "data_" + histname);
        cu_hist_ttbar = getCumulativePlot(hist_ttbar, "ttbar_" + histname);
        cu_hist_wjets = getCumulativePlot(hist_wjets, "wjets_" + histname);
        cu_hist_zjets = getCumulativePlot(hist_zjets, "zjets_" + histname);
        cu_hist_qcd = getCumulativePlot(hist_qcd, "qcd_" + histname);
        cu_hist_singleTop = getCumulativePlot(hist_singleTop, "singleTop_" + histname);
        cu_hist_diboson = getCumulativePlot(hist_diboson, "di-boson_" + histname);
##        cu_hist_Zprime500 = getCumulativePlot(hist_Zprime500, "Zprime500");
##        cu_hist_Zprime750 = getCumulativePlot(hist_Zprime750, "Zprime750");
##        cu_hist_Zprime1000 = getCumulativePlot(hist_Zprime1000, "Zprime1000");
##        cu_hist_Zprime1250 = getCumulativePlot(hist_Zprime1250, "Zprime1250");
#        cu_hist_Zprime4000 = getCumulativePlot(hist_Zprime4000, "Zprime4000");
        cu_hist_data.SetYTitle("Integrated Events/(50 GeV)");
##        
#
        cu_hist_data.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_ttbar.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_wjets.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_zjets.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_qcd.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_singleTop.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_diboson.SetAxisRange(Urange[0], Urange[1]);
        
        cu_hs = THStack("cu_MC", "cu_MC");
        cu_hs.Add(cu_hist_diboson);
        cu_hs.Add(cu_hist_ttbar);
        cu_hs.Add(cu_hist_wjets);
        cu_hs.Add(cu_hist_zjets);
        cu_hs.Add(cu_hist_singleTop);
        cu_hs.Add(cu_hist_qcd);
        
        scanvases.append(TCanvas("cu_cname" + histname, histname + "(cu)", 1200, 900))
        scanvases[-1].cd().SetRightMargin(0.04);
        
        cu_hist_data.Draw("error");
        cu_hs.Draw("hist same");
###        cu_hist_Zprime500.Draw("same");
###        cu_hist_Zprime750.Draw("same");
###        cu_hist_Zprime1000.Draw("same");
###        cu_hist_Zprime1250.Draw("same");
#        cu_hist_Zprime4000.Draw("same");
##        #        cu_hist_data2.Draw("error same");
        cu_hist_data.Draw("error same");
        leg.Draw();
##
        text1.Draw()
        postfix = ''
        if setLogY:
            scanvases[-1].SetLogy(1)
            postfix = '_log'
#        scanvases[-1].SetGridy(1)       
        name = ''.join(histname[:histname.rfind('/') + 1]) + prefix + ''.join(histname[histname.rfind('/') + 1:])
        saveAs(canvas=scanvases[-1], name=name + '_integrated' + postfix, outputFormats=outputFormats, outputFolder=savePath)
        del scanvases[-1]
        del cu_hs
        del cu_hist_data
        del cu_hist_ttbar
        del cu_hist_wjets
        del cu_hist_zjets
        del cu_hist_qcd
        del cu_hist_singleTop
        del cu_hist_diboson


# fixOverlay: Redraws the axis

def fixOverlay():
    gPad.RedrawAxis();


def getCumulativePlot(initial, type):
    name = initial.GetName()
    name = "cu_" + name + "_" + type;
    title = initial.GetTitle()
    title = "cu_" + title + "_" + type;
    xaxis = initial.GetXaxis().GetTitle();
    yaxis = initial.GetYaxis().GetTitle();
    nBins = initial.GetNbinsX();
    cu = TH1F(name, title, nBins, initial.GetXaxis().GetXmin(), initial.GetXaxis().GetXmax());
    for bin in range(1, nBins + 1):
        cu.SetBinContent(bin, initial.Integral(bin, nBins));
    
    cu.SetFillStyle(initial.GetFillStyle());
    cu.SetFillColor(initial.GetFillColor());
    cu.SetLineColor(initial.GetLineColor());
    cu.SetMarkerSize(initial.GetMarkerSize());
    cu.SetMarkerStyle(initial.GetMarkerStyle());
    cu.SetMarkerColor(initial.GetMarkerColor());
    cu.SetLineWidth(initial.GetLineWidth());
    cu.GetXaxis().SetTitle(xaxis);
    cu.GetYaxis().SetTitle(yaxis);
    return cu;

#def add_cms_label(lumi, njet="4orMoreJets", nbjet="0orMoreBtag"):
#    jetBinsLatex = {'0jet':'0 jet', '0orMoreJets':'#geq 0 jets', '1jet':'1 jet', '1orMoreJets':'#geq 1 jet',
#                    '2jets':'2 jets', '2orMoreJets':'#geq 2 jets', '3jets':'3 jets', '3orMoreJets':'#geq 3 jets',
#                    '4orMoreJets':'#geq 4 jets'}
#    
#    BjetBinsLatex = {'0btag':'0 b-tags', '0orMoreBtag':'#geq 0 b-tags', '1btag':'1 b-tags',
#                    '1orMoreBtag':'#geq 1 b-tags',
#                    '2btags':'2 b-tags', '2orMoreBtags':'#geq 2 b-tags',
#                    '3btags':'3 b-tags', '3orMoreBtags':'#geq 3 b-tags',
#                    '4orMoreBtags':'#geq 4 b-tags'}
#
#    mytext = TPaveText(0.35, 0.8, 0.6, 0.93, "NDC");
#    mytext.AddText("CMS Preliminary");
#    mytext.AddText("%.1f pb^{-1} at  #sqrt{s} = 7 TeV" % lumi);
#    if njet != "":
#        mytext.AddText("e, %s, %s" % (jetBinsLatex[njet], BjetBinsLatex[nbjet]))   
#             
#    mytext.SetFillStyle(0);
#    mytext.SetBorderSize(0);
#    mytext.SetTextFont(42);
#    mytext.SetTextAlign(13);
#    return mytext

def getJetBin(histname):
    jetBins = ["0orMoreJets",
        "1orMoreJets",
        "2orMoreJets",
        "3orMoreJets" , "4orMoreJets"]
    for bin in jetBins:
        if bin in histname:
            return bin
    #default
    return '4orMoreJets'

def getBJetBin(histname):
    bjetBins = ["0orMoreBtag",
        "1orMoreBtag",
        "2orMoreBtags",
        "3orMoreBtags", '4orMoreBtags' ]
    for bin in bjetBins:
        if bin in histname:
            return bin
    #default
    return '0orMoreBtag'



def rescaleMC(data, ttbar, wjets, qcd):
    #rescaling
    
    
    chi2 = {}
    
    eventsData = []
    appendData = eventsData.append
    eventsTTbar = []
    appendTTbar = eventsTTbar.append
    eventsWJets = []
    appendWJets = eventsWJets.append
    eventsQCD = []
    appendQCD = eventsQCD.append
    nBins = data.GetNbinsX()
    
    getData = data.GetBinContent
    getttbar = ttbar.GetBinContent
    getwjets = wjets.GetBinContent
    getqcd = qcd.GetBinContent
    for bin in range(nBins):        
        appendData(getData(bin))
        appendTTbar(getttbar(bin))
        appendWJets(getwjets(bin))
        appendQCD(getqcd(bin))
    
    events = {}
    events['data'] = eventsData
    events['ttbar'] = eventsTTbar
    events['wjets'] = eventsWJets
    events['qcd'] = eventsQCD
    
    for tscale in arange(1, 2, 0.2):
        for wscale in arange(1, 4, 0.4):
            for qscale in arange(1, 4, 0.4):
                res = getChi2(events, {'ttbar': tscale, 'wjets':wscale, 'qcd':qscale})
                chi2[res] = {'ttbar': tscale, 'wjets':wscale, 'qcd':qscale}
    keys = chi2.keys()
    result = {'ttbar': 1, 'wjets':1, 'qcd':1}
    if len(keys) > 0:
        result = sorted(keys)[0]
    return result, chi2[result]
        
            
def getChi2(events={}, scales={}):
    diff = 0
    for eventsData, eventsTTbar, eventsWJets, eventsQCD  in zip(events['data'], events['ttbar'], events['wjets'], events['qcd']):        
        eventsMC = eventsTTbar * scales['ttbar'] + eventsWJets * scales['wjets'] + eventsQCD * scales['qcd']
        diff += (eventsData - eventsMC) ** 2
    return diff / len(events['data'])

if __name__ == "__main__":
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 1001;')
    parser = OptionParser()
    parser.add_option("-l", "--logY",
                  action="store_true", dest="logY", default=False,
                  help="enable logarithmic y-axis")

    (options, args) = parser.parse_args()
    setLogY = options.logY
    plotMttbar()
