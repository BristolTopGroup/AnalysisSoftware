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
#include "../../interface/Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../../interface/Selections/TopPairMuPlusJetsReferenceSelection.h"
//QCD selections w.r.t signal ref selection
#include "../../interface/Selections/QCDNonIsolatedElectronSelection.h"
#include "../../interface/Selections/QCDConversionsSelection.h"
#include "../../interface/Selections/QCDPFRelIsoEPlusJetsSelection.h"
#include "../../interface/Selections/QCDAntiIDEPlusJetsSelection.h"
//muons
#include "../../interface/Selections/QCDPFRelIsoMuPlusJetsSelection.h"
#include "../../interface/Selections/QCDNonIsolatedMuonSelection.h"
#include "../../interface/Selections/NoIsolationMuonSelection.h"

namespace BAT {

void TTbarPlusMETAnalyser::analyse(const EventPtr event) {
	ePlusJetsSignalAnalysis(event);
	ePlusJetsQcdAnalysis(event);
	muPlusJetsSignalAnalysis(event);
	muPlusJetsQcdAnalysis(event);
}

void TTbarPlusMETAnalyser::ePlusJetsSignalAnalysis(const EventPtr event) {

	if (topEplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(topEplusJetsRefSelection_->cleanedJets(event));
		const JetCollection bJets(topEplusJetsRefSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1.);
				else
					bjetWeights.push_back(0);
			}
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		const LeptonPointer signalLepton = topEplusJetsRefSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/Ref selection");
			histMan_->H1D("BTagWeights")->Fill(bjetWeight);
			histMan_->H1D("N_BJets_reweighted")->Fill(weightIndex, event->weight() * bjetWeight);

			metAnalyserEPlusJetsRefSelection_->setScale(bjetWeight);
			electronAnalyserRefSelection_->setScale(bjetWeight);
			vertexAnalyserEPlusJetsRefSelection_->setScale(bjetWeight);
			jetAnalyserEPlusJetsRefSelection_->setScale(bjetWeight);

			metAnalyserEPlusJetsRefSelection_->analyse(event, signalLepton);
//			metAnalyserEPlusJetsRefSelection_->analyseTransverseMass(event, signalLepton);

			electronAnalyserRefSelection_->analyse(event);
			electronAnalyserRefSelection_->analyseElectron(signalElectron, event->weight());

			vertexAnalyserEPlusJetsRefSelection_->analyse(event);
			jetAnalyserEPlusJetsRefSelection_->analyse(event);

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
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

			//bbar analysis part
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/Ref selection");
			if (numberOfBjets > 1) {
				unsigned int numberOfCombinations(1);
				for (unsigned int i = 2; i < numberOfBjets; ++i)
					numberOfCombinations += i;
				for (unsigned int i = 0; i < numberOfBjets; ++i) {
					for (unsigned int j = i + 1; j < numberOfBjets; ++j) {
						double invMass = bJets.at(i)->invariantMass(bJets.at(j));
						//conserve event weight by normalising the number of combinations
						double weight = event->weight() * bjetWeight / numberOfCombinations;
						histMan_->H1D_BJetBinned("bjet_invariant_mass")->Fill(invMass, weight);
					}
				}
			}
		}
		histMan_->setCurrentBJetBin(bJets.size());
		histMan_->H1D("N_BJets")->Fill(numberOfBjets, event->weight());
//		jetAnalyserEPlusJetsRefSelection_noBtagWeights_->analyse(event);
	}

}

