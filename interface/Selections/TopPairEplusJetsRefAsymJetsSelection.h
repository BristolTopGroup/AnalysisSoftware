/*
 * TopPairEplusJetsPlusMETSelection.h
 *
 *  Created on: 29 Mar 2012
 *      Author: kreczko
 */

#ifndef TOPPAIREPLUSJETSPLUSMETSELECTION_H_
#define TOPPAIREPLUSJETSPLUSMETSELECTION_H_

#include "TopPairEPlusJetsReferenceSelection.h"

namespace BAT {

namespace TTbarEPlusJetsRefAsymJetsSelection {
enum Step {
	EvenCleaningAndTrigger,
	OneIsolatedElectron,
	LooseMuonVeto,
	DiLeptonVeto,
	ConversionVeto,
//	ConversionRejectionPartnerTrack,
//	AtLeastOneGoodJets,
//	AtLeastTwoGoodJets,
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
				"Conversion veto", //
//				"Conversion finder (partner track)", //
//				">= 1 jets", //
//				">= 2 jets", //
				">= 3 jets", //
				">= 4 jets", //
				"jet pT: 1st > 70 GeV, 2nd > 50 GeV", //
				">=1 CSV b-tag", //
				">=2 CSV b-tag" //
		};
}

class TopPairEplusJetsRefAsymJetsSelection: virtual public BAT::TopPairEPlusJetsReferenceSelection {
public:
	TopPairEplusJetsRefAsymJetsSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~TopPairEplusJetsRefAsymJetsSelection();

	virtual bool passesSelectionStep(const EventPtr event, unsigned int selectionStep) const;

	virtual bool passesAsymmetricJetCuts(const EventPtr event) const;
};

} /* namespace BAT */
#endif /* TOPPAIREPLUSJETSPLUSMETSELECTION_H_ */
