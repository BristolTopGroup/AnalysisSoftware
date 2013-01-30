/*
 * Lepton.cpp
 *
 *  Created on: 30 Oct 2011
 *      Author: kreczko
 */

#include "../../interface/RecoObjects/Lepton.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "../../interface/GlobalVariables.h"

using namespace std;
namespace BAT {
const double initialBigValue = 123456789.;

Lepton::Lepton() : //
		Particle(), //
		ecal_Isolation_DR03_(initialBigValue), //
		hcal_Isolation_DR03_(initialBigValue), //
		tracker_Isolation_DR03_(initialBigValue), //
		ecal_Isolation_DR04_(initialBigValue), //
		hcal_Isolation_DR04_(initialBigValue), //
		tracker_Isolation_DR04_(initialBigValue), //
		ecal_Isolation_DR05_(initialBigValue), //
		hcal_Isolation_DR05_(initialBigValue), //
		tracker_Isolation_DR05_(initialBigValue), //
		PFGamma_Isolation_DR03_(initialBigValue), //
		PFChargedHadron_Isolation_DR03_(initialBigValue), //
		PFNeutralHadron_Isolation_DR03_(initialBigValue), //
		PFPUChargedHadron_Isolation_DR03_(initialBigValue), //
		PFGamma_Isolation_DR04_(initialBigValue), //
		PFChargedHadron_Isolation_DR04_(initialBigValue), //
		PFNeutralHadron_Isolation_DR04_(initialBigValue), //
		PFPUChargedHadron_Isolation_DR04_(initialBigValue), //
		PFGamma_Isolation_DR05_(initialBigValue), //
		PFChargedHadron_Isolation_DR05_(initialBigValue), //
		PFNeutralHadron_Isolation_DR05_(initialBigValue), //
		PFPUChargedHadron_Isolation_DR05_(initialBigValue), //
		PFDeltaBeta_Isolation_DR04_(initialBigValue), //
		sumChargedHadronPt04_(initialBigValue), //
		sumNeutralHadronPt04_(initialBigValue), //
		sumPhotonPt04_(initialBigValue), //
		sumPUPt04_(initialBigValue), //
		PFRelativeIsolationRho_DR03_(initialBigValue), //
		zDistanceToPrimaryVertex_(initialBigValue), //
		directionalIsolation_DR02_(initialBigValue), //
		directionalIsolation_DR03_(initialBigValue), //
		directionalIsolation_GaussianFallOff_DR02_(initialBigValue), //
		directionalIsolation_GaussianFallOff_DR03_(initialBigValue), //
		pfIsolation_GaussianFallOff_DR02_(initialBigValue), //
		pfIsolation_GaussianFallOff_DR03_(initialBigValue) {
}

Lepton::Lepton(double energy, double px, double py, double pz) : //
		Particle(energy, px, py, pz), //
		ecal_Isolation_DR03_(initialBigValue), //
		hcal_Isolation_DR03_(initialBigValue), //
		tracker_Isolation_DR03_(initialBigValue), //
		ecal_Isolation_DR04_(initialBigValue), //
		hcal_Isolation_DR04_(initialBigValue), //
		tracker_Isolation_DR04_(initialBigValue), //
		ecal_Isolation_DR05_(initialBigValue), //
		hcal_Isolation_DR05_(initialBigValue), //
		tracker_Isolation_DR05_(initialBigValue), //
		PFGamma_Isolation_DR03_(initialBigValue), //
		PFChargedHadron_Isolation_DR03_(initialBigValue), //
		PFNeutralHadron_Isolation_DR03_(initialBigValue), //
		PFPUChargedHadron_Isolation_DR03_(initialBigValue), //
		PFGamma_Isolation_DR04_(initialBigValue), //
		PFChargedHadron_Isolation_DR04_(initialBigValue), //
		PFNeutralHadron_Isolation_DR04_(initialBigValue), //
		PFPUChargedHadron_Isolation_DR04_(initialBigValue), //
		PFGamma_Isolation_DR05_(initialBigValue), //
		PFChargedHadron_Isolation_DR05_(initialBigValue), //
		PFNeutralHadron_Isolation_DR05_(initialBigValue), //
		PFPUChargedHadron_Isolation_DR05_(initialBigValue), //
		PFDeltaBeta_Isolation_DR04_(initialBigValue), //
		sumChargedHadronPt04_(initialBigValue), //
		sumNeutralHadronPt04_(initialBigValue), //
		sumPhotonPt04_(initialBigValue), //
		sumPUPt04_(initialBigValue), //
		PFRelativeIsolationRho_DR03_(initialBigValue), //
		zDistanceToPrimaryVertex_(initialBigValue), //
		directionalIsolation_DR02_(initialBigValue), //
		directionalIsolation_DR03_(initialBigValue), //
		directionalIsolation_GaussianFallOff_DR02_(initialBigValue), //
		directionalIsolation_GaussianFallOff_DR03_(initialBigValue), //
		pfIsolation_GaussianFallOff_DR02_(initialBigValue), //
		pfIsolation_GaussianFallOff_DR03_(initialBigValue) {

}

void Lepton::setEcalIsolation(double isolation, double coneSize) {
	if (coneSize == 0.3)
		ecal_Isolation_DR03_ = isolation;
	else if (coneSize == 0.4)
		ecal_Isolation_DR04_ = isolation;
	else if (coneSize == 0.5)
		ecal_Isolation_DR05_ = isolation;
	else {
		std::cerr << "Lepton::setEcalIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
}

void Lepton::setHcalIsolation(double isolation, double coneSize) {
	if (coneSize == 0.3)
		hcal_Isolation_DR03_ = isolation;
	else if (coneSize == 0.4)
		hcal_Isolation_DR04_ = isolation;
	else if (coneSize == 0.5)
		hcal_Isolation_DR05_ = isolation;
	else {
		std::cerr << "Lepton::setHcalIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
}

void Lepton::setTrackerIsolation(double isolation, double coneSize) {
	if (coneSize == 0.3)
		tracker_Isolation_DR03_ = isolation;
	else if (coneSize == 0.4)
		tracker_Isolation_DR04_ = isolation;
	else if (coneSize == 0.5)
		tracker_Isolation_DR05_ = isolation;
	else {
		std::cerr << "Lepton::setTrackerIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
}

double Lepton::trackerIsolation(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.3)
		isolation = tracker_Isolation_DR03_;
	else if (coneSize == 0.4)
		isolation = tracker_Isolation_DR04_;
	else if (coneSize == 0.5)
		isolation = tracker_Isolation_DR05_;
	else {
		std::cerr << "Lepton::trackerIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
	return isolation;
}

double Lepton::hcalIsolation(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.3)
		isolation = hcal_Isolation_DR03_;
	else if (coneSize == 0.4)
		isolation = hcal_Isolation_DR04_;
	else if (coneSize == 0.5)
		isolation = hcal_Isolation_DR05_;
	else {
		std::cerr << "Lepton::hcalIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
	return isolation;
}

double Lepton::ecalIsolation(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.3)
		isolation = ecal_Isolation_DR03_;
	else if (coneSize == 0.4)
		isolation = ecal_Isolation_DR04_;
	else if (coneSize == 0.5)
		isolation = ecal_Isolation_DR05_;
	else {
		std::cerr << "Lepton::ecalIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
	return isolation;
}

void Lepton::setPFGammaIsolation(double pfGammaIso, double coneSize) {
	if (coneSize == 0.3)
		PFGamma_Isolation_DR03_ = pfGammaIso;
	else if (coneSize == 0.4)
		PFGamma_Isolation_DR04_ = pfGammaIso;
	else if (coneSize == 0.5)
		PFGamma_Isolation_DR05_ = pfGammaIso;
	else {
		std::cerr << "Lepton::setPFGammaIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
}

void Lepton::setPFChargedHadronIsolation(double chargedHadronIso, double coneSize) {
	if (coneSize == 0.3)
		PFChargedHadron_Isolation_DR03_ = chargedHadronIso;
	else if (coneSize == 0.4)
		PFChargedHadron_Isolation_DR04_ = chargedHadronIso;
	else if (coneSize == 0.5)
		PFChargedHadron_Isolation_DR05_ = chargedHadronIso;
	else {
		std::cerr << "Lepton::setPFChargedHadronIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
}

void Lepton::setPFNeutralHadronIsolation(double neutralHadronIso, double coneSize) {
	if (coneSize == 0.3)
		PFNeutralHadron_Isolation_DR03_ = neutralHadronIso;
	else if (coneSize == 0.4)
		PFNeutralHadron_Isolation_DR04_ = neutralHadronIso;
	else if (coneSize == 0.5)
		PFNeutralHadron_Isolation_DR05_ = neutralHadronIso;
	else {
		std::cerr << "Lepton::setPFNeutralHadronIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
}

void Lepton::setPFPUChargedHadronIsolation(double isolation, double coneSize) {
	if (coneSize == 0.3)
		PFPUChargedHadron_Isolation_DR03_ = isolation;
	else if (coneSize == 0.4)
		PFPUChargedHadron_Isolation_DR04_ = isolation;
	else if (coneSize == 0.5)
		PFPUChargedHadron_Isolation_DR05_ = isolation;
	else {
		std::cerr << "Lepton::setPFPUChargedHadronIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
}

void Lepton::setPFDeltaBeta_Isolation_DR04(double isolation) {
	PFDeltaBeta_Isolation_DR04_ = isolation;
}

void Lepton::setsumChargedHadronPt04(double isolation) {
	sumChargedHadronPt04_ = isolation;
}

void Lepton::setsumNeutralHadronPt04(double isolation) {
	sumNeutralHadronPt04_ = isolation;
}

void Lepton::setsumPhotonPt04(double isolation) {
	sumPhotonPt04_ = isolation;
}

void Lepton::setsumPUPt04(double isolation) {
	sumPUPt04_ = isolation;
}

void Lepton::setPFRelativeIsolationRho(double isolation) {
	PFRelativeIsolationRho_DR03_ = isolation;
}

double Lepton::PFGammaIsolation(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.3)
		isolation = PFGamma_Isolation_DR03_;
	else if (coneSize == 0.4)
		isolation = PFGamma_Isolation_DR04_;
	else if (coneSize == 0.5)
		isolation = PFGamma_Isolation_DR05_;
	else {
		std::cerr << "Lepton::PFGammaIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
	return isolation;
}

double Lepton::PFChargedHadronIsolation(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.3)
		isolation = PFChargedHadron_Isolation_DR03_;
	else if (coneSize == 0.4)
		isolation = PFChargedHadron_Isolation_DR04_;
	else if (coneSize == 0.5)
		isolation = PFChargedHadron_Isolation_DR05_;
	else {
		std::cerr << "Lepton::PFChargedHadronIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
	return isolation;
}

double Lepton::PFNeutralHadronIsolation(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.3)
		isolation = PFNeutralHadron_Isolation_DR03_;
	else if (coneSize == 0.4)
		isolation = PFNeutralHadron_Isolation_DR04_;
	else if (coneSize == 0.5)
		isolation = PFNeutralHadron_Isolation_DR05_;
	else {
		std::cerr << "Lepton::PFNeutralHadronIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
	return isolation;
}

double Lepton::PfPUChargedHadronIso(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.3)
		isolation = PFPUChargedHadron_Isolation_DR03_;
	else if (coneSize == 0.4)
		isolation = PFPUChargedHadron_Isolation_DR04_;
	else if (coneSize == 0.5)
		isolation = PFPUChargedHadron_Isolation_DR05_;
	else {
		std::cerr << "Lepton::PFNeutralHadronIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
	return isolation;
}

double Lepton::PFDeltaBeta_Isolation_DR04() const{
	return PFDeltaBeta_Isolation_DR04_;
}

double Lepton::sumChargedHadronPt04() const{
	return sumChargedHadronPt04_;
}

double Lepton::sumNeutralHadronPt04() const{
	return sumNeutralHadronPt04_;
}

double Lepton::sumPhotonPt04() const{
	return sumPhotonPt04_;
}

double Lepton::sumPUPt04() const{
	return sumPUPt04_;
}

double Lepton::relativeIsolation(double coneSize) const {
	return (ecalIsolation(coneSize) + hcalIsolation(coneSize) + trackerIsolation(coneSize)) / pt();
}



double Lepton::pfRelativeIsolation(double coneSize, bool deltaBetaCorrection) const {
	return pfIsolation(coneSize, deltaBetaCorrection) / pt();
}

double Lepton::pfIsolation(double coneSize, bool deltaBetaCorrection) const {
	double iso(0);
	if (deltaBetaCorrection){

		if(Globals::NTupleVersion >= 10){
			iso = sumChargedHadronPt04()
				+ max(0.0,sumNeutralHadronPt04()
						 + sumPhotonPt04()
								- 0.5 * sumPUPt04());
		}else{
			iso = PFChargedHadronIsolation(coneSize)
							+ max(0.0,
									PFNeutralHadronIsolation(coneSize) + PFGammaIsolation(coneSize)
											- 0.5 * PfPUChargedHadronIso(coneSize));
		}


	}else{
		if(Globals::NTupleVersion >= 10){
			iso = (sumChargedHadronPt04() + sumNeutralHadronPt04() +sumPhotonPt04());
		}else{
			iso = (PFChargedHadronIsolation(coneSize) + PFNeutralHadronIsolation(coneSize) + PFGammaIsolation(coneSize));
		}
	}
		return iso;
}

double Lepton::pfRelativeIsolationPUCorrected(double rho, double coneSize) const {
	double effectiveArea = 0;
	double eta = fabs(this->eta());
	if (eta < 1.)
		effectiveArea = 0.18;
	if (eta > 1. && eta < 1.479)
		effectiveArea = 0.19;
	if (eta > 1.479 && eta < 2.0)
		effectiveArea = 0.21;
	if (eta > 2.0 && eta < 2.2)
		effectiveArea = 0.38;
	if (eta > 2.2 && eta < 2.3)
		effectiveArea = 0.61;
	if (eta > 2.3 && eta < 2.4)
		effectiveArea = 0.73;
	if (eta > 2.4)
		effectiveArea = 0.78;

	double pfIso = PFChargedHadronIsolation(coneSize) + PFNeutralHadronIsolation(coneSize) + PFGammaIsolation(coneSize)
			- rho * effectiveArea;
	return pfIso / pt();
}

void Lepton::setZDistanceToPrimaryVertex(double dist) {
	zDistanceToPrimaryVertex_ = dist;
}

void Lepton::setDirectionalIsolation(double iso, double coneSize) {
	if (coneSize == 0.2)
		directionalIsolation_DR02_ = iso;
	else if (coneSize == 0.3)
		directionalIsolation_DR03_ = iso;
	else {
		std::cerr << "Lepton::setDirectionalIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
}

void Lepton::setDirectionalIsolationWithGaussianFallOff(double iso, double coneSize) {
	if (coneSize == 0.2)
		directionalIsolation_GaussianFallOff_DR02_ = iso;
	else if (coneSize == 0.3)
		directionalIsolation_GaussianFallOff_DR03_ = iso;
	else {
		std::cerr << "Lepton::setDirectionalIsolationWithGaussianFallOff: Unknown cone of deltaR = " << coneSize
				<< std::endl;
	}
}

void Lepton::setPFIsolationWithGaussianFallOff(double iso, double coneSize) {
	if (coneSize == 0.2)
		pfIsolation_GaussianFallOff_DR02_ = iso;
	else if (coneSize == 0.3)
		pfIsolation_GaussianFallOff_DR03_ = iso;
	else {
		std::cerr << "Lepton::setPFRelativeIsolationWithGaussianFallOff: Unknown cone of deltaR = " << coneSize
				<< std::endl;
	}
}

double Lepton::directionalIsolation(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.2)
		isolation = directionalIsolation_DR02_;
	else if (coneSize == 0.3)
		isolation = directionalIsolation_DR03_;
	else {
		std::cerr << "Lepton::directionalIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
	return isolation;
}

double Lepton::directionalIsolationWithGaussianFallOff(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.2)
		isolation = directionalIsolation_GaussianFallOff_DR02_;
	else if (coneSize == 0.3)
		isolation = directionalIsolation_GaussianFallOff_DR03_;
	else {
		std::cerr << "Lepton::directionalIsolationWithGaussianFallOff: Unknown cone of deltaR = " << coneSize
				<< std::endl;
	}
	return isolation;
}

double Lepton::pfIsolationWithGaussianFallOff(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.2)
		isolation = pfIsolation_GaussianFallOff_DR02_;
	else if (coneSize == 0.3)
		isolation = pfIsolation_GaussianFallOff_DR03_;
	else {
		std::cerr << "Lepton::pfIsolationWithGaussianFallOff: Unknown cone of deltaR = " << coneSize << std::endl;
	}
	return isolation;
}

double Lepton::ZDistanceToPrimaryVertex() const {
	return zDistanceToPrimaryVertex_;
}

Lepton::~Lepton() {
}

string Lepton::toString() const {
	stringstream out;
	out << Particle::toString();
	out << "Lepton information" << "\n";
	out << setw(30) << "rel. iso (DR = 0.3)" << setw(30) << "ECAL iso (DR = 0.3)" << setw(30) << "HCAL iso (DR = 0.3)"
			<< setw(30) << "Track. iso (DR = 0.3)" << "\n";
	out << setw(30) << relativeIsolation(0.3) << setw(30) << ecalIsolation(0.3) << setw(30) << hcalIsolation(0.3)
			<< setw(30) << trackerIsolation(0.3) << "\n";
	out << setw(30) << "rel. iso (DR = 0.4)" << setw(30) << "ECAL iso (DR = 0.4)" << setw(30) << "HCAL iso (DR = 0.4)"
			<< setw(30) << "Track. iso (DR = 0.4)" << "\n";
	out << setw(30) << relativeIsolation(0.4) << setw(30) << ecalIsolation(0.4) << setw(30) << hcalIsolation(0.4)
			<< setw(30) << trackerIsolation(0.4) << "\n";
	out << setw(30) << "rel. iso (DR = 0.5)" << setw(30) << "ECAL iso (DR = 0.5)" << setw(30) << "HCAL iso (DR = 0.5)"
			<< setw(30) << "Track. iso (DR = 0.5)" << "\n";
	out << setw(30) << relativeIsolation(0.5) << setw(30) << ecalIsolation(0.5) << setw(30) << hcalIsolation(0.5)
			<< setw(30) << trackerIsolation(0.5) << "\n";
	out << setw(30) << "PF rel. iso (DR = 0.3)" << setw(30) << "PF EGamma iso (DR = 0.3)" << setw(30)
			<< "PF neutral hadron iso (DR = 0.3)" << setw(30) << "PF charged hadron. iso (DR = 0.3)" << "\n";
	out << setw(30) << pfRelativeIsolation(0.3) << setw(30) << PFGammaIsolation(0.3) << setw(30)
			<< PFNeutralHadronIsolation(0.3) << setw(30) << PFChargedHadronIsolation(0.3) << "\n";
	out << setw(30) << "PF rel. iso (DR = 0.4)" << setw(30) << "PF EGamma iso (DR = 0.4)" << setw(30)
			<< "PF neutral hadron iso (DR = 0.4)" << setw(30) << "PF charged hadron. iso (DR = 0.4)" << "\n";
	out << setw(30) << pfRelativeIsolation(0.4) << setw(30) << PFGammaIsolation(0.4) << setw(30)
			<< PFNeutralHadronIsolation(0.4) << setw(30) << PFChargedHadronIsolation(0.4) << "\n";
	out << setw(30) << "PF rel. iso (DR = 0.5)" << setw(30) << "PF EGamma iso (DR = 0.5)" << setw(30)
			<< "PF neutral hadron iso (DR = 0.5)" << setw(30) << "PF charged hadron. iso (DR = 0.5)" << "\n";
	out << setw(30) << pfRelativeIsolation(0.5) << setw(30) << PFGammaIsolation(0.5) << setw(30)
			<< PFNeutralHadronIsolation(0.5) << setw(30) << PFChargedHadronIsolation(0.5) << "\n";
	return out.str();
}

} /* namespace BAT */
