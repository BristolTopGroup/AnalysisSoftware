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
	weight_ = event->weight() * prescale_ * scale_;
	const MuonCollection muons = event->Muons();
}

void MuonAnalyser::analyseMuon(const MuonPointer muon, double weight) {
	weight_ = weight * prescale_ * scale_;

	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->Fill("EventWeight", weight_ );
	treeMan_->Fill("pt", muon->pt() );
	treeMan_->Fill("eta", muon->eta() );	
	treeMan_->Fill("phi", muon->phi() );	
	treeMan_->Fill("relIso_04_deltaBeta", muon->PFRelIso04DeltaBeta() );

	treeMan_->Fill("relTrkIso", muon->relTrkIso() );

	double efficiencyCorrection = muon->getEfficiencyCorrection( 0 );	
	treeMan_->Fill("MuonEfficiencyCorrection", efficiencyCorrection);
}

MuonAnalyser::MuonAnalyser(boost::shared_ptr<TreeManager> treeMan, std::string histogramFolder, bool singleMuonOnly) :
		BasicAnalyser(treeMan, histogramFolder), //
		singleMuonOnly_(singleMuonOnly), //
		ttbarPlusMETAnalysisSetup_(false) {

}

MuonAnalyser::~MuonAnalyser() {
}

void MuonAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);

	treeMan_->addBranch("pt", "F", "Muons" + Globals::treePrefix_);
	treeMan_->addBranch("eta", "F", "Muons" + Globals::treePrefix_);	
	treeMan_->addBranch("phi", "F", "Muons" + Globals::treePrefix_);	
	treeMan_->addBranch("relIso_04_deltaBeta", "F", "Muons" + Globals::treePrefix_);
	treeMan_->addBranch("relTrkIso", "F", "Muons" + Globals::treePrefix_);
	treeMan_->addBranch("MuonEfficiencyCorrection", "F", "Muons" + Globals::treePrefix_);
}

void MuonAnalyser::useTTbarPlusMETSetup(bool use) {
	ttbarPlusMETAnalysisSetup_ = use;
}

}
