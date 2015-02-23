/*
 * PseudoTopParticles.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: ejclemen
 */

#include "../../interface/RecoObjects/PseudoTopParticles.h"
#include <iostream>

using namespace std;

namespace BAT {

PseudoTopParticles::PseudoTopParticles() :
		pseudoTops_() {
}

// PseudoTopParticles::PseudoTopParticles(double energy, double px, double py, double pz) :
// 		Particle(energy, px, py, pz), //
// 		particlePdgId(0), //
// 		particleMotherIndex(0), //
// 		particleStatus(0) {

// }

PseudoTopParticles::~PseudoTopParticles() {
}

void PseudoTopParticles::setPseudoTops( ParticleCollection newPseudoTops ) {
	pseudoTops_.clear();
	pseudoTops_ = newPseudoTops;
}

ParticleCollection PseudoTopParticles::getPseudoTops() const {
	return pseudoTops_;
}

}
