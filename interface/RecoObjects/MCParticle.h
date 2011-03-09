/*
 * MCParticle.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef MCPARTICLE_H_
#define MCPARTICLE_H_
#include "Particle.h"

namespace BAT {
class MCParticle;
typedef boost::shared_ptr<MCParticle> MCParticlePointer;
typedef std::vector<MCParticlePointer> MCParticleCollection;

class MCParticle: public Particle {
public:
	int pdgId() const;
	int motherIndex() const;
	int status() const;
	MCParticle();
	MCParticle(float energy, float px, float py, float pz);
	//MCParticle(int pdgId, float phi, float eta, float pt, float energy, int motherIndex, int status);
	virtual ~MCParticle();
	void setPdgId(int PdgId);
	void setMotherIndex(int MotherIndex);
	void setStatus(int Status);
protected:
	int particlePdgId;
	int particleMotherIndex;
	int particleStatus;
};

}

#endif /* MCPARTICLE_H_ */
