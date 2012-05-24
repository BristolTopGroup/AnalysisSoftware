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

	weight_ = event->weight() * prescale_;

	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		METAlgorithm::value metType = (METAlgorithm::value) index;
		if (index == METAlgorithm::patMETsPFlow || Globals::NTupleVersion >= 7) {
			const METPointer met(event->MET(metType));
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/" + prefix);
			histMan_->H1D_BJetBinned("MET")->Fill(met->et(), weight_);
			histMan_->H1D_BJetBinned("MET_phi")->Fill(met->phi(), weight_);
			histMan_->H1D_BJetBinned("METsignificance")->Fill(met->significance(), weight_);
			histMan_->H2D_BJetBinned("METsignificance_vs_MET")->Fill(met->et(), met->significance(), weight_);
		}
	}

}

void METAnalyser::analyseTransverseMass(const METPointer met, const ParticlePointer particle, double weight) {
	std::string prefix = METAlgorithm::prefixes.at(met->getUsedAlgorithm());

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/" + prefix);
	weight_ = weight * prescale_;

	double MT = transverseMass(met, particle);
	histMan_->H1D_BJetBinned("Transverse_Mass")->Fill(MT, weight_);
}

double METAnalyser::transverseMass(const METPointer met, const ParticlePointer particle) const {
	double energySquared = pow(particle->et() + met->et(), 2);
	double momentumSquared = pow(particle->px() + met->px(), 2) + pow(particle->py() + met->py(), 2);
	double MTSquared = energySquared - momentumSquared;

	if (MTSquared > 0)
		return sqrt(MTSquared);
	else
		return -1;
}

void METAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		if (index == METAlgorithm::patMETsPFlow || Globals::NTupleVersion >= 7) {
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/" + prefix);
			histMan_->addH1D_BJetBinned("MET", "MET", 1000, 0, 1000);
			histMan_->addH1D_BJetBinned("MET_phi", "MET_phi", 80, -4, 4);
			histMan_->addH1D_BJetBinned("METsignificance", "METsignificance", 1000, 0, 1000);
			histMan_->addH2D_BJetBinned("METsignificance_vs_MET", "MET vs MET significance;MET; MET significance", 200,
					0, 1000, 1000, 0, 1000);

			histMan_->addH1D_BJetBinned("Transverse_Mass", "Transverse Mass(lepton,MET);M_{T}(l,MET); Events", 1000, 0,
					1000);
		}
	}
}

METAnalyser::METAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {

}

METAnalyser::~METAnalyser() {
}

} /* namespace BAT */
