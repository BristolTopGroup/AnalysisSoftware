/*
 * QCDNonIsolatedElectronPlusMETSelection.cpp
 *
 *  Created on: 17 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDNonIsolatedElectronAsymJetsSelection.h"

namespace BAT {

QCDNonIsolatedElectronAsymJetsSelection::QCDNonIsolatedElectronAsymJetsSelection(unsigned int numberOfSelectionSteps) :
		QCDNonIsolatedElectronSelection(numberOfSelectionSteps), //
		TopPairEplusJetsRefAsymJetsSelection(numberOfSelectionSteps) {

}

QCDNonIsolatedElectronAsymJetsSelection::~QCDNonIsolatedElectronAsymJetsSelection() {
}

bool QCDNonIsolatedElectronAsymJetsSelection::passesTriggerSelection(const EventPtr event) const {
	return QCDNonIsolatedElectronSelection::passesTriggerSelection(event);
}

bool QCDNonIsolatedElectronAsymJetsSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	return QCDNonIsolatedElectronSelection::hasExactlyOneIsolatedLepton(event);
}
} /* namespace BAT */
