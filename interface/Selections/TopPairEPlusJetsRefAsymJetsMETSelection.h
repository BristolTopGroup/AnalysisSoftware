/*
 * TopPairZprimeSelection.h
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#ifndef TOPPAIRZPRIMESELECTION_H_
#define TOPPAIRZPRIMESELECTION_H_

#include "TopPairEplusJetsRefAsymJetsSelection.h"

namespace BAT {

namespace TTbarEPlusJetsRefAsymJetsMETSelection { //TODO: change to new counting
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
	MissingTransverseEnergy,
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
				"MET > 20GeV", //
				"jet pT: 1st > 70 GeV, 2nd > 50 GeV", //
				">=1 CSV b-tag", //
				">=2 CSV b-tag" //
		};
}

class TopPairEPlusJetsRefAsymJetsMETSelection: public BAT::TopPairEplusJetsRefAsymJetsSelection {
public:
	TopPairEPlusJetsRefAsymJetsMETSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~TopPairEPlusJetsRefAsymJetsMETSelection();

	virtual bool passesSelectionStep(const EventPtr event, unsigned int selectionStep) const;

	virtual bool passesMETCut(const EventPtr event) const;
};

} /* namespace BAT */
#endif /* TOPPAIRZPRIMESELECTION_H_ */
