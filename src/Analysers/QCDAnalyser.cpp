/*
 * QCDAnalyser.cpp
 *
 *  Created on: Jun 28, 2011
 *      Author: lkreczko
 */

#include "../../interface/Analysers/QCDAnalyser.h"
#include "../../interface/Readers/NTupleEventReader.h"
#include "../../interface/GlobalVariables.h"

namespace BAT {

QCDAnalyser::QCDAnalyser(HistogramManagerPtr histMan, std::string histogramFolder ) :
		BasicAnalyser(histMan, histogramFolder) {

}

QCDAnalyser::~QCDAnalyser() {
}

void QCDAnalyser::analyse(const EventPtr event) {
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));
	double weight = ttbarCand->weight();
//	TopPairEventCandidate event(Event);
	histMan_->setCurrentHistogramFolder("QCDStudy");
	if (ttbarCand->passesEPlusJetsRelIsoSelection()) {
		const ElectronPointer electron = ttbarCand->MostIsolatedElectron(ttbarCand->Electrons());
		histMan_->H1D_JetBinned("QCDest_CombRelIso")->Fill(electron->relativeIsolation(), weight);

		if (ttbarCand->GoodElectronCleanedJets().size() >= 4) {
			double rho = ttbarCand->rho();
			histMan_->H1D_BJetBinned("RelativeIsolation_PUCorrected")->Fill(electron->relativeIsolationPUCorrected(rho),
					weight);
			if (ttbarCand->passesMETCut() && ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
				histMan_->H1D_BJetBinned("RelativeIsolation_PUCorrected_WithMETCutAndAsymJetCuts")->Fill(
						electron->relativeIsolationPUCorrected(rho), weight);
			}
		}

		if (ttbarCand->GoodElectronCleanedBJets().size() >= 1)
			histMan_->H1D_JetBinned("QCDest_CombRelIso_1btag")->Fill(electron->relativeIsolation(), weight);

		if (ttbarCand->GoodElectronCleanedBJets().size() >= 2)
			histMan_->H1D_JetBinned("QCDest_CombRelIso_2btag")->Fill(electron->relativeIsolation(), weight);
	}

	if (ttbarCand->passesEPlusJetsPFIsoSelection() && Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
		const ElectronPointer electron = ttbarCand->MostPFIsolatedElectron(ttbarCand->Electrons());
		histMan_->H1D_JetBinned("QCDest_PFIsolation")->Fill(electron->pfIsolation(), weight);

		if (ttbarCand->GoodElectronCleanedBJets().size() == 1) {
			histMan_->H1D_JetBinned("QCDest_PFIsolation_1btag")->Fill(electron->pfIsolation(), weight);
			if (ttbarCand->passesAsymmetricElectronCleanedJetCuts() && ttbarCand->passesMETCut()) {
				histMan_->H1D_JetBinned("QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts")->Fill(
						electron->pfIsolation(), weight);
			}
		}

		if (ttbarCand->GoodElectronCleanedBJets().size() == 2) {
			histMan_->H1D_JetBinned("QCDest_PFIsolation_2btag")->Fill(electron->pfIsolation(), weight);
			if (ttbarCand->passesAsymmetricElectronCleanedJetCuts() && ttbarCand->passesMETCut()) {
				histMan_->H1D_JetBinned("QCDest_PFIsolation_2btag_WithMETCutAndAsymJetCuts")->Fill(
						electron->pfIsolation(), weight);
			}
		}

		if (ttbarCand->GoodElectronCleanedJets().size() >= 4) {
			histMan_->H1D_BJetBinned("PFIsolation")->Fill(electron->pfIsolation(), weight);
		}

		if (ttbarCand->MET()->pt() > 20) {
			histMan_->H1D_JetBinned("QCDest_PFIsolation_WithMETCut")->Fill(electron->pfIsolation(), weight);

			if (ttbarCand->GoodElectronCleanedJets().size() >= 2) {
				if (ttbarCand->GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCand->GoodElectronCleanedJets().at(1)->pt() > 50) {
					histMan_->H1D_JetBinned("QCDest_PFIsolation_WithMETCutAndAsymJetCuts")->Fill(electron->pfIsolation(),
							weight);

					if (ttbarCand->GoodElectronCleanedJets().size() == 3)
						histMan_->H1D_BJetBinned("PFIsolation_3jets_WithMETCutAndAsymJetCuts")->Fill(
								electron->pfIsolation(), weight);

					if (ttbarCand->GoodElectronCleanedJets().size() >= 4) {
						histMan_->H1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts_DR03")->Fill(
								electron->pfRelativeIsolation(0.3), weight);
						histMan_->H1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts_DR04")->Fill(
								electron->pfRelativeIsolation(0.4), weight);
						histMan_->H1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts_DR05")->Fill(
								electron->pfRelativeIsolation(0.5), weight);
						histMan_->H1D_BJetBinned("DirectionalIsolation_WithMETCutAndAsymJetCuts_DR02")->Fill(
								electron->directionalIsolation(0.2) / electron->et(), weight);
						histMan_->H1D_BJetBinned("DirectionalIsolation_WithMETCutAndAsymJetCuts_DR03")->Fill(
								electron->directionalIsolation(0.3) / electron->et(), weight);

						histMan_->H1D_BJetBinned("DirectionalIsolationWithGaussianFallOff_WithMETCutAndAsymJetCuts_DR02")->Fill(
								electron->directionalIsolationWithGaussianFallOff(0.2) / electron->et(), weight);
						histMan_->H1D_BJetBinned("DirectionalIsolationWithGaussianFallOff_WithMETCutAndAsymJetCuts_DR03")->Fill(
								electron->directionalIsolationWithGaussianFallOff(0.3) / electron->et(), weight);
					}

				}
			}
		}

	}

	if (ttbarCand->passesEPlusJetsRelIsoControlSelection()) {
		const ElectronPointer electron = ttbarCand->MostIsolatedElectron(ttbarCand->Electrons());
		histMan_->H1D_JetBinned("QCDest_CombRelIso_controlRegion")->Fill(electron->relativeIsolation(), weight);

		if (ttbarCand->GoodElectronCleanedBJets().size() >= 1)
			histMan_->H1D_JetBinned("QCDest_CombRelIso_controlRegion_1btag")->Fill(electron->relativeIsolation(),
					weight);

		if (ttbarCand->GoodElectronCleanedBJets().size() >= 2)
			histMan_->H1D_JetBinned("QCDest_CombRelIso_controlRegion_2btag")->Fill(electron->relativeIsolation(),
					weight);

		if (Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
			const ElectronPointer electron = ttbarCand->MostPFIsolatedElectron(ttbarCand->Electrons());
			histMan_->H1D_JetBinned("QCDest_PFIsolation_controlRegion2")->Fill(electron->pfIsolation(), weight);

			if (ttbarCand->GoodElectronCleanedBJets().size() >= 1)
				histMan_->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_1btag")->Fill(electron->pfIsolation(),
						weight);

			if (ttbarCand->GoodElectronCleanedBJets().size() >= 2)
				histMan_->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_2btag")->Fill(electron->pfIsolation(),
						weight);

			if (ttbarCand->MET()->pt() > 20) {
				histMan_->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCut")->Fill(electron->pfIsolation(),
						weight);

				if (ttbarCand->GoodElectronCleanedJets().size() >= 2) {
					if (ttbarCand->GoodElectronCleanedJets().front()->pt() > 70
							&& ttbarCand->GoodElectronCleanedJets().at(1)->pt() > 50) {
						histMan_->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCutAndAsymJetCuts")->Fill(
								electron->pfIsolation(), weight);
					}
				}
			}

			if (ttbarCand->GoodElectronCleanedJets().size() >= 2) {
				if (ttbarCand->GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCand->GoodElectronCleanedJets().at(1)->pt() > 50) {
					histMan_->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithAsymJetCuts")->Fill(
							electron->pfIsolation(), weight);

				}
			}
		}

	}

	if (ttbarCand->passesEPlusJEtsPFIsoControlSelection()
			&& Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
		const ElectronPointer electron = ttbarCand->MostPFIsolatedElectron(ttbarCand->Electrons());
		histMan_->H1D_JetBinned("QCDest_PFIsolation_controlRegion")->Fill(electron->pfIsolation(), weight);

		if (ttbarCand->GoodElectronCleanedBJets().size() == 1)
			histMan_->H1D_JetBinned("QCDest_PFIsolation_controlRegion_1btag")->Fill(electron->pfIsolation(), weight);

		if (ttbarCand->GoodElectronCleanedBJets().size() == 2)
			histMan_->H1D_JetBinned("QCDest_PFIsolation_controlRegion_2btag")->Fill(electron->pfIsolation(), weight);

	}

	if (ttbarCand->passesEPlusJEtsPFIsoControlSelection() && Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow
			&& ttbarCand->GoodElectronCleanedJets().size() >= 4) {
		const ElectronPointer electron = ttbarCand->MostPFIsolatedElectron(ttbarCand->Electrons());
		histMan_->H1D_BJetBinned("PFIsolation_controlRegion")->Fill(electron->pfIsolation(), weight);
	}

	if (ttbarCand->passesEPlusJetsQCDselection() && ttbarCand->GoodElectronCleanedJets().size() >= 4) {
		const ElectronPointer electron = ttbarCand->GoodPFNonIsolatedElectrons().front();
		histMan_->H1D("QCD_electronEta")->Fill(electron->eta(), weight);
		histMan_->H1D("QCD_electronAbsEta")->Fill(abs(electron->eta()), weight);
		histMan_->H1D("QCD_electronPt")->Fill(electron->pt(), weight);
		histMan_->H1D("QCD_electronPFIsolation")->Fill(electron->pfIsolation(), weight);
		histMan_->H1D("QCD_electronRelIso")->Fill(electron->relativeIsolation(), weight);

		histMan_->H1D("QCD_goodJetsMultiplicity")->Fill(ttbarCand->GoodElectronCleanedJets().size(), weight);
		histMan_->H1D("QCD_jetsMultiplicity")->Fill(ttbarCand->Jets().size(), weight);
		histMan_->H1D("QCD_btaggedJetsMultiplicity")->Fill(ttbarCand->GoodElectronCleanedBJets().size(), weight);
		for (unsigned index = 0; index < ttbarCand->GoodElectronCleanedJets().size(); ++index) {
			histMan_->H1D("QCD_jetPt")->Fill(ttbarCand->GoodElectronCleanedJets().at(index)->pt(), weight);
			histMan_->H1D("QCD_jetEta")->Fill(ttbarCand->GoodElectronCleanedJets().at(index)->eta(), weight);
			histMan_->H1D("QCD_jetPhi")->Fill(ttbarCand->GoodElectronCleanedJets().at(index)->phi(), weight);
			histMan_->H1D("QCD_jetMass")->Fill(ttbarCand->GoodElectronCleanedJets().at(index)->mass(), weight);
		}
		histMan_->H1D("QCD_1stJetPt")->Fill(ttbarCand->GoodElectronCleanedJets().front()->pt(), weight);
		histMan_->H1D("QCD_1stJetEta")->Fill(ttbarCand->GoodElectronCleanedJets().front()->eta(), weight);
		histMan_->H1D("QCD_1stJetPhi")->Fill(ttbarCand->GoodElectronCleanedJets().front()->phi(), weight);
		histMan_->H1D("QCD_1stJetMass")->Fill(ttbarCand->GoodElectronCleanedJets().front()->mass(), weight);
	}

	histMan_->setCurrentHistogramFolder("topReconstruction/backgroundShape");
	if (ttbarCand->passesEPlusJetsRelIsoSelection() && ttbarCand->GoodElectronCleanedJets().size() >= 4) {
		const ElectronPointer electron = ttbarCand->MostIsolatedElectron(ttbarCand->Electrons());
		if (electron->relativeIsolation() > Globals::maxElectronRelativeIsolation
				&& !std::isnan(electron->relativeIsolation()) && !isinf(electron->relativeIsolation())) {
			try {
				ttbarCand->reconstructTTbarToEPlusJets(electron);
				const ParticlePointer resonance = ttbarCand->getResonance();
				histMan_->H1D_BJetBinned("mttbar_controlRegion")->Fill(resonance->mass(), weight);
				if (ttbarCand->MET()->pt() > 20) {
					histMan_->H1D_BJetBinned("mttbar_controlRegion_withMETCut")->Fill(resonance->mass(), weight);
					if (ttbarCand->GoodElectronCleanedJets().front()->pt() > 70
							&& ttbarCand->GoodElectronCleanedJets().at(1)->pt() > 50)
						histMan_->H1D_BJetBinned("mttbar_controlRegion_withMETAndAsymJets")->Fill(resonance->mass(),
								weight);
				}

				if (ttbarCand->GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCand->GoodElectronCleanedJets().at(1)->pt() > 50)
					histMan_->H1D_BJetBinned("mttbar_controlRegion_withAsymJetsCut")->Fill(resonance->mass(), weight);

			} catch (ReconstructionException& e) {
				cout << "Could not reconstruct event: " << e.what() << endl;
			}
		}
	}

	if (ttbarCand->passesConversionSelection() && ttbarCand->GoodElectronCleanedJets().size() >= 4) {
		ElectronPointer electron;
		if (Event::usePFIsolation)
			electron = ttbarCand->GoodPFIsolatedElectrons().front();
		else
			electron = ttbarCand->GoodIsolatedElectrons().front();
		try {
			ttbarCand->reconstructTTbarToEPlusJets(electron);
			const ParticlePointer resonance = ttbarCand->getResonance();
			histMan_->H1D_BJetBinned("mttbar_conversions")->Fill(resonance->mass(), weight);
			if (ttbarCand->MET()->pt() > 20) {
				histMan_->H1D_BJetBinned("mttbar_conversions_withMETCut")->Fill(resonance->mass(), weight);
				if (ttbarCand->GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCand->GoodElectronCleanedJets().at(1)->pt() > 50)
					histMan_->H1D_BJetBinned("mttbar_conversions_withMETAndAsymJets")->Fill(resonance->mass(), weight);
			}
			if (ttbarCand->GoodElectronCleanedJets().front()->pt() > 70
					&& ttbarCand->GoodElectronCleanedJets().at(1)->pt() > 50)
				histMan_->H1D_BJetBinned("mttbar_conversions_withAsymJetsCut")->Fill(resonance->mass(), weight);
		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}

	}

	if (ttbarCand->passesConversionSelection() && ttbarCand->GoodElectronCleanedJets().size() == 3) {
		ElectronPointer electron;
		if (Event::usePFIsolation)
			electron = ttbarCand->GoodPFIsolatedElectrons().front();
		else
			electron = ttbarCand->GoodIsolatedElectrons().front();

		try {
			ttbarCand->reconstructTTbarToEPlusJetsFrom3Jets(electron);
			const ParticlePointer resonance = ttbarCand->getResonance();
			histMan_->H1D_BJetBinned("mttbar_3jets_conversions")->Fill(resonance->mass(), weight);
			if (ttbarCand->MET()->pt() > 20) {
				if (ttbarCand->GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCand->GoodElectronCleanedJets().at(1)->pt() > 50)
					histMan_->H1D_BJetBinned("mttbar_3jets_conversions_withMETAndAsymJets")->Fill(resonance->mass(),
							weight);
			}
		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
	}

	histMan_->setCurrentHistogramFolder("QCDStudy");
	if (ttbarCand->Electrons().size() > 0 && ttbarCand->GoodPFIsolatedElectrons().size() < 2
			&& Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
		const ElectronPointer electron = ttbarCand->MostPFIsolatedElectron(ttbarCand->Electrons());
		histMan_->H1D_JetBinned("MostPFIsolatedElectron_dPhiIn")->Fill(electron->dPhiIn(), weight);
		histMan_->H1D_JetBinned("MostPFIsolatedElectron_dEtaIn")->Fill(electron->dEtaIn(), weight);
	}

	histMan_->setCurrentHistogramFolder("topReconstruction/backgroundShape");
	if (ttbarCand->passesEPlusJetsAntiIsolationSelection() && ttbarCand->GoodElectronCleanedJets().size() >= 4) {
		ElectronPointer electron = ttbarCand->GoodElectrons().front();
		try {
			ttbarCand->reconstructTTbarToEPlusJets(electron);
			float mttbar = ttbarCand->mttbar();
			histMan_->H1D_BJetBinned("mttbar_antiIsolated")->Fill(mttbar, weight);
			if (ttbarCand->MET()->pt() > 20) {
				histMan_->H1D_BJetBinned("mttbar_antiIsolated_withMETCut")->Fill(mttbar, weight);
				if (ttbarCand->GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCand->GoodElectronCleanedJets().at(1)->pt() > 50) {
					histMan_->H1D_BJetBinned("mttbar_antiIsolated_withMETAndAsymJets")->Fill(mttbar, weight);
				}
			}
			if (ttbarCand->GoodElectronCleanedJets().front()->pt() > 70
					&& ttbarCand->GoodElectronCleanedJets().at(1)->pt() > 50) {
				histMan_->H1D_BJetBinned("mttbar_antiIsolated_withAsymJetsCut")->Fill(mttbar, weight);
			}

		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
	}

	if (ttbarCand->passesEPlusJetsAntiIsolationSelection() && ttbarCand->GoodElectronCleanedJets().size() == 3) {
		ElectronPointer electron = ttbarCand->GoodElectrons().front();
		try {
			ttbarCand->reconstructTTbarToEPlusJetsFrom3Jets(electron);
			float mttbar = ttbarCand->mttbar();
			histMan_->H1D_BJetBinned("mttbar_3jets_antiIsolated")->Fill(mttbar, weight);
			if (ttbarCand->MET()->pt() > 20) {
				if (ttbarCand->GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCand->GoodElectronCleanedJets().at(1)->pt() > 50) {
					histMan_->H1D_BJetBinned("mttbar_3jets_antiIsolated_withMETAndAsymJets")->Fill(mttbar, weight);
				}
			}
		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
	}

	if (ttbarCand->passedEPlusJetsAntiIDSelection() && ttbarCand->GoodElectronCleanedJets().size() >= 4) {
		const ElectronPointer electron = ttbarCand->MostPFIsolatedElectron(ttbarCand->Electrons());
		try {
			ttbarCand->reconstructTTbarToEPlusJets(electron);
			const ParticlePointer resonance = ttbarCand->getResonance();
			histMan_->H1D_BJetBinned("mttbar_antiID")->Fill(resonance->mass(), weight);
			if (ttbarCand->passesMETCut() && ttbarCand->passesAsymmetricElectronCleanedJetCuts())
				histMan_->H1D_BJetBinned("mttbar_antiID_withMETAndAsymJets")->Fill(resonance->mass(), weight);

		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
	}

	if (ttbarCand->passedEPlusJetsAntiIDSelection() && ttbarCand->GoodElectronCleanedJets().size() == 3) {
		const ElectronPointer electron = ttbarCand->MostPFIsolatedElectron(ttbarCand->Electrons());
		try {
			ttbarCand->reconstructTTbarToEPlusJetsFrom3Jets(electron);
			const ParticlePointer resonance = ttbarCand->getResonance();
			histMan_->H1D_BJetBinned("mttbar_3jets_antiID")->Fill(resonance->mass(), weight);
			if (ttbarCand->passesMETCut() && ttbarCand->passesAsymmetricElectronCleanedJetCuts())
				histMan_->H1D_BJetBinned("mttbar_3jets_antiID_withMETAndAsymJets")->Fill(resonance->mass(), weight);

		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
	}

	//moved from doNotePlots, do be revised
	histMan_->setCurrentHistogramFolder("QCDStudy");
	if (ttbarCand->GoodElectrons().size() >= 1 && ttbarCand->Jets().size() >= 2) {
		const ElectronCollection electrons = ttbarCand->GoodElectrons();
		ElectronCollection nonConversionElectrons;
		for (unsigned int index = 0; index < electrons.size(); ++index) {
			const ElectronPointer electron = electrons.at(index);
			if (electron->isFromConversion() == false && electron->isTaggedAsConversion(0.02, 0.02) == false) {
				nonConversionElectrons.push_back(electron);
			}
		}
		if (nonConversionElectrons.size() == 1) {
			const ElectronPointer electron = nonConversionElectrons.front();
			JetCollection goodjets;
			for (unsigned index = 0; index < ttbarCand->Jets().size(); ++index) {
				if (ttbarCand->Jets().at(index)->isGood())
					goodjets.push_back(ttbarCand->Jets().at(index));
			}
			if (goodjets.size() >= 2) {
				unsigned int closestID = electron->getClosestJetIndex(goodjets);
				float minDR = electron->deltaR(goodjets.at(closestID));
				float ptRel = electron->relativePtTo(goodjets.at(closestID));
				histMan_->H2D("ptRel_vs_DRmin")->Fill(minDR, ptRel, weight);
				if (ttbarCand->MET()->et() < 20 && ttbarCand->transverseWmass(electron) < 35) {
					histMan_->H1D("DRmin_QCDenriched")->Fill(minDR, weight);
					histMan_->H1D("ptRel_QCDenriched")->Fill(ptRel, weight);
				} else if (ttbarCand->MET()->et() > 30 && ttbarCand->transverseWmass(electron) > 50) {
					histMan_->H1D("DRmin_WZenriched")->Fill(minDR, weight);
					histMan_->H1D("ptRel_WZenriched")->Fill(ptRel, weight);
				}
			}
		}

	}

	ABCDMethod(ttbarCand);
}

void QCDAnalyser::ABCDMethod(const TopPairEventCandidatePtr event) {

	double weight(event->weight());

	if (passesSelectionWithoutIsolationOrConversionVeto(event)) {
		ElectronPointer electron(event->MostPFIsolatedElectron(event->GoodElectrons()));
		double conversion(0.);
		if (electron->isFromConversion() || electron->isTaggedAsConversion()) {
			conversion = 1.0;
		}
		double pfIso = electron->pfIsolation();
		if (event->GoodElectronCleanedJets().size() >= 4) {
			histMan_->H2D_BJetBinned("ABCD_Method")->Fill(pfIso, 0., weight);
			if (event->passesMETCut() && event->passesAsymmetricElectronCleanedJetCuts()) {
				histMan_->H2D_BJetBinned("ABCD_Method_WithMETCutAndAsymJetCuts")->Fill(pfIso, conversion, weight);
			}
		}

		if (event->GoodElectronCleanedJets().size() == 3) {
			histMan_->H2D_BJetBinned("ABCD_Method_3jets")->Fill(pfIso, 0., weight);
			if (event->passesMETCut() && event->passesAsymmetricElectronCleanedJetCuts()) {
				histMan_->H2D_BJetBinned("ABCD_Method_3jets_WithMETCutAndAsymJetCuts")->Fill(pfIso, conversion, weight);
			}
		}
	}

}

/**
 * Require event cleaning, HLT, at least one good vertex, at least 3 good jets, apply Z veto
 * and require to have at least one good electron but at most one good isolated electron
 */
bool QCDAnalyser::passesSelectionWithoutIsolationOrConversionVeto(const TopPairEventCandidatePtr event) {
	bool result(event->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex));
	result = result && event->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AtLeastThreeGoodJets);
	result = result && event->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::LooseMuonVeto);
	result = result && event->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::DileptonVeto);
	//require at least one good electron
	result = result && event->GoodElectrons().size() > 0;
	//If more than 2 isolated electrons return 0;
	result = result && event->GoodPFIsolatedElectrons().size() < 2;

	return result;
}

void QCDAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder("QCDStudy");
	histMan_->addH1D_JetBinned("MostPFIsolatedElectron_dPhiIn", "MostPFIsolatedElectron_dPhiIn", 50, 0, 0.1);
	histMan_->addH1D_JetBinned("MostPFIsolatedElectron_dEtaIn", "MostPFIsolatedElectron_dEtaIn", 50, 0, 0.02);
	histMan_->addH2D("ptRel_vs_DRmin", "ptRel_vs_DRmin", 100, 0, 1, 300, 0, 300);
	histMan_->addH1D("ptRel_QCDenriched", "ptRel_QCDenriched", 300, 0, 300);
	histMan_->addH1D("DRmin_QCDenriched", "DRmin_QCDenriched", 100, 0, 1);
	histMan_->addH1D("ptRel_WZenriched", "ptRel_WZenriched", 300, 0, 300);
	histMan_->addH1D("DRmin_WZenriched", "DRmin_WZenriched", 100, 0, 1);
	histMan_->addH1D_JetBinned("QCDest_CombRelIso", "RelIso", 1000, 0, 10);
	histMan_->addH1D_JetBinned("QCDest_CombRelIso_controlRegion", "RelIso control region", 1000, 0, 10);
	histMan_->addH1D_JetBinned("QCDest_CombRelIso_1btag", "RelIso (>=1 btag)", 1000, 0, 10);
	histMan_->addH1D_JetBinned("QCDest_CombRelIso_controlRegion_1btag", "RelIso control region", 1000, 0, 10);
	histMan_->addH1D_JetBinned("QCDest_CombRelIso_2btag", "RelIso (>=2 btag)", 1000, 0, 10);
	histMan_->addH1D_JetBinned("QCDest_CombRelIso_controlRegion_2btag", "RelIso control region", 1000, 0, 10);

	histMan_->addH1D_JetBinned("QCDest_PFIsolation", "PFIso", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_WithMETCut", "PFIso", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);

	histMan_->addH1D("QCD_electronEta", "Electron eta", 100, -2.4, 2.4);
	histMan_->addH1D("QCD_electronAbsEta", "Electron abs(eta)", 100, 0, 2.6);
	histMan_->addH1D("QCD_electronPt", "Electron Pt", 100, 0, 200);
	histMan_->addH1D("QCD_electronPFIsolation", "Electron PFIso", 100, 0, 5);
	histMan_->addH1D("QCD_electronRelIso", "Electron RelIso", 100, 0, 5);

	histMan_->addH1D("QCD_goodJetsMultiplicity", "Good jets multiplicity", 4, 3.5, 7.5);
	histMan_->addH1D("QCD_jetsMultiplicity", "All jets multiplicity", 4, 3.5, 7.5);
	histMan_->addH1D("QCD_btaggedJetsMultiplicity", "b-tagged jets multiplicity", 4, -0.5, 3.5);
	histMan_->addH1D("QCD_jetPt", "Jet Pt", 100, 0, 200);
	histMan_->addH1D("QCD_jetEta", "Jet Eta", 100, -2.5, 2.5);
	histMan_->addH1D("QCD_jetPhi", "Jet Phi", 100, -2.5, 2.5);
	histMan_->addH1D("QCD_jetMass", "Jet Mass", 100, 0, 40);

	histMan_->addH1D("QCD_1stJetPt", "Leading Jet Pt", 100, 0, 200);
	histMan_->addH1D("QCD_1stJetEta", "Leading Jet Eta", 100, -2.5, 2.5);
	histMan_->addH1D("QCD_1stJetPhi", "Leading Jet Phi", 100, -2.5, 2.5);
	histMan_->addH1D("QCD_1stJetMass", "Leading Jet Mass", 100, 0, 40);

	histMan_->addH1D_BJetBinned("PFIsolation", "PFIso", 500, 0, 5);

	histMan_->addH1D_BJetBinned("PFIsolation_controlRegion", "PFIso", 500, 0, 5);
//	histMan_->addH1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);
	histMan_->addH1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts_DR03", "PFIso (deltaR < 0.3);events;pf relIso",
			500, 0, 5);
	histMan_->addH1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts_DR04", "PFIso (deltaR < 0.4);events;pf relIso",
			500, 0, 5);
	histMan_->addH1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts_DR05", "PFIso (deltaR < 0.5);events;pf relIso",
			500, 0, 5);
	histMan_->addH1D_BJetBinned("DirectionalIsolation_WithMETCutAndAsymJetCuts_DR02",
			"PFIso (deltaR < 0.2);events;pf relIso", 500, 0, 5);
	histMan_->addH1D_BJetBinned("DirectionalIsolation_WithMETCutAndAsymJetCuts_DR03",
			"PFIso (deltaR < 0.3);events;pf relIso", 500, 0, 5);
	histMan_->addH1D_BJetBinned("DirectionalIsolationWithGaussianFallOff_WithMETCutAndAsymJetCuts_DR02",
			"DirectionalIso (deltaR < 0.2);events;pf relIso", 500, 0, 5);
	histMan_->addH1D_BJetBinned("DirectionalIsolationWithGaussianFallOff_WithMETCutAndAsymJetCuts_DR03",
			"DirectionalIso (deltaR < 0.3);events;pf relIso", 500, 0, 5);
	histMan_->addH1D_BJetBinned("PFIsolation_3jets_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);

	histMan_->addH1D_BJetBinned("RelativeIsolation_PUCorrected", "PFIso", 500, 0, 5);
	histMan_->addH1D_BJetBinned("RelativeIsolation_PUCorrected_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);

	//histMan_->addH1D_JetBinned("QCDest_PFIsolation_WithAsymJetCuts", "PFIso", 1000, 0, 10);

	histMan_->addH1D_JetBinned("QCDest_PFIsolation_controlRegion", "PFIso control region", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_1btag", "PFIso (>=1 btag)", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts", "PFIso (>=1 btag)", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_1btag", "PFIso control region", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_2btag", "PFIso (>=2 btag)", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_2btag_WithMETCutAndAsymJetCuts", "PFIso (>=2 btag)", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_2btag", "PFIso control region", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithMETCut", "PFIso control region", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithMETCutAndAsymJetCuts", "PFIso control region", 1000,
			0, 10);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithAsymJetCuts", "PFIso control region", 500, 0, 5);

	histMan_->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2", "PFIso control region", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCut", "PFIso control region", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCutAndAsymJetCuts", "PFIso control region", 500,
			0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithAsymJetCuts", "PFIso control region", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_1btag", "PFIso control region", 500, 0, 5);
	histMan_->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_2btag", "PFIso control region", 500, 0, 5);

	histMan_->addH2D_BJetBinned("ABCD_Method_3jets", "ABCD Method 3 jets; relative isolation; conversion veto; events",
			100, 0., 1., 2, -0.5, 1.5);
	histMan_->addH2D_BJetBinned("ABCD_Method", "ABCD Method #geq 4 jets; relative isolation; conversion veto; events",
			100, 0., 1., 2, -0.5, 1.5);
	histMan_->addH2D_BJetBinned("ABCD_Method_3jets_WithMETCutAndAsymJetCuts",
			"ABCD Method 3 jets; relative isolation; conversion veto; events", 100, 0., 1., 2, -0.5, 1.5);
	histMan_->addH2D_BJetBinned("ABCD_Method_WithMETCutAndAsymJetCuts",
			"ABCD Method #geq 4 jets; relative isolation; conversion veto; events", 100, 0., 1., 2, -0.5, 1.5);
}

}
