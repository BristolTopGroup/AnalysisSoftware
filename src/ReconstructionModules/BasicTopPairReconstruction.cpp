/*
 * BasicTopPairReconstruction.cpp
 *
 *  Created on: 25 Aug 2011
 *      Author: kreczko
 */

#include "../../interface/ReconstructionModules/BasicTopPairReconstruction.h"
#include "../../interface/ReconstructionModules/BasicNeutrinoReconstruction.h"
#include <boost/lexical_cast.hpp>

namespace BAT {

BasicTopPairReconstruction::BasicTopPairReconstruction(const LeptonPointer lepton, const METPointer met, const JetCollection jets):
		solutions(),
		alreadyReconstructed(false),
		met(met),
		jets(jets),
		leptonFromW(lepton) {
}

bool BasicTopPairReconstruction::meetsInitialCriteria() const {
	return met != 0 && leptonFromW != 0 && jets.size() >= 4;
}

std::string BasicTopPairReconstruction::getDetailsOnFailure() const {
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
	if (jets.size() < 4)
		msg += "Number of jets is too small:" + nJets + ", should be >= 4 \n";
	else
		msg += "Number of jets is OK:" + nJets + "\n";

	return msg;
}



BasicTopPairReconstruction::~BasicTopPairReconstruction() {
}

TtbarHypothesisCollection BasicTopPairReconstruction::getAllSolutions(){
	if(!alreadyReconstructed)
		reconstruct();

	SmallerDiscriminatorIsBetter solutionComparator = SmallerDiscriminatorIsBetter();
	std::sort(solutions.begin(), solutions.end(), solutionComparator);
	return solutions;
}

const TtbarHypothesisPointer BasicTopPairReconstruction::getBestSolution() {
	const TtbarHypothesisPointer bestSolution = getAllSolutions().front();//sorted by quality, front == best
	return bestSolution;
}

void BasicTopPairReconstruction::reconstruct() {
	boost::array<ParticlePointer, 2> neutrinos = getNeutrinos();
	typedef unsigned short ushort;

	for (ushort neutrinoIndex = 0; neutrinoIndex < neutrinos.size(); ++neutrinoIndex) {
		ParticlePointer neutrino = neutrinos.at(neutrinoIndex);
		for (ushort hadBindex = 0; hadBindex < jets.size(); ++hadBindex) {
			if (!meetsHadronicBJetRequirement(hadBindex))
				continue;

			for (ushort lepBindex = 0; lepBindex < jets.size(); ++lepBindex) {
				if (lepBindex == hadBindex || !meetsLeptonicBJetRequirement(lepBindex))
					continue;

				for (ushort jet1Index = 0; jet1Index < jets.size(); ++jet1Index) {
					if (jet1Index == hadBindex || jet1Index == lepBindex)
						continue;

					for (ushort jet2Index = 0; jet2Index < jets.size(); ++jet2Index) {
						if (jet2Index == hadBindex || jet2Index == lepBindex
								|| !meetsJetFromWRequirement(jet1Index, jet2Index))
							continue;
						TtbarHypothesisPointer solution(new TtbarHypothesis());
						//leptons
						solution->leptonFromW = leptonFromW;
						solution->neutrinoFromW = neutrino;
						//jets
						solution->hadronicBJet = jets.at(hadBindex);
						solution->leptonicBjet = jets.at(lepBindex);
						solution->jet1FromW = jets.at(jet1Index);
						solution->jet2FromW = jets.at(jet2Index);

						//combine reconstructed objects
						solution->combineReconstructedObjects();

						solution->discriminator = getDiscriminator(solution);

						if (meetsGlobalRequirement(solution)){
							solutions.push_back(solution);
						}

						else
							continue;
					}
				}
			}
		}
	}
	alreadyReconstructed = true;
}


boost::array<ParticlePointer, 2> BasicTopPairReconstruction::getNeutrinos() {
	BasicNeutrinoReconstruction neutrinoReconstruction(leptonFromW, met);
	return neutrinoReconstruction.getNeutrinos(NeutrinoSelection::None);
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
	return solution->isPhysical();
}

double BasicTopPairReconstruction::getDiscriminator(const TtbarHypothesisPointer solution) const{
	return 0;
}

} /* namespace BAT */
