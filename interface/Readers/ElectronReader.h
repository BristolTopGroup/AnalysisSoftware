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
#include <boost/scoped_ptr.hpp>
#include <iostream>

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

	VariableReader<MultiDoublePointer> ecalIsolationReader_DR03_;
	VariableReader<MultiDoublePointer> hcalIsolationReader_DR03_;
	VariableReader<MultiDoublePointer> trackerIsolationReader_DR03_;

	VariableReader<MultiDoublePointer> PFRelIso03Reader_;
	VariableReader<MultiDoublePointer> PFRelIso04Reader_;
	VariableReader<MultiDoublePointer> PFRelIso03DeltaBetaReader_;
	VariableReader<MultiDoublePointer> PFRelIso04DeltaBetaReader_;
	VariableReader<MultiDoublePointer> PFRelIsoWithEAReader_;

	VariableReader<MultiDoublePointer> sumChargedHadronPt03Reader_;
	VariableReader<MultiDoublePointer> sumNeutralHadronPt03Reader_;
	VariableReader<MultiDoublePointer> sumPhotonPt03Reader_;
	VariableReader<MultiDoublePointer> sumPUPt03Reader_;

	VariableReader<MultiDoublePointer> sumChargedHadronPt04Reader_;
	VariableReader<MultiDoublePointer> sumNeutralHadronPt04Reader_;
	VariableReader<MultiDoublePointer> sumPhotonPt04Reader_;
	VariableReader<MultiDoublePointer> sumPUPt04Reader_;

	VariableReader<MultiDoublePointer> sigmaIEtaIEtaReader;
	VariableReader<MultiDoublePointer> dPhiInReader;
	VariableReader<MultiDoublePointer> dEtaInReader;
	VariableReader<MultiDoublePointer> hadOverEmReader;

	VariableReader<MultiDoublePointer> vertex_dist_z;
	VariableReader<MultiDoublePointer> dist;
	VariableReader<MultiDoublePointer> dCotTheta;
	VariableReader<MultiDoublePointer> mvaTrigV0_, mvaNonTrigV0_;
	VariableReader<MultiBoolPointer> passConversionVeto_;

	VariableReader<MultiBoolPointer> passMediumElectron_;
	VariableReader<MultiBoolPointer> passMediumConversionElectron_;
	VariableReader<MultiBoolPointer> passMediumNonIsoElectron_;

	ElectronAlgorithm::value algorithm;
	ElectronCollection electrons;
	void readElectrons();

};

}

#endif /* ELECTRONREADER_H_ */
