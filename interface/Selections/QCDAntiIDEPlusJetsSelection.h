/*
 * QCDAntiIDEPlusJetsSelection.h
 *
 *  Created on: 28 May 2012
 *      Author: kreczko
 */

#ifndef QCDANTIIDEPLUSJETSSELECTION_H_
#define QCDANTIIDEPLUSJETSSELECTION_H_

#include "TopPairEPlusJetsReferenceSelection.h"

namespace BAT {

class QCDAntiIDEPlusJetsSelection: public BAT::TopPairEPlusJetsReferenceSelection {
public:
	QCDAntiIDEPlusJetsSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDAntiIDEPlusJetsSelection();
	virtual bool isGoodElectron(const ElectronPointer electron) const;
	virtual bool isIsolated(const LeptonPointer lepton) const;
	virtual void useMVAID(bool use);

protected:
	virtual bool passesWP80AntiID(const ElectronPointer electron) const;
	virtual bool passesMVAAntiID(const ElectronPointer electron) const;
	bool useMVAID_;
};

typedef boost::shared_ptr<QCDAntiIDEPlusJetsSelection> QCDAntiIDEPlusJetsSelectionPointer;

} /* namespace BAT */
#endif /* QCDANTIIDEPLUSJETSSELECTION_H_ */
