/*
 * TopPairMuPlusJetsReferenceSelection.cpp
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/TopPairMuPlusJetsReferenceSelection.h"
#include "../../interface/HighLevelTriggers.h"

using namespace std;

namespace BAT {

TopPairMuPlusJetsReferenceSelection::TopPairMuPlusJetsReferenceSelection(unsigned int numberOfSelectionSteps) : //
		BasicSelection(numberOfSelectionSteps) //
{

}

TopPairMuPlusJetsReferenceSelection::~TopPairMuPlusJetsReferenceSelection() {
}

bool TopPairMuPlusJetsReferenceSelection::isGoodJet(const JetPointer jet) const {

	/**
	 * This function tests the jet ID and eta (and pt) range for jet
	 * The cut of 20 GeV is actually obsolete since we only store jets above that threshold.
	 * However, the jet id is only valid for jets above it.
	 */

	bool passesPtAndEta = jet->pt() > 20 && fabs(jet->eta()) < 2.5;
	bool passesJetID(false);
	JetAlgorithm::value algo = jet->getUsedAlgorithm();
	if (algo == JetAlgorithm::CA08PF || algo == JetAlgorithm::PF2PAT) { //PFJet
		bool passNOD = jet->NOD() > 1;
		bool passNHF = jet->NHF() < 0.99;
		bool passNEF = jet->NEF() < 0.99;
		bool passCHF = true;
		bool passNCH = true;
		bool passCEF = true;
		if (fabs(jet->eta()) < 2.4) {
			passCEF = jet->CEF() < 0.99;
			passCHF = jet->CHF() > 0;
			passNCH = jet->NCH() > 0;
		}
		passesJetID = passNOD && passCEF && passNHF && passNEF && passCHF && passNCH;

	} else { //assume CaloJet
		bool passesEMF = jet->emf() > 0.01;
		bool passesN90Hits = jet->n90Hits() > 1;
		bool passesFHPD = jet->fHPD() < 0.98;
		passesJetID = passesEMF && passesN90Hits && passesFHPD;
	}
	return passesPtAndEta && passesJetID;

}

bool TopPairMuPlusJetsReferenceSelection::isBJet(const JetPointer jet) const {
	return jet->isBJet(BtagAlgorithm::CombinedSecondaryVertex, BtagAlgorithm::MEDIUM);
}

bool TopPairMuPlusJetsReferenceSelection::isGoodMuon(const MuonPointer muon) const {
	bool passesEtAndEta = muon->pt() > 26 && fabs(muon->eta()) < 2.1;
	bool passesD0 = fabs(muon->d0()) < 0.2; //cm
	bool passesDistanceToPV = fabs(muon->ZDistanceToPrimaryVertex()) < 0.5;
	bool passesID = muon->isGlobal() && muon->isPFMuon();

	bool passesMuonQuality_1(muon->normChi2() < 10 && muon->trackerLayersWithMeasurement() > 5);
	bool passesMuonQuality_2(muon->numberOfValidMuonHits() > 0 && muon->pixelLayersWithMeasurement() > 0);
	bool passesMuonQuality_3(muon->numberOfMatchedStations() > 1);
	bool passesMuonQuality = passesMuonQuality_1 && passesMuonQuality_2 && passesMuonQuality_3;

	return passesEtAndEta && passesD0 && passesDistanceToPV && passesID && passesMuonQuality;
}

bool TopPairMuPlusJetsReferenceSelection::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const {
	TTbarMuPlusJetsReferenceSelection::Step step = TTbarMuPlusJetsReferenceSelection::Step(selectionStep);
	switch (step) {
	case TTbarMuPlusJetsReferenceSelection::EventCleaningAndTrigger:
		return passesEventCleaning(event) && passesTriggerSelection(event);
	case TTbarMuPlusJetsReferenceSelection::OneIsolatedMuon:
		return hasExactlyOneIsolatedLepton(event);
	case TTbarMuPlusJetsReferenceSelection::LooseMuonVeto:
		return passesLooseMuonVeto(event);
	case TTbarMuPlusJetsReferenceSelection::LooseElectronVeto:
		return passesLooseElectronVeto(event);
	case TTbarMuPlusJetsReferenceSelection::AtLeastOneGoodJets:
		return hasAtLeastNGoodJets(event, 1);
	case TTbarMuPlusJetsReferenceSelection::AtLeastTwoGoodJets:
		return hasAtLeastNGoodJets(event, 2);
	case TTbarMuPlusJetsReferenceSelection::AtLeastThreeGoodJets:
		return hasAtLeastNGoodJets(event, 3);
	case TTbarMuPlusJetsReferenceSelection::AtLeastFourGoodJets:
		return hasAtLeastNGoodJets(event, 4);
	case TTbarMuPlusJetsReferenceSelection::AtLeastOneBtag:
		return hasAtLeastOneGoodBJet(event);
	case TTbarMuPlusJetsReferenceSelection::AtLeastTwoBtags:
		return hasAtLeastTwoGoodBJets(event);
	default:
		break;
	}

	return false;
}

