/*
 * BasicNeutrinoReconstruction.cpp
 *
 *  Created on: 25 Aug 2011
 *      Author: kreczko
 */

#include "../../interface/ReconstructionModules/BasicNeutrinoReconstruction.h"
#include "../../interface/ReconstructionModules/EquationSolver.h"
#include "TVector3.h"
#include <iostream>


namespace BAT {
double const BasicNeutrinoReconstruction::W_mass = 80.389;

BasicNeutrinoReconstruction::BasicNeutrinoReconstruction(const LeptonPointer lepton, const METPointer missingET):
		leptonFromW(lepton),
		met(missingET),
		neutrino1(),
		neutrino2(),
		alreadyReconstructed(false){
	if (leptonFromW == 0)
		throw ReconstructionException("Could not reconstruct neutrinos: no electron found!");
	if (met->energy() == 0)
		throw ReconstructionException("Could not reconstruct neutrinos: no MET found!");
}

boost::array<ParticlePointer, 2> BasicNeutrinoReconstruction::getNeutrinos(unsigned int selection) {
	boost::array<ParticlePointer, 2> neutrinos;
	if(!alreadyReconstructed){
		reconstructNeutrinos();
	}

	selectNeutrinos((NeutrinoSelection::value) selection);

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
	using namespace std;
	boost::array<double, 2> neutrinoZMomenta;

	double pz1(0), pz2(0);
	double lepton_M = 0.0005;
	double lepton_E = leptonFromW->energy();
	double lepton_px = leptonFromW->px();
	double lepton_py = leptonFromW->py();
	double lepton_pz = leptonFromW->pz();
	double neutrino_px = met->px();
	double neutrino_py = met->py();

	double a = W_mass * W_mass - lepton_M * lepton_M + 2.0 * lepton_px * neutrino_px
			+ 2.0 * lepton_py * neutrino_py;

	double A = 4.0 * (lepton_E * lepton_E - lepton_pz * lepton_pz);
	double B = -4.0 * a * lepton_pz;
	double C = 4.0 * lepton_E * lepton_E * (neutrino_px * neutrino_px + neutrino_py * neutrino_py) - a * a;

	double tmproot = B * B - 4.0 * A * C;
	if (tmproot < 0) {
		boost::array<double, 2> solutions = resolveComplexROOT(tmproot, A, B);
//		boost::array<double, 2> solutions = resolveComplexROOT(true, true);
//		cout << "complex ROOT" << endl;
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

//boost::array<double, 2> BasicNeutrinoReconstruction::resolveComplexROOT(){
//	boost::array<double, 2> solutions;
//	double metResolution = met->resolution();
//	double met_et = met->et();
//	METPointer newMET(met);
//
//	for(double x = -metResolution; x < metResolution; x=+0.1){
//		double newScale = (met_et + x)/met_et;
//		double metx = met->px()*(1+newScale);
//		boost::array<double, 2> mety = getMETY(metx);
//
//	}
//
//	solutions[0] = 0;
//	solutions[1] = 0;
//	return solutions;
//}

boost::array<double, 2> BasicNeutrinoReconstruction::resolveComplexROOT(double root, double A, double B){
	boost::array<double, 2> solutions;
	solutions[0] = -B/(2*A);
	solutions[1] = -B/(2*A);
	return solutions;
}

boost::array<double, 2> BasicNeutrinoReconstruction::resolveComplexROOT(bool usePxMinusSolutions_,
		bool usePxPlusSolutions_) {
	boost::array<double, 2> solutionsArray;

	double ptlep = leptonFromW->pt(), pxlep = leptonFromW->px(), pylep = leptonFromW->py(), metpx = met->px(), metpy =
			met->py();
	double mW = W_mass;
	double EquationA = 1;
	double EquationB = -3 * pylep * mW / (ptlep);
	double EquationC = mW * mW * (2 * pylep * pylep) / (ptlep * ptlep) + mW * mW
			- 4 * pxlep * pxlep * pxlep * metpx / (ptlep * ptlep) - 4 * pxlep * pxlep * pylep * metpy / (ptlep * ptlep);
	double EquationD = 4 * pxlep * pxlep * mW * metpy / (ptlep) - pylep * mW * mW * mW / ptlep;

	std::vector<long double> solutions = EquationSolve<long double>((long double) EquationA, (long double) EquationB,
			(long double) EquationC, (long double) EquationD);

	std::vector<long double> solutions2 = EquationSolve<long double>((long double) EquationA, -(long double) EquationB,
			(long double) EquationC, -(long double) EquationD);

	double deltaMin = 14000 * 14000;
	double zeroValue = -mW * mW / (4 * pxlep);
	double minPx = 0;
	double minPy = 0;

	if (usePxMinusSolutions_) {
		for (int i = 0; i < (int) solutions.size(); ++i) {
			if (solutions[i] < 0)
				continue;
			double p_x = (solutions[i] * solutions[i] - mW * mW) / (4 * pxlep);
			double p_y = (mW * mW * pylep + 2 * pxlep * pylep * p_x - mW * ptlep * solutions[i]) / (2 * pxlep * pxlep);
			double Delta2 = (p_x - metpx) * (p_x - metpx) + (p_y - metpy) * (p_y - metpy);

			//      std::cout<<"intermediate solution1 met x "<<metpx << " min px " << p_x  <<" met y "<<metpy <<" min py "<< p_y << std::endl;

			if (Delta2 < deltaMin && Delta2 > 0) {
				deltaMin = Delta2;
				minPx = p_x;
				minPy = p_y;
			}
			//     std::cout<<"solution1 met x "<<metpx << " min px " << minPx  <<" met y "<<metpy <<" min py "<< minPy << std::endl;
		}
	}

	if (usePxPlusSolutions_) {
		for (int i = 0; i < (int) solutions2.size(); ++i) {
			if (solutions2[i] < 0)
				continue;
			double p_x = (solutions2[i] * solutions2[i] - mW * mW) / (4 * pxlep);
			double p_y = (mW * mW * pylep + 2 * pxlep * pylep * p_x + mW * ptlep * solutions2[i]) / (2 * pxlep * pxlep);
			double Delta2 = (p_x - metpx) * (p_x - metpx) + (p_y - metpy) * (p_y - metpy);
			//  std::cout<<"intermediate solution2 met x "<<metpx << " min px " << minPx  <<" met y "<<metpy <<" min py "<< minPy << std::endl;
			if (Delta2 < deltaMin && Delta2 > 0) {
				deltaMin = Delta2;
				minPx = p_x;
				minPy = p_y;
			}
			//	std::cout<<"solution2 met x "<<metpx << " min px " << minPx  <<" met y "<<metpy <<" min py "<< minPy << std::endl;
		}
	}

	double pyZeroValue = (mW * mW * pxlep + 2 * pxlep * pylep * zeroValue);
	double delta2ZeroValue = (zeroValue - metpx) * (zeroValue - metpx) + (pyZeroValue - metpy) * (pyZeroValue - metpy);
	if (delta2ZeroValue < deltaMin) {
		deltaMin = delta2ZeroValue;
		minPx = zeroValue;
		minPy = pyZeroValue;
	}

	//    std::cout<<" MtW2 from min py and min px "<< sqrt((minPy*minPy+minPx*minPx))*ptlep*2 -2*(pxlep*minPx + pylep*minPy)  <<std::endl;
	///    ////Y part

	double mu_Minimum = (mW * mW) / 2 + minPx * pxlep + minPy * pylep;
	double a_Minimum = (mu_Minimum * leptonFromW->pz()) / (leptonFromW->energy() * leptonFromW->energy() - leptonFromW->pz() * leptonFromW->pz());

	solutionsArray[0] = a_Minimum;
	solutionsArray[1] = a_Minimum;
	return solutionsArray;
}



void BasicNeutrinoReconstruction::selectNeutrinos(NeutrinoSelection::value selection){
	unsigned short currentSelection(0);

	switch (selection) {
	case NeutrinoSelection::None:
		return;
	case NeutrinoSelection::pzClosestToLepton:
		fabs(neutrino1->pz() - leptonFromW->pz())
				< fabs(neutrino2->pz() - leptonFromW->pz()) ?
				currentSelection = 1 : currentSelection = 2;
		break;

	case NeutrinoSelection::mostCentral:
		fabs(neutrino1->pz()) < fabs(neutrino2->pz()) ?
				currentSelection = 1 : currentSelection = 2;
		break;

	case NeutrinoSelection::pzClosestToLeptonOrMostcentralIfAbove300:
		fabs(neutrino1->pz() - leptonFromW->pz())
				< fabs(neutrino2->pz() - leptonFromW->pz()) ?
						currentSelection = 1 : currentSelection = 2;

		if (fabs(neutrino1->pz()) > 300 || fabs(neutrino2->pz()) > 300)
			fabs(neutrino1->pz()) < fabs(neutrino2->pz()) ?
					currentSelection = 1 : currentSelection = 2;
		break;

	case NeutrinoSelection::largestValueOfCosine:
		TVector3 p3W, p3e;
		ParticlePointer leptonicW1(new Particle(*neutrino1 + *leptonFromW));
		ParticlePointer leptonicW2(new Particle(*neutrino2 + *leptonFromW));

		p3e = leptonFromW->getFourVector().Vect();

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

double BasicNeutrinoReconstruction::getMET(double metx, double mety) const{
	return sqrt(metx*metx + mety*mety);
}

boost::array<double, 2> BasicNeutrinoReconstruction::getMETX(double mety) const{
	boost::array<double, 2> solutions;

	double px = leptonFromW->px();
	double py = leptonFromW->py();
	double pt = leptonFromW->pt();
	double p = leptonFromW->getFourVector().P();
	double e = leptonFromW->energy();
	double A = pow(W_mass, 4)/4.;
	double B = W_mass*W_mass*px;
	double C = W_mass*W_mass*py;
	double D = 2*px*py;
	double E = px*px + e*e/(p*p)*pt*pt;
	double F = py*py + e*e/(p*p)*pt*pt;

	double fixed = -(B+D*mety)/(2*E);
	double tmproot = ((B+D*mety)*(B+D*mety) - 4*E*(A + C*mety + F* mety*mety));
	double metx1 = fixed + sqrt(tmproot)/(2*E);
	double metx2 = fixed - sqrt(tmproot)/(2*E);

	solutions[0] = metx1;
	solutions[1] = metx2;
	return solutions;
}

boost::array<double, 2> BasicNeutrinoReconstruction::getMETY(double metx) const{
	boost::array<double, 2> solutions;

	double px = leptonFromW->px();
	double py = leptonFromW->py();
	double pt = leptonFromW->pt();
	double p = leptonFromW->getFourVector().P();
	double e = leptonFromW->energy();
	double A = pow(W_mass, 4)/4.;
	double B = W_mass*W_mass*px;
	double C = W_mass*W_mass*py;
	double D = 2*px*py;
	double E = px*px + e*e/(p*p)*pt*pt;
	double F = py*py + e*e/(p*p)*pt*pt;

	double fixed = -(C+D*metx)/(2*F);
	double tmproot = ((C+D*metx)*(C+D*metx) - 4*F*(A + B*metx + E* metx*metx));
	double metx1 = fixed + sqrt(tmproot)/(2*F);
	double metx2 = fixed - sqrt(tmproot)/(2*F);

	solutions[0] = metx1;
	solutions[1] = metx2;
	return solutions;
}

BasicNeutrinoReconstruction::~BasicNeutrinoReconstruction() {
}

} /* namespace BAT */
