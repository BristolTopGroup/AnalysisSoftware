/*
 * BasicAnalyser.cpp
 *
 *  Created on: 3 Jul 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/BasicAnalyser.h"

namespace BAT {
BasicAnalyser::BasicAnalyser(HistogramManagerPtr histMan,
		std::string histogramFolder) :
		histMan_(histMan), //
		tTreeMan_(), //
		histogramFolder_(histogramFolder), //
		weight_(1.), //
		prescale_(1), //
		scale_(1.), //
		have_histograms_been_created_(false) {

}

BasicAnalyser::BasicAnalyser(HistogramManagerPtr histMan,
		TTreeManagerPtr tTreeMan,
		std::string histogramFolder) :
		histMan_(histMan), //
		tTreeMan_(tTreeMan), //
		histogramFolder_(histogramFolder), //
		weight_(1.), //
		prescale_(1), //
		scale_(1.), //
		have_histograms_been_created_(false) {

}

BasicAnalyser::~BasicAnalyser() {

}

void BasicAnalyser::setPrescale(unsigned int prescale) {
	prescale_ = prescale;
}

void BasicAnalyser::setScale(double scale) {
	scale_ = scale;
}
}
