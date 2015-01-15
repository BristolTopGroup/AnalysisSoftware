/*
 * WAnalyser.cpp
 *
 *  Created on: 12 Jan 2015
 *      Author: ejclemen
 */

#include "../../interface/Analysers/WAnalyser.h"

#include <boost/scoped_ptr.hpp>

namespace BAT {

void WAnalyser::analyse(const EventPtr event) {
	// weight_ = event->weight();
}

void WAnalyser::analyseHadronicW(const EventPtr event, const JetCollection jets, const JetCollection bjets) {

	weight_ = event->weight();

	// Get cleaned jets that aren't b tagged
	JetCollection jetsWithoutBs;
	for ( unsigned int jetIndex=0; jetIndex < jets.size(); ++jetIndex ) {
		bool isBJet = false;
		JetPointer thisJet = jets[jetIndex];
		for ( unsigned int bJetIndex=0; bJetIndex < bjets.size(); ++bJetIndex ) {
			JetPointer thisBJet = bjets[bJetIndex];
			if ( thisJet == thisBJet ) {
				isBJet = true;
				break;
			}
		}
		if ( !isBJet ) jetsWithoutBs.push_back( thisJet );
	}

	if ( jetsWithoutBs.size() < 2 ) return;


	// Need these for some gen level studies
	const JetCollection genJets( event->GenJets() );

	// Get each jet pair combination and form a W candidate
	for ( unsigned int jet1Index=0; jet1Index < jetsWithoutBs.size()-1; ++jet1Index ) {
		for ( unsigned int jet2Index=jet1Index+1; jet2Index < jetsWithoutBs.size(); ++jet2Index ) {
			JetPointer jet1 = jetsWithoutBs[jet1Index];
			JetPointer jet2 = jetsWithoutBs[jet2Index];

			// if (jet1->pt()<=30 || jet2->pt()<=30 ) continue;

			// if ( fabs(jet1->eta())>2.5 || fabs(jet2->eta())>2.5) continue;
			// // 	cout << "Jets with large eta : " << jet1->eta() << " " << jet2->eta() << endl;

			// if ( jet1->NOD() <=1 || jet1->NHF() >= 0.99 || jet1->NEF() >= 0.99) {
			// 	continue;
			// }
			// if ( jet2->NOD() <=1 || jet2->NHF() >= 0.99 || jet2->NEF() >= 0.99) {
			// 	continue;
			// }

			// if ( fabs(jet1->eta())<2.4 ){
			// 	if ( jet1->CEF() >= 0.99 || jet1->CHF() <= 0 || jet1->NCH() <= 0 ) {
			// 		continue;
			// 	}

			// }

			// if ( fabs(jet2->eta())<2.4 ){
			// 	if ( jet2->CEF() >= 0.99 || jet2->CHF() <= 0 || jet2->NCH() <= 0 ){
			// 		continue;
			// 	}
			// }

			Particle hadronicW(*jet1 + *jet2);

			histMan_->setCurrentHistogramFolder(histogramFolder_);
			histMan_->H1D("hadronicWMass")->Fill(hadronicW.mass() , weight_);
			histMan_->H1D("jetPt")->Fill(jet1->pt() , weight_);
			histMan_->H1D("jetPt")->Fill(jet2->pt() , weight_);
			histMan_->H1D("jetEta")->Fill(jet1->eta() , weight_);
			histMan_->H1D("jetEta")->Fill(jet2->eta() , weight_);

			const ParticlePointer genJet1 = jet1->matched_generated_jet();
			const ParticlePointer genJet2 = jet2->matched_generated_jet();

			if ( genJet1 != 0 && genJet2 != 0 ) {
				Particle hadronicW_fromGenJets(*genJet1 + *genJet2);
				histMan_->H1D("hadronicWMass_genJets")->Fill(hadronicW_fromGenJets.mass() , weight_);
				histMan_->H1D("jetPt_genJet")->Fill(genJet1->pt() , weight_);
				histMan_->H1D("jetPt_genJet")->Fill(genJet2->pt() , weight_);
				histMan_->H1D("jetEta_genJet")->Fill(genJet1->eta() , weight_);
				histMan_->H1D("jetEta_genJet")->Fill(genJet2->eta() , weight_);

				// Now check if these gen jets correspond to those matched to the W decay products
				const TTGenInfoPointer ttGen( event->TTGenInfo() );
				int quarkGenJetIndex = ttGen->getQuarkGenJetIndex();
				int quarkBarGenJetIndex = ttGen->getQuarkBarGenJetIndex();
				if ( quarkGenJetIndex >= 0 && quarkBarGenJetIndex >= 0 ) {
					JetPointer quarkGenJet( genJets[quarkGenJetIndex] );
					JetPointer quarkBarGenJet( genJets[quarkBarGenJetIndex] );

					if ( ( genJet1->getFourVector() == quarkGenJet->getFourVector() && genJet2->getFourVector() == quarkBarGenJet->getFourVector() ) ||
						( genJet2->getFourVector() == quarkGenJet->getFourVector() && genJet1->getFourVector() == quarkBarGenJet->getFourVector() ) ) {

						// These reco jets actually correspond to thos from a W
						histMan_->H1D("hadronicWMass_recoMatchedToPartons")->Fill(hadronicW.mass() , weight_);
						histMan_->H1D("jetPt_recoMatchedToPartons")->Fill(jet1->pt() , weight_);
						histMan_->H1D("jetPt_recoMatchedToPartons")->Fill(jet2->pt() , weight_);
						histMan_->H1D("jetEta_recoMatchedToPartons")->Fill(jet1->eta() , weight_);
						histMan_->H1D("jetEta_recoMatchedToPartons")->Fill(jet2->eta() , weight_);

					}
				}
			}
		}
	}

	//  Get jets that are fairly separated from other jets
	// These jets are a further subset of jetsWithoutBs
	JetCollection cleanedJets;
	for ( unsigned int jet1Index=0; jet1Index < jetsWithoutBs.size(); ++jet1Index ) {
		JetPointer jet1 = jetsWithoutBs[jet1Index];
		double minDeltaR = 999.;
		for ( unsigned int jet2Index=jet1Index+1; jet2Index < jetsWithoutBs.size(); ++jet2Index ) {

			JetPointer jet2 = jetsWithoutBs[jet2Index];

			double deltaRToOtherJet = jet1->deltaR( jet2 );

			if ( deltaRToOtherJet < minDeltaR )
				minDeltaR = deltaRToOtherJet;
		}
		if ( minDeltaR > 1 ) {
			histMan_->H1D("minDeltaR_cleanedReco")->Fill( minDeltaR, weight_ );
			cleanedJets.push_back( jet1 );
		}
	}

	if ( cleanedJets.size() < 2 ) return;

	// Get each jet pair combination and form a W candidate
	for ( unsigned int jet1Index=0; jet1Index < cleanedJets.size()-1; ++jet1Index ) {
		for ( unsigned int jet2Index=jet1Index+1; jet2Index < cleanedJets.size(); ++jet2Index ) {
			JetPointer jet1 = cleanedJets[jet1Index];
			JetPointer jet2 = cleanedJets[jet2Index];

			if (jet1->pt()<=30 || jet2->pt()<=30 ) continue;

			Particle hadronicW(*jet1 + *jet2);

			histMan_->setCurrentHistogramFolder(histogramFolder_);
			histMan_->H1D("hadronicWMass_cleanedReco")->Fill(hadronicW.mass() , weight_);
			histMan_->H1D("jetPt_cleanedReco")->Fill(jet1->pt() , weight_);
			histMan_->H1D("jetPt_cleanedReco")->Fill(jet2->pt() , weight_);
			histMan_->H1D("jetEta_cleanedReco")->Fill(jet1->eta() , weight_);
			histMan_->H1D("jetEta_cleanedReco")->Fill(jet2->eta() , weight_);
		}
	}

}

void WAnalyser::analyseHadronicW_partons(const EventPtr event) {

	weight_ = event->weight();

	// Get partons associated with W decay
	const TTGenInfoPointer ttGen( event->TTGenInfo() );
	const ParticlePointer quarkParton = ttGen->getQuark();
	const ParticlePointer quarkBarParton = ttGen->getQuarkBar();
	const Particle hadronicW_fromPartons(*quarkParton + *quarkBarParton);

	if ( quarkParton->pt() > 30 && quarkBarParton->pt() > 30 &&
			fabs(quarkParton->eta()) < 2.5 && fabs(quarkBarParton->eta()) < 2.5 ) {


		histMan_->setCurrentHistogramFolder(histogramFolder_);
		histMan_->H1D("hadronicWMass_partons")->Fill(hadronicW_fromPartons.mass() , weight_);
		histMan_->H1D("jetPt_partons")->Fill(quarkParton->pt() , weight_);
		histMan_->H1D("jetPt_partons")->Fill(quarkBarParton->pt() , weight_);
		histMan_->H1D("jetEta_partons")->Fill(quarkParton->eta() , weight_);
		histMan_->H1D("jetEta_partons")->Fill(quarkBarParton->eta() , weight_);

		// Get gen jets associated with partons
		// Note these gen jets are not cleaned against leptons.  But as we are considering the ones matched to the quarks from W decay, this shouldn't be too much of a problem
		const JetCollection genJets( event->GenJets() );
		int quarkGenJetIndex = ttGen->getQuarkGenJetIndex();
		int quarkBarGenJetIndex = ttGen->getQuarkBarGenJetIndex();

		if ( quarkGenJetIndex >= 0 && quarkBarGenJetIndex >= 0 ) {
			JetPointer quarkGenJet( genJets[quarkGenJetIndex] );
			JetPointer quarkBarGenJet( genJets[quarkBarGenJetIndex] );

			if ( quarkGenJet->pt() > 30 && quarkBarGenJet->pt() > 30 &&
				fabs(quarkGenJet->eta()) < 2.5 && fabs(quarkBarGenJet->eta()) < 2.5 ) {

				const Particle hadronicW_fromGenJetsMatchedToPartons(*quarkGenJet + *quarkBarGenJet);

				histMan_->H1D("hadronicWMass_partonsGenJets")->Fill(hadronicW_fromGenJetsMatchedToPartons.mass() , weight_);
				histMan_->H1D("jetPt_partonsGenJets")->Fill(quarkGenJet->pt() , weight_);
				histMan_->H1D("jetPt_partonsGenJets")->Fill(quarkBarGenJet->pt() , weight_);
				histMan_->H1D("jetEta_partonsGenJets")->Fill(quarkGenJet->eta() , weight_);
				histMan_->H1D("jetEta_partonsGenJets")->Fill(quarkBarGenJet->eta() , weight_);
			}
		}
	}
}

void WAnalyser::fillHistograms(std::string subcollection, std::string suffix) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	// unsigned short numberOfSolutions = allSolutions.size();
	// for (unsigned short index = 0; index < numberOfSolutions; ++index) {
	// 	TtbarHypothesisPointer currentSolution = allSolutions.at(index);
	// 	const ParticlePointer resonance = currentSolution->resonance;

