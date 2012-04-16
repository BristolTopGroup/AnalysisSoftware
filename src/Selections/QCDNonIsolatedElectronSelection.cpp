/*
 * QCDNonIsolatedElectronSelection.cpp
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDNonIsolatedElectronSelection.h"

namespace BAT {

QCDNonIsolatedElectronSelection::QCDNonIsolatedElectronSelection(unsigned int numberOfSelectionSteps) :
		QCDPFRelIsoSelection(numberOfSelectionSteps) {

}

QCDNonIsolatedElectronSelection::~QCDNonIsolatedElectronSelection() {
}

bool QCDNonIsolatedElectronSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	const ElectronCollection allElectrons(event->Electrons());

	unsigned int nGoodElectrons(0), nGoodIsolatedElectrons(0);

	for (unsigned int index = 0; index < allElectrons.size(); ++index) {
		const ElectronPointer electron(allElectrons.at(index));
		if (isGoodElectron(electron)) {
			++nGoodElectrons;
			if (isIsolated(electron) || electron->relativeIsolation() < 0.1)
				++nGoodIsolatedElectrons;
		}
	}

	return nGoodElectrons > 0 && nGoodIsolatedElectrons == 0;
}

} /* namespace BAT */
