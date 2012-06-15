/*
 * TopPairEplusJetsPlusMETSelection.cpp
 *
 *  Created on: 29 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/TopPairEplusJetsRefAsymJetsSelection.h"

namespace BAT {

TopPairEplusJetsRefAsymJetsSelection::TopPairEplusJetsRefAsymJetsSelection(unsigned int numberOfSelectionSteps) :
		TopPairEPlusJetsReferenceSelection(numberOfSelectionSteps) {

}

TopPairEplusJetsRefAsymJetsSelection::~TopPairEplusJetsRefAsymJetsSelection() {
}

bool TopPairEplusJetsRefAsymJetsSelection::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const {
	if (selectionStep <= TTbarEPlusJetsRefAsymJetsSelection::AtLeastFourGoodJets) {
		return TopPairEPlusJetsReferenceSelection::passesSelectionStep(event, selectionStep);
	} else if (selectionStep == TTbarEPlusJetsRefAsymJetsSelection::AsymmetricJetCuts) {
		return passesAsymmetricJetCuts(event);
	} else if (selectionStep == TTbarEPlusJetsRefAsymJetsSelection::AtLeastOneBtag) {
		return hasAtLeastOneGoodBJet(event);
	} else if (selectionStep == TTbarEPlusJetsRefAsymJetsSelection::AtLeastTwoBtags) {
		return hasAtLeastTwoGoodBJets(event);
	}

	return false;
}

bool TopPairEplusJetsRefAsymJetsSelection::passesAsymmetricJetCuts(const EventPtr event) const {
	const JetCollection goodElectronCleanedJets = cleanedJets(event);
	if (goodElectronCleanedJets.size() < 3) // good jets have a cut of 30 GeV!
		return false;
	JetPointer leadingJet = goodElectronCleanedJets.front();
	JetPointer secondLeadingJet = goodElectronCleanedJets.at(1);
	JetPointer thirdLeadingJet = goodElectronCleanedJets.at(2);
	return leadingJet->pt() > 70 && secondLeadingJet->pt() > 50 && thirdLeadingJet->pt() > 50;
}

} /* namespace BAT */
