/*
 * QCDEPlusJetsPlusMETConversionSelection.h
 *
 *  Created on: 17 Apr 2012
 *      Author: kreczko
 */

#ifndef QCDEPLUSJETSPLUSMETCONVERSIONSELECTION_H_
#define QCDEPLUSJETSPLUSMETCONVERSIONSELECTION_H_

#include "TopPairEplusJetsRefAsymJetsSelection.h"
#include "QCDConversionsSelection.h"

namespace BAT {

class QCDConversionsAsymJetsSelection: public BAT::QCDConversionsSelection, public BAT::TopPairEplusJetsRefAsymJetsSelection {
public:
	QCDConversionsAsymJetsSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDConversionsAsymJetsSelection();
	/**
	 * This function has inverted functionality w.r.t reference selection
	 */
	virtual bool passesConversionRejectionMissingLayers(const EventPtr event) const;
	/**
	 * This function has inverted functionality w.r.t reference selection
	 */
	virtual bool passesConversionRejectionPartnerTrack(const EventPtr event) const;

	virtual bool passesSelectionStep(const EventPtr event, unsigned int selectionStep) const;

};

} /* namespace BAT */
#endif /* QCDEPLUSJETSPLUSMETCONVERSIONSELECTION_H_ */
