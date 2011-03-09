/*
 * Particle.cpp
 *
 *  Created on: 2 Jul 2010
 *      Author: kreczko
 */

#include "../../interface/RecoObjects/Particle.h"
#include "../../interface/DetectorGeometry.h"

namespace BAT {

Particle::Particle() :
    particleMass(0), particleCharge(0), distanceFromInteractionPointInMicron(999999),
            distanceFromInteractionPointInMicron_wrt_to_BeamSpot(999999), fourvector(0., 0., 0., 0.) {

}

Particle::Particle(const Particle& particle) :
    particleMass(particle.mass()), particleCharge(particle.charge()), distanceFromInteractionPointInMicron(
            particle.d0()), distanceFromInteractionPointInMicron_wrt_to_BeamSpot(particle.d0_wrtBeamSpot()), fourvector(
            particle.getFourVector()) {

}

Particle::Particle(float energy, float px, float py, float pz) :
    particleMass(0),
    particleCharge(0),
    distanceFromInteractionPointInMicron(99999),
    distanceFromInteractionPointInMicron_wrt_to_BeamSpot(999999),
    fourvector(px, py, pz, energy) {
}

Particle::~Particle() {
}

float Particle::mass() const {
    if (particleMass == 0)
        return massFromEnergyAndMomentum();
    else
        return particleMass;
}

float Particle::d0() const {
    return distanceFromInteractionPointInMicron;
}

float Particle::d0_wrtBeamSpot() const {
    return distanceFromInteractionPointInMicron_wrt_to_BeamSpot;
}

float Particle::energy() const {
    return fourvector.Energy();
}

float Particle::et() const {
    return fourvector.Et();
}

float Particle::px() const {
    return fourvector.Px();
}

float Particle::py() const {
    return fourvector.Py();
}

float Particle::pz() const {
    return fourvector.Pz();
}

float Particle::pt() const {
    return fourvector.Pt();
}

float Particle::eta() const {
    return fourvector.Eta();
}

float Particle::phi() const {
    return fourvector.Phi();
}

float Particle::theta() const {
    return fourvector.Theta();
}

float Particle::massFromEnergyAndMomentum() const {
    return fourvector.M();
}

float Particle::charge() const {
    return particleCharge;
}

void Particle::setMass(float mass) {
    particleMass = mass;
}

void Particle::setD0(float d0) {
    distanceFromInteractionPointInMicron = d0;
}

void Particle::setD0_wrtBeamSpot(float d0) {
    distanceFromInteractionPointInMicron_wrt_to_BeamSpot = d0;
}

const FourVector& Particle::getFourVector() const {
    return fourvector;
}

void Particle::setFourVector(FourVector vector) {
    fourvector = vector;
}

void Particle::setCharge(float charge) {
    particleCharge = charge;
}

Particle & Particle::operator =(const Particle &rightHandSide) {
    if (this == &rightHandSide)
        return *this;
    fourvector = rightHandSide.getFourVector();
//    particleMass = rightHandSide.mass();
    return *this;
}

const Particle Particle::operator +(const Particle &other) const {
    Particle result = *this;
    FourVector vector = result.getFourVector() + other.getFourVector();
    result.setFourVector(vector);
    result.setMass(0);
    result.setCharge(result.charge() + other.charge());
    return result;
}

bool Particle::isInBarrelRegion() const {
    return fabs(eta()) < Detector::Barrel::MaximalAbsoluteEta;
}

bool Particle::isInCrack() const {
    bool passesMinimalEta = fabs(eta()) > Detector::Crack::MinimalAbsoluteEta;
    bool passesMaximalEta = fabs(eta()) < Detector::Crack::MaximalAbsoluteEta;
    return passesMinimalEta && passesMaximalEta;
}

bool Particle::isInEndCapRegion() const {
    return fabs(eta()) > Detector::EndCap::MinimalAbsoluteEta;
}

const char* Particle::getEtaRegion() const {
    if (isInBarrelRegion())
        return "barrel";
    else if (isInCrack())
        return "crack";
    else if (isInEndCapRegion())
        return "endcap";
    else
        return "unknown";
}

float Particle::deltaEta(const ParticlePointer other) const {
    return fabs(eta() - other->eta());
}

float Particle::deltaPhi(const ParticlePointer other) const {
    return fourvector.DeltaPhi(other->getFourVector());
}

float Particle::deltaR(const ParticlePointer other) const {
    return fourvector.DeltaR(other->getFourVector());
}

bool Particle::isWithinDeltaR(float delta_R, const ParticlePointer particle) const {
    return deltaR(particle) < delta_R;
}

float Particle::invariantMass(const ParticlePointer otherParticle) const {
    TLorentzVector combinedParticle(fourvector + otherParticle->getFourVector());
    return combinedParticle.M();
}

float Particle::relativePtTo(const ParticlePointer otherParticle) const {
    float relativePt = fourvector.Perp(otherParticle->getFourVector().Vect());
    return fabs(relativePt);
}

unsigned short Particle::getClosest(const ParticleCollection& particles) const {
    unsigned short idOfClosest = 999;
    float closestDR = 999.;
    for (unsigned short index = 0; index < particles.size(); ++index) {
        float DR = deltaR(particles.at(index));
        if (DR < closestDR) {
            closestDR = DR;
            idOfClosest = index;
        }
    }
    return idOfClosest;
}

float Particle::angle(const ParticlePointer otherParticle) const {
    return fourvector.Angle(otherParticle->getFourVector().Vect());
}
}
