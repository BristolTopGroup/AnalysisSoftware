/*
 * QCDNonIsolatedElectronPlusMETSelection.h
 *
 *  Created on: 17 Apr 2012
 *      Author: kreczko
 */

#ifndef QCDNONISOLATEDELECTRONPLUSMETSELECTION_H_
#define QCDNONISOLATEDELECTRONPLUSMETSELECTION_H_

#include "QCDNonIsolatedElectronSelection.h"
#include "TopPairEplusJetsRefAsymJetsSelection.h"

namespace BAT {

class QCDNonIsolatedElectronAsymJetsSelection: public BAT::QCDNonIsolatedElectronSelection,
		public BAT::TopPairEplusJetsRefAsymJetsSelection {
public:
	QCDNonIsolatedElectronAsymJetsSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDNonIsolatedElectronAsymJetsSelection();

	virtual bool passesTriggerSelection(const EventPtr event) const;
	virtual bool hasExactlyOneIsolatedLepton(const EventPtr event) const;
};

typedef boost::shared_ptr<QCDNonIsolatedElectronAsymJetsSelection> QCDNonIsolatedElectronPlusMETSelectionPointer;

} /* namespace BAT */
#endif /* QCDNONISOLATEDELECTRONPLUSMETSELECTION_H_ */
