/*
 * MTtbarAnalyser.cpp
 *
 *  Created on: 5 Sep 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/MTtbarAnalyser.h"
#include "../../interface/ReconstructionModules/ChiSquaredBasedTopPairReconstruction.h"
#include "../../interface/ReconstructionModules/ChiSquaredThreeJetsTopPairReconstruction.h"
#include "../../interface/ReconstructionModules/TopMassDifferenceTTbarReco.h"
#include <boost/scoped_ptr.hpp>

namespace BAT {

void MTtbarAnalyser::analyse(const TopPairEventCandidate& ttbarEvent) {
	weight = ttbarEvent.weight();
	analyseFourJetChi2(ttbarEvent);
	analyseThreeJetChi2(ttbarEvent);
	analyseFourJetTopMassDifference(ttbarEvent);
	analyseFourJetChi2QCDConversionBackground(ttbarEvent);
}

void MTtbarAnalyser::analyseFourJetChi2(const TopPairEventCandidate& ttbarEvent) {
	string type("ElectronPlusJets");
	histMan->setCurrentJetBin(ttbarEvent.GoodElectronCleanedJets().size());
	histMan->setCurrentBJetBin(ttbarEvent.GoodElectronCleanedBJets().size());

	if (ttbarEvent.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets)) {
		LeptonPointer selectedElectron = ttbarEvent.GoodPFIsolatedElectrons().front();
		METPointer met = ttbarEvent.MET();
		JetCollection jets = ttbarEvent.GoodElectronCleanedJets();

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		fillHistograms(type + "/FourJetChi2");
		if (ttbarEvent.passesMETCut() && ttbarEvent.passesAsymmetricElectronCleanedJetCuts())
			fillHistograms(type + "/FourJetChi2", "_withMETAndAsymJets");
	}

	type = "MuonPlusJets";
	histMan->setCurrentJetBin(ttbarEvent.GoodMuonCleanedJets().size());
	histMan->setCurrentBJetBin(ttbarEvent.GoodMuonCleanedBJets().size());

	if (ttbarEvent.passesMuPlusJetsSelectionStepUpTo(TTbarMuPlusJetsSelection::AtLeastFourGoodJets)) {
		LeptonPointer selectedLepton = ttbarEvent.GoodPFIsolatedMuons().front();
		METPointer met = ttbarEvent.MET();
		JetCollection jets = ttbarEvent.GoodMuonCleanedJets();

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedLepton, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		fillHistograms(type + "/FourJetChi2");
		if (ttbarEvent.passesMETCut() && ttbarEvent.passesAsymmetricMuonCleanedJetCuts())
			fillHistograms(type + "/FourJetChi2", "_withMETAndAsymJets");
	}
	//soon to be removed
	histMan->setCurrentJetBin(ttbarEvent.GoodElectronCleanedJets().size());
	histMan->setCurrentBJetBin(ttbarEvent.GoodElectronCleanedBJets().size());
}

void MTtbarAnalyser::analyseThreeJetChi2(const TopPairEventCandidate& ttbarEvent) {
	string type("ElectronPlusJets");
	histMan->setCurrentJetBin(ttbarEvent.GoodElectronCleanedJets().size());
	histMan->setCurrentBJetBin(ttbarEvent.GoodElectronCleanedBJets().size());

	if (ttbarEvent.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastTwoGoodJets)
			&& ttbarEvent.GoodElectronCleanedJets().size() == 3) {
		LeptonPointer selectedElectron = ttbarEvent.GoodPFIsolatedElectrons().front();
		METPointer met = ttbarEvent.MET();
		JetCollection jets = ttbarEvent.GoodElectronCleanedJets();

		boost::scoped_ptr<ChiSquaredThreeJetsTopPairReconstruction> chi2Reco(
				new ChiSquaredThreeJetsTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		fillHistograms(type + "/ThreeJetChi2");
		if (ttbarEvent.passesMETCut() && ttbarEvent.passesAsymmetricElectronCleanedJetCuts())
			fillHistograms(type + "/ThreeJetChi2", "_withMETAndAsymJets");
	}

	type = "MuonPlusJets";
	histMan->setCurrentJetBin(ttbarEvent.GoodMuonCleanedJets().size());
	histMan->setCurrentBJetBin(ttbarEvent.GoodMuonCleanedBJets().size());

	if (ttbarEvent.passesMuPlusJetsSelectionStepUpTo(TTbarMuPlusJetsSelection::AtLeastTwoGoodJets)
			&& ttbarEvent.GoodMuonCleanedJets().size() == 3) {
		LeptonPointer selectedLepton = ttbarEvent.GoodPFIsolatedMuons().front();
		METPointer met = ttbarEvent.MET();
		JetCollection jets = ttbarEvent.GoodMuonCleanedJets();

		boost::scoped_ptr<ChiSquaredThreeJetsTopPairReconstruction> chi2Reco(
				new ChiSquaredThreeJetsTopPairReconstruction(selectedLepton, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		fillHistograms(type + "/ThreeJetChi2");
		if (ttbarEvent.passesMETCut() && ttbarEvent.passesAsymmetricMuonCleanedJetCuts())
			fillHistograms(type + "/ThreeJetChi2", "_withMETAndAsymJets");
	}

	//soon to be removed
	histMan->setCurrentJetBin(ttbarEvent.GoodElectronCleanedJets().size());
	histMan->setCurrentBJetBin(ttbarEvent.GoodElectronCleanedBJets().size());
}

void MTtbarAnalyser::analyseFourJetTopMassDifference(const TopPairEventCandidate& ttbarEvent) {
	string type("ElectronPlusJets");
	histMan->setCurrentJetBin(ttbarEvent.GoodElectronCleanedJets().size());
	histMan->setCurrentBJetBin(ttbarEvent.GoodElectronCleanedBJets().size());

	if (ttbarEvent.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets)) {
		LeptonPointer selectedElectron = ttbarEvent.GoodPFIsolatedElectrons().front();
		METPointer met = ttbarEvent.MET();
		JetCollection jets = ttbarEvent.GoodElectronCleanedJets();

		boost::scoped_ptr<TopMassDifferenceTTbarReco> topMassDiffReco(
				new TopMassDifferenceTTbarReco(selectedElectron, met, jets));
		if (!topMassDiffReco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << topMassDiffReco->getDetailsOnFailure();
			return;
		}
		allSolutions = topMassDiffReco->getAllSolutions();
		fillHistograms(type + "/FourJetTopMassDifference");
		if (ttbarEvent.passesMETCut() && ttbarEvent.passesAsymmetricElectronCleanedJetCuts())
			fillHistograms(type + "/FourJetTopMassDifference", "_withMETAndAsymJets");
	}

	type = "MuonPlusJets";
	histMan->setCurrentJetBin(ttbarEvent.GoodMuonCleanedJets().size());
	histMan->setCurrentBJetBin(ttbarEvent.GoodMuonCleanedBJets().size());

	if (ttbarEvent.passesMuPlusJetsSelectionStepUpTo(TTbarMuPlusJetsSelection::AtLeastFourGoodJets)) {
		LeptonPointer selectedLepton = ttbarEvent.GoodPFIsolatedMuons().front();
		METPointer met = ttbarEvent.MET();
		JetCollection jets = ttbarEvent.GoodMuonCleanedJets();

		boost::scoped_ptr<TopMassDifferenceTTbarReco> topMassDiffReco(
				new TopMassDifferenceTTbarReco(selectedLepton, met, jets));
		if (!topMassDiffReco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << topMassDiffReco->getDetailsOnFailure();
			return;
		}
		allSolutions = topMassDiffReco->getAllSolutions();
		fillHistograms(type + "/FourJetTopMassDifference");
		if (ttbarEvent.passesMETCut() && ttbarEvent.passesAsymmetricMuonCleanedJetCuts())
			fillHistograms(type + "/FourJetTopMassDifference", "_withMETAndAsymJets");
	}

	//soon to be removed
	histMan->setCurrentJetBin(ttbarEvent.GoodElectronCleanedJets().size());
	histMan->setCurrentBJetBin(ttbarEvent.GoodElectronCleanedBJets().size());

}

void MTtbarAnalyser::analyseFourJetChi2QCDConversionBackground(const TopPairEventCandidate& ttbarEvent) {
	histMan->setCurrentJetBin(ttbarEvent.GoodElectronCleanedJets().size());
	histMan->setCurrentBJetBin(ttbarEvent.GoodElectronCleanedBJets().size());

	if (ttbarEvent.passesConversionSelection() && ttbarEvent.GoodElectronCleanedJets().size() >= 4) {
		LeptonPointer selectedElectron = ttbarEvent.MostPFIsolatedElectron(ttbarEvent.Electrons());
		METPointer met = ttbarEvent.MET();
		JetCollection jets = ttbarEvent.GoodElectronCleanedJets();

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		fillHistograms("ElectronPlusJets/QCDConversionsBackground");
		if (ttbarEvent.passesMETCut() && ttbarEvent.passesAsymmetricElectronCleanedJetCuts())
			fillHistograms("ElectronPlusJets/QCDConversionsBackground", "_withMETAndAsymJets");
	}
}

void MTtbarAnalyser::fillHistograms(std::string subcollection, std::string suffix) {
	histMan->setCurrentCollection("MttbarAnalysis/" + subcollection);

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
			histMan->H1D_BJetBinned("mttbar" + suffix)->Fill(mttbar, weight);
			histMan->H1D_BJetBinned("ttbar_pt" + suffix)->Fill(ttbarPt, weight);
			histMan->H1D_BJetBinned("m3" + suffix)->Fill(m3, weight);
			histMan->H1D_BJetBinned("leptonicTopMass" + suffix)->Fill(leptonicTopMass, weight);
			histMan->H1D_BJetBinned("hadronicTopMass" + suffix)->Fill(hadronicTopMass, weight);
			histMan->H1D_BJetBinned("allTopMasses" + suffix)->Fill(leptonicTopMass, weight);
			histMan->H1D_BJetBinned("allTopMasses" + suffix)->Fill(hadronicTopMass, weight);

			histMan->H1D_BJetBinned("leptonicWMass" + suffix)->Fill(leptonicWMass, weight);
			histMan->H1D_BJetBinned("hadronicWMass" + suffix)->Fill(hadronicWMass, weight);

			histMan->H1D_BJetBinned("discriminant" + suffix)->Fill(disc, weight);
		}

		double normalisedWeight = weight / numberOfSolutions;
		histMan->H1D_BJetBinned("mttbar_allSolutions" + suffix)->Fill(mttbar, normalisedWeight);
		histMan->H1D_BJetBinned("ttbar_pt_allSolutions" + suffix)->Fill(ttbarPt, normalisedWeight);
		histMan->H1D_BJetBinned("m3_allSolutions" + suffix)->Fill(m3, normalisedWeight);
		histMan->H1D_BJetBinned("leptonicTopMass_allSolutions" + suffix)->Fill(leptonicTopMass, normalisedWeight);
		histMan->H1D_BJetBinned("hadronicTopMass_allSolutions" + suffix)->Fill(hadronicTopMass, normalisedWeight);
		histMan->H1D_BJetBinned("allTopMasses_allSolutions" + suffix)->Fill(leptonicTopMass, normalisedWeight);
		histMan->H1D_BJetBinned("allTopMasses_allSolutions" + suffix)->Fill(hadronicTopMass, normalisedWeight);
		histMan->H1D_BJetBinned("leptonicWMass_allSolutions" + suffix)->Fill(leptonicWMass, normalisedWeight);
		histMan->H1D_BJetBinned("hadronicWMass_allSolutions" + suffix)->Fill(hadronicWMass, normalisedWeight);

		histMan->H1D_BJetBinned("discriminant_allSolutions" + suffix)->Fill(disc, normalisedWeight);

	}
}

void MTtbarAnalyser::createHistograms() {
	histMan->setCurrentCollection("MttbarAnalysis");

	createHistogramsFor("MuonPlusJets/FourJetChi2");
	createHistogramsFor("MuonPlusJets/ThreeJetChi2");
	createHistogramsFor("MuonPlusJets/FourJetTopMassDifference");
//	createHistogramsFor("MuonPlusJets/FourJetChi2MC"); //for input values!
//	createHistogramsFor("MuonPlusJets/QCDConversionsBackground");

	createHistogramsFor("ElectronPlusJets/FourJetChi2");
	createHistogramsFor("ElectronPlusJets/ThreeJetChi2");
	createHistogramsFor("ElectronPlusJets/FourJetTopMassDifference");
	//	createHistogramsFor("ElectronPlusJets/FourJetChi2MC"); //for input values!
	createHistogramsFor("ElectronPlusJets/QCDConversionsBackground");

}

void MTtbarAnalyser::createHistogramsFor(std::string collection) {
	boost::array<std::string, 4> histTypes = { { "", "_withMETAndAsymJets", "_allSolutions",
			"_allSolutions_withMETAndAsymJets" } };

	histMan->setCurrentCollection("MttbarAnalysis/" + collection);

	for (unsigned short index = 0; index < histTypes.size(); ++index) {
		std::string suffix = histTypes.at(index);
		histMan->addH1D_BJetBinned("mttbar" + suffix, "m(t#bar{t});m(t#bar{t}) [GeV]; events/1 GeV", 5000, 0, 5000);

		histMan->addH1D_BJetBinned("leptonicTopMass" + suffix, "leptonic Top Mass; m(t_{lep}) [GeV]; events/1 GeV",
				1000, 0, 1000);
		histMan->addH1D_BJetBinned("hadronicTopMass" + suffix, "hadronic Top Mass; m(t_{had}) [GeV]; events/1 GeV",
				1000, 0, 1000);
		histMan->addH1D_BJetBinned("allTopMasses" + suffix, "all Top Masses;m(t) [GeV]; events'1 GeV", 1000, 0, 1000);
		histMan->addH1D_BJetBinned("m3" + suffix, "m3;m(jjj) [GeV]; events/1 GeV", 5000, 0, 5000);
		histMan->addH1D_BJetBinned("ttbar_pt" + suffix, "p_{T,t#bar{t}};p_{T,t#bar{t}} [GeV]; events/1 GeV", 1000, 0,
				1000);
		histMan->addH1D_BJetBinned("leptonicWMass" + suffix, "leptonic W mass; m(W_{lep}) [GeV]; events'1 GeV", 500, 0,
				500);
		histMan->addH1D_BJetBinned("hadronicWMass" + suffix, "hadronic W mass; m(W_{had}) [GeV]; events'1 GeV", 500, 0,
				500);
		histMan->addH1D_BJetBinned("discriminant" + suffix, "discriminant;discriminant; events/0.1", 1000, 0, 100);

	}

}

MTtbarAnalyser::MTtbarAnalyser(boost::shared_ptr<HistogramManager> histMan) :
		BasicAnalyser(histMan),
		weight(0),
		currentType("ElectronPlusJets"),
		allSolutions() {
	histMan->setCurrentCollection("MttbarAnalysis");

}

MTtbarAnalyser::~MTtbarAnalyser() {
}

} /* namespace BAT */
