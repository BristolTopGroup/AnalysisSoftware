/*
 * TopPairReferenceSelection2011.h
 *
 *  Created on: 17 Oct 2011
 *      Author: symonds
 */


#ifndef TOPPAIRMUPLUSJETSREFERENCESELECTION2011_H_
#define TOPPAIRMUPLUSJETSREFERENCESELECTION2011_H_

#include "BasicSelection.h"

namespace BAT {

namespace TTbarMuPlusJetsReferenceSelection2011 {
enum Step {
	EventCleaningAndTrigger,
	OneIsolatedMuon,
	LooseMuonVeto,
	LooseElectronVeto,
	AtLeastThreeGoodJets,
	AtLeastFourGoodJets,
	AtLeastOneBtag,
	AtLeastTwoBtags,
	NUMBER_OF_SELECTION_STEPS
};

const std::string StringSteps[NUMBER_OF_SELECTION_STEPS] = { //
		"Event cleaning and High Level Trigger", //
				"exactly one isolated muon", //
				"loose lepton veto", //
				"di-lepton veto", //
				">= 3 jets", //
				">= 4 jets", //
				">=1 CSV b-tag", //
				">=2 CSV b-tag" //
		};
}

class TopPairMuPlusJetsReferenceSelection2011: public BAT::BasicSelection {
public:
	TopPairMuPlusJetsReferenceSelection2011(unsigned int numberOfSelectionSteps =
			TTbarMuPlusJetsReferenceSelection2011::NUMBER_OF_SELECTION_STEPS);
	virtual ~TopPairMuPlusJetsReferenceSelection2011();

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
	virtual bool passesLooseMuonVeto(const EventPtr event) const;
	virtual bool passesLooseElectronVeto(const EventPtr event) const;
	virtual bool hasAtLeastThreeGoodJets(const EventPtr event) const;
	virtual bool hasAtLeastFourGoodJets(const EventPtr event) const;
	virtual bool hasAtLeastOneGoodBJet(const EventPtr event) const;
	virtual bool hasAtLeastTwoGoodBJets(const EventPtr event) const;

	virtual const LeptonPointer signalLepton(const EventPtr event) const;
	virtual const JetCollection cleanedJets(const EventPtr event) const;
	virtual const JetCollection cleanedBJets(const EventPtr event) const;
};

typedef boost::shared_ptr<TopPairMuPlusJetsReferenceSelection2011> TopPairMuPlusJetsReferenceSelection2011Pointer;

} /* namespace BAT */
#endif /* TOPPAIRMUPLUSJETSREFERENCESELECTION2011_H_ */
