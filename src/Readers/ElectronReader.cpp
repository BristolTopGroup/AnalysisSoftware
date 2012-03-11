/*
 * ElectronReader.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/Readers/ElectronReader.h"
#include "../../interface/GlobalVariables.h"

namespace BAT {

ElectronReader::ElectronReader() : //
		energyReader(), //
		pxReader(), //
		pyReader(), //
		pzReader(), //
		chargeReader(), //
		superClusterEtaReader(), //
		d0_BS_Reader(), //
		d0_PV_Reader(), //
		numberOfInnerLayerMissingHitsReader(), //
		ecalIsolationReader_DR03_(), //
		hcalIsolationReader_DR03_(), //
		trackerIsolationReader_DR03_(), //
		ecalIsolationReader_DR04_(), //
		hcalIsolationReader_DR04_(), //
		trackerIsolationReader_DR04_(), //
		PFGammaIsolationReader_DR03_(), //
		PFChargedHadronIsolationReader_DR03_(), //
		PFNeutralHadronIsolationReader_DR03_(), //
		PFGammaIsolationReader_DR04_(), //
		PFChargedHadronIsolationReader_DR04_(), //
		PFNeutralHadronIsolationReader_DR04_(), //
		PFGammaIsolationReader_DR05_(), //
		PFChargedHadronIsolationReader_DR05_(), //
		PFNeutralHadronIsolationReader_DR05_(), //
		DirectionalIsolationReader_DR02_(), //
		DirectionalIsolationReader_DR03_(), //
		DirectionalIsolationReaderWithGaussianFallOff_DR02_(), //
		DirectionalIsolationReaderWithGaussianFallOff_DR03_(), //
		PFIsolationReaderWithGaussianFallOff_DR02_(), //
		PFIsolationReaderWithGaussianFallOff_DR03_(), //
		sigmaIEtaIEtaReader(), //
		dPhiInReader(), //
		dEtaInReader(), //
		hadOverEmReader(), //
		vertex_dist_z(), //
		dist(), //
		dCotTheta(), //
		CiCElectronIDReader(), //
		algorithm(ElectronAlgorithm::Calo), //
		electrons() {

}

ElectronReader::ElectronReader(TChainPointer input, ElectronAlgorithm::value algo) :
		energyReader(input, ElectronAlgorithm::prefixes.at(algo) + ".Energy"), //
		pxReader(input, ElectronAlgorithm::prefixes.at(algo) + ".Px"), //
		pyReader(input, ElectronAlgorithm::prefixes.at(algo) + ".Py"), //
		pzReader(input, ElectronAlgorithm::prefixes.at(algo) + ".Pz"), //
		chargeReader(input, ElectronAlgorithm::prefixes.at(algo) + ".Charge"), //
		superClusterEtaReader(input, ElectronAlgorithm::prefixes.at(algo) + ".SCEta"), //
		d0_BS_Reader(input, ElectronAlgorithm::prefixes.at(algo) + ".BeamSpotDXY"), //
		d0_PV_Reader(input, ElectronAlgorithm::prefixes.at(algo) + ".PrimaryVertexDXY"), //
		numberOfInnerLayerMissingHitsReader(input, ElectronAlgorithm::prefixes.at(algo) + ".MissingHits"), //
		ecalIsolationReader_DR03_(input, ElectronAlgorithm::prefixes.at(algo) + ".EcalIso03"), //
		hcalIsolationReader_DR03_(input, ElectronAlgorithm::prefixes.at(algo) + ".HcalIso03"), //
		trackerIsolationReader_DR03_(input, ElectronAlgorithm::prefixes.at(algo) + ".TrkIso03"), //
		ecalIsolationReader_DR04_(input, ElectronAlgorithm::prefixes.at(algo) + ".EcalIso04"), //
		hcalIsolationReader_DR04_(input, ElectronAlgorithm::prefixes.at(algo) + ".HcalIso04"), //
		trackerIsolationReader_DR04_(input, ElectronAlgorithm::prefixes.at(algo) + ".TrkIso04"), //
		PFGammaIsolationReader_DR03_(input, ElectronAlgorithm::prefixes.at(algo) + ".PFGammaIso03"), //
		PFChargedHadronIsolationReader_DR03_(input, ElectronAlgorithm::prefixes.at(algo) + ".PfChargedHadronIso03"), //
		PFNeutralHadronIsolationReader_DR03_(input, ElectronAlgorithm::prefixes.at(algo) + ".PfNeutralHadronIso03"), //
		PFGammaIsolationReader_DR04_(input, ElectronAlgorithm::prefixes.at(algo) + ".PFGammaIso04"), //
		PFChargedHadronIsolationReader_DR04_(input, ElectronAlgorithm::prefixes.at(algo) + ".PfChargedHadronIso04"), //
		PFNeutralHadronIsolationReader_DR04_(input, ElectronAlgorithm::prefixes.at(algo) + ".PfNeutralHadronIso04"), //
		PFGammaIsolationReader_DR05_(input, ElectronAlgorithm::prefixes.at(algo) + ".PFGammaIso05"), //
		PFChargedHadronIsolationReader_DR05_(input, ElectronAlgorithm::prefixes.at(algo) + ".PfChargedHadronIso05"), //
		PFNeutralHadronIsolationReader_DR05_(input, ElectronAlgorithm::prefixes.at(algo) + ".PfNeutralHadronIso05"), //
		DirectionalIsolationReader_DR02_(input, ElectronAlgorithm::prefixes.at(algo) + ".DirectionalPFIso02"), //
		DirectionalIsolationReader_DR03_(input, ElectronAlgorithm::prefixes.at(algo) + ".DirectionalPFIso03"), //
		DirectionalIsolationReaderWithGaussianFallOff_DR02_(input,
				ElectronAlgorithm::prefixes.at(algo) + ".DirectionalPFIso02FallOff"), //
		DirectionalIsolationReaderWithGaussianFallOff_DR03_(input,
				ElectronAlgorithm::prefixes.at(algo) + ".DirectionalPFIso03FallOff"), //
		PFIsolationReaderWithGaussianFallOff_DR02_(input, ElectronAlgorithm::prefixes.at(algo) + ".PfRelIso02FallOff"), //
		PFIsolationReaderWithGaussianFallOff_DR03_(input, ElectronAlgorithm::prefixes.at(algo) + ".PfRelIso03FallOff"), //
		sigmaIEtaIEtaReader(input, ElectronAlgorithm::prefixes.at(algo) + ".SigmaIEtaIEta"), //
		dPhiInReader(input, ElectronAlgorithm::prefixes.at(algo) + ".DeltaPhiTrkSC"), //
		dEtaInReader(input, ElectronAlgorithm::prefixes.at(algo) + ".DeltaEtaTrkSC"), //
		hadOverEmReader(input, ElectronAlgorithm::prefixes.at(algo) + ".HadronicOverEM"), //
		vertex_dist_z(input, ElectronAlgorithm::prefixes.at(algo) + ".VtxDistZ"), //
		dist(input, ElectronAlgorithm::prefixes.at(algo) + ".Dist"), //
		dCotTheta(input, ElectronAlgorithm::prefixes.at(algo) + ".DCotTheta"), //
		CiCElectronIDReader(
				new VariableReader<MultiIntPointer>(input, ElectronAlgorithm::prefixes.at(algo) + ".PassID")), //
		algorithm(algo), //
		electrons() {

}

ElectronReader::~ElectronReader() {
}

const ElectronCollection& ElectronReader::getElectrons() {
	if (electrons.empty() == false)
		electrons.clear();
	readElectrons();
	return electrons;
}

void ElectronReader::readElectrons() {

	for (unsigned int index = 0; index < energyReader.size(); index++) {
		double energy = energyReader.getVariableAt(index);
		double px = pxReader.getVariableAt(index);
		double py = pyReader.getVariableAt(index);
		double pz = pzReader.getVariableAt(index);
		ElectronPointer electron(new Electron(energy, px, py, pz));
		electron->setUsedAlgorithm(algorithm);
		electron->setCharge(chargeReader.getIntVariableAt(index));
		if (d0_BS_Reader.doesVariableExist() && algorithm == ElectronAlgorithm::Calo)
			electron->setD0_wrtBeamSpot(d0_BS_Reader.getVariableAt(index));
		electron->setD0(d0_PV_Reader.getVariableAt(index));
		electron->setZDistanceToPrimaryVertex(vertex_dist_z.getVariableAt(index));
		electron->setNumberOfMissingInnerLayerHits(numberOfInnerLayerMissingHitsReader.getIntVariableAt(index));

		electron->setEcalIsolation(ecalIsolationReader_DR03_.getVariableAt(index), 0.3);
		electron->setHcalIsolation(hcalIsolationReader_DR03_.getVariableAt(index), 0.3);
		electron->setTrackerIsolation(trackerIsolationReader_DR03_.getVariableAt(index), 0.3);
		electron->setEcalIsolation(ecalIsolationReader_DR04_.getVariableAt(index), 0.4);
		electron->setHcalIsolation(hcalIsolationReader_DR04_.getVariableAt(index), 0.4);
		electron->setTrackerIsolation(trackerIsolationReader_DR04_.getVariableAt(index), 0.4);

		electron->setSuperClusterEta(superClusterEtaReader.getVariableAt(index));
		electron->setSigmaIEtaIEta(sigmaIEtaIEtaReader.getVariableAt(index));
		electron->setDPhiIn(dPhiInReader.getVariableAt(index));
		electron->setDEtaIn(dEtaInReader.getVariableAt(index));
		electron->setHadOverEm(hadOverEmReader.getVariableAt(index));
		electron->setDistToNextTrack(dist.getVariableAt(index));
		electron->setDCotThetaToNextTrack(dCotTheta.getVariableAt(index));

		electron->setCompressedCiCElectronID(CiCElectronIDReader->getIntVariableAt(index));

		if (algorithm == ElectronAlgorithm::ParticleFlow) {
			electron->setPFGammaIsolation(PFGammaIsolationReader_DR03_.getVariableAt(index), 0.3);
			electron->setPFChargedHadronIsolation(PFChargedHadronIsolationReader_DR03_.getVariableAt(index), 0.3);
			electron->setPFNeutralHadronIsolation(PFNeutralHadronIsolationReader_DR03_.getVariableAt(index), 0.3);
			electron->setPFGammaIsolation(PFGammaIsolationReader_DR04_.getVariableAt(index), 0.4);
			electron->setPFChargedHadronIsolation(PFChargedHadronIsolationReader_DR04_.getVariableAt(index), 0.4);
			electron->setPFNeutralHadronIsolation(PFNeutralHadronIsolationReader_DR04_.getVariableAt(index), 0.4);
			electron->setPFGammaIsolation(PFGammaIsolationReader_DR05_.getVariableAt(index), 0.5);
			electron->setPFChargedHadronIsolation(PFChargedHadronIsolationReader_DR05_.getVariableAt(index), 0.5);
			electron->setPFNeutralHadronIsolation(PFNeutralHadronIsolationReader_DR05_.getVariableAt(index), 0.5);
			if (Globals::NTupleVersion >= 6) {
				electron->setDirectionalIsolation(DirectionalIsolationReader_DR02_.getVariableAt(index), 0.2);
				electron->setDirectionalIsolation(DirectionalIsolationReader_DR03_.getVariableAt(index), 0.3);
				electron->setDirectionalIsolationWithGaussianFallOff(
						DirectionalIsolationReaderWithGaussianFallOff_DR02_.getVariableAt(index), 0.2);
				electron->setDirectionalIsolationWithGaussianFallOff(
						DirectionalIsolationReaderWithGaussianFallOff_DR03_.getVariableAt(index), 0.3);
				electron->setPFIsolationWithGaussianFallOff(
						PFIsolationReaderWithGaussianFallOff_DR02_.getVariableAt(index), 0.2);
				electron->setPFIsolationWithGaussianFallOff(
						PFIsolationReaderWithGaussianFallOff_DR03_.getVariableAt(index), 0.3);
			}

		}
		electrons.push_back(electron);
	}
}

void ElectronReader::initialise() {
	energyReader.initialise();
	pxReader.initialise();
	pyReader.initialise();
	pzReader.initialise();
	chargeReader.initialise();
	superClusterEtaReader.initialise();

	if (d0_BS_Reader.doesVariableExist() && algorithm == ElectronAlgorithm::Calo)
		d0_BS_Reader.initialise();
	d0_PV_Reader.initialise();
	numberOfInnerLayerMissingHitsReader.initialise();

	ecalIsolationReader_DR03_.initialise();
	hcalIsolationReader_DR03_.initialise();
	trackerIsolationReader_DR03_.initialise();
	ecalIsolationReader_DR04_.initialise();
	hcalIsolationReader_DR04_.initialise();
	trackerIsolationReader_DR04_.initialise();

	sigmaIEtaIEtaReader.initialise();
	dPhiInReader.initialise();
	dEtaInReader.initialise();
	hadOverEmReader.initialise();

	vertex_dist_z.initialise();
	dist.initialise();
	dCotTheta.initialise();
	CiCElectronIDReader->initialise();
	if (algorithm == ElectronAlgorithm::ParticleFlow) {
		PFGammaIsolationReader_DR03_.initialise();
		PFChargedHadronIsolationReader_DR03_.initialise();
		PFNeutralHadronIsolationReader_DR03_.initialise();
		PFGammaIsolationReader_DR04_.initialise();
		PFChargedHadronIsolationReader_DR04_.initialise();
		PFNeutralHadronIsolationReader_DR04_.initialise();
		PFGammaIsolationReader_DR05_.initialise();
		PFChargedHadronIsolationReader_DR05_.initialise();
		PFNeutralHadronIsolationReader_DR05_.initialise();
		if (Globals::NTupleVersion >= 6) {
			DirectionalIsolationReader_DR02_.initialise();
			DirectionalIsolationReader_DR03_.initialise();
			DirectionalIsolationReaderWithGaussianFallOff_DR02_.initialise();
			DirectionalIsolationReaderWithGaussianFallOff_DR03_.initialise();
			PFIsolationReaderWithGaussianFallOff_DR02_.initialise();
			PFIsolationReaderWithGaussianFallOff_DR03_.initialise();
		}

	}
}

}

