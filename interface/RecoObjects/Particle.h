/*
 * Particle.h
 *
 *  Created on: 2 Jul 2010
 *      Author: kreczko
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_
#include "TLorentzVector.h"
#include <vector>
#include <string>
#include <math.h>
#include <boost/shared_ptr.hpp>

typedef TLorentzVector FourVector;

namespace BAT {
class Particle;
typedef boost::shared_ptr<Particle> ParticlePointer;
typedef std::vector<ParticlePointer> ParticleCollection;


class Particle {
public:
	Particle();
	Particle(const Particle& particle);
	Particle(float energy, float px, float py, float pz);
	virtual ~Particle();
	const Particle operator+(const Particle &other) const;
	Particle & operator=(const Particle &rightHandSide);
	float mass() const;
	float d0() const;
	float d0_wrtBeamSpot() const;
	float energy() const;
	float et() const;
	float px() const;
	float py() const;
	float pz() const;
	float pt() const;
	float eta() const;
	float phi() const;
	float theta() const;
	float massFromEnergyAndMomentum() const;
	float charge() const;
	void setMass(float mass);
	void setD0(float d0);
	void setD0_wrtBeamSpot(float d0);
	void setCharge(float charge);
	const FourVector& getFourVector() const;
	void setFourVector(FourVector fourvector);
	unsigned short getClosest(const ParticleCollection& particles) const;

	bool isInBarrelRegion() const;
	bool isInCrack() const;
	bool isInEndCapRegion() const;
	const char* getEtaRegion() const;
	float relativePtTo(const ParticlePointer otherParticle) const;
	float deltaR(const ParticlePointer otherParticle) const;
	float deltaPhi(const ParticlePointer otherParticle) const;
	float deltaEta(const ParticlePointer otherParticle) const;
	bool isWithinDeltaR(float deltaR, const ParticlePointer particle) const;
	float invariantMass(const ParticlePointer otherParticle) const;
	float angle(const ParticlePointer otherParticle) const;

protected:
	float particleMass, particleCharge;
	float distanceFromInteractionPointInMicron;
	float distanceFromInteractionPointInMicron_wrt_to_BeamSpot;
	FourVector fourvector;

};

}

#endif /* PARTICLE_H_ */
