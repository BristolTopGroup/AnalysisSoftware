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
#include "BasicNeutrinoReconstruction.h"

namespace BAT {

class BasicTopPairReconstruction {
public:
	BasicTopPairReconstruction(const ElectronPointer electron, const METPointer met, const JetCollection jets);
	virtual ~BasicTopPairReconstruction();
	const TtbarHypothesisPointer getBestSolution();
	std::vector<TtbarHypothesisPointer> getAllSolutions();

protected:
	double getDiscriminator(const TtbarHypothesisPointer) const;
	void reconstruct();
	bool meetsHadronicBJetRequirement(unsigned short jetIndex);
	bool meetsLeptonicBJetRequirement(unsigned short jetIndex);
	bool meetsJetFromWRequirement(unsigned short jet1Index, unsigned short jet2Index);
	bool meetsGlobalRequirement(const TtbarHypothesisPointer solution);

protected:
	std::vector<TtbarHypothesisPointer> solutions;
	bool alreadyReconstructed;
	compare_disc solutionComparator;
	METPointer met;
	JetCollection jets;
	ElectronPointer electronFromW;
	BasicNeutrinoReconstruction neutrinoReconstruction;
};

typedef boost::shared_ptr<BasicTopPairReconstruction> TopPairReconstruction;

} /* namespace BAT */
#endif /* BASICTOPPAIRRECONSTRUCTION_H_ */
