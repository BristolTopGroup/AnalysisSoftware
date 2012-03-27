/*
 * JetAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/JetAnalyser.h"
#include "../../interface/TopPairEventCandidate.h"

namespace BAT {

void JetAnalyser::analyse(const EventPtr event) {
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));

	histMan_->setCurrentHistogramFolder("jetStudy");
	double weight =  event->weight();
	if ( ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex)) {
		for (unsigned short jetIndex = 0; jetIndex <  event->Jets().size(); ++jetIndex)
			histMan_->H1D_BJetBinned("AllJetMass")->Fill( event->Jets().at(jetIndex)->mass());

		for (unsigned short jetIndex = 0; jetIndex <  event->GoodJets().size(); ++jetIndex) {
			double jetMass =  event->Jets().at(jetIndex)->mass();
			histMan_->H1D_BJetBinned("AllGoodJetMass")->Fill(jetMass, weight);

			if ( ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastOneGoodJets))
				histMan_->H1D_BJetBinned("GoodJetMass_atLeastOneJets")->Fill(jetMass, weight);

			if ( ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastTwoGoodJets))
				histMan_->H1D_BJetBinned("GoodJetMass_atLeastTwoJets")->Fill(jetMass, weight);

			if ( ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastThreeGoodJets))
				histMan_->H1D_BJetBinned("GoodJetMass_atLeastThreeJets")->Fill(jetMass, weight);

			if ( ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets))
				histMan_->H1D_BJetBinned("GoodJetMass_atLeastFourJets")->Fill(jetMass, weight);
		}
	}
	if ( ttbarCand->passesFullTTbarEPlusJetSelection()) {
		histMan_->H1D("goodJetsMultiplicity")->Fill( event->GoodElectronCleanedJets().size(), weight);
		histMan_->H1D("jetsMultiplicity")->Fill( event->Jets().size(), weight);
		histMan_->H1D("btaggedJetsMultiplicity")->Fill( event->GoodElectronCleanedBJets().size(), weight);
		for (unsigned index = 0; index <  event->GoodElectronCleanedJets().size(); ++index) {
			histMan_->H1D("jetPt")->Fill( event->GoodElectronCleanedJets().at(index)->pt(), weight);
			histMan_->H1D("jetEta")->Fill( event->GoodElectronCleanedJets().at(index)->eta(), weight);
			histMan_->H1D("jetPhi")->Fill( event->GoodElectronCleanedJets().at(index)->phi(), weight);
			histMan_->H1D("jetMass")->Fill( event->GoodElectronCleanedJets().at(index)->mass(), weight);
		}
		histMan_->H1D("1stJetPt")->Fill( event->GoodElectronCleanedJets().front()->pt(), weight);
		histMan_->H1D("1stJetEta")->Fill( event->GoodElectronCleanedJets().front()->eta(), weight);
		histMan_->H1D("1stJetPhi")->Fill( event->GoodElectronCleanedJets().front()->phi(), weight);
		histMan_->H1D("1stJetMass")->Fill( event->GoodElectronCleanedJets().front()->mass(), weight);
	}

	//Add NJets for TTbar selection
}

void JetAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder("jetStudy");
	histMan_->addH1D_BJetBinned("AllJetMass", "all jet mass; m(j) [GeV]; events", 500, 0, 500);
	histMan_->addH1D_BJetBinned("AllGoodJetMass", "all jet mass; m(j) [GeV]; events", 500, 0, 500);
	histMan_->addH1D_BJetBinned("GoodJetMass_atLeastOneJets", "good jet mass (>= 1 jets; m(j) [GeV]; events", 500, 0,
			500);
	histMan_->addH1D_BJetBinned("GoodJetMass_atLeastTwoJets", "good jet mass (>= 2 jets; m(j) [GeV]; events", 500, 0,
			500);
	histMan_->addH1D_BJetBinned("GoodJetMass_atLeastThreeJets", "good jet mass (>= 3 jets; m(j) [GeV]; events", 500, 0,
			500);
	histMan_->addH1D_BJetBinned("GoodJetMass_atLeastFourJets", "good jet mass (>= 4 jets; m(j) [GeV]; events", 500, 0,
			500);

	histMan_->addH1D("goodJetsMultiplicity", "Good jets multiplicity", 21, -0.5, 19.5);
	histMan_->addH1D("jetsMultiplicity", "All jets multiplicity", 21, -0.5, 19.5);
	histMan_->addH1D("btaggedJetsMultiplicity", "b-tagged jets multiplicity", 10, -0.5, 9.5);
	histMan_->addH1D("jetPt", "Jet Pt", 2000, 0, 2000);
	histMan_->addH1D("jetEta", "Jet Eta", 100, -2.5, 2.5);
	histMan_->addH1D("jetPhi", "Jet Phi", 100, -2.5, 2.5);
	histMan_->addH1D("jetMass", "Jet Mass", 500, 0, 500);

	histMan_->addH1D("1stJetPt", "Leading Jet Pt", 2000, 0, 2000);
	histMan_->addH1D("1stJetEta", "Leading Jet Eta", 100, -2.5, 2.5);
	histMan_->addH1D("1stJetPhi", "Leading Jet Phi", 100, -2.5, 2.5);
	histMan_->addH1D("1stJetMass", "Leading Jet Mass", 500, 0, 500);
}

JetAnalyser::JetAnalyser(boost::shared_ptr<HistogramManager> histMan, std::string histogramFolder ) :
		BasicAnalyser(histMan, histogramFolder) {

}

JetAnalyser::~JetAnalyser() {
}

} /* namespace BAT */
