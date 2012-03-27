/*
 * EventCountAnalyser.cpp
 *
 *  Created on: 24 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Analysers/EventCountAnalyser.h"
#include "../../interface/TopPairEventCandidate.h"

namespace BAT {

void EventCountAnalyser::analyse(const EventPtr event) {
	histMan_->setCurrentHistogramFolder("EventCount");
	double weight = event->weight();
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));

	for (unsigned int step = 0; step < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 2; ++step) {
		bool passesStep = ttbarCand->passesEPlusJetsSelectionStepUpTo((TTbarEPlusJetsSelection::Step) step);
		if (passesStep) {
			histMan_->H1D_BJetBinned("TTbarEplusJetsSelection")->Fill(step, weight);

		}

		if (step < TTbarEPlusJetsSelection::AtLeastFourGoodJets && passesStep) {
			histMan_->H1D_BJetBinned("TTbarEplusJetsSelection_3jets")->Fill(step, weight);
			histMan_->H1D_BJetBinned("TTbarEplusJetsSelection_noMETCut_3jets")->Fill(step, weight);
		}

		if (step < TTbarEPlusJetsSelection::MissingTransverseEnergy && passesStep)
			histMan_->H1D_BJetBinned("TTbarEplusJetsSelection_noMETCut")->Fill(step, weight);
	}

	if (ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastThreeGoodJets)) {
		if (ttbarCand->GoodElectronCleanedJets().size() == 3) {
			histMan_->H1D_BJetBinned("TTbarEplusJetsSelection_3jets")->Fill(
					(double) TTbarEPlusJetsSelection::AtLeastFourGoodJets, weight);
			histMan_->H1D_BJetBinned("TTbarEplusJetsSelection_noMETCut_3jets")->Fill(
					(double) TTbarEPlusJetsSelection::AtLeastFourGoodJets, weight);

			if (ttbarCand->passesMETCut()) {
				histMan_->H1D_BJetBinned("TTbarEplusJetsSelection_3jets")->Fill(
						(double) TTbarEPlusJetsSelection::MissingTransverseEnergy, weight);

				if (ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
					histMan_->H1D_BJetBinned("TTbarEplusJetsSelection_3jets")->Fill(
							(double) TTbarEPlusJetsSelection::AsymmetricJetCuts, weight);
				}
			}

			if (ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
				histMan_->H1D_BJetBinned("TTbarEplusJetsSelection_noMETCut_3jets")->Fill(
						(double) TTbarEPlusJetsSelection::AsymmetricJetCuts, weight);
			}

		}
	}

	if (ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets)
			&& ttbarCand->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AsymmetricJetCuts)) {
		histMan_->H1D_BJetBinned("TTbarEplusJetsSelection_noMETCut")->Fill(
				(double) TTbarEPlusJetsSelection::AsymmetricJetCuts, weight);

	}

}

void EventCountAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder("EventCount");

	//all cuts except the b-tag cuts
	histMan_->addH1D_BJetBinned("TTbarEplusJetsSelection", "TTbarEplusJetsSelection",
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 2, -0.5,
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 2.5);
	//all cuts except the b-tag and MET cuts
	histMan_->addH1D_BJetBinned("TTbarEplusJetsSelection_noMETCut", "TTbarEplusJetsSelection",
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 2, -0.5,
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 2.5);

	//all cuts except the b-tag cuts
	histMan_->addH1D_BJetBinned("TTbarEplusJetsSelection_3jets", "TTbarEplusJetsSelection",
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 2, -0.5,
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 2.5);
	//all cuts except the b-tag and MET cuts
	histMan_->addH1D_BJetBinned("TTbarEplusJetsSelection_noMETCut_3jets", "TTbarEplusJetsSelection",
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 2, -0.5,
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 2.5);
}

EventCountAnalyser::EventCountAnalyser(HistogramManagerPtr histMan, std::string histogramFolder ) :
		BasicAnalyser(histMan, histogramFolder) {

}

EventCountAnalyser::~EventCountAnalyser() {
}

} /* namespace BAT */
