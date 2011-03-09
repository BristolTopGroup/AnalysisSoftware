//
// 21 Jul 2010
// 
// Fit RelIso distribution in data, in various Njet bins
//
//  Try: (1) linear fit: 0.1-1.6, 0.2-1.6, 0.3-1.6
//       (2) expo fit:   0.1-1.6, 0.2-1.6, 0.3-1.6
//       (3) gaus fit:   0.1-1.6, 0.2-1.6, 0.3-1.6 (error propagation not yet implemented!)
//
// - overlay with MC
// - update lumi to 11.4 (rounded from 11.39)
// - add fit range 0.3-1.6
// - adapt to run on exclusive jet bin
// 
// configurable options: (A) which njet to use
//                       (B) how to normalize MC
//
// (A) nj: "all", "0jet", "1jet", "2jets", "3jets", "4orMoreJets",
//         "1mj", 2mj", "3mj"
//
// (B) normMode: normalization of MC
//     1: scale to int lumi
//     2: scale to n(Data)
//     3: scale to int lumi, with QCD scaled to the QCD estimate (put in by hand).
//
// eg: root -q -n 'fit_data_Njet.C("all",1)' //to fit all and norm MC to int lumi
//
// funcs: linear/pol0: y = a + bx
//        expo       : y = exp(a+bx) d
//        gaus       : y = a*exp( -0.5*((x-b)/c)^2) ) [see Chapter 5 "Fitting Histograms" of ROOT User Guide)
//------------------------------------------------------

#include "tdrstyle.C";
#include "add_text.C"
#include "colour_scheme.h"
#include <iomanip>

TFile* fdata;
TFile* fttbar;
TFile* fwjets;
TFile* fzjets;
TFile* fbce2;
TFile* fbce3;
TFile* fenri1;
TFile* fenri2;
TFile* fenri3;
TFile* fpj1;
TFile* fpj2;
TFile* fpj3;
TFile* fsTopTW;
TFile* fsToptC;

TH1F *data;
TH1F *mc;
TH1F *tt;
TH1F *wj;
TH1F *zj;
TH1F *QCD;
TH1F *stop;
TH1F *pj;
THStack *mcStack;
THStack *myStack;

// normalize to integrated luminosity (true), or data (false)?
//bool normToLumi = 0; //if false, scale to nData
//bool normQCDToEst = 1; //scale QCD MC to estimate

// How to scale MC
int normMode = 1;

double mcIntlumi = 19.; //pb-1
//double intlumi = 35.09e-3;  //27.30e-3; //pb-1
//double intlumi = 63.68e-3;//35.09e-3;  //27.30e-3; //pb-1
//double intlumi = 78e-3;  //77.48e-3
double intlumi = 26.5;//0.84;  //pb-1

//-----------------------
// which jet bin to fit
//-----------------------
string nj = "all";

double YMAX_all = 4000;
double YMAX_1mj = 2000;
double YMAX_2mj = 400;
double YMAX_3mj = -1;
double YMAX_4orMoreJets = -1;

int numBins;//number of bins in signal region
ofstream TheEstimates;

void fit_data_Njet(string nj_user = "all", int normMode_user = 1) {
    TheEstimates.open("MyEstimates.txt", ios::app);
    nj = nj_user;
    normMode = normMode_user;
    setStyle();
    getHisto();

    if (normMode < 3) {
        //fit_linear(); //bad model
        //fit_expo();
        fit_gaus();
    }
    if (normMode == 3)
        make_QCDnormToEstimate_plot();

    TheEstimates.close();
}

void fit_linear() {
    fit_njet("pol1", 0.1);
    fit_njet("pol1", 0.2);
    fit_njet("pol1", 0.3);
}

void fit_expo() {
    fit_njet("expo", 0.1);
    fit_njet("expo", 0.2);
    fit_njet("expo", 0.3);
}

void fit_gaus() {
    fit_njet("gaus", 0.1);
    fit_njet("gaus", 0.2);
    fit_njet("gaus", 0.3);
}

void setStyle() {
    // Apply TDR style
    setTDRStyle();

    // slight adaptation
    tdrStyle->SetPadRightMargin(0.05); //originally was 0.02, too narrow!
    tdrStyle->SetStatH(0.2);
    //  tdrStyle->SetOptStat(1110);//off title
    tdrStyle->SetOptStat(0);//off title
    tdrStyle->SetOptFit(0);//off title
    tdrStyle->cd();
    //gStyle->SetHatchesSpacing(1);
    //gStyle->SetHatchesLineWidth(1);
    gROOT->ForceStyle();
}

