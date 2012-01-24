/*
 * JetAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/JetAnalyser.h"

namespace BAT {

void JetAnalyser::analyse(const TopPairEventCandidate& ttbarEvent) {
	histMan->setCurrentCollection("jetStudy");
	double weight = ttbarEvent.weight();
	if (ttbarEvent.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex)) {
		for (unsigned short jetIndex = 0; jetIndex < ttbarEvent.Jets().size(); ++jetIndex)
			histMan->H1D_BJetBinned("AllJetMass")->Fill(ttbarEvent.Jets().at(jetIndex)->mass());

		for (unsigned short jetIndex = 0; jetIndex < ttbarEvent.GoodJets().size(); ++jetIndex) {
			double jetMass = ttbarEvent.Jets().at(jetIndex)->mass();
			histMan->H1D_BJetBinned("AllGoodJetMass")->Fill(jetMass, weight);

			if (ttbarEvent.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastOneGoodJets))
				histMan->H1D_BJetBinned("GoodJetMass_atLeastOneJets")->Fill(jetMass, weight);

			if (ttbarEvent.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastTwoGoodJets))
				histMan->H1D_BJetBinned("GoodJetMass_atLeastTwoJets")->Fill(jetMass, weight);

			if (ttbarEvent.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastThreeGoodJets))
				histMan->H1D_BJetBinned("GoodJetMass_atLeastThreeJets")->Fill(jetMass, weight);

			if (ttbarEvent.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets))
				histMan->H1D_BJetBinned("GoodJetMass_atLeastFourJets")->Fill(jetMass, weight);
		}
	}
	if (ttbarEvent.passesFullTTbarEPlusJetSelection()) {
		histMan->H1D("goodJetsMultiplicity")->Fill(ttbarEvent.GoodElectronCleanedJets().size(), weight);
		histMan->H1D("jetsMultiplicity")->Fill(ttbarEvent.Jets().size(), weight);
		histMan->H1D("btaggedJetsMultiplicity")->Fill(ttbarEvent.GoodElectronCleanedBJets().size(), weight);
		for (unsigned index = 0; index < ttbarEvent.GoodElectronCleanedJets().size(); ++index) {
			histMan->H1D("jetPt")->Fill(ttbarEvent.GoodElectronCleanedJets().at(index)->pt(), weight);
			histMan->H1D("jetEta")->Fill(ttbarEvent.GoodElectronCleanedJets().at(index)->eta(), weight);
			histMan->H1D("jetPhi")->Fill(ttbarEvent.GoodElectronCleanedJets().at(index)->phi(), weight);
			histMan->H1D("jetMass")->Fill(ttbarEvent.GoodElectronCleanedJets().at(index)->mass(), weight);
		}
		histMan->H1D("1stJetPt")->Fill(ttbarEvent.GoodElectronCleanedJets().front()->pt(), weight);
		histMan->H1D("1stJetEta")->Fill(ttbarEvent.GoodElectronCleanedJets().front()->eta(), weight);
		histMan->H1D("1stJetPhi")->Fill(ttbarEvent.GoodElectronCleanedJets().front()->phi(), weight);
		histMan->H1D("1stJetMass")->Fill(ttbarEvent.GoodElectronCleanedJets().front()->mass(), weight);
	}
}

void JetAnalyser::createHistograms() {
	histMan->setCurrentCollection("jetStudy");
	histMan->addH1D_BJetBinned("AllJetMass", "all jet mass; m(j) [GeV]; events", 500, 0, 500);
	histMan->addH1D_BJetBinned("AllGoodJetMass", "all jet mass; m(j) [GeV]; events", 500, 0, 500);
	histMan->addH1D_BJetBinned("GoodJetMass_atLeastOneJets", "good jet mass (>= 1 jets; m(j) [GeV]; events", 500, 0,
			500);
	histMan->addH1D_BJetBinned("GoodJetMass_atLeastTwoJets", "good jet mass (>= 2 jets; m(j) [GeV]; events", 500, 0,
			500);
	histMan->addH1D_BJetBinned("GoodJetMass_atLeastThreeJets", "good jet mass (>= 3 jets; m(j) [GeV]; events", 500, 0,
			500);
	histMan->addH1D_BJetBinned("GoodJetMass_atLeastFourJets", "good jet mass (>= 4 jets; m(j) [GeV]; events", 500, 0,
			500);

	histMan->addH1D("goodJetsMultiplicity", "Good jets multiplicity", 4, 3.5, 7.5);
	histMan->addH1D("jetsMultiplicity", "All jets multiplicity", 4, 3.5, 7.5);
	histMan->addH1D("btaggedJetsMultiplicity", "b-tagged jets multiplicity", 4, -0.5, 3.5);
	histMan->addH1D("jetPt", "Jet Pt", 100, 0, 200);
	histMan->addH1D("jetEta", "Jet Eta", 100, -2.5, 2.5);
	histMan->addH1D("jetPhi", "Jet Phi", 100, -2.5, 2.5);
	histMan->addH1D("jetMass", "Jet Mass", 100, 0, 40);

	histMan->addH1D("1stJetPt", "Leading Jet Pt", 100, 0, 200);
	histMan->addH1D("1stJetEta", "Leading Jet Eta", 100, -2.5, 2.5);
	histMan->addH1D("1stJetPhi", "Leading Jet Phi", 100, -2.5, 2.5);
	histMan->addH1D("1stJetMass", "Leading Jet Mass", 100, 0, 40);
}

JetAnalyser::JetAnalyser(boost::shared_ptr<HistogramManager> histMan) :
	BasicAnalyser(histMan) {

}

JetAnalyser::~JetAnalyser() {
}

} /* namespace BAT */
