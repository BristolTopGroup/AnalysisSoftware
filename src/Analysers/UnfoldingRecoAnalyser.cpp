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


	// Store pseudo ST
	// treeMan_->Fill("pseudoST", event->ST( pseudoJets, pseudoLepton, METPointer( new MET( pseudoMET->px(), pseudoMET->py() )) ) );
}

void UnfoldingRecoAnalyser::createTrees() {

	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->addBranch("passSelection","passSelection","Unfolding");

	treeMan_->addBranch("MET","MET","Unfolding");
	treeMan_->addBranch("HT","HT","Unfolding");
	treeMan_->addBranch("ST","ST","Unfolding");
	treeMan_->addBranch("WPT","WPT","Unfolding");
	treeMan_->addBranch("MT","MT","Unfolding");	

	// treeMan_->addBranch("pseudoST","pseudoST","Unfolding");
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
