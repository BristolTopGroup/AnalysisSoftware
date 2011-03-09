/*
 * MuonReader.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef MUONREADER_H_
#define MUONREADER_H_
#include "VariableReader.h"
#include "../RecoObjects/Muon.h"
#include "../Constants.h"

namespace BAT {

class MuonReader {
public:
	MuonReader();
	MuonReader(TChainPointer input, MuonAlgorithm::value algo = MuonAlgorithm::Default);
	virtual ~MuonReader();
	const MuonCollection& getMuons();
	void initialise();

private:
//	VariableReader<unsigned int> numberOfMuonsReader;
	VariableReader<MultiDoublePointer> energyReader;
	VariableReader<MultiDoublePointer> pxReader;
	VariableReader<MultiDoublePointer> pyReader;
	VariableReader<MultiDoublePointer> pzReader;

	VariableReader<MultiDoublePointer> ecalIsolationReader;
	VariableReader<MultiDoublePointer> hcalIsolationReader;
	VariableReader<MultiDoublePointer> trackerIsolationReader;

	VariableReader<MultiBoolPointer> isGlobalReader;

	MuonCollection muons;
	void readMuons();
};

}

#endif /* MUONREADER_H_ */
