/*
 * PseudoParticle.cpp
 *
 *  Created on: 28 Sep 2010
 *      Author: kreczko
 */

#include "../../interface/RecoObjects/PseudoParticle.h"

namespace BAT {

PseudoParticle::PseudoParticle() :
    Phi(99999), Eta(-9999), Pt(0), Theta(0), D0(9999), Charge(-5555) {

}

PseudoParticle::PseudoParticle(float phi, float eta, float pt, float theta) :
    Phi(phi), Eta(eta), Pt(pt), Theta(theta), D0(9999), Charge(-5555) {

}

PseudoParticle::~PseudoParticle() {
}

void PseudoParticle::setPhi(float phi) {
    Phi = phi;
}

void PseudoParticle::setEta(float eta) {
    Eta = eta;
}

void PseudoParticle::setPt(float pt) {
    Pt = pt;
}

void PseudoParticle::setTheta(float theta) {
    Theta = theta;
}

void PseudoParticle::setD0(float d0) {
    D0 = d0;
}

void PseudoParticle::setCharge(float charge) {
    Charge = charge;
}

float PseudoParticle::phi() const {
    return Phi;
}

float PseudoParticle::eta() const {
    return Eta;
}

float PseudoParticle::pt() const {
    return Pt;
}

float PseudoParticle::theta() const {
    return Theta;
}

float PseudoParticle::d0() const {
    return D0;
}

float PseudoParticle::charge() const {
    return Charge;
}

float PseudoParticle::deltaR(const PseudoParticlePointer otherParticle) const {
    return deltaR(Phi, Eta, otherParticle->phi(), otherParticle->eta());
}

float PseudoParticle::deltaR(const ParticlePointer otherParticle) const {
    return deltaR(Phi, Eta, otherParticle->phi(), otherParticle->eta());
}

float PseudoParticle::deltaR(float phi1, float eta1, float phi2, float eta2) const {
    return sqrt(pow(deltaPhi(phi1, phi2), 2) + pow(deltaEta(eta1, eta2), 2));
}

float PseudoParticle::deltaPhi(float phi1, float phi2) const {
    float delPhi(999.0);
    delPhi = fabs(phi1 - phi2);
    if (delPhi > TMath::Pi())
        delPhi = 2.0 * TMath::Pi() - delPhi;
    return delPhi;
}

float PseudoParticle::deltaEta(float eta1, float eta2) const{
    return eta1 - eta2;
}

bool PseudoParticle::isWithinDeltaR(float delta_R, const PseudoParticlePointer particle) const{
    return deltaR(particle) < delta_R;
}

}