bool TopPairMuPlusJetsReferenceSelection::passesEventCleaning(const EventPtr event) const {
	bool passesAllFilters(!event->isBeamScraping());
	passesAllFilters = passesAllFilters && event->passesHBHENoiseFilter();
	passesAllFilters = passesAllFilters && event->passesCSCTightBeamHaloFilter();
	passesAllFilters = passesAllFilters && event->passesHCALLaserFilter();
	passesAllFilters = passesAllFilters && event->passesTrackingFailureFilter();
	if (Globals::NTupleVersion >= 9)
		passesAllFilters = passesAllFilters && event->passesECALDeadCellTPFilter();
	if ((Globals::energyInTeV == 8) and (Globals::NTupleVersion >= 10)) { //2012 data only, v10 ntuples
		passesAllFilters = passesAllFilters && event->passesEEBadSCFilter();
		passesAllFilters = passesAllFilters && event->passesECALLaserCorrFilter();
		passesAllFilters = passesAllFilters && event->passesTrackingPOGFilters();
	}

	return passesAllFilters;
}

bool TopPairMuPlusJetsReferenceSelection::passesTriggerSelection(const EventPtr event) const {
	unsigned int runNumber(event->runnumber());
	if (event->isRealData()) {
		//2011 data: run 160404 to run 180252
		if (runNumber >= 160404 && runNumber < 173236)
			return event->HLT(HLTriggers::HLT_IsoMu24);
		else if (runNumber >= 173236 && runNumber <= 180252)
			return event->HLT(HLTriggers::HLT_IsoMu24_eta2p1);
		//2012 data: run 190456 to run 208686 (same trigger as second 2011 trigger)
		else if (runNumber >= 190456 && runNumber <= 208686)
			return event->HLT(HLTriggers::HLT_IsoMu24_eta2p1);
		else
			return false;
	} else {
		return event->HLT(HLTriggers::HLT_IsoMu24_eta2p1);
	}
}

bool TopPairMuPlusJetsReferenceSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	unsigned int nIsolatedGoodMuons(0);
	const MuonCollection allMuons(event->Muons());
	for (unsigned int index = 0; index < allMuons.size(); ++index) {
		const MuonPointer muon(allMuons.at(index));
		if (isGoodMuon(muon) && isIsolated(muon))
			++nIsolatedGoodMuons;
	}

	return nIsolatedGoodMuons == 1;

}

bool TopPairMuPlusJetsReferenceSelection::isGoodElectron(const ElectronPointer electron) const {
	bool passesEtAndEta = electron->et() > 30 && fabs(electron->eta()) < 2.5 && !electron->isInCrack();
	bool passesD0 = fabs(electron->d0()) < 0.02; //cm
	bool passesHOverE = electron->HadOverEm() < 0.05; // same for EE and EB
	bool passesID(electron->passesElectronID(ElectronID::MVAIDTrigger));
	return passesEtAndEta && passesD0 &&
			passesHOverE && passesID;
}

bool TopPairMuPlusJetsReferenceSelection::isIsolated(const LeptonPointer lepton) const {
	const MuonPointer muon(boost::static_pointer_cast<Muon>(lepton));

	return muon->pfRelativeIsolation(0.4, true) < 0.12;

}

bool TopPairMuPlusJetsReferenceSelection::passesLooseElectronVeto(const EventPtr event) const {

	unsigned int nLooseElectrons(0);
	const ElectronCollection allElectrons(event->Electrons());

	for (unsigned int index = 0; index < allElectrons.size(); ++index) {
		const ElectronPointer electron(allElectrons.at(index));
		if (isLooseElectron(electron))
			++nLooseElectrons;
	}

	//good isolated electron is always a loose electron as well
	return nLooseElectrons == 0;
}

