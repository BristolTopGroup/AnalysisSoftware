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

	//new iso
	VariableReader<MultiDoublePointer> PFRelIso03Reader_;
	VariableReader<MultiDoublePointer> PFRelIso04Reader_;
	VariableReader<MultiDoublePointer> PFRelIso03DeltaBetaReader_;
	VariableReader<MultiDoublePointer> PFRelIso04DeltaBetaReader_;

	VariableReader<MultiDoublePointer> sumChargedHadronPt03Reader_;
	VariableReader<MultiDoublePointer> sumNeutralHadronPt03Reader_;
	VariableReader<MultiDoublePointer> sumPhotonPt03Reader_;
	VariableReader<MultiDoublePointer> sumPUPt03Reader_;

	VariableReader<MultiDoublePointer> sumChargedHadronPt04Reader_;
	VariableReader<MultiDoublePointer> sumNeutralHadronPt04Reader_;
	VariableReader<MultiDoublePointer> sumPhotonPt04Reader_;
	VariableReader<MultiDoublePointer> sumPUPt04Reader_;

	VariableReader<MultiBoolPointer> isGlobalMuon;
	VariableReader<MultiBoolPointer> isTrackerMuon;
	VariableReader<MultiBoolPointer> isPFMuon;
	VariableReader<MultiDoublePointer> vertex_dist_z;
	VariableReader<MultiDoublePointer> normalisedChi2;
	VariableReader<MultiIntPointer> NumberOfValidHits;
	VariableReader<MultiIntPointer> NumberOfValidMuonHits;
	VariableReader<MultiIntPointer> PixelLayersWithMeasurement;
	VariableReader<MultiIntPointer> TrackerLayersWithMeasurement;
	VariableReader<MultiIntPointer> NumberOfValidPixelHits;
	VariableReader<MultiIntPointer> NumberOfMatchedStations;
	VariableReader<MultiIntPointer> NumberOfMatches;

	MuonAlgorithm::value algorithm;

	MuonCollection muons;
	void readMuons();
};

}

#endif /* MUONREADER_H_ */
