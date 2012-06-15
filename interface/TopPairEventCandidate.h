/*
 * TopPairEventCandidate.h
 *
 *  Created on: 9 Jul 2010
 *      Author: kreczko
 */

#ifndef TOPPAIREVENTCANDIDATE_H_
#define TOPPAIREVENTCANDIDATE_H_

#include <boost/shared_ptr.hpp>
#include "Event.h"
#include "RecoObjects/Particle.h"
#include "RecoObjects/Electron.h"
#include "RecoObjects/Jet.h"
#include "Taggers/ConversionTagger.h"
#include "TtbarHypothesis.h"
#include "ReconstructionModules/ReconstructionException.h"

namespace BAT {
namespace NeutrinoSelectionCriterion {
enum value {
	pzClosestToLepton,
	mostCentral,
	pzClosestToLeptonOrMostcentralIfAbove300,
	largestValueOfCosine,
	chi2,
	TopMassDifference
};
}

namespace TTbarReconstructionCriterion {
enum value {
	TopMassDifference, chi2
};
}

namespace TTbarEPlusJetsSelection { //TODO: change to new counting
enum Step {
	FilterOutScraping,
	HighLevelTrigger,
	GoodPrimaryvertex,
	OneIsolatedElectron,
	LooseMuonVeto,
	DileptonVeto,
	ConversionRejection,
	ConversionFinder,
	AtLeastOneGoodJet,
	AtLeastTwoGoodJets,
	AtLeastThreeGoodJets,
	AtLeastFourGoodJets,
	MissingTransverseEnergy,
	AsymmetricJetCuts,
	AtLeastOneBtag,
	AtLeastTwoBtags,
	NUMBER_OF_SELECTION_STEPS
};

const std::string StringSteps[NUMBER_OF_SELECTION_STEPS] = { "Scraping Filter", //
		"High Level Trigger", //
		"good primary vertex", //
		"exactly one isolated electron", //
		"loose muon veto", //
		"di-lepton veto", //
		"Conversion veto (missing hits)", //
		"Conversion finder (partner track)", //
		">= 1 jets", //
		">= 2 jets", //
		">= 3 jets", //
		">= 4 jets", //
		"MET > 20GeV", //
		"jet pT: 1st > 70 GeV, 2nd > 50 GeV", //
		">=1 SSV b-tag", //
		">=2 SSV b-tag" };
}

namespace TTbarMuPlusJetsSelection {
enum Step {
	EventCleaning,
	HighLevelTrigger,
	OneIsolatedMuon,
	LooseMuonVeto,
	LooseElectronVeto,
	AtLeastOneGoodJets,
	AtLeastTwoGoodJets,
	AtLeastThreeGoodJets,
	AtLeastFourGoodJets,
	MissingTransverseEnergy,
	AsymmetricJetCuts,
	AtLeastOneBtag,
	AtLeastTwoBtags,
	NUMBER_OF_SELECTION_STEPS
};

const std::string StringSteps[NUMBER_OF_SELECTION_STEPS] = { "Event cleaning", "High Level Trigger",
		"exactly one isolated muon", "loose muon veto", "electron veto", ">= 1 jets", ">= 2 jets", ">= 3 jets",
		">= 4 jets", "MET > 20GeV", "jet pT: 1st > 70 GeV, 2nd > 50 GeV", ">=1 SSV b-tag", ">=2 SSV b-tag" };
}

/**
 * @deprecated
 * Do not use this class any more
 */
class TopPairEventCandidate: public Event {

protected:
////    static double const matched_angle;// = 0.945666;
////    static double const matched_angle_sigma;// = 0.311091;
//	static double const matched_leptonic_top_mass; // = 178.377;
//	static double const matched_leptonic_top_mass_sigma; // = 31.050;
//	static double const matched_hadronic_W_mass; // = 89.9153;
//	static double const matched_hadronic_W_mass_sigma; // = 13.8711;
//	static double const matched_hadronic_top_mass; // = 182.191;
//	static double const matched_hadronic_top_mass_sigma; // = 22.1484;
////    static double const matched_ptratio;// = 0.18552;
////    static double const matched_ptratio_sigma;// = 0.401973;
//	static double const matched_pt_ttbarSystem; // = 0.0760939;
//	static double const matched_pt_ttbarSystem_sigma; // = 0.0700391;
//	static double const matched_HTSystem; // = 1;
//	static double const matched_HTSystem_sigma; // = 0.1;
//	static double const W_mass;

	ElectronPointer electronFromW;
	JetPointer leptonicBJet, hadronicBJet, jet1FromW, jet2FromW;
	ParticlePointer neutrino1, neutrino2, leptonicW1, leptonicW2, hadronicW, leptonicTop1, leptonicTop2, hadronicTop,
			ttbarResonance;
	unsigned short selectedNeutrino, currentSelectedNeutrino, hadronicBIndex, leptonicBIndex, jet1FromWIndex,
			jet2FromWIndex;
//	bool doneReconstruction_;
//	ConversionTaggerPointer conversionTagger;
//	bool doneConversionTagging;
	std::vector<TtbarHypothesisPointer> solutions;
	compare_totalChi2 compareSolutions;
public:
//	static NeutrinoSelectionCriterion::value usedNeutrinoSelection;
//	static TTbarReconstructionCriterion::value usedTTbarReconstruction;
	TopPairEventCandidate();
	TopPairEventCandidate(const Event& event);
	virtual ~TopPairEventCandidate();

	const JetPointer getLeptonicBJet() const;
	const JetPointer getHadronicBJet() const;
	const JetPointer getJet1FromHadronicW() const;
	const JetPointer getJet2FromHadronicW() const;
	const ElectronPointer getElectronFromWDecay() const;
	const ParticlePointer getNeutrinoFromWDecay() const;
	const ParticlePointer getLeptonicW() const;
	const ParticlePointer getHadronicW() const;
	const ParticlePointer getLeptonicTop() const;
	const ParticlePointer getHadronicTop() const;
	const ParticlePointer getResonance() const;

	double mttbar() const;
	double sumPt() const;
	double HT(unsigned short jetLimit) const;
	double fullHT() const;
	double transverseWmass(const ElectronPointer electron) const;
	double HTSystem() const;
	double PtRatio() const;
	double PtTtbarSystem(unsigned short neutrinoSolution) const;
	const boost::array<double, 2> computeNeutrinoPz();
	void inspectReconstructedEvent() const;
	const std::vector<TtbarHypothesisPointer>& Solutions() const;

};

typedef boost::shared_ptr<TopPairEventCandidate> TopPairEventCandidatePtr;

//TODO: make this class simpler
struct InterestingEvent {
public:
	BAT::TopPairEventCandidatePtr candidate;
//	unsigned long runNumber, eventNumber;
	std::string fileName;

	InterestingEvent(unsigned long run, unsigned long event, std::string file) :
			candidate(), //
//			runNumber(run), //
//			eventNumber(event), //
			fileName(file) {

	}

	InterestingEvent(BAT::TopPairEventCandidatePtr cand, std::string file) :
			candidate(cand), //
//			runNumber(cand.runnumber()), //
//			eventNumber(cand.eventnumber()), //
			fileName(file) {

	}
	~InterestingEvent() {

	}

	void print() {
		std::cout << "run " << candidate->runnumber() << ", event " << candidate->eventnumber() << " (Mttbar: "
				<< candidate->mttbar() << ")" << std::endl;
		std::cout << "located in: " << fileName << std::endl << std::endl;
	}
};

}

#endif /* TOPPAIREVENTCANDIDATE_H_ */
