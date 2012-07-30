/*
 * METReader.cpp
 *
 *  Created on: 4 Aug 2010
 *      Author: kreczko
 */

#include "../../interface/Readers/METReader.h"

namespace BAT {

METReader::METReader() :
		exReader(), //
		eyReader(), //
		significanceReader(), //
		met(), //
		usedAlgorithm(METAlgorithm::patMETsPFlow) {

}

METReader::METReader(TChainPointer input, METAlgorithm::value algo) :
		exReader(input, METAlgorithm::prefixes.at(algo) + ".Ex" + METAlgorithm::suffixes.at(algo)), //
		eyReader(input, METAlgorithm::prefixes.at(algo) + ".Ey" + METAlgorithm::suffixes.at(algo)), //
		multiExReader(input, METAlgorithm::prefixes.at(algo) + ".Ex" + METAlgorithm::suffixes.at(algo)), //
		multiEyReader(input, METAlgorithm::prefixes.at(algo) + ".Ey" + METAlgorithm::suffixes.at(algo)), //
		significanceReader(input, METAlgorithm::prefixes.at(algo) + ".Significance" + METAlgorithm::suffixes.at(algo)), //
		met(), //
		usedAlgorithm(algo) {

}

METReader::~METReader() {
}

void METReader::initialise() {
	exReader.initialise();
	eyReader.initialise();
	significanceReader.initialise();
}

void METReader::initialiseBlindly() {
	if (usedAlgorithm != METAlgorithm::GenMET) {
		exReader.initialiseBlindly();
		eyReader.initialiseBlindly();
		significanceReader.initialiseBlindly();
	} else {
		multiExReader.initialiseBlindly();
		multiEyReader.initialiseBlindly();
	}

}

const METPointer METReader::getMET() {
	readMET();
	return met;
}

void METReader::readMET() {
	if (usedAlgorithm != METAlgorithm::GenMET) {
		met = METPointer(new MET(exReader.getVariable(), eyReader.getVariable()));
		met->setSignificance(significanceReader.getVariable());
	} else
		met = METPointer(new MET(multiExReader.getVariableAt(0), multiEyReader.getVariableAt(0)));

	met->setUsedAlgorithm(usedAlgorithm);

}
}
