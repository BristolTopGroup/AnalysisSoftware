/*
 * MuonReader.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/Readers/MuonReader.h"

namespace BAT {

MuonReader::MuonReader() :
//	numberOfMuonsReader(),
	energyReader(),
	pxReader(),
	pyReader(),
	pzReader(),
	ecalIsolationReader(),
	hcalIsolationReader(),
	trackerIsolationReader(),
	isGlobalReader() {

}

MuonReader::MuonReader(TChainPointer input, MuonAlgorithm::value algo) :
	energyReader(input, MuonAlgorithm::prefixes.at(algo) + ".Energy"),
	pxReader(input, MuonAlgorithm::prefixes.at(algo) + ".Px"),
	pyReader(input, MuonAlgorithm::prefixes.at(algo) + ".Py"),
	pzReader(input, MuonAlgorithm::prefixes.at(algo) + ".Pz"),
	ecalIsolationReader(input, MuonAlgorithm::prefixes.at(algo) + ".EcalIso03"),
	hcalIsolationReader(input, MuonAlgorithm::prefixes.at(algo) + ".HcalIso03"),
	trackerIsolationReader(input, MuonAlgorithm::prefixes.at(algo) + ".TrkIso03"),
	isGlobalReader(input, MuonAlgorithm::prefixes.at(algo) + ".isGlobalMuon") {

}

void MuonReader::initialise() {
    energyReader.initialise();
    pxReader.initialise();
    pyReader.initialise();
    pzReader.initialise();

    ecalIsolationReader.initialise();
    hcalIsolationReader.initialise();
    trackerIsolationReader.initialise();

    isGlobalReader.initialise();
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
        muon->setEcalIsolation(ecalIsolationReader.getVariableAt(index));
        muon->setHcalIsolation(hcalIsolationReader.getVariableAt(index));
        muon->setTrackerIsolation(trackerIsolationReader.getVariableAt(index));
        muon->makeGlobal(isGlobalReader.getBoolVariableAt(index));
        muons.push_back(muon);
    }
}
MuonReader::~MuonReader() {
}

}
