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

	treeMan_->setCurrentFolder(histogramFolder_);

	ParticleCollection pseudoTopsForTTbar;

	for ( unsigned int ptIndex = 0; ptIndex < pseudoTops.size(); ++ ptIndex ) {
		// cout << "Getting pseudo top number : " << ptIndex << endl;
		const ParticlePointer pseudoTop = pseudoTops[ptIndex];
		pseudoTopsForTTbar.push_back( pseudoTop );
		treeMan_->Fill("pseudoTop_pT", pseudoTop->pt() );
		treeMan_->Fill("pseudoTop_y", pseudoTop->rapidity() );
	}

	if ( pseudoTopsForTTbar.size() == 2 ) {
		ParticlePointer pseudoTTbar( new Particle( *pseudoTopsForTTbar[0] + *pseudoTopsForTTbar[1] ) );

		treeMan_->Fill("pseudoTTbar_pT", pseudoTTbar->pt() );
		treeMan_->Fill("pseudoTTbar_y", pseudoTTbar->rapidity() );
		treeMan_->Fill("pseudoTTbar_m", pseudoTTbar->mass() );
	}
}

void PseudoTopAnalyser::createTrees() {

	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->addBranch("pseudoTop_pT","pseudoTop_pT","PseudoTop");
	treeMan_->addBranch("pseudoTop_y","pseudoTop_y","PseudoTop");
	treeMan_->addBranch("pseudoTTbar_pT","pseudoTTbar_pT","PseudoTop");
	treeMan_->addBranch("pseudoTTbar_y","pseudoTTbar_y","PseudoTop");
	treeMan_->addBranch("pseudoTTbar_m","pseudoTTbar_m","PseudoTop");

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
