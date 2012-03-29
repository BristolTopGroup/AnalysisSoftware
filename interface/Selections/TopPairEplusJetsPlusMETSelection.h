/*
 * TopPairEplusJetsPlusMETSelection.h
 *
 *  Created on: 29 Mar 2012
 *      Author: kreczko
 */

#ifndef TOPPAIREPLUSJETSPLUSMETSELECTION_H_
#define TOPPAIREPLUSJETSPLUSMETSELECTION_H_

#include "TopPairEPlusJetsZprimeSelection.h"

namespace BAT {

namespace TTbarEPlusJetsPlusMETSelection {
enum Step {
	EvenCleaningAndTrigger,
	OneIsolatedElectron,
	LooseMuonVeto,
	DiLeptonVeto,
	ConversionRejectionMissingLayers,
	ConversionRejectionPartnerTrack,
	AtLeastOneGoodJets,
	AtLeastTwoGoodJets,
	AtLeastThreeGoodJets,
	AtLeastFourGoodJets,
	AsymmetricJetCuts,
	AtLeastOneBtag,
	AtLeastTwoBtags,
	NUMBER_OF_SELECTION_STEPS
};

const std::string StringSteps[NUMBER_OF_SELECTION_STEPS] = { //
		"Event cleaning and High Level Trigger", //
				"exactly one isolated electron", //
				"loose muon veto", //
				"di-lepton veto", //
				"Conversion veto (missing hits)", //
				"Conversion finder (partner track)", //
				">= 1 jets", //
				">= 2 jets", //
				">= 3 jets", //
				">= 4 jets", //
				"jet pT: 1st > 70 GeV, 2nd > 50 GeV", //
				">=1 SSV b-tag", //
				">=2 SSV b-tag" //
		};
}

class TopPairEplusJetsPlusMETSelection: public BAT::TopPairEPlusJetsZprimeSelection {
public:
	TopPairEplusJetsPlusMETSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsPlusMETSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~TopPairEplusJetsPlusMETSelection();

	virtual bool passesSelectionStep(const EventPtr event, unsigned int selectionStep) const;
};

} /* namespace BAT */
#endif /* TOPPAIREPLUSJETSPLUSMETSELECTION_H_ */
