/*
 * BasicSelection.cpp
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/BasicSelection.h"

namespace BAT {

BasicSelection::BasicSelection(unsigned int numberOfSelectionSteps) : //
		numberOfSelectionSteps_(numberOfSelectionSteps),//
		useNonIsoTrigger_(false){

}

bool BasicSelection::passesFullSelection(const EventPtr event) const {
	return passesSelectionUpToStep(event, numberOfSelectionSteps_ -1);
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

unsigned int BasicSelection::prescale(const EventPtr event) const {
	return 1;
}

void BasicSelection::useNonIsoTrigger(bool use) {
	useNonIsoTrigger_ = use;
}

bool BasicSelection::passesFullSelectionExceptLastTwoSteps(const EventPtr event) const{
	//usually the last two selection steps are >= 1b-tag and >= 2 b=tag
	//-> ommit last two cuts
	// last cut = numberOfSelectionSteps_ -1
	// next to last cut numberOfSelectionSteps_ -2
	// numberOfSelectionSteps_ -3 = all steps except last two
	return passesSelectionUpToStep(event, numberOfSelectionSteps_ - 3);
}

BasicSelection::~BasicSelection() {
}

} /* namespace BAT */
