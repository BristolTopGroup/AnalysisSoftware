#include "TStyle.h"
#include "THStack.h"
#include <vector>
#include "TFile.h"
#include "TLegend.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLatex.h"
#include "TGraphAsymmErrors.h"

using namespace std;
void setTDRStyle();
TH1F* getCumulativePlot(TH1F* initial, TString type);

void makeNotePlots() {
    setTDRStyle();
    gStyle->SetHatchesSpacing(1.0);
    double lumi = 19.;//3.06 + 1.37 + 2.53 + 0.26;
    double oldLumi = 19.;
    double scale = lumi / oldLumi;
    double qcdScale = 1;
    //    TFile* data = TFile::Open("data2.9pb_fullSetOfVars.root");
    TFile* data = TFile::Open("data_19pb_allPF.root");
//    TFile* data2 = TFile::Open("data_4.4pb_withoutHLT.root");
    TFile* ttbar = TFile::Open("ttjet_19pb.root");
    TFile* wjets = TFile::Open("wj_19pb.root");
    TFile* zjets = TFile::Open("zj_19pb.root");
//    TFile* bce1 = TFile::Open("bce1_19pb.root");
    TFile* bce2 = TFile::Open("bce2_19pb.root");
    TFile* bce3 = TFile::Open("bce3_19pb.root");
    TFile* enri1 = TFile::Open("enri1_19pb.root");
    TFile* enri2 = TFile::Open("enri2_19pb.root");
    TFile* enri3 = TFile::Open("enri3_19pb.root");
    TFile* pj1 = TFile::Open("pj1_19pb.root");
    TFile* pj2 = TFile::Open("pj2_19pb.root");
    TFile* pj3 = TFile::Open("pj3_19pb.root");
//    TFile* vqq = TFile::Open("vqq_7.22pb_V4PFCalo.root__fullSetOfVars.root");
//    TFile* Zprime500 = TFile::Open("Zprime_M500GeV_W5GeV_19pb.root");
//    TFile* Zprime750 = TFile::Open("Zprime_M750GeV_W7500MeV_19pb.root");
//    TFile* Zprime1000 = TFile::Open("Zprime_M1TeV_W10GeV_19pb.root");
//    TFile* Zprime1250 = TFile::Open("Zprime_M1250GeV_W12500MeV_19pb.root");
//    TFile* Zprime1500 = TFile::Open("Zprime_M1500GeV_W15GeV_19pb.root");

    vector<TString> hists;
    hists.push_back("DRmin_QCDenriched");
                hists.push_back("ptRel_QCDenriched");
                hists.push_back("ptRel_WZenriched");
                hists.push_back("DRmin_WZenriched");
//                hists.push_back("HT");
//                hists.push_back("numberOfJets");
//                hists.push_back("numberOfBJets");
//                hists.push_back("MET");
//                hists.push_back("leadingJetMass");
//                hists.push_back("mtW");
//                hists.push_back("electronD0");
//                hists.push_back("neutrino_pz");
//    TString folder("");

    for (unsigned int hist = 0; hist < hists.size(); ++hist) {
        TString histname(hists.at(hist));
        TString histpath(histname);
        TH1F* hist_data = (TH1F*) data->Get(histpath);

//        TH1F* hist_data2;
//        if (histname == "mttbar_rebinned")
//            hist_data2 = (TH1F*) data2->Get(histpath);
//        else
//            hist_data2 = new TH1F(*hist_data);
        hist_data->Sumw2();
//        hist_data2->Sumw2();
        TH1F* hist_ttbar = (TH1F*) ttbar->Get(histpath);
        TH1F* hist_wjets = (TH1F*) wjets->Get(histpath);
        TH1F* hist_zjets = (TH1F*) zjets->Get(histpath);

//        TH1F* hist_bce1 = (TH1F*) bce1->Get(histpath);
        TH1F* hist_bce2 = (TH1F*) bce2->Get(histpath);
        TH1F* hist_bce3 = (TH1F*) bce3->Get(histpath);
        TH1F* hist_enri1 = (TH1F*) enri1->Get(histpath);
        TH1F* hist_enri2 = (TH1F*) enri2->Get(histpath);
        TH1F* hist_enri3 = (TH1F*) enri3->Get(histpath);
        TH1F* hist_pj1 = (TH1F*) pj1->Get(histpath);
        TH1F* hist_pj2 = (TH1F*) pj2->Get(histpath);
        TH1F* hist_pj3 = (TH1F*) pj3->Get(histpath);

//        TH1F* hist_Zprime500 = (TH1F*) Zprime500->Get(histpath);
//        TH1F* hist_Zprime750 = (TH1F*) Zprime750->Get(histpath);
//        TH1F* hist_Zprime1000 = (TH1F*) Zprime1000->Get(histpath);
//        TH1F* hist_Zprime1250 = (TH1F*) Zprime1250->Get(histpath);
//        TH1F* hist_Zprime1500 = (TH1F*) Zprime1500->Get(histpath);

        //        hist_ttbar->Scale(157.5 * lumi / 1468404.);
        //        hist_wjets->Scale(31314 * lumi / 5143895.);
        //        hist_zjets->Scale(3048 * lumi / 1084921.);

        //        cout << hist_ttbar->Integral() << endl;
        hist_ttbar->Scale(scale);
        //        cout << hist_ttbar->Integral() << endl;
        hist_wjets->Scale(scale/2);
        hist_zjets->Scale(scale);

        //        hist_bce1->Scale(0.2355e9 * 0.00046 * lumi / 2606023.);
        //        hist_bce2->Scale(0.0593e9 * 0.00234 * lumi / 2470597.);
        //        hist_bce3->Scale(0.906e6 * 0.0104 * lumi / 998674.);
        //
        //        hist_enri1->Scale(0.2355e9 * 0.0073 * lumi / 14607326.);
        //        hist_enri2->Scale(0.0593e9 * 0.059 * lumi / 27403259.);
        //        hist_enri3->Scale(0.906e6 * 0.148 * lumi / 4686413.);

//        hist_bce1->Scale(scale);
        hist_bce2->Scale(scale);
        hist_bce3->Scale(scale);

        hist_enri1->Scale(scale);
        hist_enri2->Scale(scale);
        hist_enri3->Scale(scale);

        hist_pj1->Scale(scale);
        hist_pj2->Scale(scale);
        hist_pj3->Scale(scale);

        //        hist_pj1->Scale(23620. * lumi / 2255228.);
        //        hist_pj2->Scale(3476. * lumi / 1071393.);
        //        hist_pj3->Scale(485. * lumi / 960198.);

        //        hist_Zprime500->Scale(50 * lumi / 227266);
        //        hist_Zprime750->Scale(50 * lumi / 182024);
        //        hist_Zprime1000->Scale(50 * lumi / 200759);
        //        hist_Zprime1250->Scale(50 * lumi / 298428);
        //        hist_Zprime1500->Scale(50 * lumi / 299024);

//        hist_Zprime500->Scale(scale);
//        hist_Zprime750->Scale(scale);
//        hist_Zprime1000->Scale(scale);
//        hist_Zprime1250->Scale(scale);
//        hist_Zprime1500->Scale(scale);

        TH1F* hist_qcd = new TH1F(*hist_bce2);
//        hist_qcd->Add(hist_bce2);
        hist_qcd->Add(hist_bce3);
        hist_qcd->Add(hist_enri1);
        hist_qcd->Add(hist_enri2);
        hist_qcd->Add(hist_enri3);
        hist_qcd->Add(hist_pj1);
        hist_qcd->Add(hist_pj2);
        hist_qcd->Add(hist_pj3);
        hist_qcd->Scale(qcdScale);
        //        double ndata = hist_data->Integral();
        //        double ntop = hist_ttbar->Integral();
        //        double nwj = hist_wjets->Integral();
        //        double nzj = hist_zjets->Integral();
        //        double nqcd = hist_qcd->Integral();
        //        double sumMC = ntop + nwj + nzj + nqcd;
        //        cout << ndata << " " << sumMC << endl;
        //                        hist_wjets->Scale(ndata / sumMC);
        //                        hist_ttbar->Scale(ndata / sumMC);
        //                        hist_zjets->Scale(ndata / sumMC);
        //                        hist_qcd->Scale(ndata / sumMC);
        TH1F* hist_mc = new TH1F(*hist_qcd);
        hist_mc->Add(hist_ttbar);
        hist_mc->Add(hist_zjets);
        hist_mc->Add(hist_wjets);

        unsigned int rebin = 1;
        if (histname == "mttbar") {
            hist_data->SetXTitle("M_{t#bar{t}}/GeV");
            hist_data->SetYTitle("Events/(50 GeV)");
            rebin = 50;
        } else if (histname == "m3") {
            hist_data->SetXTitle("M3/GeV");
            hist_data->SetYTitle("Events/(100 GeV)");
            rebin = 100;
        } else if (histname == "electron_et") {
            hist_data->SetXTitle("electron p_{T}/GeV");
            hist_data->SetYTitle("Events/(5 GeV)");
            rebin = 5;
        } else if (histname == "ttbar_pt") {
            hist_data->SetXTitle("p_{T} of t#bar{t} system/GeV");
            hist_data->SetYTitle("Events/(5 GeV)");
            rebin = 5;
        } else if (histname == "HT") {
            hist_data->SetXTitle("#Sigma p_{T}/GeV");
            hist_data->SetYTitle("Events/(50 GeV)");
            rebin = 50;
        } else if (histname == "numberOfJets") {
            hist_data->SetXTitle("number of jets");
            hist_data->SetYTitle("Events");
        } else if (histname == "numberOfBJets") {
            hist_data->SetXTitle("number of b-tagged jets (SSVHE medium)");
            hist_data->SetYTitle("Events");
        } else if (histname == "MET") {
            hist_data->SetXTitle("MET/GeV");
            hist_data->SetYTitle("Events/(10 GeV)");
            rebin = 10;
        } else if (histname == "leadingJetMass") {
            hist_data->SetXTitle("leading jet mass/GeV");
            hist_data->SetYTitle("Events/(4 GeV)");
            rebin = 4;
        } else if (histname == "mtW") {
            hist_data->SetXTitle("transverse W-boson mass/GeV");
            hist_data->SetYTitle("Events/(10 GeV)");
            rebin = 10;
        } else if (histname == "electronD0") {
            hist_data->SetXTitle("electron d_{0} / cm");
            hist_data->SetYTitle("Events/(0.001 cm)");
            rebin = 10;

        } else if (histname == "neutrino_pz") {
            hist_data->SetXTitle("neutrino p_{Z} /GeV");
            hist_data->SetYTitle("Events/(10 GeV)");
            rebin = 10;
        }
        else if(histname == "ptRel_QCDenriched" || histname == "ptRel_WZenriched"){
            hist_data->SetXTitle("relative p_{T} (electron, closest jet) /GeV");
            hist_data->SetYTitle("Events/(1 GeV)");
            rebin = 1;
        }

        else if (histname == "DRmin_QCDenriched" || histname == "DRmin_WZenriched") {
            hist_data->SetXTitle("#DeltaR (electron, closest jet)");
            hist_data->SetYTitle("Events");
            rebin = 1;
        }

        hist_data->Rebin(rebin);
        hist_ttbar->Rebin(rebin);
        hist_wjets->Rebin(rebin);
        hist_zjets->Rebin(rebin);
        hist_qcd->Rebin(rebin);
//        hist_Zprime500->Rebin(rebin);
//        hist_Zprime750->Rebin(rebin);
//        hist_Zprime1000->Rebin(rebin);
//        hist_Zprime1250->Rebin(rebin);
//        hist_Zprime1500->Rebin(rebin);

        hist_data->SetMarkerStyle(8);
//        hist_data2->SetMarkerStyle(8);
//        hist_data2->SetMarkerSize(1.5);
//        hist_data2->SetMarkerColor(kAzure + 1);
//        hist_data2->SetLineColor(kAzure + 1);
        hist_ttbar->SetFillStyle(1001);
        hist_ttbar->SetFillColor(kRed + 1);
        hist_wjets->SetFillStyle(1001);
        hist_wjets->SetFillColor(kGreen - 3);
        hist_zjets->SetFillStyle(1001);
        hist_zjets->SetFillColor(kAzure - 2);
        hist_qcd->SetFillStyle(1001);
        hist_qcd->SetFillColor(kYellow);

        const int nbins = hist_qcd->GetXaxis()->GetNbins();
        float binwidth = (hist_qcd->GetXaxis()->GetXmax() - hist_qcd->GetXaxis()->GetXmin()) / nbins;
        for (int i = 1; i <= nbins; i++) {
            float yvalue = hist_qcd->GetBinContent(i);
            //            float xvalue = hist_qcd->GetBinCenter(i);
            //            float ymin = yvalue - yvalue*0.5;
            //            float ymax = yvalue + yvalue*0.5;
            //            float xmin = xvalue - 0.5*binwidth;
            //            float xmax = xvalue + 0.5*binwidth;
            float error = yvalue * 0.5;
            hist_mc->SetBinError(i, error);
            //            qcdUncert->SetPointError(i, xmin, xmax, ymin, ymax);
        }
        TGraphAsymmErrors* qcdUncert = new TGraphAsymmErrors(hist_mc);

//        hist_Zprime500->SetLineColor(kTeal - 9);
//        hist_Zprime500->SetLineWidth(3);
//        hist_Zprime500->SetFillStyle(0);
//        hist_Zprime500->SetFillColor(kWhite);
//
//        hist_Zprime750->SetLineColor(kBlue - 6);
//        hist_Zprime750->SetLineWidth(3);
//        hist_Zprime750->SetFillStyle(0);
//        hist_Zprime750->SetFillColor(kWhite);
//
//        hist_Zprime1000->SetLineColor(28);
//        hist_Zprime1000->SetLineWidth(3);
//        hist_Zprime1000->SetFillStyle(0);
//        hist_Zprime1000->SetFillColor(kWhite);
//
//        hist_Zprime1250->SetLineColor(kCyan - 5);
//        hist_Zprime1250->SetLineWidth(3);
//        hist_Zprime1250->SetFillStyle(0);
//        hist_Zprime1250->SetFillColor(kWhite);
//
//        hist_Zprime1500->SetLineColor(kOrange + 1);
//        hist_Zprime1500->SetLineWidth(3);
//        hist_Zprime1500->SetFillStyle(0);
//        hist_Zprime1500->SetFillColor(kWhite);

        qcdUncert->SetFillColor(kGray + 3);
        qcdUncert->SetFillStyle(3003);

        TLegend* leg = new TLegend(0.696, 0.35, 0.94, 0.92);
        leg->SetBorderSize(0);
        leg->SetLineStyle(0);
        leg->SetTextFont(42);
        leg->SetFillStyle(0);

        leg->AddEntry(hist_data, "data", "P");
        //        leg->AddEntry(hist_data2, "data(no HLT)", "P");
//        leg->AddEntry(hist_ttbar, "t#bar{t}", "f");
//        leg->AddEntry(hist_wjets, "W#rightarrowl#nu", "f");
//        leg->AddEntry(hist_zjets, "Z/#gamma*#rightarrowl^{+}l^{-}", "f");
//        leg->AddEntry(hist_qcd, "QCD/#gamma + jets");
//        leg->AddEntry(hist_Zprime500, "Z' 0.5TeV (50pb)");
//        leg->AddEntry(hist_Zprime750, "Z' 0.75TeV (50pb)");
//        leg->AddEntry(hist_Zprime1000, "Z' 1TeV (50pb)");
//        leg->AddEntry(hist_Zprime1250, "Z' 1.25TeV (50pb)");
//        leg->AddEntry(hist_Zprime1500, "Z' 1.5TeV (50pb)");

        THStack* hs = new THStack("MC", "MC");
        hs->Add(hist_qcd);
        hs->Add(hist_zjets);
        hs->Add(hist_wjets);
        hs->Add(hist_ttbar);

        TCanvas* c = new TCanvas(TString("cname" + histname), histname, 800, 600);
        c->cd()->SetRightMargin(0.04);
        //        hist_data2->Draw("error");
        hist_data->Draw("error");
//        hs->Draw("hist same");
//        hist_Zprime500->Draw("same");
//        hist_Zprime750->Draw("same");
//        hist_Zprime1000->Draw("same");
//        hist_Zprime1250->Draw("same");
//        hist_Zprime1500->Draw("same");
        //        qcdUncert->Draw("1 same");
        //        hist_data2->Draw("error same");
        hist_data->Draw("error same");
        leg->Draw();

        //        text1 = new TLatex(3.570061, 23.08044, "CMS Preliminary");
        //        text1->SetNDC();
        //        text1->SetTextAlign(13);
        //        text1->SetX(0.184);
        //        text1->SetY(0.928);
        //        //text1->SetLineWidth(2);
        //        text1->SetTextFont(42);
        //        text1->SetTextSizePixels(24);// dflt=28
        //        text1->Draw();

        TLatex * text2 = new TLatex(3.570061, 23.08044, "~19 pb^{-1} at #sqrt{s} = 7 TeV");
        text2->SetNDC();
        text2->SetTextAlign(13);
        text2->SetX(0.184);
        text2->SetY(0.88);
        //text2->SetLineWidth(2);
        text2->SetTextFont(42);
        text2->SetTextSizePixels(24);// dflt=28
        text2->Draw();

//        TH1F* cu_hist_data = getCumulativePlot(hist_data, "data");
////        TH1F* cu_hist_data2 = getCumulativePlot(hist_data2, "data2");
//        TH1F* cu_hist_ttbar = getCumulativePlot(hist_ttbar, "ttbar");
//        TH1F* cu_hist_wjets = getCumulativePlot(hist_wjets, "wjets");
//        TH1F* cu_hist_zjets = getCumulativePlot(hist_zjets, "zjets");
//        TH1F* cu_hist_qcd = getCumulativePlot(hist_qcd, "qcd");
//        TH1F* cu_hist_Zprime500 = getCumulativePlot(hist_Zprime500, "Zprime500");
//        TH1F* cu_hist_Zprime750 = getCumulativePlot(hist_Zprime750, "Zprime750");
//        TH1F* cu_hist_Zprime1000 = getCumulativePlot(hist_Zprime1000, "Zprime1000");
//        TH1F* cu_hist_Zprime1250 = getCumulativePlot(hist_Zprime1250, "Zprime1250");
//        TH1F* cu_hist_Zprime1500 = getCumulativePlot(hist_Zprime1500, "Zprime1500");
////        if (histname == "mttbar_rebinned") {
////            cu_hist_data2->SetXTitle("M_{t#bar{t}}/GeV");
////            cu_hist_data2->SetYTitle("Integrated Events/(50 GeV)");
////        }
//
//        THStack* cu_hs = new THStack("cu_MC", "cu_MC");
//        cu_hs->Add(cu_hist_qcd);
//        cu_hs->Add(cu_hist_zjets);
//        cu_hs->Add(cu_hist_wjets);
//        cu_hs->Add(cu_hist_ttbar);
//
//        TCanvas* cu_c = new TCanvas(TString("cu_cname" + histname), TString(histname + "(cu)"), 800, 600);
//        cu_c->cd()->SetRightMargin(0.04);
//        //        cu_hist_data2->Draw("error");
//        cu_hist_data->Draw("error");
//        cu_hs->Draw("hist same");
//        cu_hist_Zprime500->Draw("same");
//        cu_hist_Zprime750->Draw("same");
//        cu_hist_Zprime1000->Draw("same");
//        cu_hist_Zprime1250->Draw("same");
//        cu_hist_Zprime1500->Draw("same");
//        //        cu_hist_data2->Draw("error same");
//        cu_hist_data->Draw("error same");
//        leg->Draw();
//
//        //        text1->Draw();
//
//        text2->Draw();
    }
}

