/*
 * QCDPFRelIsoEPlusAsymJetsMET.h
 *
 *  Created on: 20 Apr 2012
 *      Author: kreczko
 */

#ifndef QCDPFRELISOEPLUSASYMJETSMET_H_
#define QCDPFRELISOEPLUSASYMJETSMET_H_

#include "QCDPFRelIsoEPlusJetsSelection.h"
#include "TopPairEPlusJetsRefAsymJetsMETSelection.h"

namespace BAT {

class QCDPFRelIsoEPlusAsymJetsMET: public BAT::QCDPFRelIsoEPlusJetsSelection,
		public BAT::TopPairEPlusJetsRefAsymJetsMETSelection {
public:
	QCDPFRelIsoEPlusAsymJetsMET(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDPFRelIsoEPlusAsymJetsMET();

	virtual bool passesSelectionStep(const EventPtr event, unsigned int selectionStep) const;

	virtual bool passesTriggerSelection(const EventPtr event) const;
	virtual bool hasExactlyOneIsolatedLepton(const EventPtr event) const;
	virtual bool passesConversionVeto(const EventPtr event) const;

	virtual unsigned int prescale(const EventPtr event) const;

	virtual const LeptonPointer signalLepton(const EventPtr event) const;
};

} /* namespace BAT */
#endif /* QCDPFRELISOEPLUSASYMJETSMET_H_ */
