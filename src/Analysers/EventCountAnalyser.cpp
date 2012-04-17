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
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight();
	topEPlusJetsReferenceSelection(event);
	qcdSelections(event);
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));

	for (unsigned int step = 0; step < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = ttbarCand->passesEPlusJetsSelectionStepUpTo((TTbarEPlusJetsSelection::Step) step);
		if (passesStep) {
			histMan_->H1D("TTbarEplusJetsSelection_old")->Fill(step, weight_);

		}

		if (step < TTbarEPlusJetsSelection::AtLeastFourGoodJets && passesStep) {
			histMan_->H1D("TTbarEplusJetsSelection_old_3jets")->Fill(step, weight_);
			histMan_->H1D("TTbarEplusJetsSelection_old_noMETCut_3jets")->Fill(step, weight_);
		}

		if (step < TTbarEPlusJetsSelection::MissingTransverseEnergy && passesStep)
			histMan_->H1D("TTbarEplusJetsSelection_old_noMETCut")->Fill(step, weight_);
	}

	if (ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastThreeGoodJets)) {
		if (ttbarCand->GoodElectronCleanedJets().size() == 3) {
			histMan_->H1D("TTbarEplusJetsSelection_old_3jets")->Fill(
					(double) TTbarEPlusJetsSelection::AtLeastFourGoodJets, weight_);
			histMan_->H1D("TTbarEplusJetsSelection_old_noMETCut_3jets")->Fill(
					(double) TTbarEPlusJetsSelection::AtLeastFourGoodJets, weight_);

			if (ttbarCand->passesMETCut()) {
				histMan_->H1D("TTbarEplusJetsSelection_old_3jets")->Fill(
						(double) TTbarEPlusJetsSelection::MissingTransverseEnergy, weight_);

				if (ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
					histMan_->H1D("TTbarEplusJetsSelection_old_3jets")->Fill(
							(double) TTbarEPlusJetsSelection::AsymmetricJetCuts, weight_);
				}
			}

			if (ttbarCand->passesAsymmetricElectronCleanedJetCuts()) {
				histMan_->H1D("TTbarEplusJetsSelection_old_noMETCut_3jets")->Fill(
						(double) TTbarEPlusJetsSelection::AsymmetricJetCuts, weight_);
			}

		}
	}

	if (ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets)
			&& ttbarCand->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AsymmetricJetCuts)) {
		histMan_->H1D("TTbarEplusJetsSelection_old_noMETCut")->Fill((double) TTbarEPlusJetsSelection::AsymmetricJetCuts,
				weight_);

	}

}

void EventCountAnalyser::topEPlusJetsReferenceSelection(const EventPtr event) {

	for (unsigned int step = 0; step < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = topEplusJetsRefSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = topEplusJetsRefSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("TTbarEplusJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("TTbarEplusJetsSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::qcdSelections(const EventPtr event) {
	for (unsigned int step = 0; step < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoElectronSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoElectronSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoEplusJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDNonIsoEplusJetsSelection_singleCuts")->Fill(step, weight_);

		passesStep = qcdConversionSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdConversionSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDConversionSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDConversionSelection_singleCuts")->Fill(step, weight_);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDEPlusJetsPFRelIsoSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDEPlusJetsPFRelIsoSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	histMan_->addH1D("TTbarEplusJetsSelection_old", "TTbarEplusJetsSelection",
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	histMan_->addH1D("TTbarEplusJetsSelection", "TTbarEplusJetsSelection",
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	histMan_->addH1D("TTbarEplusJetsSelection_singleCuts", "TTbarEplusJetsSelection (single cuts",
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	//all cuts except the b-tag and MET cuts
	histMan_->addH1D("TTbarEplusJetsSelection_old_noMETCut", "TTbarEplusJetsSelection",
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	//all cuts except the b-tag cuts
	histMan_->addH1D("TTbarEplusJetsSelection_old_3jets", "TTbarEplusJetsSelection",
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
	//all cuts except the b-tag and MET cuts
	histMan_->addH1D("TTbarEplusJetsSelection_old_noMETCut_3jets", "TTbarEplusJetsSelection",
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	histMan_->addH1D("TTbarEplusJetsSelection_old", "TTbarEplusJetsSelection",
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	//QCD selections
	histMan_->addH1D("QCDNonIsoEplusJetsSelection", "QCDNonIsoEplusJetsSelection",
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	histMan_->addH1D("QCDNonIsoEplusJetsSelection_singleCuts", "QCDNonIsoEplusJetsSelection (single cuts",
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	histMan_->addH1D("QCDConversionSelection", "QCDConversionSelection",
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	histMan_->addH1D("QCDConversionSelection_singleCuts", "QCDConversionSelection (single cuts",
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	histMan_->addH1D("QCDEPlusJetsPFRelIsoSelection", "QCDEPlusJetsPFRelIsoSelection",
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	histMan_->addH1D("QCDEPlusJetsPFRelIsoSelection_singleCuts", "QCDEPlusJetsPFRelIsoSelection (single cuts",
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	//this does not work as it tries to access histograms without knowing the data type
//	setHistogramLabels();
}

void EventCountAnalyser::setHistogramLabels() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	for (unsigned int index = 0; index < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++index) {
		string binLabel = TTbarEPlusJetsSelection::StringSteps[index];
		histMan_->H1D("TTbarEplusJetsSelection_old")->GetXaxis()->SetBinLabel(index + 1, binLabel.c_str());
	}

	for (unsigned int index = 0; index < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++index) {
		unsigned int bin(index + 1);
		string binLabel = TTbarEPlusJetsReferenceSelection::StringSteps[index];

		histMan_->H1D("TTbarEplusJetsSelection")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("TTbarEplusJetsSelection_singleCuts")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDNonIsoEplusJetsSelection")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDNonIsoEplusJetsSelection_singleCuts")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDConversionSelection")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDConversionSelection_singleCuts")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDEPlusJetsPFRelIsoSelection")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDEPlusJetsPFRelIsoSelection_singleCuts")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
	}
}

EventCountAnalyser::EventCountAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		topEplusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection()), //
		qcdNonIsoElectronSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdConversionSelection_(new QCDConversionSelection()), //
		qcdPFRelIsoSelection_(new QCDPFRelIsoSelection()) {

}

EventCountAnalyser::~EventCountAnalyser() {
}

} /* namespace BAT */
