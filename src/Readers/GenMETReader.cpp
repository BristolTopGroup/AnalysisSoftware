/*
 * GenMETReader.cpp
 *
 *  Created on: Jul 9, 2012
 *      Author: phzss
 */

#include "../../interface/Readers/GenMETReader.h"

namespace BAT {

GenMETReader::GenMETReader() :
		exReader(),//
		eyReader(),//
		genMET_() {

}

GenMETReader::GenMETReader(TChainPointer input) :
		exReader(input, "GenMET.ExTrue"), //
		eyReader(input, "GenMET.EyTrue"), //
		genMET_() {

}

GenMETReader::~GenMETReader() {
}

//MC content only
void GenMETReader::initialise() {
	exReader.initialiseBlindly();
	eyReader.initialiseBlindly();
}

const METPointer GenMETReader::getGenMET() {
	readGenMET();
	return genMET_;
}

void GenMETReader::readGenMET() {
	genMET_ = METPointer(new MET(exReader.getVariableAt(0), eyReader.getVariableAt(0)));
}
}
