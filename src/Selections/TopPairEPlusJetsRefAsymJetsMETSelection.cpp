/*
 * TopPairZprimeSelection.cpp
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/TopPairEPlusJetsRefAsymJetsMETSelection.h"

namespace BAT {

TopPairEPlusJetsRefAsymJetsMETSelection::TopPairEPlusJetsRefAsymJetsMETSelection(unsigned int numberOfSelectionSteps) : //
		TopPairEplusJetsRefAsymJetsSelection(numberOfSelectionSteps) //
{

}

TopPairEPlusJetsRefAsymJetsMETSelection::~TopPairEPlusJetsRefAsymJetsMETSelection() {
}

bool TopPairEPlusJetsRefAsymJetsMETSelection::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const {
	if (selectionStep <= TTbarEPlusJetsRefAsymJetsMETSelection::AtLeastFourGoodJets) {
		return TopPairEPlusJetsReferenceSelection::passesSelectionStep(event, selectionStep);
	} else if (selectionStep == TTbarEPlusJetsRefAsymJetsMETSelection::MissingTransverseEnergy) {
		return passesMETCut(event);
	} else if (selectionStep == TTbarEPlusJetsRefAsymJetsMETSelection::AsymmetricJetCuts) {
		return passesAsymmetricJetCuts(event);
	} else if (selectionStep == TTbarEPlusJetsRefAsymJetsMETSelection::AtLeastOneBtag) {
		return hasAtLeastOneGoodBJet(event);
	} else if (selectionStep == TTbarEPlusJetsRefAsymJetsMETSelection::AtLeastTwoBtags) {
		return hasAtLeastTwoGoodBJets(event);
	}

	return false;
}

bool TopPairEPlusJetsRefAsymJetsMETSelection::passesMETCut(const EventPtr event) const{
	return event->MET()->et() > 20.;
}

} /* namespace BAT */
