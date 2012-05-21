/*
 * METReader.h
 *
 *  Created on: 4 Aug 2010
 *      Author: kreczko
 */

#ifndef METREADER_H_
#define METREADER_H_
#include "VariableReader.h"
#include "../RecoObjects/MET.h"

namespace BAT {

class METReader {
public:
    METReader();
    METReader(TChainPointer input, METAlgorithm::value algo = METAlgorithm::patMETsPFlow);
    const METPointer getMET();
    virtual ~METReader();
    void initialise();
private:
    VariableReader<double> exReader;
    VariableReader<double> eyReader;
    VariableReader<double> significanceReader;
    METPointer met;
    METAlgorithm::value usedAlgorithm;
    void readMET();
};

}

#endif /* METREADER_H_ */
