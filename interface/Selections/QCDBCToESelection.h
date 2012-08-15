/*
 * QCDPFRelIsoSelection.h
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#ifndef QCDBCTOESELECTION_H_
#define QCDBCTOESELECTION_H_

#include "TopPairEPlusJetsReferenceSelection.h"

namespace BAT {
/**
 * QCD control region selection for relative isolation based on the top pair e + jets reference selection.
 */
class QCDBCToESelection: virtual public BAT::TopPairEPlusJetsReferenceSelection {
public:
	QCDBCToESelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDBCToESelection();
	virtual bool isGoodElectron(const ElectronPointer electron) const;
};

typedef boost::shared_ptr<QCDBCToESelection> QCDBCToESelectionPointer;

} /* namespace BAT */
#endif /* QCDBCTOESELECTION_H_ */
