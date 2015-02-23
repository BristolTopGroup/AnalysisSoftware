/*
 * PseudoTopParticles.h
 *
 *  Created on: Feb 23, 2015
 *      Author: ejclemen
 */

#ifndef PSEUDOTOPPARTICLES_H_
#define PSEUDOTOPPARTICLES_H_
#include "Particle.h"

namespace BAT {
class PseudoTopParticles;
typedef boost::shared_ptr<PseudoTopParticles> PseudoTopParticlesPointer;


class PseudoTopParticles: public Particle {
public:
	PseudoTopParticles();
	// PseudoTopParticles(double energy, double px, double py, double pz);

	virtual ~PseudoTopParticles();

	void setPseudoTops( ParticleCollection newPseudoTops );
	ParticleCollection getPseudoTops() const;

protected:

	ParticleCollection pseudoTops_;
};

}

#endif /* PSEUDOTOPPARTICLES_H_ */
