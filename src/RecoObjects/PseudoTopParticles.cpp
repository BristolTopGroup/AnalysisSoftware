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
		tau1_( -1 ),
		tau2_( -1 ),
		tau3_( -1 ),
		tau4_( -1 ),
		tau5_( -1 ),
		tau6_( -1 ),
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

void PseudoTopParticles::setAllPseudoLeptons( MCParticleCollection newPseudoLeptons ) {
	allPseudoLeptons_.clear();
	allPseudoLeptons_ = newPseudoLeptons;
}

MCParticleCollection PseudoTopParticles::getAllPseudoLeptons() const {
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

void PseudoTopParticles::setPseudoTau1( double tau1 ) {
	tau1_ = tau1;
}

double PseudoTopParticles::getPseudoTau1() const {
	return tau1_;
}

void PseudoTopParticles::setPseudoTau2( double tau2 ) {
	tau2_ = tau2;
}

double PseudoTopParticles::getPseudoTau2() const {
	return tau2_;
}

void PseudoTopParticles::setPseudoTau3( double tau3 ) {
	tau3_ = tau3;
}

double PseudoTopParticles::getPseudoTau3() const {
	return tau3_;
}

void PseudoTopParticles::setPseudoTau4( double tau4 ) {
	tau4_ = tau4;
}

double PseudoTopParticles::getPseudoTau4() const {
	return tau4_;
}

void PseudoTopParticles::setPseudoTau5( double tau5 ) {
	tau5_ = tau5;
}

double PseudoTopParticles::getPseudoTau5() const {
	return tau5_;
}

void PseudoTopParticles::setPseudoTau6( double tau6 ) {
	tau6_ = tau6;
}

double PseudoTopParticles::getPseudoTau6() const {
	return tau6_;
}

void PseudoTopParticles::setIsSemiLeptonic( bool isSemiLeptonic ) {
	isSemiLeptonic_ = isSemiLeptonic;
}

bool PseudoTopParticles::isSemiLeptonic() const {
	return isSemiLeptonic_;
}

}
