/*
 * TopReconstructionAnalyser.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: lkreczko
 */

#include "../../interface/Analysers/TopReconstructionAnalyser.h"
#include "../../interface/TopPairEventCandidate.h"

namespace BAT {

void TopReconstructionAnalyser::analyse(const EventPtr event) {
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));
	double weight =  event->weight();

	histMan->setCurrentHistogramFolder("topReconstruction");
	if ( ttbarCand->passesNMinus1(TTbarEPlusJetsSelection::AtLeastFourGoodJets)) {
		histMan->H1D("numberOfJets")->Fill( ttbarCand->GoodJets().size());
	}

	if ( ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex)
			&&  ttbarCand->ePlusJetsLooseMuonVeto() &&  ttbarCand->electronPlusJetsZVeto()
			&&  ttbarCand->GoodElectronCleanedJets().size() >= 4) {
		const ElectronCollection coll =  ttbarCand->Electrons();
		for (unsigned int index = 0; index < coll.size(); ++index) {
			const ElectronPointer electron = coll.at(index);
			bool passesEt = electron->et() > 30;
			bool passesEta = fabs(electron->superClusterEta()) < 2.5 && electron->isInCrack() == false;
			bool passesID = electron->VBTF_WP70_ElectronID();
			bool noConversion = electron->isFromConversion() == false;
			if (passesEt && passesEta && passesID && noConversion) {
				histMan->H1D("electronD0")->Fill(electron->d0(), weight);
			}
		}
	}
	if ( ttbarCand->passesFullTTbarEPlusJetSelection()) {
//		ChiSquaredBasedTopPairReconstruction reco(ttbarCandidate.GoodPFIsolatedElectrons().front(),
//				ttbarCandidate.MET(), ttbarCandidate.GoodElectronCleanedJets());
		histMan->H1D("numberOfBJets")->Fill( ttbarCand->GoodElectronCleanedBJets().size(), weight);
		try {
			if (Event::usePFIsolation)
				 ttbarCand->reconstructTTbarToEPlusJets( ttbarCand->GoodPFIsolatedElectrons().front());
			else
				 ttbarCand->reconstructTTbarToEPlusJets( ttbarCand->GoodIsolatedElectrons().front());
		} catch (ReconstructionException &e) {
			cout << e.what() << endl;
			return;
		}
		vector<TtbarHypothesisPointer> solutions =  ttbarCand->Solutions();
		const ParticlePointer resonance =  ttbarCand->getResonance();
		double mttbar =  ttbarCand->mttbar();

		ParticlePointer leadingTop, nextToLeadingTop;

		if ( ttbarCand->getHadronicTop()->pt() >  ttbarCand->getLeptonicTop()->pt()) {
			leadingTop =  ttbarCand->getHadronicTop();
			nextToLeadingTop =  ttbarCand->getLeptonicTop();
		} else {
			leadingTop =  ttbarCand->getLeptonicTop();
			nextToLeadingTop =  ttbarCand->getHadronicTop();
		}
		double angleTops = leadingTop->angle(nextToLeadingTop);
		histMan->H1D_BJetBinned("angleTops")->Fill(angleTops, weight);
		histMan->H2D_BJetBinned("angleTops_vs_mttbar")->Fill(mttbar, angleTops, weight);
		histMan->H1D_BJetBinned("mLeptonicTop")->Fill( ttbarCand->getLeptonicTop()->mass(), weight);
		histMan->H1D_BJetBinned("mHadronicTop")->Fill( ttbarCand->getHadronicTop()->mass(), weight);
		histMan->H1D_BJetBinned("mAllTop")->Fill( ttbarCand->getLeptonicTop()->mass(), weight);
		histMan->H1D_BJetBinned("mAllTop")->Fill( ttbarCand->getHadronicTop()->mass(), weight);

		histMan->H1D_BJetBinned("MET")->Fill( ttbarCand->MET()->et(), weight);
		histMan->H2D_BJetBinned("METvsMttbar")->Fill(mttbar,  ttbarCand->MET()->et(), weight);
		histMan->H1D_BJetBinned("HT")->Fill( ttbarCand->fullHT(), weight);
		histMan->H2D_BJetBinned("HTvsMttbar")->Fill(mttbar,  ttbarCand->fullHT(), weight);
		histMan->H1D_BJetBinned("mtW")->Fill( ttbarCand->transverseWmass( ttbarCand->getElectronFromWDecay()),
				weight);

		histMan->H1D_BJetBinned("m3")->Fill( ttbarCand->M3(), weight);

		histMan->H1D_BJetBinned("mttbar")->Fill(mttbar, weight);

		histMan->H1D_BJetBinned("ttbar_pt")->Fill(resonance->pt(), weight);

		histMan->H1D_BJetBinned("ttbar_px")->Fill(resonance->px(), weight);
		histMan->H1D_BJetBinned("ttbar_py")->Fill(resonance->py(), weight);
		histMan->H1D_BJetBinned("ttbar_pz")->Fill(resonance->pz(), weight);

		histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar")->Fill(mttbar, resonance->pt(), weight);

		histMan->H1D("electron_et")->Fill( ttbarCand->getElectronFromWDecay()->et(), weight);
		histMan->H1D_BJetBinned("neutrino_pz")->Fill( ttbarCand->getNeutrinoFromWDecay()->pz(), weight);

		histMan->H1D_BJetBinned("pt_leadingTop")->Fill(leadingTop->pt(), weight);
		histMan->H1D_BJetBinned("pt_NextToLeadingTop")->Fill(nextToLeadingTop->pt(), weight);
		histMan->H2D_BJetBinned("pt_leadingTop_vs_mttbar")->Fill(mttbar, leadingTop->pt(), weight);
		histMan->H2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar")->Fill(mttbar, nextToLeadingTop->pt(), weight);

		if ( ttbarCand->passesMETCut()) {
			histMan->H1D_BJetBinned("angleTops_withMETCut")->Fill(angleTops, weight);
			histMan->H2D_BJetBinned("angleTops_vs_mttbar_withMETCut")->Fill(mttbar, angleTops, weight);
			histMan->H1D_BJetBinned("mttbar_withMETCut")->Fill(mttbar, weight);
			histMan->H1D_BJetBinned("ttbar_pt_withMETCut")->Fill(resonance->pt(), weight);
			histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_withMETCut")->Fill(mttbar, resonance->pt(), weight);

			histMan->H1D_BJetBinned("pt_leadingTop_withMETCut")->Fill(leadingTop->pt(), weight);
			histMan->H1D_BJetBinned("pt_NextToLeadingTop_withMETCut")->Fill(nextToLeadingTop->pt(), weight);
			histMan->H2D_BJetBinned("pt_leadingTop_vs_mttbar_withMETCut")->Fill(mttbar, leadingTop->pt(), weight);
			histMan->H2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withMETCut")->Fill(mttbar, nextToLeadingTop->pt(),
					weight);

			if ( ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
				histMan->H1D_BJetBinned("angleTops_withMETAndAsymJets")->Fill(angleTops, weight);
				histMan->H2D_BJetBinned("angleTops_vs_mttbar_withMETAndAsymJets")->Fill(mttbar, angleTops, weight);
				histMan->H1D_BJetBinned("mttbar_withMETAndAsymJets")->Fill(mttbar, weight);
				histMan->H1D_BJetBinned("ttbar_pt_withMETAndAsymJets")->Fill(resonance->pt(), weight);

				histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_withMETAndAsymJets")->Fill(mttbar, resonance->pt(), weight);

				histMan->H1D_BJetBinned("pt_leadingTop_withMETAndAsymJets")->Fill(leadingTop->pt(), weight);
				histMan->H1D_BJetBinned("pt_NextToLeadingTop_withMETAndAsymJets")->Fill(nextToLeadingTop->pt(), weight);
				histMan->H2D_BJetBinned("pt_leadingTop_vs_mttbar_withMETAndAsymJets")->Fill(mttbar, leadingTop->pt(),
						weight);
				histMan->H2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withMETAndAsymJets")->Fill(mttbar,
						nextToLeadingTop->pt(), weight);
				histMan->H1D_BJetBinned("HT_withMETAndAsymJets")->Fill( ttbarCand->fullHT(), weight);
				histMan->H1D_BJetBinned("mLeptonicTop_withMETAndAsymJets")->Fill(
						 ttbarCand->getLeptonicTop()->mass(), weight);
				histMan->H1D_BJetBinned("mHadronicTop_withMETAndAsymJets")->Fill(
						 ttbarCand->getHadronicTop()->mass(), weight);
				histMan->H1D_BJetBinned("mAllTop_withMETAndAsymJets")->Fill( ttbarCand->getLeptonicTop()->mass(),
						weight);
				histMan->H1D_BJetBinned("mAllTop_withMETAndAsymJets")->Fill( ttbarCand->getHadronicTop()->mass(),
						weight);
				histMan->H1D_BJetBinned("m3_withMETAndAsymJets")->Fill( ttbarCand->M3(), weight);
				histMan->H1D_BJetBinned("MET_withMETAndAsymJets")->Fill( event->MET()->et(), weight);
				histMan->H1D_BJetBinned("mtW_withMETAndAsymJets")->Fill( ttbarCand->transverseWmass(
						 ttbarCand->getElectronFromWDecay()), weight);
			}
		}

		if ( ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
			histMan->H1D_BJetBinned("angleTops_withAsymJetsCut")->Fill(angleTops, weight);
			histMan->H2D_BJetBinned("angleTops_vs_mttbar_withAsymJetsCut")->Fill(mttbar, angleTops, weight);
			histMan->H1D_BJetBinned("mttbar_withAsymJetsCut")->Fill(mttbar, weight);
			histMan->H1D_BJetBinned("ttbar_pt_withAsymJetsCut")->Fill(resonance->pt(), weight);

			histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_withAsymJetsCut")->Fill(mttbar, resonance->pt(), weight);

			histMan->H1D_BJetBinned("pt_leadingTop_withAsymJetsCut")->Fill(leadingTop->pt(), weight);
			histMan->H1D_BJetBinned("pt_NextToLeadingTop_withAsymJetsCut")->Fill(nextToLeadingTop->pt(), weight);
			histMan->H2D_BJetBinned("pt_leadingTop_vs_mttbar_withAsymJetsCut")->Fill(mttbar, leadingTop->pt(), weight);
			histMan->H2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withAsymJetsCut")->Fill(mttbar,
					nextToLeadingTop->pt(), weight);

			histMan->H1D_BJetBinned("MET_withAsymJetsCut")->Fill( event->MET()->et(), weight);

		}
		unsigned int numberOfSolutions(solutions.size());
		for (unsigned int solutionIndex = 0; solutionIndex < solutions.size(); ++solutionIndex) {
			histMan->H1D_BJetBinned("mttbar_allSolutions")->Fill(solutions.at(solutionIndex)->resonance->mass(), weight
					/ numberOfSolutions);
			histMan->H1D_BJetBinned("ttbar_pt_allSolutions")->Fill(solutions.at(solutionIndex)->resonance->pt(), weight
					/ numberOfSolutions);
			histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions")->Fill(
					solutions.at(solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(),
					weight / numberOfSolutions);

			if ( ttbarCand->passesMETCut()) {
				histMan->H1D_BJetBinned("mttbar_allSolutions_withMETCut")->Fill(
						solutions.at(solutionIndex)->resonance->mass(), weight / numberOfSolutions);
				histMan->H1D_BJetBinned("ttbar_pt_allSolutions_withMETCut")->Fill(
						solutions.at(solutionIndex)->resonance->pt(), weight / numberOfSolutions);
				histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withMETCut")->Fill(
						solutions.at(solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(),
						weight / numberOfSolutions);

				if ( ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
					histMan->H1D_BJetBinned("mttbar_allSolutions_withMETAndAsymJets")->Fill(
							solutions.at(solutionIndex)->resonance->mass(), weight / numberOfSolutions);
					histMan->H1D_BJetBinned("ttbar_pt_allSolutions_withMETAndAsymJets")->Fill(solutions.at(
							solutionIndex)->resonance->pt(), weight / numberOfSolutions);
					histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withMETAndAsymJets")->Fill(solutions.at(
							solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight
							/ numberOfSolutions);
				}
			}

		}

		histMan->setCurrentHistogramFolder("topReconstruction/backgroundShape");
		if ( ttbarCand->MET()->et() < 20) {
			histMan->H1D_BJetBinned("mttbar_QCDEnriched")->Fill(mttbar, weight);
			histMan->H1D_BJetBinned("ttbar_pt_QCDEnriched")->Fill(resonance->pt());
		}

