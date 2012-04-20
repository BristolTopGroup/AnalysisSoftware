/*
 * QCDNonIsolatedElectronAsymJetsMETSelection.h
 *
 *  Created on: 20 Apr 2012
 *      Author: kreczko
 */

#ifndef QCDNONISOLATEDELECTRONASYMJETSMETSELECTION_H_
#define QCDNONISOLATEDELECTRONASYMJETSMETSELECTION_H_

#include "QCDNonIsolatedElectronSelection.h"
#include "TopPairEPlusJetsRefAsymJetsMETSelection.h"

namespace BAT {

class QCDNonIsolatedElectronAsymJetsMETSelection: public BAT::QCDNonIsolatedElectronSelection,
		public BAT::TopPairEPlusJetsRefAsymJetsMETSelection {
public:
	QCDNonIsolatedElectronAsymJetsMETSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDNonIsolatedElectronAsymJetsMETSelection();

	virtual bool passesTriggerSelection(const EventPtr event) const;
	virtual bool hasExactlyOneIsolatedLepton(const EventPtr event) const;
};

} /* namespace BAT */
#endif /* QCDNONISOLATEDELECTRONASYMJETSMETSELECTION_H_ */
