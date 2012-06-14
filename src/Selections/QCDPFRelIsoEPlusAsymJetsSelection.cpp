/*
 * QCDEPlusJetsPlusMETPFRelIsoSelection.cpp
 *
 *  Created on: 17 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDPFRelIsoEPlusAsymJetsSelection.h"

namespace BAT {

QCDPFRelIsoEPlusAsymJetsSelection::QCDPFRelIsoEPlusAsymJetsSelection(unsigned int numberOfSelectionSteps) :
		QCDPFRelIsoEPlusJetsSelection(numberOfSelectionSteps), //
		TopPairEplusJetsRefAsymJetsSelection(numberOfSelectionSteps) {

}

QCDPFRelIsoEPlusAsymJetsSelection::~QCDPFRelIsoEPlusAsymJetsSelection() {
}

bool QCDPFRelIsoEPlusAsymJetsSelection::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const {
	return TopPairEplusJetsRefAsymJetsSelection::passesSelectionStep(event, selectionStep);
}

bool QCDPFRelIsoEPlusAsymJetsSelection::passesTriggerSelection(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::passesTriggerSelection(event);
}

bool QCDPFRelIsoEPlusAsymJetsSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::hasExactlyOneIsolatedLepton(event);
}

bool QCDPFRelIsoEPlusAsymJetsSelection::passesConversionVeto(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::passesConversionVeto(event);
}

unsigned int QCDPFRelIsoEPlusAsymJetsSelection::prescale(const EventPtr event) const{
	return QCDPFRelIsoEPlusJetsSelection::prescale(event);
}

const LeptonPointer QCDPFRelIsoEPlusAsymJetsSelection::signalLepton(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::signalLepton(event);
}

} /* namespace BAT */
