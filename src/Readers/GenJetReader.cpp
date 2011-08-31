/*
 * JetReader.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/Readers/GenJetReader.h"

namespace BAT {

GenJetReader::GenJetReader() :
    genEnergyReader(),
    genPxReader(),
    genPyReader(),
    genPzReader(),
    genMassReader(),
    genChargeReader(),
    genEmfReader(),
    genJets() {

}
GenJetReader::GenJetReader(TChainPointer input) :
    genEnergyReader(input, "GenJet.Energy"),
    genPxReader(input, "GenJet.Px"),
    genPyReader(input, "GenJet.Py"),
    genPzReader(input, "GenJet.Pz"),
    genMassReader(input, "GenJet.Mass"),
    genChargeReader(input, "GenJet.Charge"),
    genEmfReader(input, "GenJet.EMF"),
    genJets() {

}
GenJetReader::~GenJetReader() {
}

const JetCollection& GenJetReader::getGenJets() {
	if (genJets.empty() == false)
		genJets.clear();
	readGenJets();
	return genJets;
}

void GenJetReader::readGenJets() {
	for (unsigned int jetIndex = 0; jetIndex < genEnergyReader.size(); jetIndex++) {
		double energy = genEnergyReader.getVariableAt(jetIndex);
		double px = genPxReader.getVariableAt(jetIndex);
		double py = genPyReader.getVariableAt(jetIndex);
		double pz = genPzReader.getVariableAt(jetIndex);
		JetPointer jet(new Jet(energy, px, py, pz));

		jet->setUsedAlgorithm(JetAlgorithm::Calo_AntiKT_Cone05);
		jet->setMass(genMassReader.getVariableAt(jetIndex));
		jet->setCharge(genChargeReader.getVariableAt(jetIndex));
		jet->setEMF(genEmfReader.getVariableAt(jetIndex));

		genJets.push_back(jet);
	}
}

void GenJetReader::initialise() {
	genEnergyReader.initialiseBlindly();
	genPxReader.initialiseBlindly();
	genPyReader.initialiseBlindly();
	genPzReader.initialiseBlindly();
	genMassReader.initialiseBlindly();
	genEmfReader.initialiseBlindly();
	genChargeReader.initialiseBlindly();
}
}
