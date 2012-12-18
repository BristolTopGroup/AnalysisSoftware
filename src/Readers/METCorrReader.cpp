/*
 * METCorrReader.cpp
 *
 *  Created on: Nov 15, 2012
 *      Author: phzss
 */

#include "../../interface/Readers/METCorrReader.h"
#include "../../interface/GlobalVariables.h"

namespace BAT {

METCorrReader::METCorrReader() :
		corrxReader(), //
		corryReader(), //
		corrx(), //
		corry(), //
		usedCorrections(METCorrections::pfMetSysShiftCorrections) {

}

METCorrReader::METCorrReader(TChainPointer input, METCorrections::value correction) :
				corrxReader(input, METCorrections::prefixes.at(correction) + ".x"), //
				corryReader(input, METCorrections::prefixes.at(correction) + ".y"), //
				corrx(), //
				corry(), //
				usedCorrections(METCorrections::pfMetSysShiftCorrections) {

}

METCorrReader::~METCorrReader() {
}

void METCorrReader::initialise() {
	corrxReader.initialise();
	corryReader.initialise();
}

void METCorrReader::initialiseBlindly() {
	corrxReader.initialiseBlindly();
	corryReader.initialiseBlindly();
}

const double METCorrReader::getXcorrection() {
	return corrx;
}

const double METCorrReader::getYcorrection() {
	return corry;
}

void METCorrReader::readMETCorrections() {
	if (Globals::NTupleVersion > 8) {
		corrx = corrxReader.getVariable();
		corry = corryReader.getVariable();
	} else {
		corrx = 0;
		corry = 0;
	}

}
}
