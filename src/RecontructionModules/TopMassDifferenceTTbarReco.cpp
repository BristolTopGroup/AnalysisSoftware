/*
 * TopMassDifferenceTTbarReco.cpp
 *
 *  Created on: 2 Sep 2011
 *      Author: kreczko
 */

#include "../../interface/RecontructionModules/TopMassDifferenceTTbarReco.h"

namespace BAT {

double TopMassDifferenceTTbarReco::getDiscriminator(const TtbarHypothesisPointer solution) const {
	double topMassDifference = solution->leptonicTop->mass() - solution->hadronicTop->mass();
	double absoluteDeltaTopMass = fabs(topMassDifference);
	return absoluteDeltaTopMass;
}

TopMassDifferenceTTbarReco::TopMassDifferenceTTbarReco(const ElectronPointer electron, const METPointer met, const JetCollection jets) :
				BasicTopPairReconstruction(electron, met, jets) {

}

TopMassDifferenceTTbarReco::~TopMassDifferenceTTbarReco() {
}

} /* namespace BAT */
