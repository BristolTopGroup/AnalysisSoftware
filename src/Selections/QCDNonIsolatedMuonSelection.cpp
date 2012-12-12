/*
 * QCDNonIsolatedMuonSelection.cpp
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDNonIsolatedMuonSelection.h"

namespace BAT {

QCDNonIsolatedMuonSelection::QCDNonIsolatedMuonSelection(unsigned int numberOfSelectionSteps) :
		QCDPFRelIsoMuPlusJetsSelection(numberOfSelectionSteps) {

}

QCDNonIsolatedMuonSelection::~QCDNonIsolatedMuonSelection() {
}

bool QCDNonIsolatedMuonSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	const MuonCollection allMuons(event->Muons());
//
	unsigned int nGoodMuons(0), nGoodNonIsolatedMuons(0), nGoodIsolatedMuons(0);

	for (unsigned int index = 0; index < allMuons.size(); ++index) {
		const MuonPointer muon(allMuons.at(index));
		if (isGoodMuon(muon)) {
			++nGoodMuons;
			if (muon->pfRelativeIsolation(0.4, true) < 0.3)
				++nGoodIsolatedMuons;

			if (muon->pfRelativeIsolation(0.4, true) > 0.3)
				++nGoodNonIsolatedMuons;
		}
	}
	//no muons below 0.2 in PFRelIso and at least one muon with PFIso > 0.2
	return nGoodNonIsolatedMuons > 0 && nGoodIsolatedMuons == 0;
}

} /* namespace BAT */
