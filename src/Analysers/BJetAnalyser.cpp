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
	double weight = event->weight();
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	const JetCollection allJets = event->Jets();

	for (unsigned int jetIndex = 0; jetIndex < allJets.size(); ++jetIndex) {
		const JetPointer jet(allJets.at(jetIndex));

		for (unsigned int algo = 0; algo < BtagAlgorithm::NUMBER_OF_BTAGALGORITHMS; ++algo) {
			string algorithm = BtagAlgorithm::names.at(algo);
			double disc = jet->getBTagDiscriminator((BtagAlgorithm::value) algo);

			histMan_->H1D(algorithm + "_allJets")->Fill(disc, weight);

			unsigned int partonFlavour = abs(jet->partonFlavour());
			if (partonFlavour == 5) {
				//partonFlavour 5 = b
				histMan_->H1D(algorithm + "_bQuarkJets")->Fill(disc, weight);
			} else if (partonFlavour == 4) {
				//partonFlavour 4 = c
				histMan_->H1D(algorithm + "_cQuarkJets")->Fill(disc, weight);
			} else if (partonFlavour == 1 || partonFlavour == 2 || partonFlavour == 3) {
				//partonFlavour 1 = d, 2 = u, 3 = s
				histMan_->H1D(algorithm + "_udsQuarkJets")->Fill(disc, weight);
			} else if (abs(jet->partonFlavour()) == 21) {
				//partonFlavour 21 = gluons
				histMan_->H1D(algorithm + "_gluonJets")->Fill(disc, weight);
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
			histMan_->addH1D(histname, histname, 1100, -1., 10.);
		}
	}
}

BJetAnalyser::BJetAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {

}

BJetAnalyser::~BJetAnalyser() {
}

} //end namespace BAT
