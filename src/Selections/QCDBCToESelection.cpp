/*
 * QCDPFRelIsoSelection.cpp
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDBCToESelection.h"
#include <iostream>

using namespace std;

namespace BAT {

QCDBCToESelection::QCDBCToESelection(unsigned int numberOfSelectionSteps) :
		TopPairEPlusJetsReferenceSelection(numberOfSelectionSteps) {

}

QCDBCToESelection::~QCDBCToESelection() {
}

bool QCDBCToESelection::isGoodElectron(const ElectronPointer electron) const {
	bool passesEtAndEta = electron->et() > 30 && fabs(electron->eta()) < 2.5 && !electron->isInCrack();
	bool passesD0 = fabs(electron->d0()) > 0.02; //cm, inverted cut for non-prompt electrons
	bool passesHOverE = electron->HadOverEm() < 0.05; // same for EE and EB
	bool passesID(electron->passesElectronID(ElectronID::MVAIDTrigger));
	return passesEtAndEta && passesD0 && passesHOverE && passesID;
}

} /* namespace BAT */
