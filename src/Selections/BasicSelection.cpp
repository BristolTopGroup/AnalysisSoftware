/*
 * BasicSelection.cpp
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/BasicSelection.h"

namespace BAT {

BasicSelection::BasicSelection(std::vector<std::string> selectionSteps) : //
		selectionSteps_(selectionSteps), //
		numberOfSelectionSteps_(selectionSteps.size()) //
{

}

bool BasicSelection::passesFullSelection(const EventPointer event) const {
	return passesSelectionUpToStep(numberOfSelectionSteps_);
}

bool BasicSelection::passesSelectionUpToStep(unsigned int selectionStep) const {
	bool passes(false);
	for (unsigned int step = 0; step < selectionStep; ++step) {
		bool result(passesSelectionStep(step));
		if (!result) {
			passes = false;
			break;
		} else
			passes = true;
	}
	return passes;
}

bool BasicSelection::passesFullSelectionExceptStep(unsigned int selectionStep) const {
	bool passes(false);
	for (unsigned int step = 0; step < numberOfSelectionSteps_; ++step) {
		if (step == selectionStep)
			continue;

		bool result(passesSelectionStep(step));
		if (!result) {
			passes = false;
			break;
		} else
			passes = true;
	}
	return passes;
}

BasicSelection::~BasicSelection() {
}

} /* namespace BAT */