void TTbarPlusMETAnalyser::ePlusJetsQcdAnalysis(const EventPtr event) {
	//selection with respect to reference selection
	if (qcdNonIsoElectronSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdNonIsoElectronSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNonIsoElectronSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoElectronSelection_->setPrescale(prescale);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdNonIsoElectronAnalyser_->setScale(bjetWeight);
			metAnalyserqcdNonIsoElectronSelection_->setScale(bjetWeight);

			qcdNonIsoElectronAnalyser_->analyse(event);
			qcdNonIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());
			metAnalyserqcdNonIsoElectronSelection_->analyse(event, signalLepton);
//			metAnalyserqcdNonIsoElectronSelection_->analyseTransverseMass(event, signalLepton);
			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
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
			//bbar analysis part
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/QCD non iso e+jets");
			if (numberOfBjets > 1) {
				unsigned int numberOfCombinations(1);
				for (unsigned int i = 2; i < numberOfBjets; ++i)
					numberOfCombinations += i;
				for (unsigned int i = 0; i < numberOfBjets; ++i) {
					for (unsigned int j = i + 1; j < numberOfBjets; ++j) {
						double invMass = bJets.at(i)->invariantMass(bJets.at(j));
						//conserve event weight by normalising the number of combinations
						double weight = event->weight() * bjetWeight / numberOfCombinations;
						histMan_->H1D_BJetBinned("bjet_invariant_mass")->Fill(invMass, weight);
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
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronNonIsoTriggerSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronNonIsoTriggerSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoNonIsoTriggerElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoElectronNonIsoTriggerSelection_->setPrescale(prescale);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdNonIsoNonIsoTriggerElectronAnalyser_->setScale(bjetWeight);
			metAnalyserqcdNonIsoElectronNonIsoTriggerSelection_->setScale(bjetWeight);
			qcdNonIsoNonIsoTriggerElectronAnalyser_->analyse(event);
			qcdNonIsoNonIsoTriggerElectronAnalyser_->analyseElectron(signalElectron, event->weight());
			metAnalyserqcdNonIsoElectronNonIsoTriggerSelection_->analyse(event, signalLepton);
//			metAnalyserqcdNonIsoElectronNonIsoTriggerSelection_->analyseTransverseMass(event, signalLepton);
		}
	}

	if (qcdConversionSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdConversionSelection_->cleanedJets(event));
		const JetCollection bJets(qcdConversionSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
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
			metAnalyserqcdConversionSelection_->analyse(event, signalLepton);
//			metAnalyserqcdConversionSelection_->analyseTransverseMass(event, signalLepton);

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
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
			//bbar analysis part
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/QCDConversions");
			if (numberOfBjets > 1) {
				unsigned int numberOfCombinations(1);
				for (unsigned int i = 2; i < numberOfBjets; ++i)
					numberOfCombinations += i;
				for (unsigned int i = 0; i < numberOfBjets; ++i) {
					for (unsigned int j = i + 1; j < numberOfBjets; ++j) {
						double invMass = bJets.at(i)->invariantMass(bJets.at(j));
						//conserve event weight by normalising the number of combinations
						double weight = event->weight() * bjetWeight / numberOfCombinations;
						histMan_->H1D_BJetBinned("bjet_invariant_mass")->Fill(invMass, weight);
					}
				}
			}
		}
	}

	if (qcdPFRelIsoEPlusJetsSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdPFRelIsoEPlusJetsSelection_->cleanedJets(event));
		const JetCollection bJets(qcdPFRelIsoEPlusJetsSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		unsigned int prescale(qcdPFRelIsoEPlusJetsSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoEPlusJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoElectronAnalyser_->setPrescale(prescale);
		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdEPlusjetsPFRelIsoElectronAnalyser_->setScale(bjetWeight);

			qcdEPlusjetsPFRelIsoElectronAnalyser_->analyse(event);
			qcdEPlusjetsPFRelIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
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

	if (qcdPFRelIsoEPlusNonIsoTriggerSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcdPFRelIsoEPlusNonIsoTriggerSelection_->cleanedJets(event));
		const JetCollection bJets(qcdPFRelIsoEPlusNonIsoTriggerSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		unsigned int prescale(qcdPFRelIsoEPlusNonIsoTriggerSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoEPlusNonIsoTriggerSelection_->signalLepton(event);
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
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
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
			metAnalyserqcdAntiIDSelection_->analyse(event, signalLepton);
//			metAnalyserqcdAntiIDSelection_->analyseTransverseMass(event, signalLepton);

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
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
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
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
			metAnalyserqcdNoIsoNoIDSelection_->analyse(event, signalLepton);
//			metAnalyserqcdNoIsoNoIDSelection_->analyseTransverseMass(event, signalLepton);

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
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

void TTbarPlusMETAnalyser::muPlusJetsSignalAnalysis(const EventPtr event) {

	if (topMuplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(topMuplusJetsRefSelection_->cleanedJets(event));
		const JetCollection bJets(topMuplusJetsRefSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		const LeptonPointer signalLepton = topMuplusJetsRefSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(false);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/Ref selection");
			histMan_->H1D("BTagWeights")->Fill(bjetWeight);
			histMan_->H1D("N_BJets_reweighted")->Fill(weightIndex, event->weight() * bjetWeight * efficiencyCorrection);

			metAnalyserMuPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
			muonAnalyserRefSelection_->setScale(bjetWeight * efficiencyCorrection);
			vertexAnalyserMuPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
			jetAnalyserMuPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);

			metAnalyserMuPlusJetsRefSelection_->analyse(event, signalLepton);
//			metAnalyserMuPlusJetsRefSelection_->analyseTransverseMass(event, signalLepton);

			muonAnalyserRefSelection_->analyse(event);
			muonAnalyserRefSelection_->analyseMuon(signalMuon, event->weight());

			vertexAnalyserMuPlusJetsRefSelection_->analyse(event);
			jetAnalyserMuPlusJetsRefSelection_->analyse(event);

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
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
						binnedMuonAnalysers_.at(analyserIndex)->setScale(bjetWeight * efficiencyCorrection);
						binnedMuonAnalysers_.at(analyserIndex)->analyseMuon(signalMuon, event->weight());
					}
				}
			}

			//bbar analysis part
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/Ref selection");
			if (numberOfBjets > 1) {
				unsigned int numberOfCombinations(1);
				for (unsigned int i = 2; i < numberOfBjets; ++i)
					numberOfCombinations += i;
				for (unsigned int i = 0; i < numberOfBjets; ++i) {
					for (unsigned int j = i + 1; j < numberOfBjets; ++j) {
						double invMass = bJets.at(i)->invariantMass(bJets.at(j));
						//conserve event weight by normalising the number of combinations
						double weight = event->weight() * bjetWeight * efficiencyCorrection / numberOfCombinations;
						histMan_->H1D_BJetBinned("bjet_invariant_mass")->Fill(invMass, weight);
					}
				}
			}
		}
		histMan_->setCurrentBJetBin(bJets.size());
//		jetAnalyserMuPlusJetsRefSelection_noBtagWeights_->analyse(event);
		histMan_->H1D("N_BJets")->Fill(numberOfBjets, event->weight() * efficiencyCorrection);
	}

}

void TTbarPlusMETAnalyser::muPlusJetsQcdAnalysis(const EventPtr event) {
	//selection with respect to reference selection
	if (qcdNonIsoMuonSelection_->passesSelectionUpToStep(event,
			TTbarMuPlusJetsReferenceSelection::AtLeastThreeGoodJets)) {
		const JetCollection jets(qcdNonIsoMuonSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNonIsoMuonSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoMuonSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoMuonSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(true);

		qcdNonIsoMuonAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoMuonSelection_->setPrescale(prescale);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdNonIsoMuonAnalyser_->setScale(bjetWeight * efficiencyCorrection);
			metAnalyserqcdNonIsoMuonSelection_->setScale(bjetWeight * efficiencyCorrection);

			qcdNonIsoMuonAnalyser_->analyse(event);
			qcdNonIsoMuonAnalyser_->analyseMuon(signalMuon, event->weight());
			metAnalyserqcdNonIsoMuonSelection_->analyse(event, signalLepton);
//			metAnalyserqcdNonIsoMuonSelection_->analyseTransverseMass(event, signalLepton);
			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
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
						qcdNonIsoBinnedMuonAnalysers_.at(analyserIndex)->setScale(bjetWeight * efficiencyCorrection);
						qcdNonIsoBinnedMuonAnalysers_.at(analyserIndex)->analyseMuon(signalMuon, event->weight());
					}
				}
			}
			//bbar analysis part
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets");
			if (numberOfBjets > 1) {
				unsigned int numberOfCombinations(1);
				for (unsigned int i = 2; i < numberOfBjets; ++i)
					numberOfCombinations += i;
				for (unsigned int i = 0; i < numberOfBjets; ++i) {
					for (unsigned int j = i + 1; j < numberOfBjets; ++j) {
						double invMass = bJets.at(i)->invariantMass(bJets.at(j));
						//conserve event weight by normalising the number of combinations
						double weight = event->weight() * bjetWeight * efficiencyCorrection / numberOfCombinations;
						histMan_->H1D_BJetBinned("bjet_invariant_mass")->Fill(invMass, weight);
					}
				}
			}
		}
	}

	if (qcdPFRelIsoMuPlusJetsSelection_->passesSelectionUpToStep(event,
			TTbarMuPlusJetsReferenceSelection::AtLeastThreeGoodJets)) {
		const JetCollection jets(qcdPFRelIsoMuPlusJetsSelection_->cleanedJets(event));
		const JetCollection bJets(qcdPFRelIsoMuPlusJetsSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		unsigned int prescale(qcdPFRelIsoMuPlusJetsSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoMuPlusJetsSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(true);

		histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/ge3 jet selection");
		histMan_->H1D("Muon_Eta")->Fill(signalMuon->eta(), event->weight());
		histMan_->H1D("Muon_AbsEta")->Fill(fabs(signalMuon->eta()), event->weight());

		qcdMuPlusjetsPFRelIsoMuonAnalyser_->setPrescale(prescale);
		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdMuPlusjetsPFRelIsoMuonAnalyser_->setScale(bjetWeight * efficiencyCorrection);


			qcdMuPlusjetsPFRelIsoMuonAnalyser_->analyse(event);
			qcdMuPlusjetsPFRelIsoMuonAnalyser_->analyseMuon(signalMuon, event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
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
						qcdPFRelIsoBinnedMuonAnalysers_.at(analyserIndex)->setScale(bjetWeight * efficiencyCorrection);
						qcdPFRelIsoBinnedMuonAnalysers_.at(analyserIndex)->analyseMuon(signalMuon, event->weight());
					}
				}
			}
		}
	}

	if (qcdNoIsolationMuonSelection_->passesSelectionUpToStep(event,
			TTbarMuPlusJetsReferenceSelection::AtLeastThreeGoodJets)) {
		const JetCollection jets(qcdNoIsolationMuonSelection_->cleanedJets(event));
		const JetCollection bJets(qcdNoIsolationMuonSelection_->cleanedBJets(event));
		unsigned int numberOfBjets(bJets.size());
		vector<double> bjetWeights;
		if (event->isRealData()) {
			for (unsigned int index = 0; index <= numberOfBjets; ++index) {
				if (index == numberOfBjets)
					bjetWeights.push_back(1);
				else
					bjetWeights.push_back(0);
			}
		} else
			bjetWeights = BjetWeights(jets, numberOfBjets);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		unsigned int prescale(qcdNoIsolationMuonSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNoIsolationMuonSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(true);

		qcdNoIsolationMuonAnalyser_->setPrescale(prescale);
		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdNoIsolationMuonAnalyser_->setScale(bjetWeight * efficiencyCorrection);

			qcdNoIsolationMuonAnalyser_->analyse(event);
			qcdNoIsolationMuonAnalyser_->analyseMuon(signalMuon, event->weight());

		}
	}

}

void TTbarPlusMETAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/Ref selection");
	histMan_->addH1D("N_BJets", "# of b-Jets; # of b-Jet; Events", 11, -0.5, 10.5);
	histMan_->addH1D("N_BJets_reweighted", "# of b-Jets; # of b-Jet; Events", 11, -0.5, 10.5);
	histMan_->addH1D_BJetBinned("bjet_invariant_mass", "Invariant mass of 2 b-jets; m(b-jet, b-jet); Events", 5000, 0,
			5000);
	histMan_->addH1D("BTagWeights", "BTag weights; BTag weight; Events", 300, -1, 2);
	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/QCD non iso e+jets");
	histMan_->addH1D_BJetBinned("bjet_invariant_mass", "Invariant mass of 2 b-jets; m(b-jet, b-jet); Events", 5000, 0,
			5000);
	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/QCDConversions");
	histMan_->addH1D_BJetBinned("bjet_invariant_mass", "Invariant mass of 2 b-jets; m(b-jet, b-jet); Events", 5000, 0,
			5000);

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/Ref selection");
	histMan_->addH1D("N_BJets", "# of b-Jets; # of b-Jet; Events", 11, -0.5, 10.5);
	histMan_->addH1D("N_BJets_reweighted", "# of b-Jets; # of b-Jet; Events", 11, -0.5, 10.5);
	histMan_->addH1D_BJetBinned("bjet_invariant_mass", "Invariant mass of 2 b-jets; m(b-jet, b-jet); Events", 5000, 0,
			5000);
	histMan_->addH1D("BTagWeights", "BTag weights; BTag weight; Events", 300, -1, 2);
	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets");
	histMan_->addH1D_BJetBinned("bjet_invariant_mass", "Invariant mass of 2 b-jets; m(b-jet, b-jet); Events", 5000, 0,
			5000);
	//histos of central qcd estimation
	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/ge3 jet selection");
	histMan_->addH1D("Muon_Eta", "Muon #eta; #eta(#mu); Events/(0.02)", 300, -3, 3);
	histMan_->addH1D("Muon_AbsEta", "Muon |#eta|; |#eta(#mu)|; Events/(0.01)", 300, 0, 3);

	//signal
	metAnalyserEPlusJetsRefSelection_->createHistograms();
	electronAnalyserRefSelection_->createHistograms();
	metAnalyserMuPlusJetsRefSelection_->createHistograms();
	muonAnalyserRefSelection_->createHistograms();

	vertexAnalyserEPlusJetsRefSelection_->createHistograms();
	vertexAnalyserMuPlusJetsRefSelection_->createHistograms();
	//QCD region
	qcdNonIsoElectronAnalyser_->createHistograms();
	qcdNonIsoNonIsoTriggerElectronAnalyser_->createHistograms();
	metAnalyserqcdNonIsoElectronSelection_->createHistograms();
	metAnalyserqcdNonIsoElectronNonIsoTriggerSelection_->createHistograms();
	qcdNonIsoMuonAnalyser_->createHistograms();
	metAnalyserqcdNonIsoMuonSelection_->createHistograms();

	qcdConversionsElectronAnalyser_->createHistograms();
	metAnalyserqcdConversionSelection_->createHistograms();

	qcdEPlusjetsPFRelIsoElectronAnalyser_->createHistograms();
	qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->createHistograms();
	qcdMuPlusjetsPFRelIsoMuonAnalyser_->createHistograms();
	qcdNoIsolationMuonAnalyser_->createHistograms();

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
	for (unsigned int index = 0; index < binnedMuonAnalysers_.size(); ++index) {
		binnedMuonAnalysers_.at(index)->createHistograms();
		qcdNonIsoBinnedMuonAnalysers_.at(index)->createHistograms();
		qcdPFRelIsoBinnedMuonAnalysers_.at(index)->createHistograms();
	}

	jetAnalyserEPlusJetsRefSelection_->createHistograms();
	jetAnalyserMuPlusJetsRefSelection_->createHistograms();
