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
	histMan_->setCurrentHistogramFolder("TTbarPlusMetAnalysis");
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));
	double weight = event->weight();
	const METPointer met = event->MET();

	if (ttbarCand->passesFullTTbarEPlusJetSelection()) {
		const ElectronPointer electron = event->GoodPFIsolatedElectrons().front();
		double MT = ttbarCand->transverseWmass(electron);

		histMan_->H1D_BJetBinned("MET")->Fill(met->et(), weight);
		histMan_->H1D_BJetBinned("METsignificance")->Fill(met->significance(), weight);
		histMan_->H2D_BJetBinned("METsignificance_vs_MET")->Fill(met->et(), met->significance(), weight);

		histMan_->H1D_BJetBinned("MT")->Fill(MT, weight);
		if (ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
			histMan_->H1D_BJetBinned("MET_withAsymJetsCut")->Fill(met->et(), weight);
			histMan_->H1D_BJetBinned("METsignificance_withAsymJetsCut")->Fill(met->significance(), weight);
			histMan_->H2D_BJetBinned("METsignificance_vs_MET_withAsymJetsCut")->Fill(met->et(), met->significance(),
					weight);
			histMan_->H1D_BJetBinned("MT_withAsymJetsCut")->Fill(MT, weight);
			if (ttbarCand->passesMETCut()) {
				histMan_->H1D_BJetBinned("MET_withMETAndAsymJets")->Fill(met->et(), weight);
				histMan_->H1D_BJetBinned("METsignificance_withMETAndAsymJets")->Fill(met->et(), weight);

				histMan_->H1D_BJetBinned("MT_withMETAndAsymJets")->Fill(MT, weight);
			}
		}
	}

	if (ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastThreeGoodJets)
			&& ttbarCand->GoodElectronCleanedJets().size() == 3) {
		const ElectronPointer electron = event->GoodPFIsolatedElectrons().front();
		double MT = ttbarCand->transverseWmass(electron);

		histMan_->H1D_BJetBinned("MET_3jets")->Fill(met->et(), weight);
		histMan_->H1D_BJetBinned("METsignificance_3jets")->Fill(met->significance(), weight);
		histMan_->H2D_BJetBinned("METsignificance_vs_MET_3jets")->Fill(met->et(), met->significance(), weight);

		histMan_->H1D_BJetBinned("MT_3jets")->Fill(MT, weight);

		if (ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
			histMan_->H1D_BJetBinned("MET_withAsymJetsCut_3jets")->Fill(met->et(), weight);
			histMan_->H1D_BJetBinned("METsignificance_withAsymJetsCut_3jets")->Fill(met->significance(), weight);
			histMan_->H2D_BJetBinned("METsignificance_vs_MET_withAsymJetsCut_3jets")->Fill(met->et(),
					met->significance(), weight);

			histMan_->H1D_BJetBinned("MT_withAsymJetsCut_3jets")->Fill(MT, weight);

			if (ttbarCand->passesMETCut()) {
				histMan_->H1D_BJetBinned("MET_withMETAndAsymJets_3jets")->Fill(met->et(), weight);
				histMan_->H1D_BJetBinned("METsignificance_withMETAndAsymJets_3jets")->Fill(met->significance(), weight);
				histMan_->H1D_BJetBinned("MT_withMETAndAsymJets_3jets")->Fill(MT, weight);
			}
		}
	}
}

void TTbarPlusMETAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder("TTbarPlusMetAnalysis");
	//missing transverse energy
	histMan_->addH1D_BJetBinned("MET", "MET", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("MET_withMETAndAsymJets", "MET", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("MET_withAsymJetsCut", "MET", 1000, 0, 1000);

	histMan_->addH1D_BJetBinned("MET_3jets", "MET", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("MET_withMETAndAsymJets_3jets", "MET", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("MET_withAsymJetsCut_3jets", "MET", 1000, 0, 1000);

	//MET significance:
	histMan_->addH1D_BJetBinned("METsignificance", "METsignificance", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("METsignificance_withMETAndAsymJets", "METsignificance", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("METsignificance_withAsymJetsCut", "METsignificance", 1000, 0, 1000);

	histMan_->addH1D_BJetBinned("METsignificance_3jets", "METsignificance", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("METsignificance_withMETAndAsymJets_3jets", "METsignificance", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("METsignificance_withAsymJetsCut_3jets", "METsignificance", 1000, 0, 1000);

	//METsignificance vs MET
	histMan_->addH2D_BJetBinned("METsignificance_vs_MET", "MET vs MET significance;MET; MET significance", 1000, 0,
			1000, 1000, 0, 1000);
	histMan_->addH2D_BJetBinned("METsignificance_vs_MET_withAsymJetsCut",
			"MET vs MET significance;MET; MET significance", 1000, 0, 1000, 1000, 0, 1000);

	histMan_->addH2D_BJetBinned("METsignificance_vs_MET_3jets", "MET vs MET significance;MET; MET significance", 1000,
			0, 1000, 1000, 0, 1000);
	histMan_->addH2D_BJetBinned("METsignificance_vs_MET_withAsymJetsCut_3jets",
			"MET vs MET significance;MET; MET significance", 1000, 0, 1000, 1000, 0, 1000);

	//transverse mass:
	histMan_->addH1D_BJetBinned("MT", "Transverse Mass(lepton,MET);M_{T}(l,MET); Events", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("MT_withAsymJetsCut", "Transverse Mass(lepton,MET);M_{T}(l,MET);Events", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("MT_withMETAndAsymJets", "Transverse Mass(lepton,MET)", 1000, 0, 1000);

	histMan_->addH1D_BJetBinned("MT_3jets", "Transverse Mass(lepton,MET);M_{T}(l,MET); Events", 1000, 0, 1000);
	histMan_->addH1D_BJetBinned("MT_withAsymJetsCut_3jets", "Transverse Mass(lepton,MET);M_{T}(l,MET);Events", 1000, 0,
			1000);
	histMan_->addH1D_BJetBinned("MT_withMETAndAsymJets_3jets", "Transverse Mass(lepton,MET)", 1000, 0, 1000);
}

TTbarPlusMETAnalyser::TTbarPlusMETAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		metAnalyser(new METAnalyser(histMan, histogramFolder)) {

}

TTbarPlusMETAnalyser::~TTbarPlusMETAnalyser() {
}

} /* namespace BAT */
