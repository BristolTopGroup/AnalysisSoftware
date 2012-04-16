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

namespace TTbarEPlusJetsReferenceSelection {
enum Step {
	EventCleaningAndTrigger,
	OneIsolatedElectron,
	LooseMuonVeto,
	DiLeptonVeto,
	ConversionRejectionMissingLayers,
	ConversionRejectionPartnerTrack,
	AtLeastOneGoodJet,
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

class TopPairEPlusJetsReferenceSelection: public BAT::BasicSelection {
public:
	TopPairEPlusJetsReferenceSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~TopPairEPlusJetsReferenceSelection();

	virtual bool isGoodJet(const JetPointer jet) const;
	virtual bool isGoodElectron(const ElectronPointer electron) const;
	virtual bool isGoodMuon(const MuonPointer electron) const;

	//definitions of loose objects
	virtual bool isLooseElectron(const ElectronPointer electron) const;
	virtual bool isLooseMuon(const MuonPointer electron) const;
	//isolation definitions
	virtual bool isIsolated(const LeptonPointer lepton) const;

	virtual bool passesSelectionStep(const EventPtr event, unsigned int selectionStep) const;

	virtual bool passesEventCleaning(const EventPtr event) const;
	virtual bool passesTriggerSelection(const EventPtr event) const;
	virtual bool hasExactlyOneIsolatedLepton(const EventPtr event) const;
	virtual bool passesLooseLeptonVeto(const EventPtr event) const;
	virtual bool passesDileptonVeto(const EventPtr event) const;
	virtual bool passesConversionRejectionMissingLayers(const EventPtr event) const;
	virtual bool passesConversionRejectionPartnerTrack(const EventPtr event) const;
	virtual bool hasAtLeastOneGoodJet(const EventPtr event) const;
	virtual bool hasAtLeastTwoGoodJets(const EventPtr event) const;
	virtual bool hasAtLeastThreeGoodJets(const EventPtr event) const;
	virtual bool hasAtLeastFourGoodJets(const EventPtr event) const;
	virtual bool hasAtLeastOneGoodBJet(const EventPtr event) const;
	virtual bool hasAtLeastTwoGoodBJets(const EventPtr event) const;
};

} /* namespace BAT */
#endif /* TOPPAIRREFERENCESELECTION_H_ */
