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

	topEPlusJetsReferenceSelection(event);
	topEplusJetsPlusMETSelection(event);
	topEplusJetsZprimeSelection(event);
	qcdSelections(event);
	qcdAsymJetsSelections(event);
	qcdAsymJetsMETSelections(event);
	qcdNonIsoTriggerSelections(event);
	qcdNonIsoTriggerAsymJetsSelections(event);
	qcdNonIsoTriggerAsymJetsMETSelections(event);
}

void EventCountAnalyser::topEPlusJetsReferenceSelection(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight();
	for (unsigned int step = 0; step < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = topEplusJetsRefSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = topEplusJetsRefSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("TTbarEplusJetsRefSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("TTbarEplusJetsRefSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::topEplusJetsPlusMETSelection(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight();
	for (unsigned int step = 0; step < TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = topEplusAsymJetsSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = topEplusAsymJetsSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("TTbarEplusJetsRefAsymJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("TTbarEplusJetsRefAsymJetsSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::topEplusJetsZprimeSelection(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight();
	for (unsigned int step = 0; step < TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = topEplusAsymJetsMETSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = topEplusAsymJetsMETSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("TTbarEplusJetsRefAsymJetsMETSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("TTbarEplusJetsRefAsymJetsMETSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::qcdSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight();
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
			histMan_->H1D("QCDPFRelIsoEPlusJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoEPlusJetsSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::qcdAsymJetsSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight();
	for (unsigned int step = 0; step < TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoElectronAsymJetsSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoElectronAsymJetsSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoEplusJetsAsymJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDNonIsoEplusJetsAsymJetsSelection_singleCuts")->Fill(step, weight_);

		passesStep = qcdConversionAsymJetsSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdConversionAsymJetsSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDConversionAsymJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDConversionAsymJetsSelection_singleCuts")->Fill(step, weight_);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoAsymJetsSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoAsymJetsSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDPFRelIsoEPlusJetsAsymJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoEPlusJetsAsymJetsSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::qcdAsymJetsMETSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight();
	for (unsigned int step = 0; step < TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoElectronAsymJetsMETSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoElectronAsymJetsMETSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoEplusJetsAsymJetsMETSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDNonIsoEplusJetsAsymJetsMETSelection_singleCuts")->Fill(step, weight_);

		passesStep = qcdConversionAsymJetsMETSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdConversionAsymJetsMETSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDConversionAsymJetsMETSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDConversionAsymJetsMETSelection_singleCuts")->Fill(step, weight_);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoAsymJetsMETSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoAsymJetsMETSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDPFRelIsoEPlusJetsAsymJetsMETSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoEPlusJetsAsymJetsMETSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::qcdNonIsoTriggerSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight();
	unsigned int prescale(qcdNonIsoElectronNonIsoTriggerSelection_->prescale(event));

	for (unsigned int step = 0; step < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoElectronNonIsoTriggerSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoElectronNonIsoTriggerSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerSelection")->Fill(step, weight_ * prescale);
		if (passesStep)
			histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerSelection_singleCuts")->Fill(step, weight_ * prescale);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoNonIsoTriggerSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoNonIsoTriggerSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerSelection")->Fill(step, weight_ * prescale);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerSelection_singleCuts")->Fill(step, weight_ * prescale);
	}
}

void EventCountAnalyser::qcdNonIsoTriggerAsymJetsSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight();
	unsigned int prescale(qcdNonIsoElectronAsymJetsNonIsoTriggerSelection_->prescale(event));

	for (unsigned int step = 0; step < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoElectronAsymJetsNonIsoTriggerSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoElectronAsymJetsNonIsoTriggerSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerAsymJetsSelection")->Fill(step, weight_ * prescale);
		if (passesStep)
			histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerAsymJetsSelection_singleCuts")->Fill(step, weight_ * prescale);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerAsymJetsSelection")->Fill(step, weight_ * prescale);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerAsymJetsSelection_singleCuts")->Fill(step, weight_ * prescale);
	}
}

void EventCountAnalyser::qcdNonIsoTriggerAsymJetsMETSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight();
	unsigned int prescale(qcdNonIsoElectronAsymJetsMETNonIsoTriggerSelection_->prescale(event));

	for (unsigned int step = 0; step < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoElectronAsymJetsMETNonIsoTriggerSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoElectronAsymJetsMETNonIsoTriggerSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerAsymJetsMETSelection")->Fill(step, weight_ * prescale);
		if (passesStep)
			histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerAsymJetsMETSelection_singleCuts")->Fill(step, weight_ * prescale);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerAsymJetsMETSelection")->Fill(step, weight_ * prescale);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerAsymJetsMETSelection_singleCuts")->Fill(step,
					weight_ * prescale);
	}
}

void EventCountAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	const boost::array<string, 6> refSelections = { { "TTbarEplusJetsRef", "QCDNonIsoEplusJets",
			"QCDNonIsoEplusJetsNonIsoTrigger", "QCDConversion", "QCDPFRelIsoEPlusJets",
			"QCDPFRelIsoEPlusJetsNonIsoTrigger" } };

	for (unsigned int index = 0; index < refSelections.size(); ++index) {
		string selection = refSelections.at(index) + "Selection";
		histMan_->addH1D(selection, selection, TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
				TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
		histMan_->addH1D(selection + "_singleCuts", selection + " (single cuts",
				TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
				TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

		selection = refSelections.at(index) + "AsymJetsSelection";

		histMan_->addH1D(selection, selection, TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
				TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
		histMan_->addH1D(selection + "_singleCuts", selection + " (single cuts",
				TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
				TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

		selection = refSelections.at(index) + "AsymJetsMETSelection";
		histMan_->addH1D(selection, selection, TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
				TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
		histMan_->addH1D(selection + "_singleCuts", selection + " (single cuts",
				TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
				TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

	}

////Top reference selection
//	histMan_->addH1D("TTbarEplusJetsRefSelection", "TTbarEplusJetsRefSelection",
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//
//	histMan_->addH1D("TTbarEplusJetsRefSelection_singleCuts", "TTbarEplusJetsRefSelection (single cuts",
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//	//Top reference selection + asymmetric jet cuts
//	histMan_->addH1D("TTbarEplusJetsRefAsymJetsSelection", "TTbarEplusJetsRefAsymJetsSelection",
//			TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//
//	histMan_->addH1D("TTbarEplusJetsRefAsymJetsSelection_singleCuts", "TTbarEplusJetsRefAsymJetsSelection (single cuts",
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//	//Top reference selection + asymmetric jet cuts + MET cut
//	histMan_->addH1D("TTbarEplusJetsRefAsymJetsMETSelection", "TTbarEplusJetsRefAsymJetsMETSelection",
//			TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//
//	histMan_->addH1D("TTbarEplusJetsRefAsymJetsMETSelection_singleCuts", "TTbarEplusJetsRefAsymJetsMETSelection (single cuts",
//			TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//	//QCD selections
//	histMan_->addH1D("QCDNonIsoEplusJetsSelection", "QCDNonIsoEplusJetsSelection",
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//
//	histMan_->addH1D("QCDNonIsoEplusJetsSelection_singleCuts", "QCDNonIsoEplusJetsSelection (single cuts",
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//	//using non-isolated (prescaled) e + jets trigger
//	histMan_->addH1D("QCDNonIsoEplusJetsNonIsoTriggerSelection", "QCDNonIsoEplusJetsSelection",
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//
//	histMan_->addH1D("QCDNonIsoEplusJetsNonIsoTriggerSelection_singleCuts", "QCDNonIsoEplusJetsSelection (single cuts",
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//
//	histMan_->addH1D("QCDConversionSelection", "QCDConversionSelection",
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//
//	histMan_->addH1D("QCDConversionSelection_singleCuts", "QCDConversionSelection (single cuts",
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//
//	histMan_->addH1D("QCDPFRelIsoEPlusJetsSelection", "QCDPFRelIsoEPlusJetsSelection",
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//
//	histMan_->addH1D("QCDPFRelIsoEPlusJetsSelection_singleCuts", "QCDPFRelIsoEPlusJetsSelection (single cuts",
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//	//using non-isolated (prescaled) e + jets trigger
//	histMan_->addH1D("QCDPFRelIsoEPlusJetsNonIsoTriggerSelection", "QCDPFRelIsoEPlusJetsSelection",
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS, -0.5,
//			TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
//
//	histMan_->addH1D("QCDPFRelIsoEPlusJetsNonIsoTriggerSelection_singleCuts",
//			"QCDPFRelIsoEPlusJetsSelection (single cuts", TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS,
//			-0.5, TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

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
		histMan_->H1D("QCDPFRelIsoEPlusJetsSelection")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDPFRelIsoEPlusJetsSelection_singleCuts")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
	}
}

EventCountAnalyser::EventCountAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		//signal selections
		topEplusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection()), //
		topEplusAsymJetsSelection_(new TopPairEplusJetsRefAsymJetsSelection()), //
		topEplusAsymJetsMETSelection_(new TopPairEPlusJetsRefAsymJetsMETSelection()), //
		//QCD selections with respect to reference selection
		qcdNonIsoElectronSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdNonIsoElectronNonIsoTriggerSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdConversionSelection_(new QCDConversionsSelection()), //
		qcdPFRelIsoSelection_(new QCDPFRelIsoEPlusJetsSelection()), //
		qcdPFRelIsoNonIsoTriggerSelection_(new QCDPFRelIsoEPlusJetsSelection()), //
		//QCD selections with respect to reference selection + asymmetric jet cuts
		qcdNonIsoElectronAsymJetsSelection_(new QCDNonIsolatedElectronAsymJetsSelection()), //
		qcdNonIsoElectronAsymJetsNonIsoTriggerSelection_(new QCDNonIsolatedElectronAsymJetsSelection()), //
		qcdConversionAsymJetsSelection_(new QCDConversionsAsymJetsSelection()), //
		qcdPFRelIsoAsymJetsSelection_(new QCDPFRelIsoEPlusAsymJetsSelection()), //
		qcdPFRelIsoNonIsoTriggerAsymJetsSelection_(new QCDPFRelIsoEPlusAsymJetsSelection()), //
		//QCD selections with respect to reference selection + MET + asymmetric jet cuts
		qcdNonIsoElectronAsymJetsMETSelection_(new QCDNonIsolatedElectronAsymJetsMETSelection()), //
		qcdNonIsoElectronAsymJetsMETNonIsoTriggerSelection_(new QCDNonIsolatedElectronAsymJetsMETSelection()), //
		qcdConversionAsymJetsMETSelection_(new QCDConversionsAsymJetsMETSelection()), //
		qcdPFRelIsoAsymJetsMETSelection_(new QCDPFRelIsoEPlusAsymJetsMET()), //
		qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_(new QCDPFRelIsoEPlusAsymJetsMET()) {
	qcdNonIsoElectronNonIsoTriggerSelection_->useNonIsoTrigger(true);
	qcdNonIsoElectronAsymJetsNonIsoTriggerSelection_->useNonIsoTrigger(true);
	qcdNonIsoElectronAsymJetsMETNonIsoTriggerSelection_->useNonIsoTrigger(true);

	qcdPFRelIsoNonIsoTriggerSelection_->useNonIsoTrigger(true);
	qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->useNonIsoTrigger(true);
	qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->useNonIsoTrigger(true);
}

EventCountAnalyser::~EventCountAnalyser() {
}

} /* namespace BAT */