//		if (ttbarCandidate.isRealData() && mttbar > 600 && ttbarCandidate.passesEPlusJetsSelectionStepUpTo(
//				TTbarEPlusJetsSelection::AsymmetricJetCuts)) {
//			cout << "run " << ttbarCandidate.runnumber() << ", event " << ttbarCandidate.eventnumber() << ", lumi "
//					<< ttbarCandidate.lumiblock();
//			cout << ", top pair invariant mass = " << mttbar << " GeV" << endl;
//			interestingEvents.push_back(InterestingEvent(ttbarCandidate, eventReader->getCurrentFile()));
//		}

	}

	//for 3 jets only
	histMan->setCurrentHistogramFolder("topReconstruction");
	if ( ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastTwoGoodJets)
			&&  ttbarCand->GoodElectronCleanedJets().size() == 3) {
		try {
			if (Event::usePFIsolation)
				 ttbarCand->reconstructTTbarToEPlusJetsFrom3Jets( ttbarCand->GoodPFIsolatedElectrons().front());
			else
				 ttbarCand->reconstructTTbarToEPlusJetsFrom3Jets( ttbarCand->GoodIsolatedElectrons().front());
		} catch (ReconstructionException &e) {
			cout << e.what() << endl;
			return;
		}
		const ParticlePointer resonance =  ttbarCand->getResonance();
		double mttbar = resonance->mass();
		histMan->H1D_BJetBinned("mttbar_3jets")->Fill(mttbar, weight);
		if ( ttbarCand->MET()->pt() > 20) {
			if ( ttbarCand->GoodJets().front()->pt() > 70 &&  ttbarCand->GoodJets().at(1)->pt() > 50) {
				histMan->H1D_BJetBinned("mttbar_3jets_withMETAndAsymJets")->Fill(mttbar, weight);

			}
		}

	}
}

