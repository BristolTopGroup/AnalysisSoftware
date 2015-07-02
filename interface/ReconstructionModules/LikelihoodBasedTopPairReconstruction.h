/*
 * LikelihoodBasedTopPairReconstruction.h
 *
 *  Created on: 25 Aug 2011
 *      Author: kreczko
 */

#ifndef LIKELIHOODBASEDTOPPAIRRECONSTRUCTION_H_
#define LIKELIHOODBASEDTOPPAIRRECONSTRUCTION_H_

#include <boost/shared_ptr.hpp>
#include "../TtbarHypothesis.h"
#include "../../interface/GlobalVariables.h"
#include "../Rochester/NeutrinoSolver.h"
#include <iostream>

namespace BAT {

class LikelihoodBasedTopPairReconstruction {
public:
	LikelihoodBasedTopPairReconstruction(const LeptonPointer lepton, const METPointer met, const JetCollection jets, const JetCollection bjets);
	virtual ~LikelihoodBasedTopPairReconstruction();
	virtual const TtbarHypothesisPointer getBestSolution();
	virtual TtbarHypothesisCollection getAllSolutions();
	virtual bool meetsInitialCriteria() const;
	virtual std::string getDetailsOnFailure() const;
	virtual void reconstruct();

protected:
	virtual double getDiscriminator(const TtbarHypothesisPointer) const;
	virtual bool meetsHadronicBJetRequirement(JetPointer hadBJet);
	virtual bool meetsLeptonicBJetRequirement(JetPointer lepBJet);
	virtual bool meetsJetFromWRequirement(JetPointer jet1, JetPointer jet2);
	virtual bool meetsGlobalRequirement(const TtbarHypothesisPointer solution);
	virtual ParticlePointer getNeutrinoSolution( JetPointer BJet, LeptonPointer Lepton, METPointer met, double& neutrinoChi2 );
//	virtual void throwDetailedException() const;

protected:
	TtbarHypothesisCollection solutions;
	bool alreadyReconstructed;
	METPointer met;
	JetCollection jets;
	JetCollection bjets;
	LeptonPointer leptonFromW;
};

typedef boost::shared_ptr<LikelihoodBasedTopPairReconstruction> LikelihoodReconstructionPtr;

} /* namespace BAT */
#endif /* LIKELIHOODBASEDTOPPAIRRECONSTRUCTION_H_ */
