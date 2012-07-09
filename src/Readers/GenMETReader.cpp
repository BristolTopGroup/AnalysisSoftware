/*
 * GenMETReader.cpp
 *
 *  Created on: Jul 9, 2012
 *      Author: phzss
 */

#include "../../interface/Readers/GenMETReader.h"

namespace BAT {

GenMETReader::GenMETReader() :
		exReader(), eyReader(), GenMet() {

}

GenMETReader::GenMETReader(TChainPointer input) :
		exReader(input, "GenMET.ExTrue"), //
		eyReader(input, "GenMET.EyTrue") //
		{

}

GenMETReader::~GenMETReader() {
}

void GenMETReader::initialise() {
	exReader.initialise();
	eyReader.initialise();
}

void GenMETReader::initialiseBlindly() {
	exReader.initialiseBlindly();
	eyReader.initialiseBlindly();
}

const METPointer GenMETReader::getGenMET() {
	readGenMET();
	return GenMet;
}

void GenMETReader::readGenMET() {
	GenMet = METPointer(new MET(exReader.getVariable(), eyReader.getVariable()));
}
}
