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
    pseudoTop_jet_pdgIdReader_(),
    pseudoTop_lepton_energyReader_(),
    pseudoTop_lepton_pxReader_(),
    pseudoTop_lepton_pyReader_(),
    pseudoTop_lepton_pzReader_(),
    pseudoTop_lepton_pdgIdReader_(),
    pseudoTop_met_exReader_(),
    pseudoTop_met_eyReader_(),
    tau1_Reader_(),
    tau2_Reader_(),
    tau3_Reader_(),
    tau4_Reader_(),
    tau5_Reader_(),
    tau6_Reader_(),
    pseudoTopParticles_( new PseudoTopParticles() ),
    newPseudoTops_(),
    newLeptonicW_( new MCParticle() ),
    newLepton_( new MCParticle() ),
    newJets_(),
    newPseudoBs_(),
    newNeutrino_( new Particle() ),
    newPseudoMET_( new MET() ),
    isSemiLeptonic_(true){
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
    pseudoTop_jet_pdgIdReader_(input, "PseudoTopJets.pdgId"),
    pseudoTop_lepton_energyReader_(input, "PseudoTopLeptons.Energy"),
    pseudoTop_lepton_pxReader_(input, "PseudoTopLeptons.Px"),
    pseudoTop_lepton_pyReader_(input, "PseudoTopLeptons.Py"),
    pseudoTop_lepton_pzReader_(input, "PseudoTopLeptons.Pz"),
    pseudoTop_lepton_pdgIdReader_(input, "PseudoTopLeptons.pdgId"),
    pseudoTop_met_exReader_(input, "PseudoTopMETs.Ex"),
    pseudoTop_met_eyReader_(input, "PseudoTopMETs.Ey"),

    tau1_Reader_(input, "Event.tau1"),
    tau2_Reader_(input, "Event.tau2"),
    tau3_Reader_(input, "Event.tau3"),
    tau4_Reader_(input, "Event.tau4"),
    tau5_Reader_(input, "Event.tau5"),
    tau6_Reader_(input, "Event.tau6"),
    
    pseudoTopParticles_( new PseudoTopParticles() ),
    newPseudoTops_(),
    newLeptonicW_( new MCParticle() ),
    newLepton_( new MCParticle() ),
    newAllLeptons_(),
    newJets_(),
    newPseudoBs_(),
    newNeutrino_( new Particle() ),
    newPseudoMET_( new MET() ),
    isSemiLeptonic_(true){
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

    MCParticlePointer wPlus( new MCParticle() );
    MCParticlePointer wMinus( new MCParticle() );

    // Go through pseudo top decay chain and extract info
    for (unsigned int index = 0; index < pseudoTop_pdgIdReader_.size(); index++) {
        int pdgId = pseudoTop_pdgIdReader_.getIntVariableAt(index);
        double energy = pseudoTop_energyReader_.getVariableAt(index);
        double px = pseudoTop_pxReader_.getVariableAt(index);
        double py = pseudoTop_pyReader_.getVariableAt(index);
        double pz = pseudoTop_pzReader_.getVariableAt(index);

        // Pseudo tops
        if ( fabs(pdgId) == 6 ) {
            MCParticlePointer newPseudoTop = MCParticlePointer( new MCParticle( energy, px, py, pz ) );
            newPseudoTop->setPdgId( pdgId );
            newPseudoTops_.push_back( newPseudoTop );
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
            MCParticlePointer newPseudoB = MCParticlePointer( new MCParticle( energy, px, py, pz ) );
            newPseudoB->setPdgId( pdgId );
            newPseudoBs_.push_back( newPseudoB );
        }
        // Ws
        else if ( fabs( pdgId ) == 24 ) {
            if ( pdgId > 0 ) {
                wPlus = MCParticlePointer( new MCParticle( energy, px, py, pz));
                wPlus->setPdgId( pdgId );
            }
            else if ( pdgId < 0 ) {
                wMinus = MCParticlePointer( new MCParticle( energy, px, py, pz));
                wMinus->setPdgId( pdgId );
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
    else if ( newLepton_ == 0 ) {
        isSemiLeptonic_ = false;
    }

    pseudoTopParticles_->setPseudoTops( newPseudoTops_ );
    pseudoTopParticles_->setPseudoLeptonicW( newLeptonicW_ );
    pseudoTopParticles_->setPseudoLepton( newLepton_ );
    pseudoTopParticles_->setPseudoNeutrino( newNeutrino_ );
    pseudoTopParticles_->setPseudoBs( newPseudoBs_ );
    pseudoTopParticles_->setIsSemiLeptonic( isSemiLeptonic_ );

    // Get Pseudo MET
    double ex = pseudoTop_met_exReader_.getVariable();
    double ey = pseudoTop_met_eyReader_.getVariable();
    newPseudoMET_ = METPointer(new MET( ex, ey ) );

    pseudoTopParticles_->setPseudoMET( METPointer( newPseudoMET_ ) );

    // Get Pseudo NJettiness
    pseudoTopParticles_->setPseudoTau1( tau1_Reader_.getVariable() );
    pseudoTopParticles_->setPseudoTau2( tau2_Reader_.getVariable() );
    pseudoTopParticles_->setPseudoTau3( tau3_Reader_.getVariable() );
    pseudoTopParticles_->setPseudoTau4( tau4_Reader_.getVariable() );
    pseudoTopParticles_->setPseudoTau5( tau5_Reader_.getVariable() );
    pseudoTopParticles_->setPseudoTau6( tau6_Reader_.getVariable() );

    // Get Jets for HT calculation
    for (unsigned int index = 0; index < pseudoTop_jet_energyReader_.size(); index++) {
        double energy = pseudoTop_jet_energyReader_.getVariableAt(index);
        double px = pseudoTop_jet_pxReader_.getVariableAt(index);
        double py = pseudoTop_jet_pyReader_.getVariableAt(index);
        double pz = pseudoTop_jet_pzReader_.getVariableAt(index);
        int pdgId = pseudoTop_jet_pdgIdReader_.getIntVariableAt(index);
        JetPointer newJet( new Jet( energy, px, py, pz) );
        newJet->setPartonFlavour(pdgId);
        newJets_.push_back( newJet );
    }
    pseudoTopParticles_->setPseudoJets( newJets_ );


    // Get leptons for selection criteria
    for (unsigned int index = 0; index < pseudoTop_lepton_energyReader_.size(); index++) {
        double energy = pseudoTop_lepton_energyReader_.getVariableAt(index);
        double px = pseudoTop_lepton_pxReader_.getVariableAt(index);
        double py = pseudoTop_lepton_pyReader_.getVariableAt(index);
        double pz = pseudoTop_lepton_pzReader_.getVariableAt(index);
        int pdgId = pseudoTop_lepton_pdgIdReader_.getIntVariableAt(index);

        MCParticlePointer newLepton( new MCParticle( energy, px, py, pz ) );
        newLepton->setPdgId( pdgId );
        newAllLeptons_.push_back( newLepton );
    }
    pseudoTopParticles_->setAllPseudoLeptons( newAllLeptons_ );

}


void PseudoTopReader::initialise() {
    pseudoTop_pdgIdReader_.initialiseBlindly();
    pseudoTop_energyReader_.initialiseBlindly();
    pseudoTop_pxReader_.initialiseBlindly();
    pseudoTop_pyReader_.initialiseBlindly();
    pseudoTop_pzReader_.initialiseBlindly();

    pseudoTop_neutrino_pdgIdReader_.initialiseBlindly();
    pseudoTop_neutrino_energyReader_.initialiseBlindly();
    pseudoTop_neutrino_pxReader_.initialiseBlindly();
    pseudoTop_neutrino_pyReader_.initialiseBlindly();
    pseudoTop_neutrino_pzReader_.initialiseBlindly();

    pseudoTop_jet_energyReader_.initialiseBlindly();
    pseudoTop_jet_pxReader_.initialiseBlindly();
    pseudoTop_jet_pyReader_.initialiseBlindly();
    pseudoTop_jet_pzReader_.initialiseBlindly();
    pseudoTop_jet_pdgIdReader_.initialiseBlindly();
    
    pseudoTop_lepton_energyReader_.initialiseBlindly();
    pseudoTop_lepton_pxReader_.initialiseBlindly();
    pseudoTop_lepton_pyReader_.initialiseBlindly();
    pseudoTop_lepton_pzReader_.initialiseBlindly();
    pseudoTop_lepton_pdgIdReader_.initialiseBlindly();

    // pseudoTop_met_energyReader_.initialiseBlindly();
    // pseudoTop_met_pxReader_.initialiseBlindly();
    // pseudoTop_met_pyReader_.initialiseBlindly();
    // pseudoTop_met_pzReader_.initialiseBlindly();
    pseudoTop_met_exReader_.initialiseBlindly();
    pseudoTop_met_eyReader_.initialiseBlindly();

    tau1_Reader_.initialiseBlindly();
    tau2_Reader_.initialiseBlindly();
    tau3_Reader_.initialiseBlindly();
    tau4_Reader_.initialiseBlindly();
    tau5_Reader_.initialiseBlindly();
    tau6_Reader_.initialiseBlindly();

}

}
