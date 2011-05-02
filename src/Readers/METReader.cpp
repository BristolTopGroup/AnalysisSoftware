/*
 * METReader.cpp
 *
 *  Created on: 4 Aug 2010
 *      Author: kreczko
 */

#include "../../interface/Readers/METReader.h"

namespace BAT {
//const std::string METReader::algorithmPrefixes[METAlgorithm::NUMBER_OF_METALGORITHMS] = {
//        "mets",
//        "tcmets",
//        "PFMets" };

METReader::METReader() :
    exReader(), eyReader(), met() {

}

METReader::METReader(TChainPointer input, METAlgorithm::value algo) :
    exReader(input, METAlgorithm::prefixes.at(algo) + ".Ex"),
    eyReader(input, METAlgorithm::prefixes.at(algo) + ".Ey") {

}

METReader::~METReader() {
}

void METReader::initialise() {
    exReader.initialise();
    eyReader.initialise();
}

const METPointer METReader::getMET() {
    readMET();
    return met;
}

void METReader::readMET() {
    met = METPointer(new MET(exReader.getVariableAt(0), eyReader.getVariableAt(0)));
}
}
