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
		pseudoTops_(),
		pseudoLeptonicW_( new MCParticle() ),
		pseudoLepton_( new MCParticle() ),
		pseudoJets_(),
		pseudoBs_(),
		pseudoNeutrino_( new Particle() ),
		pseudoMET_( new Particle() ),
		isSemiLeptonic_(false)
{
}

// PseudoTopParticles::PseudoTopParticles(double energy, double px, double py, double pz) :
// 		Particle(energy, px, py, pz), //
// 		particlePdgId(0), //
// 		particleMotherIndex(0), //
// 		particleStatus(0) {

// }

PseudoTopParticles::~PseudoTopParticles() {
}

void PseudoTopParticles::setPseudoTops( MCParticleCollection newPseudoTops ) {
	pseudoTops_.clear();
	pseudoTops_ = newPseudoTops;
}

MCParticleCollection PseudoTopParticles::getPseudoTops() const {
	return pseudoTops_;
}

void PseudoTopParticles::setPseudoLeptonicW( MCParticlePointer newLeptonicW ) {
	pseudoLeptonicW_ = newLeptonicW;
}

MCParticlePointer PseudoTopParticles::getPseudoLeptonicW() const {
	return pseudoLeptonicW_;
}

void PseudoTopParticles::setPseudoLepton( MCParticlePointer newPseudoLepton ) {
	pseudoLepton_ = newPseudoLepton;
}

MCParticlePointer PseudoTopParticles::getPseudoLepton() const {
	return pseudoLepton_;
}

void PseudoTopParticles::setPseudoJets( JetCollection newPseudoJets ) {
	pseudoJets_.clear();
	pseudoJets_ = newPseudoJets;
}

JetCollection PseudoTopParticles::getPseudoJets() const {
	return pseudoJets_;
}

void PseudoTopParticles::setAllPseudoLeptons( ParticleCollection newPseudoLeptons ) {
	allPseudoLeptons_.clear();
	allPseudoLeptons_ = newPseudoLeptons;
}

ParticleCollection PseudoTopParticles::getAllPseudoLeptons() const {
	return allPseudoLeptons_;
}

void PseudoTopParticles::setPseudoBs( MCParticleCollection newPseudoBs ) {
	pseudoBs_.clear();
	pseudoBs_ = newPseudoBs;
}

MCParticleCollection PseudoTopParticles::getPseudoBs() const {
	return pseudoBs_;
}

void PseudoTopParticles::setPseudoNeutrino( ParticlePointer newPseudoNeutrino ) {
	pseudoNeutrino_ = newPseudoNeutrino;
}

ParticlePointer PseudoTopParticles::getPseudoNeutrino() const {
	return pseudoNeutrino_;
}

void PseudoTopParticles::setPseudoMET( ParticlePointer newPseudoMET ) {
	pseudoMET_ = newPseudoMET;
}

ParticlePointer PseudoTopParticles::getPseudoMET() const {
	return pseudoMET_;
}

void PseudoTopParticles::setIsSemiLeptonic( bool isSemiLeptonic ) {
	isSemiLeptonic_ = isSemiLeptonic;
}

bool PseudoTopParticles::isSemiLeptonic() const {
	return isSemiLeptonic_;
}

}
