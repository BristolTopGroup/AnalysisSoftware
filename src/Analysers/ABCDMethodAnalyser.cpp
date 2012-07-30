/*
 *  ABCDMethodAnalyser.cpp
 *
 *  Created on: 13th May 2012
 *      Author: J. Jacob
 */

#include "../../interface/Analysers/ABCDMethodAnalyser.h"

namespace BAT {

//constructor:
ABCDMethodAnalyser::ABCDMethodAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		qcdNoIsoNoIDSelection_(new QCDNoIsoNoIDSelection()) {
	// inherits from QCDPFRelIsoEPlusJetsSelection.h which inherits from TopPairEPlusJetsReferenceSelection, which inherits from BasicSelection.
}

ABCDMethodAnalyser::~ABCDMethodAnalyser() {
}

//defining what the analyse function in the class ABCDMethodAnalyser from ABCDMethodAnalyser.h should carry out
void ABCDMethodAnalyser::analyse(const EventPtr event) {
	double weight = event->weight() * prescale_ * scale_;
	histMan_->setCurrentHistogramFolder(histogramFolder_); //setCurrentHistogramFolder from HistogramManager.h?

	//from TTbarPlusMETAnalyser
	const METPointer met = event->MET();

	// want the event to pass the selection steps up to DiLeptonVeto (including OneIsolatedElectron which we want) - from BasicSelection.cpp
	//also want the event to pass the selection step of at least 4 good jets
	bool passesSelection = qcdNoIsoNoIDSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets);
	if (passesSelection) {
		const JetCollection jets(qcdNoIsoNoIDSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNoIsoNoIDSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		const LeptonPointer signalLepton = qcdNoIsoNoIDSelection_->signalLepton(event); //from QCDNoIsoNoIDSelection.cpp
		const ElectronPointer electron(boost::static_pointer_cast<Electron>(signalLepton));
		double pfiso = electron->pfRelativeIsolation(); //removed 0.3 from brackets
		double electronIDMVA = electron->mvaTrigV0();

		histMan_->H2D_BJetBinned("Particle_Flow_Isolation_v._Missing_Transverse_Energy")->Fill(met->et(), pfiso,
				weight);

		histMan_->H2D_BJetBinned("ElectronID_MVA_v._Missing_Transverse_Energy")->Fill(met->et(), electronIDMVA, weight);
		histMan_->H2D_BJetBinned("Particle_Flow_Isolation_v._ElectronID_MVA")->Fill(electronIDMVA, pfiso, weight);
	}
}

void ABCDMethodAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	histMan_->addH2D_BJetBinned("Particle_Flow_Isolation_v._Missing_Transverse_Energy",
			"Particle_Flow_Isolation_v._Missing_Transverse_Energy", 200, 0, 1000, 500, 0, 5);
	histMan_->addH2D_BJetBinned("ElectronID_MVA_v._Missing_Transverse_Energy",
			"ElectronID_MVA_v._Missing_Transverse_Energy", 200, 0, 1000, 220, -1.1, 1.1);

	histMan_->addH2D_BJetBinned("Particle_Flow_Isolation_v._ElectronID_MVA",
			"Particle_Flow_Isolation_v._ElectronID_MVA", 220, -1.1, 1.1, 500, 0, 5);

}

} //end namespace BAT
