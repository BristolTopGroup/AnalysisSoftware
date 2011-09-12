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

MET::MET(double ex, double ey) :
    Particle(sqrt(ex * ex + ey * ey), ex, ey, 0),
    usedAlgorithm(METAlgorithm::Calo),
    significance_(0),
    sumET_(0){

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

void MET::setSignificance(double significance){
	significance_ = significance;
}

void MET::setSumET(double sumET){
	sumET_ = sumET;
}

double MET::significance() const{
	return significance_;
}

double MET::sumET() const{
	return sumET_;
}

double MET::resolution() const {
	return 0.5 * sqrt(sumET_);
}

}
