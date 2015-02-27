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
    pseudoTop_neutrino_pdgIdReader(),
    pseudoTop_neutrino_energyReader(),
    pseudoTop_neutrino_pxReader(),
    pseudoTop_neutrino_pyReader(),
    pseudoTop_neutrino_pzReader(),
    pseudoTop_jet_energyReader(),
    pseudoTop_jet_pxReader(),
    pseudoTop_jet_pyReader(),
    pseudoTop_jet_pzReader(),
    pseudoTopParticles_( new PseudoTopParticles() ),
    newPseudoTops_(),
    newLeptonicW_( new Particle() ),
    newLepton_( new MCParticle() ),
    newJets_(),
    newPseudoBs_(),
    newNeutrino_( new Particle() ),
    newPseudoMET_( new Particle() ),
    isSemiLeptonic_(true)
{

}

PseudoTopReader::PseudoTopReader(TChainPointer input) :
    pseudoTop_pdgIdReader(input, "PseudoTops.PdgId"),
    pseudoTop_energyReader(input, "PseudoTops.Energy"),
    pseudoTop_pxReader(input, "PseudoTops.Px"),
    pseudoTop_pyReader(input, "PseudoTops.Py"),
    pseudoTop_pzReader(input, "PseudoTops.Pz"),
    pseudoTop_neutrino_pdgIdReader(input, "PseudoTopNeutrinos.PdgId"),
    pseudoTop_neutrino_energyReader(input, "PseudoTopNeutrinos.Energy"),
    pseudoTop_neutrino_pxReader(input, "PseudoTopNeutrinos.Px"),
    pseudoTop_neutrino_pyReader(input, "PseudoTopNeutrinos.Py"),
    pseudoTop_neutrino_pzReader(input, "PseudoTopNeutrinos.Pz"),
    pseudoTop_jet_energyReader(input, "PseudoTopJets.Energy"),
    pseudoTop_jet_pxReader(input, "PseudoTopJets.Px"),
    pseudoTop_jet_pyReader(input, "PseudoTopJets.Py"),
    pseudoTop_jet_pzReader(input, "PseudoTopJets.Pz"),
    pseudoTopParticles_( new PseudoTopParticles() ),
    newPseudoTops_(),
    newLeptonicW_( new Particle() ),
    newLepton_( new MCParticle() ),
    newJets_(),
    newPseudoBs_(),
    newNeutrino_( new Particle() ),
    newPseudoMET_( new Particle() ),
    isSemiLeptonic_(true)
{
}


PseudoTopReader::~PseudoTopReader() {
}

const PseudoTopParticlesPointer& PseudoTopReader::getPseudoTopParticles() {

    newPseudoTops_.clear();
    newLeptonicW_.reset();
    newLepton_.reset();
    newNeutrino_.reset();
    newJets_.clear();
    newPseudoBs_.clear();
    newPseudoMET_.reset();
    isSemiLeptonic_ = true;

    readPseudoTopParticles();
    return pseudoTopParticles_;
}

