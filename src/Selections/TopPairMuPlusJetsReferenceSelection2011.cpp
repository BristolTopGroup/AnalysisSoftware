/*
 * TopPairReferenceSelection2011.cpp
 *
 *  Created on: 17 Oct 2011
 *      Author: symonds
 */

#include "../../interface/Selections/TopPairMuPlusJetsReferenceSelection2011.h"
#include "../../interface/HighLevelTriggers.h"

using namespace std;

namespace BAT {

TopPairMuPlusJetsReferenceSelection2011::TopPairMuPlusJetsReferenceSelection2011(unsigned int numberOfSelectionSteps) : //
		BasicSelection(numberOfSelectionSteps) //
{

}

TopPairMuPlusJetsReferenceSelection2011::~TopPairMuPlusJetsReferenceSelection2011() {
}

bool TopPairMuPlusJetsReferenceSelection2011::isGoodJet(const JetPointer jet) const {
	bool passesPtAndEta = jet->pt() > 20 && fabs(jet->eta()) < 2.4;
	bool passesJetID(false);
	JetAlgorithm::value algo = jet->getUsedAlgorithm();
	if (algo == JetAlgorithm::CA08PF || algo == JetAlgorithm::PF2PAT) { //PFJet
		bool passNOD = jet->NOD() > 1;
		bool passNHF = jet->NHF() < 0.99;
		bool passNEF = jet->NEF() < 0.99;
		bool passCHF = jet->CEF() < 0.99;
		bool passNCH = true;
		bool passCEF = true;
		if (fabs(jet->eta()) < 2.4) {
			passCHF = jet->CHF() > 0;
			passNCH = jet->NCH() > 0;
		}
		passesJetID = passNOD && passCEF && passNHF && passNEF && passCHF && passNCH;
		;
	} else { //assume CaloJet
		bool passesEMF = jet->emf() > 0.01;
		bool passesN90Hits = jet->n90Hits() > 1;
		bool passesFHPD = jet->fHPD() < 0.98;
		passesJetID = passesEMF && passesN90Hits && passesFHPD;
	}
	return passesPtAndEta && passesJetID;

}

bool TopPairMuPlusJetsReferenceSelection2011::isBJet(const JetPointer jet) const {
	return jet->isBJet(BtagAlgorithm::CombinedSecondaryVertex, BtagAlgorithm::MEDIUM);
}

bool TopPairMuPlusJetsReferenceSelection2011::isGoodMuon(const MuonPointer muon) const {
	bool passesEtAndEta = muon->pt() > 26 && fabs(muon->eta()) < 2.1;
	bool passesD0 = fabs(muon->d0()) < 0.02; //cm
	bool passesDistanceToPV = fabs(muon->ZDistanceToPrimaryVertex()) < 1;
	bool passesID(muon->isTracker() && muon->isGlobal());

	bool passesMuonQuality_1(muon->normChi2() < 10 && muon->trackerLayersWithMeasurement() > 8);
	bool passesMuonQuality_2(muon->numberOfValidMuonHits() > 0 && muon->pixelLayersWithMeasurement() > 0);
	bool passesMuonQuality_3(muon->numberOfMatchedStations() > 1);
	bool passesMuonQuality = passesMuonQuality_1 && passesMuonQuality_2 && passesMuonQuality_3;

	return passesEtAndEta && passesD0 && passesDistanceToPV && passesID && passesMuonQuality;
}

bool TopPairMuPlusJetsReferenceSelection2011::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const {
	TTbarMuPlusJetsReferenceSelection2011::Step step = TTbarMuPlusJetsReferenceSelection2011::Step(selectionStep);
	switch (step) {
	case TTbarMuPlusJetsReferenceSelection2011::EventCleaningAndTrigger:
		return passesEventCleaning(event) && passesTriggerSelection(event);
	case TTbarMuPlusJetsReferenceSelection2011::OneIsolatedMuon:
		return hasExactlyOneIsolatedLepton(event);
	case TTbarMuPlusJetsReferenceSelection2011::LooseMuonVeto:
		return passesLooseMuonVeto(event);
	case TTbarMuPlusJetsReferenceSelection2011::LooseElectronVeto:
		return passesLooseElectronVeto(event);
	case TTbarMuPlusJetsReferenceSelection2011::AtLeastThreeGoodJets:
		return hasAtLeastThreeGoodJets(event);
	case TTbarMuPlusJetsReferenceSelection2011::AtLeastFourGoodJets:
		return hasAtLeastFourGoodJets(event);
	case TTbarMuPlusJetsReferenceSelection2011::AtLeastOneBtag:
		return hasAtLeastOneGoodBJet(event);
	case TTbarMuPlusJetsReferenceSelection2011::AtLeastTwoBtags:
		return hasAtLeastTwoGoodBJets(event);
	default:
		break;
	}

	return false;
}

bool TopPairMuPlusJetsReferenceSelection2011::passesEventCleaning(const EventPtr event) const {
	bool passesAllFilters(!event->isBeamScraping());
	passesAllFilters = passesAllFilters && event->passesHBHENoiseFilter();
	passesAllFilters = passesAllFilters && event->passesCSCTightBeamHaloFilter();
	passesAllFilters = passesAllFilters && event->passesHCALLaserFilter();
	//	passesAllFilters = passesAllFilters && event->passesECALDeadCellFilter();
	passesAllFilters = passesAllFilters && event->passesTrackingFailureFilter();
	if (Globals::NTupleVersion >= 9)
		passesAllFilters = passesAllFilters && event->passesECALDeadCellTPFilter();
	return passesAllFilters;
}

bool TopPairMuPlusJetsReferenceSelection2011::passesTriggerSelection(const EventPtr event) const {
	unsigned int runNumber(event->runnumber());
	if (event->isRealData()) {
		if (runNumber >= 160404 && runNumber < 173236)
			return event->HLT(HLTriggers::HLT_IsoMu24);
		else if (runNumber >= 173236)
			return event->HLT(HLTriggers::HLT_IsoMu24_eta2p1);
		else
			return false;
	} else {
		//Fall11 MC
		return event->HLT(HLTriggers::HLT_IsoMu24);
	}
}

bool TopPairMuPlusJetsReferenceSelection2011::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	unsigned int nIsolatedGoodMuons(0);
	const MuonCollection allMuons(event->Muons());
	for (unsigned int index = 0; index < allMuons.size(); ++index) {
		const MuonPointer muon(allMuons.at(index));
		if (isGoodMuon(muon) && isIsolated(muon))
			++nIsolatedGoodMuons;
	}
	return nIsolatedGoodMuons == 1;

}

