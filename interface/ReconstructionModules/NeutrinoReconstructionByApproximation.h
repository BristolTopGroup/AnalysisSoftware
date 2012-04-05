/*
 * NeutrinoReconstructionByApproximation.h
 *
 *  Created on: Apr 2, 2012
 *      Author: lkreczko
 */

#ifndef NEUTRINORECONSTRUCTIONBYAPPROXIMATION_H_
#define NEUTRINORECONSTRUCTIONBYAPPROXIMATION_H_

#include "BasicNeutrinoReconstruction.h"

namespace BAT {
namespace NeutrinoApproximation {
enum value {
	ScalingMETApproximation, SameEtaApproximation, ColinearApproximation, NullDeltaApproximation
};
}

class NeutrinoReconstructionByApproximation: public BAT::BasicNeutrinoReconstruction {
public:
	NeutrinoReconstructionByApproximation(const LeptonPointer lepton, const METPointer met);
	virtual ~NeutrinoReconstructionByApproximation();
	virtual boost::array<ParticlePointer, 2> getNeutrinos(unsigned int approximation =
			NeutrinoApproximation::ScalingMETApproximation);

	ParticlePointer scalingMETApproximation();
	ParticlePointer sameEtaApproximation();
	ParticlePointer colinearApproximation();
	ParticlePointer nullDeltaApproximation();

};

}

#endif /* NEUTRINORECONSTRUCTIONBYAPPROXIMATION_H_ */
