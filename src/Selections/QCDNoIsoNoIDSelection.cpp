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
	return QCDPFRelIsoEPlusJetsSelection::isGoodElectron(electron) || QCDAntiIDEPlusJetsSelection::isGoodElectron(
			electron);
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

bool QCDNoIsoNoIDSelection::passesSelectionUpToStep(const EventPtr event, unsigned int step) const {
	return QCDPFRelIsoEPlusJetsSelection::passesSelectionUpToStep(event, step);
}

bool QCDNoIsoNoIDSelection::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const{
	return QCDPFRelIsoEPlusJetsSelection::passesSelectionStep(event, selectionStep);
}

unsigned int QCDNoIsoNoIDSelection::prescale(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::prescale(event);
}
const JetCollection QCDNoIsoNoIDSelection::cleanedJets(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::cleanedJets(event);
}
const JetCollection QCDNoIsoNoIDSelection::cleanedBJets(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::cleanedBJets(event);
}

} /* namespace BAT */
