/*
 * METAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/METAnalyser.h"

namespace BAT {

void METAnalyser::analyse(const EventPtr event, const ParticlePointer particle) {
	analyse(event);
	analyseTransverseMass(event, particle);
}

void METAnalyser::analyse(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;

	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		METAlgorithm::value metType = (METAlgorithm::value) index;
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, index))
			continue;
		bool isMCOnlyMET = MET::isMCOnlyMETType(index);

		if (isMCOnlyMET && event->isRealData()) //these METs are MC only (Jet resolution systematics)
			continue;
		const METPointer met(event->MET(metType));
		histMan_->setCurrentHistogramFolder(histogramFolder_ + "/" + prefix);
		histMan_->H1D_BJetBinned("MET")->Fill(met->et(), weight_);
		if (index != METAlgorithm::GenMET && !event->isRealData()) {
			histMan_->H2D_BJetBinned("RecoMET_vs_GenMET")->Fill(event->GenMET()->et(), met->et(), weight_);
		}
		//do not fill other histograms for met systematics
		if ((index > METAlgorithm::patType1p2CorrectedPFMet) && (index != METAlgorithm::recoMetPFlow))
			continue;
		histMan_->H1D_BJetBinned("MET_phi")->Fill(met->phi(), weight_);
		histMan_->H1D_BJetBinned("METsignificance")->Fill(met->significance(), weight_);
		histMan_->H2D_BJetBinned("METsignificance_vs_MET")->Fill(met->et(), met->significance(), weight_);
	}

}

void METAnalyser::analyseTransverseMass(const EventPtr event, const ParticlePointer particle) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;

	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		METAlgorithm::value metType = (METAlgorithm::value) index;
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, index))
			continue;
		if (MET::isMCOnlyMETType(index) && event->isRealData())
			continue; //skip MC only METs for real data

		const METPointer met(event->MET(metType));
		histMan_->setCurrentHistogramFolder(histogramFolder_ + "/" + prefix);

		//do not fill histograms for met systematics
		if (index > METAlgorithm::patType1p2CorrectedPFMet)
			continue;

		double MT = Event::MT(particle, met);
		double angle = met->angle(particle);
		double delPhi = met->deltaPhi(particle);
		histMan_->H1D_BJetBinned("Transverse_Mass")->Fill(MT, weight_);
		histMan_->H1D_BJetBinned("Angle_lepton_MET")->Fill(angle, weight_);
		histMan_->H1D_BJetBinned("DeltaPhi_lepton_MET")->Fill(delPhi, weight_);
		if (met->et() < 20)
			histMan_->H1D_BJetBinned("Transverse_Mass_MET20")->Fill(MT, weight_);
	}
}

void METAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	histMan_->addH1D_BJetBinned("HT", "HT;HT [GeV]; Events/5 GeV", 400, 0, 2000);
	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, index))
			continue;
		std::string prefix = METAlgorithm::prefixes.at(index);
		histMan_->setCurrentHistogramFolder(histogramFolder_ + "/" + prefix);
		histMan_->addH1D_BJetBinned("MET", "Missing transverse energy; #slash{E}_{T}/GeV; events/5 GeV", 400, 0, 2000);
		histMan_->addH1D_BJetBinned("ST", "ST;ST [GeV]; Events/5 GeV", 400, 0, 2000);
		if (index != METAlgorithm::GenMET) {
			histMan_->addH2D_BJetBinned("RecoMET_vs_GenMET", "RecoMET_vs_GenMET; MET_{GEN} [GeV]; MET_{RECO} [GeV]", 60,
					0, 300, 60, 0, 300);
		}

		//do not create other histograms for met systematics
		if ((index > METAlgorithm::patType1p2CorrectedPFMet) && (index != METAlgorithm::recoMetPFlow))
			continue;
		histMan_->addH1D_BJetBinned("MET_phi", "#phi(Missing transverse energy);#phi(#slash{E}_{T});Events/0.1", 80, -4,
				4);
		histMan_->addH1D_BJetBinned("METsignificance", "METsignificance; #slash{E}_{T} significance", 1000, 0, 1000);
		histMan_->addH2D_BJetBinned("METsignificance_vs_MET",
				"Missing transverse energy vs Missing transverse energy significance;#slash{E}_{T}/GeV; #slash{E}_{T} significance",
				200, 0, 1000, 1000, 0, 1000);

		histMan_->addH1D_BJetBinned("Transverse_Mass", "Transverse Mass(lepton,MET);M_{T}(l,MET)/GeV; Events/1GeV",
				1000, 0, 1000);
		histMan_->addH1D_BJetBinned("Transverse_Mass_MET20",
				"Transverse Mass(lepton,MET);M_{T}(l,MET)/GeV; Events/1GeV", 1000, 0, 1000);
		histMan_->addH1D_BJetBinned("Angle_lepton_MET", "angle(lepton,MET);angle(l,MET); Events/0.01", 320, 0, 3.2);
		histMan_->addH1D_BJetBinned("DeltaPhi_lepton_MET", "#Delta#phi(lepton,MET);angle(l,MET); Events/0.01", 320, 0,
				3.2);
		histMan_->addH1D_BJetBinned("DeltaPhi_lepton_MET", "#Delta#phi(lepton,MET);angle(l,MET); Events/0.01", 320, 0,
				3.2);

	}
}

void METAnalyser::analyse(const EventPtr event, const ParticlePointer particle, const JetCollection jets) {
	analyse_HT(event, jets);
	analyse_ST(event, particle, jets);
	analyse(event, particle);
}

void METAnalyser::analyse_HT(const EventPtr event, const JetCollection jets) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	histMan_->H1D_BJetBinned("HT")->Fill(Event::HT(jets), weight_);
}

void METAnalyser::analyse_ST(const EventPtr event, const ParticlePointer particle, const JetCollection jets) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		METAlgorithm::value metType = (METAlgorithm::value) index;
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, index))
			continue;
		bool isMCOnlyMET = MET::isMCOnlyMETType(index);

		if (isMCOnlyMET && event->isRealData()) //these METs are MC only (Jet resolution systematics)
			continue;

		const METPointer met(event->MET(metType));
		histMan_->setCurrentHistogramFolder(histogramFolder_ + "/" + prefix);
		histMan_->H1D_BJetBinned("ST")->Fill(Event::ST(jets, particle, met), weight_);
	}
}

METAnalyser::METAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {

}

METAnalyser::~METAnalyser() {
}

} /* namespace BAT */
