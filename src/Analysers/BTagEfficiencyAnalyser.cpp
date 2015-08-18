/*
 * BTagEff.cpp
 *
 *  Created on: 23 Jul 2015
 *      Author: burns
 */

#include "../../interface/Analysers/BTagEfficiencyAnalyser.h"

using namespace BAT;

void BTagEff::analyse(const EventPtr event) {

	histMan_->setCurrentHistogramFolder(histogramFolder_);
	treeMan_->setCurrentFolder(histogramFolder_);
	int NJets = 0;
	const JetCollection allJets = event->Jets();

	for (unsigned int jetIndex = 0; jetIndex < allJets.size(); ++jetIndex) {
		const JetPointer jet(allJets.at(jetIndex));

		bool isLoose = false;
		bool isMedium = false;
		bool isTight = false;

		double jetPt = jet->pt();
		double jetEta = jet->eta();

		if (jetPt < 25 || abs(jetEta) > 2.4) continue;
		// double jetCSV = jet->getBTagDiscriminator(BtagAlgorithm::CombinedSecondaryVertexV2, BtagAlgorithm::MEDIUM);
		double jetCSV = jet->getBTagDiscriminator(BAT::BtagAlgorithm::value::CombinedSecondaryVertexV2);

		// https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation74X50ns
		if (jetCSV > 0.605) isLoose = true;
		if (jetCSV > 0.890) isMedium = true;
		if (jetCSV > 0.970) isTight = true;

		unsigned int partonFlavour = abs(jet->partonFlavour());
		// const bool isBTagged = jet->isBJet(BtagAlgorithm::CombinedSecondaryVertexV2, BtagAlgorithm::MEDIUM);
		// cout << jet->isBJet(BtagAlgorithm::CombinedSecondaryVertexV2, BtagAlgorithm::MEDIUM) << endl;
		
		treeMan_->Fill("pt", jetPt);
		treeMan_->Fill("eta", jetEta);
		treeMan_->Fill("CSV", jetCSV);
		treeMan_->Fill("partonFlavour", partonFlavour);
		treeMan_->Fill("isLoose", isLoose);
		treeMan_->Fill("isMedium", isMedium);
		treeMan_->Fill("isTight", isTight);
		++NJets;
	}


	treeMan_->Fill("NJets", NJets);
}


BTagEff::BTagEff(HistogramManagerPtr histMan, TreeManagerPtr treeMan, std::string histogramFolder) :
	BasicAnalyser(histMan, treeMan, histogramFolder) {
}

BTagEff::~BTagEff() {
}

void BTagEff::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
}

void BTagEff::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);

	treeMan_->addBranch("pt", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("eta", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("CSV", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("partonFlavour", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("isLoose", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("isMedium", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("isTight", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("NJets", "F", "Jets" + Globals::treePrefix_);
}
/* namespace BAT */
