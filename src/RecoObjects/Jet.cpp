/*
 * Jet.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/Jet.h"

namespace BAT {

Jet::Jet() :
    Particle(),
    usedAlgorithm(JetAlgorithm::Calo_AntiKT_Cone05),
    electromagneticFraction(0.),
    numberOfRecHitsContaining90PercentOfTheJetEnergy(0.),
    fractionOfEnergyIntheHottestHPDReadout(0.),
    btag_discriminators(BtagAlgorithm::NUMBER_OF_BTAGALGORITHMS),
    numberOfDaughters(0),
    chargedEmEnergyFraction(1),
    neutralHadronEnergyFraction(1),
    neutralEmEnergyFraction(1),
    chargedHadronEnergyFraction(1),
    chargedMultiplicity(0),
    partonFlavour(0) {
    for (unsigned int btag = 0; btag < btag_discriminators.size(); ++btag) {
        btag_discriminators[btag] = -9999;
    }
}

//Jet::Jet(const Particle& particle) :
//    Particle(particle),
//    usedAlgorithm(JetAlgorithm::Calo_AntiKT_Cone05),
//    electromagneticFraction(0.),
//    numberOfRecHitsContaining90PercentOfTheJetEnergy(0.),
//    fractionOfEnergyIntheHottestHPDReadout(0.),
//    btag_discriminators(BJetTagger::NUMBER_OF_BTAGALGORITHMS),
//    numberOfDaughters(0),
//    chargedEmEnergyFraction(1),
//    neutralHadronEnergyFraction(1),
//    neutralEmEnergyFraction(1),
//    chargedHadronEnergyFraction(1),
//    chargedMultiplicity(0) {
//
//}
Jet::Jet(float energy, float px, float py, float pz) :
    Particle(energy, px, py, pz),
    usedAlgorithm(JetAlgorithm::Calo_AntiKT_Cone05),
    electromagneticFraction(0.),
    btag_discriminators(BtagAlgorithm::NUMBER_OF_BTAGALGORITHMS),
    numberOfDaughters(0),
    chargedEmEnergyFraction(1),
    neutralHadronEnergyFraction(1),
    neutralEmEnergyFraction(1),
    chargedHadronEnergyFraction(1),
    chargedMultiplicity(0),
    partonFlavour(0) {
    for (unsigned int btag = 0; btag < btag_discriminators.size(); ++btag) {
        btag_discriminators[btag] = -9999;
    }

}

Jet::~Jet() {
}

JetAlgorithm::value Jet::getUsedAlgorithm() const {
    return usedAlgorithm;
}

float Jet::emf() const {
    return electromagneticFraction;
}

float Jet::n90Hits() const {
    return numberOfRecHitsContaining90PercentOfTheJetEnergy;
}

float Jet::fHPD() const {
    return fractionOfEnergyIntheHottestHPDReadout;
}

float Jet::NOD() const {
    return numberOfDaughters;
}

float Jet::CEF() const {
    return chargedEmEnergyFraction;
}

float Jet::NHF() const {
    return neutralHadronEnergyFraction;
}

float Jet::NEF() const {
    return neutralEmEnergyFraction;
}

float Jet::CHF() const {
    return chargedHadronEnergyFraction;
}

float Jet::NCH() const {
    return chargedMultiplicity;
}

void Jet::setUsedAlgorithm(JetAlgorithm::value algo) {
    usedAlgorithm = algo;
}
void Jet::setEMF(float emf) {
    electromagneticFraction = emf;
}

void Jet::setN90Hits(int n90Hits) {
    numberOfRecHitsContaining90PercentOfTheJetEnergy = n90Hits;
}

void Jet::setFHPD(float fHPD) {
    fractionOfEnergyIntheHottestHPDReadout = fHPD;
}

void Jet::setDiscriminatorForBtagType(float discriminator, BtagAlgorithm::value type) {
    btag_discriminators[type] = discriminator;
}

void Jet::setNOD(int nod) {
    numberOfDaughters = nod;
}
void Jet::setCEF(float cef) {
    chargedEmEnergyFraction = cef;
}
void Jet::setNHF(float nhf) {
    neutralHadronEnergyFraction = nhf;
}
void Jet::setNEF(float nef) {
    neutralEmEnergyFraction = nef;
}
void Jet::setCHF(float chf) {
    chargedHadronEnergyFraction = chf;
}
void Jet::setNCH(float nch) {
    chargedMultiplicity = nch;
}

bool Jet::isGood() const {
    bool passesPt = pt() > 30;
    bool passesEta = fabs(eta()) < 2.4;
    bool jetID = false;
    if (usedAlgorithm == JetAlgorithm::ParticleFlow || usedAlgorithm == JetAlgorithm::PF2PAT) {
        bool passNOD = NOD() > 1;
        bool passCEF = CEF() < 0.99;
        bool passNHF = NHF() < 0.99;
        bool passNEF = NEF() < 0.99;
        bool passCHF = true;
        bool passNCH = true;
        if (fabs(eta()) < 2.4) {
            passCHF = CHF() > 0;
            passNCH = NCH() > 0;
        }
        jetID = passNOD && passCEF && passNHF && passNEF && passCHF && passNCH;
    }
    else{
        bool passesEMF = emf() > 0.01;
        bool passesN90Hits = n90Hits() > 1;
        bool passesFHPD = fHPD() < 0.98;
        jetID = passesEMF && passesN90Hits && passesFHPD;
    }

    return passesPt && passesEta && jetID;
}

/* Values taken from
 * https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagPerformance
 */
