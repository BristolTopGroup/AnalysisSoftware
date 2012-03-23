/*
 * DiElectronAnalyser.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: lkreczko
 */

#include "../../interface/Analysers/DiElectronAnalyser.h"

namespace BAT {

void DiElectronAnalyser::analyse(const EventPtr event) {
	histMan->setCurrentHistogramFolder("DiElectronAnalysis");
	ElectronCollection electrons =  event->GoodElectrons();
	double weight =  event->weight();
	if (electrons.size() == 2) {
		ElectronPointer leadingElectron = electrons.front();
		ElectronPointer secondElectron = electrons.at(1);
		histMan->H1D_JetBinned("diElectronMass")->Fill(leadingElectron->invariantMass(secondElectron), weight);
	}

	ElectronCollection isolatedElectrons =  event->GoodPFIsolatedElectrons();
	if (isolatedElectrons.size() == 2) {
		ElectronPointer leadingElectron = isolatedElectrons.front();
		ElectronPointer secondElectron = isolatedElectrons.at(1);
		histMan->H1D_JetBinned("diElectronMass_iso")->Fill(leadingElectron->invariantMass(secondElectron), weight);
	}
}

void DiElectronAnalyser::createHistograms() {
	//create folder/collection name for the histograms
	histMan->setCurrentHistogramFolder("DiElectronAnalysis");
	// titles are in the format: histogram title; x-axis title; y-axis title
	histMan->addH1D_JetBinned("diElectronMass", "di-electron mass; m(ee) [GeV] events;", 7000, 0, 7000);
	histMan->addH1D_JetBinned("diElectronMass_iso", "di-is0-electron mass; m(ee) [GeV]; events", 7000, 0, 7000);
}

DiElectronAnalyser::DiElectronAnalyser(boost::shared_ptr<HistogramManager> histMan) :
	BasicAnalyser(histMan) {

}

DiElectronAnalyser::~DiElectronAnalyser() {
}

}
