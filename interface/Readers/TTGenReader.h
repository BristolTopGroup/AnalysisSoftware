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
