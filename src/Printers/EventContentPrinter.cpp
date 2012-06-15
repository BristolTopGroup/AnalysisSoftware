/*
 * EventPrinter.cpp
 *
 *  Created on: 22 Aug 2010
 *      Author: kreczko
 */

#include "../../interface/Printers/EventContentPrinter.h"
#include <iostream>
#include <iomanip>
using namespace std;

namespace BAT {
namespace EventContentPrinter {

extern void printJets(const JetCollection jets) {
	for (unsigned short index = 0; index < jets.size(); ++index) {
		const JetPointer jet = jets.at(index);
		cout << "Jet " << index + 1 << endl;
		printJet(jet);
	}
}

extern void printJet(const JetPointer jet) {
	printParticle(jet);
	cout << "Jet Information" << endl;
	cout << setw(30) << "JECUnc" << endl;
	cout << setw(30) << jet->JECUnc() << endl;
	cout << setw(30) << "ptRaw" << setw(30) << "pxRaw" << setw(30) << "pyRaw" << setw(30) << "pzRaw" << endl;
	cout << setw(30) << sqrt(jet->PxRaw() * jet->PxRaw() + jet->PyRaw() * jet->PyRaw()) << setw(30) << jet->PxRaw()
			<< setw(30) << jet->PyRaw() << setw(30) << jet->PzRaw() << endl;
	cout << setw(30) << "L1OffJEC" << setw(30) << "L2L3ResJEC" << setw(30) << "L2RelJEC" << setw(30) << "L3AbsJEC" << endl;
	cout << setw(30) << jet->L1OffJEC() << setw(30) << jet->L2L3ResJEC() << setw(30) << jet->L2RelJEC() << setw(30) << jet->L3AbsJEC()  << endl;;

	cout << setw(30) << "emf" << setw(30) << "n90Hits" << setw(30) << "fHPD" << setw(30) << "B tag(SSV)" << endl;
	cout << setw(30) << jet->emf() << setw(30) << jet->n90Hits() << setw(30) << jet->fHPD() << setw(30)
			<< jet->isBJet(BtagAlgorithm::SimpleSecondaryVertexHighEfficiency) << endl << endl;
	if (jet->getUsedAlgorithm() == JetAlgorithm::CA08PF || jet->getUsedAlgorithm() == JetAlgorithm::PF2PAT) {
		printPFJetExtra(jet);
	}
}

extern void printPFJetExtra(const JetPointer jet) {
	cout << setw(30) << "NOD" << setw(30) << "CEF" << setw(30) << "NHF" << setw(30) << "NEF" << endl;
	cout << setw(30) << jet->NOD() << setw(30) << jet->CEF() << setw(30) << jet->NHF() << setw(30) << jet->NEF() << endl
			<< endl;

	cout << setw(30) << "CHF" << setw(30) << "NCH" << setw(30) << "" << setw(30) << "" << endl;
	cout << setw(30) << jet->CHF() << setw(30) << jet->NCH() << setw(30) << "" << setw(30) << "" << endl << endl;
}

extern void printElectrons(const ElectronCollection electrons) {
	for (unsigned short index = 0; index < electrons.size(); ++index) {
		const ElectronPointer electron = electrons.at(index);
		cout << "Electron " << index + 1 << " " << string( 110, '=' ) << endl;
		printElectron(electron);

	}
}

extern void printMuons(const MuonCollection muons) {
	for (unsigned short index = 0; index < muons.size(); ++index) {
		const MuonPointer muon = muons.at(index);
		cout << "Muon " << index + 1 << endl;
		printMuon(muon);
	}
}

extern void printElectron(const ElectronPointer electron) {
	printParticle(electron);
	cout << "Electron Information" << endl;
	cout << setw(30) << "VBTF70" << setw(30) << "VBTF95" << setw(30) << "robust loose" << setw(30) << "robust tight"
			<< endl;
	cout << setw(30) << electron->VBTF_WP70_ElectronID() << setw(30) << electron->VBTF_WP95_ElectronID() << setw(30)
			<< electron->RobustLooseID() << setw(30) << electron->RobustTightID() << endl;

	cout << setw(30) << "sigma_{ieta ieta}" << setw(30) << "|Delta phi_{in}|" << setw(30) << "|Delta eta_{in}|"
			<< setw(30) << "HadOverEm" << endl;
	cout << setw(30) << electron->sigmaIEtaIEta() << setw(30) << fabs(electron->dPhiIn()) << setw(30)
			<< fabs(electron->dEtaIn()) << setw(30) << electron->HadOverEm() << endl;

	cout << setw(30) << "" << setw(30) << "rel. iso." << setw(30) << "" << setw(30)
			<< "superClusterEta" << endl;

	cout << setw(30) << "" << setw(30) << electron->relativeIsolation() << setw(30)
			<< "" << setw(30) << electron->superClusterEta() << endl;

	cout << setw(30) << "dCotTheta" << setw(30) << "pf rel. iso." << setw(30) << "dist" << setw(30)
				<< " " << endl;

		cout << setw(30) << electron->dCotThetaToClosestTrack()<< setw(30) << electron->pfRelativeIsolation(0.3) << setw(30)
				<< electron->distToClosestTrack() << setw(30) << "" << endl;

	//    cout << setw(30) << "isTrackerDriven" << setw(30) << "swiss_cross" << setw(30) << "isLoose" << setw(30)
	//            << "isEcalDriven" << endl;
}

extern void printMuon(const MuonPointer muon) {
	printParticle(muon);
	cout << "Muon Information" << endl;
	cout << setw(30) << "is global" << setw(30) << "is tracker" << setw(30) << "PF RelIso DR=0.3" << setw(30) << "----" << endl;
	cout << setw(30) << muon->isGlobal() << setw(30) << "" << setw(30) << muon->pfRelativeIsolation(0.3) << setw(30) << "" << endl;

}

extern void printParticle(const ParticlePointer particle) {
	cout << particle->toString() << endl;
}

extern void printGenParticle(const MCParticlePointer genParticle) {
	printParticle(genParticle);
	cout << "GenParticle Information" << endl;
	cout << setw(30) << "pdgid" << setw(30) << "status" << setw(30) << "mother index" << setw(30) << "energy" << endl;
	cout << setw(30) << genParticle->pdgId() << setw(30) << genParticle->status() << setw(30)
			<< genParticle->motherIndex() << setw(30) << genParticle->energy() << endl;
}

}
}
