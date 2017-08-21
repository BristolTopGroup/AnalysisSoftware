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

	double dummy = 1;
	return getEfficiencyCorrection( muon_scale_factor_systematic, dummy );
}

double Muon::getEfficiencyCorrection( int muon_scale_factor_systematic, double& correctionFromEtaOnly ) const {
	double muonPt = pt();
	double muonEta = eta();
	double muonAbsEta = fabs(muonEta);
	correctionFromEtaOnly = 1;

	//
	// Trigger SF
	//	
	double triggerEfficiency(1.);
	double triggerEfficiencyError(0.);
	boost::shared_ptr<TH2F> muonTriggerScaleFactorsHistogram(Globals::muonTriggerScaleFactorsHistogram);

	double triggerEfficiency_etaBins(1.);
	double triggerEfficiencyError_etaBins(0.);
	boost::shared_ptr<TH1F> muonTriggerScaleFactorsHistogram_etaBins(Globals::muonTriggerScaleFactorsHistogram_etaBins);

	double maxPt = muonTriggerScaleFactorsHistogram->GetYaxis()->GetXmax();
	unsigned int bin = 0;
	if ( muonPt <= maxPt ) {
		bin = muonTriggerScaleFactorsHistogram->FindBin( muonAbsEta, muonPt );
	}
	else {
		double lastPtBinCentre = muonTriggerScaleFactorsHistogram->GetYaxis()->GetBinCenter( muonTriggerScaleFactorsHistogram->GetNbinsY() );
		bin = muonTriggerScaleFactorsHistogram->FindBin( muonAbsEta, lastPtBinCentre );
	}
	triggerEfficiency = muonTriggerScaleFactorsHistogram->GetBinContent( bin );
	triggerEfficiencyError = muonTriggerScaleFactorsHistogram->GetBinError( bin );

	bin = muonTriggerScaleFactorsHistogram_etaBins->FindBin(muonEta);
	triggerEfficiency_etaBins = muonTriggerScaleFactorsHistogram_etaBins->GetBinContent( bin );
	triggerEfficiencyError_etaBins = muonTriggerScaleFactorsHistogram_etaBins->GetBinError( bin );

	//
	// ID SF
	//
	double idSF(1.);
	double idSFError(0.);
	boost::shared_ptr<TH2F> muonIdScaleFactorsHistogram(Globals::muonIdScaleFactorsHistogram);

	double idSF_etaBins(1.);
	double idSFError_etaBins(0.);
	boost::shared_ptr<TH1F> muonIdScaleFactorsHistogram_etaBins(Globals::muonIdScaleFactorsHistogram_etaBins);

	maxPt = muonIdScaleFactorsHistogram->GetYaxis()->GetXmax();
	bin = 0;
	if ( muonPt <= maxPt ) {
		bin = muonIdScaleFactorsHistogram->FindBin( muonAbsEta, muonPt );
	}
	else {
		double lastPtBinCentre = muonIdScaleFactorsHistogram->GetYaxis()->GetBinCenter( muonIdScaleFactorsHistogram->GetNbinsY() );
		bin = muonIdScaleFactorsHistogram->FindBin( muonAbsEta, lastPtBinCentre );
	}
	idSF = muonIdScaleFactorsHistogram->GetBinContent( bin );
	idSFError = muonIdScaleFactorsHistogram->GetBinError( bin );

	bin = muonIdScaleFactorsHistogram_etaBins->FindBin(muonEta);
	idSF_etaBins = muonIdScaleFactorsHistogram_etaBins->GetBinContent( bin );
	idSFError_etaBins = muonIdScaleFactorsHistogram_etaBins->GetBinError( bin );


	//
	// ISO scale factor
	//
	double isoSF(1.);
	double isoSFError(0.);
	boost::shared_ptr<TH2F> muonIsoScaleFactorsHistogram(Globals::muonIsoScaleFactorsHistogram);

	double isoSF_etaBins(1.);
	double isoSFError_etaBins(0.);
	boost::shared_ptr<TH1F> muonIsoScaleFactorsHistogram_etaBins(Globals::muonIsoScaleFactorsHistogram_etaBins);

	maxPt = muonIsoScaleFactorsHistogram->GetYaxis()->GetXmax();
	bin = 0;
	if ( muonPt <= maxPt ) {
		bin = muonIsoScaleFactorsHistogram->FindBin( muonAbsEta, muonPt );
	}
	else {
		double lastPtBinCentre = muonIsoScaleFactorsHistogram->GetYaxis()->GetBinCenter( muonIsoScaleFactorsHistogram->GetNbinsY() );
		bin = muonIsoScaleFactorsHistogram->FindBin( muonAbsEta, lastPtBinCentre );
	}
	isoSF = muonIsoScaleFactorsHistogram->GetBinContent( bin );
	isoSFError = muonIsoScaleFactorsHistogram->GetBinError( bin );

	bin = muonIsoScaleFactorsHistogram_etaBins->FindBin(muonEta);
	isoSF_etaBins = muonIsoScaleFactorsHistogram_etaBins->GetBinContent( bin );
	isoSFError_etaBins = muonIsoScaleFactorsHistogram_etaBins->GetBinError( bin );

	// Additional systematic uncertainties
	// Based on those from 2015 data (no new reccomendations)
	// https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffsRun2#Results_for_CMSSW_7_6_X_dataset
	idSFError += 0.01;
	idSFError_etaBins += 0.01;

	isoSFError += 0.005;
	isoSFError_etaBins += 0.005;

	triggerEfficiencyError += 0.005;
	triggerEfficiencyError_etaBins += 0.005;

	//
	// Tracking SF	
	//
	double trackingSF(1.);
	double trackingSFError(0.);
	boost::shared_ptr<TH1F> muonTrackingScaleFactorsHistogram(Globals::muonTrackingScaleFactorsHistogram);
	bin = muonTrackingScaleFactorsHistogram->FindBin( muonEta );
	trackingSF = muonTrackingScaleFactorsHistogram->GetBinContent( bin );
	trackingSFError = muonTrackingScaleFactorsHistogram->GetBinError( bin );

	if (muon_scale_factor_systematic == -1 ) {
		idSF -= idSFError;
		isoSF -= isoSFError;
		triggerEfficiency -= triggerEfficiencyError;
		trackingSF -= trackingSFError;

		idSF_etaBins -= idSFError_etaBins;
		isoSF_etaBins -= isoSFError_etaBins;
		triggerEfficiency_etaBins -= triggerEfficiencyError_etaBins;
	}
	else if (muon_scale_factor_systematic == +1 ) {
		idSF += idSFError;
		isoSF += isoSFError;
		triggerEfficiency += triggerEfficiencyError;
		trackingSF += trackingSFError;

		idSF_etaBins += idSFError_etaBins;
		isoSF_etaBins += isoSFError_etaBins;
		triggerEfficiency_etaBins += triggerEfficiencyError_etaBins;
	}

	correctionFromEtaOnly = idSF_etaBins * isoSF_etaBins * triggerEfficiency_etaBins * trackingSF;

	if ( correctionFromEtaOnly < 0.5 ) {
		std::cout << "Muon pt, eta, syst : " << muonEta << " " << muonPt << " " << muon_scale_factor_systematic << std::endl;
		std::cout << "Original SFs : " << idSF << " " << isoSF << " " << triggerEfficiency << " " << trackingSF << " " << idSF * isoSF * triggerEfficiency * trackingSF << std::endl;
		std::cout << "---> New SFs : " << idSF_etaBins << " " << isoSF_etaBins << " " << triggerEfficiency_etaBins << " " << trackingSF << " " << correctionFromEtaOnly << std::endl;		
	}

	return idSF * isoSF * triggerEfficiency * trackingSF;

}
}
