/*
 * TTbarPlusMETAnalyser.cpp
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Analysers/TTbarPlusMETAnalyser.h"
//#include "../../interface/TopPairEventCandidate.h"
//signal selections
#include "../../interface/Selections/TopPairEplusJetsRefAsymJetsSelection.h"
#include "../../interface/Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../../interface/Selections/TopPairEPlusJetsRefAsymJetsMETSelection.h"
//QCD selections w.r.t signal ref selection
#include "../../interface/Selections/QCDNonIsolatedElectronSelection.h"
#include "../../interface/Selections/QCDConversionsSelection.h"
#include "../../interface/Selections/QCDPFRelIsoEPlusJetsSelection.h"
//QCD selections w.r.t signal ref selection + asymmetric jets
#include "../../interface/Selections/QCDNonIsolatedElectronAsymJetsSelection.h"
#include "../../interface/Selections/QCDConversionsAsymJetsSelection.h"
#include "../../interface/Selections/QCDPFRelIsoEPlusAsymJetsSelection.h"
//QCD selections w.r.t signal ref selection + asymmetric jets + MET
#include "../../interface/Selections/QCDNonIsolatedElectronAsymJetsMETSelection.h"
#include "../../interface/Selections/QCDConversionsAsymJetsMETSelection.h"
#include "../../interface/Selections/QCDPFRelIsoEPlusAsymJetsMET.h"

namespace BAT {

void TTbarPlusMETAnalyser::analyse(const EventPtr event) {
	signalAnalysis(event);
	qcdAnalysis(event);
	qcdAnalysisAsymJets(event);
	qcdAnalysisAsymJetsMET(event);
}

void TTbarPlusMETAnalyser::signalAnalysis(const EventPtr event) {

	if (topEplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(topEplusJetsRefSelection_->cleanedJets(event));
		const JetCollection bJets(topEplusJetsRefSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		const LeptonPointer signalLepton = topEplusJetsRefSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		metAnalyserRefSelection_->analyse(event);
		metAnalyserRefSelection_->analyseTransverseMass(event, signalLepton);

		electronAnalyserRefSelection_->analyse(event);
		electronAnalyserRefSelection_->analyseElectron(signalElectron, event->weight());

		vertexAnalyserRefSelection_->analyse(event);
		for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
			double upperCut = index < metBins_.size() ? metBins_.at(index) : 999999.;
			double lowerCut = index == 0 ? 0. : metBins_.at(index - 1);
			const METPointer met(event->MET(METAlgorithm::patType1CorrectedPFMet));
			if (met->et() > lowerCut && met->et() < upperCut)
				binnedElectronAnalysers_.at(index)->analyseElectron(signalElectron, event->weight());
		}
	}

	if (topEplusAsymJetsMETSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(topEplusAsymJetsMETSelection_->cleanedJets(event));
		const JetCollection bJets(topEplusAsymJetsMETSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		const LeptonPointer signalLepton = topEplusAsymJetsMETSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		metAnalyserRefAsymJetsMETSelection_->analyse(event);
		metAnalyserRefAsymJetsMETSelection_->analyseTransverseMass(event, signalLepton);

		electronAnalyserRefAsymJetsMETSelection_->analyse(event);
		electronAnalyserRefAsymJetsMETSelection_->analyseElectron(signalElectron, event->weight());
	}

	if (topEplusAsymJetsSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(topEplusAsymJetsSelection_->cleanedJets(event));
		const JetCollection bJets(topEplusAsymJetsSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		const LeptonPointer signalLepton = topEplusAsymJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		metAnalyserRefAsymJetsSelection_->analyse(event);
		metAnalyserRefAsymJetsSelection_->analyseTransverseMass(event, signalLepton);

		electronAnalyserRefAsymJetsSelection_->analyse(event);
		electronAnalyserRefAsymJetsSelection_->analyseElectron(signalElectron, event->weight());
	}

}

void TTbarPlusMETAnalyser::qcdAnalysis(const EventPtr event) {
	//selection with respect to reference selection
	if (qcdNonIsoElectronSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdNonIsoElectronSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNonIsoElectronSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoSelection_->setPrescale(prescale);

		qcdNonIsoElectronAnalyser_->analyse(event);
		qcdNonIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoSelection_->analyse(event);
		metAnalyserqcdNonIsoSelection_->analyseTransverseMass(event, signalLepton);

		for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
			double upperCut = index < metBins_.size() ? metBins_.at(index) : 999999.;
			double lowerCut = index == 0 ? 0. : metBins_.at(index - 1);
			const METPointer met(event->MET(METAlgorithm::patType1CorrectedPFMet));
			if (met->et() > lowerCut && met->et() < upperCut)
				qcdNonIsoBinnedElectronAnalysers_.at(index)->analyseElectron(signalElectron, event->weight());
		}
	}

	if (qcdNonIsoElectronNonIsoTriggerSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdNonIsoElectronNonIsoTriggerSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNonIsoElectronNonIsoTriggerSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronNonIsoTriggerSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronNonIsoTriggerSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoNonIsoTriggerElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoNonIsoTriggerSelection_->setPrescale(prescale);

		qcdNonIsoNonIsoTriggerElectronAnalyser_->analyse(event);
		qcdNonIsoNonIsoTriggerElectronAnalyser_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoNonIsoTriggerSelection_->analyse(event);
		metAnalyserqcdNonIsoNonIsoTriggerSelection_->analyseTransverseMass(event, signalLepton);
	}

	if (qcdConversionSelection_->passesSelectionUpToStep(event, TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdConversionSelection_->cleanedJets(event));
		const JetCollection bJets(qcdConversionSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		unsigned int prescale(qcdConversionSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdConversionSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdConversionsElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdConversionSelection_->setPrescale(prescale);

		qcdConversionsElectronAnalyser_->analyse(event);
		qcdConversionsElectronAnalyser_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdConversionSelection_->analyse(event);
		metAnalyserqcdConversionSelection_->analyseTransverseMass(event, signalLepton);

		for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
			double upperCut = index < metBins_.size() ? metBins_.at(index) : 999999.;
			double lowerCut = index == 0 ? 0. : metBins_.at(index - 1);
			const METPointer met(event->MET(METAlgorithm::patType1CorrectedPFMet));
			if (met->et() > lowerCut && met->et() < upperCut)
				qcdConversionBinnedElectronAnalysers_.at(index)->analyseElectron(signalElectron, event->weight());
		}
	}

	if (qcdPFRelIsoSelection_->passesSelectionUpToStep(event, TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdPFRelIsoSelection_->cleanedJets(event));
		const JetCollection bJets(qcdPFRelIsoSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		unsigned int prescale(qcdPFRelIsoSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoElectronAnalyser_->setPrescale(prescale);

		qcdEPlusjetsPFRelIsoElectronAnalyser_->analyse(event);
		qcdEPlusjetsPFRelIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());
	}

	if (qcdPFRelIsoNonIsoTriggerSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdPFRelIsoNonIsoTriggerSelection_->cleanedJets(event));
		const JetCollection bJets(qcdPFRelIsoNonIsoTriggerSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		unsigned int prescale(qcdPFRelIsoNonIsoTriggerSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoNonIsoTriggerSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->setPrescale(prescale);

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->analyse(event);
		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->analyseElectron(signalElectron, event->weight());
	}

}

void TTbarPlusMETAnalyser::qcdAnalysisAsymJets(const EventPtr event) {
	//selection with respect to reference selection + asym. jet cuts
	if (qcdNonIsoElectronAsymJetsSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(qcdNonIsoElectronAsymJetsSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNonIsoElectronAsymJetsSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronAsymJetsSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronAsymJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoElectronAnalyserAsymJets_->setPrescale(prescale);
		metAnalyserqcdNonIsoAsymJetsSelection_->setPrescale(prescale);

		qcdNonIsoElectronAnalyserAsymJets_->analyse(event);
		qcdNonIsoElectronAnalyserAsymJets_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoAsymJetsSelection_->analyse(event);
		metAnalyserqcdNonIsoAsymJetsSelection_->analyseTransverseMass(event, signalLepton);
	}

	if (qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJets_->setPrescale(prescale);
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsSelection_->setPrescale(prescale);

		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJets_->analyse(event);
		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJets_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsSelection_->analyse(event);
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsSelection_->analyseTransverseMass(event, signalLepton);
	}

	if (qcdConversionAsymJetsSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(qcdConversionAsymJetsSelection_->cleanedJets(event));
		const JetCollection bJets(qcdConversionAsymJetsSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		unsigned int prescale(qcdConversionAsymJetsSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdConversionAsymJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdConversionsElectronAnalyserAsymJets_->setPrescale(prescale);
		metAnalyserqcdConversionAsymJetsSelection_->setPrescale(prescale);

		qcdConversionsElectronAnalyserAsymJets_->analyse(event);
		qcdConversionsElectronAnalyserAsymJets_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdConversionAsymJetsSelection_->analyse(event);
		metAnalyserqcdConversionAsymJetsSelection_->analyseTransverseMass(event, signalLepton);
	}

	if (qcdPFRelIsoAsymJetsSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(qcdPFRelIsoAsymJetsSelection_->cleanedJets(event));
		const JetCollection bJets(qcdPFRelIsoAsymJetsSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		unsigned int prescale(qcdPFRelIsoAsymJetsSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoAsymJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJets_->setPrescale(prescale);

		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJets_->analyse(event);
		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJets_->analyseElectron(signalElectron, event->weight());
	}

	if (qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->cleanedJets(event));
		const JetCollection bJets(qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		unsigned int prescale(qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJets_->setPrescale(prescale);

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJets_->analyse(event);
		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJets_->analyseElectron(signalElectron, event->weight());
	}

}

void TTbarPlusMETAnalyser::qcdAnalysisAsymJetsMET(const EventPtr event) {
	//selection with respect to reference selection + asym. jet cuts
	if (qcdNonIsoElectronAsymJetsMETSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(qcdNonIsoElectronAsymJetsMETSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNonIsoElectronAsymJetsMETSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronAsymJetsMETSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronAsymJetsMETSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoElectronAnalyserAsymJetsMET_->setPrescale(prescale);
		metAnalyserqcdNonIsoAsymJetsMETSelection_->setPrescale(prescale);

		qcdNonIsoElectronAnalyserAsymJetsMET_->analyse(event);
		qcdNonIsoElectronAnalyserAsymJetsMET_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoAsymJetsMETSelection_->analyse(event);
		metAnalyserqcdNonIsoAsymJetsMETSelection_->analyseTransverseMass(event, signalLepton);
	}

	if (qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->setPrescale(prescale);
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsMETSelection_->setPrescale(prescale);

		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->analyse(event);
		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsMETSelection_->analyse(event);
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsMETSelection_->analyseTransverseMass(event, signalLepton);
	}

	if (qcdConversionAsymJetsMETSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(qcdConversionAsymJetsMETSelection_->cleanedJets(event));
		const JetCollection bJets(qcdConversionAsymJetsMETSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		unsigned int prescale(qcdConversionAsymJetsMETSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdConversionAsymJetsMETSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdConversionsElectronAnalyserAsymJetsMET_->setPrescale(prescale);
		metAnalyserqcdConversionAsymJetsMETSelection_->setPrescale(prescale);

		qcdConversionsElectronAnalyserAsymJetsMET_->analyse(event);
		qcdConversionsElectronAnalyserAsymJetsMET_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdConversionAsymJetsMETSelection_->analyse(event);
		metAnalyserqcdConversionAsymJetsMETSelection_->analyseTransverseMass(event, signalLepton);
	}

	if (qcdPFRelIsoAsymJetsMETSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(qcdPFRelIsoAsymJetsMETSelection_->cleanedJets(event));
		const JetCollection bJets(qcdPFRelIsoAsymJetsMETSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		unsigned int prescale(qcdPFRelIsoAsymJetsMETSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoAsymJetsMETSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJetsMET_->setPrescale(prescale);

		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJetsMET_->analyse(event);
		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJetsMET_->analyseElectron(signalElectron, event->weight());
	}

	if (qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->cleanedJets(event));
		const JetCollection bJets(qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->cleanedBJets(event));
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		unsigned int prescale(qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->setPrescale(prescale);

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->analyse(event);
		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->analyseElectron(signalElectron, event->weight());
	}

}

void TTbarPlusMETAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	//signal
	metAnalyserRefSelection_->createHistograms();
	metAnalyserRefAsymJetsMETSelection_->createHistograms();
	metAnalyserRefAsymJetsSelection_->createHistograms();

	electronAnalyserRefSelection_->createHistograms();
	electronAnalyserRefAsymJetsSelection_->createHistograms();
	electronAnalyserRefAsymJetsMETSelection_->createHistograms();

	vertexAnalyserRefSelection_->createHistograms();
	//QCD region
	qcdNonIsoElectronAnalyser_->createHistograms();
	qcdNonIsoNonIsoTriggerElectronAnalyser_->createHistograms();
	metAnalyserqcdNonIsoSelection_->createHistograms();
	metAnalyserqcdNonIsoNonIsoTriggerSelection_->createHistograms();

	qcdConversionsElectronAnalyser_->createHistograms();
	metAnalyserqcdConversionSelection_->createHistograms();

	qcdEPlusjetsPFRelIsoElectronAnalyser_->createHistograms();
	qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->createHistograms();
	//QCD region + asym. jet cuts
	qcdNonIsoElectronAnalyserAsymJets_->createHistograms();
	qcdNonIsoNonIsoTriggerElectronAnalyserAsymJets_->createHistograms();
	metAnalyserqcdNonIsoAsymJetsSelection_->createHistograms();
	metAnalyserqcdNonIsoNonIsoTriggerAsymJetsSelection_->createHistograms();

	qcdConversionsElectronAnalyserAsymJets_->createHistograms();
	metAnalyserqcdConversionAsymJetsSelection_->createHistograms();

	qcdEPlusjetsPFRelIsoElectronAnalyserAsymJets_->createHistograms();
	qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJets_->createHistograms();
	//QCD region + asym. jet cuts
	qcdNonIsoElectronAnalyserAsymJetsMET_->createHistograms();
	qcdNonIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->createHistograms();
	metAnalyserqcdNonIsoAsymJetsMETSelection_->createHistograms();
	metAnalyserqcdNonIsoNonIsoTriggerAsymJetsMETSelection_->createHistograms();

	qcdConversionsElectronAnalyserAsymJetsMET_->createHistograms();
	metAnalyserqcdConversionAsymJetsMETSelection_->createHistograms();

	qcdEPlusjetsPFRelIsoElectronAnalyserAsymJetsMET_->createHistograms();
	qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->createHistograms();

	for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
		binnedElectronAnalysers_.at(index)->createHistograms();
		qcdConversionBinnedElectronAnalysers_.at(index)->createHistograms();
		qcdNonIsoBinnedElectronAnalysers_.at(index)->createHistograms();
	}

}

TTbarPlusMETAnalyser::TTbarPlusMETAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
	BasicAnalyser(histMan, histogramFolder), //
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
			qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_(new QCDNonIsolatedElectronAsymJetsSelection()), //
			qcdConversionAsymJetsSelection_(new QCDConversionsAsymJetsSelection()), //
			qcdPFRelIsoAsymJetsSelection_(new QCDPFRelIsoEPlusAsymJetsSelection()), //
			qcdPFRelIsoNonIsoTriggerAsymJetsSelection_(new QCDPFRelIsoEPlusAsymJetsSelection()), //
			//QCD selections with respect to reference selection + MET + asymmetric jet cuts
			qcdNonIsoElectronAsymJetsMETSelection_(new QCDNonIsolatedElectronAsymJetsMETSelection()), //
			qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_(new QCDNonIsolatedElectronAsymJetsMETSelection()), //
			qcdConversionAsymJetsMETSelection_(new QCDConversionsAsymJetsMETSelection()), //
			qcdPFRelIsoAsymJetsMETSelection_(new QCDPFRelIsoEPlusAsymJetsMET()), //
			qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_(new QCDPFRelIsoEPlusAsymJetsMET()), //
			//analysers
			//signal regions
			metAnalyserRefSelection_(new METAnalyser(histMan, histogramFolder + "/Ref selection/MET")), //
			metAnalyserRefAsymJetsMETSelection_(new METAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets + MET selection/MET")), //
			metAnalyserRefAsymJetsSelection_(
					new METAnalyser(histMan, histogramFolder + "/Ref + AsymJets selection/MET")), //
			electronAnalyserRefSelection_(new ElectronAnalyser(histMan, histogramFolder + "/Ref selection/Electron",
					true)), //
			electronAnalyserRefAsymJetsMETSelection_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets + MET selection/Electron", true)), //
			electronAnalyserRefAsymJetsSelection_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets selection/Electron", true)), //
			vertexAnalyserRefSelection_(new VertexAnalyser(histMan, histogramFolder + "/Ref selection/Vertices")), //
			//QCD region
			metAnalyserqcdNonIsoSelection_(new METAnalyser(histMan, histogramFolder
					+ "/Ref selection/QCD non iso e+jets/MET")), //
			metAnalyserqcdNonIsoNonIsoTriggerSelection_(new METAnalyser(histMan, histogramFolder
					+ "/Ref selection/QCD non iso e+jets, non iso trigger/MET")), //
			qcdNonIsoElectronAnalyser_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref selection/QCD non iso e+jets/Electron")), //
			qcdNonIsoNonIsoTriggerElectronAnalyser_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref selection/QCD non iso e+jets, non iso trigger/Electron", true)), //
			metAnalyserqcdConversionSelection_(new METAnalyser(histMan, histogramFolder
					+ "/Ref selection/QCDConversions/MET")), //
			qcdConversionsElectronAnalyser_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref selection/QCDConversions/Electron", true)), //
			qcdEPlusjetsPFRelIsoElectronAnalyser_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref selection/QCD e+jets PFRelIso/Electron")), //
			qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref selection/QCD e+jets PFRelIso, non iso trigger/Electron", true)), //
			//QCD region with asym. jets
			metAnalyserqcdNonIsoAsymJetsSelection_(new METAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets selection/QCD non iso e+jets/MET")), //
			metAnalyserqcdNonIsoNonIsoTriggerAsymJetsSelection_(new METAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets selection/QCD non iso e+jets, non iso trigger/MET")), //
			qcdNonIsoElectronAnalyserAsymJets_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets selection/QCD non iso e+jets/Electron", true)), //
			qcdNonIsoNonIsoTriggerElectronAnalyserAsymJets_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets selection/QCD non iso e+jets, non iso trigger/Electron", true)), //
			metAnalyserqcdConversionAsymJetsSelection_(new METAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets selection/QCDConversions/MET")), //
			qcdConversionsElectronAnalyserAsymJets_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets selection/QCDConversions/Electron", true)), //
			qcdEPlusjetsPFRelIsoElectronAnalyserAsymJets_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets selection/QCD e+jets PFRelIso/Electron", true)), //
			qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJets_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets selection/QCD e+jets PFRelIso, non iso trigger/Electron", true)), //
			//QCD region with asym. jets + MET
			metAnalyserqcdNonIsoAsymJetsMETSelection_(new METAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets + MET selection/QCD non iso e+jets/MET")), //
			metAnalyserqcdNonIsoNonIsoTriggerAsymJetsMETSelection_(new METAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets + MET selection/QCD non iso e+jets, non iso trigger/MET")), //
			qcdNonIsoElectronAnalyserAsymJetsMET_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets + MET selection/QCD non iso e+jets/Electron", true)), //
			qcdNonIsoNonIsoTriggerElectronAnalyserAsymJetsMET_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets + MET selection/QCD non iso e+jets, non iso trigger/Electron", true)), //
			metAnalyserqcdConversionAsymJetsMETSelection_(new METAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets + MET selection/QCDConversions/MET")), //
			qcdConversionsElectronAnalyserAsymJetsMET_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets + MET selection/QCDConversions/Electron", true)), //
			qcdEPlusjetsPFRelIsoElectronAnalyserAsymJetsMET_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets + MET selection/QCD e+jets PFRelIso/Electron", true)), //
			qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJetsMET_(new ElectronAnalyser(histMan, histogramFolder
					+ "/Ref + AsymJets + MET selection/QCD e+jets PFRelIso, non iso trigger/Electron", true)),//
			metBins_(),//
			binnedElectronAnalysers_(),//
			qcdConversionBinnedElectronAnalysers_(),//
			qcdNonIsoBinnedElectronAnalysers_()

			{
	qcdNonIsoElectronNonIsoTriggerSelection_->useNonIsoTrigger(true);
	qcdPFRelIsoNonIsoTriggerSelection_->useNonIsoTrigger(true);
	qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_->useNonIsoTrigger(true);
	qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->useNonIsoTrigger(true);
	qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_->useNonIsoTrigger(true);
	qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->useNonIsoTrigger(true);
	metBins_.push_back(30.);
	metBins_.push_back(50.);
	metBins_.push_back(75.);
	for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
		string bin = index < metBins_.size() ? boost::lexical_cast<std::string>(metBins_.at(index)) : "inf";
		string previousBin = index == 0 ? "0" : boost::lexical_cast<std::string>(metBins_.at(index - 1));
		string folder = histogramFolder_ + "/Ref selection/Electron_METbin_" + previousBin + "-" + bin;
		string qcdConversionFolder = histogramFolder_ + "/Ref selection/QCDConversions/Electron_METbin_" + previousBin
				+ "-" + bin;
		string qcdNonIsoFolder = histogramFolder_ + "/Ref selection/QCD non iso e+jets/Electron_METbin_" + previousBin
				+ "-" + bin;
		ElectronAnalyserPtr analyser(new ElectronAnalyser(histMan_, folder, true));
		ElectronAnalyserPtr qcdConversionAnalyser(new ElectronAnalyser(histMan_, qcdConversionFolder, true));
		ElectronAnalyserPtr qcdNonIsoAnalyser(new ElectronAnalyser(histMan_, qcdNonIsoFolder, true));
		binnedElectronAnalysers_.push_back(analyser);
		qcdConversionBinnedElectronAnalysers_.push_back(qcdConversionAnalyser);
		qcdNonIsoBinnedElectronAnalysers_.push_back(qcdNonIsoAnalyser);
	}
}

TTbarPlusMETAnalyser::~TTbarPlusMETAnalyser() {
}

} /* namespace BAT */
