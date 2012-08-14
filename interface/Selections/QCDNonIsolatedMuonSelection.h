/*
 * QCDNonIsolatedMuonSelection.h
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#ifndef QCDNONISOLATEDMUONSELECTION_H_
#define QCDNONISOLATEDMUONSELECTION_H_

#include "QCDPFRelIsoMuPlusJetsSelection.h"

namespace BAT {

class QCDNonIsolatedMuonSelection: public BAT::QCDPFRelIsoMuPlusJetsSelection {
public:
	QCDNonIsolatedMuonSelection(unsigned int numberOfSelectionSteps =
			TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDNonIsolatedMuonSelection();

	virtual bool hasExactlyOneIsolatedLepton(const EventPtr event) const;

};
typedef boost::shared_ptr<QCDNonIsolatedMuonSelection> QCDNonIsolatedMuonSelectionPointer;

} /* namespace BAT */
#endif /* QCDNONISOLATEDMUONSELECTION_H_ */
