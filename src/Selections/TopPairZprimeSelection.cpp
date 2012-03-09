/*
 * TopPairZprimeSelection.cpp
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/TopPairZprimeSelection.h"

namespace BAT {

TopPairZprimeSelection::TopPairZprimeSelection(std::vector<std::string> selectionSteps) : //
		TopPairReferenceSelection(selectionSteps) //
{

}

TopPairZprimeSelection::~TopPairZprimeSelection() {
}

bool TopPairZprimeSelection::passesSelectionStep(unsigned int selectionStep) const {
	return false;
}

} /* namespace BAT */
