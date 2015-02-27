/*
 * PseudoTopParticles.h
 *
 *  Created on: Feb 23, 2015
 *      Author: ejclemen
 */

#ifndef PSEUDOTOPPARTICLES_H_
#define PSEUDOTOPPARTICLES_H_
#include "Particle.h"
#include "MCParticle.h"
#include "Jet.h"

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

	void setPseudoLeptonicW( ParticlePointer newLeptonicW );
	ParticlePointer getPseudoLeptonicW() const;

	void setPseudoLepton( MCParticlePointer newPseudoLepton );
	MCParticlePointer getPseudoLepton() const;

	void setPseudoBs( ParticleCollection newPseudoBs );
	ParticleCollection getPseudoBs() const;

	void setPseudoJets( JetCollection newPseudoJets );
	JetCollection getPseudoJets() const;

	void setPseudoNeutrino( ParticlePointer newPseudoNeutrino );
	ParticlePointer getPseudoNeutrino() const;

	void setPseudoMET( ParticlePointer newPseudoMET );
	ParticlePointer getPseudoMET() const;

	void setIsSemiLeptonic( bool isSemiLeptonic_ );
	bool isSemiLeptonic() const;
protected:

	ParticleCollection pseudoTops_;
	ParticlePointer pseudoLeptonicW_;
	MCParticlePointer pseudoLepton_;
	JetCollection pseudoJets_;
	ParticleCollection pseudoBs_;
	ParticlePointer pseudoNeutrino_;
	ParticlePointer pseudoMET_;

	bool isSemiLeptonic_;
};

}

#endif /* PSEUDOTOPPARTICLES_H_ */
