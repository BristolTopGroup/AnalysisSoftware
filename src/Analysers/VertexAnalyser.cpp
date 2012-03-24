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
	double lumiWeightOnly = weight / pileUpWeight;

	if (pileUpWeight > 0) {
		histMan->H1D("nVertex")->Fill(ttbarCand->Vertices().size(), lumiWeightOnly);
		histMan->H1D("nVertex_reweighted")->Fill(ttbarCand->Vertices().size(), weight);

		histMan->H1D("nVertex_reweighted_PUWeightInTimeOnly")->Fill(ttbarCand->Vertices().size(),
				lumiWeightOnly * event->PUWeightInTimeOnly());
		histMan->H1D("nVertex_reweighted_PUWeight3BX")->Fill(ttbarCand->Vertices().size(),
				lumiWeightOnly * event->PUWeight3BX());
		histMan->H1D("nVertex_reweighted_PUWeight3D")->Fill(ttbarCand->Vertices().size(),
				lumiWeightOnly * event->PUWeight3D());

		if (ttbarCand->passesFullTTbarEPlusJetSelection()) {
			if (ttbarCand->passesMETCut() && ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
				histMan->H1D_BJetBinned("nVertex_withMETAndAsymJets")->Fill(ttbarCand->Vertices().size(),
						lumiWeightOnly);
				histMan->H1D_BJetBinned("nVertex_reweighted_withMETAndAsymJets")->Fill(ttbarCand->Vertices().size(),
						weight);

				histMan->H1D_BJetBinned("nVertex_reweighted_PUWeightInTimeOnly_withMETAndAsymJets")->Fill(
						ttbarCand->Vertices().size(), lumiWeightOnly * event->PUWeightInTimeOnly());
				histMan->H1D_BJetBinned("nVertex_reweighted_PUWeight3BX_withMETAndAsymJets")->Fill(ttbarCand->Vertices().size(),
						lumiWeightOnly * event->PUWeight3BX());
				histMan->H1D_BJetBinned("nVertex_reweighted_PUWeight3D_withMETAndAsymJets")->Fill(ttbarCand->Vertices().size(),
						lumiWeightOnly * event->PUWeight3D());
			}
		}
	}
}

void VertexAnalyser::createHistograms() {
	//create folder/collection name for the histograms
	// titles are in the format: histogram title; x-axis title; y-axis title
	histMan->setCurrentHistogramFolder("pileupStudy");
	//as they come out of simulation
	histMan->addH1D("nVertex", "number of primary vertices; N(PV); events", 51, 0, 50);
	histMan->addH1D_BJetBinned("nVertex_withMETAndAsymJets", "number of primary vertices; N(PV); events", 51, 0, 50);

	//reweighted
	histMan->addH1D("nVertex_reweighted", "number of primary vertices; N(PV); events", 51, 0, 50);
	histMan->addH1D("nVertex_reweighted_PUWeightInTimeOnly", "number of primary vertices; N(PV); events", 51, 0, 50);
	histMan->addH1D("nVertex_reweighted_PUWeight3BX", "number of primary vertices; N(PV); events", 51, 0, 50);
	histMan->addH1D("nVertex_reweighted_PUWeight3D", "number of primary vertices; N(PV); events", 51, 0, 50);

	//after full selection
	histMan->addH1D_BJetBinned("nVertex_reweighted_withMETAndAsymJets", "number of primary vertices; N(PV); events", 51,
			0, 50);
	histMan->addH1D_BJetBinned("nVertex_reweighted_PUWeightInTimeOnly_withMETAndAsymJets",
			"number of primary vertices; N(PV); events", 51, 0, 50);
	histMan->addH1D_BJetBinned("nVertex_reweighted_PUWeight3BX_withMETAndAsymJets",
			"number of primary vertices; N(PV); events", 51, 0, 50);
	histMan->addH1D_BJetBinned("nVertex_reweighted_PUWeight3D_withMETAndAsymJets",
			"number of primary vertices; N(PV); events", 51, 0, 50);

}

VertexAnalyser::VertexAnalyser(HistogramManagerPtr histMan, boost::shared_ptr<EventWeightProvider> weights) :
		BasicAnalyser(histMan), //
		weights(weights) {

}

VertexAnalyser::~VertexAnalyser() {
}

}
