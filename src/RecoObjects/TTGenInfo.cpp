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

TTGenInfo::TTGenInfo(ParticlePointer quark, ParticlePointer quarkBar, int quarkGenJetIndex, int quarkBarGenJetIndex) :
    quark_(quark),
    quarkBar_(quarkBar),
    quarkGenJetIndex_(quarkGenJetIndex),
    quarkBarGenJetIndex_(quarkBarGenJetIndex)
{

}


TTGenInfo::~TTGenInfo() {
}

const ParticlePointer TTGenInfo::getQuark() {
    return quark_;
}

const ParticlePointer TTGenInfo::getQuarkBar() {
    return quarkBar_;
}

const int TTGenInfo::getQuarkGenJetIndex() {
    return quarkGenJetIndex_;
}

const int TTGenInfo::getQuarkBarGenJetIndex() {
    return quarkBarGenJetIndex_;
}

}
