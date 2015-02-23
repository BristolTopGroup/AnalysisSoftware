/*
 * PseudoTopReader.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: ejclemen
 */

#include "../../interface/Readers/PseudoTopReader.h"

using namespace std; 

namespace BAT {

PseudoTopReader::PseudoTopReader() :
    pseudoTop_pdgIdReader(),
    pseudoTop_energyReader(),
	pseudoTop_pxReader(),
	pseudoTop_pyReader(),
	pseudoTop_pzReader(),
    pseudoTopParticles_( new PseudoTopParticles() ),
    newPseudoTops_()
{

}

PseudoTopReader::PseudoTopReader(TChainPointer input) :
    pseudoTop_pdgIdReader(input, "PseudoTops.PdgId"),
    pseudoTop_energyReader(input, "PseudoTops.Energy"),
    pseudoTop_pxReader(input, "PseudoTops.Px"),
    pseudoTop_pyReader(input, "PseudoTops.Py"),
    pseudoTop_pzReader(input, "PseudoTops.Pz"),
    pseudoTopParticles_( new PseudoTopParticles() ),
    newPseudoTops_()
{
}


PseudoTopReader::~PseudoTopReader() {
}

const PseudoTopParticlesPointer& PseudoTopReader::getPseudoTopParticles() {
    // if (genParticles.empty() == false)
    //     genParticles.clear();
    newPseudoTops_.clear();
    readPseudoTopParticles();
    return pseudoTopParticles_;
}

void PseudoTopReader::readPseudoTopParticles() {

    for (unsigned int index = 0; index < pseudoTop_pdgIdReader.size(); index++) {
        int pdgId = pseudoTop_pdgIdReader.getIntVariableAt(index);

        if ( fabs(pdgId) == 6 ) {
            double energy = pseudoTop_energyReader.getVariableAt(index);
            double px = pseudoTop_pxReader.getVariableAt(index);
            double py = pseudoTop_pyReader.getVariableAt(index);
            double pz = pseudoTop_pzReader.getVariableAt(index);

            newPseudoTops_.push_back( ParticlePointer( new Particle( energy, px, py, pz )) );
        }

        // MCParticlePointer genParticle(new MCParticle(energy, px, py, pz));
        // genParticle->setPdgId(pdgId);
        // genParticle->setStatus(status);
        // genParticle->setMotherIndex(motherIndex);

        // genParticles.push_back(genParticle);
    }
 
    pseudoTopParticles_->setPseudoTops( newPseudoTops_ );
}


void PseudoTopReader::initialise() {
    pseudoTop_pdgIdReader.initialise();
    pseudoTop_energyReader.initialise();
	pseudoTop_pxReader.initialise();
    pseudoTop_pyReader.initialise();
    pseudoTop_pzReader.initialise();
}

}
