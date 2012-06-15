/*
 * TopPairEventCandidate.cpp
 *
 *  Created on: 9 Jul 2010
 *      Author: kreczko
 */

#include "../interface/TopPairEventCandidate.h"
#include "../interface/GlobalVariables.h"
#include <iostream>
#include <iomanip>
using namespace std;

namespace BAT {

TopPairEventCandidate::TopPairEventCandidate() :
		Event(), //
		electronFromW(), //
		leptonicBJet(), //
		hadronicBJet(), //
		jet1FromW(), //
		jet2FromW(), //
		neutrino1(), //
		neutrino2(), //
		leptonicW1(), //
		leptonicW2(), //
		hadronicW(), //
		leptonicTop1(), //
		leptonicTop2(), //
		hadronicTop(), //
		selectedNeutrino(0), //
		currentSelectedNeutrino(0), //
		hadronicBIndex(0), //
		leptonicBIndex(0), //
		jet1FromWIndex(0), //
		jet2FromWIndex(0), //
		solutions(), //
		compareSolutions() {
}

TopPairEventCandidate::TopPairEventCandidate(const Event& event) :
		Event(event), //
		electronFromW(), //
		leptonicBJet(), //
		hadronicBJet(), //
		jet1FromW(), //
		jet2FromW(), //
		neutrino1(), //
		neutrino2(), //
		leptonicW1(), //
		leptonicW2(), //
		hadronicW(), //
		leptonicTop1(), //
		leptonicTop2(), //
		hadronicTop(), //
		selectedNeutrino(0), //
		currentSelectedNeutrino(0), //
		hadronicBIndex(0), //
		leptonicBIndex(0), //
		jet1FromWIndex(0), //
		jet2FromWIndex(0), //
		solutions(), //
		compareSolutions() {

}

TopPairEventCandidate::~TopPairEventCandidate() {
}

double TopPairEventCandidate::PtRatio() const {
	return TMath::Log(hadronicTop->pt() / hadronicW->pt());
}


double TopPairEventCandidate::PtTtbarSystem(unsigned short neutrinoSolution) const {
	ParticlePointer combined;
	if (neutrinoSolution == 1)
		combined = ParticlePointer(new Particle(*leptonicTop1 + *hadronicTop));
	else
		combined = ParticlePointer(new Particle(*leptonicTop2 + *hadronicTop));
	return combined->pt();
}

double TopPairEventCandidate::HT(unsigned short jetLimit) const {
	double HT(0);
	unsigned short limit = allJets.size();
	if (limit > jetLimit + 1)
		limit = jetLimit + 1;

	for (unsigned short index = 0; index < limit; ++index)
		HT += allJets.at(index)->pt();

	return HT;
}

double TopPairEventCandidate::HTSystem() const {
	return sumPt() / HT(8);
}

double TopPairEventCandidate::sumPt() const {
	return leptonicBJet->pt() + hadronicBJet->pt() + jet1FromW->pt() + jet2FromW->pt();
}

const ElectronPointer TopPairEventCandidate::getElectronFromWDecay() const {
	return electronFromW;
}

const ParticlePointer TopPairEventCandidate::getNeutrinoFromWDecay() const {
	if (selectedNeutrino == 1)
		return neutrino1;
	else
		return neutrino2;
}

const JetPointer TopPairEventCandidate::getHadronicBJet() const {
	return hadronicBJet;
}

const JetPointer TopPairEventCandidate::getLeptonicBJet() const {
	return leptonicBJet;
}

const JetPointer TopPairEventCandidate::getJet1FromHadronicW() const {
	return jet1FromW;
}

const JetPointer TopPairEventCandidate::getJet2FromHadronicW() const {
	return jet2FromW;
}

const ParticlePointer TopPairEventCandidate::getLeptonicTop() const {
	if (selectedNeutrino == 1)
		return leptonicTop1;
	else
		return leptonicTop2;
}

const ParticlePointer TopPairEventCandidate::getHadronicTop() const {
	return hadronicTop;
}

const ParticlePointer TopPairEventCandidate::getResonance() const {
	return ttbarResonance;
}



double TopPairEventCandidate::mttbar() const {
	return getResonance()->mass();
}

const std::vector<TtbarHypothesisPointer>& TopPairEventCandidate::Solutions() const {
	return solutions;
}

void TopPairEventCandidate::inspectReconstructedEvent() const {
	cout << "run " << runNumber << ", event " << eventNumber << endl;
	cout << "leptonic b jet, goodJet index " << leptonicBIndex << endl;
	EventContentPrinter::printJet(leptonicBJet);

	cout << "electron from W" << endl;
	EventContentPrinter::printElectron(electronFromW);

	cout << "MET" << endl;
	EventContentPrinter::printParticle(MET());
	cout << endl;

	cout << "reconstructed neutrino 1(selected: " << selectedNeutrino << ")" << endl;
	EventContentPrinter::printParticle(neutrino1);
	cout << endl;

	cout << "reconstructed neutrino 2(selected: " << selectedNeutrino << ")" << endl;
	EventContentPrinter::printParticle(neutrino2);
	cout << endl;

	cout << "leptonic W 1 (selected: " << selectedNeutrino << ")" << endl;
	EventContentPrinter::printParticle(leptonicW1);
	cout << endl;

	cout << "leptonic W 2 (selected: " << selectedNeutrino << ")" << endl;
	EventContentPrinter::printParticle(leptonicW2);
	cout << endl;

	cout << "leptonic top 1 (selected: " << selectedNeutrino << ")" << endl;
	EventContentPrinter::printParticle(leptonicTop1);
	cout << endl;

	cout << "leptonic top 2 (selected: " << selectedNeutrino << ")" << endl;
	EventContentPrinter::printParticle(leptonicTop2);
	cout << endl;

	cout << "hadronic b jet, goodJet index " << hadronicBIndex << endl;
	EventContentPrinter::printJet(hadronicBJet);

	cout << "jet1 from W, goodJet index " << jet1FromWIndex << endl;
	EventContentPrinter::printJet(jet1FromW);

	cout << "jet 2 from W, goodJet index " << jet2FromWIndex << endl;
	EventContentPrinter::printJet(jet2FromW);

	cout << "hadronic W" << endl;
	EventContentPrinter::printParticle(hadronicW);
	cout << endl;

	cout << "hadronic top" << endl;
	EventContentPrinter::printParticle(hadronicTop);
	cout << endl;
}

double TopPairEventCandidate::fullHT() const {
	double ht(MET()->pt());

	for (unsigned int index = 0; index < allElectrons.size(); ++index) {
		ht += allElectrons.at(index)->pt();
	}

	for (unsigned int index = 0; index < allMuons.size(); ++index) {
		ht += allMuons.at(index)->pt();
	}

	for (unsigned int index = 0; index < allJets.size(); ++index) {
		ht += allJets.at(index)->pt();
	}
	return ht;
}

double TopPairEventCandidate::transverseWmass(const ElectronPointer electron) const {
	double energySquared = pow(electron->et() + MET()->et(), 2);
	double momentumSquared = pow(electron->px() + MET()->px(), 2) + pow(electron->py() + MET()->py(), 2);
	double tMassSquared = energySquared - momentumSquared;

	if (tMassSquared > 0)
		return sqrt(tMassSquared);
	else
		return -1;
}


}
