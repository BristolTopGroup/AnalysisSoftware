/*
 * PileupAnalyser.cpp
 *
 *  Created on: 16 October 2015
 *      Author: ejclemen
 */

#include "../../interface/Analysers/PileupAnalyser.h"
#include <iostream>

using namespace BAT;
using namespace std;

void PileupAnalyser::analyse(const EventPtr event) {

	treeMan_->setCurrentFolder(histogramFolder_);

	treeMan_->Fill("EventWeight", event->weight());
	treeMan_->Fill("NVertices",	event->Vertices().size());
	treeMan_->Fill("PUWeight", event->PileUpWeight());

	// Get lepton scale factor
	int selectionCriteria = -1;
	if ( event->PassesElectronTriggerAndSelectionNoB() ) selectionCriteria = SelectionCriteria::ElectronPlusJetsReference;
	else if ( event->PassesMuonTriggerAndSelectionNoB() ) selectionCriteria = SelectionCriteria::MuonPlusJetsReference;
	const LeptonPointer signalLepton = event->getSignalLepton( selectionCriteria );
	if ( event->PassesMuonTriggerAndSelectionNoB() ) {
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection = signalMuon->getEfficiencyCorrection( 0 );
		treeMan_->Fill("LeptonEfficiencyCorrection", efficiencyCorrection);
	}
	else if ( event->PassesElectronTriggerAndSelectionNoB() ) {
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));
		double efficiencyCorrection = signalElectron->getEfficiencyCorrection( 0 );	
		treeMan_->Fill("LeptonEfficiencyCorrection", efficiencyCorrection);
	}
}

PileupAnalyser::PileupAnalyser(TreeManagerPtr treeMan, std::string histogramFolder) :
	BasicAnalyser(treeMan, histogramFolder) {
}

PileupAnalyser::~PileupAnalyser() {
}

void PileupAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);

	treeMan_->addBranch("NVertices", "F", "Pileup" + Globals::treePrefix_);
	treeMan_->addBranch("PUWeight", "F", "Pileup" + Globals::treePrefix_);
	treeMan_->addBranch("LeptonEfficiencyCorrection", "F", "Pileup" + Globals::treePrefix_);
}
/* namespace BAT */
