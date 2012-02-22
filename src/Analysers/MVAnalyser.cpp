/*
 * MVAnalyser.cpp
 *
 *  Created on: Nov 29, 2011
 *      Author: lkreczko
 */

#include "../../interface/Analysers/MVAnalyser.h"

namespace BAT {

MVAnalyser::MVAnalyser(HistogramManagerPtr histMan): BasicAnalyser(histMan) {
}

MVAnalyser::~MVAnalyser() {
}

void MVAnalyser::analyse(const TopPairEventCandidate& event)
{
	histMan->setCurrentCollection("MVAnalysis");
	histMan->H3D("Test")->FillRandom("gaus", 10);
}

void MVAnalyser::createHistograms() {
	histMan->setCurrentCollection("MVAnalysis");
	histMan->addH3D("Test", "Test", 10, 0., 10., 10, 0., 10., 10, 0., 10.);
}
}



