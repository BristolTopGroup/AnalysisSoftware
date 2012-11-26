#include <iomanip>
#include "tdrstyle.C"
#include "TTree.h"
#include "TFile.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TText.h"
#include "TLegend.h"
#include "THStack.h"
#include "TLine.h"
#include "TChain.h"
#include "TLatex.h"
#include <iostream>
#include <sstream>
#include "TLegend.h"
using namespace std;

void drawPlots();
void do2DPlots(TString variable, TString xtitle, TString ytitle);

void drawPlots(){

TString variable[8] = {"RecoHT_vs_GenHT", "RecoHT_lep_vs_GenHT_lep", "RecoHT_lep_met_vs_GenHT_lep_met", "Recoleptonic_W_pt_vs_Genleptonic_W_pt", "RecoM3_vs_GenM3", "RecoMET_vs_GenNu", "RecoHT_vs_GenParton", "GenJetHT_vs_GenParton"};

TString xtitle[8] = {"Reco HT (GeV)", "Reco HT+lep (GeV)", "Reco HT+lep+met (GeV)", "Reco leptonic W pt (GeV)", "Reco M3 (GeV)", "Reco MET (GeV)", "Reco HT GeV", "Gen Jet HT (GeV)"};
TString ytitle[8] = {"Gen HT (GeV)", "Gen HT+lep (GeV)", "Gen HT+lep+met (GeV)", "Gen leptonic W pt (GeV)", "Gen M3 (GeV)", "Gen Nu (GeV)", "Gen Parton HT (GeV)", "Gen Parton HT (GeV)"};

for(int i =0; i<8; i++){
do2DPlots(variable[i], xtitle[i], ytitle[i]);
}

}

void do2DPlots(TString variable, TString xtitle, TString ytitle){
  	setTDRStyle();
  	gStyle->SetPalette(1);

	TString dir = "../";
	TFile* tt_file = new TFile(dir + "TTJet_5050pb_PFElectron_PFMuon_PF2PATJets_PFMET_TESTING.root");  

	TH2D* tt_2d = (TH2D*) tt_file->Get("Binning/MuPlusJets/"+variable+"_1btag");

	tt_2d->Rebin2D(10,10);
  	tt_2d->GetYaxis()->SetTitle(ytitle);
  	tt_2d->GetXaxis()->SetTitle(xtitle); 
	tt_2d->GetYaxis()->SetTitleOffset(1.8);
	tt_2d->GetXaxis()->SetTitleOffset(1.5);	

        TCanvas *c= new TCanvas("c","c",10,10,800,600);
	tt_2d->Draw("COLZ");
	
  	TString plotName("plots/muPlusJets/");
        plotName += variable;
        plotName += "_1btag.png";
 
  c->SaveAs(plotName);
  delete c;
	
}
	
