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

bool Muon::isGood(short leptonID) const {
	bool passesPt = pt() > Globals::minMuonPt;
	bool passesEta = fabs(eta()) < Globals::maxAbsoluteMuonEta;
	bool passesMuonID = is_GlobalMuon && is_TrackerMuon;
	bool passesNormChi2 = normalisedChi2 < 10;

	bool passesD0 = d0() < 0.02;
	bool passesDistanceToVertex = fabs(zDistanceToPrimaryVertex_) < 1;

	bool passesNTrackerHits = numberOfValidHits > 10;
	bool passesNMuonHit = numberOfValidMuonHits > 0;
	bool passesNPixelLayersWithMeasurement = pixelLayersWithMeasurement >= 1;
	//are these two identical?
	bool passesNumberOfMatchedStations = numberOfMatchedStations > 1;
	bool passesNumberOfMatches = numberOfMatches > 1;

	return passesPt && passesEta && passesMuonID && passesNormChi2 && passesD0 && passesDistanceToVertex
			&& passesNTrackerHits && passesNMuonHit && passesNMuonHit && passesNPixelLayersWithMeasurement
			&& passesNumberOfMatchedStations && passesNumberOfMatches;
}

bool Muon::isLoose() const {
	bool passesPt = pt() > 10;
	bool passesEta = fabs(eta()) < 2.5;
	return passesPt && passesEta && is_GlobalMuon;
}

bool Muon::isPFLepton() const {
	return usedAlgorithm == MuonAlgorithm::ParticleFlow;
}

void Muon::setUsedAlgorithm(MuonAlgorithm::value algorithm){
	usedAlgorithm = algorithm;
}
}
