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

#include <boost/scoped_ptr.hpp>

namespace BAT {

void MTtbarAnalyser::analyse(const EventPtr event) {
//	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));
	weight = event->weight();
	analyseFourJetChi2(event);
	analyseThreeJetChi2(event);
	analyseFourJetTopMassDifference(event);
	analyseFourJetChi2QCDConversionBackground(event);
}

void MTtbarAnalyser::analyseFourJetChi2(const EventPtr event) {
	string type("ElectronPlusJets");

	if (topEplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		LeptonPointer selectedElectron = topEplusJetsRefSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = topEplusJetsRefSelection_->cleanedJets(event);
		JetCollection bJets = topEplusJetsRefSelection_->cleanedBJets(event);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		fillHistograms(type + "/FourJetChi2");
	}

//	type = "MuonPlusJets";
//	histMan_->setCurrentJetBin(event->GoodMuonCleanedJets().size());
//	histMan_->setCurrentBJetBin(event->GoodMuonCleanedBJets().size());
//
//	if (event->passesMuPlusJetsSelectionStepUpTo(TTbarMuPlusJetsSelection::AtLeastFourGoodJets)) {
//		LeptonPointer selectedLepton = event->GoodPFIsolatedMuons().front();
//		METPointer met = event->MET();
//		JetCollection jets = event->GoodMuonCleanedJets();
//
//		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
//				new ChiSquaredBasedTopPairReconstruction(selectedLepton, met, jets));
//		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
//			cout << chi2Reco->getDetailsOnFailure();
//			return;
//		}
//		allSolutions = chi2Reco->getAllSolutions();
//		fillHistograms(type + "/FourJetChi2");
//		if (event->passesMETCut() && event->passesAsymmetricMuonCleanedJetCuts())
//			fillHistograms(type + "/FourJetChi2", "_withMETAndAsymJets");
//	}
//	//soon to be removed
//	histMan_->setCurrentJetBin(event->GoodElectronCleanedJets().size());
//	histMan_->setCurrentBJetBin(event->GoodElectronCleanedBJets().size());
}

void MTtbarAnalyser::analyseThreeJetChi2(const EventPtr event) {
	string type("ElectronPlusJets");

	if (topEplusJetsRefSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastThreeGoodJets)) {
		JetCollection jets = topEplusJetsRefSelection_->cleanedJets(event);
		if (jets.size() != 3) //only consider 3 jet events
			return;

		JetCollection bJets = topEplusJetsRefSelection_->cleanedBJets(event);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());

		LeptonPointer selectedElectron = topEplusJetsRefSelection_->signalLepton(event);
		METPointer met = event->MET();

		boost::scoped_ptr<ChiSquaredThreeJetsTopPairReconstruction> chi2Reco(
				new ChiSquaredThreeJetsTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		fillHistograms(type + "/ThreeJetChi2");
	}

//	type = "MuonPlusJets";
//	histMan_->setCurrentJetBin(event->GoodMuonCleanedJets().size());
//	histMan_->setCurrentBJetBin(event->GoodMuonCleanedBJets().size());
//
//	if (event->passesMuPlusJetsSelectionStepUpTo(TTbarMuPlusJetsSelection::AtLeastTwoGoodJets)
//			&& event->GoodMuonCleanedJets().size() == 3) {
//		LeptonPointer selectedLepton = event->GoodPFIsolatedMuons().front();
//		METPointer met = event->MET();
//		JetCollection jets = event->GoodMuonCleanedJets();
//
//		boost::scoped_ptr<ChiSquaredThreeJetsTopPairReconstruction> chi2Reco(
//				new ChiSquaredThreeJetsTopPairReconstruction(selectedLepton, met, jets));
//		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
//			cout << chi2Reco->getDetailsOnFailure();
//			return;
//		}
//		allSolutions = chi2Reco->getAllSolutions();
//		fillHistograms(type + "/ThreeJetChi2");
//		if (event->passesMETCut() && event->passesAsymmetricMuonCleanedJetCuts())
//			fillHistograms(type + "/ThreeJetChi2", "_withMETAndAsymJets");
//	}
//
//	//soon to be removed
//	histMan_->setCurrentJetBin(event->GoodElectronCleanedJets().size());
//	histMan_->setCurrentBJetBin(event->GoodElectronCleanedBJets().size());
}

