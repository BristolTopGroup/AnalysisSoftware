/*
 * Particle.cpp
 *
 *  Created on: 2 Jul 2010
 *      Author: kreczko
 */

#include "../../interface/RecoObjects/Particle.h"

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

Particle::Particle(double energy, double px, double py, double pz) :
    particleMass(0),
    particleCharge(0),
    distanceFromInteractionPointInMicron(99999),
    distanceFromInteractionPointInMicron_wrt_to_BeamSpot(999999),
    fourvector(px, py, pz, energy) {
}

Particle::~Particle() {
}

double Particle::mass() const {
    if (particleMass == 0)
        return massFromEnergyAndMomentum();
    else
        return particleMass;
}

double Particle::d0() const {
    return distanceFromInteractionPointInMicron;
}

double Particle::d0_wrtBeamSpot() const {
    return distanceFromInteractionPointInMicron_wrt_to_BeamSpot;
}

double Particle::energy() const {
    return fourvector.Energy();
}

double Particle::et() const {
    return fourvector.Et();
}

double Particle::px() const {
    return fourvector.Px();
}

double Particle::py() const {
    return fourvector.Py();
}

double Particle::pz() const {
    return fourvector.Pz();
}

double Particle::pt() const {
    return fourvector.Pt();
}

double Particle::eta() const {
    return fourvector.Eta();
}

double Particle::phi() const {
    return fourvector.Phi();
}

double Particle::theta() const {
    return fourvector.Theta();
}

double Particle::massFromEnergyAndMomentum() const {
    return fourvector.M();
}

double Particle::charge() const {
    return particleCharge;
}

void Particle::setMass(double mass) {
    particleMass = mass;
}

void Particle::setD0(double d0) {
    distanceFromInteractionPointInMicron = d0;
}

void Particle::setD0_wrtBeamSpot(double d0) {
    distanceFromInteractionPointInMicron_wrt_to_BeamSpot = d0;
}

const FourVector& Particle::getFourVector() const {
    return fourvector;
}

void Particle::setFourVector(FourVector vector) {
    fourvector = vector;
}

void Particle::setCharge(double charge) {
    particleCharge = charge;
}

Particle & Particle::operator =(const Particle &rightHandSide) {
    if (this == &rightHandSide)
        return *this;
    fourvector = rightHandSide.getFourVector();
    particleCharge = rightHandSide.charge();
    return *this;
}

Particle Particle::operator +(const Particle &other) const {
    Particle result(*this);
    FourVector vector = result.getFourVector() + other.getFourVector();
    result.setFourVector(vector);
    result.setMass(0);
    result.setCharge(result.charge() + other.charge());
    return result;
}

//ParticlePointer Particle::operator +(const Particle* other) const {
//    ParticlePointer result(new Particle(*this));
//    FourVector vector = result->getFourVector() + other->getFourVector();
//    result->setFourVector(vector);
//    result->setMass(0);
//    result->setCharge(result->charge() + other->charge());
//    return result;
//}

bool Particle::isInBarrelRegion() const {
    return fabs(eta()) < 1.4442;
}

bool Particle::isInCrack() const {
	return !isInBarrelRegion() && !isInEndCapRegion();
}

bool Particle::isInEndCapRegion() const {
    return fabs(eta()) > 1.5660;
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

double Particle::deltaEta(const ParticlePointer other) const {
    return eta() - other->eta();
}

double Particle::deltaPhi(const ParticlePointer other) const {
    return fourvector.DeltaPhi(other->getFourVector());
}

double Particle::deltaR(const ParticlePointer other) const {
    return fourvector.DeltaR(other->getFourVector());
}

bool Particle::isWithinDeltaR(double delta_R, const ParticlePointer particle) const {
    return deltaR(particle) < delta_R;
}

double Particle::invariantMass(const ParticlePointer otherParticle) const {
    TLorentzVector combinedParticle(fourvector + otherParticle->getFourVector());
    return combinedParticle.M();
}

double Particle::relativePtTo(const ParticlePointer otherParticle) const {
    double relativePt = fourvector.Perp(otherParticle->getFourVector().Vect());
    return fabs(relativePt);
}

unsigned short Particle::getClosest(const ParticleCollection& particles) const {
    unsigned short idOfClosest = 999;
    double closestDR = 999.;
    for (unsigned short index = 0; index < particles.size(); ++index) {
        double DR = deltaR(particles.at(index));
        if (DR < closestDR) {
            closestDR = DR;
            idOfClosest = index;
        }
    }
    return idOfClosest;
}

double Particle::angle(const ParticlePointer otherParticle) const {
    return fourvector.Angle(otherParticle->getFourVector().Vect());
}
}