void PseudoTopReader::readPseudoTopParticles() {


    ParticlePointer wPlus( new Particle() );
    ParticlePointer wMinus( new Particle() );

    // Go through pseudo top decay chain and extract info
    for (unsigned int index = 0; index < pseudoTop_pdgIdReader.size(); index++) {
        int pdgId = pseudoTop_pdgIdReader.getIntVariableAt(index);
        double energy = pseudoTop_energyReader.getVariableAt(index);
        double px = pseudoTop_pxReader.getVariableAt(index);
        double py = pseudoTop_pyReader.getVariableAt(index);
        double pz = pseudoTop_pzReader.getVariableAt(index);

        // Pseudo tops
        if ( fabs(pdgId) == 6 ) {
            newPseudoTops_.push_back( ParticlePointer( new Particle( energy, px, py, pz )) );
        }
        // Leptons
        else if ( fabs(pdgId) == 11 || fabs(pdgId) == 13 ) {

            // If there's more than one dressed lepton, this event is not a semi leptonic psuedo ttbar event
            if ( newLepton_ != 0 ) {
                isSemiLeptonic_ = false;
                // Store lepton with highest pt
                MCParticle anotherNewLepton( energy, px, py, pz);
                if ( anotherNewLepton.pt() > newLepton_->pt() ) {
                    newLepton_ = MCParticlePointer( new MCParticle( anotherNewLepton ) );
                    newLepton_->setPdgId( pdgId );                    
                }

            }
            else {
                newLepton_ = MCParticlePointer( new MCParticle( energy, px, py, pz ) );
                newLepton_->setPdgId( pdgId );
            }
        }
        // Neutrino
        else if ( fabs(pdgId) == 12 || fabs(pdgId) == 14 || fabs(pdgId) == 16 ) {

            if ( newNeutrino_ != 0 ) {
                // Store neutrino with highest pt
                Particle anotherNewNeutrino( energy, px, py, pz);
                if ( anotherNewNeutrino.pt() > newNeutrino_->pt() ) {
                    newNeutrino_ = ParticlePointer( new Particle( anotherNewNeutrino ) );
                }
            }
            else {
                newNeutrino_ = ParticlePointer( new Particle( energy, px, py, pz));
            }
        }
        // Bs
        else if ( fabs(pdgId) == 5 ) {
            newPseudoBs_.push_back( ParticlePointer( new Particle( energy, px, py, pz )) );            
        }
        // Ws
        else if ( fabs( pdgId ) == 24 ) {
            if ( pdgId > 0 ) {
                wPlus = ParticlePointer( new Particle( energy, px, py, pz));
            }
            else if ( pdgId < 0 ) {
                wMinus = ParticlePointer( new Particle( energy, px, py, pz));
            }
        }
    }

    // Work out which W is on the leptonic side
    if ( newLepton_ != 0 ) {
        // W+ has positive pdgId, as do negatively charged leptons
        if ( newLepton_->pdgId() < 0 ) {
            newLeptonicW_ = wPlus;
        }
        // W- has negative pdgId, as do positively charged leptons
        else if ( newLepton_->pdgId() > 0 ) {
            newLeptonicW_ = wMinus;
        }        
    }

    pseudoTopParticles_->setPseudoTops( newPseudoTops_ );
    pseudoTopParticles_->setPseudoLeptonicW( newLeptonicW_ );
    pseudoTopParticles_->setPseudoLepton( newLepton_ );
    pseudoTopParticles_->setPseudoNeutrino( newNeutrino_ );
    pseudoTopParticles_->setPseudoBs( newPseudoBs_ );
    pseudoTopParticles_->setIsSemiLeptonic( isSemiLeptonic_ );

    // Get neutrinos and calculate MET
    ParticlePointer pseudoMet = ParticlePointer( new Particle(0,0,0,0) );
    for (unsigned int index = 0; index < pseudoTop_neutrino_pdgIdReader.size(); index++) {
        // double pdgId = pseudoTop_neutrino_pdgIdReader.getIntVariableAt(index);
        double energy = pseudoTop_neutrino_energyReader.getVariableAt(index);
        double px = pseudoTop_neutrino_pxReader.getVariableAt(index);
        double py = pseudoTop_neutrino_pyReader.getVariableAt(index);
        double pz = pseudoTop_neutrino_pzReader.getVariableAt(index);

        *pseudoMet = Particle( *pseudoMet + Particle(energy,px,py,pz) );

        // if ( fabs( pseudoMet->pt() - pseudoMet->et() ) > 1 ) {
        //     cout << pseudoMet->pt() << " " << pseudoMet->et() << endl;
        //     cout << pdgId << " " << energy << " " << px << " " << py << " " << pz << " " << endl;
        // }
    }

    pseudoTopParticles_->setPseudoMET( ParticlePointer( pseudoMet ) );

    // Get Jets for HT calculation
    for (unsigned int index = 0; index < pseudoTop_jet_energyReader.size(); index++) {
        double energy = pseudoTop_jet_energyReader.getVariableAt(index);
        double px = pseudoTop_jet_pxReader.getVariableAt(index);
        double py = pseudoTop_jet_pyReader.getVariableAt(index);
        double pz = pseudoTop_jet_pzReader.getVariableAt(index);
        newJets_.push_back( JetPointer( new Jet( energy, px, py, pz )) );
    }

    pseudoTopParticles_->setPseudoJets( newJets_ );
}


void PseudoTopReader::initialise() {
    pseudoTop_pdgIdReader.initialise();
    pseudoTop_energyReader.initialise();
    pseudoTop_pxReader.initialise();
    pseudoTop_pyReader.initialise();
    pseudoTop_pzReader.initialise();

    pseudoTop_neutrino_pdgIdReader.initialise();
    pseudoTop_neutrino_energyReader.initialise();
    pseudoTop_neutrino_pxReader.initialise();
    pseudoTop_neutrino_pyReader.initialise();
    pseudoTop_neutrino_pzReader.initialise();

    pseudoTop_jet_energyReader.initialise();
    pseudoTop_jet_pxReader.initialise();
    pseudoTop_jet_pyReader.initialise();
    pseudoTop_jet_pzReader.initialise();
}

}
