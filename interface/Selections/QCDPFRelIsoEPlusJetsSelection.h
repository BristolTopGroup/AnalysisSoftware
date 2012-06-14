/*
 * QCDPFRelIsoSelection.h
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#ifndef QCDPFRELISOSELECTION_H_
#define QCDPFRELISOSELECTION_H_

#include "TopPairEPlusJetsReferenceSelection.h"

namespace BAT {
/**
 * QCD control region selection for relative isolation based on the top pair e + jets reference selection.
 */
class QCDPFRelIsoEPlusJetsSelection: virtual public BAT::TopPairEPlusJetsReferenceSelection {
public:
	QCDPFRelIsoEPlusJetsSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDPFRelIsoEPlusJetsSelection();

	virtual bool passesTriggerSelection(const EventPtr event) const;
	virtual bool hasExactlyOneIsolatedLepton(const EventPtr event) const;
	virtual bool passesConversionVeto(const EventPtr event) const;
	virtual const ElectronPointer MostIsolatedElectron(const ElectronCollection&) const;

	virtual unsigned int prescale(const EventPtr event) const;

	virtual const LeptonPointer signalLepton(const EventPtr event) const;
};

typedef boost::shared_ptr<QCDPFRelIsoEPlusJetsSelection> QCDEPlusJetsPFRelIsoSelectionPointer;

} /* namespace BAT */
#endif /* QCDPFRELISOSELECTION_H_ */
