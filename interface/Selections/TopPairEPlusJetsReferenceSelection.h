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
	EventCleaningAndTrigger, OneIsolatedElectron, LooseMuonVeto, DiLeptonVeto, ConversionVeto,
//	ConversionRejectionPartnerTrack,
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
				"Conversion veto", //
				">= 1 jets", //
				">= 2 jets", //
				">= 3 jets", //
				">= 4 jets", //
				">=1 CSV b-tag", //
				">=2 CSV b-tag" //
		};
}

class TopPairEPlusJetsReferenceSelection: public BAT::BasicSelection {
public:
	TopPairEPlusJetsReferenceSelection(unsigned int numberOfSelectionSteps =
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~TopPairEPlusJetsReferenceSelection();

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
	virtual bool passesLooseLeptonVeto(const EventPtr event) const;
	virtual bool passesDileptonVeto(const EventPtr event) const;
	virtual bool passesConversionVeto(const EventPtr event) const;
	virtual bool hasAtLeastNGoodJets(const EventPtr event, unsigned int Njets) const;
	virtual bool hasAtLeastOneGoodBJet(const EventPtr event) const;
	virtual bool hasAtLeastTwoGoodBJets(const EventPtr event) const;

	virtual const LeptonPointer signalLepton(const EventPtr event) const;
	virtual const JetCollection cleanedJets(const EventPtr event) const;
	virtual const JetCollection cleanedBJets(const EventPtr event) const;
};

typedef boost::shared_ptr<TopPairEPlusJetsReferenceSelection> TopPairEPlusJetsReferenceSelectionPointer;

} /* namespace BAT */
#endif /* TOPPAIRREFERENCESELECTION_H_ */