void getHisto() {

    fdata = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/data_26.5pb_PFElectron_PF2PATJets_PFMET.root");
    fttbar = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/ttjet_19pb_PFElectron_PF2PATJets_PFMET.root");
    fwjets = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/wj_19pb_PFElectron_PF2PATJets_PFMET.root");
    fzjets = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/zj_19pb_PFElectron_PF2PATJets_PFMET.root");
    //    fbce1 = TFile::Open("bce1_19pb.root");
    fbce2 = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/bce2_19pb_PFElectron_PF2PATJets_PFMET.root");
    fbce3 = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/bce3_19pb_PFElectron_PF2PATJets_PFMET.root");
    fenri1 = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/enri1_19pb_PFElectron_PF2PATJets_PFMET.root");
    fenri2 = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/enri2_19pb_PFElectron_PF2PATJets_PFMET.root");
    fenri3 = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/enri3_19pb_PFElectron_PF2PATJets_PFMET.root");
    fpj1 = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/pj1_19pb_PFElectron_PF2PATJets_PFMET.root");
    fpj2 = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/pj2_19pb_PFElectron_PF2PATJets_PFMET.root");
    fpj3 = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/pj3_19pb_PFElectron_PF2PATJets_PFMET.root");
    fsTopTW = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/tW_19pb_PFElectron_PF2PATJets_PFMET.root");
    fsToptC = TFile::Open(
            "/storage/workspace/BristolAnalysisTools/outputfiles/backup/tchan_19pb_PFElectron_PF2PATJets_PFMET.root");

    if (nj == "1mj" || nj == "2mj" || nj == "3mj") {
        get_histo_inclusive(nj);
    } else if (nj == "all") {
        //--------------------------------------------------------------------------
        //                                DATA
        //--------------------------------------------------------------------------
        data = (TH1F*) fdata->Get("QCDest_CombRelIso_0jet");
        data->Add((TH1F*) fdata->Get("QCDest_CombRelIso_1jet"));
        data->Add((TH1F*) fdata->Get("QCDest_CombRelIso_2jets"));
        data->Add((TH1F*) fdata->Get("QCDest_CombRelIso_3jets"));
        data->Add((TH1F*) fdata->Get("QCDest_CombRelIso_4orMoreJets"));

        //--------------------------------------------------------------------------
        //                                 MC
        //--------------------------------------------------------------------------
        QCD = (TH1F*) fbce2->Get("QCDest_CombRelIso_0jet");
        QCD->Add((TH1F*) fbce2->Get("QCDest_CombRelIso_1jet"));
        QCD->Add((TH1F*) fbce2->Get("QCDest_CombRelIso_2jets"));
        QCD->Add((TH1F*) fbce2->Get("QCDest_CombRelIso_3jets"));
        QCD->Add((TH1F*) fbce2->Get("QCDest_CombRelIso_4orMoreJets"));

        QCD->Add((TH1F*) fbce3->Get("QCDest_CombRelIso_0jet"));
        QCD->Add((TH1F*) fbce3->Get("QCDest_CombRelIso_1jet"));
        QCD->Add((TH1F*) fbce3->Get("QCDest_CombRelIso_2jets"));
        QCD->Add((TH1F*) fbce3->Get("QCDest_CombRelIso_3jets"));
        QCD->Add((TH1F*) fbce3->Get("QCDest_CombRelIso_4orMoreJets"));

        QCD->Add((TH1F*) fenri1->Get("QCDest_CombRelIso_0jet"));
        QCD->Add((TH1F*) fenri1->Get("QCDest_CombRelIso_1jet"));
        QCD->Add((TH1F*) fenri1->Get("QCDest_CombRelIso_2jets"));
        QCD->Add((TH1F*) fenri1->Get("QCDest_CombRelIso_3jets"));
        QCD->Add((TH1F*) fenri1->Get("QCDest_CombRelIso_4orMoreJets"));

        QCD->Add((TH1F*) fenri2->Get("QCDest_CombRelIso_0jet"));
        QCD->Add((TH1F*) fenri2->Get("QCDest_CombRelIso_1jet"));
        QCD->Add((TH1F*) fenri2->Get("QCDest_CombRelIso_2jets"));
        QCD->Add((TH1F*) fenri2->Get("QCDest_CombRelIso_3jets"));
        QCD->Add((TH1F*) fenri2->Get("QCDest_CombRelIso_4orMoreJets"));

        QCD->Add((TH1F*) fenri3->Get("QCDest_CombRelIso_0jet"));
        QCD->Add((TH1F*) fenri3->Get("QCDest_CombRelIso_1jet"));
        QCD->Add((TH1F*) fenri3->Get("QCDest_CombRelIso_2jets"));
        QCD->Add((TH1F*) fenri3->Get("QCDest_CombRelIso_3jets"));
        QCD->Add((TH1F*) fenri3->Get("QCDest_CombRelIso_4orMoreJets"));

        pj = (TH1F*) fpj1->Get("QCDest_CombRelIso_0jet");
        pj->Add((TH1F*) fpj1->Get("QCDest_CombRelIso_1jet"));
        pj->Add((TH1F*) fpj1->Get("QCDest_CombRelIso_2jets"));
        pj->Add((TH1F*) fpj1->Get("QCDest_CombRelIso_3jets"));
        pj->Add((TH1F*) fpj1->Get("QCDest_CombRelIso_4orMoreJets"));

        pj->Add((TH1F*) fpj2->Get("QCDest_CombRelIso_0jet"));
        pj->Add((TH1F*) fpj2->Get("QCDest_CombRelIso_1jet"));
        pj->Add((TH1F*) fpj2->Get("QCDest_CombRelIso_2jets"));
        pj->Add((TH1F*) fpj2->Get("QCDest_CombRelIso_3jets"));
        pj->Add((TH1F*) fpj2->Get("QCDest_CombRelIso_4orMoreJets"));

        pj->Add((TH1F*) fpj3->Get("QCDest_CombRelIso_0jet"));
        pj->Add((TH1F*) fpj3->Get("QCDest_CombRelIso_1jet"));
        pj->Add((TH1F*) fpj3->Get("QCDest_CombRelIso_2jets"));
        pj->Add((TH1F*) fpj3->Get("QCDest_CombRelIso_3jets"));
        pj->Add((TH1F*) fpj3->Get("QCDest_CombRelIso_4orMoreJets"));

        wj = (TH1F*) fwjets->Get("QCDest_CombRelIso_0jet");
        wj->Add((TH1F*) fwjets->Get("QCDest_CombRelIso_1jet"));
        wj->Add((TH1F*) fwjets->Get("QCDest_CombRelIso_2jets"));
        wj->Add((TH1F*) fwjets->Get("QCDest_CombRelIso_3jets"));
        wj->Add((TH1F*) fwjets->Get("QCDest_CombRelIso_4orMoreJets"));

        zj = (TH1F*) fzjets->Get("QCDest_CombRelIso_0jet");
        zj->Add((TH1F*) fzjets->Get("QCDest_CombRelIso_1jet"));
        zj->Add((TH1F*) fzjets->Get("QCDest_CombRelIso_2jets"));
        zj->Add((TH1F*) fzjets->Get("QCDest_CombRelIso_3jets"));
        zj->Add((TH1F*) fzjets->Get("QCDest_CombRelIso_4orMoreJets"));

        tt = (TH1F*) fttbar->Get("QCDest_CombRelIso_0jet");
        tt->Add((TH1F*) fttbar->Get("QCDest_CombRelIso_1jet"));
        tt->Add((TH1F*) fttbar->Get("QCDest_CombRelIso_2jets"));
        tt->Add((TH1F*) fttbar->Get("QCDest_CombRelIso_3jets"));
        tt->Add((TH1F*) fttbar->Get("QCDest_CombRelIso_4orMoreJets"));

        stop = (TH1F*) fsTopTW->Get("QCDest_CombRelIso_0jet");
        stop->Add((TH1F*) fsTopTW->Get("QCDest_CombRelIso_1jet"));
        stop->Add((TH1F*) fsTopTW->Get("QCDest_CombRelIso_2jets"));
        stop->Add((TH1F*) fsTopTW->Get("QCDest_CombRelIso_3jets"));
        stop->Add((TH1F*) fsTopTW->Get("QCDest_CombRelIso_4orMoreJets"));

        stop->Add((TH1F*) fsToptC->Get("QCDest_CombRelIso_0jet"));
        stop->Add((TH1F*) fsToptC->Get("QCDest_CombRelIso_1jet"));
        stop->Add((TH1F*) fsToptC->Get("QCDest_CombRelIso_2jets"));
        stop->Add((TH1F*) fsToptC->Get("QCDest_CombRelIso_3jets"));
        stop->Add((TH1F*) fsToptC->Get("QCDest_CombRelIso_4orMoreJets"));

        mc = (TH1F*) QCD->Clone("sumMC");
        mc->Add(pj);
        mc->Add(wj);
        mc->Add(zj);
        mc->Add(tt);
        mc->Add(stop);
    } else {

        //--------------------------------------------------------------------------
        //                                DATA
        //--------------------------------------------------------------------------
        data = (TH1F*) fdata->Get(Form("QCDest_CombRelIso_%s", nj.c_str()));

        //--------------------------------------------------------------------------
        //                                 MC
        //--------------------------------------------------------------------------
        //    mc    = (TH1F*)fmc->Get(Form("QCDest_CombRelIso_%s",nj.c_str()));
        QCD = (TH1F*) fbce2->Get(Form("QCDest_CombRelIso_%s", nj.c_str()));
        QCD->Add((TH1F*) fbce3->Get(Form("QCDest_CombRelIso_%s", nj.c_str())));
        QCD->Add((TH1F*) fenri1->Get(Form("QCDest_CombRelIso_%s", nj.c_str())));
        QCD->Add((TH1F*) fenri2->Get(Form("QCDest_CombRelIso_%s", nj.c_str())));
        QCD->Add((TH1F*) fenri3->Get(Form("QCDest_CombRelIso_%s", nj.c_str())));
        pj = (TH1F*) fpj1->Get(Form("QCDest_CombRelIso_%s", nj.c_str()));
        pj->Add((TH1F*) fpj2->Get(Form("QCDest_CombRelIso_%s", nj.c_str())));
        pj->Add((TH1F*) fpj3->Get(Form("QCDest_CombRelIso_%s", nj.c_str())));
        wj = (TH1F*) fwjets->Get(Form("QCDest_CombRelIso_%s", nj.c_str()));
        zj = (TH1F*) fzjets->Get(Form("QCDest_CombRelIso_%s", nj.c_str()));
        tt = (TH1F*) fttbar->Get(Form("QCDest_CombRelIso_%s", nj.c_str()));
        stop = (TH1F*) fsTopTW->Get(Form("QCDest_CombRelIso_%s", nj.c_str()));
        stop->Add((TH1F*) fsToptC->Get(Form("QCDest_CombRelIso_%s", nj.c_str())));
        mc = (TH1F*) QCD->Clone("sumMC");
        mc->Add(pj);
        mc->Add(wj);
        mc->Add(zj);
        mc->Add(tt);
        mc->Add(stop);
    }

    // add photon+jets to QCD
    //QCD->Add(pj);

    //scale QCD based on difference in estimate and simulation (before or after adding photon+jets?)
    //QCD->Scale(1.09);

    // Rebin
    int this_rebin = 10;
    /*
     if(nj=="all") this_rebin = 2;
     if(nj=="1mj") this_rebin = 2;
     if(nj=="2mj") this_rebin = 5;
     if(nj=="3mj") this_rebin = 5;
     if(nj=="4orMoreJets") this_rebin = 5;
     if(nj=="1jet") this_rebin = 2;
     if(nj=="2jets") this_rebin = 5;
     if(nj=="3jets") this_rebin = 5;
     if(nj=="4j") this_rebin = 5;
     */

    data->Rebin(this_rebin);
    mc->Rebin(this_rebin);
    QCD->Rebin(this_rebin);
    wj->Rebin(this_rebin);
    zj->Rebin(this_rebin);
    tt->Rebin(this_rebin);
    pj->Rebin(this_rebin);
    stop->Rebin(this_rebin);

    double sf = intlumi / mcIntlumi;
    cout << "\n\n\n";
    cout << "-----------------------" << endl;
    cout << "data int lumi: " << intlumi << endl;
    cout << "mc int lumi: " << mcIntlumi << endl;
    cout << "scale factor: " << sf << endl;
    cout << "------------------------" << endl;

    // Scale MC to intlumi
    mc ->Scale(sf);
    QCD ->Scale(sf);
    wj ->Scale(sf);
    zj ->Scale(sf);
    pj ->Scale(sf);
    tt ->Scale(sf);
    stop->Scale(sf);

    if (normMode == 1) {
        cout << "Scale MC to measured integrated luminosity" << endl;
    } else if (normMode == 2) {
        cout << "Scale MC so that n(MC) = n(Data) [same area]" << endl;
        //    double nData = data->GetEntries();
        double nData = data->Integral();
        double nMC = mc->Integral();
        cout << "n(Data) = " << nData << endl;
        cout << "n(MC)   = " << nMC << endl;
        double sf2 = nData / nMC;
        cout << "sf n(Data)/n(MC) = " << sf2 << endl;
        // norm MC to Data
        mc ->Scale(sf2);
        QCD ->Scale(sf2);
        wj ->Scale(sf2);
        zj ->Scale(sf2);
        pj ->Scale(sf2);
        tt ->Scale(sf2);
        stop->Scale(sf2);
    }
    cout << "------------------------" << endl;

    if (normMode == 3) {
        cout << "Scale QCD MC to data-driven (average) estimate" << endl;
        if (nj == "all")
            QCD->Scale(4944.44 / QCD->GetBinContent(1));
        if (nj == "0jet")
            QCD->Scale(2131.79 / QCD->GetBinContent(1));
        if (nj == "1jet")
            QCD->Scale(2432.21 / QCD->GetBinContent(1));
        if (nj == "2jets")
            QCD->Scale(416.259 / QCD->GetBinContent(1));
        if (nj == "3jets")
            QCD->Scale(56.0141 / QCD->GetBinContent(1));
        if (nj == "4orMoreJets")
            QCD->Scale(24.1861 / QCD->GetBinContent(1));

    }

    // stack up mc
    mcStack = new THStack("mcStack", "MC stack");
    mcStack->Add(QCD); //bottom to top
    mcStack->Add(pj);
    mcStack->Add(zj);
    mcStack->Add(wj);
    //  mcStack->Add(stop);
    mcStack->Add(tt);

    // set style
    QCD->SetFillColor(Col_qcd);
    wj->SetFillColor(Col_wj);
    zj->SetFillColor(Col_zj);
    pj->SetFillColor(Col_pj);
    tt->SetFillColor(Col_sig);
    stop->SetFillColor(Col_stop);

    QCD->SetFillStyle(Sty_qcd);
    wj->SetFillStyle(Sty_wj);
    zj->SetFillStyle(Sty_zj);
    pj->SetFillStyle(Sty_pj);
    tt->SetFillStyle(Sty_sig);
    stop->SetFillStyle(Sty_stop);

    data->SetXTitle("Relative Isolation");
    data->SetYTitle("Events / 0.1");
    data->SetMarkerStyle(20);
}
//------------------------------------------------------------------------------------


