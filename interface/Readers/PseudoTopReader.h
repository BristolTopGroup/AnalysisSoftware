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

    PseudoTopParticlesPointer pseudoTopParticles_;
    ParticleCollection newPseudoTops_;

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
