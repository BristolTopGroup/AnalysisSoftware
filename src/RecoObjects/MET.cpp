/*
 * MET.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/MET.h"

namespace BAT {


MET::MET() :
    Particle(),
    usedAlgorithm(METAlgorithm::Calo) {
}

MET::MET(float ex, float ey) :
    Particle(sqrt(ex * ex + ey * ey), ex, ey, 0), usedAlgorithm(METAlgorithm::Calo) {

}

MET::~MET() {
}

bool MET::isGood() const {
    return et() > 20;
}

void MET::setUsedAlgorithm(METAlgorithm::value algo) {
    usedAlgorithm = algo;
}

METAlgorithm::value MET::getUsedAlgorithm() const {
    return usedAlgorithm;
}

}
