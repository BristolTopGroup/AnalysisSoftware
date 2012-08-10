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

namespace BAT {

class MuonReader {
public:
	MuonReader();
	MuonReader(TChainPointer input, MuonAlgorithm::value algo = MuonAlgorithm::Default);
	virtual ~MuonReader();
	const MuonCollection& getMuons();
	void initialise();

private:
	VariableReader<MultiDoublePointer> energyReader;
	VariableReader<MultiDoublePointer> pxReader;
	VariableReader<MultiDoublePointer> pyReader;
	VariableReader<MultiDoublePointer> pzReader;
	VariableReader<MultiIntPointer> chargeReader;

	VariableReader<MultiDoublePointer> d0_BS_Reader;
	VariableReader<MultiDoublePointer> d0_PV_Reader;

	VariableReader<MultiDoublePointer> ecalIsolationReader;
	VariableReader<MultiDoublePointer> hcalIsolationReader;
	VariableReader<MultiDoublePointer> trackerIsolationReader;

	VariableReader<MultiDoublePointer> PFGammaIsolationReader;
	VariableReader<MultiDoublePointer> PFChargedHadronIsolationReader;
	VariableReader<MultiDoublePointer> PFNeutralHadronIsolationReader;

	VariableReader<MultiBoolPointer> isGlobalMuonReader;
	VariableReader<MultiBoolPointer> isTrackerMuonReader;
	VariableReader<MultiDoublePointer> vertex_dist_z;
	VariableReader<MultiDoublePointer> normalisedChi2;
	VariableReader<MultiIntPointer> NumberOfValidHits;
	VariableReader<MultiIntPointer> NumberOfValidMuonHits;
	VariableReader<MultiIntPointer> PixelLayersWithMeasurement;
//	VariableReader<MultiIntPointer> TrackerLayersWithMeasurement;
	VariableReader<MultiIntPointer> NumberOfMatchedStations;
	VariableReader<MultiIntPointer> NumberOfMatches;

	MuonAlgorithm::value algorithm;

	MuonCollection muons;
	void readMuons();
};

}

#endif /* MUONREADER_H_ */
