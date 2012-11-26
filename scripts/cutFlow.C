#include "TFile.h"
#include "TH1.h"
#include "TObject.h"
#include "TString.h"
#include <string.h>
#include <iostream>

void cutFlow();
TH1D* getSample(TString sample);


void cutFlow(){

TH1D* tt = getSample("TTJet");

std::cout << "skim: " << tt->GetBinContent(1) << std::endl;
std::cout << "trigger and clean: " << tt->GetBinContent(2) << std::endl;
std::cout << "1==mu: " << tt->GetBinContent(3) << std::endl;
std::cout << "mu veto: " << tt->GetBinContent(4) << std::endl;
std::cout << "e veto: " << tt->GetBinContent(5) << std::endl;
std::cout << ">=1jets: " << tt->GetBinContent(6) << std::endl;
std::cout << ">=2jets: " << tt->GetBinContent(7) << std::endl;
std::cout << ">=3jets: " << tt->GetBinContent(8) << std::endl;
std::cout << ">=4jets: " << tt->GetBinContent(9) << std::endl;
std::cout << ">=1btag: " << tt->GetBinContent(10) << std::endl;
std::cout << ">=2btag: " << tt->GetBinContent(11) << std::endl;

}

TH1D* getSample(TString sample){
	TString dir = "../";
	TFile* tt_file = new TFile(dir + sample + "_5050pb_PFElectron_PFMuon_PF2PATJets_PFMET_TESTING.root");
	TDirectoryFile* tt_folder = (TDirectoryFile*) tt_file->Get("EventCount");
	TH1D* tt_cutflow = (TH1D*) tt_folder->Get("TTbarMuPlusJetsRefSelectionUnweighted");
    return tt_cutflow;
}

