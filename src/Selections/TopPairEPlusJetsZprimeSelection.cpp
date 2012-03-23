/*
 * TopPairZprimeSelection.cpp
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/TopPairEPlusJetsZprimeSelection.h"

namespace BAT {

TopPairEPlusJetsZprimeSelection::TopPairEPlusJetsZprimeSelection(unsigned int numberOfSelectionSteps) : //
		TopPairEPlusJetsReferenceSelection() //
{

}

TopPairEPlusJetsZprimeSelection::~TopPairEPlusJetsZprimeSelection() {
}

bool TopPairEPlusJetsZprimeSelection::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const {
	return false;
}

} /* namespace BAT */
