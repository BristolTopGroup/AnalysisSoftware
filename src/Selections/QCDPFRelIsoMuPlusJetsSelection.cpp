/*
 * QCDPFRelIsoSelection.cpp
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDPFRelIsoMuPlusJetsSelection.h"
#include <iostream>

using namespace std;

namespace BAT {

QCDPFRelIsoMuPlusJetsSelection::QCDPFRelIsoMuPlusJetsSelection(unsigned int numberOfSelectionSteps) :
		TopPairMuPlusJetsReferenceSelection(numberOfSelectionSteps) {

}

QCDPFRelIsoMuPlusJetsSelection::~QCDPFRelIsoMuPlusJetsSelection() {
}

bool QCDPFRelIsoMuPlusJetsSelection::passesTriggerSelection(const EventPtr event) const {
	unsigned int runNumber(event->runnumber());
	if (event->isRealData()) {
		if (runNumber >= 160404 && runNumber < 173236)
			return event->HLT(HLTriggers::HLT_IsoMu24);
		else if (runNumber >= 173236)
			return event->HLT(HLTriggers::HLT_IsoMu24_eta2p1);
		else
			return false;
	} else {
		return event->HLT(HLTriggers::HLT_IsoMu24_eta2p1);
	}
}

bool QCDPFRelIsoMuPlusJetsSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	const MuonCollection allMuons(event->Muons());

	unsigned int nGoodMuons(0), nGoodIsolatedMuons(0);

	for (unsigned int index = 0; index < allMuons.size(); ++index) {
		const MuonPointer muon(allMuons.at(index));
		if (isGoodMuon(muon)) {
			++nGoodMuons;
			if (isIsolated(muon))
				++nGoodIsolatedMuons;
		}
	}

	return nGoodMuons > 0 && nGoodIsolatedMuons < 2;
}

const MuonPointer QCDPFRelIsoMuPlusJetsSelection::MostIsolatedMuon(const MuonCollection& muons) const {
	double bestIsolation = 999999999;
	unsigned int bestIsolatedLepton = 990;

	for (unsigned int index = 0; index < muons.size(); ++index) {
		double currentIsolation(muons.at(index)->pfRelativeIsolation(0.4, true));

		if (currentIsolation < bestIsolation) {
			bestIsolation = currentIsolation;
			bestIsolatedLepton = index;
		}
	}
	MuonPointer muon(new Muon());

	if (bestIsolatedLepton < muons.size())
		muon = muons.at(bestIsolatedLepton);

	return muon;
}

unsigned int QCDPFRelIsoMuPlusJetsSelection::prescale(const EventPtr event) const {
	unsigned int runNumber(event->runnumber());
	unsigned int prescale(1);

	if (useNonIsoTrigger_) {
		if (event->isRealData()) {
			if (runNumber >= 160404 && runNumber <= 178419)
				prescale = event->HLTPrescale(HLTriggers::HLT_Mu17_TriCentralJet30);
			else if (runNumber > 178419)
				prescale = event->HLTPrescale(HLTriggers::HLT_Mu17_eta2p1_TriCentralPFJet30);
		} else
			prescale = event->HLTPrescale(HLTriggers::HLT_Mu17_TriCentralJet30);

	}
	return prescale;
}

const LeptonPointer QCDPFRelIsoMuPlusJetsSelection::signalLepton(const EventPtr event) const {
	const MuonCollection allMuons(event->Muons());
	MuonCollection goodMuons;
	for (unsigned int index = 0; index < allMuons.size(); ++index) {
		const MuonPointer muon(allMuons.at(index));
		if (isGoodMuon(muon)) {
			goodMuons.push_back(muon);
		}
	}

	return MostIsolatedMuon(goodMuons);

}

} /* namespace BAT */
