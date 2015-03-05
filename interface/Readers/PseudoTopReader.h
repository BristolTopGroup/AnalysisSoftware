/*
 * PseudoTopReader.h
 *
 *  Created on: Feb 23, 2015
 *      Author: ejclemen
 */

#ifndef PSEUDOTOPREADER_H_
#define PSEUDOTOPREADER_H_
#include "../RecoObjects/PseudoTopParticles.h"
#include "VariableReader.h"

#include <iostream>

namespace BAT {

class PseudoTopReader {
private:
    VariableReader<MultiIntPointer> pseudoTop_pdgIdReader;
    VariableReader<MultiDoublePointer> pseudoTop_energyReader;
    VariableReader<MultiDoublePointer> pseudoTop_pxReader;
    VariableReader<MultiDoublePointer> pseudoTop_pyReader;
    VariableReader<MultiDoublePointer> pseudoTop_pzReader;

    VariableReader<MultiIntPointer> pseudoTop_neutrino_pdgIdReader;
    VariableReader<MultiDoublePointer> pseudoTop_neutrino_energyReader;
    VariableReader<MultiDoublePointer> pseudoTop_neutrino_pxReader;
    VariableReader<MultiDoublePointer> pseudoTop_neutrino_pyReader;
    VariableReader<MultiDoublePointer> pseudoTop_neutrino_pzReader;

    VariableReader<MultiDoublePointer> pseudoTop_jet_energyReader;
    VariableReader<MultiDoublePointer> pseudoTop_jet_pxReader;
    VariableReader<MultiDoublePointer> pseudoTop_jet_pyReader;
    VariableReader<MultiDoublePointer> pseudoTop_jet_pzReader;

    VariableReader<MultiDoublePointer> pseudoTop_lepton_energyReader;
    VariableReader<MultiDoublePointer> pseudoTop_lepton_pxReader;
    VariableReader<MultiDoublePointer> pseudoTop_lepton_pyReader;
    VariableReader<MultiDoublePointer> pseudoTop_lepton_pzReader;    

    PseudoTopParticlesPointer pseudoTopParticles_;
    ParticleCollection newPseudoTops_;
    ParticlePointer newLeptonicW_;
    MCParticlePointer newLepton_;
    ParticleCollection newAllLeptons_;
    JetCollection newJets_;
    ParticleCollection newPseudoBs_;
    ParticlePointer newNeutrino_;
    ParticlePointer newPseudoMET_;

    bool isSemiLeptonic_;

    void readPseudoTopParticles();
public:
    PseudoTopReader();
    PseudoTopReader(TChainPointer input);
    virtual ~PseudoTopReader();
    const PseudoTopParticlesPointer& getPseudoTopParticles();
    void initialise();
};

}

#endif /* PSEUDOTOPREADER_H_ */
