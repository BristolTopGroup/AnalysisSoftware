/*
 * QCDConversionSelection.h
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#ifndef QCDCONVERSIONSELECTION_H_
#define QCDCONVERSIONSELECTION_H_

#include "TopPairEPlusJetsReferenceSelection.h"

namespace BAT {

class QCDConversionsSelection: virtual public BAT::TopPairEPlusJetsReferenceSelection {
public:
	QCDConversionsSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDConversionsSelection();

	virtual bool passesConversionVeto(const EventPtr event) const;
};
typedef boost::shared_ptr<QCDConversionsSelection> QCDConversionsSelectionPointer;
} /* namespace BAT */
#endif /* QCDCONVERSIONSELECTION_H_ */
