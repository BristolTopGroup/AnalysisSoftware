/*
 * JESAnalyser.cpp
 *
 *  Created on: 24 Feb 2015
 *      Author: doburns
 */

#include "../../interface/Analysers/JESAnalyser.h"

#include <boost/scoped_ptr.hpp>
#include <iostream>

using namespace std;

namespace BAT {

void JESAnalyser::analyse(const EventPtr event) {	
	const JetCollection jets(event->CleanedJets());

	if ( !( event->PassesMuonTriggerAndSelection() || event->PassesElectronTriggerAndSelection() )) return;

	treeMan_->setCurrentFolder(histogramFolder_);

	for (unsigned int index = 0; index < jets.size(); ++index) {
		if (!jets.at(index)->matched_generated_jet()) continue;
		float pt, matched_pt, ratio;
		bool isBarrel, isB;
		isB = false;
		isBarrel = false;

		pt = jets.at(index)->pt();
		treeMan_->Fill("jetPt", pt );

		matched_pt = jets.at(index)->matched_generated_jet()->pt();
		treeMan_->Fill("matchedJetPt", matched_pt );

		ratio = pt / matched_pt;
		treeMan_->Fill("recoGenRatio", ratio );

		isB = jets.at(index)->isMedium();
		treeMan_->Fill("isB", isB );

		if (fabs(jets.at(index)->eta()) < 1.3) isBarrel=true;
		treeMan_->Fill("isBarrel", isBarrel );
	}
}


void JESAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->addBranch("jetPt", "F", "JESAnalyser", false);
	treeMan_->addBranch("matchedJetPt","F", "JESAnalyser", false);
	treeMan_->addBranch("recoGenRatio","F", "JESAnalyser", false);
	treeMan_->addBranch("isB","F", "JESAnalyser", false);
	treeMan_->addBranch("isBarrel","F", "JESAnalyser", false);
}

JESAnalyser::JESAnalyser(TreeManagerPtr treeMan, std::string histogramFolder) :
		BasicAnalyser(treeMan, histogramFolder) {
}

JESAnalyser::~JESAnalyser() {
}

} /* namespace BAT */
