/*
 * METReader.cpp
 *
 *  Created on: 4 Aug 2010
 *      Author: kreczko
 */

#include "../../interface/Readers/METReader.h"
#include "../../interface/GlobalVariables.h"

namespace BAT {

METReader::METReader() :
		exReader(), //
		eyReader(), //
		significanceReader(), //
		met(), //
		usedAlgorithm(METAlgorithm::patMETsPFlow) {

}

METReader::METReader(TChainPointer input, METAlgorithm::value algo) :
		exReader(input, METAlgorithm::prefixes.at(algo) + ".Ex"), //
		eyReader(input, METAlgorithm::prefixes.at(algo) + ".Ey"), //
		multiExReader(input, METAlgorithm::prefixes.at(algo) + ".Ex"), //
		multiEyReader(input, METAlgorithm::prefixes.at(algo) + ".Ey"), //
		significanceReader(input, METAlgorithm::prefixes.at(algo) + ".Significance"), //
		met(), //
		usedAlgorithm(algo) {
	if (Globals::NTupleVersion < 8 && usedAlgorithm == METAlgorithm::GenMET) {
		multiExReader = VariableReader<MultiDoublePointer>(input, METAlgorithm::prefixes.at(usedAlgorithm) + ".ExTrue");
		multiEyReader = VariableReader<MultiDoublePointer>(input, METAlgorithm::prefixes.at(usedAlgorithm) + ".EyTrue");
	}

}

METReader::~METReader() {
}

void METReader::initialise() {
	exReader.initialise();
	eyReader.initialise();
	significanceReader.initialise();
}

void METReader::initialiseBlindly() {
	if (usedAlgorithm != METAlgorithm::GenMET || Globals::NTupleVersion >= 8) {
		exReader.initialiseBlindly();
		eyReader.initialiseBlindly();
		significanceReader.initialiseBlindly();
	} else {
		multiExReader.initialiseBlindly();
		multiEyReader.initialiseBlindly();
	}

}

const METPointer METReader::getMET(double corrx, double corry) {
	readMET(corrx, corry);
	return met;
}

void METReader::readMET(double corrx, double corry) {
	if (usedAlgorithm != METAlgorithm::GenMET || Globals::NTupleVersion >= 8) {
		met = METPointer(new MET(exReader.getVariable()+corrx, eyReader.getVariable()+corry));
		met->setSignificance(significanceReader.getVariable());
	} else
		met = METPointer(new MET(multiExReader.getVariableAt(0), multiEyReader.getVariableAt(0)));

	met->setUsedAlgorithm(usedAlgorithm);

}
}
