/*
 * MTtbarAnalyser.cpp
 *
 *  Created on: 5 Sep 2011
 *      Author: kreczko
 */
//#include "../../interface/TopPairEventCandidate.h"
#include "../../interface/Analysers/MTtbarAnalyser.h"
#include "../../interface/ReconstructionModules/ChiSquaredBasedTopPairReconstruction.h"
#include "../../interface/ReconstructionModules/ChiSquaredThreeJetsTopPairReconstruction.h"
#include "../../interface/ReconstructionModules/TopMassDifferenceTTbarReco.h"
#include "../../interface/BTagWeight.h"

#include <boost/scoped_ptr.hpp>

namespace BAT {

void MTtbarAnalyser::analyse(const EventPtr event) {
	weight_ = event->weight();
	//TODO: add BJet weight_s
	analyseFourJetChi2(event);
	analyseThreeJetChi2(event);
	analyseFourJetTopMassDifference(event);
	analyseFourJetChi2QCDBackground(event);
	analyseThreeJetChi2QCDBackground(event);
	analyseFourJetTopMassDifferenceQCDBackground(event);
}

void MTtbarAnalyser::analyseFourJetChi2(const EventPtr event) {
	if (topElectronPlusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		LeptonPointer selectedElectron = topElectronPlusJetsRefSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = topElectronPlusJetsRefSelection_->cleanedJets(event);
		JetCollection bJets = topElectronPlusJetsRefSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/Ref selection/FourJetChi2");
		}
	}

	if (topMuonPlusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		LeptonPointer selectedMuon = topMuonPlusJetsRefSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(selectedMuon));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(false);
		METPointer met = event->MET();
		JetCollection jets = topMuonPlusJetsRefSelection_->cleanedJets(event);
		JetCollection bJets = topMuonPlusJetsRefSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedMuon, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight * efficiencyCorrection;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("MuonPlusJets/Ref selection/FourJetChi2");
		}
	}
}

void MTtbarAnalyser::analyseThreeJetChi2(const EventPtr event) {
	if (topElectronPlusJetsRefSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastThreeGoodJets)) {
		JetCollection jets = topElectronPlusJetsRefSelection_->cleanedJets(event);
		if (jets.size() != 3) //only consider 3 jet events
			return;

		JetCollection bJets = topElectronPlusJetsRefSelection_->cleanedBJets(event);
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

		LeptonPointer selectedElectron = topElectronPlusJetsRefSelection_->signalLepton(event);
		METPointer met = event->MET();

		boost::scoped_ptr<ChiSquaredThreeJetsTopPairReconstruction> chi2Reco(
				new ChiSquaredThreeJetsTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/Ref selection/ThreeJetChi2");
		}
	}

	if (topMuonPlusJetsRefSelection_->passesSelectionUpToStep(event,
			TTbarMuPlusJetsReferenceSelection::AtLeastThreeGoodJets)) {
		JetCollection jets = topMuonPlusJetsRefSelection_->cleanedJets(event);
		if (jets.size() != 3) //only consider 3 jet events
			return;

		JetCollection bJets = topMuonPlusJetsRefSelection_->cleanedBJets(event);
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

		LeptonPointer selectedMuon = topMuonPlusJetsRefSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(selectedMuon));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(false);
		METPointer met = event->MET();

		boost::scoped_ptr<ChiSquaredThreeJetsTopPairReconstruction> chi2Reco(
				new ChiSquaredThreeJetsTopPairReconstruction(selectedMuon, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight * efficiencyCorrection;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("MuonPlusJets/Ref selection/ThreeJetChi2");
		}
	}
}

