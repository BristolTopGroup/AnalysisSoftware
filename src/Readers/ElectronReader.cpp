/*
 * ElectronReader.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/Readers/ElectronReader.h"
#include "../../interface/GlobalVariables.h"
#include <iostream>

using namespace std;

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
		etaReader(), //
		gsfTrackEtaReader(), //
		ecalIsolationReader_DR03_(), //
		hcalIsolationReader_DR03_(), //
		trackerIsolationReader_DR03_(), //
		PFRelIso03Reader_(), //
		PFRelIso04Reader_(), //
		PFRelIso03DeltaBetaReader_(), //
		PFRelIso04DeltaBetaReader_(), //
		PFRelIsoWithEAReader_(), //
		hltECALIsoReader_(), //
		hltHCALIsoReader_(), //
		hltTrackerIsoReader_(), //
		sumChargedHadronPt03Reader_(),//
		sumNeutralHadronPt03Reader_(),//
		sumPhotonPt03Reader_(),//
		sumPUPt03Reader_(),//
		sumChargedHadronPt04Reader_(),//
		sumNeutralHadronPt04Reader_(),//
		sumPhotonPt04Reader_(),//
		sumPUPt04Reader_(),//
		sigmaIEtaIEtaReader(), //
		dPhiInReader(), //
		dEtaInReader(), //
		hadOverEmReader(), //
		vertex_dist_z(), //
		dist(), //
		dCotTheta(), //
		passConversionVeto_(), //
		passTightElectron_(), //
		passTightConversionElectron_(), //
		passTightNonIsoElectron_(), //
		algorithm(ElectronAlgorithm::Calo), //
		electrons() {

}

ElectronReader::ElectronReader(TChainPointer input, ElectronAlgorithm::value algo) :
		energyReader(input, "Electrons.Energy"), //
		pxReader(input, "Electrons.Px"), //
		pyReader(input, "Electrons.Py"), //
		pzReader(input, "Electrons.Pz"), //
		chargeReader(input, "Electrons.Charge"), //
		superClusterEtaReader(input, "Electrons.SCEta"), //
		d0_BS_Reader(input, "Electrons.BeamSpotDXY"), //
		d0_PV_Reader(input, "Electrons.PrimaryVertexDXY"), //
		numberOfInnerLayerMissingHitsReader(input, "Electrons.MissingHits"), //
		// etaReader(input, "Electrons.Eta"), //
		// gsfTrackEtaReader(input, "Electrons.gsfTrackEta"), //
		ecalIsolationReader_DR03_(input, "Electrons.EcalIso03"), //
		hcalIsolationReader_DR03_(input, "Electrons.HcalIso03"), //
		trackerIsolationReader_DR03_(input, "Electrons.TrkIso03"), //
		PFRelIso03Reader_(input, "Electrons.PFRelIso03"), //
		PFRelIso04Reader_(input, "Electrons.PFRelIso04"), //
		PFRelIso03DeltaBetaReader_(input, "Electrons.PFRelIso03DeltaBeta"), //
		PFRelIso04DeltaBetaReader_(input, "Electrons.PFRelIso04DeltaBeta"), //
		PFRelIsoWithEAReader_(input, "Electrons.PFRelIsoWithEA"), //
		hltECALIsoReader_(input, "Electrons.hltECALIso"), //
		hltHCALIsoReader_(input, "Electrons.hltHCALIso"), //
		hltTrackerIsoReader_(input, "Electrons.hltTrackerIso"), //
		sumChargedHadronPt03Reader_(input, "Electrons.sumChargedHadronPt03"), //
		sumNeutralHadronPt03Reader_(input, "Electrons.sumNeutralHadronPt03"), //
		sumPhotonPt03Reader_(input, "Electrons.sumPhotonPt03"), //
		sumPUPt03Reader_(input, "Electrons.sumPUPt03"), //
		sumChargedHadronPt04Reader_(input, "Electrons.sumChargedHadronPt04"), //
		sumNeutralHadronPt04Reader_(input, "Electrons.sumNeutralHadronPt04"), //
		sumPhotonPt04Reader_(input, "Electrons.sumPhotonPt04"), //
		sumPUPt04Reader_(input, "Electrons.sumPUPt04"), //
		sigmaIEtaIEtaReader(input, "Electrons.SigmaIEtaIEta"), //
		dPhiInReader(input, "Electrons.DeltaPhiTrkSC"), //
		dEtaInReader(input, "Electrons.DeltaEtaTrkSC"), //
		hadOverEmReader(input, "Electrons.HadronicOverEM"), //
		vertex_dist_z(input, "Electrons.VtxDistZ"), //
		dist(input, "Electrons.Dist"), //
		dCotTheta(input, "Electrons.DCotTheta"), //
		passConversionVeto_(input, "Electrons.passConversionVeto"), //
		passTightElectron_(input, "Electrons.isTightElectron"), //
		passTightConversionElectron_(input, "Electrons.isTightConversionElectron"), //
		passTightNonIsoElectron_(input, "Electrons.isTightNonIsoElectron"), //
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

		// electron->setElectronEta( etaReader.getVariableAt(index) );
		// electron->setGsfTrackEta( gsfTrackEtaReader.getVariableAt(index) );

		electron->setEcalIsolation(ecalIsolationReader_DR03_.getVariableAt(index), 0.3);
		electron->setHcalIsolation(hcalIsolationReader_DR03_.getVariableAt(index), 0.3);
		electron->setTrackerIsolation(trackerIsolationReader_DR03_.getVariableAt(index), 0.3);

		electron->setPFRelIso03(PFRelIso03Reader_.getVariableAt(index));
		electron->setPFRelIso04(PFRelIso04Reader_.getVariableAt(index));
		electron->setPFRelIso03DeltaBeta(PFRelIso03DeltaBetaReader_.getVariableAt(index));
		electron->setPFRelIso04DeltaBeta(PFRelIso04DeltaBetaReader_.getVariableAt(index));
		electron->setPFRelIsoWithEA( PFRelIsoWithEAReader_.getVariableAt(index)); //

		electron->setHltECALIso(hltECALIsoReader_.getVariableAt(index));
		electron->setHltHCALIso(hltHCALIsoReader_.getVariableAt(index));
		electron->setHltTrackerIso(hltTrackerIsoReader_.getVariableAt(index));

		electron->setsumChargedHadronPt03(sumChargedHadronPt03Reader_.getVariableAt(index));
		electron->setsumNeutralHadronPt03(sumNeutralHadronPt03Reader_.getVariableAt(index));
		electron->setsumPhotonPt03(sumPhotonPt03Reader_.getVariableAt(index));
		electron->setsumPUPt03(sumPUPt03Reader_.getVariableAt(index));

		electron->setsumChargedHadronPt04(sumChargedHadronPt04Reader_.getVariableAt(index));
		electron->setsumNeutralHadronPt04(sumNeutralHadronPt04Reader_.getVariableAt(index));
		electron->setsumPhotonPt04(sumPhotonPt04Reader_.getVariableAt(index));
		electron->setsumPUPt04(sumPUPt04Reader_.getVariableAt(index));

		electron->setSuperClusterEta(superClusterEtaReader.getVariableAt(index));
		electron->setSigmaIEtaIEta(sigmaIEtaIEtaReader.getVariableAt(index));
		electron->setDPhiIn(dPhiInReader.getVariableAt(index));
		electron->setDEtaIn(dEtaInReader.getVariableAt(index));
		electron->setHadOverEm(hadOverEmReader.getVariableAt(index));
		electron->setDistToNextTrack(dist.getVariableAt(index));
		electron->setDCotThetaToNextTrack(dCotTheta.getVariableAt(index));

		electron->setIsTightElectron( passTightElectron_.getBoolVariableAt(index));
		electron->setIsTightConversionElectron( passTightConversionElectron_.getBoolVariableAt(index));
		electron->setIsTightNonIsoElectron( passTightNonIsoElectron_.getBoolVariableAt(index));

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

	// etaReader.initialise();
	// gsfTrackEtaReader.initialise();

	ecalIsolationReader_DR03_.initialise();
	hcalIsolationReader_DR03_.initialise();
	trackerIsolationReader_DR03_.initialise();

	PFRelIso03Reader_.initialise();
	PFRelIso04Reader_.initialise();
	PFRelIso03DeltaBetaReader_.initialise();
	PFRelIso04DeltaBetaReader_.initialise();
	PFRelIsoWithEAReader_.initialise();

	hltECALIsoReader_.initialise();
	hltHCALIsoReader_.initialise();
	hltTrackerIsoReader_.initialise();

	sumChargedHadronPt03Reader_.initialise();
	sumNeutralHadronPt03Reader_.initialise();
	sumPhotonPt03Reader_.initialise();
	sumPUPt03Reader_.initialise();

	sumChargedHadronPt04Reader_.initialise();
	sumNeutralHadronPt04Reader_.initialise();
	sumPhotonPt04Reader_.initialise();
	sumPUPt04Reader_.initialise();

	sigmaIEtaIEtaReader.initialise();
	dPhiInReader.initialise();
	dEtaInReader.initialise();
	hadOverEmReader.initialise();

	vertex_dist_z.initialise();
	dist.initialise();
	dCotTheta.initialise();

	passConversionVeto_.initialise();

	passTightElectron_.initialise();
	passTightConversionElectron_.initialise();
	passTightNonIsoElectron_.initialise();
}

}

