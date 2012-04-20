/*
 * QCDPFRelIsoEPlusAsymJetsMET.cpp
 *
 *  Created on: 20 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDPFRelIsoEPlusAsymJetsMET.h"

namespace BAT {

QCDPFRelIsoEPlusAsymJetsMET::QCDPFRelIsoEPlusAsymJetsMET(unsigned int numberOfSelectionSteps):
	QCDPFRelIsoEPlusJetsSelection(numberOfSelectionSteps),//
	TopPairEPlusJetsRefAsymJetsMETSelection(numberOfSelectionSteps){
}

QCDPFRelIsoEPlusAsymJetsMET::~QCDPFRelIsoEPlusAsymJetsMET() {
}

bool QCDPFRelIsoEPlusAsymJetsMET::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const {
	return TopPairEPlusJetsRefAsymJetsMETSelection::passesSelectionStep(event, selectionStep);
}

bool QCDPFRelIsoEPlusAsymJetsMET::passesTriggerSelection(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::passesTriggerSelection(event);
}

bool QCDPFRelIsoEPlusAsymJetsMET::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::hasExactlyOneIsolatedLepton(event);
}

bool QCDPFRelIsoEPlusAsymJetsMET::passesConversionRejectionMissingLayers(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::passesConversionRejectionMissingLayers(event);
}

bool QCDPFRelIsoEPlusAsymJetsMET::passesConversionRejectionPartnerTrack(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::passesConversionRejectionPartnerTrack(event);
}

unsigned int QCDPFRelIsoEPlusAsymJetsMET::prescale(const EventPtr event) const{
	return QCDPFRelIsoEPlusJetsSelection::prescale(event);
}

const LeptonPointer QCDPFRelIsoEPlusAsymJetsMET::signalLepton(const EventPtr event) const {
	return QCDPFRelIsoEPlusJetsSelection::signalLepton(event);
}



} /* namespace BAT */
