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

QCDAnalyser::QCDAnalyser(boost::shared_ptr<HistogramManager> histMan) :
		histMan(histMan) {

}

QCDAnalyser::~QCDAnalyser() {
}

void QCDAnalyser::analyse(const TopPairEventCandidate& Event) {
	double weight = Event.weight();
	TopPairEventCandidate ttbarCandidate(Event);
	histMan->setCurrentHistogramFolder("QCDStudy");
	if (ttbarCandidate.passesEPlusJetsRelIsoSelection()) {
		const ElectronPointer electron = ttbarCandidate.MostIsolatedElectron(ttbarCandidate.Electrons());
		histMan->H1D_JetBinned("QCDest_CombRelIso")->Fill(electron->relativeIsolation(), weight);

		if (ttbarCandidate.GoodElectronCleanedJets().size() >= 4) {
			double rho = ttbarCandidate.rho();
			histMan->H1D_BJetBinned("RelativeIsolation_PUCorrected")->Fill(electron->relativeIsolationPUCorrected(rho),
					weight);
			if (ttbarCandidate.passesMETCut() && ttbarCandidate.passesAsymmetricElectronCleanedJetCuts()) {
				histMan->H1D_BJetBinned("RelativeIsolation_PUCorrected_WithMETCutAndAsymJetCuts")->Fill(
						electron->relativeIsolationPUCorrected(rho), weight);
			}
		}

		if (ttbarCandidate.GoodElectronCleanedBJets().size() >= 1)
			histMan->H1D_JetBinned("QCDest_CombRelIso_1btag")->Fill(electron->relativeIsolation(), weight);

		if (ttbarCandidate.GoodElectronCleanedBJets().size() >= 2)
			histMan->H1D_JetBinned("QCDest_CombRelIso_2btag")->Fill(electron->relativeIsolation(), weight);
	}

	if (ttbarCandidate.passesEPlusJetsPFIsoSelection()
			&& Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
		const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
		histMan->H1D_JetBinned("QCDest_PFIsolation")->Fill(electron->pfIsolation(), weight);

		if (ttbarCandidate.GoodElectronCleanedBJets().size() == 1) {
			histMan->H1D_JetBinned("QCDest_PFIsolation_1btag")->Fill(electron->pfIsolation(), weight);
			if (ttbarCandidate.passesAsymmetricElectronCleanedJetCuts() && ttbarCandidate.passesMETCut()) {
				histMan->H1D_JetBinned("QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts")->Fill(
						electron->pfIsolation(), weight);
			}
		}

		if (ttbarCandidate.GoodElectronCleanedBJets().size() == 2) {
			histMan->H1D_JetBinned("QCDest_PFIsolation_2btag")->Fill(electron->pfIsolation(), weight);
			if (ttbarCandidate.passesAsymmetricElectronCleanedJetCuts() && ttbarCandidate.passesMETCut()) {
				histMan->H1D_JetBinned("QCDest_PFIsolation_2btag_WithMETCutAndAsymJetCuts")->Fill(
						electron->pfIsolation(), weight);
			}
		}

		if (ttbarCandidate.GoodElectronCleanedJets().size() >= 4) {
			histMan->H1D_BJetBinned("PFIsolation")->Fill(electron->pfIsolation(), weight);
		}

		if (ttbarCandidate.MET()->pt() > 20) {
			histMan->H1D_JetBinned("QCDest_PFIsolation_WithMETCut")->Fill(electron->pfIsolation(), weight);

			if (ttbarCandidate.GoodElectronCleanedJets().size() >= 2) {
				if (ttbarCandidate.GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCandidate.GoodElectronCleanedJets().at(1)->pt() > 50) {
					histMan->H1D_JetBinned("QCDest_PFIsolation_WithMETCutAndAsymJetCuts")->Fill(electron->pfIsolation(),
							weight);

					if (ttbarCandidate.GoodElectronCleanedJets().size() == 3)
						histMan->H1D_BJetBinned("PFIsolation_3jets_WithMETCutAndAsymJetCuts")->Fill(
								electron->pfIsolation(), weight);

					if (ttbarCandidate.GoodElectronCleanedJets().size() >= 4) {
						histMan->H1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts_DR03")->Fill(
								electron->pfRelativeIsolation(0.3), weight);
						histMan->H1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts_DR04")->Fill(
								electron->pfRelativeIsolation(0.4), weight);
						histMan->H1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts_DR05")->Fill(
								electron->pfRelativeIsolation(0.5), weight);
						histMan->H1D_BJetBinned("DirectionalIsolation_WithMETCutAndAsymJetCuts_DR02")->Fill(
								electron->directionalIsolation(0.2) / electron->et(), weight);
						histMan->H1D_BJetBinned("DirectionalIsolation_WithMETCutAndAsymJetCuts_DR03")->Fill(
								electron->directionalIsolation(0.3) / electron->et(), weight);

						histMan->H1D_BJetBinned("DirectionalIsolationWithGaussianFallOff_WithMETCutAndAsymJetCuts_DR02")->Fill(
								electron->directionalIsolationWithGaussianFallOff(0.2) / electron->et(), weight);
						histMan->H1D_BJetBinned("DirectionalIsolationWithGaussianFallOff_WithMETCutAndAsymJetCuts_DR03")->Fill(
								electron->directionalIsolationWithGaussianFallOff(0.3) / electron->et(), weight);
					}

				}
			}
		}

	}

	if (ttbarCandidate.passesEPlusJetsRelIsoControlSelection()) {
		const ElectronPointer electron = ttbarCandidate.MostIsolatedElectron(ttbarCandidate.Electrons());
		histMan->H1D_JetBinned("QCDest_CombRelIso_controlRegion")->Fill(electron->relativeIsolation(), weight);

		if (ttbarCandidate.GoodElectronCleanedBJets().size() >= 1)
			histMan->H1D_JetBinned("QCDest_CombRelIso_controlRegion_1btag")->Fill(electron->relativeIsolation(),
					weight);

		if (ttbarCandidate.GoodElectronCleanedBJets().size() >= 2)
			histMan->H1D_JetBinned("QCDest_CombRelIso_controlRegion_2btag")->Fill(electron->relativeIsolation(),
					weight);

		if (Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
			const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
			histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2")->Fill(electron->pfIsolation(), weight);

			if (ttbarCandidate.GoodElectronCleanedBJets().size() >= 1)
				histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_1btag")->Fill(electron->pfIsolation(),
						weight);

			if (ttbarCandidate.GoodElectronCleanedBJets().size() >= 2)
				histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_2btag")->Fill(electron->pfIsolation(),
						weight);

			if (ttbarCandidate.MET()->pt() > 20) {
				histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCut")->Fill(electron->pfIsolation(),
						weight);

				if (ttbarCandidate.GoodElectronCleanedJets().size() >= 2) {
					if (ttbarCandidate.GoodElectronCleanedJets().front()->pt() > 70
							&& ttbarCandidate.GoodElectronCleanedJets().at(1)->pt() > 50) {
						histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCutAndAsymJetCuts")->Fill(
								electron->pfIsolation(), weight);
					}
				}
			}

			if (ttbarCandidate.GoodElectronCleanedJets().size() >= 2) {
				if (ttbarCandidate.GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCandidate.GoodElectronCleanedJets().at(1)->pt() > 50) {
					histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithAsymJetCuts")->Fill(
							electron->pfIsolation(), weight);

				}
			}
		}

	}

	if (ttbarCandidate.passesEPlusJEtsPFIsoControlSelection()
			&& Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
		const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
		histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion")->Fill(electron->pfIsolation(), weight);

		if (ttbarCandidate.GoodElectronCleanedBJets().size() == 1)
			histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion_1btag")->Fill(electron->pfIsolation(), weight);

		if (ttbarCandidate.GoodElectronCleanedBJets().size() == 2)
			histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion_2btag")->Fill(electron->pfIsolation(), weight);

	}

	if (ttbarCandidate.passesEPlusJEtsPFIsoControlSelection()
			&& Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow
			&& ttbarCandidate.GoodElectronCleanedJets().size() >= 4) {
		const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
		histMan->H1D_BJetBinned("PFIsolation_controlRegion")->Fill(electron->pfIsolation(), weight);
	}

	if (ttbarCandidate.passesEPlusJetsQCDselection() && ttbarCandidate.GoodElectronCleanedJets().size() >= 4) {
		const ElectronPointer electron = ttbarCandidate.GoodPFNonIsolatedElectrons().front();
		histMan->H1D("QCD_electronEta")->Fill(electron->eta(), weight);
		histMan->H1D("QCD_electronAbsEta")->Fill(abs(electron->eta()), weight);
		histMan->H1D("QCD_electronPt")->Fill(electron->pt(), weight);
		histMan->H1D("QCD_electronPFIsolation")->Fill(electron->pfIsolation(), weight);
		histMan->H1D("QCD_electronRelIso")->Fill(electron->relativeIsolation(), weight);

		histMan->H1D("QCD_goodJetsMultiplicity")->Fill(ttbarCandidate.GoodElectronCleanedJets().size(), weight);
		histMan->H1D("QCD_jetsMultiplicity")->Fill(ttbarCandidate.Jets().size(), weight);
		histMan->H1D("QCD_btaggedJetsMultiplicity")->Fill(ttbarCandidate.GoodElectronCleanedBJets().size(), weight);
		for (unsigned index = 0; index < ttbarCandidate.GoodElectronCleanedJets().size(); ++index) {
			histMan->H1D("QCD_jetPt")->Fill(ttbarCandidate.GoodElectronCleanedJets().at(index)->pt(), weight);
			histMan->H1D("QCD_jetEta")->Fill(ttbarCandidate.GoodElectronCleanedJets().at(index)->eta(), weight);
			histMan->H1D("QCD_jetPhi")->Fill(ttbarCandidate.GoodElectronCleanedJets().at(index)->phi(), weight);
			histMan->H1D("QCD_jetMass")->Fill(ttbarCandidate.GoodElectronCleanedJets().at(index)->mass(), weight);
		}
		histMan->H1D("QCD_1stJetPt")->Fill(ttbarCandidate.GoodElectronCleanedJets().front()->pt(), weight);
		histMan->H1D("QCD_1stJetEta")->Fill(ttbarCandidate.GoodElectronCleanedJets().front()->eta(), weight);
		histMan->H1D("QCD_1stJetPhi")->Fill(ttbarCandidate.GoodElectronCleanedJets().front()->phi(), weight);
		histMan->H1D("QCD_1stJetMass")->Fill(ttbarCandidate.GoodElectronCleanedJets().front()->mass(), weight);
	}

	histMan->setCurrentHistogramFolder("topReconstruction/backgroundShape");
	if (ttbarCandidate.passesEPlusJetsRelIsoSelection() && ttbarCandidate.GoodElectronCleanedJets().size() >= 4) {
		const ElectronPointer electron = ttbarCandidate.MostIsolatedElectron(ttbarCandidate.Electrons());
		if (electron->relativeIsolation() > Globals::maxElectronRelativeIsolation
				&& !isnan(electron->relativeIsolation()) && !isinf(electron->relativeIsolation())) {
			try {
				ttbarCandidate.reconstructTTbarToEPlusJets(electron);
				const ParticlePointer resonance = ttbarCandidate.getResonance();
				histMan->H1D_BJetBinned("mttbar_controlRegion")->Fill(resonance->mass(), weight);
				if (ttbarCandidate.MET()->pt() > 20) {
					histMan->H1D_BJetBinned("mttbar_controlRegion_withMETCut")->Fill(resonance->mass(), weight);
					if (ttbarCandidate.GoodElectronCleanedJets().front()->pt() > 70
							&& ttbarCandidate.GoodElectronCleanedJets().at(1)->pt() > 50)
						histMan->H1D_BJetBinned("mttbar_controlRegion_withMETAndAsymJets")->Fill(resonance->mass(),
								weight);
				}

				if (ttbarCandidate.GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCandidate.GoodElectronCleanedJets().at(1)->pt() > 50)
					histMan->H1D_BJetBinned("mttbar_controlRegion_withAsymJetsCut")->Fill(resonance->mass(), weight);

			} catch (ReconstructionException& e) {
				cout << "Could not reconstruct event: " << e.what() << endl;
			}
		}
	}

	if (ttbarCandidate.passesConversionSelection() && ttbarCandidate.GoodElectronCleanedJets().size() >= 4) {
		ElectronPointer electron;
		if (Event::usePFIsolation)
			electron = ttbarCandidate.GoodPFIsolatedElectrons().front();
		else
			electron = ttbarCandidate.GoodIsolatedElectrons().front();
		try {
			ttbarCandidate.reconstructTTbarToEPlusJets(electron);
			const ParticlePointer resonance = ttbarCandidate.getResonance();
			histMan->H1D_BJetBinned("mttbar_conversions")->Fill(resonance->mass(), weight);
			if (ttbarCandidate.MET()->pt() > 20) {
				histMan->H1D_BJetBinned("mttbar_conversions_withMETCut")->Fill(resonance->mass(), weight);
				if (ttbarCandidate.GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCandidate.GoodElectronCleanedJets().at(1)->pt() > 50)
					histMan->H1D_BJetBinned("mttbar_conversions_withMETAndAsymJets")->Fill(resonance->mass(), weight);
			}
			if (ttbarCandidate.GoodElectronCleanedJets().front()->pt() > 70
					&& ttbarCandidate.GoodElectronCleanedJets().at(1)->pt() > 50)
				histMan->H1D_BJetBinned("mttbar_conversions_withAsymJetsCut")->Fill(resonance->mass(), weight);
		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}

	}

	if (ttbarCandidate.passesConversionSelection() && ttbarCandidate.GoodElectronCleanedJets().size() == 3) {
		ElectronPointer electron;
		if (Event::usePFIsolation)
			electron = ttbarCandidate.GoodPFIsolatedElectrons().front();
		else
			electron = ttbarCandidate.GoodIsolatedElectrons().front();

		try {
			ttbarCandidate.reconstructTTbarToEPlusJetsFrom3Jets(electron);
			const ParticlePointer resonance = ttbarCandidate.getResonance();
			histMan->H1D_BJetBinned("mttbar_3jets_conversions")->Fill(resonance->mass(), weight);
			if (ttbarCandidate.MET()->pt() > 20) {
				if (ttbarCandidate.GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCandidate.GoodElectronCleanedJets().at(1)->pt() > 50)
					histMan->H1D_BJetBinned("mttbar_3jets_conversions_withMETAndAsymJets")->Fill(resonance->mass(),
							weight);
			}
		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
	}

	histMan->setCurrentHistogramFolder("QCDStudy");
	if (ttbarCandidate.Electrons().size() > 0 && ttbarCandidate.GoodPFIsolatedElectrons().size() < 2
			&& Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
		const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
		histMan->H1D_JetBinned("MostPFIsolatedElectron_dPhiIn")->Fill(electron->dPhiIn(), weight);
		histMan->H1D_JetBinned("MostPFIsolatedElectron_dEtaIn")->Fill(electron->dEtaIn(), weight);
	}

	histMan->setCurrentHistogramFolder("topReconstruction/backgroundShape");
	if (ttbarCandidate.passesEPlusJetsAntiIsolationSelection()
			&& ttbarCandidate.GoodElectronCleanedJets().size() >= 4) {
		ElectronPointer electron = ttbarCandidate.GoodElectrons().front();
		try {
			ttbarCandidate.reconstructTTbarToEPlusJets(electron);
			float mttbar = ttbarCandidate.mttbar();
			histMan->H1D_BJetBinned("mttbar_antiIsolated")->Fill(mttbar, weight);
			if (ttbarCandidate.MET()->pt() > 20) {
				histMan->H1D_BJetBinned("mttbar_antiIsolated_withMETCut")->Fill(mttbar, weight);
				if (ttbarCandidate.GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCandidate.GoodElectronCleanedJets().at(1)->pt() > 50) {
					histMan->H1D_BJetBinned("mttbar_antiIsolated_withMETAndAsymJets")->Fill(mttbar, weight);
				}
			}
			if (ttbarCandidate.GoodElectronCleanedJets().front()->pt() > 70
					&& ttbarCandidate.GoodElectronCleanedJets().at(1)->pt() > 50) {
				histMan->H1D_BJetBinned("mttbar_antiIsolated_withAsymJetsCut")->Fill(mttbar, weight);
			}

		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
	}

	if (ttbarCandidate.passesEPlusJetsAntiIsolationSelection()
			&& ttbarCandidate.GoodElectronCleanedJets().size() == 3) {
		ElectronPointer electron = ttbarCandidate.GoodElectrons().front();
		try {
			ttbarCandidate.reconstructTTbarToEPlusJetsFrom3Jets(electron);
			float mttbar = ttbarCandidate.mttbar();
			histMan->H1D_BJetBinned("mttbar_3jets_antiIsolated")->Fill(mttbar, weight);
			if (ttbarCandidate.MET()->pt() > 20) {
				if (ttbarCandidate.GoodElectronCleanedJets().front()->pt() > 70
						&& ttbarCandidate.GoodElectronCleanedJets().at(1)->pt() > 50) {
					histMan->H1D_BJetBinned("mttbar_3jets_antiIsolated_withMETAndAsymJets")->Fill(mttbar, weight);
				}
			}
		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
	}

	if (ttbarCandidate.passedEPlusJetsAntiIDSelection() && ttbarCandidate.GoodElectronCleanedJets().size() >= 4) {
		const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
		try {
			ttbarCandidate.reconstructTTbarToEPlusJets(electron);
			const ParticlePointer resonance = ttbarCandidate.getResonance();
			histMan->H1D_BJetBinned("mttbar_antiID")->Fill(resonance->mass(), weight);
			if (ttbarCandidate.passesMETCut() && ttbarCandidate.passesAsymmetricElectronCleanedJetCuts())
				histMan->H1D_BJetBinned("mttbar_antiID_withMETAndAsymJets")->Fill(resonance->mass(), weight);

		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
	}

	if (ttbarCandidate.passedEPlusJetsAntiIDSelection() && ttbarCandidate.GoodElectronCleanedJets().size() == 3) {
		const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
		try {
			ttbarCandidate.reconstructTTbarToEPlusJetsFrom3Jets(electron);
			const ParticlePointer resonance = ttbarCandidate.getResonance();
			histMan->H1D_BJetBinned("mttbar_3jets_antiID")->Fill(resonance->mass(), weight);
			if (ttbarCandidate.passesMETCut() && ttbarCandidate.passesAsymmetricElectronCleanedJetCuts())
				histMan->H1D_BJetBinned("mttbar_3jets_antiID_withMETAndAsymJets")->Fill(resonance->mass(), weight);

		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
	}

	//moved from doNotePlots, do be revised
	histMan->setCurrentHistogramFolder("QCDStudy");
	if (ttbarCandidate.GoodElectrons().size() >= 1 && ttbarCandidate.Jets().size() >= 2) {
		const ElectronCollection electrons = ttbarCandidate.GoodElectrons();
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
			for (unsigned index = 0; index < ttbarCandidate.Jets().size(); ++index) {
				if (ttbarCandidate.Jets().at(index)->isGood())
					goodjets.push_back(ttbarCandidate.Jets().at(index));
			}
			if (goodjets.size() >= 2) {
				unsigned int closestID = electron->getClosestJetIndex(goodjets);
				float minDR = electron->deltaR(goodjets.at(closestID));
				float ptRel = electron->relativePtTo(goodjets.at(closestID));
				histMan->H2D("ptRel_vs_DRmin")->Fill(minDR, ptRel, weight);
				if (ttbarCandidate.MET()->et() < 20 && ttbarCandidate.transverseWmass(electron) < 35) {
					histMan->H1D("DRmin_QCDenriched")->Fill(minDR, weight);
					histMan->H1D("ptRel_QCDenriched")->Fill(ptRel, weight);
				} else if (ttbarCandidate.MET()->et() > 30 && ttbarCandidate.transverseWmass(electron) > 50) {
					histMan->H1D("DRmin_WZenriched")->Fill(minDR, weight);
					histMan->H1D("ptRel_WZenriched")->Fill(ptRel, weight);
				}
			}
		}

	}

	ABCDMethod(Event);
}

void QCDAnalyser::ABCDMethod(const TopPairEventCandidate& event) {

	double weight(event.weight());

	if (passesSelectionWithoutIsolationOrConversionVeto(event)) {
		ElectronPointer electron(event.MostPFIsolatedElectron(event.GoodElectrons()));
		double conversion(0.);
		if (electron->isFromConversion() || electron->isTaggedAsConversion()) {
			conversion = 1.0;
		}
		double pfIso = electron->pfIsolation();
		if (event.GoodElectronCleanedJets().size() >= 4) {
			histMan->H2D_BJetBinned("ABCD_Method")->Fill(pfIso, 0., weight);
			if (event.passesMETCut() && event.passesAsymmetricElectronCleanedJetCuts()) {
				histMan->H2D_BJetBinned("ABCD_Method_WithMETCutAndAsymJetCuts")->Fill(pfIso, conversion, weight);
			}
		}

		if (event.GoodElectronCleanedJets().size() == 3) {
			histMan->H2D_BJetBinned("ABCD_Method_3jets")->Fill(pfIso, 0., weight);
			if (event.passesMETCut() && event.passesAsymmetricElectronCleanedJetCuts()) {
				histMan->H2D_BJetBinned("ABCD_Method_3jets_WithMETCutAndAsymJetCuts")->Fill(pfIso, conversion, weight);
			}
		}
	}

}

/**
 * Require event cleaning, HLT, at least one good vertex, at least 3 good jets, apply Z veto
 * and require to have at least one good electron but at most one good isolated electron
 */
bool QCDAnalyser::passesSelectionWithoutIsolationOrConversionVeto(const TopPairEventCandidate& Event) {
	bool result(Event.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex));
	result = result && Event.passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AtLeastThreeGoodJets);
	result = result && Event.passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::LooseMuonVeto);
	result = result && Event.passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::Zveto);
	//require at least one good electron
	result = result && Event.GoodElectrons().size() > 0;
	//If more than 2 isolated electrons return 0;
	result = result && Event.GoodPFIsolatedElectrons().size() < 2;

	return result;
}

