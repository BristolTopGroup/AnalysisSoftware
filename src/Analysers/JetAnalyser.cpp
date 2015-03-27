/*
 * JetAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/JetAnalyser.h"

namespace BAT {

void JetAnalyser::analyse(const EventPtr event) {

	histMan_->setCurrentHistogramFolder(histogramFolder_);
	treeMan_->setCurrentFolder(histogramFolder_);
	
	weight_ = event->weight() * prescale_ * scale_;
	// const JetCollection jets = event->Jets();
	const JetCollection jets(event->CleanedJets());
	const JetCollection bjets(event->CleanedBJets());

	unsigned int numberOfBJets(bjets.size());
	unsigned int numberOfBJets_ptG30(0);
	unsigned int numberOfJets(0);
	unsigned int numberOfJets_ptG30(0);

	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet(jets.at(index));

		histMan_->H1D("all_jet_pT")->Fill(jet->pt(), weight_);
		histMan_->H1D("all_jet_phi")->Fill(jet->phi(), weight_);
		histMan_->H1D("all_jet_eta")->Fill(jet->eta(), weight_);
		++numberOfJets;

		if ( jet->pt() <= 30 ) continue;

		histMan_->H1D("all_jet_pT_ptG30")->Fill(jet->pt(), weight_);
		histMan_->H1D("all_jet_phi_ptG30")->Fill(jet->phi(), weight_);
		histMan_->H1D("all_jet_eta_ptG30")->Fill(jet->eta(), weight_);

		++numberOfJets_ptG30;

		// if (index < 7) {
		// 	stringstream temp;
		// 	temp << "jet" << (index + 1);
		// 	string nthJet = temp.str();
		// 	histMan_->H1D(nthJet + "_pT")->Fill(jet->pt(), weight_);
		// 	histMan_->H1D(nthJet + "_phi")->Fill(jet->phi(), weight_);
		// 	histMan_->H1D(nthJet + "_eta")->Fill(jet->eta(), weight_);
		// }

		treeMan_->Fill("pt", jet->pt());
		treeMan_->Fill("eta", jet->eta());
		treeMan_->Fill("phi", jet->phi());
	}

	for (unsigned int index = 0; index < bjets.size(); ++index) {
		const JetPointer bJet(bjets.at(index));

		histMan_->H1D("b_jet_pT")->Fill(bJet->pt(), weight_);
		histMan_->H1D("b_jet_phi")->Fill(bJet->phi(), weight_);
		histMan_->H1D("b_jet_eta")->Fill(bJet->eta(), weight_);

		if ( bJet->pt() <=30 ) continue;
		++numberOfBJets_ptG30;
		histMan_->H1D("b_jet_pT_ptG30")->Fill(bJet->pt(), weight_);
		histMan_->H1D("b_jet_phi_ptG30")->Fill(bJet->phi(), weight_);
		histMan_->H1D("b_jet_eta_ptG30")->Fill(bJet->eta(), weight_);
	}

	histMan_->H1D("N_Jets")->Fill(numberOfJets, weight_);
	histMan_->H1D("N_Jets_ptG30")->Fill(numberOfJets_ptG30, weight_);
	histMan_->H1D("N_BJets")->Fill(numberOfBJets, weight_);
	histMan_->H1D("N_BJets_ptG30")->Fill(numberOfBJets, weight_);

	if ( numberOfJets < 4 ) {
		cout << "Fewer than 4 good cleaned jets with pt > 30 GeV : " << numberOfJets << endl;
		cout << numberOfJets << " " << numberOfJets_ptG30 << " " << numberOfBJets << " " << numberOfBJets_ptG30 << endl;
	}

	treeMan_->Fill("NJets", numberOfJets);
	treeMan_->Fill("NBJets", numberOfBJets);
	treeMan_->Fill("EventWeight", weight_ );
}

void JetAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	histMan_->addH1D("all_jet_pT", "Jet p_{T}; p_{T} (GeV); Events/(1 GeV)", 1000, 0, 1000);
	histMan_->addH1D("all_jet_phi", "Jet #phi; #phi(jet); Events/(0.05)", 400, -4, 4);
	histMan_->addH1D("all_jet_eta", "Jet #eta; #eta(jet); Events/(0.05)", 300, -3, 3);

	histMan_->addH1D("all_jet_pT_ptG30", "Jet p_{T}; p_{T} (GeV); Events/(1 GeV)", 1000, 0, 1000);
	histMan_->addH1D("all_jet_phi_ptG30", "Jet #phi; #phi(jet); Events/(0.05)", 400, -4, 4);
	histMan_->addH1D("all_jet_eta_ptG30", "Jet #eta; #eta(jet); Events/(0.05)", 300, -3, 3);

	histMan_->addH1D("b_jet_pT", "B jet p_{T}; p_{T} (GeV); Events/(1 GeV)", 1000, 0, 1000);
	histMan_->addH1D("b_jet_phi", "B jet #phi; #phi(b jet); Events/(0.05)", 400, -4, 4);
	histMan_->addH1D("b_jet_eta", "B jet #eta; #eta(b jet); Events/(0.05)", 300, -3, 3);


	histMan_->addH1D("b_jet_pT_ptG30", "B jet p_{T}; p_{T} (GeV); Events/(1 GeV)", 1000, 0, 1000);
	histMan_->addH1D("b_jet_phi_ptG30", "B jet #phi; #phi(b jet); Events/(0.05)", 400, -4, 4);
	histMan_->addH1D("b_jet_eta_ptG30", "B jet #eta; #eta(b jet); Events/(0.05)", 300, -3, 3);

	// for (unsigned int index = 0; index < 7; ++index) {
	// 	stringstream temp;
	// 	temp << "jet" << (index + 1);
	// 	string nthJet = temp.str();
	// 	histMan_->addH1D(nthJet + "_pT", nthJet + " p_{T}; p_{T} (GeV); Events/(1 GeV)", 1000, 0, 1000);
	// 	histMan_->addH1D(nthJet + "_phi", nthJet + " #phi; #phi(jet); Events/(0.05)", 400, -4, 4);
	// 	histMan_->addH1D(nthJet + "_eta", nthJet + " #eta; #eta(jet); Events/(0.05)", 300, -3, 3);
	// }

	histMan_->addH1D("N_Jets", "# of Jets; # of Jets; Events", 21, -0.5, 20.5);
	histMan_->addH1D("N_Jets_ptG30", "# of Jets; # of Jets; Events", 21, -0.5, 20.5);
	histMan_->addH1D("N_BJets", "# of b-Jets; # of b-Jet; Events", 11, -0.5, 10.5);
	histMan_->addH1D("N_BJets_ptG30", "# of b-Jets; # of b-Jet; Events", 11, -0.5, 10.5);

}

void JetAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->addBranch("pt", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("eta", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("phi", "F", "Jets" + Globals::treePrefix_, false);
	treeMan_->addBranch("NJets", "F", "Jets" + Globals::treePrefix_);
	treeMan_->addBranch("NBJets", "F", "Jets" + Globals::treePrefix_);

}

JetAnalyser::JetAnalyser(boost::shared_ptr<HistogramManager> histMan, boost::shared_ptr<TreeManager> treeMan, std::string histogramFolder) :
		BasicAnalyser(histMan, treeMan, histogramFolder) {

}

JetAnalyser::JetAnalyser(boost::shared_ptr<HistogramManager> histMan, boost::shared_ptr<TreeManager> treeMan, unsigned int selectionCriteria, std::string histogramFolder) :
		BasicAnalyser(histMan, treeMan, histogramFolder),
		selection(SelectionCriteria::selection(selectionCriteria)) {

}

JetAnalyser::~JetAnalyser() {
}

} /* namespace BAT */
