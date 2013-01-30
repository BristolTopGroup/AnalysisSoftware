/*
 * TTbar_plus_X_analyser.cpp
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Analysers/TTbar_plus_X_analyser.h"
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

void TTbar_plus_X_analyser::analyse(const EventPtr event) {
	ePlusJetsSignalAnalysis(event);
	ePlusJetsQcdAnalysis(event);
	muPlusJetsSignalAnalysis(event);
	muPlusJetsQcdAnalysis(event);
}

void TTbar_plus_X_analyser::ePlusJetsSignalAnalysis(const EventPtr event) {

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

			metAnalyserEPlusJetsRefSelection_->analyse(event, signalLepton, jets);

			electronAnalyserRefSelection_->analyse(event);
			electronAnalyserRefSelection_->analyseElectron(signalElectron, event->weight());

			vertexAnalyserEPlusJetsRefSelection_->analyse(event);
			jetAnalyserEPlusJetsRefSelection_->analyse(event);

			ref_selection_binned_HT_analyser_electron_eta_->setScale(bjetWeight);
			ref_selection_binned_HT_analyser_electron_eta_->analyse(Event::HT(jets), fabs(signalElectron->eta()),
					event->weight());
			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				const METPointer met(event->MET((METAlgorithm::value) metIndex));
				ref_selection_binned_MET_analyser_electron_eta_.at(metIndex)->setScale(bjetWeight);
				ref_selection_binned_MET_analyser_electron_eta_.at(metIndex)->analyse(met->et(),
						fabs(signalElectron->eta()), event->weight());

				ref_selection_binned_ST_analyser_electron_eta_.at(metIndex)->setScale(bjetWeight);
				ref_selection_binned_ST_analyser_electron_eta_.at(metIndex)->analyse(
						Event::ST(jets, signalElectron, met), fabs(signalElectron->eta()), event->weight());

				ref_selection_binned_MT_analyser_electron_eta_.at(metIndex)->setScale(bjetWeight);
				ref_selection_binned_MT_analyser_electron_eta_.at(metIndex)->analyse(Event::MT(signalElectron, met),
						fabs(signalElectron->eta()), event->weight());
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
	}

}

void TTbar_plus_X_analyser::ePlusJetsQcdAnalysis(const EventPtr event) {
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
			metAnalyserqcdNonIsoElectronSelection_->analyse(event, signalLepton, jets);
			qcd_noniso_binned_HT_analyser_electron_eta_->setScale(bjetWeight);
			qcd_noniso_binned_HT_analyser_electron_eta_->analyse(Event::HT(jets), fabs(signalElectron->eta()),
					event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				const METPointer met(event->MET((METAlgorithm::value) metIndex));

				qcd_noniso_binned_MET_analyser_electron_eta_.at(metIndex)->setScale(bjetWeight);
				qcd_noniso_binned_MET_analyser_electron_eta_.at(metIndex)->analyse(met->et(),
						fabs(signalElectron->eta()), event->weight());

				qcd_noniso_binned_ST_analyser_electron_eta_.at(metIndex)->setScale(bjetWeight);
				qcd_noniso_binned_ST_analyser_electron_eta_.at(metIndex)->analyse(Event::ST(jets, signalElectron, met),
						fabs(signalElectron->eta()), event->weight());

				qcd_noniso_binned_MT_analyser_electron_eta_.at(metIndex)->setScale(bjetWeight);
				qcd_noniso_binned_MT_analyser_electron_eta_.at(metIndex)->analyse(Event::MT(signalElectron, met),
						fabs(signalElectron->eta()), event->weight());

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
			metAnalyserqcdConversionSelection_->analyse(event, signalLepton, jets);

			qcd_conversion_binned_HT_analyser_electron_eta_->setScale(bjetWeight);
			qcd_conversion_binned_HT_analyser_electron_eta_->analyse(Event::HT(jets), fabs(signalElectron->eta()),
					event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				const METPointer met(event->MET((METAlgorithm::value) metIndex));
				qcd_conversion_binned_MET_analyser_electron_eta_.at(metIndex)->setScale(bjetWeight);
				qcd_conversion_binned_MET_analyser_electron_eta_.at(metIndex)->analyse(met->et(),
						fabs(signalElectron->eta()), event->weight());

				qcd_conversion_binned_ST_analyser_electron_eta_.at(metIndex)->setScale(bjetWeight);
				qcd_conversion_binned_ST_analyser_electron_eta_.at(metIndex)->analyse(
						Event::ST(jets, signalElectron, met), fabs(signalElectron->eta()), event->weight());

				qcd_conversion_binned_MT_analyser_electron_eta_.at(metIndex)->setScale(bjetWeight);
				qcd_conversion_binned_MT_analyser_electron_eta_.at(metIndex)->analyse(Event::MT(signalElectron, met),
						fabs(signalElectron->eta()), event->weight());
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

			qcd_PFRelIso_binned_HT_analyser_electron_isolation_->setScale(bjetWeight);
			qcd_PFRelIso_binned_HT_analyser_electron_isolation_->analyse(Event::HT(jets),
					signalElectron->pfRelativeIsolation(0.3), event->weight());

			qcd_PFRelIso_binned_HT_analyser_electron_rhocorrected_isolation_->setScale(bjetWeight);
			qcd_PFRelIso_binned_HT_analyser_electron_rhocorrected_isolation_->analyse(Event::HT(jets),
					signalElectron->pfRelativeIsolationRhoCorrected(), event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				const METPointer met(event->MET((METAlgorithm::value) metIndex));

				//this is rho corrected as well! But a bit different, due to 2011 active areas I assume
				qcd_PFRelIso_binned_MET_analyser_electron_isolation_.at(metIndex)->setScale(bjetWeight);
				qcd_PFRelIso_binned_MET_analyser_electron_isolation_.at(metIndex)->analyse(met->et(),
						signalElectron->pfRelativeIsolation(0.3), event->weight());

				qcd_PFRelIso_binned_MET_analyser_electron_rhocorrected_isolation_.at(metIndex)->setScale(bjetWeight);
				qcd_PFRelIso_binned_MET_analyser_electron_rhocorrected_isolation_.at(metIndex)->analyse(met->et(),
						signalElectron->pfRelativeIsolationRhoCorrected(), event->weight());

				qcd_PFRelIso_binned_ST_analyser_electron_isolation_.at(metIndex)->setScale(bjetWeight);
				qcd_PFRelIso_binned_ST_analyser_electron_isolation_.at(metIndex)->analyse(
						Event::ST(jets, signalElectron, met), signalElectron->pfRelativeIsolation(0.3),
						event->weight());

				qcd_PFRelIso_binned_ST_analyser_electron_rhocorrected_isolation_.at(metIndex)->setScale(bjetWeight);
				qcd_PFRelIso_binned_ST_analyser_electron_rhocorrected_isolation_.at(metIndex)->analyse(
						Event::ST(jets, signalElectron, met), signalElectron->pfRelativeIsolationRhoCorrected(),
						event->weight());

				qcd_PFRelIso_binned_MT_analyser_electron_isolation_.at(metIndex)->setScale(bjetWeight);
				qcd_PFRelIso_binned_MT_analyser_electron_isolation_.at(metIndex)->analyse(
						Event::MT(signalElectron, met), signalElectron->pfRelativeIsolation(0.3), event->weight());

				qcd_PFRelIso_binned_MT_analyser_electron_rhocorrected_isolation_.at(metIndex)->setScale(bjetWeight);
				qcd_PFRelIso_binned_MT_analyser_electron_rhocorrected_isolation_.at(metIndex)->analyse(
						Event::MT(signalElectron, met), signalElectron->pfRelativeIsolationRhoCorrected(),
						event->weight());
			}
		}
	}
}

void TTbar_plus_X_analyser::muPlusJetsSignalAnalysis(const EventPtr event) {

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

			metAnalyserMuPlusJetsRefSelection_->analyse(event, signalLepton, jets);

			muonAnalyserRefSelection_->analyse(event);
			muonAnalyserRefSelection_->analyseMuon(signalMuon, event->weight());

			vertexAnalyserMuPlusJetsRefSelection_->analyse(event);
			jetAnalyserMuPlusJetsRefSelection_->analyse(event);

			ref_selection_binned_HT_analyser_muon_eta_->setScale(bjetWeight * efficiencyCorrection);
			ref_selection_binned_HT_analyser_muon_eta_->analyse(Event::HT(jets), fabs(signalMuon->eta()),
					event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				const METPointer met(event->MET((METAlgorithm::value) metIndex));
				ref_selection_binned_MET_analyser_muon_eta_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				ref_selection_binned_MET_analyser_muon_eta_.at(metIndex)->analyse(met->et(), fabs(signalMuon->eta()),
						event->weight());

				ref_selection_binned_ST_analyser_muon_eta_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				ref_selection_binned_ST_analyser_muon_eta_.at(metIndex)->analyse(Event::ST(jets, signalMuon, met),
						fabs(signalMuon->eta()), event->weight());

				ref_selection_binned_MT_analyser_muon_eta_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				ref_selection_binned_MT_analyser_muon_eta_.at(metIndex)->analyse(Event::MT(signalMuon, met),
						fabs(signalMuon->eta()), event->weight());
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
		histMan_->H1D("N_BJets")->Fill(numberOfBjets, event->weight() * efficiencyCorrection);
	}

}

void TTbar_plus_X_analyser::muPlusJetsQcdAnalysis(const EventPtr event) {
	//selection with respect to reference selection
	if (qcd_noniso_muon_plus_jets_selection_->passesSelectionUpToStep(event,
			TTbarMuPlusJetsReferenceSelection::AtLeastThreeGoodJets)) {
		const JetCollection jets(qcd_noniso_muon_plus_jets_selection_->cleanedJets(event));
		const JetCollection bJets(qcd_noniso_muon_plus_jets_selection_->cleanedBJets(event));
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
		unsigned int prescale(qcd_noniso_muon_plus_jets_selection_->prescale(event));
		const LeptonPointer signalLepton = qcd_noniso_muon_plus_jets_selection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(true);

		qcdNonIsoElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoElectronSelection_->setPrescale(prescale);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdNonIsoMuonAnalyser_->setScale(bjetWeight * efficiencyCorrection);
			metAnalyserqcdNonIsoMuonSelection_->setScale(bjetWeight * efficiencyCorrection);

			qcdNonIsoMuonAnalyser_->analyse(event);
			qcdNonIsoMuonAnalyser_->analyseMuon(signalMuon, event->weight());
			metAnalyserqcdNonIsoMuonSelection_->analyse(event, signalLepton, jets);

			qcd_noniso_binned_HT_analyser_muon_eta_->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_HT_analyser_muon_eta_->analyse(Event::HT(jets), fabs(signalMuon->eta()), event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				const METPointer met(event->MET((METAlgorithm::value) metIndex));
				qcd_noniso_binned_MET_analyser_muon_eta_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_noniso_binned_MET_analyser_muon_eta_.at(metIndex)->analyse(met->et(), fabs(signalMuon->eta()),
						event->weight());

				qcd_noniso_binned_ST_analyser_muon_eta_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_noniso_binned_ST_analyser_muon_eta_.at(metIndex)->analyse(Event::ST(jets, signalMuon, met),
						fabs(signalMuon->eta()), event->weight());

				qcd_noniso_binned_MT_analyser_muon_eta_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_noniso_binned_MT_analyser_muon_eta_.at(metIndex)->analyse(Event::MT(signalMuon, met),
						fabs(signalMuon->eta()), event->weight());
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

		qcdMuPlusjetsPFRelIsoMuonAnalyser_->setPrescale(prescale);
		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			histMan_->setCurrentBJetBin(weightIndex);
			qcdMuPlusjetsPFRelIsoMuonAnalyser_->setScale(bjetWeight * efficiencyCorrection);

			qcdMuPlusjetsPFRelIsoMuonAnalyser_->analyse(event);
			qcdMuPlusjetsPFRelIsoMuonAnalyser_->analyseMuon(signalMuon, event->weight());

			qcd_PFRelIso_binned_HT_analyser_muon_eta_->setScale(bjetWeight * efficiencyCorrection);
			qcd_PFRelIso_binned_HT_analyser_muon_eta_->analyse(Event::HT(jets), fabs(signalMuon->eta()),
					event->weight());
			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				const METPointer met(event->MET((METAlgorithm::value) metIndex));
				qcd_PFRelIso_binned_MET_analyser_muon_eta_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_PFRelIso_binned_MET_analyser_muon_eta_.at(metIndex)->analyse(met->et(), fabs(signalMuon->eta()),
						event->weight());

				qcd_PFRelIso_binned_ST_analyser_muon_eta_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_PFRelIso_binned_ST_analyser_muon_eta_.at(metIndex)->analyse(Event::ST(jets, signalMuon, met),
						fabs(signalMuon->eta()), event->weight());

				qcd_PFRelIso_binned_MT_analyser_muon_eta_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_PFRelIso_binned_MT_analyser_muon_eta_.at(metIndex)->analyse(Event::MT(signalMuon, met),
						fabs(signalMuon->eta()), event->weight());

			}
		}
	}

	if (qcd_noiso_muon_plus_jets_selection_->passesSelectionUpToStep(event,
			TTbarMuPlusJetsReferenceSelection::AtLeastThreeGoodJets)) {
		const JetCollection jets(qcd_noiso_muon_plus_jets_selection_->cleanedJets(event));
		const JetCollection bJets(qcd_noiso_muon_plus_jets_selection_->cleanedBJets(event));
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
		unsigned int prescale(qcd_noiso_muon_plus_jets_selection_->prescale(event));
		const LeptonPointer signalLepton = qcd_noiso_muon_plus_jets_selection_->signalLepton(event);
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

void TTbar_plus_X_analyser::createHistograms() {
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
	//signal
	metAnalyserEPlusJetsRefSelection_->createHistograms();
	electronAnalyserRefSelection_->createHistograms();
	metAnalyserMuPlusJetsRefSelection_->createHistograms();
	muonAnalyserRefSelection_->createHistograms();

	vertexAnalyserEPlusJetsRefSelection_->createHistograms();
	vertexAnalyserMuPlusJetsRefSelection_->createHistograms();
	//QCD region
	qcdNonIsoElectronAnalyser_->createHistograms();
	metAnalyserqcdNonIsoElectronSelection_->createHistograms();
	qcdNonIsoMuonAnalyser_->createHistograms();
	qcdNoIsolationMuonAnalyser_->createHistograms();
	metAnalyserqcdNonIsoMuonSelection_->createHistograms();

	qcdConversionsElectronAnalyser_->createHistograms();
	metAnalyserqcdConversionSelection_->createHistograms();

	qcdEPlusjetsPFRelIsoElectronAnalyser_->createHistograms();
	qcdMuPlusjetsPFRelIsoMuonAnalyser_->createHistograms();

	for (unsigned int index = 0; index < ref_selection_binned_MET_analyser_electron_eta_.size(); ++index) {
		ref_selection_binned_MET_analyser_electron_eta_.at(index)->createHistograms();
	}

	ref_selection_binned_HT_analyser_electron_eta_->createHistograms();
	ref_selection_binned_HT_analyser_muon_eta_->createHistograms();
	qcd_noniso_binned_HT_analyser_electron_eta_->createHistograms();
	qcd_noniso_binned_HT_analyser_muon_eta_->createHistograms();

	qcd_PFRelIso_binned_HT_analyser_electron_isolation_->createHistograms();
	qcd_PFRelIso_binned_HT_analyser_electron_rhocorrected_isolation_->createHistograms();
	qcd_PFRelIso_binned_HT_analyser_muon_eta_->createHistograms();
	qcd_conversion_binned_HT_analyser_electron_eta_->createHistograms();

	for (unsigned int index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, index))
			continue;
		ref_selection_binned_MET_analyser_electron_eta_.at(index)->createHistograms();
		qcd_conversion_binned_MET_analyser_electron_eta_.at(index)->createHistograms();
		qcd_noniso_binned_MET_analyser_electron_eta_.at(index)->createHistograms();
		qcd_PFRelIso_binned_MET_analyser_electron_isolation_.at(index)->createHistograms();
		qcd_PFRelIso_binned_MET_analyser_electron_rhocorrected_isolation_.at(index)->createHistograms();

		ref_selection_binned_MET_analyser_muon_eta_.at(index)->createHistograms();
		qcd_PFRelIso_binned_MET_analyser_muon_eta_.at(index)->createHistograms();
		qcd_noniso_binned_MET_analyser_muon_eta_.at(index)->createHistograms();

		ref_selection_binned_ST_analyser_electron_eta_.at(index)->createHistograms();
		qcd_conversion_binned_ST_analyser_electron_eta_.at(index)->createHistograms();
		qcd_noniso_binned_ST_analyser_electron_eta_.at(index)->createHistograms();
		qcd_PFRelIso_binned_ST_analyser_electron_isolation_.at(index)->createHistograms();
		qcd_PFRelIso_binned_ST_analyser_electron_rhocorrected_isolation_.at(index)->createHistograms();

		ref_selection_binned_ST_analyser_muon_eta_.at(index)->createHistograms();
		qcd_PFRelIso_binned_ST_analyser_muon_eta_.at(index)->createHistograms();
		qcd_noniso_binned_ST_analyser_muon_eta_.at(index)->createHistograms();

		ref_selection_binned_MT_analyser_electron_eta_.at(index)->createHistograms();
		qcd_conversion_binned_MT_analyser_electron_eta_.at(index)->createHistograms();
		qcd_noniso_binned_MT_analyser_electron_eta_.at(index)->createHistograms();
		qcd_PFRelIso_binned_MT_analyser_electron_isolation_.at(index)->createHistograms();
		qcd_PFRelIso_binned_MT_analyser_electron_rhocorrected_isolation_.at(index)->createHistograms();

		ref_selection_binned_MT_analyser_muon_eta_.at(index)->createHistograms();
		qcd_PFRelIso_binned_MT_analyser_muon_eta_.at(index)->createHistograms();
		qcd_noniso_binned_MT_analyser_muon_eta_.at(index)->createHistograms();

	}

	jetAnalyserEPlusJetsRefSelection_->createHistograms();
	jetAnalyserMuPlusJetsRefSelection_->createHistograms();

}

TTbar_plus_X_analyser::TTbar_plus_X_analyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		topEplusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection()), //
		topMuplusJetsRefSelection_(new TopPairMuPlusJetsReferenceSelection()), //
		//QCD selections with respect to reference selection
		qcdNonIsoElectronSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdConversionSelection_(new QCDConversionsSelection()), //
		qcdPFRelIsoEPlusJetsSelection_(new QCDPFRelIsoEPlusJetsSelection()), //
		qcdPFRelIsoMuPlusJetsSelection_(new QCDPFRelIsoMuPlusJetsSelection()), //
		qcd_noniso_muon_plus_jets_selection_(new QCDNonIsolatedMuonSelection()), //
		qcd_noiso_muon_plus_jets_selection_(new NoIsolationMuonSelection()), //
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
		metAnalyserqcdNonIsoMuonSelection_(
				new METAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets/MET")), //
		qcdNonIsoElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/EPlusJets/QCD non iso e+jets/Electron", true)), //
		qcdNonIsoMuonAnalyser_(
				new MuonAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets/Muon", true)), //
		metAnalyserqcdConversionSelection_(new METAnalyser(histMan, histogramFolder + "/EPlusJets/QCDConversions/MET")), //
		qcdConversionsElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/EPlusJets/QCDConversions/Electron", true)), //
		qcdEPlusjetsPFRelIsoElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/EPlusJets/QCD e+jets PFRelIso/Electron", true)), //
		qcdMuPlusjetsPFRelIsoMuonAnalyser_(
				new MuonAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD mu+jets PFRelIso/Muon", true)), //
		qcdNoIsolationMuonAnalyser_(new MuonAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD No Iso/Muon", true)), //
		metBins_(), //
		ht_bins_(), //
		st_bins_(), //
		mt_bins_(), //
		ref_selection_binned_MET_analyser_electron_eta_(), //
		qcd_conversion_binned_MET_analyser_electron_eta_(), //
		qcd_noniso_binned_MET_analyser_electron_eta_(), //
		qcd_PFRelIso_binned_MET_analyser_electron_isolation_(), //
		qcd_PFRelIso_binned_MET_analyser_electron_rhocorrected_isolation_(), //
		ref_selection_binned_MET_analyser_muon_eta_(), //
		qcd_noniso_binned_MET_analyser_muon_eta_(), //
		qcd_PFRelIso_binned_MET_analyser_muon_eta_(), //
		//HT
		ref_selection_binned_HT_analyser_electron_eta_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_HT_Analysis")), //
		qcd_conversion_binned_HT_analyser_electron_eta_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_HT_Analysis")), //
		qcd_noniso_binned_HT_analyser_electron_eta_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_HT_Analysis")), //
		qcd_PFRelIso_binned_HT_analyser_electron_isolation_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD e+jets PFRelIso/Binned_HT_Analysis")), //
		qcd_PFRelIso_binned_HT_analyser_electron_rhocorrected_isolation_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD e+jets PFRelIso/Binned_HT_Analysis")), //
		//HT muons
		ref_selection_binned_HT_analyser_muon_eta_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_HT_Analysis")), //
		qcd_noniso_binned_HT_analyser_muon_eta_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD mu+jets PFRelIso/Binned_HT_Analysis")), //
		qcd_PFRelIso_binned_HT_analyser_muon_eta_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets/Binned_HT_Analysis")), //
		//ST
		ref_selection_binned_ST_analyser_electron_eta_(), //
		qcd_conversion_binned_ST_analyser_electron_eta_(), //
		qcd_noniso_binned_ST_analyser_electron_eta_(), //
		qcd_PFRelIso_binned_ST_analyser_electron_isolation_(), //
		qcd_PFRelIso_binned_ST_analyser_electron_rhocorrected_isolation_(), //
		//ST
		ref_selection_binned_ST_analyser_muon_eta_(), //
		qcd_noniso_binned_ST_analyser_muon_eta_(), //
		qcd_PFRelIso_binned_ST_analyser_muon_eta_(), //
		//MT electrons
		ref_selection_binned_MT_analyser_electron_eta_(), //
		qcd_conversion_binned_MT_analyser_electron_eta_(), //
		qcd_noniso_binned_MT_analyser_electron_eta_(), //
		qcd_PFRelIso_binned_MT_analyser_electron_isolation_(), //
		qcd_PFRelIso_binned_MT_analyser_electron_rhocorrected_isolation_(), //
		//MT muons
		ref_selection_binned_MT_analyser_muon_eta_(), //
		qcd_noniso_binned_MT_analyser_muon_eta_(), //
		qcd_PFRelIso_binned_MT_analyser_muon_eta_(), //
		jetAnalyserEPlusJetsRefSelection_(new JetAnalyser(histMan, histogramFolder + "/EPlusJets/Ref selection/Jets")), //
		jetAnalyserMuPlusJetsRefSelection_(new JetAnalyser(histMan, histogramFolder + "/MuPlusJets/Ref selection/Jets")) {
	//MET bins: 25, 45, 70, 100, 150, inf
	metBins_.push_back(25.);
	metBins_.push_back(45.);
	metBins_.push_back(70.);
	metBins_.push_back(100.);
	metBins_.push_back(150.);
	//HT: 50, 150, 250, 350, 450, 650, 1100, inf
	ht_bins_.push_back(50);
	ht_bins_.push_back(150);
	ht_bins_.push_back(250);
	ht_bins_.push_back(350);
	ht_bins_.push_back(450);
	ht_bins_.push_back(650);
	ht_bins_.push_back(1100);

	//ST:150, 250, 350, 450, 550, 750, 1250, inf
	st_bins_.push_back(150);
	st_bins_.push_back(250);
	st_bins_.push_back(350);
	st_bins_.push_back(450);
	st_bins_.push_back(550);
	st_bins_.push_back(750);
	st_bins_.push_back(1250);

	//[0, 40, 65, 85, 150, inf
	mt_bins_.push_back(40);
	mt_bins_.push_back(65);
	mt_bins_.push_back(85);
	mt_bins_.push_back(150);

	make_binned_MET_analysers();
	make_binned_HT_analysers();
	make_binned_ST_analysers();
	make_binned_MT_analysers();
}

TTbar_plus_X_analyser::~TTbar_plus_X_analyser() {
}

void TTbar_plus_X_analyser::make_binned_MET_analysers() {
	//for all MET types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			continue;
		string metPrefix = METAlgorithm::names.at(metIndex);

		Binned_Variable_analyser_ptr ref_selection_binned_met_electron_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_conversion_binned_met_electron_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_met_electron_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_PFRelIso_binned_met_electron_isolation_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD e+jets PFRelIso/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_PFRelIso_binned_met_electron_rhocorrected_isolation_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD e+jets PFRelIso/Binned_MET_Analysis"));
		ref_selection_binned_met_electron_eta_analyser->set_variables(metPrefix, metBins_, "electron_absolute_eta", 30,
				0.0, 3.0);
		qcd_conversion_binned_met_electron_eta_analyser->set_variables(metPrefix, metBins_, "electron_absolute_eta", 30,
				0.0, 3.0);
		qcd_noniso_binned_met_electron_eta_analyser->set_variables(metPrefix, metBins_, "electron_absolute_eta", 30,
				0.0, 3.0);
		qcd_PFRelIso_binned_met_electron_isolation_analyser->set_variables(metPrefix, metBins_,
				"electron_pfIsolation_03", 500, 0, 5);
		qcd_PFRelIso_binned_met_electron_rhocorrected_isolation_analyser->set_variables(metPrefix, metBins_,
				"electron_rhoCorrectedIso_03", 500, 0, 5);
		ref_selection_binned_MET_analyser_electron_eta_.push_back(ref_selection_binned_met_electron_eta_analyser);
		qcd_conversion_binned_MET_analyser_electron_eta_.push_back(qcd_conversion_binned_met_electron_eta_analyser);
		qcd_noniso_binned_MET_analyser_electron_eta_.push_back(qcd_noniso_binned_met_electron_eta_analyser);
		qcd_PFRelIso_binned_MET_analyser_electron_isolation_.push_back(
				qcd_PFRelIso_binned_met_electron_isolation_analyser);
		qcd_PFRelIso_binned_MET_analyser_electron_rhocorrected_isolation_.push_back(
				qcd_PFRelIso_binned_met_electron_rhocorrected_isolation_analyser);

		//binned MET muons
		Binned_Variable_analyser_ptr ref_selection_binned_met_muon_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_PFRelIso_binned_met_muon_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD mu+jets PFRelIso/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_met_muon_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets/Binned_MET_Analysis"));

		ref_selection_binned_met_muon_eta_analyser->set_variables(metPrefix, metBins_, "muon_absolute_eta", 30, 0.0,
				3.0);
		qcd_PFRelIso_binned_met_muon_eta_analyser->set_variables(metPrefix, metBins_, "muon_absolute_eta", 30, 0.0,
				3.0);
		qcd_noniso_binned_met_muon_eta_analyser->set_variables(metPrefix, metBins_, "muon_absolute_eta", 30, 0.0, 3.0);
		ref_selection_binned_MET_analyser_muon_eta_.push_back(ref_selection_binned_met_muon_eta_analyser);
		qcd_PFRelIso_binned_MET_analyser_muon_eta_.push_back(qcd_PFRelIso_binned_met_muon_eta_analyser);
		qcd_noniso_binned_MET_analyser_muon_eta_.push_back(qcd_noniso_binned_met_muon_eta_analyser);
	}
}

void TTbar_plus_X_analyser::make_binned_HT_analysers() {
	ref_selection_binned_HT_analyser_electron_eta_->set_variables("HT", ht_bins_, "electron_absolute_eta", 30, 0.0,
			3.0);
	qcd_conversion_binned_HT_analyser_electron_eta_->set_variables("HT", ht_bins_, "electron_absolute_eta", 30, 0.0,
			3.0);
	qcd_noniso_binned_HT_analyser_electron_eta_->set_variables("HT", ht_bins_, "electron_absolute_eta", 30, 0.0, 3.0);
	qcd_PFRelIso_binned_HT_analyser_electron_isolation_->set_variables("HT", ht_bins_, "electron_pfIsolation_03", 500,
			0, 5);
	qcd_PFRelIso_binned_HT_analyser_electron_rhocorrected_isolation_->set_variables("HT", ht_bins_,
			"electron_rhoCorrectedIso_03", 500, 0, 5);

	ref_selection_binned_HT_analyser_muon_eta_->set_variables("HT", ht_bins_, "muon_absolute_eta", 30, 0.0, 3.0);
	qcd_PFRelIso_binned_HT_analyser_muon_eta_->set_variables("HT", ht_bins_, "muon_absolute_eta", 30, 0.0, 3.0);
	qcd_noniso_binned_HT_analyser_muon_eta_->set_variables("HT", ht_bins_, "muon_absolute_eta", 30, 0.0, 3.0);
}

void TTbar_plus_X_analyser::make_binned_ST_analysers() {
	//for all ST types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			continue;
		string metPrefix = METAlgorithm::names.at(metIndex);

		Binned_Variable_analyser_ptr ref_selection_binned_ST_electron_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_conversion_binned_ST_electron_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_ST_electron_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_PFRelIso_binned_ST_electron_isolation_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD e+jets PFRelIso/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_PFRelIso_binned_ST_electron_rhocorrected_isolation_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD e+jets PFRelIso/Binned_ST_Analysis"));
		ref_selection_binned_ST_electron_eta_analyser->set_variables("ST_with_" + metPrefix, st_bins_,
				"electron_absolute_eta", 30, 0.0, 3.0);
		qcd_conversion_binned_ST_electron_eta_analyser->set_variables("ST_with_" + metPrefix, st_bins_,
				"electron_absolute_eta", 30, 0.0, 3.0);
		qcd_noniso_binned_ST_electron_eta_analyser->set_variables("ST_with_" + metPrefix, st_bins_,
				"electron_absolute_eta", 30, 0.0, 3.0);
		qcd_PFRelIso_binned_ST_electron_isolation_analyser->set_variables("ST_with_" + metPrefix, st_bins_,
				"electron_pfIsolation_03", 500, 0, 5);
		qcd_PFRelIso_binned_ST_electron_rhocorrected_isolation_analyser->set_variables("ST_with_" + metPrefix, st_bins_,
				"electron_rhoCorrectedIso_03", 500, 0, 5);
		ref_selection_binned_ST_analyser_electron_eta_.push_back(ref_selection_binned_ST_electron_eta_analyser);
		qcd_conversion_binned_ST_analyser_electron_eta_.push_back(qcd_conversion_binned_ST_electron_eta_analyser);
		qcd_noniso_binned_ST_analyser_electron_eta_.push_back(qcd_noniso_binned_ST_electron_eta_analyser);
		qcd_PFRelIso_binned_ST_analyser_electron_isolation_.push_back(
				qcd_PFRelIso_binned_ST_electron_isolation_analyser);
		qcd_PFRelIso_binned_ST_analyser_electron_rhocorrected_isolation_.push_back(
				qcd_PFRelIso_binned_ST_electron_rhocorrected_isolation_analyser);

		//binned ST muons
		Binned_Variable_analyser_ptr ref_selection_binned_ST_muon_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_PFRelIso_binned_ST_muon_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD mu+jets PFRelIso/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_ST_muon_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets/Binned_ST_Analysis"));

		ref_selection_binned_ST_muon_eta_analyser->set_variables("ST_with_" + metPrefix, st_bins_, "muon_absolute_eta",
				30, 0.0, 3.0);
		qcd_PFRelIso_binned_ST_muon_eta_analyser->set_variables("ST_with_" + metPrefix, st_bins_, "muon_absolute_eta",
				30, 0.0, 3.0);
		qcd_noniso_binned_ST_muon_eta_analyser->set_variables("ST_with_" + metPrefix, st_bins_, "muon_absolute_eta", 30,
				0.0, 3.0);
		ref_selection_binned_ST_analyser_muon_eta_.push_back(ref_selection_binned_ST_muon_eta_analyser);
		qcd_PFRelIso_binned_ST_analyser_muon_eta_.push_back(qcd_PFRelIso_binned_ST_muon_eta_analyser);
		qcd_noniso_binned_ST_analyser_muon_eta_.push_back(qcd_noniso_binned_ST_muon_eta_analyser);
	}
}

void TTbar_plus_X_analyser::make_binned_MT_analysers() {
	//for all MT types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			continue;
		string metPrefix = METAlgorithm::names.at(metIndex);

		Binned_Variable_analyser_ptr ref_selection_binned_MT_electron_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_conversion_binned_MT_electron_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_MT_electron_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_PFRelIso_binned_MT_electron_isolation_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD e+jets PFRelIso/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_PFRelIso_binned_MT_electron_rhocorrected_isolation_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD e+jets PFRelIso/Binned_MT_Analysis"));
		ref_selection_binned_MT_electron_eta_analyser->set_variables("MT_with_" + metPrefix, mt_bins_,
				"electron_absolute_eta", 30, 0.0, 3.0);
		qcd_conversion_binned_MT_electron_eta_analyser->set_variables("MT_with_" + metPrefix, mt_bins_,
				"electron_absolute_eta", 30, 0.0, 3.0);
		qcd_noniso_binned_MT_electron_eta_analyser->set_variables("MT_with_" + metPrefix, mt_bins_,
				"electron_absolute_eta", 30, 0.0, 3.0);
		qcd_PFRelIso_binned_MT_electron_isolation_analyser->set_variables("MT_with_" + metPrefix, mt_bins_,
				"electron_pfIsolation_03", 500, 0, 5);
		qcd_PFRelIso_binned_MT_electron_rhocorrected_isolation_analyser->set_variables("MT_with_" + metPrefix, mt_bins_,
				"electron_rhoCorrectedIso_03", 500, 0, 5);
		ref_selection_binned_MT_analyser_electron_eta_.push_back(ref_selection_binned_MT_electron_eta_analyser);
		qcd_conversion_binned_MT_analyser_electron_eta_.push_back(qcd_conversion_binned_MT_electron_eta_analyser);
		qcd_noniso_binned_MT_analyser_electron_eta_.push_back(qcd_noniso_binned_MT_electron_eta_analyser);
		qcd_PFRelIso_binned_MT_analyser_electron_isolation_.push_back(
				qcd_PFRelIso_binned_MT_electron_isolation_analyser);
		qcd_PFRelIso_binned_MT_analyser_electron_rhocorrected_isolation_.push_back(
				qcd_PFRelIso_binned_MT_electron_rhocorrected_isolation_analyser);

		//binned MT muons
		Binned_Variable_analyser_ptr ref_selection_binned_MT_muon_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_PFRelIso_binned_MT_muon_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD mu+jets PFRelIso/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_MT_muon_eta_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets/Binned_MT_Analysis"));

		ref_selection_binned_MT_muon_eta_analyser->set_variables("MT_with_" + metPrefix, mt_bins_, "muon_absolute_eta",
				30, 0.0, 3.0);
		qcd_PFRelIso_binned_MT_muon_eta_analyser->set_variables("MT_with_" + metPrefix, mt_bins_, "muon_absolute_eta",
				30, 0.0, 3.0);
		qcd_noniso_binned_MT_muon_eta_analyser->set_variables("MT_with_" + metPrefix, mt_bins_, "muon_absolute_eta", 30,
				0.0, 3.0);
		ref_selection_binned_MT_analyser_muon_eta_.push_back(ref_selection_binned_MT_muon_eta_analyser);
		qcd_PFRelIso_binned_MT_analyser_muon_eta_.push_back(qcd_PFRelIso_binned_MT_muon_eta_analyser);
		qcd_noniso_binned_MT_analyser_muon_eta_.push_back(qcd_noniso_binned_MT_muon_eta_analyser);
	}
}

} /* namespace BAT */
