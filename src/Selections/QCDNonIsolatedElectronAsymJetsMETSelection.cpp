/*
 * QCDNonIsolatedElectronAsymJetsMETSelection.cpp
 *
 *  Created on: 20 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDNonIsolatedElectronAsymJetsMETSelection.h"

namespace BAT {

QCDNonIsolatedElectronAsymJetsMETSelection::QCDNonIsolatedElectronAsymJetsMETSelection(unsigned int numberOfSelectionSteps):
	QCDNonIsolatedElectronSelection(numberOfSelectionSteps),//
	TopPairEPlusJetsRefAsymJetsMETSelection(numberOfSelectionSteps){

}

QCDNonIsolatedElectronAsymJetsMETSelection::~QCDNonIsolatedElectronAsymJetsMETSelection() {
}

bool QCDNonIsolatedElectronAsymJetsMETSelection::passesTriggerSelection(const EventPtr event) const {
	return QCDNonIsolatedElectronSelection::passesTriggerSelection(event);
}

bool QCDNonIsolatedElectronAsymJetsMETSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	return QCDNonIsolatedElectronSelection::hasExactlyOneIsolatedLepton(event);
}

} /* namespace BAT */