// tdrGrid: Turns the grid lines on (true) or off (false)

//void tdrGrid(bool gridOn) {
//    tdrStyle->SetPadGridX(gridOn);
//    tdrStyle->SetPadGridY(gridOn);
//}

// fixOverlay: Redraws the axis

void fixOverlay() {
    gPad->RedrawAxis();
}

void setTDRStyle() {
    TStyle *tdrStyle = new TStyle("tdrStyle", "Style for P-TDR");

    // For the canvas:
    tdrStyle->SetCanvasBorderMode(0);
    tdrStyle->SetCanvasColor(kWhite);
    tdrStyle->SetCanvasDefH(600); //Height of canvas
    tdrStyle->SetCanvasDefW(600); //Width of canvas
    tdrStyle->SetCanvasDefX(0); //POsition on screen
    tdrStyle->SetCanvasDefY(0);

    // For the Pad:
    tdrStyle->SetPadBorderMode(0);
    // tdrStyle->SetPadBorderSize(Width_t size = 1);
    tdrStyle->SetPadColor(kWhite);
    tdrStyle->SetPadGridX(false);
    tdrStyle->SetPadGridY(false);
    tdrStyle->SetGridColor(0);
    tdrStyle->SetGridStyle(3);
    tdrStyle->SetGridWidth(1);

    // For the frame:
    tdrStyle->SetFrameBorderMode(0);
    tdrStyle->SetFrameBorderSize(1);
    tdrStyle->SetFrameFillColor(0);
    tdrStyle->SetFrameFillStyle(0);
    tdrStyle->SetFrameLineColor(1);
    tdrStyle->SetFrameLineStyle(1);
    tdrStyle->SetFrameLineWidth(1);

    // For the histo:
    // tdrStyle->SetHistFillColor(1);
    // tdrStyle->SetHistFillStyle(0);
    tdrStyle->SetHistLineColor(1);
    tdrStyle->SetHistLineStyle(0);
    tdrStyle->SetHistLineWidth(1);
    // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
    // tdrStyle->SetNumberContours(Int_t number = 20);

    tdrStyle->SetEndErrorSize(2);
    //  tdrStyle->SetErrorMarker(20);
    tdrStyle->SetErrorX(0.);

    tdrStyle->SetMarkerStyle(20);

    //For the fit/function:
    tdrStyle->SetOptFit(1);
    tdrStyle->SetFitFormat("5.4g");
    tdrStyle->SetFuncColor(2);
    tdrStyle->SetFuncStyle(1);
    tdrStyle->SetFuncWidth(1);

    //For the date:
    tdrStyle->SetOptDate(0);
    // tdrStyle->SetDateX(Float_t x = 0.01);
    // tdrStyle->SetDateY(Float_t y = 0.01);

    // For the statistics box:
    tdrStyle->SetOptFile(0);
    tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
    tdrStyle->SetStatColor(kWhite);
    tdrStyle->SetStatFont(42);
    tdrStyle->SetStatFontSize(0.025);
    tdrStyle->SetStatTextColor(1);
    tdrStyle->SetStatFormat("6.4g");
    tdrStyle->SetStatBorderSize(1);
    tdrStyle->SetStatH(0.1);
    tdrStyle->SetStatW(0.15);
    // tdrStyle->SetStatStyle(Style_t style = 1001);
    // tdrStyle->SetStatX(Float_t x = 0);
    // tdrStyle->SetStatY(Float_t y = 0);

    // Margins:
    tdrStyle->SetPadTopMargin(0.05);
    tdrStyle->SetPadBottomMargin(0.13);
    tdrStyle->SetPadLeftMargin(0.16);
    tdrStyle->SetPadRightMargin(0.02);

    // For the Global title:

    tdrStyle->SetOptTitle(0);
    tdrStyle->SetTitleFont(42);
    tdrStyle->SetTitleColor(1);
    tdrStyle->SetTitleTextColor(1);
    tdrStyle->SetTitleFillColor(10);
    tdrStyle->SetTitleFontSize(0.05);
    // tdrStyle->SetTitleH(0); // Set the height of the title box
    // tdrStyle->SetTitleW(0); // Set the width of the title box
    // tdrStyle->SetTitleX(0); // Set the position of the title box
    // tdrStyle->SetTitleY(0.985); // Set the position of the title box
    // tdrStyle->SetTitleStyle(Style_t style = 1001);
    // tdrStyle->SetTitleBorderSize(2);

    // For the axis titles:

    tdrStyle->SetTitleColor(1, "XYZ");
    tdrStyle->SetTitleFont(42, "XYZ");
    tdrStyle->SetTitleSize(0.06, "XYZ");
    // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
    // tdrStyle->SetTitleYSize(Float_t size = 0.02);
    tdrStyle->SetTitleXOffset(0.9);
    tdrStyle->SetTitleYOffset(1.25);
    // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

    // For the axis labels:

    tdrStyle->SetLabelColor(1, "XYZ");
    tdrStyle->SetLabelFont(42, "XYZ");
    tdrStyle->SetLabelOffset(0.007, "XYZ");
    tdrStyle->SetLabelSize(0.05, "XYZ");

    // For the axis:

    tdrStyle->SetAxisColor(1, "XYZ");
    tdrStyle->SetStripDecimals(kTRUE);
    tdrStyle->SetTickLength(0.03, "XYZ");
    tdrStyle->SetNdivisions(510, "XYZ");
    tdrStyle->SetPadTickX(1); // To get tick marks on the opposite side of the frame
    tdrStyle->SetPadTickY(1);

    // Change for log plots:
    tdrStyle->SetOptLogx(0);
    tdrStyle->SetOptLogy(0);
    tdrStyle->SetOptLogz(0);

    // Postscript options:
    tdrStyle->SetPaperSize(20., 20.);
    // tdrStyle->SetLineScalePS(Float_t scale = 3);
    // tdrStyle->SetLineStyleString(Int_t i, const char* text);
    // tdrStyle->SetHeaderPS(const char* header);
    // tdrStyle->SetTitlePS(const char* pstitle);

    // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
    // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
    // tdrStyle->SetPaintTextFormat(const char* format = "g");
    // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
    // tdrStyle->SetTimeOffset(Double_t toffset);
    // tdrStyle->SetHistMinimumZero(kTRUE);

    tdrStyle->cd();

}

