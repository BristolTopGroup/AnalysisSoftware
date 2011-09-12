/*
 * MTtbarAnalyser.cpp
 *
 *  Created on: 5 Sep 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/MTtbarAnalyser.h"
#include "../../interface/RecontructionModules/ChiSquaredBasedTopPairReconstruction.h"
#include "../../interface/RecontructionModules/ChiSquaredThreeJetsTopPairReconstruction.h"
#include "../../interface/RecontructionModules/TopMassDifferenceTTbarReco.h"
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

	if (ttbarEvent.passesSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets)) {
		ElectronPointer selectedElectron = ttbarEvent.GoodPFIsolatedElectrons().front();
		METPointer met = ttbarEvent.MET();
		JetCollection jets = ttbarEvent.GoodJets();

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		fillHistograms("FourJetChi2");
		if (ttbarEvent.passesMETCut() && ttbarEvent.passesAsymmetricJetCuts())
			fillHistograms("FourJetChi2", "_withMETAndAsymJets");
	}
}

void MTtbarAnalyser::analyseThreeJetChi2(const TopPairEventCandidate& ttbarEvent) {
	if (ttbarEvent.passesSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastTwoGoodJets)
			&& ttbarEvent.hasExactlyThreeGoodJets()) {
		ElectronPointer selectedElectron = ttbarEvent.GoodPFIsolatedElectrons().front();
		METPointer met = ttbarEvent.MET();
		JetCollection jets = ttbarEvent.GoodJets();

		boost::scoped_ptr<ChiSquaredThreeJetsTopPairReconstruction> chi2Reco(
				new ChiSquaredThreeJetsTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		fillHistograms("ThreeJetChi2");
		if (ttbarEvent.passesMETCut() && ttbarEvent.passesAsymmetricJetCuts())
			fillHistograms("ThreeJetChi2", "_withMETAndAsymJets");
	}
}

void MTtbarAnalyser::analyseFourJetTopMassDifference(const TopPairEventCandidate& ttbarEvent) {
	if (ttbarEvent.passesSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets)) {
		ElectronPointer selectedElectron = ttbarEvent.GoodPFIsolatedElectrons().front();
		METPointer met = ttbarEvent.MET();
		JetCollection jets = ttbarEvent.GoodJets();

		boost::scoped_ptr<TopMassDifferenceTTbarReco> topMassDiffReco(
				new TopMassDifferenceTTbarReco(selectedElectron, met, jets));
		if (!topMassDiffReco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << topMassDiffReco->getDetailsOnFailure();
			return;
		}
		allSolutions = topMassDiffReco->getAllSolutions();
		fillHistograms("FourJetTopMassDifference");
		if (ttbarEvent.passesMETCut() && ttbarEvent.passesAsymmetricJetCuts())
			fillHistograms("FourJetTopMassDifference", "_withMETAndAsymJets");
	}
}

void MTtbarAnalyser::analyseFourJetChi2QCDConversionBackground(const TopPairEventCandidate& ttbarEvent) {
	if (ttbarEvent.passesConversionSelection() && ttbarEvent.hasAtLeastFourGoodJets()) {
		ElectronPointer selectedElectron = ttbarEvent.MostPFIsolatedElectron();
		METPointer met = ttbarEvent.MET();
		JetCollection jets = ttbarEvent.GoodJets();

		boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
				new ChiSquaredBasedTopPairReconstruction(selectedElectron, met, jets));
		if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
			cout << chi2Reco->getDetailsOnFailure();
			return;
		}
		allSolutions = chi2Reco->getAllSolutions();
		fillHistograms("QCDConversionsBackground");
		if (ttbarEvent.passesMETCut() && ttbarEvent.passesAsymmetricJetCuts())
			fillHistograms("QCDConversionsBackground", "_withMETAndAsymJets");
	}
}

void MTtbarAnalyser::fillHistograms(std::string subcollection, std::string suffix){
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

		double normalisedWeight = weight/numberOfSolutions;
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

	createHistogramsFor("FourJetChi2");
	createHistogramsFor("ThreeJetChi2");
	createHistogramsFor("FourJetTopMassDifference");
//	createHistogramsFor("FourJetChi2MC"); //for input values!
	createHistogramsFor("QCDConversionsBackground");

}

void MTtbarAnalyser::createHistogramsFor(std::string collection) {
	boost::array<std::string, 4> histTypes = {{"", "_withMETAndAsymJets", "_allSolutions", "_allSolutions_withMETAndAsymJets"}};
	histMan->setCurrentCollection("MttbarAnalysis/" + collection);

	for(unsigned short index = 0; index < histTypes.size(); ++index){
		std::string suffix = histTypes.at(index);
		histMan->addH1D_BJetBinned("mttbar" + suffix, "mttbar", 5000, 0, 5000);

			histMan->addH1D_BJetBinned("leptonicTopMass" + suffix, "leptonicTopMass", 1000, 0, 1000);
			histMan->addH1D_BJetBinned("hadronicTopMass" + suffix, "hadronicTopMass", 1000, 0, 1000);
			histMan->addH1D_BJetBinned("allTopMasses" + suffix, "allTopMasses", 1000, 0, 1000);
			histMan->addH1D_BJetBinned("m3" + suffix, "m3", 5000, 0, 5000);
			histMan->addH1D_BJetBinned("ttbar_pt" + suffix, "ttbar_pt", 1000, 0, 1000);
			histMan->addH1D_BJetBinned("leptonicWMass" + suffix, "leptonicWMass", 500, 0, 500);
			histMan->addH1D_BJetBinned("hadronicWMass" + suffix, "hadronicWMass", 500, 0, 500);
			histMan->addH1D_BJetBinned("discriminant" + suffix, "discriminant", 1000, 0, 100);

	}

}

MTtbarAnalyser::MTtbarAnalyser(boost::shared_ptr<HistogramManager> histMan) :
    	    BasicAnalyser(histMan),
    	    weight(0),
    	    allSolutions() {
	histMan->setCurrentCollection("MttbarAnalysis");

}

MTtbarAnalyser::~MTtbarAnalyser() {
}

} /* namespace BAT */
