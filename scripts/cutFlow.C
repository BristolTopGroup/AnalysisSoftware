#include "TFile.h"
#include "TH1.h"
#include "TObject.h"
#include "TString.h"
#include "TCanvas.h"
#include <string.h>
#include <iostream>
#include <iomanip>
#include "tdrstyle.C"


void cutFlow();
TH1D* getSample(TString sample, bool muon);


void cutFlow(){
	bool muon = true;
TH1D* tt = getSample("TTJet", muon);
TH1D* ttEff = new TH1D("cut eff","cut eff",10,0,10);

for(int i =1; i<10; i++){
ttEff->SetBinContent(i+1, tt->GetBinContent(i+1)/tt->GetBinContent(i));
}	
 
  TCanvas *c1 = new TCanvas("cutflow","cutflow",600, 500);
	tt->Draw();
	c1->SaveAs("plots/cutFlow/cutFlow.png");

  TCanvas *c2 = new TCanvas("cutflow eff","cutflow eff",600, 500);
	ttEff->Draw();
	c2->SaveAs("plots/cutFlow/cutEff.png");

double weight = 5050*157.5/6712238;
weight = 1.;

if(muon == true){
std::cout  << setprecision(6)<< "skim &  " << weight*tt->GetBinContent(1) << "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(1)<< "  \\\\ " << std::endl;
std::cout<< setprecision(6) << "trigger and clean & " << weight*tt->GetBinContent(2) << setprecision(3) << "  $\\pm$ " << weight*tt->GetBinError(2)<< "  \\\\ " << std::endl;
std::cout << setprecision(6)<< "1==mu &  " << weight*tt->GetBinContent(3) << "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(3)<< "  \\\\ " << std::endl;
std::cout<< setprecision(6) << "mu veto &  " << weight*tt->GetBinContent(4)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(4)<< "  \\\\ "  << std::endl;
std::cout<< setprecision(6) << "e veto &  " << weight*tt->GetBinContent(5)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(5) << "  \\\\ " << std::endl;
std::cout<< setprecision(6) << "$\\geq$1jets &  " << weight*tt->GetBinContent(6)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(6)<< "  \\\\ "  << std::endl;
std::cout << setprecision(6)<< "$\\geq$2jets &  " << weight*tt->GetBinContent(7)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(7)<< "  \\\\ "  << std::endl;
std::cout<< setprecision(6) << "$\\geq$3jets &  " << weight*tt->GetBinContent(8)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(8) << "  \\\\ " << std::endl;
std::cout<< setprecision(6) << "$\\geq$4jets &  " << weight*tt->GetBinContent(9)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(9) << "  \\\\ " << std::endl;
std::cout<< setprecision(6) << "$\\geq$1btag &  " << weight*tt->GetBinContent(10) << "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(10)<< "  \\\\ " << std::endl;
std::cout<< setprecision(6) << "$\\geq$2btag &  " << weight*tt->GetBinContent(11) << "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(11)<< "  \\\\ " << std::endl;
}else{

	std::cout  << setprecision(6)<< "skim &  " << weight*tt->GetBinContent(1) << "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(1)<< "  \\\\ " << std::endl;
	std::cout<< setprecision(6) << "trigger and clean & " << weight*tt->GetBinContent(2) << setprecision(3) << "  $\\pm$ " << weight*tt->GetBinError(2)<< "  \\\\ " << std::endl;
	std::cout << setprecision(6)<< "1==mu &  " << weight*tt->GetBinContent(3) << "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(3)<< "  \\\\ " << std::endl;
	std::cout<< setprecision(6) << "mu veto &  " << weight*tt->GetBinContent(4)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(4)<< "  \\\\ "  << std::endl;
	std::cout<< setprecision(6) << "e veto &  " << weight*tt->GetBinContent(5)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(5) << "  \\\\ " << std::endl;
	std::cout<< setprecision(6) << "conv. veto &  " << weight*tt->GetBinContent(6)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(6) << "  \\\\ " << std::endl;
	std::cout<< setprecision(6) << "$\\geq$1jets &  " << weight*tt->GetBinContent(7)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(7)<< "  \\\\ "  << std::endl;
	std::cout << setprecision(6)<< "$\\geq$2jets &  " << weight*tt->GetBinContent(8)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(8)<< "  \\\\ "  << std::endl;
	std::cout<< setprecision(6) << "$\\geq$3jets &  " << weight*tt->GetBinContent(9)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(9) << "  \\\\ " << std::endl;
	std::cout<< setprecision(6) << "$\\geq$4jets &  " << weight*tt->GetBinContent(10)<< "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(10) << "  \\\\ " << std::endl;
	std::cout<< setprecision(6) << "$\\geq$1btag &  " << weight*tt->GetBinContent(11) << "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(11)<< "  \\\\ " << std::endl;
	std::cout<< setprecision(6) << "$\\geq$2btag &  " << weight*tt->GetBinContent(12) << "  $\\pm$ " << setprecision(3) << weight*tt->GetBinError(12)<< "  \\\\ " << std::endl;


}
}

TH1D* getSample(TString sample, bool muon){
	TString dir = "../";
	TFile* tt_file = new TFile(dir + sample + "_5050pb_PFElectron_PFMuon_PF2PATJets_PFMET_TEST.root");
	TDirectoryFile* tt_folder = (TDirectoryFile*) tt_file->Get("EventCount");
	TH1D* tt_cutflow;
	if(muon == true){
	tt_cutflow = (TH1D*) tt_folder->Get("TTbarMuPlusJetsRefSelectionUnweighted");
	}else{
    tt_cutflow = (TH1D*) tt_folder->Get("TTbarEplusJetsRefSelectionUnweighted");
	}
    return tt_cutflow;
}

