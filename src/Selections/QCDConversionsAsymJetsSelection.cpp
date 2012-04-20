/*
 * QCDEPlusJetsPlusMETConversionSelection.cpp
 *
 *  Created on: 17 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDConversionsAsymJetsSelection.h"

namespace BAT {

QCDConversionsAsymJetsSelection::QCDConversionsAsymJetsSelection(unsigned int numberOfSelectionSteps) :
		QCDConversionsSelection(numberOfSelectionSteps), //
		TopPairEplusJetsRefAsymJetsSelection(numberOfSelectionSteps) {

}

QCDConversionsAsymJetsSelection::~QCDConversionsAsymJetsSelection() {
}

bool QCDConversionsAsymJetsSelection::passesConversionRejectionMissingLayers(const EventPtr event) const {
	return QCDConversionsSelection::passesConversionRejectionMissingLayers(event);
}

bool QCDConversionsAsymJetsSelection::passesConversionRejectionPartnerTrack(const EventPtr event) const {
	return QCDConversionsSelection::passesConversionRejectionPartnerTrack(event);
}

bool QCDConversionsAsymJetsSelection::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const{
	return TopPairEplusJetsRefAsymJetsSelection::passesSelectionStep(event, selectionStep);
}

} /* namespace BAT */
