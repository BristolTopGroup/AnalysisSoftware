/*
 * MuonReader.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/Readers/MuonReader.h"
#include "../../interface/GlobalVariables.h"
#include <iostream>

using namespace std;

namespace BAT {

MuonReader::MuonReader() :
		energyReader(), //
		pxReader(), //
		pyReader(), //
		pzReader(), //
		chargeReader(), //
		d0_BS_Reader(), //
		d0_PV_Reader(), //
		ecalIsolationReader(), //
		hcalIsolationReader(), //
		trackerIsolationReader(), //
		relTrackIsolationReader_(), //
		PFRelIso03Reader_(), //
		PFRelIso04Reader_(), //
		PFRelIso03DeltaBetaReader_(), //
		PFRelIso04DeltaBetaReader_(), //
		sumChargedHadronPt03Reader_(),//
		sumNeutralHadronPt03Reader_(),//
		sumPhotonPt03Reader_(),//
		sumPUPt03Reader_(),//
		sumChargedHadronPt04Reader_(),//
		sumNeutralHadronPt04Reader_(),//
		sumPhotonPt04Reader_(),//
		sumPUPt04Reader_(),//


		isGlobalMuon(), //
		isTrackerMuon(), //
		isPFMuon(), //
		vertex_dist_z(), //
		normalisedChi2(), //
		NumberOfValidHits(), //
		NumberOfValidMuonHits(), //
		PixelLayersWithMeasurement(), //
		TrackerLayersWithMeasurement(), //
		NumberOfValidPixelHits(), //
		NumberOfMatchedStations(), //
		algorithm(MuonAlgorithm::Default), //
		muons() {

}

MuonReader::MuonReader(TChainPointer input, MuonAlgorithm::value algo) :
		energyReader(input, "Muons.Energy"), //
		pxReader(input, "Muons.Px"), //
		pyReader(input, "Muons.Py"), //
		pzReader(input, "Muons.Pz"), //
		chargeReader(input, "Muons.Charge"), //
		d0_BS_Reader(input, "Muons.BeamSpotDXY"), //
		d0_PV_Reader(input, "Muons.PrimaryVertexDXY"), //
		ecalIsolationReader(input, "Muons.EcalIso03"), //
		hcalIsolationReader(input, "Muons.HcalIso03"), //
		trackerIsolationReader(input, "Muons.TrkIso03"), //
		relTrackIsolationReader_(input, "Muons.RelTrkIso03"), //
		PFRelIso03Reader_(input, "Muons.PFRelIso03"), //
		PFRelIso04Reader_(input, "Muons.PFRelIso04"), //
		PFRelIso03DeltaBetaReader_(input, "Muons.PFRelIso03DeltaBeta"), //
		PFRelIso04DeltaBetaReader_(input, "Muons.PFRelIso04DeltaBeta"), //
		sumChargedHadronPt03Reader_(input, "Muons.sumChargedHadronPt03"), //
		sumNeutralHadronPt03Reader_(input, "Muons.sumNeutralHadronPt03"), //
		sumPhotonPt03Reader_(input, "Muons.sumPhotonPt03"), //
		sumPUPt03Reader_(input, "Muons.sumPUPt03"), //
		sumChargedHadronPt04Reader_(input, "Muons.sumChargedHadronPt04"), //
		sumNeutralHadronPt04Reader_(input, "Muons.sumNeutralHadronPt04"), //
		sumPhotonPt04Reader_(input, "Muons.sumPhotonPt04"), //
		sumPUPt04Reader_(input, "Muons.sumPUPt04"), //

		isGlobalMuon(input, "Muons.isGlobalMuon"), //
		isTrackerMuon(input, "Muons.isTrackerMuon"), //
		isPFMuon(input, "Muons.isPFMuon"), //
		vertex_dist_z(input, "Muons.Vertex.DistZ"), //
		normalisedChi2(input, "Muons.GlobalTrack.NormalizedChi2"), //
		NumberOfValidHits(input, "Muons.InnerTrack.NumberOfValidHits"), //
		NumberOfValidMuonHits(input, "Muons.GlobalTrack.NumberOfValidMuonHits"), //
		PixelLayersWithMeasurement(input, "Muons.InnerTrack.PixelLayersWithMeasurement"), //
		TrackerLayersWithMeasurement(input,
				"Muons.InnerTrack.TrackerLayersWithMeasurement"), //
		NumberOfValidPixelHits(input, "Muons.InnerTrack.NumberOfValidPixelHits"), //
		NumberOfMatchedStations(input, "Muons.NumberOfMatchedStations"), //
		NumberOfMatches(input, "Muons.NumberOfMatches"), //
		algorithm(algo), muons() {

}

void MuonReader::initialise() {
	energyReader.initialise();
	pxReader.initialise();
	pyReader.initialise();
	pzReader.initialise();
	chargeReader.initialise();

	d0_BS_Reader.initialise();
	d0_PV_Reader.initialise();
	ecalIsolationReader.initialise();
	hcalIsolationReader.initialise();
	trackerIsolationReader.initialise();

	// relTrackIsolationReader_.initialise();

	PFRelIso03Reader_.initialise();
	PFRelIso04Reader_.initialise();
	PFRelIso03DeltaBetaReader_.initialise();
	PFRelIso04DeltaBetaReader_.initialise();

	sumChargedHadronPt03Reader_.initialise();
	sumNeutralHadronPt03Reader_.initialise();
	sumPhotonPt03Reader_.initialise();
	sumPUPt03Reader_.initialise();

	sumChargedHadronPt04Reader_.initialise();
	sumNeutralHadronPt04Reader_.initialise();
	sumPhotonPt04Reader_.initialise();
	sumPUPt04Reader_.initialise();

	isGlobalMuon.initialise();
	isTrackerMuon.initialise();

	vertex_dist_z.initialise();
	normalisedChi2.initialise();
	NumberOfValidHits.initialise();
	NumberOfValidMuonHits.initialise();
	PixelLayersWithMeasurement.initialise();
	if (Globals::NTupleVersion >= 8) {
		isPFMuon.initialise();
		TrackerLayersWithMeasurement.initialise();
		NumberOfValidPixelHits.initialise();
	} else {
		cout << "Using an old (<8) nTuple version. Following variables will not be available:" << endl;
		cout << "Muon::isPFMuon(), Muon::trackerLayersWithMeasurement(), Muon::numberOfValidPixelHits()" << endl;
	}
	NumberOfMatchedStations.initialise();
	NumberOfMatches.initialise();

}

const MuonCollection& MuonReader::getMuons() {
	if (muons.empty() == false)
		muons.clear();
	readMuons();
	return muons;
}

void MuonReader::readMuons() {
	for (unsigned int index = 0; index < energyReader.size(); index++) {
		double energy = energyReader.getVariableAt(index);
		double px = pxReader.getVariableAt(index);
		double py = pyReader.getVariableAt(index);
		double pz = pzReader.getVariableAt(index);

		MuonPointer muon(new Muon(energy, px, py, pz));
		muon->setUsedAlgorithm(algorithm);
		muon->setCharge(chargeReader.getIntVariableAt(index));

		muon->setEcalIsolation(ecalIsolationReader.getVariableAt(index));
		muon->setHcalIsolation(hcalIsolationReader.getVariableAt(index));
		muon->setTrackerIsolation(trackerIsolationReader.getVariableAt(index));

		// muon->setRelTrkIsolation(relTrackIsolationReader_.getVariableAt(index));

		muon->setPFRelIso03(PFRelIso03Reader_.getVariableAt(index));
		muon->setPFRelIso04(PFRelIso04Reader_.getVariableAt(index));
		muon->setPFRelIso03DeltaBeta(PFRelIso03DeltaBetaReader_.getVariableAt(index));
		muon->setPFRelIso04DeltaBeta(PFRelIso04DeltaBetaReader_.getVariableAt(index));
		muon->setsumChargedHadronPt03(sumChargedHadronPt03Reader_.getVariableAt(index));
		muon->setsumNeutralHadronPt03(sumNeutralHadronPt03Reader_.getVariableAt(index));
		muon->setsumPhotonPt03(sumPhotonPt03Reader_.getVariableAt(index));
		muon->setsumPUPt03(sumPUPt03Reader_.getVariableAt(index));

		muon->setsumChargedHadronPt04(sumChargedHadronPt04Reader_.getVariableAt(index));
		muon->setsumNeutralHadronPt04(sumNeutralHadronPt04Reader_.getVariableAt(index));
		muon->setsumPhotonPt04(sumPhotonPt04Reader_.getVariableAt(index));
		muon->setsumPUPt04(sumPUPt04Reader_.getVariableAt(index));


		muon->setD0(d0_PV_Reader.getVariableAt(index));
		muon->setD0_wrtBeamSpot(d0_BS_Reader.getVariableAt(index));

		muon->makeGlobal(isGlobalMuon.getBoolVariableAt(index));
		muon->setTrackerMuon(isTrackerMuon.getBoolVariableAt(index));

		muon->setZDistanceToPrimaryVertex(vertex_dist_z.getVariableAt(index));
		muon->setNormalisedChi2(normalisedChi2.getVariableAt(index));
		muon->setNumberOfValidHits(NumberOfValidHits.getIntVariableAt(index));
		muon->setNumberOfValidMuonHits(NumberOfValidMuonHits.getIntVariableAt(index));
		muon->setPixelLayersWithMeasurement(PixelLayersWithMeasurement.getIntVariableAt(index));
		muon->setNumberOfMatchedStations(NumberOfMatchedStations.getIntVariableAt(index));
		muon->setNumberOfMatches(NumberOfMatches.getIntVariableAt(index));

		if (Globals::NTupleVersion >= 8) {
			muon->setPFMuon(isPFMuon.getBoolVariableAt(index));
			muon->setTrackerLayersWithMeasurement(TrackerLayersWithMeasurement.getIntVariableAt(index));
			muon->setNumberOfValidPixelHits(NumberOfValidPixelHits.getIntVariableAt(index));
		}

		muons.push_back(muon);
	}
}
MuonReader::~MuonReader() {
}

}
