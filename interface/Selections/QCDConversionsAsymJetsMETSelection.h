/*
 * QCDConversionsAsymJetsMETSelection.h
 *
 *  Created on: 20 Apr 2012
 *      Author: kreczko
 */

#ifndef QCDCONVERSIONSASYMJETSMETSELECTION_H_
#define QCDCONVERSIONSASYMJETSMETSELECTION_H_

#include "QCDConversionsSelection.h"
#include "TopPairEPlusJetsRefAsymJetsMETSelection.h"

namespace BAT {

class QCDConversionsAsymJetsMETSelection: public BAT::QCDConversionsSelection,
		public BAT::TopPairEPlusJetsRefAsymJetsMETSelection {
public:
	QCDConversionsAsymJetsMETSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDConversionsAsymJetsMETSelection();

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
#endif /* QCDCONVERSIONSASYMJETSMETSELECTION_H_ */