bool TopPairMuPlusJetsReferenceSelection2011::isGoodElectron(const ElectronPointer electron) const {
	bool passesEtAndEta = electron->et() > 30 && fabs(electron->eta()) < 2.5 && !electron->isInCrack();
	bool passesD0 = fabs(electron->d0()) < 0.02; //cm
	bool passesHOverE = electron->HadOverEm() < 0.05; // same for EE and EB
	bool passesID(electron->passesElectronID(ElectronID::MVAIDTrigger));
	return passesEtAndEta && passesD0 &&
//			passesDistanceToPV &&
			passesHOverE && passesID;
}

bool TopPairMuPlusJetsReferenceSelection2011::isIsolated(const LeptonPointer lepton) const {
	const MuonPointer muon(boost::static_pointer_cast<Muon>(lepton));
	return muon->pfRelativeIsolation(0.4) < 0.125;
}

bool TopPairMuPlusJetsReferenceSelection2011::passesLooseElectronVeto(const EventPtr event) const {

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

bool TopPairMuPlusJetsReferenceSelection2011::isLooseMuon(const MuonPointer muon) const {
	bool passesPt = muon->pt() > 10;
	bool passesEta = fabs(muon->eta()) < 2.5;
	bool isGlobalOrTracker = muon->isGlobal() || muon->isTracker();
	bool isLooselyIsolated = muon->pfRelativeIsolation(0.4) < 0.2;

	return passesPt && passesEta && isGlobalOrTracker && isLooselyIsolated;
}

bool TopPairMuPlusJetsReferenceSelection2011::passesLooseMuonVeto(const EventPtr event) const {
	unsigned int nLooseMuons(0);
	const MuonCollection allMuons(event->Muons());

	for (unsigned int index = 0; index < allMuons.size(); ++index) {
		const MuonPointer muon(allMuons.at(index));
		if (isLooseMuon(muon))
			++nLooseMuons;
	}

	return nLooseMuons < 2;
}

bool TopPairMuPlusJetsReferenceSelection2011::isLooseElectron(const ElectronPointer electron) const {

	bool passesEtAndEta = electron->et() > 15. && fabs(electron->eta()) < 2.5 && !electron->isInCrack();
//	bool passesID(electron->passesElectronID(ElectronID::MVAIDTrigger));
	bool passesIso = electron->pfRelativeIsolation(0.3) < 0.2;

	return passesEtAndEta && passesIso;
}

bool TopPairMuPlusJetsReferenceSelection2011::hasAtLeastThreeGoodJets(const EventPtr event) const {
	const JetCollection goodJets(cleanedJets(event));
	unsigned int nJetsAbove30GeV(0);
	for (unsigned int index = 0; index < goodJets.size(); ++index) {
		if (goodJets.at(index)->pt() > 30.)
			++nJetsAbove30GeV;
	}
	return nJetsAbove30GeV > 2;
}

bool TopPairMuPlusJetsReferenceSelection2011::hasAtLeastFourGoodJets(const EventPtr event) const {
	const JetCollection goodJets(cleanedJets(event));
	unsigned int nJetsAbove30GeV(0);

	for (unsigned int index = 0; index < goodJets.size(); ++index) {
		if (goodJets.at(index)->pt() > 30.)
			++nJetsAbove30GeV;
	}
	return nJetsAbove30GeV > 3;
}

bool TopPairMuPlusJetsReferenceSelection2011::hasAtLeastOneGoodBJet(const EventPtr event) const {
	return cleanedBJets(event).size() > 0;
}

bool TopPairMuPlusJetsReferenceSelection2011::hasAtLeastTwoGoodBJets(const EventPtr event) const {
	return cleanedBJets(event).size() > 1;
}

const LeptonPointer TopPairMuPlusJetsReferenceSelection2011::signalLepton(const EventPtr event) const {
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
		if (isGoodMuon(muon) && isIsolated(muon))
			goodIsolatedMuons.push_back(muon);
	}

	return goodIsolatedMuons.front();

}

const JetCollection TopPairMuPlusJetsReferenceSelection2011::cleanedJets(const EventPtr event) const {
	const JetCollection jets(event->Jets());
	JetCollection cleanedJets;

	if (!hasExactlyOneIsolatedLepton(event)) //if no signal lepton is found, can't clean jets, return them all!
		return jets;

	const LeptonPointer lepton(signalLepton(event));

	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet(jets.at(index));
		if (!jet->isWithinDeltaR(0.3, lepton))
			cleanedJets.push_back(jet);
	}

	return cleanedJets;
}

const JetCollection TopPairMuPlusJetsReferenceSelection2011::cleanedBJets(const EventPtr event) const {
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
