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

	histMan_->setCurrentHistogramFolder(histogramFolder_);

	ParticleCollection pseudoTopsForTTbar;

	for ( unsigned int ptIndex = 0; ptIndex < pseudoTops.size(); ++ ptIndex ) {
		// cout << "Getting pseudo top number : " << ptIndex << endl;
		const ParticlePointer pseudoTop = pseudoTops[ptIndex];
		pseudoTopsForTTbar.push_back( pseudoTop );
		histMan_->H1D("pseudoTop_pT")->Fill(pseudoTop->pt() , weight_);
		histMan_->H1D("pseudoTop_y")->Fill(pseudoTop->rapidity() , weight_);
	}

	if ( pseudoTopsForTTbar.size() == 2 ) {
		ParticlePointer pseudoTTbar( new Particle( *pseudoTopsForTTbar[0] + *pseudoTopsForTTbar[1] ) );

		histMan_->H1D("pseudoTTbar_pT")->Fill(pseudoTTbar->pt() , weight_);
		histMan_->H1D("pseudoTTbar_y")->Fill(pseudoTTbar->rapidity() , weight_);
		histMan_->H1D("pseudoTTbar_m")->Fill(pseudoTTbar->mass() , weight_);
	}
}

void PseudoTopAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	histMan_->addH1D("pseudoTop_pT", "pseudoTop pT; p_{T}^{t} [GeV]; events/20 GeV", 25, 0, 500);
	histMan_->addH1D("pseudoTop_y", "pseudoTop y; y^t; ; events/0.1", 50, -2.5, 2.5);

	histMan_->addH1D("pseudoTTbar_pT", "pseudoTTbar pT; p_{T}^{t#bar{t}} [GeV]; events/20 GeV", 25, 0, 500);
	histMan_->addH1D("pseudoTTbar_y", "pseudoTTbar y; y^{t#bar{t}} [GeV]; events/0.1", 50, -2.5, 2.5);
	histMan_->addH1D("pseudoTTbar_m", "pseudoTTbar m; m^{t#bar{t}} [GeV]; events/20 GeV", 80, 0, 1600);

}

PseudoTopAnalyser::PseudoTopAnalyser(boost::shared_ptr<HistogramManager> histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {
}

PseudoTopAnalyser::~PseudoTopAnalyser() {
}

} /* namespace BAT */
