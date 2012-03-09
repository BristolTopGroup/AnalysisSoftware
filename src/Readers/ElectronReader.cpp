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
		ecalIsolationReader(), //
		hcalIsolationReader(), //
		trackerIsolationReader(), //
		PFGammaIsolationReader(), //
		PFChargedHadronIsolationReader(), //
		PFNeutralHadronIsolationReader(), //
		DirectionalIsolationReader(), //
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
		ecalIsolationReader(input, ElectronAlgorithm::prefixes.at(algo) + ".EcalIso03"), //
		hcalIsolationReader(input, ElectronAlgorithm::prefixes.at(algo) + ".HcalIso03"), //
		trackerIsolationReader(input, ElectronAlgorithm::prefixes.at(algo) + ".TrkIso03"), //
		PFGammaIsolationReader(input, ElectronAlgorithm::prefixes.at(algo) + ".PFGammaIso03"), //
		PFChargedHadronIsolationReader(input, ElectronAlgorithm::prefixes.at(algo) + ".PfChargedHadronIso03"), //
		PFNeutralHadronIsolationReader(input, ElectronAlgorithm::prefixes.at(algo) + ".PfNeutralHadronIso03"), //
		DirectionalIsolationReader(input, ElectronAlgorithm::prefixes.at(algo) + ".DirectionalPFIso02"), //
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
		electron->setEcalIsolation(ecalIsolationReader.getVariableAt(index));
		electron->setHcalIsolation(hcalIsolationReader.getVariableAt(index));
		electron->setTrackerIsolation(trackerIsolationReader.getVariableAt(index));
		electron->setSuperClusterEta(superClusterEtaReader.getVariableAt(index));
		electron->setSigmaIEtaIEta(sigmaIEtaIEtaReader.getVariableAt(index));
		electron->setDPhiIn(dPhiInReader.getVariableAt(index));
		electron->setDEtaIn(dEtaInReader.getVariableAt(index));
		electron->setHadOverEm(hadOverEmReader.getVariableAt(index));
		electron->setDistToNextTrack(dist.getVariableAt(index));
		electron->setDCotThetaToNextTrack(dCotTheta.getVariableAt(index));

		electron->setCompressedCiCElectronID(CiCElectronIDReader->getIntVariableAt(index));

		if (algorithm == ElectronAlgorithm::ParticleFlow) {
			electron->setPFGammaIsolation(PFGammaIsolationReader.getVariableAt(index));
			electron->setPFChargedHadronIsolation(PFChargedHadronIsolationReader.getVariableAt(index));
			electron->setPFNeutralHadronIsolation(PFNeutralHadronIsolationReader.getVariableAt(index));
			if (Globals::NTupleVersion >= 6)
				electron->setDirectionalIsolation(DirectionalIsolationReader.getVariableAt(index));
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

	ecalIsolationReader.initialise();
	hcalIsolationReader.initialise();
	trackerIsolationReader.initialise();

	sigmaIEtaIEtaReader.initialise();
	dPhiInReader.initialise();
	dEtaInReader.initialise();
	hadOverEmReader.initialise();

	vertex_dist_z.initialise();
	dist.initialise();
	dCotTheta.initialise();
	CiCElectronIDReader->initialise();
	if (algorithm == ElectronAlgorithm::ParticleFlow) {
		PFGammaIsolationReader.initialise();
		PFChargedHadronIsolationReader.initialise();
		PFNeutralHadronIsolationReader.initialise();
		if (Globals::NTupleVersion >= 6)
			DirectionalIsolationReader.initialise();
	}
}

}

