/*
 * BasicSelection.cpp
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/BasicSelection.h"

namespace BAT {

BasicSelection::BasicSelection(unsigned int numberOfSelectionSteps) : //
		numberOfSelectionSteps_(numberOfSelectionSteps) //
{

}

bool BasicSelection::passesFullSelection(const EventPtr event) const {
	return passesSelectionUpToStep(event, numberOfSelectionSteps_);
}

bool BasicSelection::passesSelectionUpToStep(const EventPtr event, unsigned int selectionStep) const {
	bool passes(false);
	for (unsigned int step = 0; step < selectionStep + 1; ++step) {
		bool result(passesSelectionStep(event, step));
		if (!result) {
			passes = false;
			break;
		} else
			passes = true;
	}
	return passes;
}

bool BasicSelection::passesFullSelectionExceptStep(const EventPtr event, unsigned int excludedStep) const {
	bool passes(false);
	for (unsigned int step = 0; step < numberOfSelectionSteps_; ++step) {
		if (step == excludedStep)
			continue;

		bool result(passesSelectionStep(event, step));
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
