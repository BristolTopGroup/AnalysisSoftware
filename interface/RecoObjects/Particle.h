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
	Particle(double energy, double px, double py, double pz);
	virtual ~Particle();
	Particle operator+(const Particle &other) const;
	Particle & operator=(const Particle &rightHandSide);
	double mass() const;
	double d0() const;
	double d0_wrtBeamSpot() const;
	double energy() const;
	double et() const;
	double momentum() const;
	double px() const;
	double py() const;
	double pz() const;
	double pt() const;
	double eta() const;
	double phi() const;
	double theta() const;
	double massFromEnergyAndMomentum() const;
	double charge() const;
	void setMass(double mass);
	void setD0(double d0);
	void setD0_wrtBeamSpot(double d0);
	void setCharge(double charge);
	const FourVector& getFourVector() const;
	void setFourVector(FourVector fourvector);
	unsigned short getClosest(const ParticleCollection& particles) const;

	bool isInBarrelRegion() const;
	bool isInCrack() const;
	bool isInEndCapRegion() const;
	double relativePtTo(const ParticlePointer otherParticle) const;
	double deltaR(const ParticlePointer otherParticle) const;
	double deltaPhi(const ParticlePointer otherParticle) const;
	double deltaEta(const ParticlePointer otherParticle) const;
	bool isWithinDeltaR(double deltaR, const ParticlePointer particle) const;
	double invariantMass(const ParticlePointer otherParticle) const;
	double angle(const ParticlePointer otherParticle) const;
	virtual std::string toString() const;

protected:
	double particleMass, particleCharge;
	double distanceFromInteractionPointInCM;
	double distanceFromInteractionPointInCM_wrt_to_BeamSpot;
	FourVector fourvector;

};

}

#endif /* PARTICLE_H_ */
