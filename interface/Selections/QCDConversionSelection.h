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

class QCDConversionSelection: public BAT::TopPairEPlusJetsReferenceSelection {
public:
	QCDConversionSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDConversionSelection();

	/**
	 * This function has inverted functionality w.r.t reference selection
	 */
	virtual bool passesConversionRejectionMissingLayers(const EventPtr event) const;
	/**
	 * This function has inverted functionality w.r.t reference selection
	 */
	virtual bool passesConversionRejectionPartnerTrack(const EventPtr event) const;

	virtual bool failsEitherConversionVeto(const EventPtr event) const;
};

} /* namespace BAT */
#endif /* QCDCONVERSIONSELECTION_H_ */
