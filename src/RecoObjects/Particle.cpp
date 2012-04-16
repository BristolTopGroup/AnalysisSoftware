/*
 * Particle.cpp
 *
 *  Created on: 2 Jul 2010
 *      Author: kreczko
 */

#include "../../interface/RecoObjects/Particle.h"
#include <sstream>
#include <iomanip>

using namespace std;

namespace BAT {

Particle::Particle() :
		particleMass(0), //
		particleCharge(0), //
		distanceFromInteractionPointInCM(999999), //
		distanceFromInteractionPointInCM_wrt_to_BeamSpot(999999), //
		fourvector(0., 0., 0., 0.) {

}

Particle::Particle(const Particle& particle) :
		particleMass(particle.mass()), //
		particleCharge(particle.charge()), //
		distanceFromInteractionPointInCM(particle.d0()), //
		distanceFromInteractionPointInCM_wrt_to_BeamSpot(particle.d0_wrtBeamSpot()), //
		fourvector(particle.getFourVector()) {

}

Particle::Particle(double energy, double px, double py, double pz) :
		particleMass(0), //
		particleCharge(0), //
		distanceFromInteractionPointInCM(99999), //
		distanceFromInteractionPointInCM_wrt_to_BeamSpot(999999), //
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
	return distanceFromInteractionPointInCM;
}

double Particle::d0_wrtBeamSpot() const {
	return distanceFromInteractionPointInCM_wrt_to_BeamSpot;
}

double Particle::energy() const {
	return fourvector.Energy();
}

double Particle::et() const {
	return fourvector.Et();
}

double Particle::momentum() const {
	return fourvector.P();
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
	distanceFromInteractionPointInCM = d0;
}

void Particle::setD0_wrtBeamSpot(double d0) {
	distanceFromInteractionPointInCM_wrt_to_BeamSpot = d0;
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

bool Particle::isInBarrelRegion() const {
	return fabs(eta()) < 1.4442;
}

bool Particle::isInCrack() const {
	return !isInBarrelRegion() && !isInEndCapRegion();
}

bool Particle::isInEndCapRegion() const {
	return fabs(eta()) > 1.5660;
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

string Particle::toString() const {
	stringstream out;
	out << "Particle information" << "\n";
	out << setw(30) << "energy" << setw(30) << "px" << setw(30) << "py" << setw(30) << "pz" << "\n";
	out << setw(30) << energy() << setw(30) << px() << setw(30) << py() << setw(30) << pz() << "\n";
	out << setw(30) << "phi" << setw(30) << "eta" << setw(30) << "theta" << setw(30) << "  " << "\n";
	out << setw(30) << phi() << setw(30) << eta()  << setw(30) << theta() << setw(30) << " " << "\n";
	out << setw(30) << "momentum" << setw(30) << "E_T" << setw(30) << "p_T" << setw(30) << "  " << "\n";
	out << setw(30) << momentum() << setw(30) << et() << setw(30) << pt() << setw(30) << "  " << "\n";
	out << setw(30) << "m_dyn" << setw(30) << "m_fix" << setw(30) << "charge" << setw(30) << "  " << "\n";
	out << setw(30) << massFromEnergyAndMomentum() << setw(30) <<  mass() << setw(30) << charge() << setw(30) << "  " << "\n";
	out << setw(30) << "d0 =" << setw(30) << "d0_bs" << setw(30) << "  " << setw(30) << "  " << "\n";
	out << setw(30) << d0() << setw(30) << d0_wrtBeamSpot() << setw(30) << "  " << setw(30) << "  " << "\n";
	return out.str();
}
}
