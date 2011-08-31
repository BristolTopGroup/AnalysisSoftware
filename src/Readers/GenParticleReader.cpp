/*
 * GenParticleReader.cpp
 *
 *  Created on: Feb 23, 2011
 *      Author: phzss
 */

#include "../../interface/Readers/GenParticleReader.h"
namespace BAT {

GenParticleReader::GenParticleReader() :
    energyReader(),
	pxReader(),
	pyReader(),
	pzReader(),
	pdgIdReader(),
	statusReader(),
	motherIndexReader() {

}

GenParticleReader::GenParticleReader(TChainPointer input) :
    energyReader(input, "GenParticle.Energy"),
    pxReader(input, "GenParticle.Px"),
    pyReader(input, "GenParticle.Py"),
    pzReader(input, "GenParticle.Pz"),
    pdgIdReader(input, "GenParticle.PdgId"),
    statusReader(input, "GenParticle.Status"),
    motherIndexReader(input, "GenParticle.MotherIndex") {

}


GenParticleReader::~GenParticleReader() {
}

const MCParticleCollection& GenParticleReader::getGenParticles() {
    if (genParticles.empty() == false)
        genParticles.clear();
    readGenParticles();
    return genParticles;
}

void GenParticleReader::readGenParticles() {
    for (unsigned int index = 0; index < pxReader.size(); index++) {
    	double energy = energyReader.getVariableAt(index);
    	double px = pxReader.getVariableAt(index);
    	double py = pyReader.getVariableAt(index);
    	double pz = pzReader.getVariableAt(index);


        int pdgId = pdgIdReader.getIntVariableAt(index);
        int status = statusReader.getIntVariableAt(index);
        int motherIndex = motherIndexReader.getIntVariableAt(index);

        MCParticlePointer genParticle(new MCParticle(energy, px, py, pz));
        genParticle->setPdgId(pdgId);
        genParticle->setStatus(status);
        genParticle->setMotherIndex(motherIndex);

        genParticles.push_back(genParticle);
    }
}


void GenParticleReader::initialise() {
    energyReader.initialiseBlindly();
	pxReader.initialiseBlindly();
    pyReader.initialiseBlindly();
    pzReader.initialiseBlindly();

    pdgIdReader.initialiseBlindly();
    statusReader.initialiseBlindly();
    motherIndexReader.initialiseBlindly();
}

}
