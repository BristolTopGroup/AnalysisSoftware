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

QCDAnalyser::QCDAnalyser(boost::shared_ptr<HistogramManager> histMan):
	histMan(histMan){

}

QCDAnalyser::~QCDAnalyser() {
}

void QCDAnalyser::analyse(TopPairEventCandidate& Event){
	float weight = Event.weight();
	histMan->setCurrentCollection("QCDStudy");
	if (Event.passesEPlusJetsRelIsoSelection()) {
		const ElectronPointer electron = Event.MostIsolatedElectron(Event.Electrons());
		histMan->H1D_JetBinned("QCDest_CombRelIso")->Fill(electron->relativeIsolation(), weight);

		if (Event.GoodBJets().size() >= 1)
			histMan->H1D_JetBinned("QCDest_CombRelIso_1btag")->Fill(electron->relativeIsolation(), weight);

		if (Event.GoodBJets().size() >= 2)
			histMan->H1D_JetBinned("QCDest_CombRelIso_2btag")->Fill(electron->relativeIsolation(), weight);
	}

	if (Event.passesEPlusJetsPFIsoSelection() && Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
		const ElectronPointer electron = Event.MostPFIsolatedElectron(Event.Electrons());
		histMan->H1D_JetBinned("QCDest_PFIsolation")->Fill(electron->pfIsolation(), weight);

		if (Event.GoodBJets().size() >= 1)
			histMan->H1D_JetBinned("QCDest_PFIsolation_1btag")->Fill(electron->pfIsolation(), weight);

		if (Event.GoodBJets().size() >= 2)
			histMan->H1D_JetBinned("QCDest_PFIsolation_2btag")->Fill(electron->pfIsolation(), weight);

		if (Event.GoodJets().size() >= 4)
			histMan->H1D_BJetBinned("PFIsolation")->Fill(electron->pfIsolation(), weight);
		if (Event.MET()->pt() > 20) {
			histMan->H1D_JetBinned("QCDest_PFIsolation_WithMETCut")->Fill(electron->pfIsolation(), weight);

			if (Event.GoodJets().size() >= 2) {
				if (Event.GoodJets().front()->pt() > 70 && Event.GoodJets().at(1)->pt() > 50) {
					histMan->H1D_JetBinned("QCDest_PFIsolation_WithMETCutAndAsymJetCuts")->Fill(
							electron->pfIsolation(), weight);
					if (Event.GoodJets().size() >= 4)
						histMan->H1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts")->Fill(electron->pfIsolation(),
								weight);
				}
			}
		}

	}

	if (Event.passesEPlusJetsRelIsoControlSelection()) {
		const ElectronPointer electron = Event.MostIsolatedElectron(Event.Electrons());
		histMan->H1D_JetBinned("QCDest_CombRelIso_controlRegion")->Fill(electron->relativeIsolation(), weight);

		if (Event.GoodBJets().size() >= 1)
			histMan->H1D_JetBinned("QCDest_CombRelIso_controlRegion_1btag")->Fill(electron->relativeIsolation(), weight);

		if (Event.GoodBJets().size() >= 2)
			histMan->H1D_JetBinned("QCDest_CombRelIso_controlRegion_2btag")->Fill(electron->relativeIsolation(), weight);

		if (Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
			const ElectronPointer electron = Event.MostPFIsolatedElectron(Event.Electrons());
			histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2")->Fill(electron->pfIsolation(), weight);

			if (Event.GoodBJets().size() >= 1)
				histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_1btag")->Fill(electron->pfIsolation(), weight);

			if (Event.GoodBJets().size() >= 2)
				histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_2btag")->Fill(electron->pfIsolation(), weight);

			if (Event.MET()->pt() > 20) {
				histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCut")->Fill(electron->pfIsolation(),
						weight);

				if (Event.GoodJets().size() >= 2) {
					if (Event.GoodJets().front()->pt() > 70 && Event.GoodJets().at(1)->pt() > 50) {
						histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCutAndAsymJetCuts")->Fill(
								electron->pfIsolation(), weight);
					}
				}
			}

			if (Event.GoodJets().size() >= 2) {
				if (Event.GoodJets().front()->pt() > 70 && Event.GoodJets().at(1)->pt() > 50) {
					histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithAsymJetCuts")->Fill(
							electron->pfIsolation(), weight);

				}
			}
		}

	}

	if (Event.passesEPlusJEtsPFIsoControlSelection() && Globals::electronAlgorithm
			== ElectronAlgorithm::ParticleFlow) {
		const ElectronPointer electron = Event.MostPFIsolatedElectron(Event.Electrons());
		histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion")->Fill(electron->pfIsolation(), weight);

		if (Event.GoodBJets().size() >= 1)
			histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion_1btag")->Fill(electron->pfIsolation(), weight);

		if (Event.GoodBJets().size() >= 2)
			histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion_2btag")->Fill(electron->pfIsolation(), weight);
	}

	histMan->setCurrentCollection("topReconstruction/backgroundShape");
	if (Event.passesEPlusJetsRelIsoSelection() && Event.hasAtLeastFourGoodJets()) {
		const ElectronPointer electron = Event.MostIsolatedElectron(Event.Electrons());
		if (electron->relativeIsolation() > Globals::maxElectronRelativeIsolation && !isnan(electron->relativeIsolation()) && !isinf(
				electron->relativeIsolation())) {
			try {
				Event.reconstructTTbar(electron);
				const ParticlePointer resonance = Event.getResonance();
				histMan->H1D_BJetBinned("mttbar_controlRegion")->Fill(resonance->mass(), weight);
				if (Event.MET()->pt() > 20) {
					histMan->H1D_BJetBinned("mttbar_controlRegion_withMETCut")->Fill(resonance->mass(), weight);
					if (Event.GoodJets().front()->pt() > 70 && Event.GoodJets().at(1)->pt() > 50)
						histMan->H1D_BJetBinned("mttbar_controlRegion_withMETAndAsymJets")->Fill(resonance->mass(),
								weight);
				}

				if (Event.GoodJets().front()->pt() > 70 && Event.GoodJets().at(1)->pt() > 50)
					histMan->H1D_BJetBinned("mttbar_controlRegion_withAsymJetsCut")->Fill(resonance->mass(), weight);

			} catch (ReconstructionException& e) {
				cout << "Could not reconstruct event: " << e.what() << endl;
			}
		}
	}

	if (Event.passesConversionSelection()) {
		ElectronPointer electron;
		if (Event::usePFIsolation)
			electron = Event.GoodPFIsolatedElectrons().front();
		else
			electron = Event.GoodIsolatedElectrons().front();
		try {
			Event.reconstructTTbar(electron);
			const ParticlePointer resonance = Event.getResonance();
			histMan->H1D_BJetBinned("mttbar_conversions")->Fill(resonance->mass(), weight);
			if (Event.MET()->pt() > 20) {
				histMan->H1D_BJetBinned("mttbar_conversions_withMETCut")->Fill(resonance->mass(), weight);
				if (Event.GoodJets().front()->pt() > 70 && Event.GoodJets().at(1)->pt() > 50)
					histMan->H1D_BJetBinned("mttbar_conversions_withMETAndAsymJets")->Fill(resonance->mass(), weight);
			}
			if (Event.GoodJets().front()->pt() > 70 && Event.GoodJets().at(1)->pt() > 50)
				histMan->H1D_BJetBinned("mttbar_conversions_withAsymJetsCut")->Fill(resonance->mass(), weight);
		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}

	}

	histMan->setCurrentCollection("QCDStudy");
	if (Event.Electrons().size() > 0 && Event.GoodPFIsolatedElectrons().size() < 2
			&& Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
		const ElectronPointer electron = Event.MostPFIsolatedElectron(Event.Electrons());
		histMan->H1D_JetBinned("MostPFIsolatedElectron_dPhiIn")->Fill(electron->dPhiIn(), weight);
		histMan->H1D_JetBinned("MostPFIsolatedElectron_dEtaIn")->Fill(electron->dEtaIn(), weight);
	}

	histMan->setCurrentCollection("topReconstruction/backgroundShape");
	if (Event.passesEPlusJetsAntiIsolationSelection()) {
		ElectronPointer electron = Event.GoodElectrons().front();
		try {
			Event.reconstructTTbar(electron);
			float mttbar = Event.mttbar();
			histMan->H1D_BJetBinned("mttbar_antiIsolated")->Fill(mttbar, weight);
			if (Event.MET()->pt() > 20) {
				histMan->H1D_BJetBinned("mttbar_antiIsolated_withMETCut")->Fill(mttbar, weight);
				if (Event.GoodJets().front()->pt() > 70 && Event.GoodJets().at(1)->pt() > 50) {
					histMan->H1D_BJetBinned("mttbar_antiIsolated_withMETAndAsymJets")->Fill(mttbar, weight);
				}
			}
			if (Event.GoodJets().front()->pt() > 70 && Event.GoodJets().at(1)->pt() > 50) {
				histMan->H1D_BJetBinned("mttbar_antiIsolated_withAsymJetsCut")->Fill(mttbar, weight);
			}

		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
	}

	if (Event.passedEPlusJetsAntiIDSelection()) {
		const ElectronPointer electron = Event.MostPFIsolatedElectron(Event.Electrons());
		try {
			Event.reconstructTTbar(electron);
			const ParticlePointer resonance = Event.getResonance();
			histMan->H1D_BJetBinned("mttbar_antiID")->Fill(resonance->mass(), weight);
			if (Event.passesMETCut() && Event.passesAsymmetricElectronCleanedJetCuts())
				histMan->H1D_BJetBinned("mttbar_antiID_withMETAndAsymJets")->Fill(resonance->mass(), weight);

		} catch (ReconstructionException& e) {
			cout << "Could not reconstruct event: " << e.what() << endl;
		}
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
	histMan->addH1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);

	//histMan->addH1D_JetBinned("QCDest_PFIsolation_WithAsymJetCuts", "PFIso", 1000, 0, 10);

	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion", "PFIso control region", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_1btag", "PFIso (>=1 btag)", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_1btag", "PFIso control region", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_2btag", "PFIso (>=2 btag)", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_2btag", "PFIso control region", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithMETCut", "PFIso control region", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithMETCutAndAsymJetCuts", "PFIso control region",
			1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithAsymJetCuts", "PFIso control region", 1000, 0, 10);

	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2", "PFIso control region", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCut", "PFIso control region", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCutAndAsymJetCuts", "PFIso control region",
			1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithAsymJetCuts", "PFIso control region", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_1btag", "PFIso control region", 1000, 0, 10);
	histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_2btag", "PFIso control region", 1000, 0, 10);
}

}
