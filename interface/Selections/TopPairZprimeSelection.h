/*
 * TopPairZprimeSelection.h
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#ifndef TOPPAIRZPRIMESELECTION_H_
#define TOPPAIRZPRIMESELECTION_H_

#include "TopPairReferenceSelection.h"

namespace BAT {

class TopPairZprimeSelection: public BAT::TopPairReferenceSelection {
public:
	TopPairZprimeSelection(std::vector<std::string> selectionSteps);
	virtual ~TopPairZprimeSelection();

	virtual bool passesSelectionStep(unsigned int selectionStep) const;
};

} /* namespace BAT */
#endif /* TOPPAIRZPRIMESELECTION_H_ */
