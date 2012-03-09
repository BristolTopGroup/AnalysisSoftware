/*
 * TopPairReferenceSelection.h
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#ifndef TOPPAIRREFERENCESELECTION_H_
#define TOPPAIRREFERENCESELECTION_H_

#include "BasicSelection.h"

namespace BAT {

namespace TTbarEPlusJetsReferenceSelection { //TODO: change to new counting
enum Step {
	EvenCleaningAndTrigger,
	OneIsolatedElectron,
	LooseMuonVeto,
	DiLeptonVeto,
	ConversionRejectionMissinLayers,
	ConversionRejectionPartnerTrack,
	AtLeastOneGoodJets,
	AtLeastTwoGoodJets,
	AtLeastThreeGoodJets,
	AtLeastFourGoodJets,
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
				">=1 SSV b-tag", //
				">=2 SSV b-tag" //
		};
}

class TopPairReferenceSelection: public BAT::BasicSelection {
public:
	TopPairReferenceSelection(std::vector<std::string> selectionSteps);
	virtual ~TopPairReferenceSelection();

	virtual bool isGoodJet(const JetPointer jet) const;
	virtual bool isGoodElectron(const ElectronPointer electron) const;
	virtual bool isGoodMuon(const MuonPointer electron) const;

	//definitions of loose objects
	virtual bool isLooseElectron(const ElectronPointer electron) const;
	virtual bool isLooseMuon(const MuonPointer electron) const;

	virtual bool passesSelectionStep(unsigned int selectionStep) const;
};

} /* namespace BAT */
#endif /* TOPPAIRREFERENCESELECTION_H_ */
