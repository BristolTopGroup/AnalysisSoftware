/*
 * MuonAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/MuonAnalyser.h"

namespace BAT {

void MuonAnalyser::analyse(const EventPtr ttbarEvent) {

}

void MuonAnalyser::createHistograms() {

}

MuonAnalyser::MuonAnalyser(HistogramManagerPtr histMan, std::string histogramFolder ) :
	BasicAnalyser(histMan, histogramFolder) {

}

MuonAnalyser::~MuonAnalyser() {
}

} /* namespace BAT */