void TopReconstructionAnalyser::createHistograms() {
	histMan->setCurrentHistogramFolder("topReconstruction");
	histMan->addH1D("electron_et", "electron_et", 500, 0, 500);

	histMan->setCurrentHistogramFolder("topReconstruction/backgroundShape");
	histMan->addH1D_BJetBinned("mttbar_conversions", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_antiIsolated", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_QCDEnriched", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_controlRegion", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_antiID", "mttbar", 5000, 0, 5000);

	histMan->addH1D_BJetBinned("mttbar_3jets_conversions", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_3jets_antiIsolated", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_3jets_antiID", "mttbar", 5000, 0, 5000);

	histMan->addH1D_BJetBinned("mttbar_conversions_withMETCut", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_antiIsolated_withMETCut", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_controlRegion_withMETCut", "mttbar", 5000, 0, 5000);

	histMan->addH1D_BJetBinned("mttbar_conversions_withMETAndAsymJets", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_antiIsolated_withMETAndAsymJets", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_controlRegion_withMETAndAsymJets", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_antiID_withMETAndAsymJets", "mttbar", 5000, 0, 5000);

	histMan->addH1D_BJetBinned("mttbar_3jets_conversions_withMETAndAsymJets", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_3jets_antiIsolated_withMETAndAsymJets", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_3jets_antiID_withMETAndAsymJets", "mttbar", 5000, 0, 5000);

	histMan->addH1D_BJetBinned("mttbar_conversions_withAsymJetsCut", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_antiIsolated_withAsymJetsCut", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_controlRegion_withAsymJetsCut", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("ttbar_pt_QCDEnriched", "ttbar_pt", 1000, 0, 1000);

	histMan->setCurrentHistogramFolder("topReconstruction");
	histMan->addH1D_BJetBinned("mttbar", "mttbar", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_withMETCut", "mttbar_withMETCut", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_withMETAndAsymJets", "mttbar_withMETAndAsymJets", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_withAsymJetsCut", "mttbar_withAsymJetsCut", 5000, 0, 5000);

	histMan->addH1D_BJetBinned("mttbar_3jets", "mttbar_3jet", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_3jets_withMETAndAsymJets", "mttbar_3jet_withMETAndAsymJets", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_allSolutions", "mttbar_allSolutions", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_allSolutions_withMETCut", "mttbar_allSolutions", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mttbar_allSolutions_withMETAndAsymJets", "mttbar_allSolutions", 5000, 0, 5000);

	histMan->addH1D_BJetBinned("mttbar_allSolutions_withAsymJetsCut", "mttbar_allSolutions", 5000, 0, 5000);

	histMan->addH1D_BJetBinned("mLeptonicTop", "mLeptonicTop", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mHadronicTop", "mHadronicTop", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mAllTop", "mAllTop", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("m3", "m3", 5000, 0, 5000);

	histMan->addH1D_BJetBinned("mLeptonicTop_withMETAndAsymJets", "mLeptonicTop", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mHadronicTop_withMETAndAsymJets", "mHadronicTop", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("mAllTop_withMETAndAsymJets", "mAllTop", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("m3_withMETAndAsymJets", "m3", 5000, 0, 5000);

	histMan->addH1D_BJetBinned("ttbar_pt", "ttbar_pt", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("ttbar_pt_withMETCut", "ttbar_pt", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("ttbar_pt_withMETAndAsymJets", "ttbar_pt", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("ttbar_pt_withAsymJetsCut", "ttbar_pt", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("ttbar_pt_allSolutions", "ttbar_pt_allSolutions", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("ttbar_pt_allSolutions_withMETCut", "ttbar_pt_allSolutions", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("ttbar_pt_allSolutions_withMETAndAsymJets", "ttbar_pt_allSolutions", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("ttbar_pt_allSolutions_withAsymJetsCut", "ttbar_pt_allSolutions", 1000, 0, 1000);

	histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar", "ttbar_pt_vs_mttbar", 100, 0, 5000, 40, 0, 1000);
	histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions", "ttbar_pt_vs_mttbar_allSolutions", 100, 0, 5000, 40,
			0, 1000);

	histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_withMETCut", "ttbar_pt_vs_mttbar", 100, 0, 5000, 40, 0, 1000);
	histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withMETCut", "ttbar_pt_vs_mttbar", 100, 0, 5000, 40, 0,
			1000);

	histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_withMETAndAsymJets", "ttbar_pt_vs_mttbar", 100, 0, 5000, 40, 0, 1000);
	histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withMETAndAsymJets", "ttbar_pt_vs_mttbar", 100, 0,
			5000, 40, 0, 1000);

	histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_withAsymJetsCut", "ttbar_pt_vs_mttbar", 100, 0, 5000, 40, 0, 1000);
	histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withAsymJetsCut", "ttbar_pt_vs_mttbar", 100, 0, 5000,
			40, 0, 1000);
	//
	histMan->addH1D_BJetBinned("ttbar_px", "ttbar_px", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("ttbar_py", "ttbar_py", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("ttbar_pz", "ttbar_pz", 1000, 0, 1000);

	histMan->addH1D_BJetBinned("HT", "HT", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("HT_withMETAndAsymJets", "HT", 5000, 0, 5000);
	histMan->addH2D_BJetBinned("HTvsMttbar", "HT vs mttbar", 500, 0, 5000, 500, 0, 5000);
	histMan->addH1D("numberOfJets", "numberOfJets", 10, 0, 10);
	histMan->addH1D("numberOfBJets", "numberOfBJets", 10, 0, 10);
	histMan->addH1D_BJetBinned("MET", "MET", 200, 0, 1000);
	histMan->addH1D_BJetBinned("MET_withMETAndAsymJets", "MET", 200, 0, 1000);
	histMan->addH1D_BJetBinned("MET_withAsymJetsCut", "MET", 200, 0, 1000);
	histMan->addH2D_BJetBinned("METvsMttbar", "MET vs mttbar", 500, 0, 5000, 200, 0, 1000);
	histMan->addH1D_BJetBinned("mtW", "mtW", 600, 0, 600);
	histMan->addH1D_BJetBinned("mtW_withMETAndAsymJets", "mtW", 600, 0, 600);
	histMan->addH1D("electronD0", "electronD0", 1000, 0, 0.2);
	histMan->addH1D_BJetBinned("neutrino_pz", "neutrino_pz", 1000, -500, 500);

	histMan->addH1D_BJetBinned("pt_leadingTop", "pt_leadingTop", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("pt_NextToLeadingTop", "pt_NextToLeadingTop", 1000, 0, 1000);
	histMan->addH2D_BJetBinned("pt_leadingTop_vs_mttbar", "pt_leadingTop_vs_mttbar", 100, 0, 5000, 40, 0, 1000);
	histMan->addH2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar", "pt_NextToLeadingTop_vs_mttbar", 100, 0, 5000, 40, 0,
			1000);
	histMan->addH1D_BJetBinned("pt_leadingTop_withMETCut", "pt_leadingTop_withMETCut", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("pt_NextToLeadingTop_withMETCut", "pt_NextToLeadingTop_withMETCut", 1000, 0, 1000);
	histMan->addH2D_BJetBinned("pt_leadingTop_vs_mttbar_withMETCut", "pt_leadingTop_vs_mttbar_withMETCut", 100, 0,
			5000, 40, 0, 1000);
	histMan->addH2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withMETCut", "pt_NextToLeadingTop_vs_mttbar_withMETCut",
			100, 0, 5000, 40, 0, 1000);

	histMan->addH1D_BJetBinned("pt_leadingTop_withMETAndAsymJets", "pt_leadingTop_withMETAndAsymJets", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("pt_NextToLeadingTop_withMETAndAsymJets", "pt_NextToLeadingTop_withMETAndAsymJets",
			1000, 0, 1000);
	histMan->addH2D_BJetBinned("pt_leadingTop_vs_mttbar_withMETAndAsymJets",
			"pt_leadingTop_vs_mttbar_withMETAndAsymJets", 100, 0, 5000, 40, 0, 1000);
	histMan->addH2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withMETAndAsymJets",
			"pt_NextToLeadingTop_vs_mttbar_withMETAndAsymJets", 100, 0, 5000, 40, 0, 1000);

	histMan->addH1D_BJetBinned("pt_leadingTop_withAsymJetsCut", "pt_leadingTop_withMETAndAsymJets", 1000, 0, 1000);
	histMan->addH1D_BJetBinned("pt_NextToLeadingTop_withAsymJetsCut", "pt_NextToLeadingTop_withMETAndAsymJets", 1000,
			0, 1000);
	histMan->addH2D_BJetBinned("pt_leadingTop_vs_mttbar_withAsymJetsCut", "pt_leadingTop_vs_mttbar_withAsymJetsCut",
			100, 0, 5000, 40, 0, 1000);
	histMan->addH2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withAsymJetsCut",
			"pt_NextToLeadingTop_vs_mttbar_withAsymJetsCut", 100, 0, 5000, 40, 0, 1000);

	histMan->addH1D_BJetBinned("angleTops", "angle between top quarks", 400, 0, 4);
	histMan->addH1D_BJetBinned("angleTops_withMETCut", "angle between top quarks", 400, 0, 4);
	histMan->addH1D_BJetBinned("angleTops_withMETAndAsymJets", "angle between top quarks", 400, 0, 4);
	histMan->addH1D_BJetBinned("angleTops_withAsymJetsCut", "angle between top quarks", 400, 0, 4);

	histMan->addH2D_BJetBinned("angleTops_vs_mttbar", "angleTops_vs_mttbar", 100, 0, 5000, 40, 0, 4);
	histMan->addH2D_BJetBinned("angleTops_vs_mttbar_withMETCut", "angleTops_vs_mttbar", 100, 0, 5000, 40, 0, 4);
	histMan->addH2D_BJetBinned("angleTops_vs_mttbar_withMETAndAsymJets", "angleTops_vs_mttbar", 100, 0, 5000, 40, 0, 4);
	histMan->addH2D_BJetBinned("angleTops_vs_mttbar_withAsymJetsCut", "angleTops_vs_mttbar", 100, 0, 5000, 40, 0, 4);
}

TopReconstructionAnalyser::TopReconstructionAnalyser(boost::shared_ptr<HistogramManager> histMan) :
	BasicAnalyser(histMan){

}

TopReconstructionAnalyser::~TopReconstructionAnalyser() {
}

}
