/*
 * Muon.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/Muon.h"

namespace BAT {

Muon::Muon() :
	Particle(), is_Global(false), ecal_Isolation(0.), hcal_Isolation(0), tracker_Isolation(0) {

}

Muon::Muon(float energy, float px, float py, float pz) :
	Particle(energy, px, py, pz), is_Global(false), ecal_Isolation(0.), hcal_Isolation(0), tracker_Isolation(0) {

}

Muon::~Muon() {
}

bool Muon::isGlobal() const {
	return is_Global;
}

void Muon::makeGlobal(bool global) {
	is_Global = global;
}

float Muon::ecalIsolation() const {
	return ecal_Isolation;
}

void Muon::setEcalIsolation(float isolation) {
	ecal_Isolation = isolation;
}

float Muon::hcalIsolation() const {
	return hcal_Isolation;
}

void Muon::setHcalIsolation(float isolation) {
	hcal_Isolation = isolation;
}

float Muon::trackerIsolation() const {
	return tracker_Isolation;
}

void Muon::setTrackerIsolation(float isolation) {
	tracker_Isolation = isolation;
}

float Muon::relativeIsolation() const {
	return (ecal_Isolation + hcal_Isolation + tracker_Isolation) / pt();
}

bool Muon::isIsolated() const{
	return relativeIsolation() < 0.2;
}

bool Muon::isGood() const{
	bool passesPt = pt() > 10;
	bool passesEta = fabs(eta()) < 2.5;
	return passesPt && passesEta && is_Global;
}
}
