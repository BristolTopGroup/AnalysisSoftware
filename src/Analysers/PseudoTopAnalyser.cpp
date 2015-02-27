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

void PseudoTopAnalyser::analyse(const EventPtr event) {
	weight_ = event->weight();

	const PseudoTopParticlesPointer pseudoTopParticles = event->PseudoTopParticles();
	const ParticleCollection pseudoTops = pseudoTopParticles->getPseudoTops();
	const ParticlePointer pseudoLeptonicW = pseudoTopParticles->getPseudoLeptonicW();
	const MCParticlePointer pseudoLepton = pseudoTopParticles->getPseudoLepton();
	const ParticleCollection pseudoBs = pseudoTopParticles->getPseudoBs();
	const ParticlePointer pseudoMET = pseudoTopParticles->getPseudoMET();
	const ParticlePointer pseudoNeutrino = pseudoTopParticles->getPseudoNeutrino();
	const JetCollection pseudoJets = pseudoTopParticles->getPseudoJets();

	treeMan_->setCurrentFolder(histogramFolder_);

	ParticleCollection pseudoTopsForTTbar;

	// Only consider events with two pseudo tops
	if ( pseudoTops.size() != 2 ) return;

	// Also only consider events that are semi leptonic at the pseudo top level
	if ( !pseudoTopParticles->isSemiLeptonic() ) return;

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
	// Branches for top
	treeMan_->addBranch("pseudoTop_pT","pseudoTop_pT","Unfolding");
	treeMan_->addBranch("pseudoTop_y","pseudoTop_y","Unfolding");
	// Branches for ttbar
	treeMan_->addBranch("pseudoTTbar_pT","pseudoTTbar_pT","Unfolding");
	treeMan_->addBranch("pseudoTTbar_y","pseudoTTbar_y","Unfolding");
	treeMan_->addBranch("pseudoTTbar_m","pseudoTTbar_m","Unfolding");
	// Branches for lepton
	treeMan_->addBranch("pseudoLepton_pT","pseudoLepton_pT","Unfolding");
	treeMan_->addBranch("pseudoLepton_eta","pseudoLepton_eta","Unfolding");
	// Branches for b jets
	treeMan_->addBranch("pseudoB_pT","pseudoB_pT","Unfolding");
	treeMan_->addBranch("pseudoB_eta","pseudoB_eta","Unfolding");
	// Branch for pseudo MET
	treeMan_->addBranch("pseudoMET","pseudoMET","Unfolding");
	treeMan_->addBranch("pseudoMET_mass","pseudoMET_mass","Unfolding");
	// Branches for other pseudo global variables
	treeMan_->addBranch("pseudoHT","pseudoHT","Unfolding");
	treeMan_->addBranch("pseudoST","pseudoST","Unfolding");
	treeMan_->addBranch("pseudoWPT","pseudoWPT","Unfolding");
	treeMan_->addBranch("pseudoMT","pseudoMT","Unfolding");

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
