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

	VariableReader<MultiDoublePointer> PFGammaIsolationReader_DR03_;
	VariableReader<MultiDoublePointer> PFChargedHadronIsolationReader_DR03_;
	VariableReader<MultiDoublePointer> PFNeutralHadronIsolationReader_DR03_;
	VariableReader<MultiDoublePointer> PFPUChargedHadron_Isolation_DR03_;
	VariableReader<MultiDoublePointer> PFGammaIsolationReader_DR04_;
	VariableReader<MultiDoublePointer> PFChargedHadronIsolationReader_DR04_;
	VariableReader<MultiDoublePointer> PFNeutralHadronIsolationReader_DR04_;
	VariableReader<MultiDoublePointer> PFPUChargedHadron_Isolation_DR04_;
	VariableReader<MultiDoublePointer> PFGammaIsolationReader_DR05_;
	VariableReader<MultiDoublePointer> PFChargedHadronIsolationReader_DR05_;
	VariableReader<MultiDoublePointer> PFNeutralHadronIsolationReader_DR05_;
	VariableReader<MultiDoublePointer> PFPUChargedHadron_Isolation_DR05_;

	//new iso
	VariableReader<MultiDoublePointer> PFDeltaBeta_Isolation_DR04_;
	VariableReader<MultiDoublePointer> sumChargedHadronPt04_;
	VariableReader<MultiDoublePointer> sumNeutralHadronPt04_;
	VariableReader<MultiDoublePointer> sumPhotonPt04_;
	VariableReader<MultiDoublePointer> sumPUPt04_;

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
