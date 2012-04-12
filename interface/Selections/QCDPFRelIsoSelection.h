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
class QCDPFRelIsoSelection: public BAT::TopPairEPlusJetsReferenceSelection {
public:
	QCDPFRelIsoSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDPFRelIsoSelection();

	virtual bool passesTriggerSelection(const EventPtr event) const;
	virtual bool hasExactlyOneIsolatedLepton(const EventPtr event) const;
	virtual bool passesConversionRejectionMissingLayers(const EventPtr event) const;
	virtual bool passesConversionRejectionPartnerTrack(const EventPtr event) const;
	const ElectronPointer MostIsolatedElectron(const ElectronCollection&) const;
};

} /* namespace BAT */
#endif /* QCDPFRELISOSELECTION_H_ */
