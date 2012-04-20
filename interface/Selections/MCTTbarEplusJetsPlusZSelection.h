/*
 * MCTTbarEplusJetsPlusZSelection.h
 *
 *  Created on: 19 Apr 2012
 *      Author: kreczko
 */

#ifndef MCTTBAREPLUSJETSPLUSZSELECTION_H_
#define MCTTBAREPLUSJETSPLUSZSELECTION_H_

#include "MCTTbarEplusJetsSelection.h"

namespace BAT {

class MCTTbarEplusJetsPlusZSelection: public BAT::MCTTbarEplusJetsSelection {
public:
	MCTTbarEplusJetsPlusZSelection(unsigned int numberOfSelectionSteps = 1);
	virtual ~MCTTbarEplusJetsPlusZSelection();
};

} /* namespace BAT */
#endif /* MCTTBAREPLUSJETSPLUSZSELECTION_H_ */
