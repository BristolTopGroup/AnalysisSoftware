/*
 * ElectronReader.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef ELECTRONREADER_H_
#define ELECTRONREADER_H_
#include "VariableReader.h"
#include "../RecoObjects/Electron.h"

namespace BAT {
class ElectronReader {
public:
	ElectronReader();
	ElectronReader(TChainPointer input, ElectronAlgorithm::value algo = ElectronAlgorithm::Calo);
	virtual ~ElectronReader();
	const ElectronCollection& getElectrons();
	void initialise();
private:
	VariableReader<MultiDoublePointer> energyReader;
	VariableReader<MultiDoublePointer> pxReader;
	VariableReader<MultiDoublePointer> pyReader;
	VariableReader<MultiDoublePointer> pzReader;
	VariableReader<MultiIntPointer> chargeReader;
	VariableReader<MultiDoublePointer> superClusterEtaReader;
	VariableReader<MultiDoublePointer> d0_BS_Reader;
	VariableReader<MultiDoublePointer> d0_PV_Reader;
	VariableReader<MultiIntPointer> numberOfInnerLayerMissingHitsReader;

	VariableReader<MultiDoublePointer> ecalIsolationReader;
	VariableReader<MultiDoublePointer> hcalIsolationReader;
	VariableReader<MultiDoublePointer> trackerIsolationReader;

	VariableReader<MultiDoublePointer> PFGammaIsolationReader;
    VariableReader<MultiDoublePointer> PFChargedHadronIsolationReader;
    VariableReader<MultiDoublePointer> PFNeutralHadronIsolationReader;

	VariableReader<MultiDoublePointer> sigmaIEtaIEtaReader;
	VariableReader<MultiDoublePointer> dPhiInReader;
	VariableReader<MultiDoublePointer> dEtaInReader;
	VariableReader<MultiDoublePointer> hadOverEmReader;

	VariableReader<MultiDoublePointer> sharedFractionInnerHits;
	VariableReader<MultiIntPointer> trackIDReader;
	VariableReader<MultiDoublePointer> track_phi;
	VariableReader<MultiDoublePointer> track_eta;
	VariableReader<MultiDoublePointer> track_pt;
	VariableReader<MultiDoublePointer> track_theta;
	VariableReader<MultiIntPointer> track_charge;

	VariableReader<MultiDoublePointer> vertex_dist_z;
	VariableReader<MultiDoublePointer> dist;
	VariableReader<MultiDoublePointer> dCotTheta;
	ElectronAlgorithm::value algorithm;
	ElectronCollection electrons;
	void readElectrons();

};

}

#endif /* ELECTRONREADER_H_ */
