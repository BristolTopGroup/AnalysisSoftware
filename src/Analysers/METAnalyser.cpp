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

		treeMan_->setCurrentFolder(histogramFolder_);
		treeMan_->Fill("MET_" + prefix,met->et());
		treeMan_->Fill("MET_phi_" + prefix,met->phi());
	}

}

void METAnalyser::analyseTransverseMass(const EventPtr event, const ParticlePointer particle) {
	weight_ = event->weight() * prescale_ * scale_;

	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		METAlgorithm::value metType = (METAlgorithm::value) index;
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, index))
			continue;
		if (MET::isMCOnlyMETType(index) && event->isRealData())
			continue; //skip MC only METs for real data

		const METPointer met(event->MET(metType));
		// double MT = Event::MT(particle, met);
		// double angle = met->angle(particle);
		// double delPhi = met->deltaPhi(particle);
	}
}

void METAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->addBranch("HT", "F", "MET" + Globals::treePrefix_);
	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		treeMan_->addBranch("MET_" + prefix, "F", "MET" + Globals::treePrefix_);
		treeMan_->addBranch("MET_phi_" + prefix, "F", "MET" + Globals::treePrefix_);
		treeMan_->addBranch("ST_" + prefix, "F", "MET" + Globals::treePrefix_);
		treeMan_->addBranch("WPT_" + prefix, "F", "MET" + Globals::treePrefix_);
		treeMan_->addBranch("MT_" + prefix, "F", "MET" + Globals::treePrefix_);
	}
}

void METAnalyser::analyse(const EventPtr event, const ParticlePointer particle, const JetCollection jets) {
	analyse_HT(event, jets);
	analyse_ST(event, particle, jets);
	analyse(event, particle);
}

void METAnalyser::analyse_HT(const EventPtr event, const JetCollection jets) {
	weight_ = event->weight() * prescale_ * scale_;
	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->Fill("EventWeight",weight_);
	treeMan_->Fill("HT",Event::HT(jets));
}

void METAnalyser::analyse_ST(const EventPtr event, const ParticlePointer particle, const JetCollection jets) {
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

		float ST = Event::ST(jets, particle, met);
		float WPT = Event::WPT(particle, met);
		float MT = Event::MT(particle, met);

		treeMan_->setCurrentFolder(histogramFolder_);
		treeMan_->Fill("ST_" + prefix,ST);
		treeMan_->Fill("WPT_" + prefix,WPT);
		treeMan_->Fill("MT_" + prefix,MT);
	}
}

METAnalyser::METAnalyser(TreeManagerPtr treeMan, std::string histogramFolder) :
		BasicAnalyser(treeMan, histogramFolder) {
}


METAnalyser::~METAnalyser() {
}

} /* namespace BAT */
