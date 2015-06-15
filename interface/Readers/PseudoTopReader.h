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
    VariableReader<MultiIntPointer> pseudoTop_pdgIdReader_;
    VariableReader<MultiDoublePointer> pseudoTop_energyReader_;
    VariableReader<MultiDoublePointer> pseudoTop_pxReader_;
    VariableReader<MultiDoublePointer> pseudoTop_pyReader_;
    VariableReader<MultiDoublePointer> pseudoTop_pzReader_;

    VariableReader<MultiIntPointer> pseudoTop_neutrino_pdgIdReader_;
    VariableReader<MultiDoublePointer> pseudoTop_neutrino_energyReader_;
    VariableReader<MultiDoublePointer> pseudoTop_neutrino_pxReader_;
    VariableReader<MultiDoublePointer> pseudoTop_neutrino_pyReader_;
    VariableReader<MultiDoublePointer> pseudoTop_neutrino_pzReader_;

    VariableReader<MultiDoublePointer> pseudoTop_jet_energyReader_;
    VariableReader<MultiDoublePointer> pseudoTop_jet_pxReader_;
    VariableReader<MultiDoublePointer> pseudoTop_jet_pyReader_;
    VariableReader<MultiDoublePointer> pseudoTop_jet_pzReader_;
    VariableReader<MultiIntPointer> pseudoTop_jet_pdgIdReader_;

    VariableReader<MultiDoublePointer> pseudoTop_lepton_energyReader_;
    VariableReader<MultiDoublePointer> pseudoTop_lepton_pxReader_;
    VariableReader<MultiDoublePointer> pseudoTop_lepton_pyReader_;
    VariableReader<MultiDoublePointer> pseudoTop_lepton_pzReader_;

    PseudoTopParticlesPointer pseudoTopParticles_;
    MCParticleCollection newPseudoTops_;
    MCParticlePointer newLeptonicW_;
    MCParticlePointer newLepton_;
    ParticleCollection newAllLeptons_;
    JetCollection newJets_;
    MCParticleCollection newPseudoBs_;
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
