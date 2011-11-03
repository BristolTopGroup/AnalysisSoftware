/*
 * ChiSquaredThreeJetsTopPairReconstruction.h
 *
 *  Created on: 2 Sep 2011
 *      Author: kreczko
 */

#ifndef CHISQUAREDTHREEJETSTOPPAIRRECONSTRUCTION_H_
#define CHISQUAREDTHREEJETSTOPPAIRRECONSTRUCTION_H_

#include "ChiSquaredBasedTopPairReconstruction.h"

namespace BAT {

class ChiSquaredThreeJetsTopPairReconstruction: public BAT::ChiSquaredBasedTopPairReconstruction {
public:
	ChiSquaredThreeJetsTopPairReconstruction(const LeptonPointer lepton, const METPointer met,
			const JetCollection jets);
	virtual ~ChiSquaredThreeJetsTopPairReconstruction();
	virtual bool meetsInitialCriteria() const;
	virtual std::string getDetailsOnFailure() const;

protected:
	virtual bool meetsJetFromWRequirement(unsigned short jet1Index, unsigned short jet2Index);
	virtual bool meetsGlobalRequirement(const TtbarHypothesisPointer solution);

};

} /* namespace BAT */
#endif /* CHISQUAREDTHREEJETSTOPPAIRRECONSTRUCTION_H_ */
