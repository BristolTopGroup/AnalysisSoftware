/*
 * TTbarPlusMETAnalyser.cpp
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Analysers/TTbarPlusMETAnalyser.h"
#include "../../interface/TopPairEventCandidate.h"

namespace BAT {

void TTbarPlusMETAnalyser::analyse(const EventPtr event) {
	histMan->setCurrentHistogramFolder("TTbarPlusMetAnalysis");
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));
	double weight = event->weight();

	if (ttbarCand->passesFullTTbarEPlusJetSelection()) {
		histMan->H1D_BJetBinned("MET")->Fill(event->MET()->et(), weight);
		if (ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
			histMan->H1D_BJetBinned("MET_withAsymJetsCut")->Fill(event->MET()->et(), weight);
			if (ttbarCand->passesMETCut()) {
				histMan->H1D_BJetBinned("MET_withMETAndAsymJets")->Fill(event->MET()->et(), weight);
			}
		}
	}

	if (ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastThreeGoodJets)
			&& ttbarCand->GoodElectronCleanedJets().size() == 3) {
		histMan->H1D_BJetBinned("MET_3jets")->Fill(event->MET()->et(), weight);
		if (ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
			histMan->H1D_BJetBinned("MET_withAsymJetsCut_3jets")->Fill(event->MET()->et(), weight);
			if (ttbarCand->passesMETCut()) {
				histMan->H1D_BJetBinned("MET_withMETAndAsymJets_3jets")->Fill(event->MET()->et(), weight);
			}
		}
	}
}

void TTbarPlusMETAnalyser::createHistograms() {
	histMan->setCurrentHistogramFolder("TTbarPlusMetAnalysis");
	histMan->addH1D_BJetBinned("MET", "MET", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("MET_withMETAndAsymJets", "MET", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("MET_withAsymJetsCut", "MET", 1000, 0, 1000);

	histMan->addH1D_BJetBinned("MET_3jets", "MET", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("MET_withMETAndAsymJets_3jets", "MET", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("MET_withAsymJetsCut_3jets", "MET", 1000, 0, 1000);
}

TTbarPlusMETAnalyser::TTbarPlusMETAnalyser(HistogramManagerPtr histMan) :
		BasicAnalyser(histMan) {

}

TTbarPlusMETAnalyser::~TTbarPlusMETAnalyser() {
}

} /* namespace BAT */
