/*
 * BasicNeutrinoReconstruction.h
 *
 *  Created on: 25 Aug 2011
 *      Author: kreczko
 */

#ifndef BASICNEUTRINORECONSTRUCTION_H_
#define BASICNEUTRINORECONSTRUCTION_H_

#include "../RecoObjects/Electron.h"
#include "../RecoObjects/MET.h"
#include "../RecoObjects/Particle.h"
#include <boost/shared_ptr.hpp>
#include "ReconstructionException.h"

namespace BAT {
namespace NeutrinoSelection {
enum value {
	None,
    pzClosestToLepton,
    mostCentral,
    pzClosestToLeptonOrMostcentralIfAbove300,
    largestValueOfCosine
};
}


class BasicNeutrinoReconstruction {
public:
	BasicNeutrinoReconstruction(const ElectronPointer electron, const METPointer met);
	virtual ~BasicNeutrinoReconstruction();
	void setMETResolution(double resolution);
	boost::array<ParticlePointer, 2> getNeutrinos(NeutrinoSelection::value selection = NeutrinoSelection::None);

protected:
	boost::array<double, 2> resolveComplexROOT(double root, double A, double B);
	boost::array<double, 2> computeNeutrinoPz();
	void reconstructNeutrinos();
	void selectNeutrinos(NeutrinoSelection::value selection = NeutrinoSelection::None);
	bool isValidSolution(const ParticlePointer neutrino);

protected:
	static double const W_mass;

	ElectronPointer electronFromW;
	METPointer met;
	ParticlePointer neutrino1, neutrino2;
	bool alreadyReconstructed;
};

typedef boost::shared_ptr<BasicNeutrinoReconstruction> NeutrinoReconstruction;

} /* namespace BAT */
#endif /* BASICNEUTRINORECONSTRUCTION_H_ */
