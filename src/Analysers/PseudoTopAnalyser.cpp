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

double const PseudoTopAnalyser::minLeptonPt_ = 30;
double const PseudoTopAnalyser::maxLeptonAbsEta_ = 2.4;
double const PseudoTopAnalyser::minVetoLeptonPt_ = 15;
double const PseudoTopAnalyser::maxVetoLeptonAbsEta_ = 2.5;
double const PseudoTopAnalyser::minNeutrinoSumPt_ = 30;
double const PseudoTopAnalyser::minWMt_ = 30;
unsigned int const PseudoTopAnalyser::minNJets_ = 4;
unsigned int const PseudoTopAnalyser::minNBJets_ = 2;
double const PseudoTopAnalyser::minJetPt_ = 30;
double const PseudoTopAnalyser::maxJetAbsEta_ = 2.4;

void PseudoTopAnalyser::analyse(const EventPtr event) {
	weight_ = event->weight();

	treeMan_->setCurrentFolder(histogramFolder_);

	// Store gen selection criteria
	treeMan_->Fill("isSemiLeptonicElectron", event->isSemiLeptonicElectron());
	treeMan_->Fill("isSemiLeptonicMuon", event->isSemiLeptonicMuon());

	// cout << Bool_t(event->isSemiLeptonicElectron()) << " " << event->isSemiLeptonicElectron() << " " << event->isSemiLeptonicMuon() << endl;

	const PseudoTopParticlesPointer pseudoTopParticles = event->PseudoTopParticles();
	const ParticleCollection pseudoTops = pseudoTopParticles->getPseudoTops();
	const ParticlePointer pseudoLeptonicW = pseudoTopParticles->getPseudoLeptonicW();
	const MCParticlePointer pseudoLepton = pseudoTopParticles->getPseudoLepton();
	const ParticleCollection allPseudoLeptons = pseudoTopParticles->getAllPseudoLeptons();
	const ParticleCollection pseudoBs = pseudoTopParticles->getPseudoBs();
	const ParticlePointer pseudoMET = pseudoTopParticles->getPseudoMET();
	const ParticlePointer pseudoNeutrino = pseudoTopParticles->getPseudoNeutrino();
	const JetCollection pseudoJets = pseudoTopParticles->getPseudoJets();

	ParticleCollection pseudoTopsForTTbar;

	// Only consider events with two pseudo tops
	if ( pseudoTops.size() != 2 ) return;

	// Also only consider events that are semi leptonic at the pseudo top level
	if ( !pseudoTopParticles->isSemiLeptonic() ) return;

	// Check if event passes event selection (at pseudo top level)
	if ( passesEventSelection( pseudoLepton, pseudoNeutrino, pseudoJets, pseudoBs, allPseudoLeptons, pseudoMET ) ) {
		treeMan_->Fill("passesEventSelection",1);
	}
	else {
		treeMan_->Fill("passesEventSelection",0);
	}

	// Store info on top
	for ( unsigned int ptIndex = 0; ptIndex < pseudoTops.size(); ++ ptIndex ) {
		// cout << "Getting pseudo top number : " << ptIndex << endl;
		const ParticlePointer pseudoTop = pseudoTops[ptIndex];
		pseudoTopsForTTbar.push_back( pseudoTop );
		treeMan_->Fill("pseudoTop_pT", pseudoTop->pt() );
		treeMan_->Fill("pseudoTop_y", pseudoTop->rapidity() );
	}

	// Store info on ttbar
	ParticlePointer pseudoTTbar( new Particle( *pseudoTopsForTTbar[0] + *pseudoTopsForTTbar[1] ) );
	treeMan_->Fill("pseudoTTbar_pT", pseudoTTbar->pt() );
	treeMan_->Fill("pseudoTTbar_y", pseudoTTbar->rapidity() );
	treeMan_->Fill("pseudoTTbar_m", pseudoTTbar->mass() );


	// Store info on lepton
	treeMan_->Fill("pseudoLepton_pT", pseudoLepton->pt() );
	treeMan_->Fill("pseudoLepton_eta", pseudoLepton->eta() );

	// Store info on Bs
	for ( unsigned int pbIndex = 0; pbIndex < pseudoBs.size(); ++ pbIndex ) {
		treeMan_->Fill("pseudoB_pT", pseudoBs[pbIndex]->pt() );
		treeMan_->Fill("pseudoB_eta", pseudoBs[pbIndex]->eta() );
	}

	// Store pseudo MET
	treeMan_->Fill("pseudoMET", pseudoMET->et() );
	treeMan_->Fill("pseudoMET_mass", pseudoMET->mass() );

	// Store pseudo HT
	treeMan_->Fill("pseudoHT", event->HT( pseudoJets ) );

	// Store pseudo ST
	treeMan_->Fill("pseudoST", event->ST( pseudoJets, pseudoLepton, METPointer( new MET( pseudoMET->px(), pseudoMET->py() )) ) );

	// Store pseudo MT and WPT
	// These are from the W reconstructed from the pseudo particles
	// i.e. use the neutrino assocaited with the W rather than the more "global" MET

	METPointer metFromNeutrino( new MET( pseudoNeutrino->px(), pseudoNeutrino->py() ) );
	double MT = event->MT( pseudoLepton, metFromNeutrino );
	treeMan_->Fill("pseudoWPT", pseudoLeptonicW->pt() );
	treeMan_->Fill("pseudoMT", MT );
}

