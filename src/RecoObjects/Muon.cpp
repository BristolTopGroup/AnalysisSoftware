/*
 * Muon.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/Muon.h"
#include "../../interface/GlobalVariables.h"

namespace BAT {
const double initialBigValue = 123456789;

Muon::Muon() :
	Lepton(),
	usedAlgorithm(MuonAlgorithm::Default),
	is_GlobalMuon(false),
	is_TrackerMuon(false),
	normalisedChi2(initialBigValue),
	numberOfValidMuonHits(0),
	numberOfValidHits(0),
	pixelLayersWithMeasurement(0),
	numberOfMatches(0),
	numberOfMatchedStations(0) {

}

Muon::Muon(double energy, double px, double py, double pz) :
		Lepton(energy, px, py, pz),
		is_GlobalMuon(false),
		is_TrackerMuon(false),
		normalisedChi2(initialBigValue),
		numberOfValidMuonHits(0),
		numberOfValidHits(0),
		pixelLayersWithMeasurement(0),
		numberOfMatches(0),
		numberOfMatchedStations(0) {

}

Muon::~Muon() {
}

bool Muon::isGlobal() const {
	return is_GlobalMuon;
}

bool Muon::isTracker() const {
	return is_TrackerMuon;
}

void Muon::makeGlobal(bool global) {
	is_GlobalMuon = global;
}

void Muon::setTrackerMuon(bool isTrackerMuon) {
	is_TrackerMuon = isTrackerMuon;
}

void Muon::setNormalisedChi2(double normChi2){
	normalisedChi2 = normChi2;
}

void Muon::setNumberOfValidHits(int nValidHits){
	numberOfValidHits = nValidHits;
}

void Muon::setNumberOfValidMuonHits(int nValidHits){
	numberOfValidMuonHits = nValidHits;
}

void Muon::setPixelLayersWithMeasurement(int pixelLayers){
	pixelLayersWithMeasurement = pixelLayers;
}

void Muon::setNumberOfMatchedStations(int nMatchedStations){
	numberOfMatchedStations = nMatchedStations;
}

void Muon::setNumberOfMatches(int nMatches){
	numberOfMatches = nMatches;
}

bool Muon::isPFLepton() const {
	return usedAlgorithm == MuonAlgorithm::ParticleFlow;
}

void Muon::setUsedAlgorithm(MuonAlgorithm::value algorithm){
	usedAlgorithm = algorithm;
}
}
