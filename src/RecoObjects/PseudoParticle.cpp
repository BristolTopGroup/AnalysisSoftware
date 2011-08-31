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

PseudoParticle::PseudoParticle(double phi, double eta, double pt, double theta) :
    Phi(phi), Eta(eta), Pt(pt), Theta(theta), D0(9999), Charge(-5555) {

}

PseudoParticle::~PseudoParticle() {
}

void PseudoParticle::setPhi(double phi) {
    Phi = phi;
}

void PseudoParticle::setEta(double eta) {
    Eta = eta;
}

void PseudoParticle::setPt(double pt) {
    Pt = pt;
}

void PseudoParticle::setTheta(double theta) {
    Theta = theta;
}

void PseudoParticle::setD0(double d0) {
    D0 = d0;
}

void PseudoParticle::setCharge(double charge) {
    Charge = charge;
}

double PseudoParticle::phi() const {
    return Phi;
}

double PseudoParticle::eta() const {
    return Eta;
}

double PseudoParticle::pt() const {
    return Pt;
}

double PseudoParticle::theta() const {
    return Theta;
}

double PseudoParticle::d0() const {
    return D0;
}

double PseudoParticle::charge() const {
    return Charge;
}

double PseudoParticle::deltaR(const PseudoParticlePointer otherParticle) const {
    return deltaR(Phi, Eta, otherParticle->phi(), otherParticle->eta());
}

double PseudoParticle::deltaR(const ParticlePointer otherParticle) const {
    return deltaR(Phi, Eta, otherParticle->phi(), otherParticle->eta());
}

double PseudoParticle::deltaR(double phi1, double eta1, double phi2, double eta2) const {
    return sqrt(pow(deltaPhi(phi1, phi2), 2) + pow(deltaEta(eta1, eta2), 2));
}

double PseudoParticle::deltaPhi(double phi1, double phi2) const {
    double delPhi(999.0);
    delPhi = fabs(phi1 - phi2);
    if (delPhi > TMath::Pi())
        delPhi = 2.0 * TMath::Pi() - delPhi;
    return delPhi;
}

double PseudoParticle::deltaEta(double eta1, double eta2) const{
    return eta1 - eta2;
}

bool PseudoParticle::isWithinDeltaR(double delta_R, const PseudoParticlePointer particle) const{
    return deltaR(particle) < delta_R;
}

}
