/*
 * QCDNoIsoNoIDSelection.cpp
 *
 *  Created on: 14 Jun 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDNoIsoNoIDSelection.h"

namespace BAT {

QCDNoIsoNoIDSelection::QCDNoIsoNoIDSelection(unsigned int numberOfSelectionSteps) :
		QCDAntiIDEPlusJetsSelection(numberOfSelectionSteps), //
		QCDPFRelIsoEPlusJetsSelection(numberOfSelectionSteps) {

}

QCDNoIsoNoIDSelection::~QCDNoIsoNoIDSelection() {
}

bool QCDNoIsoNoIDSelection::isGoodElectron(const ElectronPointer electron) const {
	return QCDPFRelIsoEPlusJetsSelection::isGoodElectron(electron)
			|| QCDAntiIDEPlusJetsSelection::isGoodElectron(electron);
}

bool QCDNoIsoNoIDSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::hasExactlyOneIsolatedLepton(event);
}

const LeptonPointer QCDNoIsoNoIDSelection::signalLepton(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::signalLepton(event);
}

bool QCDNoIsoNoIDSelection::isIsolated(const LeptonPointer lepton) const {
	return QCDPFRelIsoEPlusJetsSelection::isIsolated(lepton);
}

} /* namespace BAT */
