/*
 * BasicTopPairReconstruction.cpp
 *
 *  Created on: 25 Aug 2011
 *      Author: kreczko
 */

#include "../../interface/RecontructionModules/BasicTopPairReconstruction.h"

namespace BAT {

BasicTopPairReconstruction::BasicTopPairReconstruction(const ElectronPointer electron, const METPointer met, const JetCollection jets):
		solutions(),
		alreadyReconstructed(false),
		solutionComparator(),
		met(met),
		jets(jets),
		electronFromW(electron),
		neutrinoReconstruction(electron, met){

}

BasicTopPairReconstruction::~BasicTopPairReconstruction() {
}

std::vector<TtbarHypothesisPointer> BasicTopPairReconstruction::getAllSolutions(){
	if(!alreadyReconstructed)
		reconstruct();

	std::sort(solutions.begin(), solutions.end(), solutionComparator);
	return solutions;
}

const TtbarHypothesisPointer BasicTopPairReconstruction::getBestSolution() {
	return getAllSolutions().front();
}

void BasicTopPairReconstruction::reconstruct() {
	ParticlePointer neutrino = neutrinoReconstruction.getNeutrinos(NeutrinoSelection::mostCentral)[0];

	typedef unsigned short ushort;

	for (ushort hadBindex = 0; hadBindex < jets.size(); ++hadBindex) {
		if (!meetsHadronicBJetRequirement(hadBindex))
			continue;

		for (ushort lepBindex = 0; lepBindex < jets.size(); ++lepBindex) {
			if (lepBindex == hadBindex || !meetsLeptonicBJetRequirement(lepBindex))
				continue;

			for (ushort jet1Index = 0; jet1Index < jets.size(); ++jet1Index) {
				if (jet1Index == hadBindex || jet1Index == lepBindex)
					continue;

				for (ushort jet2Index = 0; jet2Index < jets.size();
						++jet2Index) {
					if (jet2Index == hadBindex || jet2Index == lepBindex
							|| !meetsJetFromWRequirement(jet1Index, jet2Index))
						continue;
					TtbarHypothesisPointer solution(new TtbarHypothesis());
					//leptons
					solution->electronFromW = electronFromW;
					solution->neutrinoFromW = neutrino;
					//jets
					solution->hadronicBJet = jets.at(hadBindex);
					solution->leptonicBjet = jets.at(lepBindex);
					solution->jet1FromW = jets.at(jet1Index);
					solution->jet2FromW = jets.at(jet2Index);

					//combine reconstructed objects
					solution->combineReconstructedObjects();

					solution->disc = getDiscriminator(solution);

					if(meetsGlobalRequirement(solution))
						solutions.push_back(solution);
					else
						continue;
				}
			}
		}
	}
	alreadyReconstructed = true;
}

bool BasicTopPairReconstruction::meetsHadronicBJetRequirement(unsigned short jetIndex){
	return true;
}

bool BasicTopPairReconstruction::meetsLeptonicBJetRequirement(unsigned short jetIndex){
	return true;
}

bool BasicTopPairReconstruction::meetsJetFromWRequirement(unsigned short jet1Index, unsigned short jet2Index){
	return jet1Index != jet2Index;// == require 4 jets
}

bool BasicTopPairReconstruction::meetsGlobalRequirement(const TtbarHypothesisPointer solution){
	return true;
}

double BasicTopPairReconstruction::getDiscriminator(const TtbarHypothesisPointer solution) const{
	return 0;
}

} /* namespace BAT */