void MTtbarAnalyser::analyseFourJetTopMassDifference(const EventPtr event) {
	if (topElectronPlusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		LeptonPointer selectedElectron = topElectronPlusJetsRefSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = topElectronPlusJetsRefSelection_->cleanedJets(event);
		JetCollection bJets = topElectronPlusJetsRefSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<TopMassDifferenceTTbarReco> topMassDiffReco(
				new TopMassDifferenceTTbarReco(selectedElectron, met, jets));
		if (!topMassDiffReco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << topMassDiffReco->getDetailsOnFailure();
			return;
		}
		allSolutions = topMassDiffReco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/Ref selection/FourJetTopMassDifference");
		}
	}

	if (topMuonPlusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		LeptonPointer selectedMuon = topMuonPlusJetsRefSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(selectedMuon));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(false);
		METPointer met = event->MET();
		JetCollection jets = topMuonPlusJetsRefSelection_->cleanedJets(event);
		JetCollection bJets = topMuonPlusJetsRefSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<TopMassDifferenceTTbarReco> topMassDiffReco(
				new TopMassDifferenceTTbarReco(selectedMuon, met, jets));
		if (!topMassDiffReco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << topMassDiffReco->getDetailsOnFailure();
			return;
		}
		allSolutions = topMassDiffReco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight * efficiencyCorrection;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("MuonPlusJets/Ref selection/FourJetTopMassDifference");
		}
	}
}

void MTtbarAnalyser::analyseFourJetChi2QCDBackground(const EventPtr event) {
	if (qcdElectronConversionSelection_->passesFullSelectionExceptLastTwoSteps(event)) { //passes all except b-tag!
		LeptonPointer selectedElectron = qcdElectronConversionSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = qcdElectronConversionSelection_->cleanedJets(event);
		JetCollection bJets = qcdElectronConversionSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/QCDConversions/FourJetChi2");
		}
	}

	if (qcdElectronNonIsoSelection_->passesFullSelectionExceptLastTwoSteps(event)) { //passes all except b-tag!
		LeptonPointer selectedElectron = qcdElectronNonIsoSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = qcdElectronNonIsoSelection_->cleanedJets(event);
		JetCollection bJets = qcdElectronNonIsoSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/QCD non iso e+jets/FourJetChi2");
		}
	}

	if (qcdElectronAntiIDSelection_->passesFullSelectionExceptLastTwoSteps(event)) { //passes all except b-tag!
		LeptonPointer selectedElectron = qcdElectronAntiIDSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = qcdElectronAntiIDSelection_->cleanedJets(event);
		JetCollection bJets = qcdElectronAntiIDSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/QCDAntiID/FourJetChi2");
		}
	}

	if (qcdMuonNonIsoSelection_->passesFullSelectionExceptLastTwoSteps(event)) { //passes all except b-tag!
		LeptonPointer selectedMuon = qcdMuonNonIsoSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(selectedMuon));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(true);
		METPointer met = event->MET();
		JetCollection jets = qcdMuonNonIsoSelection_->cleanedJets(event);
		JetCollection bJets = qcdMuonNonIsoSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedMuon, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}

		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight * efficiencyCorrection;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("MuonPlusJets/QCD non iso mu+jets/FourJetChi2");
		}
	}
}