//	jetAnalyserEPlusJetsRefSelection_noBtagWeights_->createHistograms();
//	jetAnalyserMuPlusJetsRefSelection_noBtagWeights_->createHistograms();

}

TTbarPlusMETAnalyser::TTbarPlusMETAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		topEplusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection()), //
		topMuplusJetsRefSelection_(new TopPairMuPlusJetsReferenceSelection()), //
		//QCD selections with respect to reference selection
		qcdNonIsoElectronSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdNonIsoElectronNonIsoTriggerSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdNonIsoMuonSelection_(new QCDNonIsolatedMuonSelection()), //
		qcdConversionSelection_(new QCDConversionsSelection()), //
		qcdPFRelIsoEPlusJetsSelection_(new QCDPFRelIsoEPlusJetsSelection()), //
		qcdPFRelIsoEPlusNonIsoTriggerSelection_(new QCDPFRelIsoEPlusJetsSelection()), //
		qcdPFRelIsoMuPlusJetsSelection_(new QCDPFRelIsoMuPlusJetsSelection()), //
		qcdNoIsolationMuonSelection_(new NoIsolationMuonSelection()), //
		qcdAntiIDSelection_(new QCDAntiIDEPlusJetsSelection()), //
		qcdNoIsoNoIDSelection_(new QCDNoIsoNoIDSelection()), //
		//analysers
		//signal regions
		metAnalyserEPlusJetsRefSelection_(new METAnalyser(histMan, histogramFolder + "/EPlusJets/Ref selection/MET")), //
		metAnalyserMuPlusJetsRefSelection_(new METAnalyser(histMan, histogramFolder + "/MuPlusJets/Ref selection/MET")), //
		electronAnalyserRefSelection_(
				new ElectronAnalyser(histMan, histogramFolder + "/EPlusJets/Ref selection/Electron", true)), //
		muonAnalyserRefSelection_(new MuonAnalyser(histMan, histogramFolder + "/MuPlusJets/Ref selection/Muon", true)), //
		vertexAnalyserEPlusJetsRefSelection_(
				new VertexAnalyser(histMan, histogramFolder + "/EPlusJets/Ref selection/Vertices")), //
		vertexAnalyserMuPlusJetsRefSelection_(
				new VertexAnalyser(histMan, histogramFolder + "/MuPlusJets/Ref selection/Vertices")), //
		//QCD region
		metAnalyserqcdNonIsoElectronSelection_(
				new METAnalyser(histMan, histogramFolder + "/EPlusJets/QCD non iso e+jets/MET")), //
		metAnalyserqcdNonIsoElectronNonIsoTriggerSelection_(
				new METAnalyser(histMan, histogramFolder + "/EPlusJets/QCD non iso e+jets, non iso trigger/MET")), //
		metAnalyserqcdNonIsoMuonSelection_(
				new METAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets/MET")), //
		qcdNonIsoElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/EPlusJets/QCD non iso e+jets/Electron", true)), //
		qcdNonIsoNonIsoTriggerElectronAnalyser_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/EPlusJets/QCD non iso e+jets, non iso trigger/Electron", true)), //
		qcdNonIsoMuonAnalyser_(
				new MuonAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets/Muon", true)), //
		metAnalyserqcdConversionSelection_(new METAnalyser(histMan, histogramFolder + "/EPlusJets/QCDConversions/MET")), //
		qcdConversionsElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/EPlusJets/QCDConversions/Electron", true)), //
		qcdEPlusjetsPFRelIsoElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/EPlusJets/QCD e+jets PFRelIso/Electron", true)), //
		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/EPlusJets/QCD e+jets PFRelIso, non iso trigger/Electron", true)), //
		qcdMuPlusjetsPFRelIsoMuonAnalyser_(
				new MuonAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD mu+jets PFRelIso/Muon", true)), //
		qcdNoIsolationMuonAnalyser_(
				new MuonAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD No Iso/Muon", true)), //
		metAnalyserqcdAntiIDSelection_(new METAnalyser(histMan, histogramFolder + "/EPlusJets/QCDAntiID/MET")), //
		qcdAntiIDElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/EPlusJets/QCDAntiID/Electron", true)), //
		metAnalyserqcdNoIsoNoIDSelection_(new METAnalyser(histMan, histogramFolder + "/EPlusJets/QCDNoIsoNoID/MET")), //
		qcdNoIsoNoIDElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/EPlusJets/QCDNoIsoNoID/Electron", true)), //
		metBins_(), //
		binnedElectronAnalysers_(), //
		binnedMuonAnalysers_(), //
		qcdConversionBinnedElectronAnalysers_(), //
		qcdNonIsoBinnedElectronAnalysers_(), //
		qcdPFRelIsoBinnedElectronAnalysers_(), //
		qcdAntiIDBinnedElectronAnalysers_(), //
		qcdNoIsoNoIDBinnedElectronAnalysers_(), //
		qcdNonIsoBinnedMuonAnalysers_(), //
		qcdPFRelIsoBinnedMuonAnalysers_(), //
//		jetAnalyserEPlusJetsRefSelection_noBtagWeights_(
//				new JetAnalyser(histMan, histogramFolder + "/EPlusJets/Ref selection/Jets_noBtagWeights")), //
//		jetAnalyserMuPlusJetsRefSelection_noBtagWeights_(
//				new JetAnalyser(histMan, histogramFolder + "/MuPlusJets/Ref selection/Jets_noBtagWeights")),//
		jetAnalyserEPlusJetsRefSelection_(new JetAnalyser(histMan, histogramFolder + "/EPlusJets/Ref selection/Jets")), //
		jetAnalyserMuPlusJetsRefSelection_(new JetAnalyser(histMan, histogramFolder + "/MuPlusJets/Ref selection/Jets")) {
	qcdNonIsoElectronNonIsoTriggerSelection_->useNonIsoTrigger(true);
	qcdPFRelIsoEPlusNonIsoTriggerSelection_->useNonIsoTrigger(true);
	metBins_.push_back(25.);
	metBins_.push_back(45.);
	metBins_.push_back(70.);
	metBins_.push_back(100.);

	//for all MET types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			continue;
		string metPrefix = METAlgorithm::names.at(metIndex);
		for (unsigned int index = 0; index < metBins_.size() + 1; ++index) {
			string bin = index < metBins_.size() ? boost::lexical_cast<std::string>(metBins_.at(index)) : "inf";
			string previousBin = index == 0 ? "0" : boost::lexical_cast<std::string>(metBins_.at(index - 1));
			string folder = histogramFolder_ + "/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_" + metPrefix
					+ "_bin_" + previousBin + "-" + bin;
			string folder_muons = histogramFolder_ + "/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_" + metPrefix
					+ "_bin_" + previousBin + "-" + bin;
			string qcdConversionFolder = histogramFolder_ + "/EPlusJets/QCDConversions/BinnedMETAnalysis/Electron_"
					+ metPrefix + "_bin_" + previousBin + "-" + bin;
			string qcdNonIsoFolder = histogramFolder_ + "/EPlusJets/QCD non iso e+jets/BinnedMETAnalysis/Electron_"
					+ metPrefix + "_bin_" + previousBin + "-" + bin;
			string qcdNonIsoFolder_muons = histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets/BinnedMETAnalysis/Muon_"
					+ metPrefix + "_bin_" + previousBin + "-" + bin;
			string qcdPFRelIsoFolder = histogramFolder_ + "/EPlusJets/QCD e+jets PFRelIso/BinnedMETAnalysis/Electron_"
					+ metPrefix + "_bin_" + previousBin + "-" + bin;
			string qcdPFRelIsoFolder_muons = histogramFolder_
					+ "/MuPlusJets/QCD mu+jets PFRelIso/BinnedMETAnalysis/Muon_" + metPrefix + "_bin_" + previousBin
					+ "-" + bin;
			string qcdAntiIDFolder = histogramFolder_ + "/EPlusJets/QCDAntiID/BinnedMETAnalysis/Electron_" + metPrefix
					+ "_bin_" + previousBin + "-" + bin;
			string qcdNoIsoNoIDFolder = histogramFolder_ + "/EPlusJets/QCDNoIsoNoID/BinnedMETAnalysis/Electron_"
					+ metPrefix + "_bin_" + previousBin + "-" + bin;
			ElectronAnalyserPtr analyser(new ElectronAnalyser(histMan_, folder, true));
			MuonAnalyserPtr analyser_muon(new MuonAnalyser(histMan_, folder_muons, true));
			ElectronAnalyserPtr qcdConversionAnalyser(new ElectronAnalyser(histMan_, qcdConversionFolder, true));
			ElectronAnalyserPtr qcdNonIsoAnalyser(new ElectronAnalyser(histMan_, qcdNonIsoFolder, true));
			ElectronAnalyserPtr qcdPFRelIsoAnalyser(new ElectronAnalyser(histMan_, qcdPFRelIsoFolder, true));
			ElectronAnalyserPtr qcdAntiIDAnalyser(new ElectronAnalyser(histMan_, qcdAntiIDFolder, true));
			ElectronAnalyserPtr qcdNoIsoNoIDAnalyser(new ElectronAnalyser(histMan_, qcdNoIsoNoIDFolder, true));
			MuonAnalyserPtr qcdNonIsoAnalyser_muon(new MuonAnalyser(histMan_, qcdNonIsoFolder_muons, true));
			MuonAnalyserPtr qcdPFRelIsoAnalyser_muon(new MuonAnalyser(histMan_, qcdPFRelIsoFolder_muons, true));
			analyser->useTTbarPlusMETSetup(true);
			analyser_muon->useTTbarPlusMETSetup(true);
			qcdConversionAnalyser->useTTbarPlusMETSetup(true);
			qcdNonIsoAnalyser->useTTbarPlusMETSetup(true);
			qcdPFRelIsoAnalyser->useTTbarPlusMETSetup(true);
			qcdAntiIDAnalyser->useTTbarPlusMETSetup(true);
			qcdNoIsoNoIDAnalyser->useTTbarPlusMETSetup(true);
			qcdNonIsoAnalyser_muon->useTTbarPlusMETSetup(true);
			qcdPFRelIsoAnalyser_muon->useTTbarPlusMETSetup(true);

			binnedElectronAnalysers_.push_back(analyser);
			binnedMuonAnalysers_.push_back(analyser_muon);
			qcdConversionBinnedElectronAnalysers_.push_back(qcdConversionAnalyser);
			qcdNonIsoBinnedElectronAnalysers_.push_back(qcdNonIsoAnalyser);
			qcdPFRelIsoBinnedElectronAnalysers_.push_back(qcdPFRelIsoAnalyser);
			qcdAntiIDBinnedElectronAnalysers_.push_back(qcdAntiIDAnalyser);
			qcdNoIsoNoIDBinnedElectronAnalysers_.push_back(qcdNoIsoNoIDAnalyser);
			qcdNonIsoBinnedMuonAnalysers_.push_back(qcdNonIsoAnalyser_muon);
			qcdPFRelIsoBinnedMuonAnalysers_.push_back(qcdPFRelIsoAnalyser_muon);
		}
	}
}

TTbarPlusMETAnalyser::~TTbarPlusMETAnalyser() {
}

} /* namespace BAT */
