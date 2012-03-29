/*
 * METAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/METAnalyser.h"

namespace BAT {

void METAnalyser::analyse(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	const double weight = event->weight();
	const METPointer met = event->MET();

	histMan_->H1D_BJetBinned("MET")->Fill(met->et(), weight);
	histMan_->H1D_BJetBinned("METsignificance")->Fill(met->significance(), weight);
	histMan_->H2D_BJetBinned("METsignificance_vs_MET")->Fill(met->et(), met->significance(), weight);
}

void METAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	histMan_->addH1D_BJetBinned("MET", "MET", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("METsignificance", "METsignificance", 1000, 0, 1000);
	histMan_->addH2D_BJetBinned("METsignificance_vs_MET", "MET vs MET significance;MET; MET significance", 1000, 0,

			1000, 1000, 0, 1000);
}

METAnalyser::METAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {

}

METAnalyser::~METAnalyser() {
}

} /* namespace BAT */
