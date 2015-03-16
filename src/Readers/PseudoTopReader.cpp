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
    pseudoTop_pdgIdReader_(),
    pseudoTop_energyReader_(),
	pseudoTop_pxReader_(),
	pseudoTop_pyReader_(),
	pseudoTop_pzReader_(),
    pseudoTop_neutrino_pdgIdReader_(),
    pseudoTop_neutrino_energyReader_(),
    pseudoTop_neutrino_pxReader_(),
    pseudoTop_neutrino_pyReader_(),
    pseudoTop_neutrino_pzReader_(),
    pseudoTop_jet_energyReader_(),
    pseudoTop_jet_pxReader_(),
    pseudoTop_jet_pyReader_(),
    pseudoTop_jet_pzReader_(),
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
    pseudoTop_pdgIdReader_(input, "PseudoTops.PdgId"),
    pseudoTop_energyReader_(input, "PseudoTops.Energy"),
    pseudoTop_pxReader_(input, "PseudoTops.Px"),
    pseudoTop_pyReader_(input, "PseudoTops.Py"),
    pseudoTop_pzReader_(input, "PseudoTops.Pz"),
    pseudoTop_neutrino_pdgIdReader_(input, "PseudoTopNeutrinos.PdgId"),
    pseudoTop_neutrino_energyReader_(input, "PseudoTopNeutrinos.Energy"),
    pseudoTop_neutrino_pxReader_(input, "PseudoTopNeutrinos.Px"),
    pseudoTop_neutrino_pyReader_(input, "PseudoTopNeutrinos.Py"),
    pseudoTop_neutrino_pzReader_(input, "PseudoTopNeutrinos.Pz"),
    pseudoTop_jet_energyReader_(input, "PseudoTopJets.Energy"),
    pseudoTop_jet_pxReader_(input, "PseudoTopJets.Px"),
    pseudoTop_jet_pyReader_(input, "PseudoTopJets.Py"),
    pseudoTop_jet_pzReader_(input, "PseudoTopJets.Pz"),
    pseudoTop_lepton_energyReader_(input, "PseudoTopLeptons.Energy"),
    pseudoTop_lepton_pxReader_(input, "PseudoTopLeptons.Px"),
    pseudoTop_lepton_pyReader_(input, "PseudoTopLeptons.Py"),
    pseudoTop_lepton_pzReader_(input, "PseudoTopLeptons.Pz"),
    pseudoTopParticles_( new PseudoTopParticles() ),
    newPseudoTops_(),
    newLeptonicW_( new Particle() ),
    newLepton_( new MCParticle() ),
    newAllLeptons_(),
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
    newAllLeptons_.clear();
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
    for (unsigned int index = 0; index < pseudoTop_pdgIdReader_.size(); index++) {
        int pdgId = pseudoTop_pdgIdReader_.getIntVariableAt(index);
        double energy = pseudoTop_energyReader_.getVariableAt(index);
        double px = pseudoTop_pxReader_.getVariableAt(index);
        double py = pseudoTop_pyReader_.getVariableAt(index);
        double pz = pseudoTop_pzReader_.getVariableAt(index);

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
    for (unsigned int index = 0; index < pseudoTop_neutrino_pdgIdReader_.size(); index++) {
        // double pdgId = pseudoTop_neutrino_pdgIdReader_.getIntVariableAt(index);
        double energy = pseudoTop_neutrino_energyReader_.getVariableAt(index);
        double px = pseudoTop_neutrino_pxReader_.getVariableAt(index);
        double py = pseudoTop_neutrino_pyReader_.getVariableAt(index);
        double pz = pseudoTop_neutrino_pzReader_.getVariableAt(index);

        *pseudoMet = Particle( *pseudoMet + Particle(energy,px,py,pz) );

        // if ( fabs( pseudoMet->pt() - pseudoMet->et() ) > 1 ) {
        //     cout << pseudoMet->pt() << " " << pseudoMet->et() << endl;
        //     cout << pdgId << " " << energy << " " << px << " " << py << " " << pz << " " << endl;
        // }
    }

    pseudoTopParticles_->setPseudoMET( ParticlePointer( pseudoMet ) );

    // Get Jets for HT calculation
    for (unsigned int index = 0; index < pseudoTop_jet_energyReader_.size(); index++) {
        double energy = pseudoTop_jet_energyReader_.getVariableAt(index);
        double px = pseudoTop_jet_pxReader_.getVariableAt(index);
        double py = pseudoTop_jet_pyReader_.getVariableAt(index);
        double pz = pseudoTop_jet_pzReader_.getVariableAt(index);
        newJets_.push_back( JetPointer( new Jet( energy, px, py, pz )) );
    }

    pseudoTopParticles_->setPseudoJets( newJets_ );

    // Get leptons for selection criteria
    for (unsigned int index = 0; index < pseudoTop_lepton_energyReader_.size(); index++) {
        double energy = pseudoTop_lepton_energyReader_.getVariableAt(index);
        double px = pseudoTop_lepton_pxReader_.getVariableAt(index);
        double py = pseudoTop_lepton_pyReader_.getVariableAt(index);
        double pz = pseudoTop_lepton_pzReader_.getVariableAt(index);
        newAllLeptons_.push_back( ParticlePointer( new Particle( energy, px, py, pz )) );
    }

    pseudoTopParticles_->setAllPseudoLeptons( newAllLeptons_ );

}


void PseudoTopReader::initialise() {
    pseudoTop_pdgIdReader_.initialise();
    pseudoTop_energyReader_.initialise();
    pseudoTop_pxReader_.initialise();
    pseudoTop_pyReader_.initialise();
    pseudoTop_pzReader_.initialise();

    pseudoTop_neutrino_pdgIdReader_.initialise();
    pseudoTop_neutrino_energyReader_.initialise();
    pseudoTop_neutrino_pxReader_.initialise();
    pseudoTop_neutrino_pyReader_.initialise();
    pseudoTop_neutrino_pzReader_.initialise();

    pseudoTop_jet_energyReader_.initialise();
    pseudoTop_jet_pxReader_.initialise();
    pseudoTop_jet_pyReader_.initialise();
    pseudoTop_jet_pzReader_.initialise();

    pseudoTop_lepton_energyReader_.initialise();
    pseudoTop_lepton_pxReader_.initialise();
    pseudoTop_lepton_pyReader_.initialise();
    pseudoTop_lepton_pzReader_.initialise();
}

}
