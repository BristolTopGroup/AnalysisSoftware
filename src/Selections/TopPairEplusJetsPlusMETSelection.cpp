/*
 * TopPairEplusJetsPlusMETSelection.cpp
 *
 *  Created on: 29 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/TopPairEplusJetsPlusMETSelection.h"

namespace BAT {

TopPairEplusJetsPlusMETSelection::TopPairEplusJetsPlusMETSelection(unsigned int numberOfSelectionSteps) :
		TopPairEPlusJetsZprimeSelection(numberOfSelectionSteps) {

}

TopPairEplusJetsPlusMETSelection::~TopPairEplusJetsPlusMETSelection() {
}

bool TopPairEplusJetsPlusMETSelection::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const {
	if (selectionStep <= TTbarEPlusJetsPlusMETSelection::AtLeastFourGoodJets) {
		return TopPairEPlusJetsReferenceSelection::passesSelectionStep(event, selectionStep);
	} else if (selectionStep == TTbarEPlusJetsPlusMETSelection::AsymmetricJetCuts) {
		return passesAsymmetricJetCuts(event);
	} else if (selectionStep == TTbarEPlusJetsPlusMETSelection::AtLeastOneBtag) {
		return hasAtLeastOneGoodBJet(event);
	} else if (selectionStep == TTbarEPlusJetsPlusMETSelection::AtLeastTwoBtags) {
		return hasAtLeastTwoGoodBJets(event);
	}

	return false;
}

} /* namespace BAT */
