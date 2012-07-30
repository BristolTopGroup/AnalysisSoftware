/*
 * BJetAnalyser.cpp
 *
 *  Created on: 3 Jul 2011
 *      Author: J. Jacob
 *      Modified: L . Kreczko
 */
#include "../../interface/Analysers/BJetAnalyser.h"
#include "../../interface/GlobalVariables.h"

namespace BAT {

const boost::array<std::string, 5> JETTYPES = { { "allJets", //
		"bQuarkJets", //
		"cQuarkJets", //
		"udsQuarkJets", //
		"gluonJets" } };

void BJetAnalyser::analyse(const EventPtr event) {
	weight_ = event->weight() * prescale_ * scale_;
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	const JetCollection allJets = event->Jets();

	for (unsigned int jetIndex = 0; jetIndex < allJets.size(); ++jetIndex) {
		const JetPointer jet(allJets.at(jetIndex));

		for (unsigned int algo = 0; algo < BtagAlgorithm::NUMBER_OF_BTAGALGORITHMS; ++algo) {
			string algorithm = BtagAlgorithm::names.at(algo);
			double disc = jet->getBTagDiscriminator((BtagAlgorithm::value) algo);

			histMan_->H1D(algorithm + "_allJets")->Fill(disc, weight_);

			unsigned int partonFlavour = abs(jet->partonFlavour());
			if (partonFlavour == 5) {
				//partonFlavour 5 = b
				histMan_->H1D(algorithm + "_bQuarkJets")->Fill(disc, weight_);
			} else if (partonFlavour == 4) {
				//partonFlavour 4 = c
				histMan_->H1D(algorithm + "_cQuarkJets")->Fill(disc, weight_);
			} else if (partonFlavour == 1 || partonFlavour == 2 || partonFlavour == 3) {
				//partonFlavour 1 = d, 2 = u, 3 = s
				histMan_->H1D(algorithm + "_udsQuarkJets")->Fill(disc, weight_);
			} else if (abs(jet->partonFlavour()) == 21) {
				//partonFlavour 21 = gluons
				histMan_->H1D(algorithm + "_gluonJets")->Fill(disc, weight_);
			}
		}
	}

}

void BJetAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	for (unsigned int algo = 0; algo < BtagAlgorithm::NUMBER_OF_BTAGALGORITHMS; ++algo) {
		for (unsigned int index = 0; index < JETTYPES.size(); ++index) {
			string jettype = JETTYPES.at(index);
			string algorithm = BtagAlgorithm::names.at(algo);
			string histname = algorithm + "_" + jettype;
			if (algorithm == "CombinedSecondaryVertex") {
				histMan_->addH1D(histname, histname, 200, -0.5, 1.5);
			} else if (algorithm == "CombinedSecondaryVertexMVA") {
				histMan_->addH1D(histname, histname, 200, -0.5, 1.5);
			} else if (algorithm == "JetBProbability") {
				histMan_->addH1D(histname, histname, 1050, -0.5, 10.);
			} else if (algorithm == "JetProbability") {
				histMan_->addH1D(histname, histname, 1050, -0.5, 10.);
			} else if (algorithm == "SimpleSecondaryVertexHighEfficiency") {
				histMan_->addH1D(histname, histname, 600, 0., 6.);
			} else if (algorithm == "SimpleSecondaryVertexHighPurity") {
				histMan_->addH1D(histname, histname, 600, 0., 6.);
			} else if (algorithm == "SoftMuon") {
				histMan_->addH1D(histname, histname, 600, -5., 1.);
			} else if (algorithm == "SoftMuonByIP3d") {
				histMan_->addH1D(histname, histname, 27000, -120., 150.);
			} else if (algorithm == "SoftMuonByPt") {
				histMan_->addH1D(histname, histname, 5500, -5., 50.);
			} else if (algorithm == "TrackCountingHighEfficiency") {
				histMan_->addH1D(histname, histname, 17000, -70., 100.);
			} else if (algorithm == "TrackCountingHighPurity") {
				histMan_->addH1D(histname, histname, 17000, -70., 100.);
			}
		}
	}
}

BJetAnalyser::BJetAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {

}

BJetAnalyser::~BJetAnalyser() {
}

} //end namespace BAT
