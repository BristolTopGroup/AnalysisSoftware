/*
 * QCDNonIsolatedElectronSelection.cpp
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDNonIsolatedElectronSelection.h"

namespace BAT {

QCDNonIsolatedElectronSelection::QCDNonIsolatedElectronSelection(unsigned int numberOfSelectionSteps) :
		QCDPFRelIsoEPlusJetsSelection(numberOfSelectionSteps) {

}

QCDNonIsolatedElectronSelection::~QCDNonIsolatedElectronSelection() {
}

bool QCDNonIsolatedElectronSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	const ElectronCollection allElectrons(event->Electrons());
//
	unsigned int nGoodElectrons(0), nGoodNonIsolatedElectrons(0), nGoodIsolatedElectrons(0);

	for (unsigned int index = 0; index < allElectrons.size(); ++index) {
		const ElectronPointer electron(allElectrons.at(index));
		if (isGoodElectron(electron)) {
			++nGoodElectrons;
			if (electron->pfRelativeIsolation(0.3) < 0.2)
				++nGoodIsolatedElectrons;

			if (electron->pfRelativeIsolation(0.3) > 0.2)
				++nGoodNonIsolatedElectrons;
		}
	}
	//no electrons below 0.2 in PFRelIso and at least one electron with PFIso > 0.2
	return nGoodNonIsolatedElectrons > 0 && nGoodIsolatedElectrons == 0;
}
} /* namespace BAT */
