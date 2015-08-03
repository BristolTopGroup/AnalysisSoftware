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
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->Fill("EventWeight", event->weight());

	// Get each jet pair combination and form a W candidate
	for ( unsigned int jet1Index=0; jet1Index < jetsWithoutBs.size()-1; ++jet1Index ) {
		for ( unsigned int jet2Index=jet1Index+1; jet2Index < jetsWithoutBs.size(); ++jet2Index ) {
			JetPointer jet1 = jetsWithoutBs[jet1Index];
			JetPointer jet2 = jetsWithoutBs[jet2Index];

			// W mass from RAW jets
			// Note there will be some bias, as only jets with corrected pt > 30 are in the ntuple
			ParticlePointer jet1Raw = jet1->raw_jet();
			ParticlePointer jet2Raw = jet2->raw_jet();

			if ( jet1Raw->pt() > 30 && jet2Raw->pt() > 30 ) {
				Particle hadronicWRaw(*jet1Raw + *jet2Raw);

				histMan_->H1D("hadronicWMass_RAW")->Fill(hadronicWRaw.mass() , weight_);
				histMan_->H1D("jetPt_RAW")->Fill(jet1Raw->pt() , weight_);
				histMan_->H1D("jetPt_RAW")->Fill(jet2Raw->pt() , weight_);
				histMan_->H1D("jetEta_RAW")->Fill(jet1Raw->eta() , weight_);
				histMan_->H1D("jetEta_RAW")->Fill(jet2Raw->eta() , weight_);
			}


			if (jet1->pt()<=30 || jet2->pt()<=30 ) continue;

			Particle hadronicW(*jet1 + *jet2);
			histMan_->setCurrentHistogramFolder(histogramFolder_);
			histMan_->H1D("hadronicWMass")->Fill(hadronicW.mass() , weight_);
			histMan_->H1D("jetPt")->Fill(jet1->pt() , weight_);
			histMan_->H1D("jetPt")->Fill(jet2->pt() , weight_);
			histMan_->H1D("jetEta")->Fill(jet1->eta() , weight_);
			histMan_->H1D("jetEta")->Fill(jet2->eta() , weight_);

			treeMan_->setCurrentFolder(histogramFolder_);
			treeMan_->Fill("mjj",hadronicW.mass());
			treeMan_->Fill("jetPt",jet1->pt());
			treeMan_->Fill("jetPt",jet2->pt());
			treeMan_->Fill("jetEta",jet1->eta());
			treeMan_->Fill("jetEta",jet2->eta());
			treeMan_->Fill("NPU",event->Vertices().size());

			// Look at matched generator jets
			const ParticlePointer genJet1 = jet1->matched_generated_jet();
			const ParticlePointer genJet2 = jet2->matched_generated_jet();

			if ( genJet1 != 0 && genJet2 != 0 ) {
				Particle hadronicW_fromGenJets(*genJet1 + *genJet2);
				histMan_->H1D("hadronicWMass_genJets")->Fill(hadronicW_fromGenJets.mass() , weight_);
				histMan_->H1D("jetPt_genJet")->Fill(genJet1->pt() , weight_);
				histMan_->H1D("jetPt_genJet")->Fill(genJet2->pt() , weight_);
				histMan_->H1D("jetEta_genJet")->Fill(genJet1->eta() , weight_);
				histMan_->H1D("jetEta_genJet")->Fill(genJet2->eta() , weight_);

				treeMan_->Fill("mjj_genJet",hadronicW.mass());
				treeMan_->Fill("genJetPt",genJet1->pt());
				treeMan_->Fill("genJetPt",genJet2->pt());
				treeMan_->Fill("genJetEta",genJet1->eta());
				treeMan_->Fill("genJetEta",genJet2->eta());

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

		treeMan_->setCurrentFolder(histogramFolder_);
		treeMan_->Fill("mjj_parton",hadronicW_fromPartons.mass());
		treeMan_->Fill("partonPt",quarkParton->pt());
		treeMan_->Fill("partonPt",quarkBarParton->pt());
		treeMan_->Fill("partonEta",quarkParton->eta());
		treeMan_->Fill("partonEta",quarkBarParton->eta());

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

	histMan_->addH1D("hadronicWMass_RAW", "hadronic W mass with RAW jets; m(W_{had}) [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetPt_RAW", "jet pt with RAW jets; p_{t} [GeV]; events/1 GeV", 500, 0, 500);
	histMan_->addH1D("jetEta_RAW", "jet eta with RAW jets; #eta; events/0.06", 100, -3, 3);

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

void WAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);

	treeMan_->addBranch("mjj", "F", "W Bosons" + Globals::treePrefix_);
	treeMan_->addBranch("mjj_genJet", "F", "W Bosons" + Globals::treePrefix_);
	treeMan_->addBranch("mjj_parton", "F", "W Bosons" + Globals::treePrefix_);
	treeMan_->addBranch("jetPt", "F", "W Bosons" + Globals::treePrefix_, false);
	treeMan_->addBranch("jetEta", "F", "W Bosons" + Globals::treePrefix_, false);
	treeMan_->addBranch("genJetPt", "F", "W Bosons" + Globals::treePrefix_, false);
	treeMan_->addBranch("genJetEta", "F", "W Bosons" + Globals::treePrefix_, false);
	treeMan_->addBranch("partonPt", "F", "W Bosons" + Globals::treePrefix_, false);
	treeMan_->addBranch("partonEta", "F", "W Bosons" + Globals::treePrefix_, false);

	treeMan_->addBranch("NPU", "F", "W Bosons" + Globals::treePrefix_);
}

WAnalyser::WAnalyser(boost::shared_ptr<HistogramManager> histMan, boost::shared_ptr<TreeManager> treeMan, std::string histogramFolder) :
		BasicAnalyser(histMan, treeMan, histogramFolder) {
}

WAnalyser::~WAnalyser() {
}

} /* namespace BAT */
