/*
 * UnfoldingRecoAnalyser.cpp
 *
 *  Created on: 24 Feb 2015
 *      Author: ejclemen
 */

#include "../../interface/Analysers/UnfoldingRecoAnalyser.h"

#include <boost/scoped_ptr.hpp>
#include <iostream>

using namespace std;

namespace BAT {

void UnfoldingRecoAnalyser::analyse(const EventPtr event) {
	weight_ = event->weight();

	treeMan_->setCurrentFolder(histogramFolder_);

	int selectionCriteria = -1;

	if ( event->PassesElectronSelection() ) selectionCriteria = SelectionCriteria::ElectronPlusJetsReference;
	else if ( event->PassesMuonSelection() ) selectionCriteria = SelectionCriteria::MuonPlusJetsReference;


	if ( event->PassesElectronSelection() && event->PassesMuonSelection() ) {
		cout << "-------------> This should not happen <-------------" << endl;
	}
	// Store selection flags first
	treeMan_->Fill("passSelection",selectionCriteria);

	if ( !( event->PassesMuonSelection() || event->PassesElectronSelection() )) return;

	const JetCollection jets(event->getCleanedJets( selectionCriteria ));
	const LeptonPointer signalLepton = event->getSignalLepton( selectionCriteria );
	METAlgorithm::value metType = (METAlgorithm::value) 0;
	const METPointer met(event->MET(metType));

	treeMan_->Fill("MET",met->et());
	treeMan_->Fill("HT",Event::HT(jets));
	treeMan_->Fill("ST",Event::ST(jets,signalLepton,met));
	treeMan_->Fill("WPT",Event::WPT(signalLepton,met));
	treeMan_->Fill("MT",Event::MT(signalLepton,met));

}

void UnfoldingRecoAnalyser::createTrees() {

	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->addBranch("passSelection", "F", "Unfolding");

	treeMan_->addBranch("MET", "F", "Unfolding");
	treeMan_->addBranch("HT", "F", "Unfolding");
	treeMan_->addBranch("ST", "F", "Unfolding");
	treeMan_->addBranch("WPT", "F", "Unfolding");
	treeMan_->addBranch("MT", "F", "Unfolding");	
}

void UnfoldingRecoAnalyser::createHistograms() {

cout << "I shouldn't be here" << endl;

}

UnfoldingRecoAnalyser::UnfoldingRecoAnalyser(boost::shared_ptr<HistogramManager> histMan, TreeManagerPtr treeMan, std::string histogramFolder) :
		BasicAnalyser(histMan, treeMan, histogramFolder) {
}

UnfoldingRecoAnalyser::~UnfoldingRecoAnalyser() {
}

} /* namespace BAT */
