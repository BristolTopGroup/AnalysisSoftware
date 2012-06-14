/*
 * QCDNoIsoNoIDSelection.h
 *
 *  Created on: 14 Jun 2012
 *      Author: kreczko
 */

#ifndef QCDNOISONOIDSELECTION_H_
#define QCDNOISONOIDSELECTION_H_

#include "QCDAntiIDEPlusJetsSelection.h"
#include "QCDPFRelIsoEPlusJetsSelection.h"

namespace BAT {

class QCDNoIsoNoIDSelection: virtual public BAT::QCDAntiIDEPlusJetsSelection,
		virtual public BAT::QCDPFRelIsoEPlusJetsSelection {
public:
	QCDNoIsoNoIDSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDNoIsoNoIDSelection();

	virtual bool isGoodElectron(const ElectronPointer electron) const;
	virtual bool hasExactlyOneIsolatedLepton(const EventPtr event) const;
	virtual const LeptonPointer signalLepton(const EventPtr event) const;
	virtual bool isIsolated(const LeptonPointer lepton) const;
};

typedef boost::shared_ptr<QCDNoIsoNoIDSelection> QCDNoIsoNoIDSelectionPointer;
} /* namespace BAT */
#endif /* QCDNOISONOIDSELECTION_H_ */
