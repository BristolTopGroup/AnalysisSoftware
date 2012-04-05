/*
 * NeutrinoReconstructionByApproximation.cpp
 *
 *  Created on: Apr 2, 2012
 *      Author: lkreczko
 */

#include "../../interface/ReconstructionModules/NeutrinoReconstructionByApproximation.h"

namespace BAT {
const double initialBigValue = 123456789;

boost::array<ParticlePointer, 2> NeutrinoReconstructionByApproximation::getNeutrinos(unsigned int approximation) {
	ParticlePointer neutrino(new Particle(initialBigValue, initialBigValue, initialBigValue, initialBigValue));
	switch (approximation) {
	case NeutrinoApproximation::ScalingMETApproximation:
		neutrino = scalingMETApproximation();
		break;
	case NeutrinoApproximation::SameEtaApproximation:
		neutrino = sameEtaApproximation();
		break;
	case NeutrinoApproximation::ColinearApproximation:
		neutrino = colinearApproximation();
		break;
	case NeutrinoApproximation::NullDeltaApproximation:
		neutrino = nullDeltaApproximation();
		break;
	}

	boost::array<ParticlePointer, 2> neutrinos;
	neutrinos.at(0) = neutrino;
	neutrinos.at(1) = neutrino;

	return neutrinos;
}

ParticlePointer NeutrinoReconstructionByApproximation::scalingMETApproximation() {
	const double WMass(BasicNeutrinoReconstruction::W_mass);
	double MissingPx(met->px()), MissingPy(met->py());
	double MissingPt = met->pt();
	double SumPt = MissingPt + leptonFromW->pt();
	double SumPx = MissingPx + leptonFromW->px();
	double SumPy = MissingPy + leptonFromW->py();

	double WTransverseMass = sqrt(SumPt * SumPt - SumPx * SumPx - SumPy * SumPy);
	double factor = WMass / WTransverseMass;

	double NewMissingPx = MissingPx * (factor * factor);
	double NewMissingPy = MissingPy * (factor * factor);

	MissingPt = sqrt(NewMissingPx * NewMissingPx + NewMissingPy * NewMissingPy);
	SumPx = NewMissingPx + leptonFromW->px();
	SumPy = NewMissingPy + leptonFromW->py();

	double alpha = WMass * WMass + SumPx * SumPx + SumPy * SumPy - leptonFromW->energy() * leptonFromW->energy();
	double beta = 0.5 * (alpha - MissingPt * MissingPt + leptonFromW->pz() * leptonFromW->pz());
	double lambda = 2. * beta * leptonFromW->pz() / (leptonFromW->energy() * leptonFromW->energy() - leptonFromW->pz()
			* leptonFromW->pz());

	double Pz = lambda / 2.;
	double energy = sqrt(MissingPt * MissingPt + Pz * Pz);

	ParticlePointer neutrino(new Particle(energy, NewMissingPx, NewMissingPy, Pz));

	return neutrino;
}

ParticlePointer NeutrinoReconstructionByApproximation::sameEtaApproximation() {
	double Theta = leptonFromW->theta();
	double MissingPt(met->pt());
	double Phi = met->phi();
	//calculate the neutrino momentum based on it's transverse momentum and the leptons theta angle(z, xy-plane)
	double momentum = MissingPt / sin(Theta);

	TVector3 Vector;
	Vector.SetMagThetaPhi(momentum, Theta, Phi);

	ParticlePointer neutrino(new Particle(momentum, Vector.Px(), Vector.Py(), Vector.Pz()));

	return neutrino;
}

ParticlePointer NeutrinoReconstructionByApproximation::colinearApproximation() {
	double MissingPx(met->px()), MissingPy(met->py());
	double leptonPz(leptonFromW->pz());
	double energy = sqrt(MissingPx * MissingPx + MissingPy * MissingPy + leptonPz * leptonPz);

	ParticlePointer neutrino(new Particle(energy, MissingPx, MissingPy, leptonPz));
	return neutrino;
}

ParticlePointer NeutrinoReconstructionByApproximation::nullDeltaApproximation() {
	const double WMass(BasicNeutrinoReconstruction::W_mass);
	double MissingPx(met->px()), MissingPy(met->py()), MissingPt(met->pt());
	double leptonPz(leptonFromW->pz()), leptonEnergy(leptonFromW->energy());

	double SumPx = MissingPx + leptonFromW->px();
	double SumPy = MissingPy + leptonFromW->py();

	double alpha = WMass * WMass + SumPx * SumPx + SumPy * SumPy - leptonEnergy * leptonEnergy;
	double beta = 0.5 * (alpha - MissingPt * MissingPt + leptonPz * leptonPz);
	double lambda = 2. * beta * leptonPz / (leptonEnergy * leptonEnergy - leptonPz * leptonPz);
	double Pz = lambda / 2.;
	double energy = sqrt(MissingPt * MissingPt + Pz * Pz);

	ParticlePointer neutrino(new Particle(energy, MissingPx, MissingPy, Pz));
	return neutrino;
}

NeutrinoReconstructionByApproximation::NeutrinoReconstructionByApproximation(const LeptonPointer lepton,
		const METPointer met) :
	BasicNeutrinoReconstruction(lepton, met) {
}

NeutrinoReconstructionByApproximation::~NeutrinoReconstructionByApproximation() {
}

}
