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

void do2DPlots();
void do2DPlots(bool muon, TString variable, TString xtitle, TString ytitle);
void getBinning(bool muon, TString variable, TString xtitle, TString ytitle);
void do2DPlots(){

bool muon = true;

TString variable[10] = {"GenHT_vs_RecoHT", "GenHT_lep_vs_RecoHT_lep", "GenHT_lep_met_vs_RecoHT_lep_met", "Genleptonic_W_pt_vs_Recoleptonic_W_pt", "GenM3_vs_RecoM3", "GenNu_vs_RecoMET", "GenParton_vs_RecoHT", "GenJetHT_vs_GenParton", "GenMuPlusMETPt_vs_RecoMuPlusMetPt", "GendPhiMuMet_vs_RecodPhiMuMetPt"};
TString xtitle[10] = {"Gen HT (GeV)", "Gen HT+lep (GeV)", "Gen HT+lep+met (GeV)", "Gen leptonic W pt (GeV)", "Gen M3 (GeV)", "Reco Nu (GeV)", "Gen HT GeVReco Parton HT (GeV)", "Gen Jet HT (GeV)", "Gen #mu pt + MET  (GeV)", "Gen #Delta#Phi(#mu,MET)"};
TString ytitle[10] = {"Reco HT (GeV)", "Reco HT+lep (GeV)", "Reco HT+lep+met (GeV)", "Reco leptonic W pt (GeV)", "Reco M3 (GeV)", "Reco MET (GeV)", "Reco HT (GeV)", "Gen Parton HT (GeV)", "Reco #mu pt + MET  (GeV)", "Reco #Delta#Phi(#mu,MET)"};



for(int i =8; i<10; i++){
do2DPlots(muon, variable[i], xtitle[i], ytitle[i]);
//getBinning(muon, variable[i], xtitle[i], ytitle[i]);
}

}

void do2DPlots(bool muon, TString variable, TString xtitle, TString ytitle){

	TString leptonFolder;
	if(muon == true){
		leptonFolder = "MuPlusJets/";
	}else{
		leptonFolder = "EPlusJets/";
		}	
		
  	setTDRStyle();
  	gStyle->SetPalette(1);

	TString dir = "../";
	TFile* tt_file = new TFile(dir + "TTJet_5050pb_PFElectron_PFMuon_PF2PATJets_PFMET_TEST.root");  



TString Nbtags[5] = {"_0btag","_1btag", "_2btags","_3btags",  "_4orMoreBtags"};

for(int i = 2; i < 3; i++){

	TH2D* tt_2d = (TH2D*) tt_file->Get("Binning/"+leptonFolder+variable+Nbtags[i]);

	tt_2d->Rebin2D(10,10);
  	tt_2d->GetYaxis()->SetTitle(ytitle);
  	tt_2d->GetXaxis()->SetTitle(xtitle); 
	tt_2d->GetYaxis()->SetTitleOffset(1.8);
	tt_2d->GetXaxis()->SetTitleOffset(1.5);	

        TCanvas *c= new TCanvas("c","c",10,10,800,600);
	tt_2d->Draw("COLZ");
	
  	TString plotName("plots/"+leptonFolder);
        plotName += variable;
        plotName += Nbtags[i]+".png";
 
  c->SaveAs(plotName);
  delete c;
	
}


}


void getBinning(bool muon, TString variable, TString xtitle, TString ytitle){

	TString leptonFolder;
	if(muon == true){
		leptonFolder = "MuPlusJets/";
	}else{
		leptonFolder = "EPlusJets/";
		}

	TString dir = "../";
	TFile* tt_file = new TFile(dir + "TTJet_5050pb_PFElectron_PFMuon_PF2PATJets_PFMET_TEST.root");

	TH2D* tt_2d = (TH2D*) tt_file->Get("Binning/"+leptonFolder+variable+"_2btags");

	int i = 0;
	int binMin[60];
	binMin[0] = 0;
//	int binCho[6] = {0, 80, 110, 160, 240, 340}; //hT
//	int binCho[6] = {0, 60, 90, 120, 160, 240}; //mwt
//	int binCho[5] = {0, 100, 140, 190, 299}; //m3
	int binCho[16] = {90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 220, 240, 280, 499};
	
for(int bin = 0; bin<tt_2d->GetNbinsX(); bin++){
	double purity[60];
	double stability[60];

	stability[i] = tt_2d->Integral(binMin[i], bin+1,binMin[i],bin+1)/tt_2d->Integral(binMin[i], bin+1,0,tt_2d->GetNbinsX()+1);
	purity[i] = tt_2d->Integral(binMin[i], bin+1,binMin[i],bin+1)/tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1);

double weight  = 20000*210.5/6920475;

// 	if(purity[i]>=0.6 && stability[i]>=0.6 && tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1)*weight >1500){
// 	cout << "purity: " << purity[i] << ", stability: " << stability[i] << " bin: " << bin << " evts: " << tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1)*weight << endl;
// 	i++;
// 	binMin[i] = bin;
// 	}
		
		
		if(bin == binCho[i]){
		cout << bin*4 << " & " ;
		//	cout <<setprecision(2) << purity[i] << " & " ;
			//cout <<setprecision(2) << "purity: " << purity[i] << ", stability: " << stability[i] << " bin: " << bin*2000/500 << endl;
		i++;
		binMin[i] = bin;
		}


}
cout << " " << endl;

