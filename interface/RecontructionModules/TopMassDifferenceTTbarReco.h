/*
 * TopMassDifferenceTTbarReco.h
 *
 *  Created on: 2 Sep 2011
 *      Author: kreczko
 */

#ifndef TOPMASSDIFFERENCETTBARRECO_H_
#define TOPMASSDIFFERENCETTBARRECO_H_

#include "BasicTopPairReconstruction.h"

namespace BAT {

class TopMassDifferenceTTbarReco: public BAT::BasicTopPairReconstruction {
public:
	TopMassDifferenceTTbarReco(const LeptonPointer lepton, const METPointer met, const JetCollection jets);
	virtual ~TopMassDifferenceTTbarReco();

	double getDiscriminator(const TtbarHypothesisPointer) const;
};

} /* namespace BAT */
#endif /* TOPMASSDIFFERENCETTBARRECO_H_ */
