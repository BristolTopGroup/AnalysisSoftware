/*
 * DiElectronAnalyser.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: lkreczko
 */

#include "../../interface/Analysers/DiElectronAnalyser.h"

namespace BAT {

void DiElectronAnalyser::analyse(const EventPtr event) {
	histMan_->setCurrentHistogramFolder("DiElectronAnalysis");
	ElectronCollection electrons = event->Electrons();
	weight_ = event->weight() * prescale_ * scale_;
	if (electrons.size() == 2) {
		ElectronPointer leadingElectron = electrons.front();
		ElectronPointer secondElectron = electrons.at(1);
		histMan_->H1D_JetBinned("diElectronMass")->Fill(leadingElectron->invariantMass(secondElectron), weight_);
	}

	ElectronCollection isolatedElectrons;

	for (unsigned int index = 0; index < electrons.size(); ++index) {
		const ElectronPointer electron(electrons.at(index));
		if (electron->pfRelativeIsolation(0.3) < 0.1)
			isolatedElectrons.push_back(electron);
	}
	if (isolatedElectrons.size() == 2) {
		ElectronPointer leadingElectron = isolatedElectrons.front();
		ElectronPointer secondElectron = isolatedElectrons.at(1);
		histMan_->H1D_JetBinned("diElectronMass_iso")->Fill(leadingElectron->invariantMass(secondElectron), weight_);
	}
}

void DiElectronAnalyser::createHistograms() {
	//create folder/collection name for the histograms
	histMan_->setCurrentHistogramFolder("DiElectronAnalysis");
	// titles are in the format: histogram title; x-axis title; y-axis title
	histMan_->addH1D_JetBinned("diElectronMass", "di-electron mass; m(ee) [GeV] events;", 7000, 0, 7000);
	histMan_->addH1D_JetBinned("diElectronMass_iso", "di-is0-electron mass; m(ee) [GeV]; events", 7000, 0, 7000);
}

DiElectronAnalyser::DiElectronAnalyser(boost::shared_ptr<HistogramManager> histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {

}

DiElectronAnalyser::~DiElectronAnalyser() {
}

}
