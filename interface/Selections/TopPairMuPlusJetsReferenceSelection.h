/*
 * TopPairReferenceSelection.h
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#ifndef TOPPAIRMUPLUSJETSREFERENCESELECTION_H_
#define TOPPAIRMUPLUSJETSREFERENCESELECTION_H_

#include "BasicSelection.h"

namespace BAT {

namespace TTbarMuPlusJetsReferenceSelection {
enum Step {
	EventCleaningAndTrigger,
	OneIsolatedMuon,
	LooseMuonVeto,
	LooseElectronVeto,
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
				"exactly one isolated muon", //
				"loose muon veto", //
				"loose electron veto", //
				">= 1 jets",
				">= 2 jets",
				">= 3 jets", //
				">= 4 jets", //
				">=1 CSV b-tag", //
				">=2 CSV b-tag" //
		};
}

class TopPairMuPlusJetsReferenceSelection: public BAT::BasicSelection {
public:
	TopPairMuPlusJetsReferenceSelection(unsigned int numberOfSelectionSteps =
			TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~TopPairMuPlusJetsReferenceSelection();

	virtual bool isGoodJet(const JetPointer jet) const;
	virtual bool isBJet(const JetPointer jet) const;
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
	virtual bool passesLooseElectronVeto(const EventPtr event) const;
	virtual bool passesLooseMuonVeto(const EventPtr event) const;
	virtual bool hasAtLeastNGoodJets(const EventPtr event, int Njets) const;
	virtual bool hasAtLeastOneGoodBJet(const EventPtr event) const;
	virtual bool hasAtLeastTwoGoodBJets(const EventPtr event) const;

	virtual const LeptonPointer signalLepton(const EventPtr event) const;
	virtual const JetCollection cleanedJets(const EventPtr event) const;
	virtual const JetCollection cleanedBJets(const EventPtr event) const;
};

typedef boost::shared_ptr<TopPairMuPlusJetsReferenceSelection> TopPairMuPlusJetsReferenceSelectionPointer;

} /* namespace BAT */
#endif /* TOPPAIRMUPLUSJETSREFERENCESELECTION_H_ */
