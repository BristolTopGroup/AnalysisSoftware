/*
 * VertexAnalyser.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: lkreczko
 */

#include "../../interface/Analysers/VertexAnalyser.h"

namespace BAT {

void VertexAnalyser::analyse(const TopPairEventCandidate& ttbarEvent) {
	histMan->setCurrentCollection("pileupStudy");

	double weight = ttbarEvent.weight();
	double pileUpWeight = ttbarEvent.PileUpWeight();

	if (pileUpWeight > 0) {
		histMan->H1D("nVertex")->Fill(ttbarEvent.Vertices().size(), weight / pileUpWeight);
		histMan->H1D("nVertex_reweighted")->Fill(ttbarEvent.Vertices().size(), weight);

		if (ttbarEvent.passesFullTTbarEPlusJetSelection()) {
			if (ttbarEvent.passesMETCut() && ttbarEvent.passesAsymmetricElectronCleanedJetCuts()) {
				histMan->H1D_BJetBinned("nVertex_withMETAndAsymJets")->Fill(ttbarEvent.Vertices().size(), weight
						/ pileUpWeight);
				histMan->H1D_BJetBinned("nVertex_reweighted_withMETAndAsymJets")->Fill(ttbarEvent.Vertices().size(),
						weight);
			}
		}
	}
}

void VertexAnalyser::createHistograms() {
	//create folder/collection name for the histograms
	// titles are in the format: histogram title; x-axis title; y-axis title
	histMan->setCurrentCollection("pileupStudy");
	histMan->addH1D("nVertex", "number of primary vertices; N(PV); events", 51, 0, 50);
	histMan->addH1D("nVertex_reweighted", "number of primary vertices; N(PV); events", 51, 0, 50);
	histMan->addH1D_BJetBinned("nVertex_reweighted_withMETAndAsymJets", "number of primary vertices; N(PV); events",
			51, 0, 50);
	histMan->addH1D_BJetBinned("nVertex_withMETAndAsymJets", "number of primary vertices; N(PV); events", 51, 0, 50);
}

VertexAnalyser::VertexAnalyser(boost::shared_ptr<HistogramManager> histMan,
		boost::shared_ptr<EventWeightProvider> weights) :
	BasicAnalyser(histMan),
	weights(weights) {

}

VertexAnalyser::~VertexAnalyser() {
}

}
