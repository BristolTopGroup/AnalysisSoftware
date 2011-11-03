/*
 * BasicTopPairReconstruction.h
 *
 *  Created on: 25 Aug 2011
 *      Author: kreczko
 */

#ifndef BASICTOPPAIRRECONSTRUCTION_H_
#define BASICTOPPAIRRECONSTRUCTION_H_

#include <boost/shared_ptr.hpp>
#include "../TtbarHypothesis.h"

namespace BAT {

class BasicTopPairReconstruction {
public:
	BasicTopPairReconstruction(const LeptonPointer lepton, const METPointer met, const JetCollection jets);
	virtual ~BasicTopPairReconstruction();
	virtual const TtbarHypothesisPointer getBestSolution();
	virtual TtbarHypothesisCollection getAllSolutions();
	virtual bool meetsInitialCriteria() const;
	virtual std::string getDetailsOnFailure() const;

protected:
	virtual double getDiscriminator(const TtbarHypothesisPointer) const;
	virtual void reconstruct();
	virtual bool meetsHadronicBJetRequirement(unsigned short jetIndex);
	virtual bool meetsLeptonicBJetRequirement(unsigned short jetIndex);
	virtual bool meetsJetFromWRequirement(unsigned short jet1Index, unsigned short jet2Index);
	virtual bool meetsGlobalRequirement(const TtbarHypothesisPointer solution);
	virtual boost::array<ParticlePointer, 2> getNeutrinos();
//	virtual void throwDetailedException() const;

protected:
	TtbarHypothesisCollection solutions;
	bool alreadyReconstructed;
	METPointer met;
	JetCollection jets;
	LeptonPointer leptonFromW;
};

typedef boost::shared_ptr<BasicTopPairReconstruction> TopPairReconstruction;

} /* namespace BAT */
#endif /* BASICTOPPAIRRECONSTRUCTION_H_ */
