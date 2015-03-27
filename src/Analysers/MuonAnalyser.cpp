/*
 * MuonAnalyser.cpp
 *
 *  Created on: 3 Jul 2011
 *      Author: kreczko
 */
#include "../../interface/Analysers/MuonAnalyser.h"
#include "../../interface/GlobalVariables.h"
namespace BAT {

void MuonAnalyser::analyse(const EventPtr event) {
	if (singleMuonOnly_)
		return;
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	const MuonCollection muons = event->Muons();

	histMan_->H1D("Number_Of_Muons")->Fill(muons.size(), weight_);
	for (unsigned int index = 0; index < muons.size(); ++index) {
		const MuonPointer muon(muons.at(index));

		histMan_->H1D("All_Muon_Pt")->Fill(muon->pt(), weight_);
		histMan_->H1D("All_Muon_Eta")->Fill(muon->eta(), weight_);
		histMan_->H1D("All_Muon_AbsEta")->Fill(fabs(muon->eta()), weight_);
		histMan_->H1D("All_Muon_Phi")->Fill(muon->phi(), weight_);
		histMan_->H1D("All_Muon_pfIsolation_03")->Fill(muon->PFRelIso03(), weight_);
		histMan_->H1D("All_Muon_pfIsolation_04")->Fill(muon->PFRelIso04(), weight_);

		histMan_->H1D("All_Muon_dB")->Fill(muon->d0(), weight_);
	}
}

void MuonAnalyser::analyseMuon(const MuonPointer muon, double weight) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = weight * prescale_ * scale_;

	histMan_->H1D("muon_eta")->Fill(muon->eta(), weight_);
	histMan_->H1D("muon_AbsEta")->Fill(fabs(muon->eta()), weight_);
	histMan_->H1D("muon_pfIsolation_03_DeltaBeta")->Fill(muon->PFRelIso03DeltaBeta(), weight_);
	histMan_->H1D("muon_pfIsolation_04_DeltaBeta")->Fill(muon->PFRelIso04DeltaBeta(), weight_);

	if (!ttbarPlusMETAnalysisSetup_) {
		histMan_->H1D("muon_pT")->Fill(muon->pt(), weight_);
		histMan_->H1D("muon_phi")->Fill(muon->phi(), weight_);

		histMan_->H1D("muon_dB")->Fill(muon->d0(), weight_);
	}

	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->Fill("EventWeight", weight_ );
	treeMan_->Fill("pt", muon->pt() );
	treeMan_->Fill("eta", muon->eta() );	
	treeMan_->Fill("relIso_04_deltaBeta", muon->PFRelIso04DeltaBeta() );	
}

MuonAnalyser::MuonAnalyser(HistogramManagerPtr histMan, boost::shared_ptr<TreeManager> treeMan, std::string histogramFolder, bool singleMuonOnly) :
		BasicAnalyser(histMan, treeMan, histogramFolder), //
		singleMuonOnly_(singleMuonOnly), //
		ttbarPlusMETAnalysisSetup_(false) {

}

MuonAnalyser::~MuonAnalyser() {
}

void MuonAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	if (!ttbarPlusMETAnalysisSetup_) {
		//analyse all muons
		if (!singleMuonOnly_) {
			histMan_->addH1D("Number_Of_Muons", "Number of muons;N(#mu);Events ", 6, -0.5, 5.5);
			histMan_->addH1D("All_Muon_Pt", "Muon p_{T};p_{T}(#mu)/GeV;Events/2GeV ", 500, 0, 1000);
			histMan_->addH1D("All_Muon_Eta", "Muon #eta; #eta(#mu); Events/(0.02)", 300, -3, 3);
			histMan_->addH1D("All_Muon_AbsEta", "Muon |#eta|; |#eta(#mu)|; Events/(0.01)", 300, 0, 3);
			histMan_->addH1D("All_Muon_Phi", "Muon #phi; #phi(#mu); Events/(0.02)", 400, -4, 4);
			histMan_->addH1D("All_Muon_pfIsolation_03",
					"Muon relative pf isolation (DR=0.3); PF relative isolation; Events/(0.01)", 500, 0, 5);
			histMan_->addH1D("All_Muon_pfIsolation_04",
					"Muon relative pf isolation (DR=0.4); PF relative isolation; Events/(0.01)", 500, 0, 5);
			histMan_->addH1D("All_Muon_dB", "Muon dB(PV); dB/cm; Events/(0.001)", 200, 0, 0.2);
		}
		//single muon histograms for analyseMuon
		histMan_->addH1D("muon_pT", "Muon p_{T}; p_{T} (GeV); Events/(1 GeV)", 1000, 0, 1000);
		histMan_->addH1D("muon_phi", "Muon #phi; #phi(#mu); Events/(0.05)", 400, -4, 4);

		//b-jet binning useful for QCD estimates

		histMan_->addH1D("muon_pfIsolation_05",
				"Muon relative pf isolation (DR=0.5); PF relative isolation; Events/(0.01)", 500, 0, 5);

		histMan_->addH1D("muon_dB", "Muon dB(PV); dB/cm; Events/(0.001 cm)", 200, 0, 0.2);
	}
	histMan_->addH1D("muon_eta", "Muon #eta; #eta(#mu); Events/(0.02)", 300, -3, 3);
	histMan_->addH1D("muon_AbsEta", "Muon |#eta|; |#eta(#mu)|; Events/(0.01)", 300, 0, 3);
	histMan_->addH1D("muon_pfIsolation_03_DeltaBeta",
			"Muon relative pf isolation (DR=0.3); PF relative isolation; Events/(0.01)", 500, 0, 5);
	histMan_->addH1D("muon_pfIsolation_04_DeltaBeta",
			"Muon relative pf isolation (DR=0.4); PF relative isolation; Events/(0.01)", 500, 0, 5);

}

void MuonAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);

	treeMan_->addBranch("pt", "F", "Muons" + Globals::treePrefix_);
	treeMan_->addBranch("eta", "F", "Muons" + Globals::treePrefix_);	
	treeMan_->addBranch("relIso_04_deltaBeta", "F", "Muons" + Globals::treePrefix_);	
}

void MuonAnalyser::useTTbarPlusMETSetup(bool use) {
	ttbarPlusMETAnalysisSetup_ = use;
}

}
