/*
 * QCDConversionsAsymJetsMETSelection.cpp
 *
 *  Created on: 20 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDConversionsAsymJetsMETSelection.h"

namespace BAT {

QCDConversionsAsymJetsMETSelection::QCDConversionsAsymJetsMETSelection(unsigned int numberOfSelectionSteps) :
		QCDConversionsSelection(numberOfSelectionSteps), //
		TopPairEPlusJetsRefAsymJetsMETSelection(numberOfSelectionSteps) {

}

QCDConversionsAsymJetsMETSelection::~QCDConversionsAsymJetsMETSelection() {
}

bool QCDConversionsAsymJetsMETSelection::passesConversionRejectionMissingLayers(const EventPtr event) const {
	return QCDConversionsSelection::passesConversionRejectionMissingLayers(event);
}

bool QCDConversionsAsymJetsMETSelection::passesConversionRejectionPartnerTrack(const EventPtr event) const {
	return QCDConversionsSelection::passesConversionRejectionPartnerTrack(event);
}

bool QCDConversionsAsymJetsMETSelection::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const {
	return TopPairEPlusJetsRefAsymJetsMETSelection::passesSelectionStep(event, selectionStep);
}

} /* namespace BAT */
