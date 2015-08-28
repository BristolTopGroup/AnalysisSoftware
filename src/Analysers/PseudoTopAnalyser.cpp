/*
 * PseudoTopAnalyser.cpp
 *
 *  Created on: 24 Feb 2015
 *      Author: ejclemen
 */

#include "../../interface/Analysers/PseudoTopAnalyser.h"

#include <boost/scoped_ptr.hpp>
#include <iostream>

using namespace std;

namespace BAT {

double const PseudoTopAnalyser::minLeptonPt_ = 23;
double const PseudoTopAnalyser::maxLeptonAbsEta_ = 2.4;
double const PseudoTopAnalyser::minVetoLeptonPt_ = 15;
double const PseudoTopAnalyser::maxVetoLeptonAbsEta_ = 2.5;
double const PseudoTopAnalyser::minNeutrinoSumPt_ = 23;
double const PseudoTopAnalyser::minWMt_ = 30;
unsigned int const PseudoTopAnalyser::minNJets_ = 4;
unsigned int const PseudoTopAnalyser::minNBJets_ = 2;
double const PseudoTopAnalyser::minJetPt_ = 25;
double const PseudoTopAnalyser::maxJetAbsEta_ = 2.4;

void PseudoTopAnalyser::analyse(const EventPtr event) {
	weight_ = event->weight();

	treeMan_->setCurrentFolder(histogramFolder_);

	// Store gen selection criteria
	treeMan_->Fill("isSemiLeptonicElectron", event->isSemiLeptonicElectron());
	treeMan_->Fill("isSemiLeptonicMuon", event->isSemiLeptonicMuon());


	const PseudoTopParticlesPointer pseudoTopParticles = event->PseudoTopParticles();
	const MCParticleCollection pseudoTops = pseudoTopParticles->getPseudoTops();
	const MCParticlePointer pseudoLeptonicW = pseudoTopParticles->getPseudoLeptonicW();
	const MCParticlePointer pseudoLepton = pseudoTopParticles->getPseudoLepton();
	const ParticleCollection allPseudoLeptons = pseudoTopParticles->getAllPseudoLeptons();
	const MCParticleCollection pseudoBs = pseudoTopParticles->getPseudoBs();
	const ParticlePointer pseudoMET = pseudoTopParticles->getPseudoMET();
	const ParticlePointer pseudoNeutrino = pseudoTopParticles->getPseudoNeutrino();
	const JetCollection pseudoJets = pseudoTopParticles->getPseudoJets();

	ParticleCollection pseudoTopsForTTbar;

	// // Only consider events with two pseudo tops
	// if ( pseudoTops.size() != 2 ) return;

	// // Also only consider events that are semi leptonic at the pseudo top level
	// if ( !pseudoTopParticles->isSemiLeptonic() ) return;

	// Check if event passes event selection (at pseudo top level)
	if ( passesEventSelection( pseudoLepton, pseudoNeutrino, pseudoJets, pseudoBs, allPseudoLeptons, pseudoMET ) ) {
		treeMan_->Fill("passesGenEventSelection",1);
	}
	else {
		treeMan_->Fill("passesGenEventSelection",0);
	}

	//
	// TOP VARIABLES
	// Top reco at particle level performed
	//

	// Store info on top
	for ( unsigned int ptIndex = 0; ptIndex < pseudoTops.size(); ++ ptIndex ) {
		// cout << "Getting pseudo top number : " << ptIndex << endl;
		const ParticlePointer pseudoTop = pseudoTops[ptIndex];
		pseudoTopsForTTbar.push_back( pseudoTop );
		treeMan_->Fill("pseudoTop_pT", pseudoTop->pt() );
		treeMan_->Fill("pseudoTop_y", pseudoTop->rapidity() );
	}

	if( pseudoTopsForTTbar.size() == 2 ) {
		// Store info on ttbar
		ParticlePointer pseudoTTbar( new Particle( *pseudoTopsForTTbar[0] + *pseudoTopsForTTbar[1] ) );
		treeMan_->Fill("pseudoTTbar_pT", pseudoTTbar->pt() );
		treeMan_->Fill("pseudoTTbar_y", pseudoTTbar->rapidity() );
		treeMan_->Fill("pseudoTTbar_m", pseudoTTbar->mass() );
	}


	// Store info on Bs
	if ( pseudoBs.size() == 2 ) {
		unsigned int leadingPsuedoBIndex = 0;
		if ( pseudoBs[1]->pt() > pseudoBs[0]->pt() ) {
			leadingPsuedoBIndex = 1;
		}
		unsigned int subleadingPsuedoBIndex = ( leadingPsuedoBIndex == 0 ) ? 1 : 0;
		treeMan_->Fill("pseudoB_pT", pseudoBs[leadingPsuedoBIndex]->pt() );
		treeMan_->Fill("pseudoB_eta", pseudoBs[leadingPsuedoBIndex]->eta() );
		treeMan_->Fill("pseudoB_pT", pseudoBs[subleadingPsuedoBIndex]->pt() );
		treeMan_->Fill("pseudoB_eta", pseudoBs[subleadingPsuedoBIndex]->eta() );
		// for ( unsigned int pbIndex = 0; pbIndex < pseudoBs.size(); ++pbIndex ) {
		// 	treeMan_->Fill("pseudoB_pT", pseudoBs[pbIndex]->pt() );
		// 	treeMan_->Fill("pseudoB_eta", pseudoBs[pbIndex]->eta() );
		// }
	}


	//
	// GLOBAL VARIABLES
	// No top reco at particle level
	//

	if ( allPseudoLeptons.size() > 0 ) {
		// Store info on lepton
		treeMan_->Fill("pseudoLepton_pT", allPseudoLeptons[0]->pt() );
		treeMan_->Fill("pseudoLepton_eta", allPseudoLeptons[0]->eta() );		
	}
	else {
		treeMan_->Fill("pseudoLepton_pT", 0 );
		treeMan_->Fill("pseudoLepton_eta", -50 );
	}

	// Store pseudo MET
	if ( pseudoMET != 0 ) {
		treeMan_->Fill("pseudoMET", pseudoMET->et() );
		METAlgorithm::value metType = (METAlgorithm::value) 0;
		const METPointer met(event->MET(metType));
		if ( pseudoMET->et() <= 0 && met->et() > 0 ) {
		}
		treeMan_->Fill("pseudoMET_mass", pseudoMET->mass() );
	}
	else {
		treeMan_->Fill("pseudoMET", 0 );
	}


	// Store pseudo HT
	treeMan_->Fill("pseudoHT", event->HT( pseudoJets ) );

	// Store pseudo ST
	if ( allPseudoLeptons.size() > 0 ) {
		treeMan_->Fill("pseudoST", event->ST( pseudoJets, allPseudoLeptons[0], METPointer( new MET( pseudoMET->px(), pseudoMET->py() )) ) );
	}
	else {
		treeMan_->Fill("pseudoST", 0 );
	}

	// Store pseudo MT and WPT
	// These are from the W reconstructed from the pseudo particles
	// i.e. use the neutrino assocaited with the W rather than the more "global" MET
	if ( pseudoLeptonicW != 0 ) {
		treeMan_->Fill("pseudoWPT_reco", pseudoLeptonicW->pt() );
	}
	else {
		treeMan_->Fill("pseudoWPT_reco", 0 );
	}

	if ( pseudoMET != 0 && allPseudoLeptons.size() > 0 ) {
		double WPT = event->WPT( allPseudoLeptons[0], METPointer( new MET( pseudoMET->px(), pseudoMET->py() ))  );
		treeMan_->Fill("pseudoWPT", WPT );

		double MT = event->MT( allPseudoLeptons[0], METPointer( new MET( pseudoMET->px(), pseudoMET->py() )) );
		treeMan_->Fill("pseudoMT", MT );
	}
	else {
		treeMan_->Fill("pseudoWPT", 0 );
		treeMan_->Fill("pseudoMT", 0 );
	}

	// NJets && NBJets
	unsigned int numberOfBJets(0);
	unsigned int numberOfJets(0);
	for (unsigned int index = 0; index < pseudoJets.size(); ++index) {
		const JetPointer jet(pseudoJets.at(index));
		if (jet->pt() < 25 ) continue;
		++numberOfJets;
	}
	for (unsigned int index = 0; index < pseudoBs.size(); ++index) {
		const MCParticlePointer bJet(pseudoBs.at(index));
		if ( bJet->pt() < 25 ) continue;
		++numberOfBJets;
	}

	treeMan_->Fill("NPseudoJets", numberOfJets );
	treeMan_->Fill("NPseudoBJets", numberOfBJets );
}

void PseudoTopAnalyser::createTrees() {

	treeMan_->setCurrentFolder(histogramFolder_);
	// Generator level selecton
	treeMan_->addBranch("isSemiLeptonicElectron", "F","Unfolding");
	treeMan_->addBranch("isSemiLeptonicMuon", "F","Unfolding");

	treeMan_->addBranch("passesGenEventSelection","F","Unfolding");

	// Branches for top
	treeMan_->addBranch("pseudoTop_pT", "F", "Unfolding", false);
	treeMan_->addBranch("pseudoTop_y", "F", "Unfolding", false);
	// Branches for ttbar
	treeMan_->addBranch("pseudoTTbar_pT", "F","Unfolding");
	treeMan_->addBranch("pseudoTTbar_y", "F","Unfolding");
	treeMan_->addBranch("pseudoTTbar_m", "F","Unfolding");
	// Branches for lepton
	treeMan_->addBranch("pseudoLepton_pT", "F","Unfolding");
	treeMan_->addBranch("pseudoLepton_eta", "F","Unfolding");
	// Branches for b jets
	treeMan_->addBranch("pseudoB_pT", "F", "Unfolding", false);
	treeMan_->addBranch("pseudoB_eta", "F", "Unfolding", false);
	// Branch for pseudo MET
	treeMan_->addBranch("pseudoMET", "F","Unfolding");
	treeMan_->addBranch("pseudoMET_mass", "F","Unfolding");
	// Branches for other pseudo global variables
	treeMan_->addBranch("pseudoHT", "F","Unfolding");
	treeMan_->addBranch("pseudoST", "F","Unfolding");
	treeMan_->addBranch("pseudoWPT_reco", "F","Unfolding");
	treeMan_->addBranch("pseudoWPT", "F","Unfolding");
	treeMan_->addBranch("pseudoMT", "F","Unfolding");

	// Number of pseudo jets
	treeMan_->addBranch("NPseudoJets", "F", "Unfolding");
	treeMan_->addBranch("NPseudoBJets", "F", "Unfolding");

}

bool PseudoTopAnalyser::passesEventSelection( const MCParticlePointer pseudoLepton, const ParticlePointer pseudoNeutrino, const JetCollection pseudoJets, const MCParticleCollection pseudoBs, const ParticleCollection allPseudoLeptons, const ParticlePointer pseudoMET ) {

	// Event selection taken from here : https://twiki.cern.ch/twiki/bin/view/LHCPhysics/ParticleLevelTopDefinitions
	unsigned int numberGoodLeptons = 0;
	unsigned int numberVetoLeptons = 0;
	ParticlePointer leadingLepton;
	for ( unsigned int leptonIndex = 0; leptonIndex < allPseudoLeptons.size(); ++ leptonIndex ) {
		const ParticlePointer lepton = allPseudoLeptons.at(leptonIndex);

		// Check if this is a good signal type lepton
		if ( lepton->pt() > minLeptonPt_ && fabs(lepton->eta()) < maxLeptonAbsEta_ ) {
			++numberGoodLeptons;
			if ( leadingLepton == 0 ) leadingLepton = lepton;
		}
		
		// Check if this is a veto lepton
		if ( lepton->pt() > minVetoLeptonPt_ && fabs(lepton->eta()) < maxVetoLeptonAbsEta_ ) {
			++numberVetoLeptons;
		}
	}

	// Neutrino pt sum
	bool passesNeutrinoSumPt = false;
	if ( pseudoMET != 0 ) {
		if ( pseudoMET->pt() > minNeutrinoSumPt_ ) passesNeutrinoSumPt = true;
	}

	// W MT
	bool passesWMT = false;
	if ( leadingLepton != 0 && pseudoMET != 0 ) {
		double genMT = sqrt( 2 * leadingLepton->pt() * pseudoMET->pt() * ( 1 - cos(leadingLepton->phi() - pseudoMET->phi() ) ) );
		if (genMT > minWMt_) passesWMT = true;
	}


	// Jets
	unsigned int numberGoodJets = 0;
	unsigned int numberGoodBJets = 0;

	for ( unsigned int jetIndex = 0; jetIndex < pseudoJets.size(); ++ jetIndex ) {
		const JetPointer jet = pseudoJets.at(jetIndex);

		// Check if this is a good jet
		if ( jet->pt() > minJetPt_ && fabs(jet->eta()) < maxJetAbsEta_ ) {
			++numberGoodJets;

			// Check if this is also a good b jet
			if ( fabs( jet->partonFlavour() ) == 5 ) {
				++numberGoodBJets;
			}
		}
	}
	
	if ( numberGoodLeptons == 1 && numberVetoLeptons <= 1 && passesNeutrinoSumPt && passesWMT && numberGoodJets >= minNJets_ && numberGoodBJets >= minNBJets_ ) {
		return true;
	}
	else return false;

}

void PseudoTopAnalyser::createHistograms() {

cout << "I shouldn't be here" << endl;

}

PseudoTopAnalyser::PseudoTopAnalyser(boost::shared_ptr<HistogramManager> histMan, TreeManagerPtr treeMan, std::string histogramFolder) :
		BasicAnalyser(histMan, treeMan, histogramFolder) {
}

PseudoTopAnalyser::~PseudoTopAnalyser() {
}

} /* namespace BAT */
