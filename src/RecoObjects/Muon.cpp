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
	double triggerEfficiencyRelativeError(0.05);
	double muonPt = pt();
	double muonEta = fabs(eta());

	if ( muonPt >=20 && muonPt < 30 ) {
		if ( muonEta >= 0 && muonEta < 1.2 ) {
			triggerEfficiency = 0.895;
		}
		else if ( muonEta >= 1.2 && muonEta < 2.4 ) {
			triggerEfficiency = 0.817;
		}
	}
	else if ( muonPt >=30 && muonPt < 40 ) {
		if ( muonEta >= 0 && muonEta < 1.2 ) {
			triggerEfficiency = 0.926;
		}
		else if ( muonEta >= 1.2 && muonEta < 2.4 ) {
			triggerEfficiency = 0.866;
		}
	}
	else if ( muonPt >=40 && muonPt < 50 ) {
		if ( muonEta >= 0 && muonEta < 1.2 ) {
			triggerEfficiency = 0.933;
		}
		else if ( muonEta >= 1.2 && muonEta < 2.4 ) {
			triggerEfficiency = 0.894;
		}
	}
	else if ( muonPt >=50  ) {
		if ( muonEta >= 0 && muonEta < 1.2 ) {
			triggerEfficiency = 0.947;
		}
		else if ( muonEta >= 1.2 && muonEta < 2.4 ) {
			triggerEfficiency = 0.893;
		}
	}
	// boost::shared_ptr<TH1F> electronTriggerScaleFactorsHistogram(Globals::electronTriggerScaleFactorsHistogram);
	// double electronPt = pt();
	// double maxPt = electronTriggerScaleFactorsHistogram->GetXaxis()->GetXmax();
	// // cout << "Electron pt : " << electronPt << endl;
	// // cout << "Max pt in histogram" << endl;
	// // cout << maxPt << endl;
	// unsigned int bin = 0;
	// if ( electronPt <= maxPt ) {
	// 	bin = electronTriggerScaleFactorsHistogram->FindBin( electronPt );
	// }
	// else {
	// 	bin = electronTriggerScaleFactorsHistogram->GetNbinsX();
	// }

	// triggerEfficiency = electronTriggerScaleFactorsHistogram->GetBinContent( bin );


	double idIsoSF(1.);
	boost::shared_ptr<TH2F> muonIDIsoScaleFactorsHistogram(Globals::muonIdIsoScaleFactorsHistogram);
	double bin = muonIDIsoScaleFactorsHistogram->FindBin( muonEta, muonPt );
	idIsoSF = muonIDIsoScaleFactorsHistogram->GetBinContent( bin );
	double idIsoSFError = muonIDIsoScaleFactorsHistogram->GetBinError( bin );

	if (muon_scale_factor_systematic == -1 ) {
		idIsoSF -= idIsoSFError;
		triggerEfficiency = triggerEfficiency * ( 1 - triggerEfficiencyRelativeError );
	}
	else if ( muon_scale_factor_systematic == 1 ) {
		idIsoSF += idIsoSFError;
		triggerEfficiency = triggerEfficiency * ( 1 + triggerEfficiencyRelativeError );
	}

	return idIsoSF * triggerEfficiency;
}
}
