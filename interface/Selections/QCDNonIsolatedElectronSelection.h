/*
 * QCDNonIsolatedElectronSelection.h
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#ifndef QCDNONISOLATEDELECTRONSELECTION_H_
#define QCDNONISOLATEDELECTRONSELECTION_H_

#include "QCDPFRelIsoEPlusJetsSelection.h"

namespace BAT {

class QCDNonIsolatedElectronSelection: public BAT::QCDPFRelIsoEPlusJetsSelection {
public:
	QCDNonIsolatedElectronSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDNonIsolatedElectronSelection();

	virtual bool hasExactlyOneIsolatedLepton(const EventPtr event) const;

};
typedef boost::shared_ptr<QCDNonIsolatedElectronSelection> QCDNonIsolatedElectronSelectionPointer;

} /* namespace BAT */
#endif /* QCDNONISOLATEDELECTRONSELECTION_H_ */
