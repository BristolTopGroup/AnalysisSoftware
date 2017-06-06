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

	void setPseudoTops( MCParticleCollection newPseudoTops );
	MCParticleCollection getPseudoTops() const;

	void setPseudoLeptonicW( MCParticlePointer newLeptonicW );
	MCParticlePointer getPseudoLeptonicW() const;

	void setPseudoLepton( MCParticlePointer newPseudoLepton );
	MCParticlePointer getPseudoLepton() const;

	void setPseudoBs( MCParticleCollection newPseudoBs );
	MCParticleCollection getPseudoBs() const;

	void setPseudoJets( JetCollection newPseudoJets );
	JetCollection getPseudoJets() const;

	void setAllPseudoLeptons( MCParticleCollection newPseudoLeptons );
	MCParticleCollection getAllPseudoLeptons() const;

	void setPseudoNeutrino( ParticlePointer newPseudoNeutrino );
	ParticlePointer getPseudoNeutrino() const;

	void setPseudoMET( ParticlePointer newPseudoMET );
	ParticlePointer getPseudoMET() const;

	void setIsSemiLeptonic( bool isSemiLeptonic_ );
	bool isSemiLeptonic() const;
protected:

	MCParticleCollection pseudoTops_;
	MCParticlePointer pseudoLeptonicW_;
	MCParticlePointer pseudoLepton_;
	MCParticleCollection allPseudoLeptons_;
	JetCollection pseudoJets_;
	MCParticleCollection pseudoBs_;
	ParticlePointer pseudoNeutrino_;
	ParticlePointer pseudoMET_;

	bool isSemiLeptonic_;
};

}

#endif /* PSEUDOTOPPARTICLES_H_ */
