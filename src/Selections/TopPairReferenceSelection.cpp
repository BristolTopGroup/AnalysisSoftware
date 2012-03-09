/*
 * TopPairReferenceSelection.cpp
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/TopPairReferenceSelection.h"

using namespace std;

namespace BAT {

TopPairReferenceSelection::TopPairReferenceSelection(vector<string> selectionSteps) : //
		BasicSelection(selectionSteps) //
{

}

TopPairReferenceSelection::~TopPairReferenceSelection() {
}

bool TopPairReferenceSelection::isGoodJet(const JetPointer jet) const {
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

bool TopPairReferenceSelection::isGoodElectron(const ElectronPointer electron) const {
	bool passesEtAndEta = electron->et() > 30. && fabs(electron->eta()) < 2.5 && !electron->isInCrack();
	bool passesD0 = fabs(electron->d0()) < 0.02; //cm
	bool passesDistanceToPV = fabs(electron->ZDistanceToPrimaryVertex()) < 1;
	//since H/E is used at trigger level, use the same cut here:
	bool passesHOverE = electron->HadOverEm() < 0.05; // same for EE and EB
	bool passesID(false);
	return passesEtAndEta && passesD0 && passesDistanceToPV && passesHOverE && passesID;
}

bool TopPairReferenceSelection::isGoodMuon(const MuonPointer muon) const {
	return false;
}

bool TopPairReferenceSelection::isLooseElectron(const ElectronPointer electron) const {
	return false;
}

bool TopPairReferenceSelection::isLooseMuon(const MuonPointer muon) const {
	return false;
}

bool TopPairReferenceSelection::passesSelectionStep(unsigned int selectionStep) const {
	return false;
}

} /* namespace BAT */
