/*
 * METAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/METAnalyser.h"

namespace BAT {

void METAnalyser::analyse(const EventPtr event) {

}

void METAnalyser::createHistograms() {

}

METAnalyser::METAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
	BasicAnalyser(histMan, histogramFolder) {

}

METAnalyser::~METAnalyser() {
}

} /* namespace BAT */
