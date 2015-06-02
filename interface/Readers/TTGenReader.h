/*
 * TTGenReader.h
 *
 *  Created on: 6 Feb 2015
 *      Author: ejclemen
 */

#ifndef TTGenReader_H_
#define TTGenReader_H_
#include "../RecoObjects/TTGenInfo.h"
#include "../RecoObjects/Particle.h"
#include "../RecoObjects/Jet.h"
#include "VariableReader.h"

namespace BAT {

class TTGenReader {
private:
    // Final decay products
    VariableReader<double> quarkPxReader;
    VariableReader<double> quarkPyReader;
    VariableReader<double> quarkPzReader;
    VariableReader<double> quarkEReader;
    VariableReader<int> quarkGenJetIndexReader;
    VariableReader<double> quarkBarPxReader;
    VariableReader<double> quarkBarPyReader;
    VariableReader<double> quarkBarPzReader;
    VariableReader<double> quarkBarEReader;
    VariableReader<int> quarkBarGenJetIndexReader;
    VariableReader<double> leptonicBPxReader;
    VariableReader<double> leptonicBPyReader;
    VariableReader<double> leptonicBPzReader;
    VariableReader<double> leptonicBEReader;
    VariableReader<int> leptonicBGenJetIndexReader;
    VariableReader<double> hadronicBPxReader;
    VariableReader<double> hadronicBPyReader;
    VariableReader<double> hadronicBPzReader;
    VariableReader<double> hadronicBEReader;
    VariableReader<int> hadronicBGenJetIndexReader;
    VariableReader<double> singleleptonPxReader;
    VariableReader<double> singleleptonPyReader;
    VariableReader<double> singleleptonPzReader;
    VariableReader<double> singleleptonEReader;
    VariableReader<double> singleneutrinoPxReader;
    VariableReader<double> singleneutrinoPyReader;
    VariableReader<double> singleneutrinoPzReader;
    VariableReader<double> singleneutrinoEReader;

    // W
    VariableReader<double> leptonicWPxReader;
    VariableReader<double> leptonicWPyReader;
    VariableReader<double> leptonicWPzReader;
    VariableReader<double> leptonicWEReader;

    // Tops
    VariableReader<double> leptonicTopPxReader;
    VariableReader<double> leptonicTopPyReader;
    VariableReader<double> leptonicTopPzReader;
    VariableReader<double> leptonicTopEReader;
    VariableReader<double> hadronicTopPxReader;
    VariableReader<double> hadronicTopPyReader;
    VariableReader<double> hadronicTopPzReader;
    VariableReader<double> hadronicTopEReader;

    TTGenInfoPointer ttGenInfo_;
    void readTTGenInfo();
public:
    TTGenReader();
    TTGenReader(TChainPointer input);
    virtual ~TTGenReader();
    const TTGenInfoPointer& getTTGenInfo();
    void initialise();
};

}

#endif /* TTGenReader_H_ */
