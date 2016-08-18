/*
 * JetAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/JetAnalyser.h"

namespace BAT {

void JetAnalyser::analyse(const EventPtr event) {

	treeMan_->setCurrentFolder(histogramFolder_);
	
	weight_ = event->weight() * prescale_ * scale_;
	// const JetCollection jets = event->Jets();
	const JetCollection jets(event->CleanedJets());
	const JetCollection bjets(event->CleanedBJets());

	unsigned int numberOfBJets = event->NJets(bjets);
	unsigned int numberOfJets = event->NJets(jets);

	treeMan_->Fill("NJets", numberOfJets);
	treeMan_->Fill("NBJets", numberOfBJets);

	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet(jets.at(index));

		if (jet->pt() < 25 ) continue;

		if (index < 5) {
			stringstream temp;
			temp << "jet" << (index + 1);
			string nthJet = temp.str();

			treeMan_->Fill( nthJet + "_pt", jet->pt());
			treeMan_->Fill( nthJet + "_eta", jet->eta());
			treeMan_->Fill( nthJet + "_phi", jet->phi());
		}

		treeMan_->Fill("pt", jet->pt());
		treeMan_->Fill("eta", jet->eta());
		treeMan_->Fill("phi", jet->phi());
	}

	for (unsigned int index = 0; index < bjets.size(); ++index) {
		const JetPointer bJet(bjets.at(index));

		if ( bJet->pt() < 25 ) continue;

		treeMan_->Fill("bjet_pt", bJet->pt());
		treeMan_->Fill("bjet_eta", bJet->eta());
		treeMan_->Fill("bjet_phi", bJet->phi());
	}

	// if ( numberOfJets < 4 ) {
	// 	cout << "Fewer than 4 good cleaned jets with pt > 25 GeV : " << numberOfJets << endl;
	// 	cout << numberOfJets << " " << numberOfBJets << endl;
	// }


	treeMan_->Fill("EventWeight", weight_ );

	if ( event->PassesMuonTriggerAndSelection() ) {
		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::MuonPlusJetsReference );
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection( 0 );
		treeMan_->Fill("MuonEfficiencyCorrection", efficiencyCorrection);
	}
	else if ( event->PassesElectronTriggerAndSelection() ) {
		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::ElectronPlusJetsReference );
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));
		double efficiencyCorrection = event->isRealData() ? 1. : signalElectron->getEfficiencyCorrection( 0 );	
		treeMan_->Fill("ElectronEfficiencyCorrection", efficiencyCorrection);
	}
}

void JetAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->addBranch("pt", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("eta", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("phi", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("bjet_pt", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("bjet_eta", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("bjet_phi", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("NJets", "F", "Jets" + Globals::treePrefix_);
	treeMan_->addBranch("NBJets", "F", "Jets" + Globals::treePrefix_);

	treeMan_->addBranch("ElectronEfficiencyCorrection", "F", "Jets" + Globals::treePrefix_);
	treeMan_->addBranch("MuonEfficiencyCorrection", "F", "Jets" + Globals::treePrefix_);

	for (unsigned int index = 0; index < 5; ++index) {
		stringstream temp;
		temp << "jet" << (index + 1);
		string nthJet = temp.str();
		treeMan_->addBranch(nthJet + "_pt", "F", "Jets" + Globals::treePrefix_);
		treeMan_->addBranch(nthJet + "_eta", "F", "Jets" + Globals::treePrefix_);
		treeMan_->addBranch(nthJet + "_phi", "F", "Jets" + Globals::treePrefix_);
	}
}

JetAnalyser::JetAnalyser(boost::shared_ptr<TreeManager> treeMan, std::string histogramFolder) :
		BasicAnalyser(treeMan, histogramFolder) {

}

JetAnalyser::JetAnalyser(boost::shared_ptr<TreeManager> treeMan, unsigned int selectionCriteria, std::string histogramFolder) :
		BasicAnalyser(treeMan, histogramFolder),
		selection(SelectionCriteria::selection(selectionCriteria)) {

}

JetAnalyser::~JetAnalyser() {
}

} /* namespace BAT */
