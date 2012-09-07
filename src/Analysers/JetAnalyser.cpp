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
	weight_ = event->weight() * prescale_ * scale_;
	const JetCollection jets = event->Jets();

	unsigned int numberOfBJets(0);
	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet(jets.at(index));
		histMan_->H1D_BJetBinned("all_jet_pT")->Fill(jet->pt(), weight_);
		histMan_->H1D_BJetBinned("all_jet_phi")->Fill(jet->phi(), weight_);
		histMan_->H1D_BJetBinned("all_jet_eta")->Fill(jet->eta(), weight_);
		if (jet->isBJet(BtagAlgorithm::CombinedSecondaryVertex, BtagAlgorithm::MEDIUM))
			++numberOfBJets;

		if (index < 7) {
			stringstream temp;
			temp << "jet" << (index + 1);
			string nthJet = temp.str();
			histMan_->H1D_BJetBinned(nthJet + "_pT")->Fill(jet->pt(), weight_);
			histMan_->H1D_BJetBinned(nthJet + "_phi")->Fill(jet->phi(), weight_);
			histMan_->H1D_BJetBinned(nthJet + "_eta")->Fill(jet->eta(), weight_);
		}

	}
	histMan_->H1D_BJetBinned("N_Jets")->Fill(jets.size(), weight_);
	histMan_->H1D("N_BJets")->Fill(numberOfBJets, weight_);
}

void JetAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	histMan_->addH1D_BJetBinned("all_jet_pT", "Jet p_{T}; p_{T} (GeV); Events/(1 GeV)", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("all_jet_phi", "Jet #phi; #phi(jet); Events/(0.05)", 400, -4, 4);
	histMan_->addH1D_BJetBinned("all_jet_eta", "Jet #eta; #eta(jet); Events/(0.05)", 300, -3, 3);
	for (unsigned int index = 0; index < 7; ++index) {
		stringstream temp;
		temp << "jet" << (index + 1);
		string nthJet = temp.str();
		histMan_->addH1D_BJetBinned(nthJet + "_pT", nthJet + " p_{T}; p_{T} (GeV); Events/(1 GeV)", 1000, 0, 1000);
		histMan_->addH1D_BJetBinned(nthJet + "_phi", nthJet + " #phi; #phi(jet); Events/(0.05)", 400, -4, 4);
		histMan_->addH1D_BJetBinned(nthJet + "_eta", nthJet + " #eta; #eta(jet); Events/(0.05)", 300, -3, 3);
	}
	histMan_->addH1D_BJetBinned("N_Jets", "# of Jets; # of Jets; Events", 21, -0.5, 20.5);
	histMan_->addH1D("N_BJets", "# of b-Jets; # of b-Jet; Events", 11, -0.5, 10.5);
}

JetAnalyser::JetAnalyser(boost::shared_ptr<HistogramManager> histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {

}

JetAnalyser::~JetAnalyser() {
}

} /* namespace BAT */
