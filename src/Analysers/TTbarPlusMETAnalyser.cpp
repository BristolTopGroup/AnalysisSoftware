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
	const METPointer met = event->MET();

	if (ttbarCand->passesFullTTbarEPlusJetSelection()) {
		histMan->H1D_BJetBinned("MET")->Fill(met->et(), weight);
		histMan->H2D_BJetBinned("METsignificance_vs_MET")->Fill(met->et(),met->significance(), weight);
		if (ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
			histMan->H1D_BJetBinned("MET_withAsymJetsCut")->Fill(met->et(), weight);
			histMan->H2D_BJetBinned("METsignificance_vs_MET_withAsymJetsCut")->Fill(met->et(), met->significance(), weight);
			if (ttbarCand->passesMETCut()) {
				histMan->H1D_BJetBinned("MET_withMETAndAsymJets")->Fill(met->et(), weight);
			}
		}
	}

	if (ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastThreeGoodJets)
			&& ttbarCand->GoodElectronCleanedJets().size() == 3) {
		histMan->H1D_BJetBinned("MET_3jets")->Fill(met->et(), weight);
		histMan->H2D_BJetBinned("METsignificance_vs_MET_3jets")->Fill(met->et(), met->significance(), weight);
		if (ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
			histMan->H1D_BJetBinned("MET_withAsymJetsCut_3jets")->Fill(met->et(), weight);
			histMan->H2D_BJetBinned("METsignificance_vs_MET_withAsymJetsCut_3jets")->Fill(met->et(), met->significance(), weight);
			if (ttbarCand->passesMETCut()) {
				histMan->H1D_BJetBinned("MET_withMETAndAsymJets_3jets")->Fill(met->et(), weight);
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

	histMan->addH2D_BJetBinned("METsignificance_vs_MET", "MET vs MET significance;MET; MET significance", 1000, 0, 1000,
			200, 0, 1000);
	histMan->addH2D_BJetBinned("METsignificance_vs_MET_withAsymJetsCut", "MET vs MET significance;MET; MET significance", 1000, 0, 1000,
			200, 0, 1000);

	histMan->addH2D_BJetBinned("METsignificance_vs_MET_3jets", "MET vs MET significance;MET; MET significance", 1000, 0, 1000,
			200, 0, 1000);
	histMan->addH2D_BJetBinned("METsignificance_vs_MET_withAsymJetsCut_3jets", "MET vs MET significance;MET; MET significance", 1000, 0, 1000,
			200, 0, 1000);
}

TTbarPlusMETAnalyser::TTbarPlusMETAnalyser(HistogramManagerPtr histMan) :
		BasicAnalyser(histMan) {

}

TTbarPlusMETAnalyser::~TTbarPlusMETAnalyser() {
}

} /* namespace BAT */
