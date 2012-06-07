/*
 *  ABCDMethodAnalyser.cpp
 *
 *  Created on: 13th May 2012
 *      Author: J. Jacob
 */

#include "../../interface/Analysers/ABCDMethodAnalyser.h"
#include "../../interface/GlobalVariables.h"
#include "../../interface/Selections/QCDPFRelIsoEPlusJetsSelection.h"

namespace BAT {

//constructor:
ABCDMethodAnalyser::ABCDMethodAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		qcdSelection_(new QCDPFRelIsoEPlusJetsSelection()) {
	// inherits from QCDPFRelIsoEPlusJetsSelection.h which inherits from TopPairEPlusJetsReferenceSelection, which inherits from BasicSelection.
}

ABCDMethodAnalyser::~ABCDMethodAnalyser() {
}

//defining what the analyse function in the class ABCDMethodAnalyser from ABCDMethodAnalyser.h should carry out
void ABCDMethodAnalyser::analyse(const EventPtr event) {
	double weight = event->weight();
	histMan_->setCurrentHistogramFolder(histogramFolder_); //setCurrentHistogramFolder from HistogramManager.h?

	//from TTbarPlusMETAnalyser
	const METPointer met = event->MET();
	//cout << met;

	// want the event to pass the selection steps up to DiLeptonVeto (including OneIsolatedElectron which we want) - from BasicSelection.cpp
	//also want the event to pass the selection step of at least 4 good jets
	bool passesSelection = qcdSelection_->passesSelectionUpToStep(event, TTbarEPlusJetsReferenceSelection::DiLeptonVeto)
			&& qcdSelection_->passesSelectionStep(event, TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets);

	if (passesSelection) {
		const LeptonPointer signalLepton = qcdSelection_->signalLepton(event); //from QCDPFRelIsoEPlusJetsSelection.cpp
		const ElectronPointer electron(boost::static_pointer_cast<Electron>(signalLepton));
		double pfiso = electron->pfRelativeIsolation(); //removed 0.3 from brackets
		bool isConversion = electron->passConversionVeto();

		histMan_->H1D_BJetBinned("Particle_Flow_Isolation")->Fill(pfiso, weight);
		histMan_->H1D_BJetBinned("Conversion_Veto")->Fill(isConversion, weight);
		histMan_->H1D_BJetBinned("Missing_Transverse_Energy")->Fill(met->et(), weight);

		histMan_->H2D_BJetBinned("Particle_Flow_Isolation_v._Missing_Transverse_Energy")->Fill(met->et(), pfiso,
				weight);
		histMan_->H2D_BJetBinned("Conversion_Veto_v._Missing_Transverse_Energy")->Fill(met->et(), isConversion, weight);

		histMan_->H2D_BJetBinned("Particle_Flow_Isolation_vs._Conversion_Veto")->Fill(isConversion, pfiso, weight);
	}
}

void ABCDMethodAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	histMan_->addH1D_BJetBinned("Particle_Flow_Isolation", "Particle_Flow_Isolation", 500, 0, 5);
	histMan_->addH1D_BJetBinned("Conversion_Veto", "Conversion_Veto", 2, 0.0, 2.0);
	histMan_->addH1D_BJetBinned("Missing_Transverse_Energy", "Missing_Transverse_Energy", 200, 0, 1000);

	histMan_->addH2D_BJetBinned("Particle_Flow_Isolation_v._Missing_Transverse_Energy",
			"Particle_Flow_Isolation_v._Missing_Transverse_Energy", 200, 0, 1000, 500, 0, 5);
	histMan_->addH2D_BJetBinned("Conversion_Veto_v._Missing_Transverse_Energy",
			"Conversion_Veto_v._Missing_Transverse_Energy", 200, 0, 1000, 2, 0.0, 2.0);

	histMan_->addH2D_BJetBinned("Particle_Flow_Isolation_vs._Conversion_Veto",
			"Particle_Flow_Isolation_vs._Conversion_Veto", 2, 0.0, 2.0, 500, 0, 5);

}

} //end namespace BAT
