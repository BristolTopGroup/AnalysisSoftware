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
		shiftedMETReader(),
		met(), //
		usedAlgorithm(METAlgorithm::MET) {

}

METReader::METReader(TChainPointer input, METAlgorithm::value algo) :
		exReader(input, "MET.Ex"), //
		eyReader(input, "MET.Ey"), //
		multiExReader(input, "MET.Ex"), //
		multiEyReader(input, "MET.Ey"), //
		significanceReader(input, "MET.Significance"), //
		shiftedMETReader(input, "MET.METUncertaintiesPt"),
		shiftedMET_Px_Reader(input, "MET.METUncertaintiesPx"),
		shiftedMET_Py_Reader(input, "MET.METUncertaintiesPy"),

		met(), //
		usedAlgorithm(algo) {
	// if (Globals::NTupleVersion < 8 && usedAlgorithm == METAlgorithm::GenMET) {
	// 	multiExReader = VariableReader<MultiDoublePointer>(input, METAlgorithm::prefixes.at(usedAlgorithm) + ".ExTrue");
	// 	multiEyReader = VariableReader<MultiDoublePointer>(input, METAlgorithm::prefixes.at(usedAlgorithm) + ".EyTrue");
	// }

}

METReader::~METReader() {
}

void METReader::initialise() {
	exReader.initialise();
	eyReader.initialise();
	significanceReader.initialise();
	shiftedMETReader.initialise();
	shiftedMET_Px_Reader.initialise();
	shiftedMET_Py_Reader.initialise();

}

void METReader::initialiseBlindly() {
	// if (usedAlgorithm != METAlgorithm::GenMET || Globals::NTupleVersion >= 8) {
		exReader.initialiseBlindly();
		eyReader.initialiseBlindly();
		significanceReader.initialiseBlindly();
		shiftedMETReader.initialiseBlindly();
		shiftedMET_Px_Reader.initialiseBlindly();
		shiftedMET_Py_Reader.initialiseBlindly();

	// } else {
	// 	multiExReader.initialiseBlindly();
	// 	multiEyReader.initialiseBlindly();
	// }

}

const METPointer METReader::getMET(double corrx, double corry) {
	readMET(corrx, corry);
	return met;
}

void METReader::readMET(double corrx, double corry) {
	// if (usedAlgorithm != METAlgorithm::GenMET || Globals::NTupleVersion >= 8) {
		met = METPointer(new MET(exReader.getVariable()+corrx, eyReader.getVariable()+corry));
		met->setSignificance(significanceReader.getVariable());

		std::vector<double> metUncertainties,metUncertainties_Px, metUncertainties_Py;
		for (unsigned int index = 0; index < shiftedMETReader.size(); index++) {
			metUncertainties.push_back( shiftedMETReader.getVariableAt( index ));
			metUncertainties_Px.push_back( shiftedMET_Px_Reader.getVariableAt( index ));
			metUncertainties_Py.push_back( shiftedMET_Py_Reader.getVariableAt( index ));
		}
		met->setMETUncertinaties( metUncertainties );
		met->setMET_Px_Uncertinaties( metUncertainties_Px );
		met->setMET_Py_Uncertinaties( metUncertainties_Py );
	// } else
	// 	met = METPointer(new MET(multiExReader.getVariableAt(0), multiEyReader.getVariableAt(0)));

	met->setUsedAlgorithm(usedAlgorithm);

}
}
