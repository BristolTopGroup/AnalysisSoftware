/*
 * ChiSquaredBasedTopPairReconstruction.h
 *
 *  Created on: 25 Aug 2011
 *      Author: kreczko
 */

#ifndef CHISQUAREDBASEDTOPPAIRRECONSTRUCTION_H_
#define CHISQUAREDBASEDTOPPAIRRECONSTRUCTION_H_

#include "BasicTopPairReconstruction.h"

namespace BAT {

class ChiSquaredBasedTopPairReconstruction: virtual public BAT::BasicTopPairReconstruction {
public:
	ChiSquaredBasedTopPairReconstruction(const LeptonPointer lepton, const METPointer met, const JetCollection jets);
	virtual ~ChiSquaredBasedTopPairReconstruction();

protected:
	//chiSquared variables from SM ttbar MC
	static double const leptonic_top_mass;
	static double const leptonic_top_mass_sigma;

	static double const hadronic_W_mass;
	static double const hadronic_W_mass_sigma;

	static double const hadronic_top_mass;
	static double const hadronic_top_mass_sigma;

	static double const pt_ttbarSystem;
	static double const pt_ttbarSystem_sigma;

	static double const HTSystem;
	static double const HTSystem_sigma;

	virtual double HT(unsigned short jetLimit) const;
	virtual double getDiscriminator(const TtbarHypothesisPointer) const;
};

} /* namespace BAT */
#endif /* CHISQUAREDBASEDTOPPAIRRECONSTRUCTION_H_ */
