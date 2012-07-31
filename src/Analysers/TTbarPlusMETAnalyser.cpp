/*
 * TTbarPlusMETAnalyser.cpp
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Analysers/TTbarPlusMETAnalyser.h"
#include "../../interface/BTagWeight.h"
//#include "../../interface/TopPairEventCandidate.h"
//signal selections
#include "../../interface/Selections/TopPairEplusJetsRefAsymJetsSelection.h"
#include "../../interface/Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../../interface/Selections/TopPairEPlusJetsRefAsymJetsMETSelection.h"
//QCD selections w.r.t signal ref selection
#include "../../interface/Selections/QCDNonIsolatedElectronSelection.h"
#include "../../interface/Selections/QCDConversionsSelection.h"
#include "../../interface/Selections/QCDPFRelIsoEPlusJetsSelection.h"
#include "../../interface/Selections/QCDAntiIDEPlusJetsSelection.h"
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
}

void TTbarPlusMETAnalyser::signalAnalysis(const EventPtr event) {

	if (topEplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(topEplusJetsRefSelection_->cleanedJets(event));
		const JetCollection bJets(topEplusJetsRefSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= 4; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
			if (numberOfBjets > 4)
				bjetWeights.at(4) = 1;
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		const LeptonPointer signalLepton = topEplusJetsRefSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			histMan_->setCurrentHistogramFolder(histogramFolder_);
			histMan_->H1D("N_BJets")->Fill(numberOfBjets, event->weight() * bjetWeight);

			metAnalyserRefSelection_->setScale(bjetWeight);
			electronAnalyserRefSelection_->setScale(bjetWeight);
			vertexAnalyserRefSelection_->setScale(bjetWeight);
			jetAnalyserRefSelection_->setScale(bjetWeight);

			metAnalyserRefSelection_->analyse(event);
			metAnalyserRefSelection_->analyseTransverseMass(event, signalLepton);

			electronAnalyserRefSelection_->analyse(event);
			electronAnalyserRefSelection_->analyseElectron(signalElectron, event->weight());

			vertexAnalyserRefSelection_->analyse(event);
			jetAnalyserRefSelection_->analyse(event);

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				bool isMCOnlyMET = metIndex == METAlgorithm::patType1p2CorrectedPFMetJetResUp
						|| metIndex == METAlgorithm::patType1p2CorrectedPFMetJetResDown
						|| metIndex == METAlgorithm::GenMET;
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
					double upperCut = index < metBins_.size() ? metBins_.at(index) : 999999.;
					double lowerCut = index == 0 ? 0. : metBins_.at(index - 1);
					unsigned int analyserIndex = index + metIndex * (metBins_.size() + 1);
					const METPointer met(event->MET((METAlgorithm::value) metIndex));
					if (met->et() >= lowerCut && met->et() < upperCut) {
						binnedElectronAnalysers_.at(analyserIndex)->setScale(bjetWeight);
						binnedElectronAnalysers_.at(analyserIndex)->analyseElectron(signalElectron, event->weight());
					}
				}
			}
		}
		histMan_->setCurrentBJetBin(bJets.size());
		jetAnalyserRefSelection_noBtagWeights_->analyse(event);
	}

}

void TTbarPlusMETAnalyser::qcdAnalysis(const EventPtr event) {
	//selection with respect to reference selection
	if (qcdNonIsoElectronSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdNonIsoElectronSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNonIsoElectronSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= 4; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
			if (numberOfBjets > 4)
				bjetWeights.at(4) = 1;
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoSelection_->setPrescale(prescale);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdNonIsoElectronAnalyser_->setScale(bjetWeight);
			metAnalyserqcdNonIsoSelection_->setScale(bjetWeight);

			qcdNonIsoElectronAnalyser_->analyse(event);
			qcdNonIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());
			metAnalyserqcdNonIsoSelection_->analyse(event);
			metAnalyserqcdNonIsoSelection_->analyseTransverseMass(event, signalLepton);
			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				bool isMCOnlyMET = metIndex == METAlgorithm::patType1p2CorrectedPFMetJetResUp
						|| metIndex == METAlgorithm::patType1p2CorrectedPFMetJetResDown
						|| metIndex == METAlgorithm::GenMET;
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
					double upperCut = index < metBins_.size() ? metBins_.at(index) : 999999.;
					double lowerCut = index == 0 ? 0. : metBins_.at(index - 1);
					unsigned int analyserIndex = index + metIndex * (metBins_.size() + 1);
					const METPointer met(event->MET((METAlgorithm::value) metIndex));
					if (met->et() >= lowerCut && met->et() < upperCut) {
						qcdNonIsoBinnedElectronAnalysers_.at(analyserIndex)->setScale(bjetWeight);
						qcdNonIsoBinnedElectronAnalysers_.at(analyserIndex)->analyseElectron(signalElectron,
								event->weight());
					}
				}
			}
		}
	}

	if (qcdNonIsoElectronNonIsoTriggerSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdNonIsoElectronNonIsoTriggerSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNonIsoElectronNonIsoTriggerSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= 4; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
			if (numberOfBjets > 4)
				bjetWeights.at(4) = 1;
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronNonIsoTriggerSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronNonIsoTriggerSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoNonIsoTriggerElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoNonIsoTriggerSelection_->setPrescale(prescale);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdNonIsoNonIsoTriggerElectronAnalyser_->setScale(bjetWeight);
			metAnalyserqcdNonIsoNonIsoTriggerSelection_->setScale(bjetWeight);
			qcdNonIsoNonIsoTriggerElectronAnalyser_->analyse(event);
			qcdNonIsoNonIsoTriggerElectronAnalyser_->analyseElectron(signalElectron, event->weight());
			metAnalyserqcdNonIsoNonIsoTriggerSelection_->analyse(event);
			metAnalyserqcdNonIsoNonIsoTriggerSelection_->analyseTransverseMass(event, signalLepton);
		}
	}

	if (qcdConversionSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdConversionSelection_->cleanedJets(event));
		const JetCollection bJets(qcdConversionSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= 4; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
			if (numberOfBjets > 4)
				bjetWeights.at(4) = 1;
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		unsigned int prescale(qcdConversionSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdConversionSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdConversionsElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdConversionSelection_->setPrescale(prescale);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdConversionsElectronAnalyser_->setScale(bjetWeight);
			metAnalyserqcdConversionSelection_->setScale(bjetWeight);

			qcdConversionsElectronAnalyser_->analyse(event);
			qcdConversionsElectronAnalyser_->analyseElectron(signalElectron, event->weight());
			metAnalyserqcdConversionSelection_->analyse(event);
			metAnalyserqcdConversionSelection_->analyseTransverseMass(event, signalLepton);

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				bool isMCOnlyMET = metIndex == METAlgorithm::patType1p2CorrectedPFMetJetResUp
						|| metIndex == METAlgorithm::patType1p2CorrectedPFMetJetResDown
						|| metIndex == METAlgorithm::GenMET;
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
					double upperCut = index < metBins_.size() ? metBins_.at(index) : 999999.;
					double lowerCut = index == 0 ? 0. : metBins_.at(index - 1);
					unsigned int analyserIndex = index + metIndex * (metBins_.size() + 1);
					const METPointer met(event->MET((METAlgorithm::value) metIndex));
					if (met->et() >= lowerCut && met->et() < upperCut) {
						qcdConversionBinnedElectronAnalysers_.at(analyserIndex)->setScale(bjetWeight);
						qcdConversionBinnedElectronAnalysers_.at(analyserIndex)->analyseElectron(signalElectron,
								event->weight() * bjetWeight);
					}
				}
			}
		}
	}

	if (qcdPFRelIsoSelection_->passesSelectionUpToStep(event, TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdPFRelIsoSelection_->cleanedJets(event));
		const JetCollection bJets(qcdPFRelIsoSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= 4; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
			if (numberOfBjets > 4)
				bjetWeights.at(4) = 1;
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		unsigned int prescale(qcdPFRelIsoSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoElectronAnalyser_->setPrescale(prescale);
		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdEPlusjetsPFRelIsoElectronAnalyser_->setScale(bjetWeight);

			qcdEPlusjetsPFRelIsoElectronAnalyser_->analyse(event);
			qcdEPlusjetsPFRelIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				bool isMCOnlyMET = metIndex == METAlgorithm::patType1p2CorrectedPFMetJetResUp
						|| metIndex == METAlgorithm::patType1p2CorrectedPFMetJetResDown
						|| metIndex == METAlgorithm::GenMET;
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
					double upperCut = index < metBins_.size() ? metBins_.at(index) : 999999.;
					double lowerCut = index == 0 ? 0. : metBins_.at(index - 1);
					unsigned int analyserIndex = index + metIndex * (metBins_.size() + 1);
					const METPointer met(event->MET((METAlgorithm::value) metIndex));
					if (met->et() >= lowerCut && met->et() < upperCut) {
						qcdPFRelIsoBinnedElectronAnalysers_.at(analyserIndex)->setScale(bjetWeight);
						qcdPFRelIsoBinnedElectronAnalysers_.at(analyserIndex)->analyseElectron(signalElectron,
								event->weight());
					}
				}
			}
		}
	}

	if (qcdPFRelIsoNonIsoTriggerSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdPFRelIsoNonIsoTriggerSelection_->cleanedJets(event));
		const JetCollection bJets(qcdPFRelIsoNonIsoTriggerSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= 4; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
			if (numberOfBjets > 4)
				bjetWeights.at(4) = 1;
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		unsigned int prescale(qcdPFRelIsoNonIsoTriggerSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoNonIsoTriggerSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->setPrescale(prescale);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->setScale(bjetWeight);
			qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->analyse(event);
			qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->analyseElectron(signalElectron, event->weight());
		}
	}

	if (qcdAntiIDSelection_->passesSelectionUpToStep(event, TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdAntiIDSelection_->cleanedJets(event));
		const JetCollection bJets(qcdAntiIDSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= 4; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
			if (numberOfBjets > 4)
				bjetWeights.at(4) = 1;
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		unsigned int prescale(qcdAntiIDSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdAntiIDSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdAntiIDElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdAntiIDSelection_->setPrescale(prescale);
		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdAntiIDElectronAnalyser_->setScale(bjetWeight);
			metAnalyserqcdAntiIDSelection_->setScale(bjetWeight);

			qcdAntiIDElectronAnalyser_->analyse(event);
			qcdAntiIDElectronAnalyser_->analyseElectron(signalElectron, event->weight());
			metAnalyserqcdAntiIDSelection_->analyse(event);
			metAnalyserqcdAntiIDSelection_->analyseTransverseMass(event, signalLepton);

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				bool isMCOnlyMET = metIndex == METAlgorithm::patType1p2CorrectedPFMetJetResUp
						|| metIndex == METAlgorithm::patType1p2CorrectedPFMetJetResDown
						|| metIndex == METAlgorithm::GenMET;
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
					double upperCut = index < metBins_.size() ? metBins_.at(index) : 999999.;
					double lowerCut = index == 0 ? 0. : metBins_.at(index - 1);
					unsigned int analyserIndex = index + metIndex * (metBins_.size() + 1);
					const METPointer met(event->MET((METAlgorithm::value) metIndex));
					if (met->et() >= lowerCut && met->et() < upperCut) {
						qcdAntiIDBinnedElectronAnalysers_.at(analyserIndex)->setScale(bjetWeight);
						qcdAntiIDBinnedElectronAnalysers_.at(analyserIndex)->analyseElectron(signalElectron,
								event->weight());
					}
				}
			}
		}
	}

	if (qcdNoIsoNoIDSelection_->passesSelectionUpToStep(event, TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdNoIsoNoIDSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNoIsoNoIDSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= 4; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
			if (numberOfBjets > 4)
				bjetWeights.at(4) = 1;
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		unsigned int prescale(qcdNoIsoNoIDSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNoIsoNoIDSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNoIsoNoIDElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdNoIsoNoIDSelection_->setPrescale(prescale);
		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdNoIsoNoIDElectronAnalyser_->setScale(bjetWeight);
			metAnalyserqcdNoIsoNoIDSelection_->setScale(bjetWeight);
			qcdNoIsoNoIDElectronAnalyser_->analyse(event);
			qcdNoIsoNoIDElectronAnalyser_->analyseElectron(signalElectron, event->weight());
			metAnalyserqcdNoIsoNoIDSelection_->analyse(event);
			metAnalyserqcdNoIsoNoIDSelection_->analyseTransverseMass(event, signalLepton);

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				bool isMCOnlyMET = metIndex == METAlgorithm::patType1p2CorrectedPFMetJetResUp
						|| metIndex == METAlgorithm::patType1p2CorrectedPFMetJetResDown
						|| metIndex == METAlgorithm::GenMET;
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
					double upperCut = index < metBins_.size() ? metBins_.at(index) : 999999.;
					double lowerCut = index == 0 ? 0. : metBins_.at(index - 1);
					unsigned int analyserIndex = index + metIndex * (metBins_.size() + 1);
					const METPointer met(event->MET((METAlgorithm::value) metIndex));
					if (met->et() >= lowerCut && met->et() < upperCut) {
						qcdNoIsoNoIDBinnedElectronAnalysers_.at(analyserIndex)->setScale(bjetWeight);
						qcdNoIsoNoIDBinnedElectronAnalysers_.at(analyserIndex)->analyseElectron(signalElectron,
								event->weight());
					}
				}
			}
		}
	}

}

void TTbarPlusMETAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	histMan_->addH1D("N_BJets", "# of b-Jets; # of b-Jet; Events", 11, -0.5, 10.5);
	//signal
	metAnalyserRefSelection_->createHistograms();
	electronAnalyserRefSelection_->createHistograms();

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

	metAnalyserqcdAntiIDSelection_->createHistograms();
	qcdAntiIDElectronAnalyser_->createHistograms();
	metAnalyserqcdNoIsoNoIDSelection_->createHistograms();
	qcdNoIsoNoIDElectronAnalyser_->createHistograms();

	for (unsigned int index = 0; index < binnedElectronAnalysers_.size(); ++index) {
		binnedElectronAnalysers_.at(index)->createHistograms();
		qcdConversionBinnedElectronAnalysers_.at(index)->createHistograms();
		qcdNonIsoBinnedElectronAnalysers_.at(index)->createHistograms();
		qcdPFRelIsoBinnedElectronAnalysers_.at(index)->createHistograms();
		qcdAntiIDBinnedElectronAnalysers_.at(index)->createHistograms();
		qcdNoIsoNoIDBinnedElectronAnalysers_.at(index)->createHistograms();
	}

	jetAnalyserRefSelection_->createHistograms();
	jetAnalyserRefSelection_noBtagWeights_->createHistograms();

}

TTbarPlusMETAnalyser::TTbarPlusMETAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		topEplusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection()), //
		//QCD selections with respect to reference selection
		qcdNonIsoElectronSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdNonIsoElectronNonIsoTriggerSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdConversionSelection_(new QCDConversionsSelection()), //
		qcdPFRelIsoSelection_(new QCDPFRelIsoEPlusJetsSelection()), //
		qcdPFRelIsoNonIsoTriggerSelection_(new QCDPFRelIsoEPlusJetsSelection()), //
		qcdAntiIDSelection_(new QCDAntiIDEPlusJetsSelection()), //
		qcdNoIsoNoIDSelection_(new QCDNoIsoNoIDSelection()), //
		//analysers
		//signal regions
		metAnalyserRefSelection_(new METAnalyser(histMan, histogramFolder + "/Ref selection/MET")), //
		electronAnalyserRefSelection_(new ElectronAnalyser(histMan, histogramFolder + "/Ref selection/Electron", true)), //
		vertexAnalyserRefSelection_(new VertexAnalyser(histMan, histogramFolder + "/Ref selection/Vertices")), //
		//QCD region
		metAnalyserqcdNonIsoSelection_(
				new METAnalyser(histMan, histogramFolder + "/Ref selection/QCD non iso e+jets/MET")), //
		metAnalyserqcdNonIsoNonIsoTriggerSelection_(
				new METAnalyser(histMan, histogramFolder + "/Ref selection/QCD non iso e+jets, non iso trigger/MET")), //
		qcdNonIsoElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/Ref selection/QCD non iso e+jets/Electron")), //
		qcdNonIsoNonIsoTriggerElectronAnalyser_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/Ref selection/QCD non iso e+jets, non iso trigger/Electron", true)), //
		metAnalyserqcdConversionSelection_(
				new METAnalyser(histMan, histogramFolder + "/Ref selection/QCDConversions/MET")), //
		qcdConversionsElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/Ref selection/QCDConversions/Electron", true)), //
		qcdEPlusjetsPFRelIsoElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/Ref selection/QCD e+jets PFRelIso/Electron")), //
		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/Ref selection/QCD e+jets PFRelIso, non iso trigger/Electron", true)), //
		metAnalyserqcdAntiIDSelection_(new METAnalyser(histMan, histogramFolder + "/Ref selection/QCDAntiID/MET")), //
		qcdAntiIDElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/Ref selection/QCDAntiID/Electron", true)), //
		metAnalyserqcdNoIsoNoIDSelection_(
				new METAnalyser(histMan, histogramFolder + "/Ref selection/QCDNoIsoNoID/MET")), //
		qcdNoIsoNoIDElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/Ref selection/QCDNoIsoNoID/Electron", true)), //
		metBins_(), //
		binnedElectronAnalysers_(), //
		qcdConversionBinnedElectronAnalysers_(), //
		qcdNonIsoBinnedElectronAnalysers_(), //
		qcdPFRelIsoBinnedElectronAnalysers_(), //
		qcdAntiIDBinnedElectronAnalysers_(), //
		qcdNoIsoNoIDBinnedElectronAnalysers_(), //
		jetAnalyserRefSelection_(new JetAnalyser(histMan, histogramFolder + "/Ref selection/Jets")), //
		jetAnalyserRefSelection_noBtagWeights_(
				new JetAnalyser(histMan, histogramFolder + "/Ref selection/Jets_noBtagWeights")) {
	qcdNonIsoElectronNonIsoTriggerSelection_->useNonIsoTrigger(true);
	qcdPFRelIsoNonIsoTriggerSelection_->useNonIsoTrigger(true);
	metBins_.push_back(25.);
	metBins_.push_back(45.);
	metBins_.push_back(70.);
	metBins_.push_back(100.);

	//for all MET types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		string metPrefix = METAlgorithm::names.at(metIndex);
		for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
			string bin = index < metBins_.size() ? boost::lexical_cast<std::string>(metBins_.at(index)) : "inf";
			string previousBin = index == 0 ? "0" : boost::lexical_cast<std::string>(metBins_.at(index - 1));
			string folder = histogramFolder_ + "/Ref selection/BinnedMETAnalysis/Electron_" + metPrefix + "_bin_"
					+ previousBin + "-" + bin;
			string qcdConversionFolder = histogramFolder_ + "/Ref selection/BinnedMETAnalysis/QCDConversions/Electron_"
					+ metPrefix + "_bin_" + previousBin + "-" + bin;
			string qcdNonIsoFolder = histogramFolder_ + "/Ref selection/BinnedMETAnalysis/QCD non iso e+jets/Electron_"
					+ metPrefix + "_bin_" + previousBin + "-" + bin;
			string qcdPFRelIsoFolder = histogramFolder_
					+ "/Ref selection/BinnedMETAnalysis/QCD e+jets PFRelIso/Electron_" + metPrefix + "_bin_"
					+ previousBin + "-" + bin;
			string qcdAntiIDFolder = histogramFolder_ + "/Ref selection/BinnedMETAnalysis/QCDAntiID/Electron_"
					+ metPrefix + "_bin_" + previousBin + "-" + bin;
			string qcdNoIsoNoIDFolder = histogramFolder_ + "/Ref selection/BinnedMETAnalysis/QCDNoIsoNoID/Electron_"
					+ metPrefix + "_bin_" + previousBin + "-" + bin;
			ElectronAnalyserPtr analyser(new ElectronAnalyser(histMan_, folder, true));
			ElectronAnalyserPtr qcdConversionAnalyser(new ElectronAnalyser(histMan_, qcdConversionFolder, true));
			ElectronAnalyserPtr qcdNonIsoAnalyser(new ElectronAnalyser(histMan_, qcdNonIsoFolder, true));
			ElectronAnalyserPtr qcdPFRelIsoAnalyser(new ElectronAnalyser(histMan_, qcdPFRelIsoFolder, true));
			ElectronAnalyserPtr qcdAntiIDAnalyser(new ElectronAnalyser(histMan_, qcdAntiIDFolder, true));
			ElectronAnalyserPtr qcdNoIsoNoIDAnalyser(new ElectronAnalyser(histMan_, qcdNoIsoNoIDFolder, true));
			analyser->useTTbarPlusMETSetup(true);
			qcdConversionAnalyser->useTTbarPlusMETSetup(true);
			qcdNonIsoAnalyser->useTTbarPlusMETSetup(true);
			qcdPFRelIsoAnalyser->useTTbarPlusMETSetup(true);
			qcdAntiIDAnalyser->useTTbarPlusMETSetup(true);
			qcdNoIsoNoIDAnalyser->useTTbarPlusMETSetup(true);

			binnedElectronAnalysers_.push_back(analyser);
			qcdConversionBinnedElectronAnalysers_.push_back(qcdConversionAnalyser);
			qcdNonIsoBinnedElectronAnalysers_.push_back(qcdNonIsoAnalyser);
			qcdPFRelIsoBinnedElectronAnalysers_.push_back(qcdPFRelIsoAnalyser);
			qcdAntiIDBinnedElectronAnalysers_.push_back(qcdAntiIDAnalyser);
			qcdNoIsoNoIDBinnedElectronAnalysers_.push_back(qcdNoIsoNoIDAnalyser);
		}
	}
}

TTbarPlusMETAnalyser::~TTbarPlusMETAnalyser() {
}

} /* namespace BAT */