// nj must take one of these values: "1mj", "2mj","3mj"
int get_histo_inclusive(string nj) {

    int ijet;
    if (nj == "1mj")
        ijet = 1;
    else if (nj == "2mj")
        ijet = 2;
    else if (nj == "3mj")
        ijet = 3;
    else
        return -1;

    string ijlabel[4] = { ">=0jet", ">=1jet", ">=2jets", ">=3jets" };

    // 0-jet
    TH1F *data_0jet = (TH1F*) fdata->Get("QCDest_CombRelIso_0jet");
    TH1F * QCD_0jet = (TH1F*) fbce2->Get("QCDest_CombRelIso_0jet");
    QCD_0jet->Add((TH1F*) fbce3->Get("QCDest_CombRelIso_0jet"));
    QCD_0jet->Add((TH1F*) fenri1->Get("QCDest_CombRelIso_0jet"));
    QCD_0jet->Add((TH1F*) fenri2->Get("QCDest_CombRelIso_0jet"));
    QCD_0jet->Add((TH1F*) fenri3->Get("QCDest_CombRelIso_0jet"));
    TH1F * pj_0jet = (TH1F*) fpj1->Get("QCDest_CombRelIso_0jet");
    pj_0jet->Add((TH1F*) fpj2->Get(Form("QCDest_CombRelIso_0jet", nj.c_str())));
    pj_0jet->Add((TH1F*) fpj3->Get(Form("QCDest_CombRelIso_0jet", nj.c_str())));
    TH1F * wj_0jet = (TH1F*) fwjets->Get("QCDest_CombRelIso_0jet");
    TH1F * zj_0jet = (TH1F*) fzjets->Get("QCDest_CombRelIso_0jet");
    TH1F * tt_0jet = (TH1F*) fttbar->Get("QCDest_CombRelIso_0jet");
    TH1F *stop_0jet = (TH1F*) fsTopTW->Get("QCDest_CombRelIso_0jet");
    stop_0jet->Add((TH1F*) fsToptC->Get("QCDest_CombRelIso_0jet"));
    TH1F * mc_0jet = (TH1F*) QCD_0jet->Clone("sumMC_0jet");
    mc_0jet->Add(pj_0jet);
    mc_0jet->Add(wj_0jet);
    mc_0jet->Add(zj_0jet);
    mc_0jet->Add(tt_0jet);
    mc_0jet->Add(stop_0jet);

    // 1-jet
    TH1F *data_1jet = (TH1F*) fdata->Get("QCDest_CombRelIso_1jet");
    TH1F * QCD_1jet = (TH1F*) fbce2->Get("QCDest_CombRelIso_1jet");
    QCD_1jet->Add((TH1F*) fbce3->Get("QCDest_CombRelIso_1jet"));
    QCD_1jet->Add((TH1F*) fenri1->Get("QCDest_CombRelIso_1jet"));
    QCD_1jet->Add((TH1F*) fenri2->Get("QCDest_CombRelIso_1jet"));
    QCD_1jet->Add((TH1F*) fenri3->Get("QCDest_CombRelIso_1jet"));
    TH1F * pj_1jet = (TH1F*) fpj1->Get("QCDest_CombRelIso_1jet");
    pj_1jet->Add((TH1F*) fpj2->Get(Form("QCDest_CombRelIso_1jet", nj.c_str())));
    pj_1jet->Add((TH1F*) fpj3->Get(Form("QCDest_CombRelIso_1jet", nj.c_str())));
    TH1F * wj_1jet = (TH1F*) fwjets->Get("QCDest_CombRelIso_1jet");
    TH1F * zj_1jet = (TH1F*) fzjets->Get("QCDest_CombRelIso_1jet");
    TH1F * tt_1jet = (TH1F*) fttbar->Get("QCDest_CombRelIso_1jet");
    TH1F *stop_1jet = (TH1F*) fsTopTW->Get("QCDest_CombRelIso_1jet");
    stop_1jet->Add((TH1F*) fsToptC->Get("QCDest_CombRelIso_1jet"));
    TH1F * mc_1jet = QCD_1jet->Clone("sumMC_1jet");
    mc_1jet->Add(pj_1jet);
    mc_1jet->Add(wj_1jet);
    mc_1jet->Add(zj_1jet);
    mc_1jet->Add(tt_1jet);
    mc_1jet->Add(stop_1jet);

    // 2-jet
    TH1F *data_2jets = (TH1F*) fdata->Get("QCDest_CombRelIso_2jets");
    TH1F * QCD_2jets = (TH1F*) fbce2->Get("QCDest_CombRelIso_2jets");
    QCD_2jets->Add((TH1F*) fbce3->Get("QCDest_CombRelIso_2jets"));
    QCD_2jets->Add((TH1F*) fenri1->Get("QCDest_CombRelIso_2jets"));
    QCD_2jets->Add((TH1F*) fenri2->Get("QCDest_CombRelIso_2jets"));
    QCD_2jets->Add((TH1F*) fenri3->Get("QCDest_CombRelIso_2jets"));
    TH1F * pj_2jets = (TH1F*) fpj1->Get("QCDest_CombRelIso_2jets");
    pj_2jets->Add((TH1F*) fpj2->Get(Form("QCDest_CombRelIso_2jets", nj.c_str())));
    pj_2jets->Add((TH1F*) fpj3->Get(Form("QCDest_CombRelIso_2jets", nj.c_str())));
    TH1F * wj_2jets = (TH1F*) fwjets->Get("QCDest_CombRelIso_2jets");
    TH1F * zj_2jets = (TH1F*) fzjets->Get("QCDest_CombRelIso_2jets");
    TH1F * tt_2jets = (TH1F*) fttbar->Get("QCDest_CombRelIso_2jets");
    TH1F *stop_2jets = (TH1F*) fsTopTW->Get("QCDest_CombRelIso_2jets");
    stop_2jets->Add((TH1F*) fsToptC->Get("QCDest_CombRelIso_2jets"));
    TH1F * mc_2jets = QCD_2jets->Clone("sumMC_2jets");
    mc_2jets->Add(pj_2jets);
    mc_2jets->Add(wj_2jets);
    mc_2jets->Add(zj_2jets);
    mc_2jets->Add(tt_2jets);
    mc_2jets->Add(stop_2jets);

    // 3-jet
    TH1F *data_3jets = (TH1F*) fdata->Get("QCDest_CombRelIso_3jets");
    TH1F * QCD_3jets = (TH1F*) fbce2->Get("QCDest_CombRelIso_3jets");
    QCD_3jets->Add((TH1F*) fbce3->Get("QCDest_CombRelIso_3jets"));
    QCD_3jets->Add((TH1F*) fenri1->Get("QCDest_CombRelIso_3jets"));
    QCD_3jets->Add((TH1F*) fenri2->Get("QCDest_CombRelIso_3jets"));
    QCD_3jets->Add((TH1F*) fenri3->Get("QCDest_CombRelIso_3jets"));
    TH1F * pj_3jets = (TH1F*) fpj1->Get("QCDest_CombRelIso_3jets");
    pj_3jets->Add((TH1F*) fpj2->Get(Form("QCDest_CombRelIso_3jets", nj.c_str())));
    pj_3jets->Add((TH1F*) fpj3->Get(Form("QCDest_CombRelIso_3jets", nj.c_str())));
    TH1F * wj_3jets = (TH1F*) fwjets->Get("QCDest_CombRelIso_3jets");
    TH1F * zj_3jets = (TH1F*) fzjets->Get("QCDest_CombRelIso_3jets");
    TH1F * tt_3jets = (TH1F*) fttbar->Get("QCDest_CombRelIso_3jets");
    TH1F *stop_3jets = (TH1F*) fsTopTW->Get("QCDest_CombRelIso_3jets");
    stop_3jets->Add((TH1F*) fsToptC->Get("QCDest_CombRelIso_3jets"));
    TH1F * mc_3jets = QCD_3jets->Clone("sumMC_3jets");
    mc_3jets->Add(pj_3jets);
    mc_3jets->Add(wj_3jets);
    mc_3jets->Add(zj_3jets);
    mc_3jets->Add(tt_3jets);
    mc_3jets->Add(stop_3jets);

    // >=4-jet
    TH1F *data_4orMoreJets = (TH1F*) fdata->Get("QCDest_CombRelIso_4orMoreJets");
    TH1F * QCD_4orMoreJets = (TH1F*) fbce2->Get("QCDest_CombRelIso_4orMoreJets");
    QCD_4orMoreJets->Add((TH1F*) fbce3->Get("QCDest_CombRelIso_4orMoreJets"));
    QCD_4orMoreJets->Add((TH1F*) fenri1->Get("QCDest_CombRelIso_4orMoreJets"));
    QCD_4orMoreJets->Add((TH1F*) fenri2->Get("QCDest_CombRelIso_4orMoreJets"));
    QCD_4orMoreJets->Add((TH1F*) fenri3->Get("QCDest_CombRelIso_4orMoreJets"));
    TH1F * pj_4orMoreJets = (TH1F*) fpj1->Get("QCDest_CombRelIso_4orMoreJets");
    pj_4orMoreJets->Add((TH1F*) fpj2->Get(Form("QCDest_CombRelIso_4orMoreJets", nj.c_str())));
    pj_4orMoreJets->Add((TH1F*) fpj3->Get(Form("QCDest_CombRelIso_4orMoreJets", nj.c_str())));
    TH1F * wj_4orMoreJets = (TH1F*) fwjets->Get("QCDest_CombRelIso_4orMoreJets");
    TH1F * zj_4orMoreJets = (TH1F*) fzjets->Get("QCDest_CombRelIso_4orMoreJets");
    TH1F * tt_4orMoreJets = (TH1F*) fttbar->Get("QCDest_CombRelIso_4orMoreJets");
    TH1F *stop_4orMoreJets = (TH1F*) fsTopTW->Get("QCDest_CombRelIso_4orMoreJets");
    stop_4orMoreJets->Add((TH1F*) fsToptC->Get("QCDest_CombRelIso_4orMoreJets"));
    TH1F * mc_4orMoreJets = QCD_4orMoreJets->Clone("sumMC_4orMoreJets");
    mc_4orMoreJets->Add(pj_4orMoreJets);
    mc_4orMoreJets->Add(wj_4orMoreJets);
    mc_4orMoreJets->Add(zj_4orMoreJets);
    mc_4orMoreJets->Add(tt_4orMoreJets);
    mc_4orMoreJets->Add(stop_4orMoreJets);

    // First make clone of 4orMoreJets since this will be added for all cases
    data = (TH1F*) data_4orMoreJets->Clone("data");
    mc = (TH1F*) mc_4orMoreJets->Clone("mc");
    QCD = (TH1F*) QCD_4orMoreJets->Clone("QCD");
    pj = (TH1F*) pj_4orMoreJets->Clone("pj");
    wj = (TH1F*) wj_4orMoreJets->Clone("wj");
    zj = (TH1F*) zj_4orMoreJets->Clone("zj");
    tt = (TH1F*) tt_4orMoreJets->Clone("tt");
    stop = (TH1F*) stop_4orMoreJets->Clone("stop");

    //----------------------------------
    // >=0jet: 0jet+1jet+2jets+3jets+4orMoreJets = all
    // >=1jet:    1jet+2jets+3jets+4orMoreJets
    // >=2jets:       2jets+3jets+4orMoreJets
    // >=3jets:          3jets+4orMoreJets
    // >=4j:             4orMoreJets = 4orMoreJets
    //----------------------------------

    // Add 1jet
    if (nj == "1mj") {
        data->Add(data_1jet);
        mc->Add(mc_1jet);
        QCD->Add(QCD_1jet);
        pj->Add(pj_1jet);
        wj->Add(wj_1jet);
        zj->Add(zj_1jet);
        tt->Add(tt_1jet);
        stop->Add(stop_1jet);
    }

    // Add 2jets
    if (nj == "1mj" || nj == "2mj") {
        data->Add(data_2jets);
        mc->Add(mc_2jets);
        QCD->Add(QCD_2jets);
        pj->Add(pj_2jets);
        wj->Add(wj_2jets);
        zj->Add(zj_2jets);
        tt->Add(tt_2jets);
        stop->Add(stop_2jets);
    }

    // Add 3jets
    data->Add(data_3jets);
    mc->Add(mc_3jets);
    QCD->Add(QCD_3jets);
    pj->Add(pj_3jets);
    wj->Add(wj_3jets);
    zj->Add(zj_3jets);
    tt->Add(tt_3jets);
    stop->Add(stop_3jets);

    cout << "-----------------------------------" << endl;
    cout << "     DATA      nEntries    Integral" << endl;
    cout << "-----------------------------------" << endl;
    cout << setw(10) << "0jet" << setw(10) << data_0jet->GetEntries() << setw(10) << data_0jet->Integral() << endl;
    cout << setw(10) << "1jet" << setw(10) << data_1jet->GetEntries() << setw(10) << data_1jet->Integral() << endl;
    cout << setw(10) << "2jets" << setw(10) << data_2jets->GetEntries() << setw(10) << data_2jets->Integral() << endl;
    cout << setw(10) << "3jets" << setw(10) << data_3jets->GetEntries() << setw(10) << data_3jets->Integral() << endl;
    cout << setw(10) << ">=4j" << setw(10) << data_4orMoreJets->GetEntries() << setw(10)
            << data_4orMoreJets->Integral() << endl;
    cout << "-----------------------------------" << endl;
    cout << setw(10) << ijlabel[ijet] << setw(10) << data->GetEntries() << setw(10) << data->Integral() << endl;
    cout << "-----------------------------------" << endl;

    return 0;
}

