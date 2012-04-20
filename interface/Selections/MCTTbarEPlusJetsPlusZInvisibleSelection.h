/*
 * MCTTbarEPlusJetsPlusZInvisibleSelection.h
 *
 *  Created on: 19 Apr 2012
 *      Author: kreczko
 */

#ifndef MCTTBAREPLUSJETSPLUSZINVISIBLESELECTION_H_
#define MCTTBAREPLUSJETSPLUSZINVISIBLESELECTION_H_

#include "MCTTbarEplusJetsPlusZSelection.h"

namespace BAT {

class MCTTbarEPlusJetsPlusZInvisibleSelection: public BAT::MCTTbarEplusJetsPlusZSelection {
public:
	MCTTbarEPlusJetsPlusZInvisibleSelection(unsigned int numberOfSelectionSteps = 1);
	virtual ~MCTTbarEPlusJetsPlusZInvisibleSelection();
};

} /* namespace BAT */
#endif /* MCTTBAREPLUSJETSPLUSZINVISIBLESELECTION_H_ */
