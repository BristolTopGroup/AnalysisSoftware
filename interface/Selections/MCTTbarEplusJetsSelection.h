/*
 * MCTTbarEplusJetsSelection.h
 *
 *  Created on: 19 Apr 2012
 *      Author: kreczko
 */

#ifndef MCTTBAREPLUSJETSSELECTION_H_
#define MCTTBAREPLUSJETSSELECTION_H_

#include "BasicSelection.h"

namespace BAT {

class MCTTbarEplusJetsSelection: public BAT::BasicSelection {
public:
	MCTTbarEplusJetsSelection(unsigned int numberOfSelectionSteps = 1);
	virtual ~MCTTbarEplusJetsSelection();
};

} /* namespace BAT */
#endif /* MCTTBAREPLUSJETSSELECTION_H_ */
