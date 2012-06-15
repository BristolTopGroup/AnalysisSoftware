/*
 * JetAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/JetAnalyser.h"

namespace BAT {

void JetAnalyser::analyse(const EventPtr event) {

	histMan_->setCurrentHistogramFolder(histogramFolder_);
	double weight =  event->weight();

	//Add NJets for TTbar selection
}

void JetAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
}

JetAnalyser::JetAnalyser(boost::shared_ptr<HistogramManager> histMan, std::string histogramFolder ) :
		BasicAnalyser(histMan, histogramFolder) {

}

JetAnalyser::~JetAnalyser() {
}

} /* namespace BAT */