bool Jet::isBJet(BtagAlgorithm::value type, BtagAlgorithm::workingPoint wp) const {
    float cut(-9998);
    switch (type) {
    case BtagAlgorithm::GenPartonFlavour:
        return abs(partonFlavour) == 5;

    case BtagAlgorithm::SimpleSecondaryVertexHighEffBTag:
        if (wp == BtagAlgorithm::LOOSE)
            cut = -9998;//no input found
        else if (wp == BtagAlgorithm::MEDIUM)
            cut = 1.74;
        else if (wp == BtagAlgorithm::TIGHT)
            cut = 3.05;
        break;

    case BtagAlgorithm::SimpleSecondaryVertexHighPurBTag:
        if (wp == BtagAlgorithm::LOOSE)
            cut = -9998;//no input found
        else if (wp == BtagAlgorithm::MEDIUM)
            cut = -9998;//no input found
        else if (wp == BtagAlgorithm::TIGHT)
            cut = 2.;
        break;

    case BtagAlgorithm::TrackCountingHighEffBTag:
        if (wp == BtagAlgorithm::LOOSE)
            cut = 1.7;
        else if (wp == BtagAlgorithm::MEDIUM)
            cut = 3.3;
        else if (wp == BtagAlgorithm::TIGHT)
            cut = 10.2;
        break;

    case BtagAlgorithm::TrackCountingHighPurBTag:
        if (wp == BtagAlgorithm::LOOSE)
            cut = 1.19;
        else if (wp == BtagAlgorithm::MEDIUM)
            cut = 1.93;
        else if (wp == BtagAlgorithm::TIGHT)
            cut = 3.41;
        break;

    case BtagAlgorithm::JetProbabilityBTag:
        if (wp == BtagAlgorithm::LOOSE)
            cut = 0.215;
        else if (wp == BtagAlgorithm::MEDIUM)
            cut = 0.459;
        else if (wp == BtagAlgorithm::TIGHT)
            cut = 0.669;
        break;

    case BtagAlgorithm::JetBProbabilityBTag:
        if (wp == BtagAlgorithm::LOOSE)
            cut = 0.988;
        else if (wp == BtagAlgorithm::MEDIUM)
            cut = 1.83;
        else if (wp == BtagAlgorithm::TIGHT)
            cut = 1.95;
        break;
    default:
        return false;

    }
    return btag_discriminators[type] > cut;
}
}
;

