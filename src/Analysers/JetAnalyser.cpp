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
	weight_ = event->weight() * prescale_;
	const JetCollection jets = event->Jets();

	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet(jets.at(index));
		histMan_->H1D("jet_pT")->Fill(jet->pt(), weight_);
		histMan_->H1D("jet_phi")->Fill(jet->phi(), weight_);
		histMan_->H1D("jet_eta")->Fill(jet->eta(), weight_);
	}

	//Add NJets for TTbar selection
}

void JetAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	histMan_->addH1D("jet_pT", "Jet p_{T}; p_{T} (GeV); Events/(1 GeV)", 1000, 0, 1000);
	histMan_->addH1D("jet_phi", "Jet #phi; #phi(jet); Events/(0.05)", 400, -4, 4);
	histMan_->addH1D("jet_eta", "Jet #eta; #eta(jet); Events/(0.05)", 300, -3, 3);
}

JetAnalyser::JetAnalyser(boost::shared_ptr<HistogramManager> histMan, std::string histogramFolder ) :
		BasicAnalyser(histMan, histogramFolder) {

}

JetAnalyser::~JetAnalyser() {
}

} /* namespace BAT */
