/*
 * Lepton.cpp
 *
 *  Created on: 30 Oct 2011
 *      Author: kreczko
 */

#include "../../interface/RecoObjects/Lepton.h"

namespace BAT {
const double initialBigValue = 123456789;

Lepton::Lepton() : //
		Particle(), //
		ecal_Isolation(initialBigValue), //
		hcal_Isolation(initialBigValue), //
		tracker_Isolation(initialBigValue), //
		PFGamma_Isolation(initialBigValue), //
		PFChargedHadron_Isolation(initialBigValue), //
		PFNeutralHadron_Isolation(initialBigValue), //
		zDistanceToPrimaryVertex(initialBigValue), //
		directionalIsolation_(initialBigValue) {
}

Lepton::Lepton(double energy, double px, double py, double pz) : //
		Particle(energy, px, py, pz), //
		ecal_Isolation(initialBigValue), //
		hcal_Isolation(initialBigValue), //
		tracker_Isolation(initialBigValue), //
		PFGamma_Isolation(initialBigValue), //
		PFChargedHadron_Isolation(initialBigValue), //
		PFNeutralHadron_Isolation(initialBigValue), //
		zDistanceToPrimaryVertex(initialBigValue), //
		directionalIsolation_(initialBigValue) {

}

void Lepton::setEcalIsolation(double isolation) {
	ecal_Isolation = isolation;
}

void Lepton::setHcalIsolation(double isolation) {
	hcal_Isolation = isolation;
}

void Lepton::setTrackerIsolation(double isolation) {
	tracker_Isolation = isolation;
}

double Lepton::trackerIsolation() const {
	return tracker_Isolation;
}

double Lepton::hcalIsolation() const {
	return hcal_Isolation;
}

double Lepton::ecalIsolation() const {
	return ecal_Isolation;
}

void Lepton::setPFGammaIsolation(double pfGammaIso) {
	PFGamma_Isolation = pfGammaIso;
}

void Lepton::setPFChargedHadronIsolation(double chargedHadronIso) {
	PFChargedHadron_Isolation = chargedHadronIso;
}

void Lepton::setPFNeutralHadronIsolation(double neutralHadronIso) {
	PFNeutralHadron_Isolation = neutralHadronIso;
}

double Lepton::PFGammaIsolation() const {
	return PFGamma_Isolation;
}

double Lepton::PFChargedHadronIsolation() const {
	return PFChargedHadron_Isolation;
}

double Lepton::PFNeutralHadronIsolation() const {
	return PFNeutralHadron_Isolation;
}

double Lepton::relativeIsolation() const {
	return (ecal_Isolation + hcal_Isolation + tracker_Isolation) / pt();
}

double Lepton::pfIsolation() const {
	return (PFChargedHadron_Isolation + PFNeutralHadron_Isolation + PFGamma_Isolation) / pt();
}

void Lepton::setZDistanceToPrimaryVertex(double dist) {
	zDistanceToPrimaryVertex = dist;
}

void Lepton::setDirectionalIsolation(double iso) {
	directionalIsolation_ = iso;
}

double Lepton::directionalIsolation() const {
	return directionalIsolation_;
}

Lepton::~Lepton() {
}

} /* namespace BAT */