	// 	double hadronicWMass = currentSolution->hadronicW->mass();

	// 	if (index == 0) {
	// 		histMan_->H1D_BJetBinned("hadronicWMass" + suffix)->Fill(hadronicWMass, weight_ * scale_);
	// 	}

	// 	double normalisedWeight = weight_ * scale_ / numberOfSolutions;
	// 	histMan_->H1D_BJetBinned("hadronicWMass_allSolutions" + suffix)->Fill(hadronicWMass, normalisedWeight);
	// }
}

void WAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	histMan_->addH1D("hadronicWMass", "hadronic W mass; m(W_{had}) [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetPt", "jet pt; p_{t} [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetEta", "jet eta; #eta; events/0.06", 100, -3, 3);

	histMan_->addH1D("hadronicWMass_genJets", "hadronic W mass from gen jets; m(W_{had}) [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetPt_genJet", "gen jet pt; p_{t} [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetEta_genJet", "gen jet eta; #eta; events/0.06", 100, -3, 3);

	histMan_->addH1D("hadronicWMass_partons", "hadronic W mass from partons; m(W_{had}) [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetPt_partons", "parton pt; p_{t} [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetEta_partons", "parton eta; #eta; events/0.06", 100, -3, 3);

	histMan_->addH1D("hadronicWMass_partonsGenJets", "hadronic W mass from gen jets matched to partons; m(W_{had}) [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetPt_partonsGenJets", "gen jet matched to parton pt; p_{t} [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetEta_partonsGenJets", "gen jet matched to parton eta; #eta; events/0.06", 100, -3, 3);

	histMan_->addH1D("hadronicWMass_recoMatchedToPartons", "hadronic W mass from reco jets matched to partons; m(W_{had}) [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetPt_recoMatchedToPartons", "reco jet matched to parton pt; p_{t} [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetEta_recoMatchedToPartons", "reco jet matched to parton eta; #eta; events/0.06", 100, -3, 3);

	histMan_->addH1D("hadronicWMass_cleanedReco", "hadronic W mass; m(W_{had}) [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetPt_cleanedReco", "jet pt; p_{t} [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetEta_cleanedReco", "jet eta; #eta; events/0.06", 100, -3, 3);
	histMan_->addH1D("minDeltaR_cleanedReco", "deltaR to nearest jet; #Delta R; events/0.013", 100, 0, 1.3);


}

WAnalyser::WAnalyser(boost::shared_ptr<HistogramManager> histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {
}

WAnalyser::~WAnalyser() {
}

} /* namespace BAT */