void MTtbarAnalyser::analyseFourJetTopMassDifference(const EventPtr event) {
	string type("ElectronPlusJets");

	if (topEplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		LeptonPointer selectedElectron = topEplusJetsRefSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = topEplusJetsRefSelection_->cleanedJets(event);
		JetCollection bJets = topEplusJetsRefSelection_->cleanedBJets(event);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());

		boost::scoped_ptr<TopMassDifferenceTTbarReco> topMassDiffReco(
				new TopMassDifferenceTTbarReco(selectedElectron, met, jets));
		if (!topMassDiffReco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << topMassDiffReco->getDetailsOnFailure();
			return;
		}
		allSolutions = topMassDiffReco->getAllSolutions();
		fillHistograms(type + "/FourJetTopMassDifference");
	}

//	type = "MuonPlusJets";
//	histMan_->setCurrentJetBin(event->GoodMuonCleanedJets().size());
//	histMan_->setCurrentBJetBin(event->GoodMuonCleanedBJets().size());
//
//	if (event->passesMuPlusJetsSelectionStepUpTo(TTbarMuPlusJetsSelection::AtLeastFourGoodJets)) {
//		LeptonPointer selectedLepton = event->GoodPFIsolatedMuons().front();
//		METPointer met = event->MET();
//		JetCollection jets = event->GoodMuonCleanedJets();
//
//		boost::scoped_ptr<TopMassDifferenceTTbarReco> topMassDiffReco(
//				new TopMassDifferenceTTbarReco(selectedLepton, met, jets));
//		if (!topMassDiffReco->meetsInitialCriteria()) { //reports details on failure and skips event
//			cout << topMassDiffReco->getDetailsOnFailure();
//			return;
//		}
//		allSolutions = topMassDiffReco->getAllSolutions();
//		fillHistograms(type + "/FourJetTopMassDifference");
//		if (event->passesMETCut() && event->passesAsymmetricMuonCleanedJetCuts())
//			fillHistograms(type + "/FourJetTopMassDifference", "_withMETAndAsymJets");
//	}
}

void MTtbarAnalyser::analyseFourJetChi2QCDConversionBackground(const EventPtr event) {

	if (qcdConversionSelection_->passesFullSelectionExceptLastTwoSteps(event)) { //passes all except b-tag!
		LeptonPointer selectedElectron = qcdConversionSelection_->signalLepton(event);
		METPointer met = event->MET();
		JetCollection jets = qcdConversionSelection_->cleanedJets(event);
		JetCollection bJets = qcdConversionSelection_->cleanedBJets(event);
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(bJets.size());

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		fillHistograms("ElectronPlusJets/QCDConversionsBackground");
//		if (event->passesMETCut() && event->passesAsymmetricElectronCleanedJetCuts())
//			fillHistograms("ElectronPlusJets/QCDConversionsBackground", "_withMETAndAsymJets");
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
			histMan_->H1D_BJetBinned("mttbar" + suffix)->Fill(mttbar, weight);
			histMan_->H1D_BJetBinned("ttbar_pt" + suffix)->Fill(ttbarPt, weight);
			histMan_->H1D_BJetBinned("m3" + suffix)->Fill(m3, weight);
			histMan_->H1D_BJetBinned("leptonicTopMass" + suffix)->Fill(leptonicTopMass, weight);
			histMan_->H1D_BJetBinned("hadronicTopMass" + suffix)->Fill(hadronicTopMass, weight);
			histMan_->H1D_BJetBinned("allTopMasses" + suffix)->Fill(leptonicTopMass, weight);
			histMan_->H1D_BJetBinned("allTopMasses" + suffix)->Fill(hadronicTopMass, weight);

			histMan_->H1D_BJetBinned("leptonicWMass" + suffix)->Fill(leptonicWMass, weight);
			histMan_->H1D_BJetBinned("hadronicWMass" + suffix)->Fill(hadronicWMass, weight);

			histMan_->H1D_BJetBinned("discriminant" + suffix)->Fill(disc, weight);
		}

		double normalisedWeight = weight / numberOfSolutions;
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
	boost::array<std::string, 4> histTypes = { { "",
//			"_withMETAndAsymJets",
			"_allSolutions",
//			"_allSolutions_withMETAndAsymJets"
			} };

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
		weight(0), //
		currentType("ElectronPlusJets"), //
		allSolutions(), //
		topEplusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection()), //
		qcdAntiIDSelection_(new QCDAntiIDEPlusJetsSelection()), //
		qcdConversionSelection_(new QCDConversionsSelection()), //
		qcdNonIsoSelection_(new QCDNonIsolatedElectronSelection()) {
	qcdAntiIDSelection_->useMVAID(true);
}

MTtbarAnalyser::~MTtbarAnalyser() {
}

} /* namespace BAT */
