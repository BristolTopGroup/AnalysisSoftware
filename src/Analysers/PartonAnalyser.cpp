/*
 * PartonAnalyser.cpp
 *
 *  Created on: 24 Feb 2015
 *      Author: ejclemen
 */

#include "../../interface/Analysers/PartonAnalyser.h"

#include <boost/scoped_ptr.hpp>
#include <iostream>

using namespace std;

namespace BAT {

void PartonAnalyser::analyse(const EventPtr event) {

	if ( !( event->isSemiLeptonicElectron() || event->isSemiLeptonicMuon() ) ) return;

	weight_ = event->weight();

	treeMan_->setCurrentFolder(histogramFolder_);

	const TTGenInfoPointer ttGen( event->TTGenInfo() );
	const ParticlePointer leptonicTop = ttGen->getLeptonicTop();
	const ParticlePointer hadronicTop = ttGen->getHadronicTop();

	// const ParticlePointer ttbar = leptonicTop + hadronicTop
	// ParticleCollection topsForTTbar;

	// Only consider events where both top partons are available
	if ( leptonicTop == 0 || hadronicTop == 0 ) return;

	ParticlePointer ttbar( new Particle(*leptonicTop + * hadronicTop ) );

	// Store info on leptonic top
	treeMan_->Fill("lepTopPt_parton", leptonicTop->pt() );
	treeMan_->Fill("lepTopRap_parton", leptonicTop->rapidity() );

	// Store info on hadronic top
	treeMan_->Fill("hadTopPt_parton", hadronicTop->pt() );
	treeMan_->Fill("hadTopRap_parton", hadronicTop->rapidity() );

	// Store info on ttbar
	treeMan_->Fill("ttbarPt_parton", ttbar->pt() );
	treeMan_->Fill("ttbarRap_parton", ttbar->rapidity() );
	treeMan_->Fill("ttbarM_parton", ttbar->mass() );


	// Store info on lepton
	const ParticlePointer lepton = ttGen->getsinglelepton();
	treeMan_->Fill("leptonPt_parton", lepton->pt() );
	treeMan_->Fill("leptonEta_parton", lepton->eta() );

	// Store info on neutrino
	const ParticlePointer neutrino = ttGen->getsingleneutrino();
	treeMan_->Fill("neutrinoPt_parton", neutrino->pt() );
	treeMan_->Fill("neutrinoEta_parton", neutrino->eta() );

	// Store info on leptonic W
	const ParticlePointer leptonicW = ttGen->getLeptonicW();
	treeMan_->Fill("lepWPt_parton", leptonicW->pt() );
	treeMan_->Fill("lepWEta_parton", leptonicW->eta() );
}

void PartonAnalyser::createTrees() {

	treeMan_->setCurrentFolder(histogramFolder_);

	treeMan_->addBranch("lepTopPt_parton", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("hadTopPt_parton", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("lepTopRap_parton", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("hadTopRap_parton", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("ttbarPt_parton", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("ttbarM_parton", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("ttbarRap_parton", "F", "Unfolding" + Globals::treePrefix_);

	treeMan_->addBranch("leptonPt_parton", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("leptonEta_parton", "F", "Unfolding" + Globals::treePrefix_);

	treeMan_->addBranch("neutrinoPt_parton", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("neutrinoEta_parton", "F", "Unfolding" + Globals::treePrefix_);

	treeMan_->addBranch("lepWPt_parton", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("lepWEta_parton", "F", "Unfolding" + Globals::treePrefix_);
}

PartonAnalyser::PartonAnalyser(TreeManagerPtr treeMan, std::string histogramFolder) :
		BasicAnalyser(treeMan, histogramFolder) {
}

PartonAnalyser::~PartonAnalyser() {
}

} /* namespace BAT */
