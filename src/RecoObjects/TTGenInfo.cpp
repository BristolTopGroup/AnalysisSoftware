/*
 * TTGenInfo.cpp
 *
 *  Created on: Feb 06, 2015
 *      Author: ejclemen
 */

#include "../../interface/RecoObjects/TTGenInfo.h"

namespace BAT {

TTGenInfo::TTGenInfo()
{

}
TTGenInfo::TTGenInfo(ParticlePointer leptonicTop, ParticlePointer hadronicTop, ParticlePointer leptonicW, ParticlePointer quark, ParticlePointer quarkBar, ParticlePointer leptonicB, ParticlePointer hadronicB, ParticlePointer singlelepton, ParticlePointer singleneutrino, int quarkGenJetIndex, int quarkBarGenJetIndex, int leptonicBGenJetIndex, int hadronicBGenJetIndex ) :

    leptonicTop_(leptonicTop),
    hadronicTop_(hadronicTop),

    leptonicW_(leptonicW),

    quark_(quark),
    quarkBar_(quarkBar),

    leptonicB_(leptonicB),
    hadronicB_(hadronicB),

    singlelepton_(singlelepton),
    singleneutrino_(singleneutrino),

    quarkGenJetIndex_(quarkGenJetIndex),
    quarkBarGenJetIndex_(quarkBarGenJetIndex),

    leptonicBGenJetIndex_(leptonicBGenJetIndex),
    hadronicBGenJetIndex_(hadronicBGenJetIndex){

}


TTGenInfo::~TTGenInfo() {
}

const ParticlePointer TTGenInfo::getLeptonicTop() {
    return leptonicTop_;
}

const ParticlePointer TTGenInfo::getHadronicTop() {
    return hadronicTop_;
}

const ParticlePointer TTGenInfo::getLeptonicW() {
    return leptonicW_;
}

const ParticlePointer TTGenInfo::getQuark() {
    return quark_;
}

const ParticlePointer TTGenInfo::getQuarkBar() {
    return quarkBar_;
}

const ParticlePointer TTGenInfo::getleptonicB() {
    return leptonicB_;
}

const ParticlePointer TTGenInfo::gethadronicB() {
    return hadronicB_;
}

const ParticlePointer TTGenInfo::getsinglelepton() {
    return singlelepton_;
}

const ParticlePointer TTGenInfo::getsingleneutrino() {
    return singleneutrino_;
}

const int TTGenInfo::getQuarkGenJetIndex() {
    return quarkGenJetIndex_;
}

const int TTGenInfo::getQuarkBarGenJetIndex() {
    return quarkBarGenJetIndex_;
}

const int TTGenInfo::getleptonicBGenJetIndex() {
    return leptonicBGenJetIndex_;
}

const int TTGenInfo::gethadronicBGenJetIndex() {
    return hadronicBGenJetIndex_;
}

}
