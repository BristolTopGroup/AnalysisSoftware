/*
 * GenMETReader.h
 *
 *  Created on: Jul 9, 2012
 *      Author: phzss
 */

#ifndef GENMETREADER_H_
#define GENMETREADER_H_

#include "VariableReader.h"
#include "../RecoObjects/MET.h"

namespace BAT {

class GenMETReader {
public:
    GenMETReader();
    GenMETReader(TChainPointer input);
    const METPointer getGenMET();
    virtual ~GenMETReader();
    void initialise();
    void initialiseBlindly();
private:
    VariableReader<double> exReader;
    VariableReader<double> eyReader;
    METPointer GenMet;
    void readGenMET();
};

}

#endif /* GENMETREADER_H_ */
