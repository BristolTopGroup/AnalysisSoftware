/*
 * TopPairReferenceSelection.cpp
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../../interface/HighLevelTriggers.h"
#include "../../interface/GlobalVariables.h"

using namespace std;

namespace BAT {

TopPairEPlusJetsReferenceSelection::TopPairEPlusJetsReferenceSelection(unsigned int numberOfSelectionSteps) : //
		BasicSelection(numberOfSelectionSteps) //
{

}

TopPairEPlusJetsReferenceSelection::~TopPairEPlusJetsReferenceSelection() {
}

bool TopPairEPlusJetsReferenceSelection::isGoodJet(const JetPointer jet) const {
	bool passesPtAndEta = jet->pt() > Globals::minJetPt && fabs(jet->eta()) < Globals::maxAbsoluteJetEta;
	bool passesJetID(false);
	JetAlgorithm::value algo = jet->getUsedAlgorithm();
	if (algo == JetAlgorithm::CA08PF || algo == JetAlgorithm::PF2PAT) { //PFJet
		bool passNOD = jet->NOD() > 1;
		bool passCEF = jet->CEF() < 0.99;
		bool passNHF = jet->NHF() < 0.99;
		bool passNEF = jet->NEF() < 0.99;
		bool passCHF = true;
		bool passNCH = true;
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

bool TopPairEPlusJetsReferenceSelection::isGoodMuon(const MuonPointer muon) const {
	return false;
}

bool TopPairEPlusJetsReferenceSelection::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const {
	TTbarEPlusJetsReferenceSelection::Step step = TTbarEPlusJetsReferenceSelection::Step(selectionStep);
	switch (step) {
	case TTbarEPlusJetsReferenceSelection::EventCleaningAndTrigger:
		return passesEventCleaning(event) && passesTriggerSelection(event);
	case TTbarEPlusJetsReferenceSelection::OneIsolatedElectron:
		return hasExactlyOneIsolatedLepton(event);
	case TTbarEPlusJetsReferenceSelection::LooseMuonVeto:
		return passesLooseLeptonVeto(event);
	case TTbarEPlusJetsReferenceSelection::DiLeptonVeto:
		return passesDileptonVeto(event);
	case TTbarEPlusJetsReferenceSelection::ConversionRejectionMissingLayers:
		return passesConversionRejectionMissingLayers(event);
	case TTbarEPlusJetsReferenceSelection::ConversionRejectionPartnerTrack:
		return passesConversionRejectionPartnerTrack(event);
	case TTbarEPlusJetsReferenceSelection::AtLeastOneGoodJet:
		return hasAtLeastOneGoodJet(event);
	case TTbarEPlusJetsReferenceSelection::AtLeastTwoGoodJets:
			return hasAtLeastTwoGoodJets(event);
	case TTbarEPlusJetsReferenceSelection::AtLeastThreeGoodJets:
			return hasAtLeastThreeGoodJets(event);
	case TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets:
			return hasAtLeastFourGoodJets(event);
	case TTbarEPlusJetsReferenceSelection::AtLeastOneBtag:
			return hasAtLeastOneGoodBJet(event);
	case TTbarEPlusJetsReferenceSelection::AtLeastTwoBtags:
			return hasAtLeastTwoGoodBJets(event);
	default:
		break;
	}

	return false;
}

bool TopPairEPlusJetsReferenceSelection::passesEventCleaning(const EventPtr event) const {
	unsigned int numberOfTracks = event->Tracks().size();
	if (numberOfTracks > 10) {
		//more then 25% of the tracks are high purity
		double fractionOfGoodTracks(event->numberOfHighPurityTracks() / numberOfTracks);
		return fractionOfGoodTracks > 0.25;
	} else
		return event->isBeamScraping() == false;
}

bool TopPairEPlusJetsReferenceSelection::passesTriggerSelection(const EventPtr event) const {
	unsigned int runNumber(event->runnumber());
	if (event->isRealData()) {
		if (runNumber >= 160404 && runNumber <= 163869)
			return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);
		else if (runNumber > 163869 && runNumber <= 165633)
			return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);
		else if (runNumber > 165633 && runNumber <= 178380)
			return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
		else if (runNumber > 178380)
			return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
		else
			return false;
	} else {
		return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30)
				|| event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30)
				|| event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
	}
}

bool TopPairEPlusJetsReferenceSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	unsigned int nIsolatedGoodElectrons(0);
	const ElectronCollection allElectrons(event->Electrons());
	for (unsigned int index = 0; index < allElectrons.size(); ++index) {
		const ElectronPointer electron(allElectrons.at(index));
		if (isGoodElectron(electron) && isIsolated(electron))
			++nIsolatedGoodElectrons;
	}
//	return nIsolatedGoodElectrons == 1;

	return event->GoodPFIsolatedElectrons().size() == 1;
}

bool TopPairEPlusJetsReferenceSelection::isGoodElectron(const ElectronPointer electron) const {
	bool passesEtAndEta = electron->et() > 30. && fabs(electron->eta()) < 2.5 && !electron->isInCrack();
	bool passesD0 = fabs(electron->d0()) < 0.02; //cm
	bool passesDistanceToPV = fabs(electron->ZDistanceToPrimaryVertex()) < 1;
	//since H/E is used at trigger level, use the same cut here:
	bool passesHOverE = electron->HadOverEm() < 0.05; // same for EE and EB
	bool passesID(electron->passesElectronID(Globals::electronID));
	return passesEtAndEta && passesD0 && passesDistanceToPV && passesHOverE && passesID;
}

bool TopPairEPlusJetsReferenceSelection::isIsolated(const LeptonPointer lepton) const {
	return lepton->pfRelativeIsolation(Globals::electronIsolationCone) < 0.1;
}

bool TopPairEPlusJetsReferenceSelection::passesLooseLeptonVeto(const EventPtr event) const {
	unsigned int nLooseMuons(0);
	const MuonCollection allMuons(event->Muons());

	for (unsigned int index = 0; index < allMuons.size(); ++index) {
		const MuonPointer muon(allMuons.at(index));
		if (isLooseMuon(muon))
			++nLooseMuons;
	}

	return nLooseMuons == 0;
}

bool TopPairEPlusJetsReferenceSelection::isLooseMuon(const MuonPointer muon) const {
	bool passesPt = muon->pt() > 10;
	bool passesEta = fabs(muon->eta()) < 2.5;
	bool isGlobal = muon->isGlobal();
	bool isLooselyIsolated = muon->pfRelativeIsolation(Globals::muonIsolationCone) < 0.2;

	return passesPt && passesEta && isGlobal && isLooselyIsolated;
}

bool TopPairEPlusJetsReferenceSelection::passesDileptonVeto(const EventPtr event) const {
	unsigned int nLooseElectrons(0);
	const ElectronCollection allElectrons(event->Electrons());

	for (unsigned int index = 0; index < allElectrons.size(); ++index) {
		const ElectronPointer electron(allElectrons.at(index));
		if (isLooseElectron(electron))
			++nLooseElectrons;
	}

	//good isolated electron is always a loose electron as well
	return nLooseElectrons < 2;
}

bool TopPairEPlusJetsReferenceSelection::isLooseElectron(const ElectronPointer electron) const {

	bool passesEtAndEta = electron->et() > 20. && fabs(electron->eta()) < 2.5 && !electron->isInCrack();
	bool passesID(electron->passesElectronID(ElectronID::CiCLooseMC));
	bool passesIso = electron->pfRelativeIsolation(Globals::electronIsolationCone) < 0.2;

	return passesEtAndEta && passesID && passesIso;
}

bool TopPairEPlusJetsReferenceSelection::passesConversionRejectionMissingLayers(const EventPtr event) const {
	bool hasMissingHitsInInnerLayer(true);

	if (event->GoodPFIsolatedElectrons().size() > 0) {
		const ElectronPointer electron(event->GoodPFIsolatedElectrons().front());
		hasMissingHitsInInnerLayer = electron->innerLayerMissingHits() > 0;
	}

	return !hasMissingHitsInInnerLayer;
}

bool TopPairEPlusJetsReferenceSelection::passesConversionRejectionPartnerTrack(const EventPtr event) const {
	bool hasPartnerTrack(true);

	if (event->GoodPFIsolatedElectrons().size() > 0) {
		const ElectronPointer electron(event->GoodPFIsolatedElectrons().front());
		hasPartnerTrack = fabs(electron->dCotThetaToClosestTrack()) < 0.02 && //
				fabs(electron->distToClosestTrack()) < 0.02;
	}

	return !hasPartnerTrack;
}

bool TopPairEPlusJetsReferenceSelection::hasAtLeastOneGoodJet(const EventPtr event) const {
	return event->GoodElectronCleanedJets().size() > 0;
}

bool TopPairEPlusJetsReferenceSelection::hasAtLeastTwoGoodJets(const EventPtr event) const {
	return event->GoodElectronCleanedJets().size() > 1;
}

bool TopPairEPlusJetsReferenceSelection::hasAtLeastThreeGoodJets(const EventPtr event) const {
	return event->GoodElectronCleanedJets().size() > 2;
}

bool TopPairEPlusJetsReferenceSelection::hasAtLeastFourGoodJets(const EventPtr event) const {
	return event->GoodElectronCleanedJets().size() > 3;
}

bool TopPairEPlusJetsReferenceSelection::hasAtLeastOneGoodBJet(const EventPtr event) const {
	return event->GoodElectronCleanedBJets().size() > 0;
}

bool TopPairEPlusJetsReferenceSelection::hasAtLeastTwoGoodBJets(const EventPtr event) const {
	return event->GoodElectronCleanedBJets().size() > 1;
}

} /* namespace BAT */
