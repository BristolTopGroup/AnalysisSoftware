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
	BasicNeutrinoReconstruction(const LeptonPointer lepton, const METPointer met);
	virtual ~BasicNeutrinoReconstruction();
//	void setMETResolution(double resolution);
	virtual boost::array<ParticlePointer, 2> getNeutrinos(unsigned int selection = NeutrinoSelection::None);
	static double const W_mass;
protected:
	boost::array<double, 2> resolveComplexROOT(double root, double A, double B);
	boost::array<double, 2> resolveComplexROOT(bool, bool);
	boost::array<double, 2> resolveComplexROOT();
	boost::array<double, 2> computeNeutrinoPz();
	void reconstructNeutrinos();
	void selectNeutrinos(NeutrinoSelection::value selection = NeutrinoSelection::None);
	bool isValidSolution(const ParticlePointer neutrino);

	//functions for MET_x and MET_y which satisfy the W-> l nu condition
	boost::array<double, 2> getMETX(double mety) const;
	boost::array<double, 2> getMETY(double metx) const;
	double getMET(double metx, double mety) const;


protected:


	LeptonPointer leptonFromW;
	METPointer met;
	ParticlePointer neutrino1, neutrino2;
	bool alreadyReconstructed;
};

typedef boost::shared_ptr<BasicNeutrinoReconstruction> NeutrinoReconstruction;

} /* namespace BAT */
#endif /* BASICNEUTRINORECONSTRUCTION_H_ */
