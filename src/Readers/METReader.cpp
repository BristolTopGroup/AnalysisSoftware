/*
 * METReader.cpp
 *
 *  Created on: 4 Aug 2010
 *      Author: kreczko
 */

#include "../../interface/Readers/METReader.h"

namespace BAT {

METReader::METReader() :
		exReader(),//
		eyReader(),//
		met() {

}

METReader::METReader(TChainPointer input, METAlgorithm::value algo) :
		exReader(input, METAlgorithm::prefixes.at(algo) + ".Ex"), //
		eyReader(input, METAlgorithm::prefixes.at(algo) + ".Ey"), //
		significanceReader(input, METAlgorithm::prefixes.at(algo) + ".Significance") {

}

METReader::~METReader() {
}

void METReader::initialise() {
	exReader.initialise();
	eyReader.initialise();
	significanceReader.initialise();
}

void METReader::initialiseBlindly() {
	exReader.initialiseBlindly();
	eyReader.initialiseBlindly();
	significanceReader.initialiseBlindly();
}

const METPointer METReader::getMET() {
	readMET();
	return met;
}

void METReader::readMET() {
	met = METPointer(new MET(exReader.getVariable(), eyReader.getVariable()));
	met->setSignificance(significanceReader.getVariable());
}
}
