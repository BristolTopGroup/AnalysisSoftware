/*
 * VertexAnalyser.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: lkreczko
 */

#include "../../interface/Analysers/VertexAnalyser.h"
//#include "../../interface/TopPairEventCandidate.h"

namespace BAT {

void VertexAnalyser::analyse(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	double weight = event->weight() * prescale_ * scale_;
	double pileUpWeight = event->PileUpWeight();
	double lumiWeightOnly = weight / pileUpWeight;
	unsigned int nVertices = event->Vertices().size();

	double PUWeightInTimeOnly(event->PUWeightInTimeOnly());
	double PUWeight3BX(event->PUWeight3BX());
//	double PUWeight3D(event->PUWeight3D());

	if (pileUpWeight > 0) { //otherwise lumiWeightOnly infinite
		histMan_->H1D("nVertex")->Fill(nVertices, lumiWeightOnly);
		histMan_->H1D("nVertex_reweighted")->Fill(nVertices, weight);

		histMan_->H1D("nVertex_reweighted_PUWeightInTimeOnly")->Fill(nVertices, lumiWeightOnly * PUWeightInTimeOnly);
		histMan_->H1D("nVertex_reweighted_PUWeight3BX")->Fill(nVertices, lumiWeightOnly * PUWeight3BX);
//		histMan_->H1D("nVertex_reweighted_PUWeight3D")->Fill(nVertices, lumiWeightOnly * PUWeight3D);
	}
}

void VertexAnalyser::createHistograms() {
	// titles are in the format: histogram title; x-axis title; y-axis title
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	//as they come out of simulation
	histMan_->addH1D("nVertex", "number of primary vertices; N(PV); events", 51, 0, 50);
	//reweighted
	histMan_->addH1D("nVertex_reweighted", "number of primary vertices; N(PV); events", 51, 0, 50);
	histMan_->addH1D("nVertex_reweighted_PUWeightInTimeOnly", "number of primary vertices; N(PV); events", 51, 0, 50);
	histMan_->addH1D("nVertex_reweighted_PUWeight3BX", "number of primary vertices; N(PV); events", 51, 0, 50);
//	histMan_->addH1D("nVertex_reweighted_PUWeight3D", "number of primary vertices; N(PV); events", 51, 0, 50);
}

VertexAnalyser::VertexAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {

}

VertexAnalyser::~VertexAnalyser() {
}

}