i = 0;
binMin[0] = 0;
for(int bin = 0; bin<tt_2d->GetNbinsX(); bin++){
	double purity[60];
	double stability[60];

	stability[i] = tt_2d->Integral(binMin[i], bin+1,binMin[i],bin+1)/tt_2d->Integral(binMin[i], bin+1,0,tt_2d->GetNbinsX()+1);
	purity[i] = tt_2d->Integral(binMin[i], bin+1,binMin[i],bin+1)/tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1);

double weight  = 20000*210.5/6920475;

// 	if(purity[i]>=0.6 && stability[i]>=0.6 && tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1)*weight >1500){
// 	cout << "purity: " << purity[i] << ", stability: " << stability[i] << " bin: " << bin << " evts: " << tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1)*weight << endl;
// 	i++;
// 	binMin[i] = bin;
// 	}
		
		
		if(bin == binCho[i]){
		//cout << bin*4 << " & " ;
			cout<<setprecision(2) << tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1) << " & " ;
			//cout <<setprecision(2) << "purity: " << purity[i] << ", stability: " << stability[i] << " bin: " << bin*2000/500 << endl;
		i++;
		binMin[i] = bin;
		}


}
cout << " " << endl;

i = 0;
binMin[0] = 0;
for(int bin = 0; bin<tt_2d->GetNbinsX(); bin++){
	double purity[60];
	double stability[60];

	stability[i] = tt_2d->Integral(binMin[i], bin+1,binMin[i],bin+1)/tt_2d->Integral(binMin[i], bin+1,0,tt_2d->GetNbinsX()+1);
	purity[i] = tt_2d->Integral(binMin[i], bin+1,binMin[i],bin+1)/tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1);

double weight  = 20000*210.5/6920475;

// 	if(purity[i]>=0.6 && stability[i]>=0.6 && tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1)*weight >1500){
// 	cout << "purity: " << purity[i] << ", stability: " << stability[i] << " bin: " << bin << " evts: " << tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1)*weight << endl;
// 	i++;
// 	binMin[i] = bin;
// 	}
		
		
		if(bin == binCho[i]){
		//cout << bin*4 << " & " ;
			cout<<setprecision(2) << tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1)*weight << " & " ;
			//cout <<setprecision(2) << "purity: " << purity[i] << ", stability: " << stability[i] << " bin: " << bin*2000/500 << endl;
		i++;
		binMin[i] = bin;
		}


}
cout << " " << endl;

i = 0;
binMin[0] = 0;
for(int bin = 0; bin<tt_2d->GetNbinsX(); bin++){
	double purity[60];
	double stability[60];

	stability[i] = tt_2d->Integral(binMin[i], bin+1,binMin[i],bin+1)/tt_2d->Integral(binMin[i], bin+1,0,tt_2d->GetNbinsX()+1);
	purity[i] = tt_2d->Integral(binMin[i], bin+1,binMin[i],bin+1)/tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1);

double weight  = 20000*210.5/6920475;

// 	if(purity[i]>=0.6 && stability[i]>=0.6 && tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1)*weight >1500){
// 	cout << "purity: " << purity[i] << ", stability: " << stability[i] << " bin: " << bin << " evts: " << tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1)*weight << endl;
// 	i++;
// 	binMin[i] = bin;
// 	}
		
		
		if(bin == binCho[i]){
		//cout << bin*4 << " & " ;
			cout <<setprecision(2) << purity[i] << " & " ;
			//cout <<setprecision(2) << "purity: " << purity[i] << ", stability: " << stability[i] << " bin: " << bin*2000/500 << endl;
		i++;
		binMin[i] = bin;
		}


}
cout << " " << endl;

i = 0;
binMin[0] = 0;
for(int bin = 0; bin<tt_2d->GetNbinsX(); bin++){
	double purity[60];
	double stability[60];

	stability[i] = tt_2d->Integral(binMin[i], bin+1,binMin[i],bin+1)/tt_2d->Integral(binMin[i], bin+1,0,tt_2d->GetNbinsX()+1);
	purity[i] = tt_2d->Integral(binMin[i], bin+1,binMin[i],bin+1)/tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1);

double weight  = 20000*210.5/6920475;

// 	if(purity[i]>=0.6 && stability[i]>=0.6 && tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1)*weight >1500){
// 	cout << "purity: " << purity[i] << ", stability: " << stability[i] << " bin: " << bin << " evts: " << tt_2d->Integral(0,tt_2d->GetNbinsX()+1 ,binMin[i],bin+1)*weight << endl;
// 	i++;
// 	binMin[i] = bin;
// 	}
		
		
		if(bin == binCho[i]){
		//cout << bin*4 << " & " ;
			cout <<setprecision(2) << stability[i] << " & " ;
			//cout <<setprecision(2) << "purity: " << purity[i] << ", stability: " << stability[i] << " bin: " << bin*2000/500 << endl;
		i++;
		binMin[i] = bin;
		}


}

}
