/*
 * METCorrReader.h
 *
 *  Created on: Nov 15, 2012
 *      Author: phzss
 */

#ifndef METCORRREADER_H_
#define METCORRREADER_H_

#include "VariableReader.h"
#include "../RecoObjects/MET.h"

namespace BAT {

class METCorrReader {
public:
	METCorrReader();
	METCorrReader(TChainPointer input, METCorrections::value correction);
	const double getXcorrection();
	const double getYcorrection();
	virtual ~METCorrReader();
	void initialise();
	void initialiseBlindly();
	void readMETCorrections();
private:
	VariableReader<double> corrxReader;
	VariableReader<double> corryReader;

	double corrx, corry;
	METCorrections::value usedCorrections;
};

}

#endif /* METCORRREADER_H_ */