void make_QCDnormToEstimate_plot() {

    TCanvas *c1 = new TCanvas("c1", "RelIso fit", 600, 400);//,"",500,500);

    data->GetXaxis()->SetRangeUser(0, 1.6 - 0.01);
    if (nj == "2jets")
        data->GetYaxis()->SetRangeUser(0, 16.);

    // draw data
    data->Draw();
    if (nj == "all" && YMAX_all > 0)
        data->GetYaxis()->SetRangeUser(0, YMAX_all);
    if (nj == "1mj" && YMAX_1mj > 0)
        data->GetYaxis()->SetRangeUser(0, YMAX_1mj);
    if (nj == "2mj" && YMAX_2mj > 0)
        data->GetYaxis()->SetRangeUser(0, YMAX_2mj);
    if (nj == "3mj" && YMAX_3mj > 0)
        data->GetYaxis()->SetRangeUser(0, YMAX_3mj);
    if (nj == "4orMoreJets" && YMAX_4orMoreJets > 0)
        data->GetYaxis()->SetRangeUser(0, YMAX_4orMoreJets);

    // draw mc
    mcStack->Draw("ahist same");
    data->Draw("ae same");
    data->Draw("axis same");

    // Add "CMS Preliminary", integrated luminosity and sqrt(s), and legend
    add_cms_label(intlumi, nj);
    add_legend_nofit();

    string out = Form("ele_reliso_normToLumi_QCDnormToEst_%s", nj.c_str());

    cout << "out: " << out << endl;
    c1->SaveAs(Form("%s.C", out.c_str()));
    c1->SaveAs(Form("%s.eps", out.c_str()));
    gROOT->ProcessLine(Form(".!ps2pdf -dEPSCrop %s.eps", out.c_str()));
    gROOT->ProcessLine(Form(".!rm -f %s.eps", out.c_str()));

    c1->Close(); //crucial!

}

