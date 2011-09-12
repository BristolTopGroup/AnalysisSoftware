/*
 * BasicNeutrinoReconstruction.cpp
 *
 *  Created on: 25 Aug 2011
 *      Author: kreczko
 */

#include "../../interface/RecontructionModules/BasicNeutrinoReconstruction.h"
#include "TVector3.h"

namespace BAT {
double const BasicNeutrinoReconstruction::W_mass = 80.389;

BasicNeutrinoReconstruction::BasicNeutrinoReconstruction(const ElectronPointer electron, const METPointer missingET):
		electronFromW(electron),
		met(missingET),
		neutrino1(),
		neutrino2(),
		alreadyReconstructed(false){
	if (electronFromW == 0)
		throw ReconstructionException("Could not reconstruct neutrinos: no electron found!");
	if (met->energy() == 0)
		throw ReconstructionException("Could not reconstruct neutrinos: no MET found!");
}

boost::array<ParticlePointer, 2> BasicNeutrinoReconstruction::getNeutrinos(NeutrinoSelection::value selection) {
	boost::array<ParticlePointer, 2> neutrinos;
	if(!alreadyReconstructed){
		reconstructNeutrinos();
	}

	selectNeutrinos(selection);

	neutrinos.at(0) = neutrino1;
	neutrinos.at(1) = neutrino2;

	return neutrinos;
}

void BasicNeutrinoReconstruction::reconstructNeutrinos() {
	boost::array<double, 2> neutrinoPzs = computeNeutrinoPz();
	double energy1 = sqrt(met->et() * met->et() + neutrinoPzs.at(0) * neutrinoPzs.at(0));
	double energy2 = sqrt(met->et() * met->et() + neutrinoPzs.at(1) * neutrinoPzs.at(1));
	neutrino1 = ParticlePointer(new Particle(energy1, met->px(), met->py(), neutrinoPzs.at(0)));
	neutrino2 = ParticlePointer(new Particle(energy2, met->px(), met->py(), neutrinoPzs.at(1)));

	if (isnan(neutrino1->energy()) && isnan(neutrino2->energy())
		)
		throw ReconstructionException("No physical neutrino solution found");
	else if (isnan(neutrino1->energy())
		)
		neutrino1 = neutrino2;
	else if (isnan(neutrino2->energy())
		)
		neutrino2 = neutrino1;
}

boost::array<double, 2> BasicNeutrinoReconstruction::computeNeutrinoPz() {
	boost::array<double, 2> neutrinoZMomenta;

	double pz1(0), pz2(0);
	double electron_M = 0.0005;
	double electron_E = electronFromW->energy();
	double electron_px = electronFromW->px();
	double electron_py = electronFromW->py();
	double electron_pz = electronFromW->pz();
	double neutrino_px = met->px();
	double neutrino_py = met->py();

	double a = W_mass * W_mass - electron_M * electron_M + 2.0 * electron_px * neutrino_px
			+ 2.0 * electron_py * neutrino_py;

	double A = 4.0 * (electron_E * electron_E - electron_pz * electron_pz);
	double B = -4.0 * a * electron_pz;
	double C = 4.0 * electron_E * electron_E * (neutrino_px * neutrino_px + neutrino_py * neutrino_py) - a * a;

	double tmproot = B * B - 4.0 * A * C;
	if (tmproot < 0) {
		boost::array<double, 2> solutions = resolveComplexROOT(tmproot, A, B);
		pz1 = solutions[0];
		pz2 = solutions[1];
	} else {
		pz1 = (-B + TMath::Sqrt(tmproot)) / (2.0 * A);
		pz2 = (-B - TMath::Sqrt(tmproot)) / (2.0 * A);

	}

	neutrinoZMomenta[0] = pz1;
	neutrinoZMomenta[1] = pz2;

	return neutrinoZMomenta;
}

boost::array<double, 2> BasicNeutrinoReconstruction::resolveComplexROOT(double root, double A, double B){
	boost::array<double, 2> solutions;
	solutions[0] = -B/(2*A);
	solutions[1] = -B/(2*A);
	return solutions;
}


void BasicNeutrinoReconstruction::selectNeutrinos(NeutrinoSelection::value selection){
	unsigned short currentSelection(0);

	switch (selection) {
	case NeutrinoSelection::None:
		return;
	case NeutrinoSelection::pzClosestToLepton:
		fabs(neutrino1->pz() - electronFromW->pz())
				< fabs(neutrino2->pz() - electronFromW->pz()) ?
				currentSelection = 1 : currentSelection = 2;
		break;

	case NeutrinoSelection::mostCentral:
		fabs(neutrino1->pz()) < fabs(neutrino2->pz()) ?
				currentSelection = 1 : currentSelection = 2;
		break;

	case NeutrinoSelection::pzClosestToLeptonOrMostcentralIfAbove300:
		fabs(neutrino1->pz() - electronFromW->pz())
				< fabs(neutrino2->pz() - electronFromW->pz()) ?
						currentSelection = 1 : currentSelection = 2;

		if (fabs(neutrino1->pz()) > 300 || fabs(neutrino2->pz()) > 300)
			fabs(neutrino1->pz()) < fabs(neutrino2->pz()) ?
					currentSelection = 1 : currentSelection = 2;
		break;

	case NeutrinoSelection::largestValueOfCosine:
		TVector3 p3W, p3e;
		ParticlePointer leptonicW1(new Particle(*neutrino1 + *electronFromW));
		ParticlePointer leptonicW2(new Particle(*neutrino2 + *electronFromW));

		p3e = electronFromW->getFourVector().Vect();

		p3W = leptonicW1->getFourVector().Vect();
		double sinthcm1 = 2. * (p3e.Perp(p3W)) / W_mass;

		p3W = leptonicW2->getFourVector().Vect();
		double sinthcm2 = 2. * (p3e.Perp(p3W)) / W_mass;

		double costhcm1 = TMath::Sqrt(1. - sinthcm1 * sinthcm1);
		double costhcm2 = TMath::Sqrt(1. - sinthcm2 * sinthcm2);
		costhcm1 > costhcm2 ?
				currentSelection = 1 : currentSelection = 2;
		break;
	}

	if (currentSelection == 1)//over-write the 2nd neutrino
		neutrino2 = neutrino1;

	if (currentSelection == 2)//over-write the 1st neutrino
		neutrino1 = neutrino2;
}


BasicNeutrinoReconstruction::~BasicNeutrinoReconstruction() {
}

} /* namespace BAT */
