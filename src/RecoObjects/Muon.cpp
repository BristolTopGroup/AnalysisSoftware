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
		numberOfMatchedStations_(-1) {

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
		numberOfMatchedStations_(-1) {

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

double Muon::getEfficiencyCorrection(bool qcd, int muon_scale_factor_systematic) const {
	double correction(1.);
	double muEta(eta());

	if(Globals::energyInTeV == 7){
	if (muEta < -1.5)
		correction = 1.003;
	else if (muEta >= -1.5 && muEta < -1.2)
		correction = 0.980;
	else if (muEta >= -1.2 && muEta < -0.9)
		correction = 0.941;
	else if (muEta >= -0.9 && muEta < 0)
		correction = 0.974;
	else if (muEta >= 0 && muEta < 0.9)
		correction = 0.977;
	else if (muEta >= 0.9 && muEta < 1.2)
		correction = 0.939;
	else if (muEta >= 1.2 && muEta < 1.5)
		correction = 0.967;
	else if (muEta >= 1.5)
		correction = 1.023;
	//8TeV scale factors from https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffs#22Jan2013_ReReco_of_2012_data_re
	//Specifically: ID & Iso: https://indico.cern.ch/event/257630/material/slides/2?contribId=1, Slide 12, "Tight Id" & Slide 26 "pfCombIso04dBeta (< 0.12) for	Tight Id"
	//Specifically: Trigger: https://indico.cern.ch/event/257000/material/slides/0?contribId=2, Slide 13, "IsoMu24 TightID PFIsodB"
	}else if(qcd == false){ //corrections for ID, Iso and Trigger respectively
		if(abs(muEta)<0.9) {
			switch (muon_scale_factor_systematic) {
				case -1:
					correction = (0.9925-0.0002)*(0.9959-0.00002)*(0.9837-0.00021);
					break;
				case 1:
					correction = (0.9925+0.0002)*(0.9959+0.00002)*(0.9837+0.00021);
					break;
				default:
					correction = 0.9925*0.9959*0.9837;
			}
		}
		else if(abs(muEta)>=0.9 && abs(muEta)<1.2) {
			switch (muon_scale_factor_systematic) {
				case -1:
					correction = (0.9928-0.0003)*(0.9878-0.0003)*(0.9656-0.00066);
					break;
				case 1:
					correction = (0.9928+0.0003)*(0.9878-0.0003)*(0.9656+0.00066);
					break;
				default:
					correction = 0.9928*0.9878*0.9656;
			}
		}
		else if(abs(muEta)>=1.2 && abs(muEta)<2.1) {
			switch (muon_scale_factor_systematic) {
				case -1:
					correction = (0.9960-0.0003)*(1.0027-0.0002)*(0.9962-0.00052);
					break;
				case 1:
					correction = (0.9960+0.0003)*(1.0027+0.0002)*(0.9962+0.00052);
					break;
				default:
					correction = 0.9960*1.0027*0.9962;
			}
		}
		else if(abs(muEta)>=2.1 && abs(muEta)<=2.4) { //Note: Trigger scale factors only provided up to absolute eta of 2.1 in the link above, so I have used the same as for the 1.2 to 2.1 eta range.
			switch (muon_scale_factor_systematic) {
				case -1:
					correction = (0.9952-0.0006)*(1.0633-0.0007)*(0.9962-0.00052);
					break;
				case 1:
					correction = (0.9952+0.0006)*(1.0633+0.0007)*(0.9962+0.00052);
					break;
				default:
					correction = 0.9952*1.0633*0.9962;
			}
		}
	}
	else{ //QCD has non-isolated muon hence isolation scale factor is irrelevant here
		if(abs(muEta)<0.9) {
			switch (muon_scale_factor_systematic) {
				case -1:
					correction = (0.9925-0.0002)*(0.9837-0.00021);
					break;
				case 1:
					correction = (0.9925+0.0002)*(0.9837+0.00021);
					break;
				default:
					correction = 0.9925*0.9837;
			}
		}
		else if(abs(muEta)>=0.9 && abs(muEta)<1.2) {
			switch (muon_scale_factor_systematic) {
				case -1:
					correction = (0.9928-0.0003)*(0.9656-0.00066);
					break;
				case 1:
					correction = (0.9928+0.0003)*(0.9656+0.00066);
					break;
				default:
					correction = 0.9928*0.9656;
			}
		}
		else if(abs(muEta)>=1.2 && abs(muEta)<2.1) {
			switch (muon_scale_factor_systematic) {
				case -1:
					correction = (0.9960-0.0003)*(0.9962-0.00052);
					break;
				case 1:
					correction = (0.9960+0.0003)*(0.9962+0.00052);
					break;
				default:
					correction = 0.9960*0.9962;
			}
		}
		else if(abs(muEta)>=2.1 && abs(muEta)<=2.4) {
			switch (muon_scale_factor_systematic) {
				case -1:
					correction = (0.9952-0.0006)*(0.9962-0.00052);
					break;
				case 1:
					correction = (0.9952+0.0006)*(0.9962+0.00052);
					break;
				default:
					correction = 0.9952*0.9962;
			}
		}
	}
	return correction;
}

}
