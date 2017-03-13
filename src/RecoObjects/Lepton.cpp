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
		PFRelIso03_(initialBigValue), //
		PFRelIso04_(initialBigValue), //
		PFRelIso03DeltaBeta_(initialBigValue), //
		PFRelIso04DeltaBeta_(initialBigValue), //
		PFRelIsoWithEA_(initialBigValue), //
		sumChargedHadronPt03_(initialBigValue), //
		sumNeutralHadronPt03_(initialBigValue), //
		sumPhotonPt03_(initialBigValue), //
		sumPUPt03_(initialBigValue), //
		sumChargedHadronPt04_(initialBigValue), //
		sumNeutralHadronPt04_(initialBigValue), //
		sumPhotonPt04_(initialBigValue), //
		sumPUPt04_(initialBigValue), //
		zDistanceToPrimaryVertex_(initialBigValue) {
}

Lepton::Lepton(double energy, double px, double py, double pz) : //
		Particle(energy, px, py, pz), //
		ecal_Isolation_DR03_(initialBigValue), //
		hcal_Isolation_DR03_(initialBigValue), //
		tracker_Isolation_DR03_(initialBigValue), //
		PFRelIso03_(initialBigValue), //
		PFRelIso04_(initialBigValue), //
		PFRelIso03DeltaBeta_(initialBigValue), //
		PFRelIso04DeltaBeta_(initialBigValue), //
		PFRelIsoWithEA_(initialBigValue), //
		sumChargedHadronPt03_(initialBigValue), //
		sumNeutralHadronPt03_(initialBigValue), //
		sumPhotonPt03_(initialBigValue), //
		sumPUPt03_(initialBigValue), //
		sumChargedHadronPt04_(initialBigValue), //
		sumNeutralHadronPt04_(initialBigValue), //
		sumPhotonPt04_(initialBigValue), //
		sumPUPt04_(initialBigValue), //
		zDistanceToPrimaryVertex_(initialBigValue) {

}

void Lepton::setPFRelIsoWithEA( double isolation ) {
	PFRelIsoWithEA_ = isolation;
}

void Lepton::setEcalIsolation(double isolation, double coneSize) {
	if (coneSize == 0.3)
		ecal_Isolation_DR03_ = isolation;
	else {
		std::cerr << "Lepton::setEcalIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
}

void Lepton::setHcalIsolation(double isolation, double coneSize) {
	if (coneSize == 0.3)
		hcal_Isolation_DR03_ = isolation;
	else {
		std::cerr << "Lepton::setHcalIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
}

void Lepton::setTrackerIsolation(double isolation, double coneSize) {
	if (coneSize == 0.3)
		tracker_Isolation_DR03_ = isolation;
	else {
		std::cerr << "Lepton::setTrackerIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
}

double Lepton::trackerIsolation(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.3)
		isolation = tracker_Isolation_DR03_;
	else {
		std::cerr << "Lepton::trackerIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
	return isolation;
}

double Lepton::hcalIsolation(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.3)
		isolation = hcal_Isolation_DR03_;
	else {
		std::cerr << "Lepton::hcalIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
	return isolation;
}

double Lepton::ecalIsolation(double coneSize) const {
	double isolation(initialBigValue);
	if (coneSize == 0.3)
		isolation = ecal_Isolation_DR03_;
	else {
		std::cerr << "Lepton::ecalIsolation: Unknown cone of deltaR = " << coneSize << std::endl;
	}
	return isolation;
}


void Lepton::setPFRelIso03( double isolation ) {
	PFRelIso03_ = isolation;
}

void Lepton::setPFRelIso04( double isolation ) {
	PFRelIso04_ = isolation;
}

void Lepton::setPFRelIso03DeltaBeta( double isolation ) {
	PFRelIso03DeltaBeta_ = isolation;
}

void Lepton::setPFRelIso04DeltaBeta( double isolation ) {
	PFRelIso04DeltaBeta_ = isolation;
}

void Lepton::setHltECALIso( double isolation ) {
	hltECALIso_ = isolation;
}

void Lepton::setHltHCALIso( double isolation ) {
	hltHCALIso_ = isolation;
}

void Lepton::setHltTrackerIso( double isolation ) {
	hltTrackerIso_ = isolation;
}

void Lepton::setsumChargedHadronPt03(double isolation) {
	sumChargedHadronPt03_ = isolation;
}

void Lepton::setsumNeutralHadronPt03(double isolation) {
	sumNeutralHadronPt03_ = isolation;
}

void Lepton::setsumPhotonPt03(double isolation) {
	sumPhotonPt03_ = isolation;
}

void Lepton::setsumPUPt03(double isolation) {
	sumPUPt03_ = isolation;
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

double Lepton::PFRelIsoWithEA() const {
	return PFRelIsoWithEA_;
}

double Lepton::PFRelIso03() const {
	return PFRelIso03_;
}

double Lepton::PFRelIso04() const {
	return PFRelIso04_;
}

double Lepton::PFRelIso03DeltaBeta() const {
	return PFRelIso03DeltaBeta_;
}

double Lepton::PFRelIso04DeltaBeta() const {
	return PFRelIso04DeltaBeta_;
}

double Lepton::hltECALIso() const {
	return hltECALIso_;
}

double Lepton::hltHCALIso() const {
	return hltHCALIso_;
}

double Lepton::hltTrackerIso() const {
	return hltTrackerIso_;
}

double Lepton::sumChargedHadronPt03() const{
	return sumChargedHadronPt03_;
}

double Lepton::sumNeutralHadronPt03() const{
	return sumNeutralHadronPt03_;
}

double Lepton::sumPhotonPt03() const{
	return sumPhotonPt03_;
}

double Lepton::sumPUPt03() const{
	return sumPUPt03_;
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

void Lepton::setZDistanceToPrimaryVertex(double dist) {
	zDistanceToPrimaryVertex_ = dist;
}

double Lepton::ZDistanceToPrimaryVertex() const {
	return zDistanceToPrimaryVertex_;
}

Lepton::~Lepton() {
}

} /* namespace BAT */
