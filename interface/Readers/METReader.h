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
//    const static std::string algorithmPrefixes[METAlgorithm::NUMBER_OF_METALGORITHMS];

    METReader();
    METReader(TChainPointer input, METAlgorithm::value algo = METAlgorithm::Calo);
    const METPointer getMET();
    virtual ~METReader();
    void initialise();
private:
    VariableReader<MultiDoublePointer> exReader;
    VariableReader<MultiDoublePointer> eyReader;
    METPointer met;
    METAlgorithm::value usedAlgorithm;
    void readMET();
};

}

#endif /* METREADER_H_ */
