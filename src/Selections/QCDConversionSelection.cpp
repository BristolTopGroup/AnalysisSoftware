/*
 * QCDConversionSelection.cpp
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDConversionSelection.h"

namespace BAT {

QCDConversionSelection::QCDConversionSelection(unsigned int numberOfSelectionSteps) :
		TopPairEPlusJetsReferenceSelection(numberOfSelectionSteps) {

}

QCDConversionSelection::~QCDConversionSelection() {
}

bool QCDConversionSelection::passesConversionRejectionMissingLayers(const EventPtr event) const{
	return failsEitherConversionVeto(event);
}

bool QCDConversionSelection::passesConversionRejectionPartnerTrack(const EventPtr event) const{
	return failsEitherConversionVeto(event);
}

bool QCDConversionSelection::failsEitherConversionVeto(const EventPtr event) const{
	bool failsMissingHits(!TopPairEPlusJetsReferenceSelection::passesConversionRejectionMissingLayers(event));
	bool failsPartnerTrack(!TopPairEPlusJetsReferenceSelection::passesConversionRejectionPartnerTrack(event));
	return failsMissingHits || failsPartnerTrack;
}

} /* namespace BAT */
