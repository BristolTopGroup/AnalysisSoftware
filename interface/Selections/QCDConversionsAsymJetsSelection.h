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
	virtual bool passesConversionVeto(const EventPtr event) const;

};

} /* namespace BAT */
#endif /* QCDEPLUSJETSPLUSMETCONVERSIONSELECTION_H_ */
