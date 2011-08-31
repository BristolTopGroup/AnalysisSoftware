/*
 * MCParticle.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef MCPARTICLE_H_
#define MCPARTICLE_H_
#include "Particle.h"
#include "Jet.h"

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
	MCParticle(double energy, double px, double py, double pz);
	virtual ~MCParticle();
	void setPdgId(int PdgId);
	void setMotherIndex(int MotherIndex);
	void setStatus(int Status);
	bool isQuark() const;
	bool isLepton() const;
	bool isNeutrino() const;
	unsigned short getClosestJetIndex(const JetCollection& jets) const;
protected:
	int particlePdgId;
	int particleMotherIndex;
	int particleStatus;
};

}

#endif /* MCPARTICLE_H_ */
