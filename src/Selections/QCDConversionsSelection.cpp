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

bool QCDConversionsSelection::passesConversionVeto(const EventPtr event) const{
	//invert the conversion veto
	return !TopPairEPlusJetsReferenceSelection::passesConversionVeto(event);
}

} /* namespace BAT */
