/*
 * TopPairReferenceSelection.cpp
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../../interface/HighLevelTriggers.h"

using namespace std;

namespace BAT {

TopPairEPlusJetsReferenceSelection::TopPairEPlusJetsReferenceSelection(unsigned int numberOfSelectionSteps) : //
		BasicSelection(numberOfSelectionSteps) //
{

}

TopPairEPlusJetsReferenceSelection::~TopPairEPlusJetsReferenceSelection() {
}

bool TopPairEPlusJetsReferenceSelection::isGoodJet(const JetPointer jet) const {
	bool passesPtAndEta = jet->pt() > 30 && fabs(jet->eta()) < 2.5;
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
		;
	} else { //assume CaloJet
		bool passesEMF = jet->emf() > 0.01;
		bool passesN90Hits = jet->n90Hits() > 1;
		bool passesFHPD = jet->fHPD() < 0.98;
		passesJetID = passesEMF && passesN90Hits && passesFHPD;
	}
	return passesPtAndEta && passesJetID;

}

bool TopPairEPlusJetsReferenceSelection::isBJet(const JetPointer jet) const {
	return jet->isBJet(BtagAlgorithm::CombinedSecondaryVertex, BtagAlgorithm::MEDIUM);
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
	case TTbarEPlusJetsReferenceSelection::ConversionVeto:
		return passesConversionVeto(event);
	case TTbarEPlusJetsReferenceSelection::AtLeastOneGoodJet:
		return hasAtLeastNGoodJets(event, 1);
	case TTbarEPlusJetsReferenceSelection::AtLeastTwoGoodJets:
		return hasAtLeastNGoodJets(event, 2);
	case TTbarEPlusJetsReferenceSelection::AtLeastThreeGoodJets:
		return hasAtLeastNGoodJets(event, 3);
	case TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets:
		return hasAtLeastNGoodJets(event, 4);
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
	bool passesAllFilters(!event->isBeamScraping());
	passesAllFilters = passesAllFilters && event->passesHBHENoiseFilter();
	passesAllFilters = passesAllFilters && event->passesCSCTightBeamHaloFilter();
	passesAllFilters = passesAllFilters && event->passesHCALLaserFilter();
	//	passesAllFilters = passesAllFilters && event->passesECALDeadCellFilter();
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

bool TopPairEPlusJetsReferenceSelection::passesTriggerSelection(const EventPtr event) const {
	unsigned int runNumber(event->runnumber());
	if (event->isRealData()) {
		if (Globals::energyInTeV == 7){
			if (runNumber >= 160404 && runNumber <= 163869)
				return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);
			else if (runNumber > 163869 && runNumber <= 165633)
				return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);
			else if (runNumber > 165633 && runNumber <= 178380)
				return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
			else if (runNumber > 178380 && runNumber <= 193621)
				return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
			else if (runNumber > 193621)
				return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30);
			else
				return false;
		} else {
			//2012 data triggers
			return event->HLT(HLTriggers::HLT_Ele27_WP80);
		}
	} else {
		if (Globals::energyInTeV == 8) {
			//Summer12 MC
			//do not use HLTs in Summer12 MC as they don't use JEC
			//https://hypernews.cern.ch/HyperNews/CMS/get/top-trigger/66.html
			//			return true;
			//let's put it back - discussion inconclusive but it is better to have a scale factor than efficiency corrections
//			bool fired_START52_V5 = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
//			bool fired_START52_V9 = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30)
//					|| event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30);
//			bool fired_START53_V7A = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet50_40_30)
//					|| event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet50_40_30);

			return event->HLT(HLTriggers::HLT_Ele27_WP80);

		} else {
			//Fall11 MC
			return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
		}
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
	return nIsolatedGoodElectrons == 1;

//	return event->GoodPFIsolatedElectrons().size() == 1;
}

bool TopPairEPlusJetsReferenceSelection::isGoodElectron(const ElectronPointer electron) const {
	bool passesEtAndEta = electron->et() > 30 && fabs(electron->eta()) < 2.5 && !electron->isInCrack();
	bool passesD0 = fabs(electron->d0()) < 0.02; //cm
//	bool passesDistanceToPV = fabs(electron->ZDistanceToPrimaryVertex()) < 1;
			//since H/E is used at trigger level, use the same cut here:
	bool passesHOverE = electron->HadOverEm() < 0.05; // same for EE and EB
	bool passesID(electron->passesElectronID(ElectronID::MVAIDTrigger));
	return passesEtAndEta && passesD0 &&
//			passesDistanceToPV &&
			passesHOverE && passesID;
}

bool TopPairEPlusJetsReferenceSelection::isIsolated(const LeptonPointer lepton) const {
	const ElectronPointer electron(boost::static_pointer_cast<Electron>(lepton));
	//TODO: switch to rho-based isolation

	return electron->pfRelativeIsolationRhoCorrected() < 0.1;
//	return electron->pfRelativeIsolation(0.3) < 0.1;
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
	bool isGlobalOrTracker = muon->isGlobal() || muon->isTracker();
	bool isLooselyIsolated = muon->pfRelativeIsolation(0.4) < 0.2;

	return passesPt && passesEta && isGlobalOrTracker && isLooselyIsolated;
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

	bool passesEtAndEta = electron->et() > 30. && fabs(electron->eta()) < 2.5 && !electron->isInCrack();
	bool passesID(electron->passesElectronID(ElectronID::MVAIDTrigger));
	bool passesIso = electron->pfRelativeIsolationRhoCorrected() < 0.2;

	return passesEtAndEta && passesID && passesIso;
}

bool TopPairEPlusJetsReferenceSelection::passesConversionVeto(const EventPtr event) const {
	if (!hasExactlyOneIsolatedLepton(event))
		return false;

	const ElectronPointer electron(boost::static_pointer_cast<Electron>(signalLepton(event)));
	return electron->passConversionVeto();
}

bool TopPairEPlusJetsReferenceSelection::hasAtLeastNGoodJets(const EventPtr event, unsigned int Njets) const {
	const JetCollection goodJets(cleanedJets(event));
	unsigned int nJetsAbove30GeV(0);
	for (unsigned int index = 0; index < goodJets.size(); ++index) {
		if (goodJets.at(index)->pt() > 30.)
			++nJetsAbove30GeV;
	}
		return nJetsAbove30GeV >=Njets;
}

bool TopPairEPlusJetsReferenceSelection::hasAtLeastOneGoodBJet(const EventPtr event) const {
	return cleanedBJets(event).size() > 0;
}

bool TopPairEPlusJetsReferenceSelection::hasAtLeastTwoGoodBJets(const EventPtr event) const {
	return cleanedBJets(event).size() > 1;
}

const LeptonPointer TopPairEPlusJetsReferenceSelection::signalLepton(const EventPtr event) const {
	if (!hasExactlyOneIsolatedLepton(event)) {
		cerr << "An error occurred in SignalSelection in event (no = " << event->eventnumber();
		cerr << ", run = " << event->runnumber() << ", lumi = " << event->lumiblock() << "!" << endl;
		cerr << "File = " << event->file() << endl;
		cerr
				<< "Access exception: No signal lepton could be found. Event doesn't pass 'hasExactlyOneIsolatedLepton' selection"
				<< endl;
		throw "Access exception: No signal lepton could be found. Event doesn't pass 'hasExactlyOneIsolatedLepton' selection";
	}

	const ElectronCollection allElectrons(event->Electrons());
	ElectronCollection goodIsolatedElectrons;
	for (unsigned int index = 0; index < allElectrons.size(); ++index) {
		const ElectronPointer electron(allElectrons.at(index));
		if (isGoodElectron(electron) && isIsolated(electron))
			goodIsolatedElectrons.push_back(electron);
	}

	return goodIsolatedElectrons.front();

}

const JetCollection TopPairEPlusJetsReferenceSelection::cleanedJets(const EventPtr event) const {
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

const JetCollection TopPairEPlusJetsReferenceSelection::cleanedBJets(const EventPtr event) const {
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