void MTtbarAnalyser::analyseFourJetTopMassDifferenceQCDBackground(const EventPtr event) {
	if (qcdElectronConversionSelection_->passesFullSelectionExceptLastTwoSteps(event)) { //passes all except b-tag!
		LeptonPointer selectedElectron = qcdElectronConversionSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = qcdElectronConversionSelection_->cleanedJets(event);
		JetCollection bJets = qcdElectronConversionSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<TopMassDifferenceTTbarReco> chi2Reco(
				new TopMassDifferenceTTbarReco(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/QCDConversions/FourJetTopMassDifference");
		}
	}

	if (qcdElectronNonIsoSelection_->passesFullSelectionExceptLastTwoSteps(event)) { //passes all except b-tag!
		LeptonPointer selectedElectron = qcdElectronNonIsoSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = qcdElectronNonIsoSelection_->cleanedJets(event);
		JetCollection bJets = qcdElectronNonIsoSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<TopMassDifferenceTTbarReco> chi2Reco(
				new TopMassDifferenceTTbarReco(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/QCD non iso e+jets/FourJetTopMassDifference");
		}
	}

	if (qcdElectronAntiIDSelection_->passesFullSelectionExceptLastTwoSteps(event)) { //passes all except b-tag!
		LeptonPointer selectedElectron = qcdElectronAntiIDSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = qcdElectronAntiIDSelection_->cleanedJets(event);
		JetCollection bJets = qcdElectronAntiIDSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<TopMassDifferenceTTbarReco> chi2Reco(
				new TopMassDifferenceTTbarReco(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/QCDAntiID/FourJetTopMassDifference");
		}
	}

	if (qcdMuonNonIsoSelection_->passesFullSelectionExceptLastTwoSteps(event)) { //passes all except b-tag!
		LeptonPointer selectedMuon = qcdMuonNonIsoSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(selectedMuon));
		double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(true);
		METPointer met = event->MET();
		JetCollection jets = qcdMuonNonIsoSelection_->cleanedJets(event);
		JetCollection bJets = qcdMuonNonIsoSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<TopMassDifferenceTTbarReco> chi2Reco(new TopMassDifferenceTTbarReco(selectedMuon, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight * efficiencyCorrection;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("MuonPlusJets/QCD non iso mu+jets/FourJetTopMassDifference");
		}
	}
}

void MTtbarAnalyser::analyseThreeJetChi2QCDBackground(const EventPtr event) {
	if (qcdElectronConversionSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastThreeGoodJets)) { //passes all except b-tag!
		LeptonPointer selectedElectron = qcdElectronConversionSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = qcdElectronConversionSelection_->cleanedJets(event);
		if (jets.size() != 3) //only consider 3 jet events
			return;
		JetCollection bJets = qcdElectronConversionSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<ChiSquaredThreeJetsTopPairReconstruction> chi2Reco(
				new ChiSquaredThreeJetsTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/QCDConversions/ThreeJetChi2");
		}
	}

	if (qcdElectronNonIsoSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastThreeGoodJets)) { //passes all except b-tag!
		LeptonPointer selectedElectron = qcdElectronNonIsoSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = qcdElectronNonIsoSelection_->cleanedJets(event);
		if (jets.size() != 3) //only consider 3 jet events
			return;
		JetCollection bJets = qcdElectronNonIsoSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<ChiSquaredThreeJetsTopPairReconstruction> chi2Reco(
				new ChiSquaredThreeJetsTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/QCD non iso e+jets/ThreeJetChi2");
		}
	}

	if (qcdElectronAntiIDSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastThreeGoodJets)) { //passes all except b-tag!
		LeptonPointer selectedElectron = qcdElectronAntiIDSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = qcdElectronAntiIDSelection_->cleanedJets(event);
		if (jets.size() != 3) //only consider 3 jet events
			return;
		JetCollection bJets = qcdElectronAntiIDSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<ChiSquaredThreeJetsTopPairReconstruction> chi2Reco(
				new ChiSquaredThreeJetsTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("ElectronPlusJets/QCDAntiID/ThreeJetChi2");
		}
	}

	if (qcdMuonNonIsoSelection_->passesSelectionUpToStep(event,
			TTbarMuPlusJetsReferenceSelection::AtLeastThreeGoodJets)) { //passes all except b-tag!
		LeptonPointer selectedMuon = qcdMuonNonIsoSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(selectedMuon));
				double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(true);
		METPointer met = event->MET();
		JetCollection jets = qcdMuonNonIsoSelection_->cleanedJets(event);
		if (jets.size() != 3) //only consider 3 jet events
			return;
		JetCollection bJets = qcdMuonNonIsoSelection_->cleanedBJets(event);
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

		boost::scoped_ptr<ChiSquaredThreeJetsTopPairReconstruction> chi2Reco(
				new ChiSquaredThreeJetsTopPairReconstruction(selectedMuon, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		for (unsigned int weight_Index = 0; weight_Index < bjetWeights.size(); ++weight_Index) {
			double bjetWeight = bjetWeights.at(weight_Index);
			scale_ = bjetWeight * efficiencyCorrection;
			histMan_->setCurrentBJetBin(weight_Index);
			fillHistograms("MuonPlusJets/QCD non iso mu+jets/ThreeJetChi2");
		}
	}
}

void MTtbarAnalyser::fillHistograms(std::string subcollection, std::string suffix) {
	histMan_->setCurrentHistogramFolder("MttbarAnalysis/" + subcollection);

	unsigned short numberOfSolutions = allSolutions.size();
	for (unsigned short index = 0; index < numberOfSolutions; ++index) {
		TtbarHypothesisPointer currentSolution = allSolutions.at(index);
		const ParticlePointer resonance = currentSolution->resonance;
		double mttbar = resonance->mass();
		double ttbarPt = resonance->pt();
		double m3 = currentSolution->M3();
		double hadronicTopMass = currentSolution->hadronicTop->mass();
		double leptonicTopMass = currentSolution->leptonicTop->mass();
		double hadronicWMass = currentSolution->hadronicW->mass();
		double leptonicWMass = currentSolution->leptonicW->mass();
		double disc = currentSolution->discriminator;

		if (index == 0) {
			histMan_->H1D_BJetBinned("mttbar" + suffix)->Fill(mttbar, weight_ * scale_);
			histMan_->H1D_BJetBinned("ttbar_pt" + suffix)->Fill(ttbarPt, weight_ * scale_);
			histMan_->H1D_BJetBinned("m3" + suffix)->Fill(m3, weight_ * scale_);
			histMan_->H1D_BJetBinned("leptonicTopMass" + suffix)->Fill(leptonicTopMass, weight_ * scale_);
			histMan_->H1D_BJetBinned("hadronicTopMass" + suffix)->Fill(hadronicTopMass, weight_ * scale_);
			histMan_->H1D_BJetBinned("allTopMasses" + suffix)->Fill(leptonicTopMass, weight_ * scale_);
			histMan_->H1D_BJetBinned("allTopMasses" + suffix)->Fill(hadronicTopMass, weight_ * scale_);

			histMan_->H1D_BJetBinned("leptonicWMass" + suffix)->Fill(leptonicWMass, weight_ * scale_);
			histMan_->H1D_BJetBinned("hadronicWMass" + suffix)->Fill(hadronicWMass, weight_ * scale_);

			histMan_->H1D_BJetBinned("discriminant" + suffix)->Fill(disc, weight_ * scale_);
		}

		double normalisedWeight = weight_ * scale_ / numberOfSolutions;
		histMan_->H1D_BJetBinned("mttbar_allSolutions" + suffix)->Fill(mttbar, normalisedWeight);
		histMan_->H1D_BJetBinned("ttbar_pt_allSolutions" + suffix)->Fill(ttbarPt, normalisedWeight);
		histMan_->H1D_BJetBinned("m3_allSolutions" + suffix)->Fill(m3, normalisedWeight);
		histMan_->H1D_BJetBinned("leptonicTopMass_allSolutions" + suffix)->Fill(leptonicTopMass, normalisedWeight);
		histMan_->H1D_BJetBinned("hadronicTopMass_allSolutions" + suffix)->Fill(hadronicTopMass, normalisedWeight);
		histMan_->H1D_BJetBinned("allTopMasses_allSolutions" + suffix)->Fill(leptonicTopMass, normalisedWeight);
		histMan_->H1D_BJetBinned("allTopMasses_allSolutions" + suffix)->Fill(hadronicTopMass, normalisedWeight);
		histMan_->H1D_BJetBinned("leptonicWMass_allSolutions" + suffix)->Fill(leptonicWMass, normalisedWeight);
		histMan_->H1D_BJetBinned("hadronicWMass_allSolutions" + suffix)->Fill(hadronicWMass, normalisedWeight);

		histMan_->H1D_BJetBinned("discriminant_allSolutions" + suffix)->Fill(disc, normalisedWeight);

	}
}

void MTtbarAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder("MttbarAnalysis");

	createHistogramsFor("MuonPlusJets/Ref selection/FourJetChi2");
	createHistogramsFor("MuonPlusJets/Ref selection/ThreeJetChi2");
	createHistogramsFor("MuonPlusJets/Ref selection/FourJetTopMassDifference");
//	createHistogramsFor("MuonPlusJets/FourJetChi2MC"); //for input values!
	createHistogramsFor("MuonPlusJets/QCD non iso mu+jets/FourJetChi2");
	createHistogramsFor("MuonPlusJets/QCD non iso mu+jets/ThreeJetChi2");
	createHistogramsFor("MuonPlusJets/QCD non iso mu+jets/FourJetTopMassDifference");

	createHistogramsFor("ElectronPlusJets/Ref selection/FourJetChi2");
	createHistogramsFor("ElectronPlusJets/Ref selection/ThreeJetChi2");
	createHistogramsFor("ElectronPlusJets/Ref selection/FourJetTopMassDifference");
//		createHistogramsFor("ElectronPlusJets/FourJetChi2MC"); //for input values!
	createHistogramsFor("ElectronPlusJets/QCDConversions/FourJetChi2");
	createHistogramsFor("ElectronPlusJets/QCDConversions/ThreeJetChi2");
	createHistogramsFor("ElectronPlusJets/QCDConversions/FourJetTopMassDifference");
	createHistogramsFor("ElectronPlusJets/QCD non iso e+jets/FourJetChi2");
	createHistogramsFor("ElectronPlusJets/QCD non iso e+jets/ThreeJetChi2");
	createHistogramsFor("ElectronPlusJets/QCD non iso e+jets/FourJetTopMassDifference");
	createHistogramsFor("ElectronPlusJets/QCDAntiID/FourJetChi2");
	createHistogramsFor("ElectronPlusJets/QCDAntiID/ThreeJetChi2");
	createHistogramsFor("ElectronPlusJets/QCDAntiID/FourJetTopMassDifference");

}

