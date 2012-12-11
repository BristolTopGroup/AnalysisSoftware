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
	VariableReader<MultiDoublePointer> ecalIsolationReader_DR04_;
	VariableReader<MultiDoublePointer> hcalIsolationReader_DR04_;
	VariableReader<MultiDoublePointer> trackerIsolationReader_DR04_;

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
	VariableReader<MultiDoublePointer> PFRelativeIsolationRho_DR03_;

	VariableReader<MultiDoublePointer> DirectionalIsolationReader_DR02_;
	VariableReader<MultiDoublePointer> DirectionalIsolationReader_DR03_;
	VariableReader<MultiDoublePointer> DirectionalIsolationReaderWithGaussianFallOff_DR02_;
	VariableReader<MultiDoublePointer> DirectionalIsolationReaderWithGaussianFallOff_DR03_;
	VariableReader<MultiDoublePointer> PFIsolationReaderWithGaussianFallOff_DR02_;
	VariableReader<MultiDoublePointer> PFIsolationReaderWithGaussianFallOff_DR03_;

	VariableReader<MultiDoublePointer> sigmaIEtaIEtaReader;
	VariableReader<MultiDoublePointer> dPhiInReader;
	VariableReader<MultiDoublePointer> dEtaInReader;
	VariableReader<MultiDoublePointer> hadOverEmReader;

	VariableReader<MultiDoublePointer> vertex_dist_z;
	VariableReader<MultiDoublePointer> dist;
	VariableReader<MultiDoublePointer> dCotTheta;
	VariableReader<MultiDoublePointer> mvaTrigV0_, mvaNonTrigV0_;
	VariableReader<MultiBoolPointer> passConversionVeto_;

	ElectronAlgorithm::value algorithm;
	ElectronCollection electrons;
	void readElectrons();

};

}

#endif /* ELECTRONREADER_H_ */
