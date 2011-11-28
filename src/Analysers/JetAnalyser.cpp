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
}

JetAnalyser::JetAnalyser(boost::shared_ptr<HistogramManager> histMan) :
	BasicAnalyser(histMan) {

}

JetAnalyser::~JetAnalyser() {
}

} /* namespace BAT */
