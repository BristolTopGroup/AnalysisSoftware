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

bool QCDConversionsAsymJetsSelection::passesConversionVeto(const EventPtr event) const{
	return QCDConversionsSelection::passesConversionVeto(event);
}

} /* namespace BAT */
