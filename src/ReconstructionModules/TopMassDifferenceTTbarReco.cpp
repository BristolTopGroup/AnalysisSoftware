/*
 * TopMassDifferenceTTbarReco.cpp
 *
 *  Created on: 2 Sep 2011
 *      Author: kreczko
 */

#include "../../interface/ReconstructionModules/TopMassDifferenceTTbarReco.h"

namespace BAT {

double TopMassDifferenceTTbarReco::getDiscriminator(const TtbarHypothesisPointer solution) const {
	double topMassDifference = solution->leptonicTop->mass() - solution->hadronicTop->mass();
	double absoluteDeltaTopMass = fabs(topMassDifference);
	return absoluteDeltaTopMass;
}

TopMassDifferenceTTbarReco::TopMassDifferenceTTbarReco(const LeptonPointer lepton, const METPointer met, const JetCollection jets) :
				BasicTopPairReconstruction(lepton, met, jets) {

}

TopMassDifferenceTTbarReco::~TopMassDifferenceTTbarReco() {
}

} /* namespace BAT */