void PseudoTopAnalyser::createTrees() {

	treeMan_->setCurrentFolder(histogramFolder_);
	// Generator level selecton
	treeMan_->addBranch("isSemiLeptonicElectron", "F","Unfolding");
	treeMan_->addBranch("isSemiLeptonicMuon", "F","Unfolding");

	treeMan_->addBranch("passesEventSelection","F","Unfolding");

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
	treeMan_->addBranch("pseudoWPT", "F","Unfolding");
	treeMan_->addBranch("pseudoMT", "F","Unfolding");

}

bool PseudoTopAnalyser::passesEventSelection( const ParticlePointer pseudoLepton, const ParticlePointer pseudoNeutrino, const JetCollection pseudoJets, const ParticleCollection pseudoBs, const ParticleCollection allPseudoLeptons, const ParticlePointer pseudoMET ) {

	// Event selection taken from here : https://twiki.cern.ch/twiki/bin/view/LHCPhysics/ParticleLevelTopDefinitions
	unsigned int numberGoodLeptons = 0;
	unsigned int numberVetoLeptons = 0;
	for ( unsigned int leptonIndex = 0; leptonIndex < allPseudoLeptons.size(); ++ leptonIndex ) {
		const ParticlePointer lepton = allPseudoLeptons.at(leptonIndex);

		// Check if this is a good signal type lepton
		if ( lepton->pt() > minLeptonPt_ && fabs(lepton->eta()) > maxLeptonAbsEta_ ) {
			++numberGoodLeptons;
		}
		
		// Check if this is a veto lepton
		if ( lepton->pt() > minVetoLeptonPt_ && fabs(lepton->eta()) > maxVetoLeptonAbsEta_ ) {
			++numberVetoLeptons;
		}
	}

	// Neutrino pt sum
	bool passesNeutrinoSumPt = false;
	if ( pseudoMET->pt() > minNeutrinoSumPt_ ) passesNeutrinoSumPt = true;

	// W MT
	double genMT = sqrt( 2 * pseudoLepton->pt() * pseudoNeutrino->pt() * ( 1 - cos(pseudoLepton->phi() - pseudoNeutrino->phi() ) ) );
	bool passesWMT = false;
	if (genMT > minWMt_) passesWMT = true;

	// Jets
	unsigned int numberGoodJets = 0;
	for ( unsigned int jetIndex = 0; jetIndex < pseudoJets.size(); ++ jetIndex ) {
		const JetPointer jet = pseudoJets.at(jetIndex);

		// Check if this is a good jet
		if ( jet->pt() > minJetPt_ && fabs(jet->eta()) > maxJetAbsEta_ ) {
			++numberGoodJets;
		}		
	}

	// B Jets
	unsigned int numberGoodBJets = 0;
	for ( unsigned int jetIndex = 0; jetIndex < pseudoBs.size(); ++ jetIndex ) {
		const ParticlePointer bjet = pseudoBs.at(jetIndex);

		// Check if this is a good jet
		if ( bjet->pt() > minJetPt_ && fabs(bjet->eta()) > maxJetAbsEta_ ) {
			++numberGoodBJets;
		}		
	}
	

	if ( numberGoodLeptons == 1 && numberVetoLeptons <= 1 && passesNeutrinoSumPt && passesWMT && numberGoodJets >= minNJets_ && numberGoodBJets >= minNBJets_ ) return true;
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
