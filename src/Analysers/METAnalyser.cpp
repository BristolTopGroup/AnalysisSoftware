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
			//do not fill other histograms for met systematics
			if (index > METAlgorithm::patType1p2CorrectedPFMet)
				continue;
			histMan_->H1D_BJetBinned("MET_phi")->Fill(met->phi(), weight_);
			histMan_->H1D_BJetBinned("METsignificance")->Fill(met->significance(), weight_);
			histMan_->H2D_BJetBinned("METsignificance_vs_MET")->Fill(met->et(), met->significance(), weight_);
		}
	}

}

void METAnalyser::analyseTransverseMass(const EventPtr event, const ParticlePointer particle) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_;

	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		METAlgorithm::value metType = (METAlgorithm::value) index;
		if (index == METAlgorithm::patMETsPFlow || Globals::NTupleVersion >= 7) {
			//do not fill histograms for met systematics
			if (index > METAlgorithm::patType1p2CorrectedPFMet)
				continue;
			const METPointer met(event->MET(metType));
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/" + prefix);

			double MT = transverseMass(met, particle);
			double angle = met->angle(particle);
			histMan_->H1D_BJetBinned("Transverse_Mass")->Fill(MT, weight_);
			histMan_->H1D_BJetBinned("Angle_lepton_MET")->Fill(angle, weight_);
			if (met->et() < 20)
				histMan_->H1D_BJetBinned("Transverse_Mass_MET20")->Fill(MT, weight_);
		}
	}
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
			histMan_->addH1D_BJetBinned("MET", "Missing transverse energy; #slash{E}_{T}/GeV; events/1 GeV", 1000, 0,
					1000);
			//do not create other histograms for met systematics
			if (index > METAlgorithm::patType1p2CorrectedPFMet)
				continue;
			histMan_->addH1D_BJetBinned("MET_phi", "#phi(Missing transverse energy);#phi(#slash{E}_{T});Events/0.1",
					80, -4, 4);
			histMan_->addH1D_BJetBinned("METsignificance", "METsignificance; #slash{E}_{T} significance", 1000, 0, 1000);
			histMan_->addH2D_BJetBinned(
					"METsignificance_vs_MET",
					"Missing transverse energy vs Missing transverse energy significance;#slash{E}_{T}/GeV; #slash{E}_{T} significance",
					200, 0, 1000, 1000, 0, 1000);

			histMan_->addH1D_BJetBinned("Transverse_Mass", "Transverse Mass(lepton,MET);M_{T}(l,MET)/GeV; Events/1GeV",
					1000, 0, 1000);
			histMan_->addH1D_BJetBinned("Transverse_Mass_MET20",
					"Transverse Mass(lepton,MET);M_{T}(l,MET)/GeV; Events/1GeV", 1000, 0, 1000);
			histMan_->addH1D_BJetBinned("Angle_lepton_MET", "angle(lepton,MET);angle(l,MET); Events/0.01", 320, 0, 3.2);
		}
	}
}

METAnalyser::METAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
	BasicAnalyser(histMan, histogramFolder) {

}

METAnalyser::~METAnalyser() {
}

} /* namespace BAT */
