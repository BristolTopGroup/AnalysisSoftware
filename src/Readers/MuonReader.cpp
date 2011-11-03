/*
 * MuonReader.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/Readers/MuonReader.h"

namespace BAT {

MuonReader::MuonReader() :
	energyReader(),
	pxReader(),
	pyReader(),
	pzReader(),
	chargeReader(),
	d0_BS_Reader(),
	d0_PV_Reader(),
	ecalIsolationReader(),
	hcalIsolationReader(),
	trackerIsolationReader(),
	PFGammaIsolationReader(),
	PFChargedHadronIsolationReader(),
	PFNeutralHadronIsolationReader(),
	isGlobalMuonReader(),
	isTrackerMuonReader(),
	vertex_dist_z(),
	normalisedChi2(),
	NumberOfValidHits(),
	NumberOfValidMuonHits(),
	PixelLayersWithMeasurement(),
	NumberOfMatchedStations(),
	algorithm(MuonAlgorithm::Default),
	muons(){

}

MuonReader::MuonReader(TChainPointer input, MuonAlgorithm::value algo) :
	energyReader(input, MuonAlgorithm::prefixes.at(algo) + ".Energy"),
	pxReader(input, MuonAlgorithm::prefixes.at(algo) + ".Px"),
	pyReader(input, MuonAlgorithm::prefixes.at(algo) + ".Py"),
	pzReader(input, MuonAlgorithm::prefixes.at(algo) + ".Pz"),
	chargeReader(input, MuonAlgorithm::prefixes.at(algo) + ".Charge"),
	d0_BS_Reader(input, MuonAlgorithm::prefixes.at(algo) + ".BeamSpotDXY"),
	d0_PV_Reader(input, MuonAlgorithm::prefixes.at(algo) + ".PrimaryVertexDXY"),
	ecalIsolationReader(input, MuonAlgorithm::prefixes.at(algo) + ".EcalIso03"),
	hcalIsolationReader(input, MuonAlgorithm::prefixes.at(algo) + ".HcalIso03"),
	trackerIsolationReader(input, MuonAlgorithm::prefixes.at(algo) + ".TrkIso03"),
	PFGammaIsolationReader(input, MuonAlgorithm::prefixes.at(algo) + ".PFGammaIso03"),
	PFChargedHadronIsolationReader(input, MuonAlgorithm::prefixes.at(algo) + ".PfChargedHadronIso03"),
	PFNeutralHadronIsolationReader(input, MuonAlgorithm::prefixes.at(algo) + ".PfNeutralHadronIso03"),
	isGlobalMuonReader(input, MuonAlgorithm::prefixes.at(algo) + ".isGlobalMuon"),
	isTrackerMuonReader(input, MuonAlgorithm::prefixes.at(algo) + ".isTrackerMuon"),
	vertex_dist_z(input, MuonAlgorithm::prefixes.at(algo) + ".Vertex.DistZ"),
	normalisedChi2(input, MuonAlgorithm::prefixes.at(algo) + ".GlobalTrack.NormalizedChi2"),
	NumberOfValidHits(input, MuonAlgorithm::prefixes.at(algo) + ".InnerTrack.NumberOfValidHits"),
	NumberOfValidMuonHits(input, MuonAlgorithm::prefixes.at(algo) + ".GlobalTrack.NumberOfValidMuonHits"),
	PixelLayersWithMeasurement(input, MuonAlgorithm::prefixes.at(algo) + ".InnerTrack.PixelLayersWithMeasurement"),
	NumberOfMatchedStations(input, MuonAlgorithm::prefixes.at(algo) + ".NumberOfMatchedStations"),
	NumberOfMatches(input, MuonAlgorithm::prefixes.at(algo) + ".NumberOfMatches"),
	algorithm(algo),
	muons() {

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
    PFGammaIsolationReader.initialise();
    PFChargedHadronIsolationReader.initialise();
    PFNeutralHadronIsolationReader.initialise();

    isGlobalMuonReader.initialise();
    isTrackerMuonReader.initialise();
    vertex_dist_z.initialise();
    normalisedChi2.initialise();
    NumberOfValidHits.initialise();
    NumberOfValidMuonHits.initialise();
    PixelLayersWithMeasurement.initialise();
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
        muon->setPFGammaIsolation(PFGammaIsolationReader.getVariableAt(index));
        muon->setPFChargedHadronIsolation(PFChargedHadronIsolationReader.getVariableAt(index));
        muon->setPFNeutralHadronIsolation(PFNeutralHadronIsolationReader.getVariableAt(index));

        muon->setD0(d0_PV_Reader.getVariableAt(index));
        muon->setD0_wrtBeamSpot(d0_BS_Reader.getVariableAt(index));

        muon->makeGlobal(isGlobalMuonReader.getBoolVariableAt(index));
        muon->setTrackerMuon(isTrackerMuonReader.getBoolVariableAt(index));

        muon->setZDistanceToPrimaryVertex(vertex_dist_z.getVariableAt(index));
        muon->setNormalisedChi2(normalisedChi2.getVariableAt(index));
        muon->setNumberOfValidHits(NumberOfValidHits.getIntVariableAt(index));
        muon->setNumberOfValidMuonHits(NumberOfValidMuonHits.getIntVariableAt(index));
        muon->setPixelLayersWithMeasurement(PixelLayersWithMeasurement.getIntVariableAt(index));
        muon->setNumberOfMatchedStations(NumberOfMatchedStations.getIntVariableAt(index));
        muon->setNumberOfMatches(NumberOfMatches.getIntVariableAt(index));


        muons.push_back(muon);
    }
}
MuonReader::~MuonReader() {
}

}
