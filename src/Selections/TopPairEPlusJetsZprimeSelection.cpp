/*
 * TopPairZprimeSelection.cpp
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/TopPairEPlusJetsZprimeSelection.h"

namespace BAT {

TopPairEPlusJetsZprimeSelection::TopPairEPlusJetsZprimeSelection(unsigned int numberOfSelectionSteps) : //
		TopPairEPlusJetsReferenceSelection() //
{

}

TopPairEPlusJetsZprimeSelection::~TopPairEPlusJetsZprimeSelection() {
}

bool TopPairEPlusJetsZprimeSelection::passesSelectionStep(const EventPtr event, unsigned int selectionStep) const {
	if (selectionStep <= TTbarEPlusJetsZprimeSelection::AtLeastFourGoodJets) {
		return TopPairEPlusJetsReferenceSelection::passesSelectionStep(event, selectionStep);
	} else if (selectionStep == TTbarEPlusJetsZprimeSelection::MissingTransverseEnergy) {
		return event->MET()->et() > 20.;
	} else if (selectionStep == TTbarEPlusJetsZprimeSelection::AsymmetricJetCuts) {
		return passesAsymmetricJetCuts(event);
	} else if (selectionStep == TTbarEPlusJetsZprimeSelection::AtLeastOneBtag) {
		return hasAtLeastOneGoodBJet(event);
	} else if (selectionStep == TTbarEPlusJetsZprimeSelection::AtLeastTwoBtags) {
		return hasAtLeastTwoGoodBJets(event);
	}

	return false;
}

bool TopPairEPlusJetsZprimeSelection::passesAsymmetricJetCuts(const EventPtr event) const {
	const JetCollection goodElectronCleanedJets = event->GoodElectronCleanedJets();
	if (goodElectronCleanedJets.size() < 3) // good jets have a cut of 30 GeV!
		return false;
	JetPointer leadingJet = goodElectronCleanedJets.front();
	JetPointer secondLeadingJet = goodElectronCleanedJets.at(1);
	JetPointer thirdLeadingJet = goodElectronCleanedJets.at(2);
	return leadingJet->pt() > 70 && secondLeadingJet->pt() > 50 && thirdLeadingJet->pt() > 50;
}

} /* namespace BAT */
