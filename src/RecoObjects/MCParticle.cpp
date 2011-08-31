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

MCParticle::MCParticle(double energy, double px, double py, double pz): Particle(energy, px, py, pz),
		particlePdgId(0), particleMotherIndex(0), particleStatus(0){

}

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

bool MCParticle::isQuark() const {
	if ((abs(pdgId())>0) && (abs(pdgId())<9)) return true; else	return false;
}

bool MCParticle::isLepton() const {
	if ((abs(pdgId())>10) && (abs(pdgId())<19)) return true; else	return false;
}

bool MCParticle::isNeutrino() const {
	if ((abs(pdgId())==12) || (abs(pdgId())==14) || (abs(pdgId())==16) || (abs(pdgId())==18)) return true; else return false;
}

unsigned short MCParticle::getClosestJetIndex(const JetCollection& jets) const {
    unsigned short idOfClosest = 999;
    double closestDR = 999.;
    for (unsigned short index = 0; index < jets.size(); ++index) {
        double DR = deltaR(jets.at(index));
        if (DR < closestDR) {
            closestDR = DR;
            idOfClosest = index;
        }
    }
    return idOfClosest;
}

}
