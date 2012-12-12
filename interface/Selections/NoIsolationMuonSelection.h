/*
 * NoIsolationMuonSelection.h
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#ifndef NOISOLATIONMUONSELECTION_H_
#define NOISOLATIONMUONSELECTION_H_

#include "QCDPFRelIsoMuPlusJetsSelection.h"

namespace BAT {

class NoIsolationMuonSelection: public BAT::QCDPFRelIsoMuPlusJetsSelection {
public:
	NoIsolationMuonSelection(unsigned int numberOfSelectionSteps =
			TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~NoIsolationMuonSelection();

	virtual bool hasExactlyOneIsolatedLepton(const EventPtr event) const;

};
typedef boost::shared_ptr<NoIsolationMuonSelection> NoIsolationMuonSelectionPointer;

} /* namespace BAT */
#endif /* NOISOLATIONMUONSELECTION_H_ */
