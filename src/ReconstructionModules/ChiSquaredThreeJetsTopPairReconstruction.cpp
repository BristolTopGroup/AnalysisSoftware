/*
 * ChiSquaredThreeJetsTopPairReconstruction.cpp
 *
 *  Created on: 2 Sep 2011
 *      Author: kreczko
 */

#include "../../interface/ReconstructionModules/ChiSquaredThreeJetsTopPairReconstruction.h"
#include <boost/lexical_cast.hpp>
#include "../../interface/ReconstructionModules/ReconstructionException.h"

namespace BAT {

bool ChiSquaredThreeJetsTopPairReconstruction::meetsJetFromWRequirement(unsigned short jet1Index,
		unsigned short jet2Index) {
	return jet1Index == jet2Index; // two jets from W merged, therefore only 3 jets are needed
}

bool ChiSquaredThreeJetsTopPairReconstruction::meetsGlobalRequirement(const TtbarHypothesisPointer solution) {
	return solution->isPhysical() && solution->hadronicW->mass() > 40;
}

bool ChiSquaredThreeJetsTopPairReconstruction::meetsInitialCriteria() const {
	return met != 0 && leptonFromW != 0 && jets.size() == 3; // needs exactly 3 jets
}

std::string ChiSquaredThreeJetsTopPairReconstruction::getDetailsOnFailure() const {
	std::string msg = "Initial Criteria not met: \n";
	if (leptonFromW == 0)
		msg += "Electron from W: not filled \n";
	else
		msg += "Electron from W: filled \n";

	if (met == 0)
		msg += "Missing transverse energy: not filled \n";
	else
		msg += "Missing transverse energy: filled \n";
	std::string nJets(boost::lexical_cast<std::string>(jets.size()));
	if (jets.size() != 3) {
		if (jets.size() > 3)
			msg += "Number of jets is too large:" + nJets + ", should be == 3 \n";
		else
			msg += "Number of jets is too small:" + nJets + ", should be == 3 \n";
	}

	else
		msg += "Number of jets is OK:" + nJets + "\n";

	return msg;
}

ChiSquaredThreeJetsTopPairReconstruction::ChiSquaredThreeJetsTopPairReconstruction(const LeptonPointer lepton,
		const METPointer met, const JetCollection jets) :
		BasicTopPairReconstruction(lepton, met, jets), ChiSquaredBasedTopPairReconstruction(lepton, met, jets) {

}

ChiSquaredThreeJetsTopPairReconstruction::~ChiSquaredThreeJetsTopPairReconstruction() {
}

} /* namespace BAT */
