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

bool QCDConversionsAsymJetsMETSelection::passesConversionVeto(const EventPtr event) const{
	return QCDConversionsSelection::passesConversionVeto(event);
}

} /* namespace BAT */
