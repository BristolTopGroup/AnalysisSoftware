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
	histMan->setCurrentCollection("QCDStudy");
	if (ttbarCandidate.passesEPlusJetsRelIsoSelection()) {
		const ElectronPointer electron = ttbarCandidate.MostIsolatedElectron(ttbarCandidate.Electrons());
		histMan->H1D_JetBinned("QCDest_CombRelIso")->Fill(electron->relativeIsolation(), weight);

		if (ttbarCandidate.GoodJets().size() >= 4) {
			double rho = ttbarCandidate.rho();
			histMan->H1D_BJetBinned("RelativeIsolation_PUCorrected")->Fill(electron->relativeIsolationPUCorrected(rho),
					weight);
			if (ttbarCandidate.passesMETCut() && ttbarCandidate.passesAsymmetricElectronCleanedJetCuts()) {
				histMan->H1D_BJetBinned("RelativeIsolation_PUCorrected_WithMETCutAndAsymJetCuts")->Fill(
						electron->relativeIsolationPUCorrected(rho), weight);
			}
		}

		if (ttbarCandidate.GoodBJets().size() >= 1)
			histMan->H1D_JetBinned("QCDest_CombRelIso_1btag")->Fill(electron->relativeIsolation(), weight);

		if (ttbarCandidate.GoodBJets().size() >= 2)
			histMan->H1D_JetBinned("QCDest_CombRelIso_2btag")->Fill(electron->relativeIsolation(), weight);
	}

	if (ttbarCandidate.passesEPlusJetsPFIsoSelection()
			&& Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
		const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
		histMan->H1D_JetBinned("QCDest_PFIsolation")->Fill(electron->pfIsolation(), weight);

		if (ttbarCandidate.GoodBJets().size() == 1) {
			histMan->H1D_JetBinned("QCDest_PFIsolation_1btag")->Fill(electron->pfIsolation(), weight);
			if (ttbarCandidate.passesAsymmetricElectronCleanedJetCuts() && ttbarCandidate.passesMETCut()) {
				histMan->H1D_JetBinned("QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts")->Fill(
						electron->pfIsolation(), weight);
			}
		}

		if (ttbarCandidate.GoodBJets().size() == 2) {
			histMan->H1D_JetBinned("QCDest_PFIsolation_2btag")->Fill(electron->pfIsolation(), weight);
			if (ttbarCandidate.passesAsymmetricElectronCleanedJetCuts() && ttbarCandidate.passesMETCut()) {
				histMan->H1D_JetBinned("QCDest_PFIsolation_2btag_WithMETCutAndAsymJetCuts")->Fill(
						electron->pfIsolation(), weight);
			}
		}

		if (ttbarCandidate.GoodJets().size() >= 4) {
			histMan->H1D_BJetBinned("PFIsolation")->Fill(electron->pfIsolation(), weight);
		}

		if (ttbarCandidate.MET()->pt() > 20) {
			histMan->H1D_JetBinned("QCDest_PFIsolation_WithMETCut")->Fill(electron->pfIsolation(), weight);

			if (ttbarCandidate.GoodJets().size() >= 2) {
				if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
					histMan->H1D_JetBinned("QCDest_PFIsolation_WithMETCutAndAsymJetCuts")->Fill(electron->pfIsolation(),
							weight);

					if (ttbarCandidate.GoodElectronCleanedJets().size() == 3)
						histMan->H1D_BJetBinned("PFIsolation_3jets_WithMETCutAndAsymJetCuts")->Fill(
								electron->pfIsolation(), weight);

					if (ttbarCandidate.GoodJets().size() >= 4)
						histMan->H1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts")->Fill(electron->pfIsolation(),
								weight);
				}
			}
		}

	}

	if (ttbarCandidate.passesEPlusJetsRelIsoControlSelection()) {
		const ElectronPointer electron = ttbarCandidate.MostIsolatedElectron(ttbarCandidate.Electrons());
		histMan->H1D_JetBinned("QCDest_CombRelIso_controlRegion")->Fill(electron->relativeIsolation(), weight);

		if (ttbarCandidate.GoodBJets().size() >= 1)
			histMan->H1D_JetBinned("QCDest_CombRelIso_controlRegion_1btag")->Fill(electron->relativeIsolation(),
					weight);

		if (ttbarCandidate.GoodBJets().size() >= 2)
			histMan->H1D_JetBinned("QCDest_CombRelIso_controlRegion_2btag")->Fill(electron->relativeIsolation(),
					weight);

		if (Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
			const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
			histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2")->Fill(electron->pfIsolation(), weight);

			if (ttbarCandidate.GoodBJets().size() >= 1)
				histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_1btag")->Fill(electron->pfIsolation(),
						weight);

			if (ttbarCandidate.GoodBJets().size() >= 2)
				histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_2btag")->Fill(electron->pfIsolation(),
						weight);

			if (ttbarCandidate.MET()->pt() > 20) {
				histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCut")->Fill(electron->pfIsolation(),
						weight);

				if (ttbarCandidate.GoodJets().size() >= 2) {
					if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
						histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCutAndAsymJetCuts")->Fill(
								electron->pfIsolation(), weight);
					}
				}
			}

			if (ttbarCandidate.GoodJets().size() >= 2) {
				if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
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

		if (ttbarCandidate.GoodBJets().size() == 1)
			histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion_1btag")->Fill(electron->pfIsolation(), weight);

		if (ttbarCandidate.GoodBJets().size() == 2)
			histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion_2btag")->Fill(electron->pfIsolation(), weight);

	}

	if (ttbarCandidate.passesEPlusJEtsPFIsoControlSelection()
			&& Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow
			&& ttbarCandidate.GoodElectronCleanedJets().size() >= 4) {
		const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
		histMan->H1D_BJetBinned("PFIsolation_controlRegion")->Fill(electron->pfIsolation(), weight);
	}

	histMan->setCurrentCollection("topReconstruction/backgroundShape");
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
					if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
						histMan->H1D_BJetBinned("mttbar_controlRegion_withMETAndAsymJets")->Fill(resonance->mass(),
								weight);
				}

				if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
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
				if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
					histMan->H1D_BJetBinned("mttbar_conversions_withMETAndAsymJets")->Fill(resonance->mass(), weight);
			}
			if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
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
				if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
					histMan->H1D_BJetBinned("mttbar_3jets_conversions_withMETAndAsymJets")->Fill(resonance->mass(),
							weight);
			}
		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
	}

	histMan->setCurrentCollection("QCDStudy");
	if (ttbarCandidate.Electrons().size() > 0 && ttbarCandidate.GoodPFIsolatedElectrons().size() < 2
			&& Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
		const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
		histMan->H1D_JetBinned("MostPFIsolatedElectron_dPhiIn")->Fill(electron->dPhiIn(), weight);
		histMan->H1D_JetBinned("MostPFIsolatedElectron_dEtaIn")->Fill(electron->dEtaIn(), weight);
	}

	histMan->setCurrentCollection("topReconstruction/backgroundShape");
	if (ttbarCandidate.passesEPlusJetsAntiIsolationSelection()
			&& ttbarCandidate.GoodElectronCleanedJets().size() >= 4) {
		ElectronPointer electron = ttbarCandidate.GoodElectrons().front();
		try {
			ttbarCandidate.reconstructTTbarToEPlusJets(electron);
			float mttbar = ttbarCandidate.mttbar();
			histMan->H1D_BJetBinned("mttbar_antiIsolated")->Fill(mttbar, weight);
			if (ttbarCandidate.MET()->pt() > 20) {
				histMan->H1D_BJetBinned("mttbar_antiIsolated_withMETCut")->Fill(mttbar, weight);
				if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
					histMan->H1D_BJetBinned("mttbar_antiIsolated_withMETAndAsymJets")->Fill(mttbar, weight);
				}
			}
			if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
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
				if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
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
	histMan->setCurrentCollection("QCDStudy");
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

	//signal region >= 4 jets
	if (event.passesFullTTbarEPlusJetSelection()) {
		double pfIso = event.GoodIsolatedElectrons().front()->pfIsolation();
		histMan->H2D_BJetBinned("ABCD_Method")->Fill(pfIso, 0., weight);
		if (event.passesMETCut() && event.passesAsymmetricElectronCleanedJetCuts()) {
			histMan->H2D_BJetBinned("ABCD_Method_WithMETCutAndAsymJetCuts")->Fill(pfIso, 0., weight);
		}
	}

	//signal region ==3 jets
	if (event.GoodElectronCleanedJets().size() == 3
			&& event.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastThreeGoodJets)) {
		double pfIso = event.GoodIsolatedElectrons().front()->pfIsolation();
		histMan->H2D_BJetBinned("ABCD_Method_3jets")->Fill(pfIso, 0., weight);
		if (event.passesMETCut() && event.passesAsymmetricElectronCleanedJetCuts()) {
			histMan->H2D_BJetBinned("ABCD_Method_3jets_WithMETCutAndAsymJetCuts")->Fill(pfIso, 0., weight);
		}
	}

	//no isolation requirement
	if (event.passesEPlusJetsPFIsoSelection()) {

	}
}

void QCDAnalyser::createHistograms() {
	histMan->setCurrentCollection("QCDStudy");
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

	histMan->addH1D_BJetBinned("PFIsolation", "PFIso", 500, 0, 5);

	histMan->addH1D_BJetBinned("PFIsolation_controlRegion", "PFIso", 500, 0, 5);
	histMan->addH1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);
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
