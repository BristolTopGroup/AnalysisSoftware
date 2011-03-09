/*
 * MCParticle.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/MCParticle.h"

namespace BAT {

MCParticle::MCParticle() : Particle(),
	particlePdgId(0), particleMotherIndex(0), particleStatus(0){

}

MCParticle::MCParticle(float energy, float px, float py, float pz): Particle(energy, px, py, pz),
		particlePdgId(0), particleMotherIndex(0), particleStatus(0){

}
//MCParticle::Particle(const MCParticle& particle) :
//	particlePdgId(particle.pdgId()), particleMotherIndex(particle.motherIndex()), particleStatus(particle.status) {
//
//}

//MCParticle::Particle(int pdgId, float phi, float eta, float pt, float energy, int motherIndex, int status) {
//
//}

//MCParticle::Particle(): Particle() {
//
//}

MCParticle::~MCParticle() {
}

int MCParticle::pdgId() const {
	return particlePdgId;
}

int MCParticle::motherIndex() const {
	return particleMotherIndex;
}

int MCParticle::status() const {
	return particleStatus;
}

void MCParticle::setStatus(int Status) {
	particleStatus = Status;
}

void MCParticle::setMotherIndex(int MotherIndex){
	particleMotherIndex = MotherIndex;
}

void MCParticle::setPdgId(int PdgId){
	particlePdgId = PdgId;
}


}