void QCDAnalyser::createHistograms() {
	histMan->setCurrentHistogramFolder("QCDStudy");
	histMan->addH1D_JetBinned("MostPFIsolatedElectron_dPhiIn", "MostPFIsolatedElectron_dPhiIn", 50, 0, 0.1);
	histMan->addH1D_JetBinned("MostPFIsolatedElectron_dEtaIn", "MostPFIsolatedElectron_dEtaIn", 50, 0, 0.02);
	histMan->addH2D("ptRel_vs_DRmin", "ptRel_vs_DRmin", 100, 0, 1, 300, 0, 300);
	histMan->addH1D("ptRel_QCDenriched", "ptRel_QCDenriched", 300, 0, 300);
	histMan->addH1D("DRmin_QCDenriched", "DRmin_QCDenriched", 100, 0, 1);
	histMan->addH1D("ptRel_WZenriched", "ptRel_WZenriched", 300, 0, 300);
	histMan->addH1D("DRmin_WZenriched", "DRmin_WZenriched", 100, 0, 1);
	histMan->addH1D_JetBinned("QCDest_CombRelIso", "RelIso", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_CombRelIso_controlRegion", "RelIso control region", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_CombRelIso_1btag", "RelIso (>=1 btag)", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_CombRelIso_controlRegion_1btag", "RelIso control region", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_CombRelIso_2btag", "RelIso (>=2 btag)", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_CombRelIso_controlRegion_2btag", "RelIso control region", 1000, 0, 10);

	histMan->addH1D_JetBinned("QCDest_PFIsolation", "PFIso", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_WithMETCut", "PFIso", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);

	histMan->addH1D("QCD_electronEta", "Electron eta", 100, -2.4, 2.4);
	histMan->addH1D("QCD_electronAbsEta", "Electron abs(eta)", 100, 0, 2.6);
	histMan->addH1D("QCD_electronPt", "Electron Pt", 100, 0, 200);
	histMan->addH1D("QCD_electronPFIsolation", "Electron PFIso", 100, 0, 5);
	histMan->addH1D("QCD_electronRelIso", "Electron RelIso", 100, 0, 5);

	histMan->addH1D("QCD_goodJetsMultiplicity", "Good jets multiplicity", 4, 3.5, 7.5);
	histMan->addH1D("QCD_jetsMultiplicity", "All jets multiplicity", 4, 3.5, 7.5);
	histMan->addH1D("QCD_btaggedJetsMultiplicity", "b-tagged jets multiplicity", 4, -0.5, 3.5);
	histMan->addH1D("QCD_jetPt", "Jet Pt", 100, 0, 200);
	histMan->addH1D("QCD_jetEta", "Jet Eta", 100, -2.5, 2.5);
	histMan->addH1D("QCD_jetPhi", "Jet Phi", 100, -2.5, 2.5);
	histMan->addH1D("QCD_jetMass", "Jet Mass", 100, 0, 40);

	histMan->addH1D("QCD_1stJetPt", "Leading Jet Pt", 100, 0, 200);
	histMan->addH1D("QCD_1stJetEta", "Leading Jet Eta", 100, -2.5, 2.5);
	histMan->addH1D("QCD_1stJetPhi", "Leading Jet Phi", 100, -2.5, 2.5);
	histMan->addH1D("QCD_1stJetMass", "Leading Jet Mass", 100, 0, 40);

	histMan->addH1D_BJetBinned("PFIsolation", "PFIso", 500, 0, 5);

	histMan->addH1D_BJetBinned("PFIsolation_controlRegion", "PFIso", 500, 0, 5);
//	histMan->addH1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);
	histMan->addH1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts_DR03", "PFIso (deltaR < 0.3);events;pf relIso",
			500, 0, 5);
	histMan->addH1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts_DR04", "PFIso (deltaR < 0.4);events;pf relIso",
			500, 0, 5);
	histMan->addH1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts_DR05", "PFIso (deltaR < 0.5);events;pf relIso",
			500, 0, 5);
	histMan->addH1D_BJetBinned("DirectionalIsolation_WithMETCutAndAsymJetCuts_DR02",
			"PFIso (deltaR < 0.2);events;pf relIso", 500, 0, 5);
	histMan->addH1D_BJetBinned("DirectionalIsolation_WithMETCutAndAsymJetCuts_DR03",
			"PFIso (deltaR < 0.3);events;pf relIso", 500, 0, 5);
	histMan->addH1D_BJetBinned("DirectionalIsolationWithGaussianFallOff_WithMETCutAndAsymJetCuts_DR02",
			"DirectionalIso (deltaR < 0.2);events;pf relIso", 500, 0, 5);
	histMan->addH1D_BJetBinned("DirectionalIsolationWithGaussianFallOff_WithMETCutAndAsymJetCuts_DR03",
			"DirectionalIso (deltaR < 0.3);events;pf relIso", 500, 0, 5);
	histMan->addH1D_BJetBinned("PFIsolation_3jets_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);

	histMan->addH1D_BJetBinned("RelativeIsolation_PUCorrected", "PFIso", 500, 0, 5);
	histMan->addH1D_BJetBinned("RelativeIsolation_PUCorrected_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);

	//histMan->addH1D_JetBinned("QCDest_PFIsolation_WithAsymJetCuts", "PFIso", 1000, 0, 10);

	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion", "PFIso control region", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_1btag", "PFIso (>=1 btag)", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts", "PFIso (>=1 btag)", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_1btag", "PFIso control region", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_2btag", "PFIso (>=2 btag)", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_2btag_WithMETCutAndAsymJetCuts", "PFIso (>=2 btag)", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_2btag", "PFIso control region", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithMETCut", "PFIso control region", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithMETCutAndAsymJetCuts", "PFIso control region", 1000,
			0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithAsymJetCuts", "PFIso control region", 500, 0, 5);

	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2", "PFIso control region", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCut", "PFIso control region", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCutAndAsymJetCuts", "PFIso control region", 500,
			0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithAsymJetCuts", "PFIso control region", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_1btag", "PFIso control region", 500, 0, 5);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_2btag", "PFIso control region", 500, 0, 5);

	histMan->addH2D_BJetBinned("ABCD_Method_3jets", "ABCD Method 3 jets; relative isolation; conversion veto; events",
			100, 0., 1., 2, -0.5, 1.5);
	histMan->addH2D_BJetBinned("ABCD_Method", "ABCD Method #geq 4 jets; relative isolation; conversion veto; events",
			100, 0., 1., 2, -0.5, 1.5);
	histMan->addH2D_BJetBinned("ABCD_Method_3jets_WithMETCutAndAsymJetCuts",
			"ABCD Method 3 jets; relative isolation; conversion veto; events", 100, 0., 1., 2, -0.5, 1.5);
	histMan->addH2D_BJetBinned("ABCD_Method_WithMETCutAndAsymJetCuts",
			"ABCD Method #geq 4 jets; relative isolation; conversion veto; events", 100, 0., 1., 2, -0.5, 1.5);
}

}
