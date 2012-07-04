/*
 * JetReader.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/Readers/JetReader.h"
#include "../../interface/GlobalVariables.h"

namespace BAT {

JetReader::JetReader() : //
		energyReader(), ////
		JECUncReader(),
		L1OffJECReader(),
		L2L3ResJECReader(),
		L2RelJECReader(),
		L3AbsJECReader(),
		pxReader(), ////
		pyReader(), ////
		pzReader(), ////
		pxRawReader(),
		pyRawReader(),
		pzRawReader(),
		massReader(), ////
		chargeReader(), ////
		emfReader(), ////
		n90HitsReader(), ////
		fHPDReader(), ////
		NODReader(), ////
		CEFReader(), ////
		NHFReader(), //
		NEFReader(), //
		CHFReader(), //
		NCHReader(), //
		btagCombinedSecondaryVertexReader(), //
		btagCombinedSecondaryVertexMVAReader(), //
		btagJetBProbabilityReader(), //
		btagJetProbabilityReader(), //
		btagSimpleSecondaryVertexHEReader(), //
		btagSimpleSecondaryVertexHPReader(), //
		btagSoftMuonReader(), //
		btagSoftMuonByIP3dReader(), //
		btagSoftMuonByPtReader(), //
		btagTrackCountingHighPurityReader(), //
		btagTrackCountingHighEfficiencyReader(), //
		PartonFlavour(),
		jets(), //
		usedAlgorithm(JetAlgorithm::Calo_AntiKT_Cone05) {

}
JetReader::JetReader(TChainPointer input, JetAlgorithm::value algo) :
		energyReader(input, JetAlgorithm::prefixes.at(algo) + ".Energy"), //
		JECUncReader(input, JetAlgorithm::prefixes.at(algo) + ".JECUnc"), //
		L1OffJECReader(input, JetAlgorithm::prefixes.at(algo) + ".L1OffJEC"),
		L2L3ResJECReader(input, JetAlgorithm::prefixes.at(algo) + ".L2L3ResJEC"),
		L2RelJECReader(input, JetAlgorithm::prefixes.at(algo) + ".L2RelJEC"),
		L3AbsJECReader(input, JetAlgorithm::prefixes.at(algo) + ".L3AbsJEC"),
		pxReader(input, JetAlgorithm::prefixes.at(algo) + ".Px"), //
		pyReader(input, JetAlgorithm::prefixes.at(algo) + ".Py"), //
		pzReader(input, JetAlgorithm::prefixes.at(algo) + ".Pz"), //
		pxRawReader(input, JetAlgorithm::prefixes.at(algo) + ".PxRAW"), //
		pyRawReader(input, JetAlgorithm::prefixes.at(algo) + ".PyRAW"), //
		pzRawReader(input, JetAlgorithm::prefixes.at(algo) + ".PzRAW"), //
		massReader(input, JetAlgorithm::prefixes.at(algo) + ".Mass"), //
		chargeReader(input, JetAlgorithm::prefixes.at(algo) + ".Charge"), //
		emfReader(input, JetAlgorithm::prefixes.at(algo) + ".EMF"), //
		n90HitsReader(input, JetAlgorithm::prefixes.at(algo) + ".n90Hits"), //
		fHPDReader(input, JetAlgorithm::prefixes.at(algo) + ".fHPD"), //
		NODReader(input, JetAlgorithm::prefixes.at(algo) + ".NConstituents"), //
		CEFReader(input, JetAlgorithm::prefixes.at(algo) + ".ChargedEmEnergyFraction"), //
		NHFReader(input, JetAlgorithm::prefixes.at(algo) + ".NeutralHadronEnergyFraction"), //
		NEFReader(input, JetAlgorithm::prefixes.at(algo) + ".NeutralEmEnergyFraction"), //
		CHFReader(input, JetAlgorithm::prefixes.at(algo) + ".ChargedHadronEnergyFraction"), //
		NCHReader(input, JetAlgorithm::prefixes.at(algo) + ".ChargedMultiplicity"), //
		btagCombinedSecondaryVertexReader(input, JetAlgorithm::prefixes.at(algo) + ".CombinedSecondaryVertexBJetTag"), //
		btagCombinedSecondaryVertexMVAReader(input,
				JetAlgorithm::prefixes.at(algo) + ".CombinedSecondaryVertexMVABJetTag"), //
		btagJetBProbabilityReader(input, JetAlgorithm::prefixes.at(algo) + ".JetBProbabilityBTag"), //
		btagJetProbabilityReader(input, JetAlgorithm::prefixes.at(algo) + ".JetProbabilityBTag"), //
		btagSimpleSecondaryVertexHEReader(input, JetAlgorithm::prefixes.at(algo) + ".SimpleSecondaryVertexHighEffBTag"), //
		btagSimpleSecondaryVertexHPReader(input, JetAlgorithm::prefixes.at(algo) + ".SimpleSecondaryVertexHighPurBTag"), //
		btagSoftMuonReader(input, JetAlgorithm::prefixes.at(algo) + ".SoftMuonBJetTag"), //
		btagSoftMuonByIP3dReader(input, JetAlgorithm::prefixes.at(algo) + ".SoftMuonByIP3dBJetTag"), //
		btagSoftMuonByPtReader(input, JetAlgorithm::prefixes.at(algo) + ".SoftMuonByPtBJetTag"), //
		btagTrackCountingHighPurityReader(input, JetAlgorithm::prefixes.at(algo) + ".TrackCountingHighPurBTag"), //
		btagTrackCountingHighEfficiencyReader(input, JetAlgorithm::prefixes.at(algo) + ".TrackCountingHighEffBTag"), //
		PartonFlavour(input, JetAlgorithm::prefixes.at(algo) + ".PartonFlavour"),//
		jets(), //
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

void JetReader::readJets() {
	for (unsigned int jetIndex = 0; jetIndex < energyReader.size(); jetIndex++) {
		double energy = energyReader.getVariableAt(jetIndex);
		double px = pxReader.getVariableAt(jetIndex);
		double py = pyReader.getVariableAt(jetIndex);
		double pz = pzReader.getVariableAt(jetIndex);
		double JECUnc = JECUncReader.getVariableAt(jetIndex);

		//applying JES + or - systematic, 0 by default)
		energy = energy * (1+JECUnc*Globals::JESsystematic);
		px = px * (1+JECUnc*Globals::JESsystematic);
		py = py * (1+JECUnc*Globals::JESsystematic);
		pz = pz * (1+JECUnc*Globals::JESsystematic);

		JetPointer jet(new Jet(energy, px, py, pz));
		jet->setUsedAlgorithm(usedAlgorithm);
		jet->setMass(massReader.getVariableAt(jetIndex));
		jet->setCharge(chargeReader.getVariableAt(jetIndex));
		jet->setPxRaw(pxRawReader.getVariableAt(jetIndex));
		jet->setPyRaw(pyRawReader.getVariableAt(jetIndex));
		jet->setPzRaw(pzRawReader.getVariableAt(jetIndex));
		jet->setJECUnc(JECUncReader.getVariableAt(jetIndex));
		jet->setL1OffJEC(L1OffJECReader.getVariableAt(jetIndex));
		jet->setL2L3ResJEC(L2L3ResJECReader.getVariableAt(jetIndex));
		jet->setL2RelJEC(L2RelJECReader.getVariableAt(jetIndex));
		jet->setL3AbsJEC(L3AbsJECReader.getVariableAt(jetIndex));

		if (usedAlgorithm == JetAlgorithm::Calo_AntiKT_Cone05) {
			jet->setEMF(emfReader.getVariableAt(jetIndex));
			jet->setN90Hits(n90HitsReader.getIntVariableAt(jetIndex));
			jet->setFHPD(fHPDReader.getVariableAt(jetIndex));
		} else {
			jet->setEMF(0);
			jet->setN90Hits(0);
			jet->setFHPD(0);
		}

		//set b-tagging
		//combined secondary vertex
		jet->setDiscriminatorForBtagType(btagCombinedSecondaryVertexReader.getVariableAt(jetIndex),
				BtagAlgorithm::CombinedSecondaryVertex);
		jet->setDiscriminatorForBtagType(btagCombinedSecondaryVertexMVAReader.getVariableAt(jetIndex),
				BtagAlgorithm::CombinedSecondaryVertexMVA);
		//jet (b) probability
		jet->setDiscriminatorForBtagType(btagJetBProbabilityReader.getVariableAt(jetIndex),
				BtagAlgorithm::JetBProbability);
		jet->setDiscriminatorForBtagType(btagJetProbabilityReader.getVariableAt(jetIndex),
				BtagAlgorithm::JetProbability);
		//simple secondary vertex
		jet->setDiscriminatorForBtagType(btagSimpleSecondaryVertexHEReader.getVariableAt(jetIndex),
				BtagAlgorithm::SimpleSecondaryVertexHighEfficiency);
		jet->setDiscriminatorForBtagType(btagSimpleSecondaryVertexHPReader.getVariableAt(jetIndex),
				BtagAlgorithm::SimpleSecondaryVertexHighPurity);
		//soft muon
		jet->setDiscriminatorForBtagType(btagSoftMuonReader.getVariableAt(jetIndex), BtagAlgorithm::SoftMuon);
		jet->setDiscriminatorForBtagType(btagSoftMuonByIP3dReader.getVariableAt(jetIndex),
				BtagAlgorithm::SoftMuonByIP3d);
		jet->setDiscriminatorForBtagType(btagSoftMuonByPtReader.getVariableAt(jetIndex), BtagAlgorithm::SoftMuonByPt);
		//track counting
		jet->setDiscriminatorForBtagType(btagTrackCountingHighPurityReader.getVariableAt(jetIndex),
				BtagAlgorithm::TrackCountingHighPurity);
		jet->setDiscriminatorForBtagType(btagTrackCountingHighEfficiencyReader.getVariableAt(jetIndex),
				BtagAlgorithm::TrackCountingHighEfficiency);

		//parton flavour
		jet->setPartonFlavour(PartonFlavour.getIntVariableAt(jetIndex));

		if (usedAlgorithm == JetAlgorithm::CA08PF || usedAlgorithm == JetAlgorithm::PF2PAT) {
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

void JetReader::initialise() {
	energyReader.initialise();
	JECUncReader.initialise();
	L1OffJECReader.initialise();
	L2L3ResJECReader.initialise();
	L2RelJECReader.initialise();
	L3AbsJECReader.initialise();
	pxReader.initialise();
	pyReader.initialise();
	pzReader.initialise();
	pxRawReader.initialise();
	pyRawReader.initialise();
	pzRawReader.initialise();
	massReader.initialise();
	chargeReader.initialise();
	if (usedAlgorithm == JetAlgorithm::Calo_AntiKT_Cone05) {
		emfReader.initialise();
		n90HitsReader.initialise();
		fHPDReader.initialise();
	}

	btagCombinedSecondaryVertexReader.initialise();
	btagCombinedSecondaryVertexMVAReader.initialise();
	btagJetBProbabilityReader.initialise();
	btagJetProbabilityReader.initialise();
	btagSimpleSecondaryVertexHEReader.initialise();
	btagSimpleSecondaryVertexHPReader.initialise();
	btagSoftMuonReader.initialise();
	btagSoftMuonByIP3dReader.initialise();
	btagSoftMuonByPtReader.initialise();
	btagTrackCountingHighPurityReader.initialise();
	btagTrackCountingHighEfficiencyReader.initialise();
	PartonFlavour.initialise();
	if (usedAlgorithm == JetAlgorithm::CA08PF || usedAlgorithm == JetAlgorithm::PF2PAT) {
		NODReader.initialise();
		CEFReader.initialise();
		NHFReader.initialise();
		NEFReader.initialise();
		CHFReader.initialise();
		NCHReader.initialise();
	}
}
}