bool TopPairMuPlusJetsReferenceSelection::isLooseMuon(const MuonPointer muon) const {
	bool passesPt = muon->pt() > 10;
	bool passesEta = fabs(muon->eta()) < 2.5;
	bool isPFMuon = muon->isPFMuon();
	bool isGlobalOrTracker = muon->isGlobal() || muon->isTracker();
	bool isLooselyIsolated = muon->pfRelativeIsolation(0.4, true) < 0.2;

	return isPFMuon && passesPt && passesEta && isGlobalOrTracker && isLooselyIsolated;
}

bool TopPairMuPlusJetsReferenceSelection::passesLooseMuonVeto(const EventPtr event) const {
	unsigned int nLooseMuons(0);
	const MuonCollection allMuons(event->Muons());

	for (unsigned int index = 0; index < allMuons.size(); ++index) {
		const MuonPointer muon(allMuons.at(index));
		if (isLooseMuon(muon))
			++nLooseMuons;
	}

	return nLooseMuons < 2;
}

bool TopPairMuPlusJetsReferenceSelection::isLooseElectron(const ElectronPointer electron) const {

	bool passesEtAndEta = electron->et() > 20. && fabs(electron->eta()) < 2.5;
	bool passesID(electron->passesElectronID(ElectronID::MVAIDTrigger));
	bool passesIso = electron->pfRelativeIsolationRhoCorrected() < 0.15;
	return passesEtAndEta && passesIso && passesID;
}

bool TopPairMuPlusJetsReferenceSelection::hasAtLeastNGoodJets(const EventPtr event, int Njets) const {
	const JetCollection goodJets(cleanedJets(event));
	int nJetsAbove30GeV(0);

	for (unsigned int index = 0; index < goodJets.size(); ++index) {

		const JetPointer jet(goodJets.at(index));
		if (goodJets.at(index)->pt() > 30.)
			++nJetsAbove30GeV;
	}
	return nJetsAbove30GeV >= Njets;
}

bool TopPairMuPlusJetsReferenceSelection::hasAtLeastOneGoodBJet(const EventPtr event) const {
	return cleanedBJets(event).size() > 0;
}

bool TopPairMuPlusJetsReferenceSelection::hasAtLeastTwoGoodBJets(const EventPtr event) const {
	return cleanedBJets(event).size() > 1;
}

const LeptonPointer TopPairMuPlusJetsReferenceSelection::signalLepton(const EventPtr event) const {
	if (!hasExactlyOneIsolatedLepton(event)) {
		cerr << "An error occurred in SignalSelection in event (no = " << event->eventnumber();
		cerr << ", run = " << event->runnumber() << ", lumi = " << event->lumiblock() << "!" << endl;
		cerr << "File = " << event->file() << endl;
		cerr
				<< "Access exception: No signal lepton could be found. Event doesn't pass 'hasExactlyOneIsolatedLepton' selection"
				<< endl;
		throw "Access exception: No signal lepton could be found. Event doesn't pass 'hasExactlyOneIsolatedLepton' selection";
	}

	const MuonCollection allMuons(event->Muons());
	MuonCollection goodIsolatedMuons;
	for (unsigned int index = 0; index < allMuons.size(); ++index) {
		const MuonPointer muon(allMuons.at(index));
		if (isGoodMuon(muon) && isIsolated(muon)) {
			goodIsolatedMuons.push_back(muon);
		}
	}

	return goodIsolatedMuons.front();

}

const JetCollection TopPairMuPlusJetsReferenceSelection::cleanedJets(const EventPtr event) const {
	const JetCollection jets(event->Jets());
	JetCollection cleanedJets;

	//if no signal lepton is found, can't clean jets, return them all!
	if (!hasExactlyOneIsolatedLepton(event))
		return jets;

	const LeptonPointer lepton(signalLepton(event));

	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet(jets.at(index));
		if (!jet->isWithinDeltaR(0.3, lepton) && isGoodJet(jet))
			cleanedJets.push_back(jet);
	}

	return cleanedJets;
}

const JetCollection TopPairMuPlusJetsReferenceSelection::cleanedBJets(const EventPtr event) const {
	const JetCollection jets(cleanedJets(event));
	JetCollection cleanedBJets;

	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet(jets.at(index));
		if (isBJet(jet))
			cleanedBJets.push_back(jet);
	}

	return cleanedBJets;

}

} /* namespace BAT */