//------------------------------------------------------------------------------
//
//                                  main fit code
//
//------------------------------------------------------------------------------

void fit_njet(string function = "pol1", float Fit_From_user = 0.1) {

    static bool firstTime = true;
    cout << "\nSwitched from chi2 fit to likelihood fit (better with low stats)\n" << endl;

    // Try: 0.1, 0.2
    float Fit_From = Fit_From_user;
    cout << "-----------------------" << endl;
    cout << " FIT:  " << function << "   range " << Fit_From << "--1.6" << endl;
    cout << "-----------------------" << endl;

    double lw = 2; //line width (function)


    TCanvas *c1 = new TCanvas("c1", "RelIso fit", 600, 400);//,"",500,500);


    //  gStyle->SetOptStat(1110);//off title


    data->GetXaxis()->SetRangeUser(0, 1.6 - 0.01);
    if (nj == "2jets") {
        //    double data_max = data->GetMaximum();
        data->GetYaxis()->SetRangeUser(0, 18.);
    }
    // MET>25
    //  data->GetYaxis()->SetRangeUser(0,50.);


    //data->SetTitleFont(42,"XY");
    //  data->GetYaxis()->SetTitleOffset(1.5);

    data->Draw();
    if (firstTime) {
        YMAX_all = 2 * data->GetMaximum();
        firstTime = false;
    }

    //if(nj=="all" && YMAX_all>0) data->GetYaxis()->SetRangeUser( 0,YMAX_all );
    data->GetYaxis()->SetRangeUser(0, YMAX_all);

    /*
     if(nj=="all" && YMAX_all>0) data->GetYaxis()->SetRangeUser(0,YMAX_all);
     if(nj=="1mj"  && YMAX_1mj >0) data->GetYaxis()->SetRangeUser(0,YMAX_1mj);
     if(nj=="2mj"  && YMAX_2mj >0) data->GetYaxis()->SetRangeUser(0,YMAX_2mj);
     if(nj=="3mj"  && YMAX_3mj >0) data->GetYaxis()->SetRangeUser(0,YMAX_3mj);
     if(nj=="4orMoreJets"  && YMAX_4orMoreJets >0) data->GetYaxis()->SetRangeUser(0,YMAX_4orMoreJets);
     */

    // draw mc
    //  mc->Draw("ahist same");
    mcStack->Draw("ahist same");
    data->Draw("ae same");
    data->Draw("axis same");

    cout << "Fit Range: " << Fit_From << "-1.6" << endl;

    //  TFitResultPtr myFitResult = data->Fit("pol1","0S","ah",Fit_From,1.6); // <----Fit Range
    //    const char *func = Form("%s", function);

    // Fit options used:
    //  L : likelihood method
    //  S : store fit results
    //  0 : do not draw
    TFitResultPtr myFitResult = data->Fit(function.c_str(), "0SL", "ah", Fit_From, 1.6); // <----Fit Range
    cout << "func " << function.c_str() << endl;
    // Fit line in red
    //  TF1 *myf = data->GetFunction("pol1");
    TF1 *myf = data->GetFunction(function.c_str());
    myf->SetLineColor(kRed);
    myf->SetLineWidth(lw);

    // Extrapolation in dashed blue
    TF1 *myf2 = (TF1*) myf->Clone(); //range 0-0.1
    myf2->SetLineColor(kBlue);
    myf2->SetLineStyle(kDashed);
    myf2->SetLineWidth(lw);
    myf2->SetRange(0, Fit_From);

    myf->Draw("same");
    myf2->Draw("same");

    // Get estimate from extrapolation
    double n_extrap = myf2->Integral(0, 0.1) / 0.1; //note divided by bin width=0.1

    double p0 = myf->GetParameter(0);
    double p1 = myf->GetParameter(1);
    double e0 = myf->GetParError(0);
    double e1 = myf->GetParError(1);

    double chi2 = myFitResult->Chi2();
    double ndf = myFitResult->Ndf();

    cout << "------------" << endl;
    cout << "Fit results" << endl;
    cout << "------------" << endl;
    cout << "n extrap = " << n_extrap << endl;
    cout << "p0 = " << p0 << " +/- " << e0 << endl;
    cout << "p1 = " << p1 << " +/- " << e1 << endl;
    cout << "chi2/ndf =  " << chi2 << " / " << ndf << endl;

    // Construct new functions to see how estimate varies within fit parameter
    // uncertainties, ie error of p0 and p1

    // vary p0 (normalization)
    //  TF1 *newF1_up = new TF1("pol1","pol1",0,1.6);
    TF1 *newF1_up = new TF1(function.c_str(), function.c_str(), 0, 1.6);
    newF1_up->SetParameters(p0 + e0, p1);
    newF1_up->SetLineColor(kGreen);
    newF1_up->SetLineWidth(lw);
    //newF1_up->SetLineStyle(kDashed);
    //newF1_up->Draw("same");

    //  TF1 *newF1_down = new TF1("pol1","pol1",0,1.6);
    TF1 *newF1_down = new TF1(function.c_str(), function.c_str(), 0, 1.6);
    newF1_down->SetParameters(p0 - e0, p1);
    newF1_down->SetLineColor(kGreen);
    newF1_down->SetLineWidth(lw);
    //newF1_down->SetLineStyle(kDashed);
    //newF1_down->Draw("same");

    /*
     // vary p1 (gradient)
     TF1 *newF2_up = new TF1("pol1","pol1",0,1.6);
     newF2_up->SetParameters(p0, p1+e1);
     newF2_up->SetLineColor(kViolet);
     newF2_up->SetLineWidth(lw);
     //newF2_up->SetLineStyle(kDashed);
     //newF2_up->Draw("same");

     TF1 *newF2_down = new TF1("pol1","pol1",0,1.6);
     newF2_down->SetParameters(p0, p1-e1);
     newF2_down->SetLineColor(kViolet);
     newF2_down->SetLineWidth(lw);
     //newF2_down->SetLineStyle(kDashed);
     //newF2_down->Draw("same");
     */

    // Get the new estimates
    double est_1 = newF1_up->Integral(0, 0.1) / 0.1;
    double est_2 = newF1_down->Integral(0, 0.1) / 0.1;
    //double est_3 = newF2_up->Integral(0,0.1)/0.1;
    //double est_4 = newF2_down->Integral(0,0.1)/0.1;
    cout << "n extrap (p0 + e0,   p1     ) = " << est_1 << endl;
    cout << "n extrap (p0 - e0,   p1     ) = " << est_2 << endl;
    //cout << "n extrap (p0     ,   p1 + e1) = " << est_3 << endl;
    //cout << "n extrap (p0     ,   p1 - e1) = " << est_4 << endl;

    // take the maximum deviation as the uncertainty
    double est_unc = 0;
    if (fabs(est_1 - n_extrap) > est_unc)
        est_unc = fabs(est_1 - n_extrap);
    if (fabs(est_2 - n_extrap) > est_unc)
        est_unc = fabs(est_2 - n_extrap);
    //if( fabs(est_3 - n_extrap) > est_unc ) est_unc = fabs(est_3 - n_extrap);
    //if( fabs(est_4 - n_extrap) > est_unc ) est_unc = fabs(est_4 - n_extrap);

    cout << "Take the maximum deviation as the uncertainty of the QCD estimate." << endl;
    cout << "--------------------------------------------" << endl;
    cout << "  RESULT: " << function << " " << Fit_From << "-1.6" << endl;
    cout << "--------------------------------------------" << endl;
    // print out MC prediction for QCD + photon+jets
    if (normMode == 1) {
        cout << "\n  QCD predicted =  " << QCD->Integral(0, 1) << " (Lumi)" << endl;
        TheEstimates << "\n  QCD predicted =  " << QCD->Integral(0, 1) << " (Lumi)" << endl;
    }
    cout << "\n  QCD estimate  =  " << n_extrap << "  +/-  " << est_unc << " (vary p0)" << endl;
    TheEstimates << "  QCD estimate  =  " << n_extrap << "  +/-  " << est_unc << " (vary p0)" << endl;

    //----------------------
    // error propagation
    //----------------------
    double N = 0;
    double sigmaN = 0;

    if (function == "pol1") {
        double c = p0;
        double m = p1;

        double eC = e0;
        double eM = e1;

        N = 0.05 * m + c;
        sigmaN = TMath::Hypot(0.05 * eM, eC);

    } else if (function == "expo") {
        double a = p0;
        double b = p1;
        double eA = e0;
        double eB = e1;

        double x2 = 0.1;

        double exp_ab = exp(a + 0.1 * b);
        double exp_a = exp(a);

        N = 10 / b * (exp_ab - exp_a);

        // del(N)/del(a/b)
        double dNda = 1 / (b * x2) * (exp_ab - exp_a);
        double dNdb = -1 / (b * b * x2) * (exp_ab - exp_a) + 1 / b * exp_ab;

        // Note: sigma(N) = sqrt( dNda^2*eA^2 + dBdb^2*eB^2  )
        sigmaN = TMath::Hypot(dNda * eA, dNdb * eB);
    } else if (function == "gaus") {
        double p2 = myf->GetParameter(2);
        double e2 = myf->GetParError(2);
        // Not finished. Calculated estimate is wrong.
        cout << "\n--> Error propagation for Gaussian not yet implemented!" << endl;
        //calculate_gaus_unc(p0,p1,p2);
    }

    cout << "\n  QCD estimate  =  " << N;
    cout << "  +/-  " << sigmaN << " (error propagation)\n\n";
    cout << "--------------------------------------------" << endl;

    // Add "CMS Preliminary", integrated luminosity and sqrt(s), and legend
    add_cms_label(intlumi, nj);
    //  add_fit_res( Fit_From, chi2, ndf );
    add_legend(function);

    // save as eps, then convert to pdf
    cout << "Fit_From=" << Fit_From << endl;
    string out;
    if (function == "pol1")
        out = Form("ele_reliso_fit_%s_linear_from_0%.0f", nj.c_str(), Fit_From * 10.0);
    else
        out = Form("ele_reliso_fit_%s_%s_from_0%.0f", function.c_str(), nj.c_str(), Fit_From * 10.0);

    if (normMode == 1)
        out += "_normToLumi";
    else
        out += "_normToData";

    cout << "out: " << out << endl;
    c1->SaveAs(Form("%s.C", out.c_str())); //save as C++ root macro
    c1->SaveAs(Form("%s.eps", out.c_str()));
    gROOT->ProcessLine(Form(".!ps2pdf -dEPSCrop %s.eps", out.c_str()));
    gROOT->ProcessLine(Form(".!rm -f %s.eps", out.c_str()));

    c1->Close(); //crucial!
}

