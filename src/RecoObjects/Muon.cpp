/*
 * Muon.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/Muon.h"
#include "../../interface/GlobalVariables.h"


#include <iostream>

using namespace std;

namespace BAT {
const double initialBigValue = 123456789;

Muon::Muon() :
		Lepton(), //
		usedAlgorithm_(MuonAlgorithm::Default), //
		isGlobalMuon_(false), //
		isTrackerMuon_(false), //
		isPFMuon_(false), //
		normalisedChi2_(initialBigValue), //
		numberOfValidMuonHits_(-1), //
		numberOfValidPixelHits_(-1), //
		numberOfValidHits_(-1), //
		pixelLayersWithMeasurement_(-1), //
		trackerLayersWithMeasurement_(-1), //
		numberOfMatches_(-1), //
		numberOfMatchedStations_(-1),
		relTrkIso_(-1) {

}

Muon::Muon(double energy, double px, double py, double pz) :
		Lepton(energy, px, py, pz), //
		usedAlgorithm_(MuonAlgorithm::Default), //
		isGlobalMuon_(false), //
		isTrackerMuon_(false), //
		isPFMuon_(false), //
		normalisedChi2_(initialBigValue), //
		numberOfValidMuonHits_(-1), //
		numberOfValidPixelHits_(-1), //
		numberOfValidHits_(-1), //
		pixelLayersWithMeasurement_(-1), //
		trackerLayersWithMeasurement_(-1), //
		numberOfMatches_(-1), //
		numberOfMatchedStations_(-1),
		relTrkIso_(-1) {

}

Muon::~Muon() {
}

bool Muon::isGlobal() const {
	return isGlobalMuon_;
}

bool Muon::isTracker() const {
	return isTrackerMuon_;
}

bool Muon::isPFMuon() const {
	return isPFMuon_;
}

void Muon::makeGlobal(bool global) {
	isGlobalMuon_ = global;
}

void Muon::setTrackerMuon(bool isTrackerMuon) {
	isTrackerMuon_ = isTrackerMuon;
}

void Muon::setPFMuon(bool pf) {
	isPFMuon_ = pf;
}

void Muon::setNormalisedChi2(double normChi2) {
	normalisedChi2_ = normChi2;
}

void Muon::setNumberOfValidHits(int nValidHits) {
	numberOfValidHits_ = nValidHits;
}

void Muon::setNumberOfValidMuonHits(int nValidHits) {
	numberOfValidMuonHits_ = nValidHits;
}

void Muon::setNumberOfValidPixelHits(int nValidHits) {
	numberOfValidPixelHits_ = nValidHits;
}

void Muon::setPixelLayersWithMeasurement(int pixelLayers) {
	pixelLayersWithMeasurement_ = pixelLayers;
}

void Muon::setNumberOfMatchedStations(int nMatchedStations) {
	numberOfMatchedStations_ = nMatchedStations;
}

void Muon::setNumberOfMatches(int nMatches) {
	numberOfMatches_ = nMatches;
}

bool Muon::isPFLepton() const {
	return usedAlgorithm_ == MuonAlgorithm::ParticleFlow;
}

void Muon::setUsedAlgorithm(MuonAlgorithm::value algorithm) {
	usedAlgorithm_ = algorithm;
}

void Muon::setTrackerLayersWithMeasurement(int layers) {
	trackerLayersWithMeasurement_ = layers;
}

int Muon::trackerLayersWithMeasurement() const {
	return trackerLayersWithMeasurement_;
}

int Muon::numberOfMatchedStations() const {
	return numberOfMatchedStations_;
}

int Muon::numberOfMatches() const {
	return numberOfMatches_;
}

int Muon::numberOfValidHits() const {
	return numberOfValidHits_;
}

int Muon::numberOfValidMuonHits() const {
	return numberOfValidMuonHits_;
}

int Muon::numberOfValidPixelHits() const {
	return numberOfValidPixelHits_;
}

int Muon::pixelLayersWithMeasurement() const {
	return pixelLayersWithMeasurement_;
}

double Muon::normChi2() const {
	return normalisedChi2_;
}

void Muon::setRelTrkIsolation( double relIso ) {
	relTrkIso_ = relIso;
}

double Muon::relTrkIso() const {
	return relTrkIso_;
}



double Muon::getEfficiencyCorrection( int muon_scale_factor_systematic ) const {
	
	double triggerEfficiency(1.);
	double triggerEfficiencyError(0.);
	boost::shared_ptr<TH2F> muonTriggerScaleFactorsHistogram(Globals::muonTriggerScaleFactorsHistogram);
	double muonPt = pt();
	double muonAbsEta = fabs(eta());
	double maxPt = muonTriggerScaleFactorsHistogram->GetYaxis()->GetXmax();

	unsigned int bin = 0;
	if ( muonPt <= maxPt ) {
		bin = muonTriggerScaleFactorsHistogram->FindBin( muonAbsEta, muonPt );
	}
	else {
		bin = muonTriggerScaleFactorsHistogram->FindBin( muonAbsEta, maxPt );
	}
	triggerEfficiency = muonTriggerScaleFactorsHistogram->GetBinContent( bin );
	triggerEfficiencyError = muonTriggerScaleFactorsHistogram->GetBinError( bin );

	double idIsoSF(1.);
	double idIsoSFError(0.);
	boost::shared_ptr<TH2F> muonIDIsoScaleFactorsHistogram(Globals::muonIdIsoScaleFactorsHistogram);
	maxPt = muonIDIsoScaleFactorsHistogram->GetYaxis()->GetXmax();
	bin = 0;
	if ( muonPt <= maxPt ) {
		bin = muonIDIsoScaleFactorsHistogram->FindBin( muonAbsEta, muonPt );
	}
	else {
		bin = muonIDIsoScaleFactorsHistogram->FindBin( muonAbsEta, maxPt );
	}
	idIsoSF = muonIDIsoScaleFactorsHistogram->GetBinContent( bin );
	idIsoSFError = muonIDIsoScaleFactorsHistogram->GetBinError( bin );

	if (muon_scale_factor_systematic == -1 ) {
		idIsoSF -= idIsoSFError;
		triggerEfficiency -= triggerEfficiencyError;
	}
	else if ( muon_scale_factor_systematic == 1 ) {
		idIsoSF += idIsoSFError;
		triggerEfficiency += triggerEfficiencyError;
	}

	return idIsoSF * triggerEfficiency;
}
}
