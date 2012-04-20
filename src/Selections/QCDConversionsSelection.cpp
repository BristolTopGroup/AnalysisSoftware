/*
 * QCDConversionSelection.cpp
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDConversionsSelection.h"

namespace BAT {

QCDConversionsSelection::QCDConversionsSelection(unsigned int numberOfSelectionSteps) :
		TopPairEPlusJetsReferenceSelection(numberOfSelectionSteps) {

}

QCDConversionsSelection::~QCDConversionsSelection() {
}

bool QCDConversionsSelection::passesConversionRejectionMissingLayers(const EventPtr event) const{
	return failsEitherConversionVeto(event);
}

bool QCDConversionsSelection::passesConversionRejectionPartnerTrack(const EventPtr event) const{
	return failsEitherConversionVeto(event);
}

bool QCDConversionsSelection::failsEitherConversionVeto(const EventPtr event) const{
	bool failsMissingHits(!TopPairEPlusJetsReferenceSelection::passesConversionRejectionMissingLayers(event));
	bool failsPartnerTrack(!TopPairEPlusJetsReferenceSelection::passesConversionRejectionPartnerTrack(event));
	return failsMissingHits || failsPartnerTrack;
}

} /* namespace BAT */
