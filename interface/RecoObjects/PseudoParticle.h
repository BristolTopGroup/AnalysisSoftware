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
    double Phi, Eta, Pt, Theta, D0, Charge;
public:
    PseudoParticle();
    PseudoParticle(double phi, double eta, double pt, double theta);
    virtual ~PseudoParticle();
    double pt() const;
    double eta() const;
    double phi() const;
    double theta() const;
    double charge() const;
    double d0() const;

    void setPt(double pt);
    void setEta(double eta);
    void setPhi(double phi);
    void setTheta(double theta);
    void setCharge(double charge);
    void setD0(double d0);

    double deltaR(double phi1, double eta1, double phi2, double eta2) const;
    double deltaR(const ParticlePointer otherParticle) const;
    double deltaR(const PseudoParticlePointer otherParticle) const;
    double deltaPhi(double phi1, double phi2) const;
    double deltaEta(double eta1, double eta2) const;
    bool isWithinDeltaR(double delta_R, const ParticlePointer particle) const;
    bool isWithinDeltaR(double delta_R, const PseudoParticlePointer particle) const;
};

}

#endif /* PSEUDOPARTICLE_H_ */
