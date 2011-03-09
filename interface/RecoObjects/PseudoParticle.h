/*
 * PseudoParticle.h
 *
 *  Created on: 28 Sep 2010
 *      Author: kreczko
 */

#ifndef PSEUDOPARTICLE_H_
#define PSEUDOPARTICLE_H_
#include "Particle.h"

namespace BAT {
class PseudoParticle;

typedef boost::shared_ptr<PseudoParticle> PseudoParticlePointer;
typedef std::vector<PseudoParticle> PseudoParticleCollection;

class PseudoParticle {
protected:
    float Phi, Eta, Pt, Theta, D0, Charge;
public:
    PseudoParticle();
    PseudoParticle(float phi, float eta, float pt, float theta);
    virtual ~PseudoParticle();
    float pt() const;
    float eta() const;
    float phi() const;
    float theta() const;
    float charge() const;
    float d0() const;

    void setPt(float pt);
    void setEta(float eta);
    void setPhi(float phi);
    void setTheta(float theta);
    void setCharge(float charge);
    void setD0(float d0);

    float deltaR(float phi1, float eta1, float phi2, float eta2) const;
    float deltaR(const ParticlePointer otherParticle) const;
    float deltaR(const PseudoParticlePointer otherParticle) const;
    float deltaPhi(float phi1, float phi2) const;
    float deltaEta(float eta1, float eta2) const;
    bool isWithinDeltaR(float delta_R, const ParticlePointer particle) const;
    bool isWithinDeltaR(float delta_R, const PseudoParticlePointer particle) const;
};

}

#endif /* PSEUDOPARTICLE_H_ */
