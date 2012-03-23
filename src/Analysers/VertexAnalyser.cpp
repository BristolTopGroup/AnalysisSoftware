/*
 * VertexAnalyser.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: lkreczko
 */

#include "../../interface/Analysers/VertexAnalyser.h"
#include "../../interface/TopPairEventCandidate.h"

namespace BAT {

void VertexAnalyser::analyse(const EventPtr event) {
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));

	histMan->setCurrentHistogramFolder("pileupStudy");

	double weight = ttbarCand->weight();
	double pileUpWeight = ttbarCand->PileUpWeight();

	if (pileUpWeight > 0) {
		histMan->H1D("nVertex")->Fill(ttbarCand->Vertices().size(), weight / pileUpWeight);
		histMan->H1D("nVertex_reweighted")->Fill(ttbarCand->Vertices().size(), weight);

		if (ttbarCand->passesFullTTbarEPlusJetSelection()) {
			if (ttbarCand->passesMETCut() && ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
				histMan->H1D_BJetBinned("nVertex_withMETAndAsymJets")->Fill(ttbarCand->Vertices().size(),
						weight / pileUpWeight);
				histMan->H1D_BJetBinned("nVertex_reweighted_withMETAndAsymJets")->Fill(ttbarCand->Vertices().size(),
						weight);
			}
		}
	}
}

void VertexAnalyser::createHistograms() {
	//create folder/collection name for the histograms
	// titles are in the format: histogram title; x-axis title; y-axis title
	histMan->setCurrentHistogramFolder("pileupStudy");
	histMan->addH1D("nVertex", "number of primary vertices; N(PV); events", 51, 0, 50);
	histMan->addH1D("nVertex_reweighted", "number of primary vertices; N(PV); events", 51, 0, 50);
	histMan->addH1D_BJetBinned("nVertex_reweighted_withMETAndAsymJets", "number of primary vertices; N(PV); events", 51,
			0, 50);
	histMan->addH1D_BJetBinned("nVertex_withMETAndAsymJets", "number of primary vertices; N(PV); events", 51, 0, 50);
}

VertexAnalyser::VertexAnalyser(HistogramManagerPtr histMan, boost::shared_ptr<EventWeightProvider> weights) :
		BasicAnalyser(histMan), //
		weights(weights) {

}

VertexAnalyser::~VertexAnalyser() {
}

}
