/*
 * Selections.h
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#ifndef SELECTIONS_H_
#define SELECTIONS_H_

#include "../Analysers/BasicAnalyser.h"
#include "../Selections/TopPairEPlusJetsReferenceSelection.h"

namespace BAT {

class Selections: public BAT::BasicAnalyser {
public:
	Selections(HistogramManagerPtr);
	virtual ~Selections();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();

private:
	SelectionPointer topEplusJetsReferenceSelection_;
};

} /* namespace BAT */
#endif /* SELECTIONS_H_ */
