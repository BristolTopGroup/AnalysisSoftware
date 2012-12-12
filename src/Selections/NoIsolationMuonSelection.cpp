/*
 * QCDNonIsolatedMuonSelection.cpp
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/NoIsolationMuonSelection.h"

namespace BAT {

NoIsolationMuonSelection::NoIsolationMuonSelection(unsigned int numberOfSelectionSteps) :
		QCDPFRelIsoMuPlusJetsSelection(numberOfSelectionSteps) {

}

NoIsolationMuonSelection::~NoIsolationMuonSelection() {
}

bool NoIsolationMuonSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	const MuonCollection allMuons(event->Muons());
//
	unsigned int nGoodMuons(0);

	for (unsigned int index = 0; index < allMuons.size(); ++index) {
		const MuonPointer muon(allMuons.at(index));
		if (isGoodMuon(muon)) {
			++nGoodMuons;
		}
	}
	//no muons below 0.2 in PFRelIso and at least one muon with PFIso > 0.2
	return nGoodMuons > 0;
}

} /* namespace BAT */