TH1F* getCumulativePlot(TH1F* initial, TString type) {
    TString name(initial->GetName());
    name = "cu_" + name + "_" + type;
    TString title(initial->GetTitle());
    title = "cu_" + title + "_" + type;
    TString xaxis = initial->GetXaxis()->GetTitle();
    TString yaxis = initial->GetYaxis()->GetTitle();
    unsigned int nBins = initial->GetNbinsX();
    TH1F* cu = new TH1F(name, title, nBins, initial->GetXaxis()->GetXmin(), initial->GetXaxis()->GetXmax());
    for (unsigned int bin = 1; bin <= nBins; ++bin) {
        cu->SetBinContent(bin, initial->Integral(bin, nBins));
    }
    cu->SetFillStyle(initial->GetFillStyle());
    cu->SetFillColor(initial->GetFillColor());
    cu->SetLineColor(initial->GetLineColor());
    cu->SetMarkerSize(initial->GetMarkerSize());
    cu->SetMarkerStyle(initial->GetMarkerStyle());
    cu->SetMarkerColor(initial->GetMarkerColor());
    cu->SetLineWidth(initial->GetLineWidth());
    cu->GetXaxis()->SetTitle(xaxis);
    cu->GetYaxis()->SetTitle(yaxis);
    return cu;
}

