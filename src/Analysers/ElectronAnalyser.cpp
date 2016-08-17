/*
 * ElectronAnalyser.cpp
 *
 *  Created on: 3 Jul 2011
 *      Author: kreczko
 */
#include "../../interface/Analysers/ElectronAnalyser.h"
namespace BAT {

void ElectronAnalyser::analyse(const EventPtr event) {
	weight_ = event->weight() * prescale_ * scale_;
	const ElectronCollection electrons = event->Electrons();
	if (singleElectronOnly_)
		return;
}

void ElectronAnalyser::analyseElectron(const ElectronPointer electron, double weight) {
	weight_ = weight * prescale_ * scale_;
	if (!singleElectronOnly_)
		return;

	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->Fill("EventWeight", weight_ );

	treeMan_->Fill("pt", electron->pt() );
	treeMan_->Fill("eta", electron->eta() );	
	treeMan_->Fill("phi", electron->phi() );	
	treeMan_->Fill("etaSC", electron->superClusterEta() );
	treeMan_->Fill("relIso_03_deltaBeta", electron->PFRelIso03DeltaBeta() );

	double efficiencyCorrection = electron->getEfficiencyCorrection( 0 );	
	treeMan_->Fill("ElectronEfficiencyCorrection", efficiencyCorrection);

}

ElectronAnalyser::ElectronAnalyser(boost::shared_ptr<TreeManager> treeMan, std::string histogramFolder, bool singleElectronOnly) :
		BasicAnalyser(treeMan, histogramFolder), //
		singleElectronOnly_(singleElectronOnly), //
		ttbarPlusMETAnalysisSetup_(false) {

}

ElectronAnalyser::~ElectronAnalyser() {
}

void ElectronAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);

	treeMan_->addBranch("pt", "F", "Electrons" + Globals::treePrefix_);
	treeMan_->addBranch("eta", "F", "Electrons" + Globals::treePrefix_);
	treeMan_->addBranch("phi", "F", "Electrons" + Globals::treePrefix_);
	treeMan_->addBranch("etaSC", "F", "Electrons" + Globals::treePrefix_);
	treeMan_->addBranch("relIso_03_deltaBeta", "F", "Electrons" + Globals::treePrefix_);

	treeMan_->addBranch("ElectronEfficiencyCorrection", "F", "Electrons" + Globals::treePrefix_);
}

void ElectronAnalyser::useTTbarPlusMETSetup(bool use) {
	ttbarPlusMETAnalysisSetup_ = use;
}

}
