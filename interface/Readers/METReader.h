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
	METReader(TChainPointer input, METAlgorithm::value algo = METAlgorithm::MET);
	const METPointer getMET(double corrx, double corry);
	virtual ~METReader();
	void initialise();
	void initialiseBlindly();
private:
	VariableReader<double> exReader;
	VariableReader<double> eyReader;
	//temporarily
	VariableReader<MultiDoublePointer> multiExReader;
	VariableReader<MultiDoublePointer> multiEyReader;
	VariableReader<double> significanceReader;
	VariableReader<MultiDoublePointer> shiftedMETReader;
	VariableReader<MultiDoublePointer> shiftedMET_Px_Reader;
	VariableReader<MultiDoublePointer> shiftedMET_Py_Reader;

	METPointer met;
	METAlgorithm::value usedAlgorithm;
	void readMET(double corrx, double corry);
};

}

#endif /* METREADER_H_ */
