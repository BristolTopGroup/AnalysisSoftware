/*
 * QCDEPlusJetsPlusMETPFRelIsoSelection.h
 *
 *  Created on: 17 Apr 2012
 *      Author: kreczko
 */

#ifndef QCDEPLUSJETSPLUSMETPFRELISOSELECTION_H_
#define QCDEPLUSJETSPLUSMETPFRELISOSELECTION_H_

#include "QCDPFRelIsoEPlusJetsSelection.h"
#include "TopPairEplusJetsRefAsymJetsSelection.h"

namespace BAT {

/**
 * Inherits the trigger, electron and conversion selection steps from QCDEPlusJetsPFRelIsoSelection
 * and all other selection steps from TopPairEplusJetsPlusMETSelection
 */
class QCDPFRelIsoEPlusAsymJetsSelection: public BAT::QCDPFRelIsoEPlusJetsSelection,
		public BAT::TopPairEplusJetsRefAsymJetsSelection {
public:
	QCDPFRelIsoEPlusAsymJetsSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDPFRelIsoEPlusAsymJetsSelection();

	virtual bool passesSelectionStep(const EventPtr event, unsigned int selectionStep) const;

	virtual bool passesTriggerSelection(const EventPtr event) const;
	virtual bool hasExactlyOneIsolatedLepton(const EventPtr event) const;
	virtual bool passesConversionVeto(const EventPtr event) const;

	virtual unsigned int prescale(const EventPtr event) const;

	virtual const LeptonPointer signalLepton(const EventPtr event) const;

};

} /* namespace BAT */
#endif /* QCDEPLUSJETSPLUSMETPFRELISOSELECTION_H_ */