void calculate_gaus_unc(double a, double b, double c) {

    // This formula gives wrong results. ??
    cout << "a: " << a << endl;
    cout << "b: " << b << endl;
    cout << "c: " << c << endl;
    const double x1 = 0.1;
    double exp1 = exp(-0.5 * pow((x1 - b) / c, 2));
    double exp2 = exp(-0.5 * pow(b / c, 2));
    double est = -a * c * c / x1 * (exp1 / (x1 - b) + exp2 / b);
    //cout << "est: "<< est << endl;

}
//------------------------------------------------------------------------------------

void ScaleMCtoEstimate(string nj_user = "all", float estimate) {
    //aim: scale QCD in signal region to data estimate and compare MC to data
    nj = nj_user;
    setStyle();
    getHisto();

    data->GetXaxis()->SetRangeUser(0, 1.6);

    tempCan = new TCanvas("tempCan", "tempCan", 600, 400);
    data->Draw("");
    data->SetMaximum(2 * data->GetMaximum());

    myStack = new THStack("myStack", "myMC stack");
    float mcQCD = QCD->Integral(0, 1);
    cout << mcQCD << endl;
    QCD->Scale(estimate / mcQCD);//setes QCD to estimate
    //QCD->Add(pj);

    myStack->Add(QCD);
    myStack->Add(pj);
    myStack->Add(stop);
    myStack->Add(zj);
    myStack->Add(wj);
    myStack->Add(tt);
    myStack->Draw("ahist same");
    /*
     QCD->Draw("hist same");
     pj->Draw("hist same");
     wj->Draw("hist same");
     zj->Draw("hist same");
     tt->Draw("hist same");
     stop->Draw("hist same");
     */

    data->Draw("ae same");
    data->Draw("axis same");

    //mcStack->Draw("ahist same");
    //data->Draw("ae same");
    tempCan->SaveAs(Form("MCQCDscaledtoData_%s.pdf", nj_user));
    tempCan->Close();

    //  cout<<mcStack->GetBinContent(1)<<endl;
}