void MTtbarAnalyser::createHistogramsFor(std::string collection) {
	boost::array<std::string, 4> histTypes = { { "", "_allSolutions", } };

	histMan_->setCurrentHistogramFolder("MttbarAnalysis/" + collection);

	for (unsigned short index = 0; index < histTypes.size(); ++index) {
		std::string suffix = histTypes.at(index);
		histMan_->addH1D_BJetBinned("mttbar" + suffix, "m(t#bar{t});m(t#bar{t}) [GeV]; events/1 GeV", 5000, 0, 5000);

		histMan_->addH1D_BJetBinned("leptonicTopMass" + suffix, "leptonic Top Mass; m(t_{lep}) [GeV]; events/1 GeV",
				1000, 0, 1000);
		histMan_->addH1D_BJetBinned("hadronicTopMass" + suffix, "hadronic Top Mass; m(t_{had}) [GeV]; events/1 GeV",
				1000, 0, 1000);
		histMan_->addH1D_BJetBinned("allTopMasses" + suffix, "all Top Masses;m(t) [GeV]; events/1 GeV", 1000, 0, 1000);
		histMan_->addH1D_BJetBinned("m3" + suffix, "m3;m(jjj) [GeV]; events/1 GeV", 5000, 0, 5000);
		histMan_->addH1D_BJetBinned("ttbar_pt" + suffix, "p_{T,t#bar{t}};p_{T,t#bar{t}} [GeV]; events/1 GeV", 1000, 0,
				1000);
		histMan_->addH1D_BJetBinned("leptonicWMass" + suffix, "leptonic W mass; m(W_{lep}) [GeV]; events/1 GeV", 500, 0,
				500);
		histMan_->addH1D_BJetBinned("hadronicWMass" + suffix, "hadronic W mass; m(W_{had}) [GeV]; events/1 GeV", 500, 0,
				500);
		histMan_->addH1D_BJetBinned("discriminant" + suffix, "discriminant;discriminant; events/0.1", 1000, 0, 100);

	}

}

MTtbarAnalyser::MTtbarAnalyser(boost::shared_ptr<HistogramManager> histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		currentType("ElectronPlusJets"), //
		allSolutions(), //
		topElectronPlusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection()), //
		topMuonPlusJetsRefSelection_(new TopPairMuPlusJetsReferenceSelection()), //
		qcdElectronAntiIDSelection_(new QCDAntiIDEPlusJetsSelection()), //
		qcdElectronConversionSelection_(new QCDConversionsSelection()), //
		qcdElectronNonIsoSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdMuonNonIsoSelection_(new QCDNonIsolatedMuonSelection()) {

	qcdElectronAntiIDSelection_->useMVAID(true);
}

MTtbarAnalyser::~MTtbarAnalyser() {
}

} /* namespace BAT */
