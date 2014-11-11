/*
 * TTbar_plus_X_analyser.cpp
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Analysers/TTbar_plus_X_analyser.h"
#include "../../interface/BTagWeight.h"
#include "../../interface/GlobalVariables.h"
//#include "../../interface/TopPairEventCandidate.h"
//signal selections
#include "../../interface/Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../../interface/Selections/TopPairMuPlusJetsReferenceSelection.h"
//QCD selections w.r.t signal ref selection
#include "../../interface/Selections/QCDNonIsolatedElectronSelection.h"
#include "../../interface/Selections/QCDConversionsSelection.h"
#include "../../interface/Selections/QCDPFRelIsoEPlusJetsSelection.h"
//muons
#include "../../interface/Selections/QCDPFRelIsoMuPlusJetsSelection.h"
#include "../../interface/Selections/QCDNonIsolatedMuonSelection.h"

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
		} else {
			bjetWeights = BjetWeights(jets, numberOfBjets);
		}
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/Ref selection");

		const LeptonPointer signalLepton = topEplusJetsRefSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));
		double efficiencyCorrection = event->isRealData() ? 1. : signalElectron->getEfficiencyCorrection(false, Globals::ElectronScaleFactorSystematic, event->runnumber());

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			if ( bjetWeight == 0 ) continue;

			histMan_->setCurrentBJetBin(weightIndex);
			histMan_->H1D("BTagWeights")->Fill(bjetWeight);
			histMan_->H1D("N_BJets_reweighted")->Fill(weightIndex, event->weight() * bjetWeight * efficiencyCorrection);

			metAnalyserEPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
			electronAnalyserRefSelection_->setScale(bjetWeight * efficiencyCorrection);
			vertexAnalyserEPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
			jetAnalyserEPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);

			metAnalyserEPlusJetsRefSelection_->analyse(event, signalLepton, jets);

			electronAnalyserRefSelection_->analyse(event);
			electronAnalyserRefSelection_->analyseElectron(signalElectron, event->weight());

			vertexAnalyserEPlusJetsRefSelection_->analyse(event);
			jetAnalyserEPlusJetsRefSelection_->analyse(event);

			ref_selection_binned_HT_analyser_electron_->setScale(bjetWeight * efficiencyCorrection);

			vector<double> fit_variable_values;
			fit_variable_values.push_back(fabs(signalElectron->eta()));
			fit_variable_values.push_back(Event::M3(jets));
			fit_variable_values.push_back(Event::M_bl(bJets, signalElectron));
			fit_variable_values.push_back(Event::angle_bl(bJets, signalElectron));
			ref_selection_binned_HT_analyser_electron_->analyse(Event::HT(jets), fit_variable_values, event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				const METPointer met(
						event->MET((METAlgorithm::value) metIndex));
				ref_selection_binned_MET_analyser_electron_.at(metIndex)->setScale(
						bjetWeight * efficiencyCorrection);
				ref_selection_binned_MET_analyser_electron_.at(metIndex)->analyse(
						met->et(), fit_variable_values, event->weight());

				ref_selection_binned_ST_analyser_electron_.at(metIndex)->setScale(
						bjetWeight * efficiencyCorrection);
				ref_selection_binned_ST_analyser_electron_.at(metIndex)->analyse(
						Event::ST(jets, signalElectron, met),
						fit_variable_values, event->weight());

				ref_selection_binned_MT_analyser_electron_.at(metIndex)->setScale(
						bjetWeight * efficiencyCorrection);
				ref_selection_binned_MT_analyser_electron_.at(metIndex)->analyse(
						Event::MT(signalElectron, met), fit_variable_values,
						event->weight());

				ref_selection_binned_WPT_analyser_electron_.at(metIndex)->setScale(
						bjetWeight * efficiencyCorrection);
				ref_selection_binned_WPT_analyser_electron_.at(metIndex)->analyse(
						Event::WPT(signalElectron, met), fit_variable_values,
						event->weight());
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
		double efficiencyCorrection = event->isRealData() ? 1. : signalElectron->getEfficiencyCorrection(false, Globals::ElectronScaleFactorSystematic, event->runnumber()); // should really be  qcd=true, not false, and should have a loop in Electron.cpp to get scale factors for QCD selection, but it doesn't exist (yet)

		qcdNonIsoElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoElectronSelection_->setPrescale(prescale);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			if ( bjetWeight == 0 ) continue;

			histMan_->setCurrentBJetBin(weightIndex);
			qcdNonIsoElectronAnalyser_->setScale(bjetWeight * efficiencyCorrection);
			metAnalyserqcdNonIsoElectronSelection_->setScale(bjetWeight * efficiencyCorrection);

			qcdNonIsoElectronAnalyser_->analyse(event);
			qcdNonIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());
			metAnalyserqcdNonIsoElectronSelection_->analyse(event, signalLepton, jets);
			qcd_noniso_binned_HT_analyser_electron_->setScale(bjetWeight * efficiencyCorrection);

			vector<double> fit_variable_values;
			fit_variable_values.push_back(fabs(signalElectron->eta()));
			fit_variable_values.push_back(Event::M3(jets));
			fit_variable_values.push_back(Event::M_bl(bJets, signalElectron));
			fit_variable_values.push_back(Event::angle_bl(bJets, signalElectron));

			qcd_noniso_binned_HT_analyser_electron_->analyse(Event::HT(jets), fit_variable_values, event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				const METPointer met(event->MET((METAlgorithm::value) metIndex));

				qcd_noniso_binned_MET_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_noniso_binned_MET_analyser_electron_.at(metIndex)->analyse(met->et(),
						fit_variable_values, event->weight());

				qcd_noniso_binned_ST_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_noniso_binned_ST_analyser_electron_.at(metIndex)->analyse(Event::ST(jets, signalElectron, met),
						fit_variable_values, event->weight());

				qcd_noniso_binned_MT_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_noniso_binned_MT_analyser_electron_.at(metIndex)->analyse(Event::MT(signalElectron, met),
						fit_variable_values, event->weight());

				qcd_noniso_binned_WPT_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_noniso_binned_WPT_analyser_electron_.at(metIndex)->analyse(Event::WPT(signalElectron, met),
						fit_variable_values, event->weight());
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
						double weight = event->weight() * bjetWeight * efficiencyCorrection/ numberOfCombinations;
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
		double efficiencyCorrection = event->isRealData() ? 1. : signalElectron->getEfficiencyCorrection(false, Globals::ElectronScaleFactorSystematic, event->runnumber());

		qcdConversionsElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdConversionSelection_->setPrescale(prescale);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			if ( bjetWeight == 0 ) continue;

			histMan_->setCurrentBJetBin(weightIndex);
			qcdConversionsElectronAnalyser_->setScale(bjetWeight * efficiencyCorrection);
			metAnalyserqcdConversionSelection_->setScale(bjetWeight * efficiencyCorrection);

			qcdConversionsElectronAnalyser_->analyse(event);
			qcdConversionsElectronAnalyser_->analyseElectron(signalElectron, event->weight());
			metAnalyserqcdConversionSelection_->analyse(event, signalLepton, jets);

			qcd_conversion_binned_HT_analyser_electron_->setScale(bjetWeight * efficiencyCorrection);
			vector<double> fit_variable_values;
			fit_variable_values.push_back(fabs(signalElectron->eta()));
			fit_variable_values.push_back(Event::M3(jets));
			fit_variable_values.push_back(Event::M_bl(bJets, signalElectron));
			fit_variable_values.push_back(Event::angle_bl(bJets, signalElectron));
			qcd_conversion_binned_HT_analyser_electron_->analyse(Event::HT(jets), fit_variable_values, event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				const METPointer met(event->MET((METAlgorithm::value) metIndex));

				qcd_conversion_binned_MET_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_conversion_binned_MET_analyser_electron_.at(metIndex)->analyse(met->et(),
						fit_variable_values, event->weight());

				qcd_conversion_binned_ST_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_conversion_binned_ST_analyser_electron_.at(metIndex)->analyse(
						Event::ST(jets, signalElectron, met), fit_variable_values, event->weight());

				qcd_conversion_binned_MT_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_conversion_binned_MT_analyser_electron_.at(metIndex)->analyse(Event::MT(signalElectron, met),
						fit_variable_values, event->weight());

				qcd_conversion_binned_WPT_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_conversion_binned_WPT_analyser_electron_.at(metIndex)->analyse(Event::WPT(signalElectron, met),
						fit_variable_values, event->weight());

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
						double weight = event->weight() * bjetWeight * efficiencyCorrection/ numberOfCombinations;
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
		double efficiencyCorrection = event->isRealData() ? 1. : signalElectron->getEfficiencyCorrection(false, Globals::ElectronScaleFactorSystematic, event->runnumber());

		qcdEPlusjetsPFRelIsoElectronAnalyser_->setPrescale(prescale);
		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			if ( bjetWeight == 0 ) continue;

			histMan_->setCurrentBJetBin(weightIndex);
			qcdEPlusjetsPFRelIsoElectronAnalyser_->setScale(bjetWeight * efficiencyCorrection);

			qcdEPlusjetsPFRelIsoElectronAnalyser_->analyse(event);
			qcdEPlusjetsPFRelIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());
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
		histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/Ref selection");

		const LeptonPointer signalLepton = topMuplusJetsRefSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(false, Globals::MuonScaleFactorSystematic, event->runnumber());

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);

			if ( bjetWeight == 0 ) {
				if ( weightIndex == bjetWeights.size() -1 )	std::cout << "Weight of ZERO" << std::endl;
				continue;
			}

			histMan_->setCurrentBJetBin(weightIndex);
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

			ref_selection_binned_HT_analyser_muon_->setScale(bjetWeight * efficiencyCorrection);
			vector<double> fit_variable_values;
			fit_variable_values.push_back(fabs(signalMuon->eta()));
			fit_variable_values.push_back(Event::M3(jets));
			fit_variable_values.push_back(Event::M_bl(bJets, signalMuon));
			fit_variable_values.push_back(Event::angle_bl(bJets, signalMuon));
			ref_selection_binned_HT_analyser_muon_->analyse(Event::HT(jets), fit_variable_values, event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				const METPointer met(event->MET((METAlgorithm::value) metIndex));
				ref_selection_binned_MET_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				ref_selection_binned_MET_analyser_muon_.at(metIndex)->analyse(met->et(), fit_variable_values,
						event->weight());

				ref_selection_binned_ST_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				ref_selection_binned_ST_analyser_muon_.at(metIndex)->analyse(Event::ST(jets, signalMuon, met),
						fit_variable_values, event->weight());

				ref_selection_binned_MT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				ref_selection_binned_MT_analyser_muon_.at(metIndex)->analyse(Event::MT(signalMuon, met),
						fit_variable_values, event->weight());

				ref_selection_binned_WPT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				ref_selection_binned_WPT_analyser_muon_.at(metIndex)->analyse(Event::WPT(signalMuon, met),
						fit_variable_values, event->weight());
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
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(true, Globals::MuonScaleFactorSystematic, event->runnumber());

		qcdNonIsoMuonAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoMuonSelection_->setPrescale(prescale);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			if ( bjetWeight == 0 ) continue;

			histMan_->setCurrentBJetBin(weightIndex);
			qcdNonIsoMuonAnalyser_->setScale(bjetWeight * efficiencyCorrection);
			metAnalyserqcdNonIsoMuonSelection_->setScale(bjetWeight * efficiencyCorrection);

			qcdNonIsoMuonAnalyser_->analyse(event);
			qcdNonIsoMuonAnalyser_->analyseMuon(signalMuon, event->weight());
			metAnalyserqcdNonIsoMuonSelection_->analyse(event, signalLepton, jets);

			qcd_noniso_binned_HT_analyser_muon_->setScale(bjetWeight * efficiencyCorrection);
			vector<double> fit_variable_values;
			fit_variable_values.push_back(fabs(signalMuon->eta()));
			fit_variable_values.push_back(Event::M3(jets));
			fit_variable_values.push_back(Event::M_bl(bJets, signalMuon));
			fit_variable_values.push_back(Event::angle_bl(bJets, signalMuon));
			qcd_noniso_binned_HT_analyser_muon_->analyse(Event::HT(jets), fit_variable_values, event->weight());

			for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
				if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
					continue;
				bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
				//skip MC only MET entries
				if (isMCOnlyMET && event->isRealData())
					continue;
				string metPrefix = METAlgorithm::names.at(metIndex);
				const METPointer met(event->MET((METAlgorithm::value) metIndex));
				qcd_noniso_binned_MET_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_noniso_binned_MET_analyser_muon_.at(metIndex)->analyse(met->et(), fit_variable_values,
						event->weight());

				qcd_noniso_binned_ST_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_noniso_binned_ST_analyser_muon_.at(metIndex)->analyse(Event::ST(jets, signalMuon, met),
						fit_variable_values, event->weight());

				qcd_noniso_binned_MT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_noniso_binned_MT_analyser_muon_.at(metIndex)->analyse(Event::MT(signalMuon, met),
						fit_variable_values, event->weight());

				qcd_noniso_binned_WPT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
				qcd_noniso_binned_WPT_analyser_muon_.at(metIndex)->analyse(Event::WPT(signalMuon, met),
						fit_variable_values, event->weight());

			}
			//bbar analysis part
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets ge3j");
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
	//selection with respect to reference selection and ge4 jets
	if (qcd_noniso_muon_plus_jets_selection_ge4j_->passesSelectionUpToStep(event,
			TTbarMuPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		const JetCollection jets(qcd_noniso_muon_plus_jets_selection_ge4j_->cleanedJets(event));
		const JetCollection bJets(qcd_noniso_muon_plus_jets_selection_ge4j_->cleanedBJets(event));
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
		unsigned int prescale(qcd_noniso_muon_plus_jets_selection_ge4j_->prescale(event));
		const LeptonPointer signalLepton = qcd_noniso_muon_plus_jets_selection_ge4j_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(true, Globals::MuonScaleFactorSystematic, event->runnumber());

		qcdNonIsoElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoElectronSelection_->setPrescale(prescale);

		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			if ( bjetWeight == 0 ) continue;

			histMan_->setCurrentBJetBin(weightIndex);
			qcdNonIsoMuonAnalyser_ge4j_->setScale(bjetWeight * efficiencyCorrection);
			metAnalyserqcdNonIsoMuonSelection_ge4j_->setScale(bjetWeight * efficiencyCorrection);

			qcdNonIsoMuonAnalyser_ge4j_->analyse(event);
			qcdNonIsoMuonAnalyser_ge4j_->analyseMuon(signalMuon, event->weight());
			metAnalyserqcdNonIsoMuonSelection_ge4j_->analyse(event, signalLepton, jets);

			//bbar analysis part
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets ge4j");
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
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(true, Globals::MuonScaleFactorSystematic, event->runnumber());

		qcdMuPlusjetsPFRelIsoMuonAnalyser_->setPrescale(prescale);
		for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
			double bjetWeight = bjetWeights.at(weightIndex);
			if ( bjetWeight == 0 ) continue;

			histMan_->setCurrentBJetBin(weightIndex);
			qcdMuPlusjetsPFRelIsoMuonAnalyser_->setScale(bjetWeight * efficiencyCorrection);

			qcdMuPlusjetsPFRelIsoMuonAnalyser_->analyse(event);
			qcdMuPlusjetsPFRelIsoMuonAnalyser_->analyseMuon(signalMuon, event->weight());
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
	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets ge3j");
	histMan_->addH1D_BJetBinned("bjet_invariant_mass", "Invariant mass of 2 b-jets; m(b-jet, b-jet); Events", 5000, 0,
			5000);
	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets ge4j");
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
	qcdNonIsoMuonAnalyser_ge4j_->createHistograms();
	metAnalyserqcdNonIsoMuonSelection_->createHistograms();
	metAnalyserqcdNonIsoMuonSelection_ge4j_->createHistograms();

	qcdConversionsElectronAnalyser_->createHistograms();
	metAnalyserqcdConversionSelection_->createHistograms();

	qcdEPlusjetsPFRelIsoElectronAnalyser_->createHistograms();
	qcdMuPlusjetsPFRelIsoMuonAnalyser_->createHistograms();

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
		qcd_noniso_muon_plus_jets_selection_(new QCDNonIsolatedMuonSelection()), //
		qcd_noniso_muon_plus_jets_selection_ge4j_(new QCDNonIsolatedMuonSelection()), //
		qcdPFRelIsoEPlusJetsSelection_(new QCDPFRelIsoEPlusJetsSelection()), //
		qcdPFRelIsoMuPlusJetsSelection_(new QCDPFRelIsoMuPlusJetsSelection()), //
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
				new METAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets ge3j/MET")), //
		metAnalyserqcdNonIsoMuonSelection_ge4j_(
				new METAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets ge4j/MET")), //
		qcdNonIsoElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/EPlusJets/QCD non iso e+jets/Electron", true)), //
		qcdNonIsoMuonAnalyser_(
				new MuonAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets ge3j/Muon", true)), //
		qcdNonIsoMuonAnalyser_ge4j_(
				new MuonAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets ge4j/Muon", true)), //
		metAnalyserqcdConversionSelection_(new METAnalyser(histMan, histogramFolder + "/EPlusJets/QCDConversions/MET")), //
		qcdConversionsElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/EPlusJets/QCDConversions/Electron", true)), //
		qcdEPlusjetsPFRelIsoElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/EPlusJets/QCD e+jets PFRelIso/Electron", true)), //
		qcdMuPlusjetsPFRelIsoMuonAnalyser_(
				new MuonAnalyser(histMan, histogramFolder + "/MuPlusJets/QCD mu+jets PFRelIso ge3j/Muon", true)), //
		metBins_(), //
		ht_bins_(), //
		st_bins_(), //
		mt_bins_(), //
		wpt_bins_(), //
		//MET
		ref_selection_binned_MET_analyser_electron_(), //
		qcd_conversion_binned_MET_analyser_electron_(), //
		qcd_noniso_binned_MET_analyser_electron_(), //
		ref_selection_binned_MET_analyser_muon_(), //
		qcd_noniso_binned_MET_analyser_muon_(), //

		//HT
		ref_selection_binned_HT_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_HT_Analysis")), //
		qcd_conversion_binned_HT_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_HT_Analysis")), //
		qcd_noniso_binned_HT_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_HT_Analysis")), //
		//HT muons
		ref_selection_binned_HT_analyser_muon_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_HT_Analysis")), //
		qcd_noniso_binned_HT_analyser_muon_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets ge3j/Binned_HT_Analysis")), //
		//ST
		ref_selection_binned_ST_analyser_electron_(), //
		qcd_conversion_binned_ST_analyser_electron_(), //
		qcd_noniso_binned_ST_analyser_electron_(), //
		//ST
		ref_selection_binned_ST_analyser_muon_(), //
		qcd_noniso_binned_ST_analyser_muon_(), //
		//MT electrons
		ref_selection_binned_MT_analyser_electron_(), //
		qcd_conversion_binned_MT_analyser_electron_(), //
		qcd_noniso_binned_MT_analyser_electron_(), //
		//MT muons
		ref_selection_binned_MT_analyser_muon_(), //
		qcd_noniso_binned_MT_analyser_muon_(), //
		//WPT electrons
		ref_selection_binned_WPT_analyser_electron_(), //
		qcd_conversion_binned_WPT_analyser_electron_(), //
		qcd_noniso_binned_WPT_analyser_electron_(), //
		//WPT muons
		ref_selection_binned_WPT_analyser_muon_(), //
		qcd_noniso_binned_WPT_analyser_muon_(), //
		jetAnalyserEPlusJetsRefSelection_(new JetAnalyser(histMan, histogramFolder + "/EPlusJets/Ref selection/Jets")), //
		jetAnalyserMuPlusJetsRefSelection_(
				new JetAnalyser(histMan, histogramFolder + "/MuPlusJets/Ref selection/Jets")), //
		electron_variables_(), //
		muon_variables_() {

	//bins: start is always assumed to be 0 and the last is read as > X
	//MET bins:
	// very old: 25, 45, 70, 100, 150, inf
	// old: [0.0, 31.0, 58.0, 96.0, 142.0, 191.0, inf]
	// new: [0.0, 27.0, 52.0, 87.0, 129.0, 171.0, inf]
	metBins_.push_back(27.);
	metBins_.push_back(52.);
	metBins_.push_back(87.);
	metBins_.push_back(129.);
	metBins_.push_back(171.);
	//HT:
	// very old: 50, 150, 250, 350, 450, 650, 1100, inf
	// old: [0.0, 190.0, 225.0, 262.0, 302.0, 345.0, 392.0, 445.0, 501.0, 562.0, 623.0, 689.0, 766.0, inf]
	// new: [0.0, 187.0, 217.0, 250.0, 287.0, 327.0, 369.0, 414.0, 464.0, 517.0, 575.0, 634.0, 696.0, 772.0, inf]
	ht_bins_.push_back(187.0);
	ht_bins_.push_back(217.0);
	ht_bins_.push_back(250.0);
	ht_bins_.push_back(287.0);
	ht_bins_.push_back(327.0);
	ht_bins_.push_back(369.0);
	ht_bins_.push_back(414.0);
	ht_bins_.push_back(464.0);
	ht_bins_.push_back(517.0);
	ht_bins_.push_back(575.0);
	ht_bins_.push_back(634.0);
	ht_bins_.push_back(696.0);
	ht_bins_.push_back(772.0);

	//ST:
	// very old: 150, 250, 350, 450, 550, 750, 1250, inf new: 350 & 400 & 450 & 500 & 580 & 700 & inf
	// old: [0.0, 285.0, 329.0, 376.0, 428.0, 484.0, 544.0, 609.0, 678.0, 751.0, 830.0, 911.0, 1028.0, inf]
	// new: [0.0, 281.0, 324.0, 367.0, 415.0, 466.0, 521.0, 581.0, 645.0, 714.0, 783.0, 861.0, 946.0, inf]
	st_bins_.push_back(281.0);
	st_bins_.push_back(324.0);
	st_bins_.push_back(367.0);
	st_bins_.push_back(415.0);
	st_bins_.push_back(466.0);
	st_bins_.push_back(521.0);
	st_bins_.push_back(581.0);
	st_bins_.push_back(645.0);
	st_bins_.push_back(714.0);
	st_bins_.push_back(783.0);
	st_bins_.push_back(861.0);
	st_bins_.push_back(946.0);

	//MT:
	// very old: 0, 40, 65, 85, 150, inf
	// old: [0.0, 28.0, 66.0, inf]
	// new: [0.0, 23.0, 58.0, inf]
	mt_bins_.push_back(23.0);
	mt_bins_.push_back(58.0);

	//WPT:
	// very old: 0, 40, 70, 100, 130, 170 inf
	// old: [0.0, 31.0, 59.0, 88.0, 118.0, 151.0, 187.0, 227.0, 267.0, inf]
	// new: [0.0, 27.0, 52.0, 78.0, 105.0, 134.0, 165.0, 198.0, 235.0, inf]
	wpt_bins_.push_back(27.0);
	wpt_bins_.push_back(52.0);
	wpt_bins_.push_back(78.0);
	wpt_bins_.push_back(105.0);
	wpt_bins_.push_back(134.0);
	wpt_bins_.push_back(165.0);
	wpt_bins_.push_back(198.0);
	wpt_bins_.push_back(235.0);

	Variable electron_absolute_eta = Variable("electron_absolute_eta", 30, 0.0, 3.0);
	Variable muon_absolute_eta = Variable("muon_absolute_eta", 30, 0.0, 3.0);
	Variable M3 = Variable("M3", 200, 0.0, 1000.);
	Variable M_bl = Variable("M_bl", 200, 0.0, 1000.);
	Variable angle_bl = Variable("angle_bl", 40, 0, 4.);
	electron_variables_.push_back(electron_absolute_eta);
	muon_variables_.push_back(muon_absolute_eta);
	electron_variables_.push_back(M3);
	muon_variables_.push_back(M3);
	electron_variables_.push_back(M_bl);
	muon_variables_.push_back(M_bl);
	electron_variables_.push_back(angle_bl);
	muon_variables_.push_back(angle_bl);

	make_binned_MET_analysers();
	make_binned_HT_analysers();
	make_binned_ST_analysers();
	make_binned_MT_analysers();
	make_binned_WPT_analysers();
}

TTbar_plus_X_analyser::~TTbar_plus_X_analyser() {
}

void TTbar_plus_X_analyser::make_binned_MET_analysers() {
	//for all MET types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			continue;
		string metPrefix = METAlgorithm::names.at(metIndex);

		Binned_Variable_analyser_ptr ref_selection_binned_met_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_conversion_binned_met_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_met_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_MET_Analysis"));

		Variable MET = Variable(metPrefix, metBins_);
		ref_selection_binned_met_electron_analyser->set_variables(MET, electron_variables_);
		qcd_conversion_binned_met_electron_analyser->set_variables(MET, electron_variables_);
		qcd_noniso_binned_met_electron_analyser->set_variables(MET, electron_variables_);

		ref_selection_binned_MET_analyser_electron_.push_back(ref_selection_binned_met_electron_analyser);
		qcd_conversion_binned_MET_analyser_electron_.push_back(qcd_conversion_binned_met_electron_analyser);
		qcd_noniso_binned_MET_analyser_electron_.push_back(qcd_noniso_binned_met_electron_analyser);

		//binned MET muons
		Binned_Variable_analyser_ptr ref_selection_binned_met_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_met_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets ge3j/Binned_MET_Analysis"));

		ref_selection_binned_met_muon_analyser->set_variables(MET, muon_variables_);
		qcd_noniso_binned_met_muon_analyser->set_variables(MET, muon_variables_);

		ref_selection_binned_MET_analyser_muon_.push_back(ref_selection_binned_met_muon_analyser);
		qcd_noniso_binned_MET_analyser_muon_.push_back(qcd_noniso_binned_met_muon_analyser);

	}
}

void TTbar_plus_X_analyser::make_binned_HT_analysers() {
	// multi-variable analysers
	Variable HT = Variable("HT", ht_bins_);
	ref_selection_binned_HT_analyser_electron_->set_variables(HT, electron_variables_);
	ref_selection_binned_HT_analyser_muon_->set_variables(HT, muon_variables_);

	qcd_noniso_binned_HT_analyser_electron_->set_variables(HT, electron_variables_);
	qcd_noniso_binned_HT_analyser_muon_->set_variables(HT, muon_variables_);

	qcd_conversion_binned_HT_analyser_electron_->set_variables(HT, electron_variables_);
}

void TTbar_plus_X_analyser::make_binned_ST_analysers() {
	//for all ST types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			continue;
		string metPrefix = METAlgorithm::names.at(metIndex);
		Variable ST = Variable("ST_with_" + metPrefix, st_bins_);
		Binned_Variable_analyser_ptr ref_selection_binned_ST_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_conversion_binned_ST_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_ST_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_ST_Analysis"));
		ref_selection_binned_ST_electron_analyser->set_variables(ST, electron_variables_);

		qcd_conversion_binned_ST_electron_analyser->set_variables(ST, electron_variables_);
		qcd_noniso_binned_ST_electron_analyser->set_variables(ST, electron_variables_);
		ref_selection_binned_ST_analyser_electron_.push_back(ref_selection_binned_ST_electron_analyser);
		qcd_conversion_binned_ST_analyser_electron_.push_back(qcd_conversion_binned_ST_electron_analyser);
		qcd_noniso_binned_ST_analyser_electron_.push_back(qcd_noniso_binned_ST_electron_analyser);

		//binned ST muons
		Binned_Variable_analyser_ptr ref_selection_binned_ST_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_ST_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets ge3j/Binned_ST_Analysis"));

		ref_selection_binned_ST_muon_analyser->set_variables(ST, muon_variables_);
		qcd_noniso_binned_ST_muon_analyser->set_variables(ST, muon_variables_);

		ref_selection_binned_ST_analyser_muon_.push_back(ref_selection_binned_ST_muon_analyser);
		qcd_noniso_binned_ST_analyser_muon_.push_back(qcd_noniso_binned_ST_muon_analyser);

	}
}

void TTbar_plus_X_analyser::make_binned_MT_analysers() {
	//for all MT types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			continue;
		string metPrefix = METAlgorithm::names.at(metIndex);
		Variable MT = Variable("MT_with_" + metPrefix, mt_bins_);
		Binned_Variable_analyser_ptr ref_selection_binned_MT_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_conversion_binned_MT_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_MT_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_MT_Analysis"));
		ref_selection_binned_MT_electron_analyser->set_variables(MT, electron_variables_);
		qcd_conversion_binned_MT_electron_analyser->set_variables(MT, electron_variables_);
		qcd_noniso_binned_MT_electron_analyser->set_variables(MT, electron_variables_);

		ref_selection_binned_MT_analyser_electron_.push_back(ref_selection_binned_MT_electron_analyser);
		qcd_conversion_binned_MT_analyser_electron_.push_back(qcd_conversion_binned_MT_electron_analyser);
		qcd_noniso_binned_MT_analyser_electron_.push_back(qcd_noniso_binned_MT_electron_analyser);

		//binned MT muons
		Binned_Variable_analyser_ptr ref_selection_binned_MT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_MT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets ge3j/Binned_MT_Analysis"));

		ref_selection_binned_MT_muon_analyser->set_variables(MT, muon_variables_);
		qcd_noniso_binned_MT_muon_analyser->set_variables(MT, muon_variables_);

		ref_selection_binned_MT_analyser_muon_.push_back(ref_selection_binned_MT_muon_analyser);
		qcd_noniso_binned_MT_analyser_muon_.push_back(qcd_noniso_binned_MT_muon_analyser);

	}
}

void TTbar_plus_X_analyser::make_binned_WPT_analysers() {
	//for all WPT types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			continue;
		string metPrefix = METAlgorithm::names.at(metIndex);
		Variable WPT = Variable("WPT_with_" + metPrefix, wpt_bins_);
		Binned_Variable_analyser_ptr ref_selection_binned_WPT_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_WPT_Analysis"));
		Binned_Variable_analyser_ptr qcd_conversion_binned_WPT_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_WPT_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_WPT_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_WPT_Analysis"));
		ref_selection_binned_WPT_electron_analyser->set_variables(WPT, electron_variables_);
		qcd_conversion_binned_WPT_electron_analyser->set_variables(WPT, electron_variables_);
		qcd_noniso_binned_WPT_electron_analyser->set_variables(WPT, electron_variables_);
		ref_selection_binned_WPT_analyser_electron_.push_back(ref_selection_binned_WPT_electron_analyser);
		qcd_conversion_binned_WPT_analyser_electron_.push_back(qcd_conversion_binned_WPT_electron_analyser);
		qcd_noniso_binned_WPT_analyser_electron_.push_back(qcd_noniso_binned_WPT_electron_analyser);

		//binned WPT muons
		Binned_Variable_analyser_ptr no_selection_binned_WPT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/No selection/Binned_WPT_Analysis"));
		Binned_Variable_analyser_ptr ref_selection_binned_WPT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_WPT_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_WPT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets ge3j/Binned_WPT_Analysis"));

		no_selection_binned_WPT_muon_analyser->set_variables(WPT, muon_variables_);
		ref_selection_binned_WPT_muon_analyser->set_variables(WPT, muon_variables_);
		qcd_noniso_binned_WPT_muon_analyser->set_variables(WPT, muon_variables_);

		ref_selection_binned_WPT_analyser_muon_.push_back(ref_selection_binned_WPT_muon_analyser);
		qcd_noniso_binned_WPT_analyser_muon_.push_back(qcd_noniso_binned_WPT_muon_analyser);

	}
}

} /* namespace BAT */
