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
		PFGammaIsolationReader_DR03_(), //
		PFChargedHadronIsolationReader_DR03_(), //
		PFNeutralHadronIsolationReader_DR03_(), //
		PFPUChargedHadron_Isolation_DR03_(), //
		PFGammaIsolationReader_DR04_(), //
		PFChargedHadronIsolationReader_DR04_(), //
		PFNeutralHadronIsolationReader_DR04_(), //
		PFPUChargedHadron_Isolation_DR04_(), //
		PFGammaIsolationReader_DR05_(), //
		PFChargedHadronIsolationReader_DR05_(), //
		PFNeutralHadronIsolationReader_DR05_(), //
		PFPUChargedHadron_Isolation_DR05_(), //
		PFDeltaBeta_Isolation_DR04_(), //
		sumChargedHadronPt04_(),//
		sumNeutralHadronPt04_(),//
		sumPhotonPt04_(),//
		sumPUPt04_(),//


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
		// PFGammaIsolationReader_DR03_(input, "Muons.PFGammaIso03"), //
		// PFChargedHadronIsolationReader_DR03_(input, "Muons.PfChargedHadronIso03"), //
		// PFNeutralHadronIsolationReader_DR03_(input, "Muons.PfNeutralHadronIso03"), //
		// PFPUChargedHadron_Isolation_DR03_(input, "Muons.PfPUChargedHadronIso03"), //
		// PFGammaIsolationReader_DR04_(input, "Muons.PFGammaIso04"), //
		// PFChargedHadronIsolationReader_DR04_(input, "Muons.PfChargedHadronIso04"), //
		// PFNeutralHadronIsolationReader_DR04_(input, "Muons.PfNeutralHadronIso04"), //
		// PFPUChargedHadron_Isolation_DR04_(input, "Muons.PfPUChargedHadronIso04"), //
		// PFGammaIsolationReader_DR05_(input, "Muons.PFGammaIso05"), //
		// PFChargedHadronIsolationReader_DR05_(input, "Muons.PfChargedHadronIso05"), //
		// PFNeutralHadronIsolationReader_DR05_(input, "Muons.PfNeutralHadronIso05"), //
		// PFPUChargedHadron_Isolation_DR05_(input, "Muons.PfPUChargedHadronIso05"), //
		// PFDeltaBeta_Isolation_DR04_(input, "Muons.PFRelIso04DeltaBeta"), //
		// sumChargedHadronPt04_(input, "Muons.sumChargedHadronPt04"), //
		// sumNeutralHadronPt04_(input, "Muons.sumNeutralHadronPt04"), //
		// sumPhotonPt04_(input, "Muons.sumPhotonPt04"), //
		// sumPUPt04_(input, "Muons.sumPUPt04"), //
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
	// if (algorithm == MuonAlgorithm::ParticleFlow) {
	// 	PFGammaIsolationReader_DR03_.initialise();
	// 	PFChargedHadronIsolationReader_DR03_.initialise();
	// 	PFNeutralHadronIsolationReader_DR03_.initialise();
	// 	PFGammaIsolationReader_DR04_.initialise();
	// 	PFChargedHadronIsolationReader_DR04_.initialise();
	// 	PFNeutralHadronIsolationReader_DR04_.initialise();
	// 	PFGammaIsolationReader_DR05_.initialise();
	// 	PFChargedHadronIsolationReader_DR05_.initialise();
	// 	PFNeutralHadronIsolationReader_DR05_.initialise();
	// 	if (Globals::NTupleVersion >= 9) {
	// 		PFPUChargedHadron_Isolation_DR03_.initialise();
	// 		PFPUChargedHadron_Isolation_DR04_.initialise();
	// 		PFPUChargedHadron_Isolation_DR05_.initialise();
	// 		PFDeltaBeta_Isolation_DR04_.initialise();
	// 	}else {
	// 		cout << "Using an old (<9) nTuple version. Following variables will not be available:" << endl;
	// 		cout << "Muon::PfPUChargedHadronIso()" << endl;
	// 	}

	// 			if(Globals::NTupleVersion >= 10) {
	// 				sumChargedHadronPt04_.initialise();
	// 				sumNeutralHadronPt04_.initialise();
	// 				sumPhotonPt04_.initialise();
	// 				sumPUPt04_.initialise();
	// 			}
	// }

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

		// if (algorithm == MuonAlgorithm::ParticleFlow) {
		// 	muon->setPFGammaIsolation(PFGammaIsolationReader_DR03_.getVariableAt(index), 0.3);
		// 	muon->setPFChargedHadronIsolation(PFChargedHadronIsolationReader_DR03_.getVariableAt(index), 0.3);
		// 	muon->setPFNeutralHadronIsolation(PFNeutralHadronIsolationReader_DR03_.getVariableAt(index), 0.3);
		// 	muon->setPFGammaIsolation(PFGammaIsolationReader_DR04_.getVariableAt(index), 0.4);
		// 	muon->setPFChargedHadronIsolation(PFChargedHadronIsolationReader_DR04_.getVariableAt(index), 0.4);
		// 	muon->setPFNeutralHadronIsolation(PFNeutralHadronIsolationReader_DR04_.getVariableAt(index), 0.4);
		// 	muon->setPFGammaIsolation(PFGammaIsolationReader_DR05_.getVariableAt(index), 0.5);
		// 	muon->setPFChargedHadronIsolation(PFChargedHadronIsolationReader_DR05_.getVariableAt(index), 0.5);
		// 	muon->setPFNeutralHadronIsolation(PFNeutralHadronIsolationReader_DR05_.getVariableAt(index), 0.5);
		// 	if (Globals::NTupleVersion >= 9) {
		// 		muon->setPFPUChargedHadronIsolation(PFPUChargedHadron_Isolation_DR03_.getVariableAt(index), 0.3);
		// 		muon->setPFPUChargedHadronIsolation(PFPUChargedHadron_Isolation_DR04_.getVariableAt(index), 0.4);
		// 		muon->setPFPUChargedHadronIsolation(PFPUChargedHadron_Isolation_DR05_.getVariableAt(index), 0.5);
		// 		muon->setPFDeltaBeta_Isolation_DR04(PFDeltaBeta_Isolation_DR04_.getVariableAt(index));
		// 	} else {
		// 		muon->setPFPUChargedHadronIsolation(0, 0.3);
		// 		muon->setPFPUChargedHadronIsolation(0, 0.4);
		// 		muon->setPFPUChargedHadronIsolation(0, 0.5);
		// 	}

		// 	if (Globals::NTupleVersion >= 10) {
		// 			muon->setsumChargedHadronPt04(sumChargedHadronPt04_.getVariableAt(index));
		// 			muon->setsumNeutralHadronPt04(sumNeutralHadronPt04_.getVariableAt(index));
		// 			muon->setsumPhotonPt04(sumPhotonPt04_.getVariableAt(index));
		// 			muon->setsumPUPt04(sumPUPt04_.getVariableAt(index));
		// 	}

		// }

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
