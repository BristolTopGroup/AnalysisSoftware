/*
 * JetReader.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/Readers/JetReader.h"

namespace BAT {

JetReader::JetReader() :
    energyReader(),
    pxReader(),
    pyReader(),
    pzReader(),
    massReader(),
    chargeReader(),
    emfReader(),
    n90HitsReader(),
    fHPDReader(),
    NODReader(),
    CEFReader(),
    NHFReader(),
    NEFReader(),
    CHFReader(),
    NCHReader(),
    btagSimpleSecondaryVertexReader(),
    btagTrackCountingHighPurityReader(),
    btagTrackCountingHighEfficiencyReader(),
//    genEnergyReader(),
//    genPxReader(),
//    genPyReader(),
//    genPzReader(),
//    genMassReader(),
//    genChargeReader(),
//    genEmfReader(),
    jets(),
//    genJets(),
    usedAlgorithm(JetAlgorithm::Calo_AntiKT_Cone05) {

}
JetReader::JetReader(TChainPointer input, JetAlgorithm::value algo) :
    energyReader(input, JetAlgorithm::prefixes.at(algo) + ".Energy"),
    pxReader(input, JetAlgorithm::prefixes.at(algo) + ".Px"),
    pyReader(input, JetAlgorithm::prefixes.at(algo) + ".Py"),
    pzReader(input, JetAlgorithm::prefixes.at(algo) + ".Pz"),
    massReader(input, JetAlgorithm::prefixes.at(algo) + ".Mass"),
    chargeReader(input, JetAlgorithm::prefixes.at(algo) + ".Charge"),
    emfReader(input, JetAlgorithm::prefixes.at(algo) + ".EMF"),
    n90HitsReader(input, JetAlgorithm::prefixes.at(algo) + ".n90Hits"),
    fHPDReader(input, JetAlgorithm::prefixes.at(algo) + ".fHPD"),
    NODReader(input, JetAlgorithm::prefixes.at(algo) + ".NConstituents"),
    CEFReader(input, JetAlgorithm::prefixes.at(algo) + ".ChargedEmEnergyFraction"),
    NHFReader(input, JetAlgorithm::prefixes.at(algo) + ".NeutralHadronEnergyFraction"),
    NEFReader(input, JetAlgorithm::prefixes.at(algo) + ".NeutralEmEnergyFraction"),
    CHFReader(input, JetAlgorithm::prefixes.at(algo) + ".ChargedHadronEnergyFraction"),
    NCHReader(input, JetAlgorithm::prefixes.at(algo) + ".ChargedMultiplicity"),
    btagSimpleSecondaryVertexReader(input, JetAlgorithm::prefixes.at(algo) + ".SimpleSecondaryVertexHighEffBTag"),
    btagTrackCountingHighPurityReader(input, JetAlgorithm::prefixes.at(algo) + ".TrackCountingHighPurBTag"),
    btagTrackCountingHighEfficiencyReader(input, JetAlgorithm::prefixes.at(algo) + ".TrackCountingHighEffBTag"),
//    genEnergyReader(input, "GenJet.Energy"),
//    genPxReader(input, "GenJet.Px"),
//    genPyReader(input, "GenJet.Py"),
//    genPzReader(input, "GenJet.Pz"),
//    genMassReader(input, "GenJet.Mass"),
//    genChargeReader(input, "GenJet.Charge"),
//    genEmfReader(input, "GenJet.EMF"),
    jets(),
//    genJets(),
    usedAlgorithm(algo) {

}
JetReader::~JetReader() {
}

const JetCollection& JetReader::getJets() {
	if (jets.empty() == false)
		jets.clear();
	readJets();
	return jets;
}

//const JetCollection& JetReader::getGenJets() {
//	if (genJets.empty() == false)
//		genJets.clear();
//	readGenJets();
//	return genJets;
//}

void JetReader::readJets() {
	for (unsigned int jetIndex = 0; jetIndex < energyReader.size(); jetIndex++) {
		float energy = energyReader.getVariableAt(jetIndex);
		float px = pxReader.getVariableAt(jetIndex);
		float py = pyReader.getVariableAt(jetIndex);
		float pz = pzReader.getVariableAt(jetIndex);
		JetPointer jet(new Jet(energy, px, py, pz));
		jet->setUsedAlgorithm(usedAlgorithm);
		jet->setMass(massReader.getVariableAt(jetIndex));
		jet->setCharge(chargeReader.getVariableAt(jetIndex));

		if (usedAlgorithm == JetAlgorithm::Calo_AntiKT_Cone05) {
			jet->setEMF(emfReader.getVariableAt(jetIndex));
			jet->setN90Hits(n90HitsReader.getIntVariableAt(jetIndex));
			jet->setFHPD(fHPDReader.getVariableAt(jetIndex));
		} else {
			jet->setEMF(0);
			jet->setN90Hits(0);
			jet->setFHPD(0);
		}

		jet->setDiscriminatorForBtagType(
				btagSimpleSecondaryVertexReader.getVariableAt(jetIndex),
				BtagAlgorithm::SimpleSecondaryVertexHighEffBTag);
		jet->setDiscriminatorForBtagType(
				btagTrackCountingHighPurityReader.getVariableAt(jetIndex),
				BtagAlgorithm::TrackCountingHighPurBTag);
		jet->setDiscriminatorForBtagType(
				btagTrackCountingHighEfficiencyReader.getVariableAt(jetIndex),
				BtagAlgorithm::TrackCountingHighEffBTag);

		if (usedAlgorithm == JetAlgorithm::CA08PF || usedAlgorithm
				== JetAlgorithm::PF2PAT) {
			jet->setNOD(NODReader.getIntVariableAt(jetIndex));
			jet->setCEF(CEFReader.getVariableAt(jetIndex));
			jet->setNHF(NHFReader.getVariableAt(jetIndex));
			jet->setNEF(NEFReader.getVariableAt(jetIndex));
			jet->setCHF(CHFReader.getVariableAt(jetIndex));
			jet->setNCH(NCHReader.getIntVariableAt(jetIndex));
		}
		jets.push_back(jet);
	}
}

//void JetReader::readGenJets() {
//	for (unsigned int jetIndex = 0; jetIndex < genEnergyReader.size(); jetIndex++) {
//		float energy = genEnergyReader.getVariableAt(jetIndex);
//		float px = genPxReader.getVariableAt(jetIndex);
//		float py = genPyReader.getVariableAt(jetIndex);
//		float pz = genPzReader.getVariableAt(jetIndex);
//		JetPointer jet(new Jet(energy, px, py, pz));
//
//		jet->setUsedAlgorithm(usedAlgorithm);
//		jet->setMass(genMassReader.getVariableAt(jetIndex));
//		jet->setCharge(genChargeReader.getVariableAt(jetIndex));
//		jet->setEMF(genEmfReader.getVariableAt(jetIndex));
//
//		genJets.push_back(jet);
//	}
//}

void JetReader::initialise() {
	energyReader.initialise();
	pxReader.initialise();
	pyReader.initialise();
	pzReader.initialise();
	massReader.initialise();
	chargeReader.initialise();
	if (usedAlgorithm == JetAlgorithm::Calo_AntiKT_Cone05) {
		emfReader.initialise();
		n90HitsReader.initialise();
		fHPDReader.initialise();
	}
	btagSimpleSecondaryVertexReader.initialise();
	btagTrackCountingHighPurityReader.initialise();
	btagTrackCountingHighEfficiencyReader.initialise();
	if (usedAlgorithm == JetAlgorithm::CA08PF || usedAlgorithm
			== JetAlgorithm::PF2PAT) {
		NODReader.initialise();
		CEFReader.initialise();
		NHFReader.initialise();
		NEFReader.initialise();
		CHFReader.initialise();
		NCHReader.initialise();
	}

//	genEnergyReader.initialiseBlindly();
//	genPxReader.initialiseBlindly();
//	genPyReader.initialiseBlindly();
//	genPzReader.initialiseBlindly();
//	genMassReader.initialiseBlindly();
//	genEmfReader.initialiseBlindly();
//	genChargeReader.initialiseBlindly();
}
}
