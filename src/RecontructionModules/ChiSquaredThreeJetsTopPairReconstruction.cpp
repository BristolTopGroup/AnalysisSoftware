/*
 * ChiSquaredThreeJetsTopPairReconstruction.cpp
 *
 *  Created on: 2 Sep 2011
 *      Author: kreczko
 */

#include "../../interface/RecontructionModules/ChiSquaredThreeJetsTopPairReconstruction.h"

namespace BAT {

bool ChiSquaredThreeJetsTopPairReconstruction::meetsJetFromWRequirement(unsigned short jet1Index, unsigned short jet2Index) {
	return jet1Index == jet2Index;// two jets from W merged, therefore only 3 jets are needed
}

bool ChiSquaredThreeJetsTopPairReconstruction::meetsInitialCriteria() const {
	return met != 0 && electronFromW != 0 && jets.size() == 3;// needs exactly 3 jets
}
ChiSquaredThreeJetsTopPairReconstruction::ChiSquaredThreeJetsTopPairReconstruction(const ElectronPointer electron, const METPointer met, const JetCollection jets):
		ChiSquaredBasedTopPairReconstruction(electron, met, jets) {

}

ChiSquaredThreeJetsTopPairReconstruction::~ChiSquaredThreeJetsTopPairReconstruction() {
}

} /* namespace BAT */
