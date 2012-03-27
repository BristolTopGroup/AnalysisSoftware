/*
 * MVAnalyser.cpp
 *
 *  Created on: Nov 29, 2011
 *      Author: lkreczko
 */

#include "../../interface/Analysers/MVAnalyser.h"

namespace BAT {

MVAnalyser::MVAnalyser(HistogramManagerPtr histMan, std::string histogramFolder ) :
		BasicAnalyser(histMan, histogramFolder) {
}

MVAnalyser::~MVAnalyser() {
}

void MVAnalyser::analyse(const EventPtr event)
{
	histMan_->setCurrentHistogramFolder("MVAnalysis");
	histMan_->H3D("Test")->FillRandom("gaus", 10);
}

void MVAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder("MVAnalysis");
	histMan_->addH3D("Test", "Test", 10, 0., 10., 10, 0., 10., 10, 0., 10.);
}
}



