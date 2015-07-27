/*
 * JetReader.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/Readers/JetReader.h"
#include "../../interface/GlobalVariables.h"
#include <iostream>

using namespace std;

namespace BAT {

JetReader::JetReader() : //
		energyReader(), ////
		JECReader(), //
		JECUncReader(),
		L1OffJECReader(),
		L2L3ResJECReader(),
		L2RelJECReader(),
		L3AbsJECReader(),
		pxReader(), ////
		pyReader(), ////
		pzReader(), ////
		energyRawReader(),
		pxRawReader(),
		pyRawReader(),
		pzRawReader(),
		massReader(), ////
		chargeReader(), ////
		matchedGeneratedJetEnergyReader(), ////
		matchedGeneratedJetPxReader(), ////
		matchedGeneratedJetPyReader(), ////
		matchedGeneratedJetPzReader(), ////
		matchedPartonEnergyReader(), ////
		matchedPartonPxReader(), ////
		matchedPartonPyReader(), ////
		matchedPartonPzReader(), ////
		matchedPartonPdgIdReader(), ////
		emfReader(), ////
		n90HitsReader(), ////
		fHPDReader(), ////
		NODReader(), ////
		CEFReader(), ////
		NHFReader(), //
		NEFReader(), //
		CHFReader(), //
		NCHReader(), //
		btagCSVv2Reader(), //
		isBtagReader(), //
		btagSFReader(), //
		btagSFUpReader(), //
		btagSFDownReader(), //
		PartonFlavour(),
		jets(), //
		usedAlgorithm(JetAlgorithm::Calo_AntiKT_Cone05), //
		isRealData() {

}
JetReader::JetReader(TChainPointer input, JetAlgorithm::value algo) :
		energyReader(input, "Jets.Energy"), //
		JECReader(input, "Jets.JEC"), //
		JECUncReader(input, "Jets.JECUnc"), //
		L1OffJECReader(input, "Jets.L1OffJEC"),
		L2L3ResJECReader(input, "Jets.L2L3ResJEC"),
		L2RelJECReader(input, "Jets.L2RelJEC"),
		L3AbsJECReader(input, "Jets.L3AbsJEC"),
		pxReader(input, "Jets.Px"), //
		pyReader(input, "Jets.Py"), //
		pzReader(input, "Jets.Pz"), //
		energyRawReader(input, "Jets.EnergyRAW"), //
		pxRawReader(input, "Jets.PxRAW"), //
		pyRawReader(input, "Jets.PyRAW"), //
		pzRawReader(input, "Jets.PzRAW"), //
		massReader(input, "Jets.Mass"), //
		chargeReader(input, "Jets.Charge"), //
		matchedGeneratedJetEnergyReader(input, "Jets.GenJet.Energy"), //
		matchedGeneratedJetPxReader(input, "Jets.GenJet.Px"), //
		matchedGeneratedJetPyReader(input, "Jets.GenJet.Py"), //
		matchedGeneratedJetPzReader(input, "Jets.GenJet.Pz"), //
		matchedPartonEnergyReader(input, "Jets.GenParton.Energy"), //
		matchedPartonPxReader(input, "Jets.GenParton.Px"), //
		matchedPartonPyReader(input, "Jets.GenParton.Py"), //
		matchedPartonPzReader(input, "Jets.GenParton.Pz"), //
		matchedPartonPdgIdReader(input, "Jets.GenParton.PdgId"), //
		emfReader(input, "Jets.EMF"), //
		n90HitsReader(input, "Jets.n90Hits"), //
		fHPDReader(input, "Jets.fHPD"), //
		NODReader(input, "Jets.NConstituents"), //
		CEFReader(input, "Jets.ChargedEmEnergyFraction"), //
		NHFReader(input, "Jets.NeutralHadronEnergyFraction"), //
		NEFReader(input, "Jets.NeutralEmEnergyFraction"), //
		CHFReader(input, "Jets.ChargedHadronEnergyFraction"), //
		NCHReader(input, "Jets.ChargedMultiplicity"), //
		btagCSVv2Reader(input, "Jets.combinedInclusiveSecondaryVertexV2BJetTags"), //
		isBtagReader(input, "Jets.passesMediumCSV"), //
		btagSFReader(input, "Jets.btagSF"), //
		btagSFUpReader(input, "Jets.btagSFUp"), //
		btagSFDownReader(input, "Jets.btagSFDown"), //
		PartonFlavour(input, "Jets.PartonFlavour"),//
		jets(), //
		usedAlgorithm(algo), //
		isRealData(false) {

}
JetReader::~JetReader() {
}

const JetCollection& JetReader::getJets(bool isRealData) {
	if (jets.empty() == false)
		jets.clear();
	readJets(isRealData);
	return jets;
}

void JetReader::readJets(bool isRealData) {
	for (unsigned int jetIndex = 0; jetIndex < energyReader.size(); jetIndex++) {
		double energy = energyReader.getVariableAt(jetIndex);
		double px = pxReader.getVariableAt(jetIndex);
		double py = pyReader.getVariableAt(jetIndex);
		double pz = pzReader.getVariableAt(jetIndex);

		double energyRaw = energyRawReader.getVariableAt(jetIndex);
		double pxRaw = pxRawReader.getVariableAt(jetIndex);
		double pyRaw = pyRawReader.getVariableAt(jetIndex);
		double pzRaw = pzRawReader.getVariableAt(jetIndex);

		double JEC = 1.0;
		if ( JECReader.size() > jetIndex ) {
			JEC = JECReader.getVariableAt(jetIndex);
		}

		double JECUnc = JECUncReader.getVariableAt(jetIndex);
		JECUnc = 0.02;
		// cout << Globals::JESsystematic << " " << JECUnc << endl;
		//applying JES + or - systematic, 0 by default)
		energy = energy * (1+JECUnc*Globals::JESsystematic);
		px = px * (1+JECUnc*Globals::JESsystematic);
		py = py * (1+JECUnc*Globals::JESsystematic);
		pz = pz * (1+JECUnc*Globals::JESsystematic);

		//make unsmeared jet object pointer
		JetPointer unsmearedJet(new Jet(energy, px, py, pz));
		JetPointer jet(new Jet(energy, px, py, pz));

		//get matched gen jet variables if MC, applyJetSmearing is True and there is a matchedGeneratedJet:
		// if (!isRealData) {
		// 	double matchedGeneratedJetEnergy = matchedGeneratedJetEnergyReader.getVariableAt(jetIndex); //
		// 	if (matchedGeneratedJetEnergy !=0) {
		// 		double matchedGeneratedJetEnergy = matchedGeneratedJetEnergyReader.getVariableAt(jetIndex); //
		// 		double matchedGeneratedJetPx = matchedGeneratedJetPxReader.getVariableAt(jetIndex); //
		// 		double matchedGeneratedJetPy = matchedGeneratedJetPyReader.getVariableAt(jetIndex); //
		// 		double matchedGeneratedJetPz = matchedGeneratedJetPzReader.getVariableAt(jetIndex); //

		// 		//store matched generated jet variables in a matchedGeneratedJet pointer
		// 		JetPointer matchedGeneratedJet(new Jet(matchedGeneratedJetEnergy, matchedGeneratedJetPx, matchedGeneratedJetPy, matchedGeneratedJetPz));
		// 		jet->set_matched_generated_jet(matchedGeneratedJet);
		// 		if ( Globals::applyJetSmearing ) {
		// 			//smear the unsmeared jet
		// 			const ParticlePointer smearedJet(Jet::smear_jet(unsmearedJet, matchedGeneratedJet, Globals::JetSmearingSystematic));

		// 			FourVector smearedJetFourVector(smearedJet->px(), smearedJet->py(), smearedJet->pz(), smearedJet->energy());
		// 			jet->setFourVector(smearedJetFourVector);

		// 			//store the unsmeared jet and the matched generated jet in the jet (i.e.smeared jet) object
		// 			jet->set_unsmeared_jet(unsmearedJet);
		// 		}
		// 	}

		// 	double matchedPartonEnergy = matchedPartonEnergyReader.getVariableAt(jetIndex); //
		// 	if (matchedPartonEnergy !=0) {
		// 		double matchedPartonEnergy = matchedPartonEnergyReader.getVariableAt(jetIndex); //
		// 		double matchedPartonPx = matchedPartonPxReader.getVariableAt(jetIndex); //
		// 		double matchedPartonPy = matchedPartonPyReader.getVariableAt(jetIndex); //
		// 		double matchedPartonPz = matchedPartonPzReader.getVariableAt(jetIndex); //
		// 		double matchedPartonPdgId = matchedPartonPdgIdReader.getIntVariableAt(jetIndex); //

		// 		// Store as mc particle pointer
		// 		MCParticlePointer matchedParton( new MCParticle(matchedPartonEnergy, matchedPartonPx, matchedPartonPy, matchedPartonPz));
		// 		matchedParton->setPdgId( matchedPartonPdgId );
		// 		jet->set_matched_parton( matchedParton );
		// 	}
		// }

		jet->setUsedAlgorithm(usedAlgorithm);
		jet->setMass(massReader.getVariableAt(jetIndex));
		jet->setCharge(chargeReader.getVariableAt(jetIndex));


		jet->setEnergyRaw(energyRaw);
		jet->setPxRaw(pxRaw);
		jet->setPyRaw(pyRaw);
		jet->setPzRaw(pzRaw);

		JetPointer rawJet(new Jet(energyRaw, pxRaw, pyRaw, pzRaw));
		jet->set_raw_jet(rawJet);


		jet->setJEC(JEC);
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
		jet->setDiscriminatorForBtagType(btagCSVv2Reader.getVariableAt(jetIndex),
				BtagAlgorithm::CombinedSecondaryVertexV2);
		jet->setIsBJet(isBtagReader.getBoolVariableAt(jetIndex));

		if ( !isRealData ) {
			jet->setBTagSF( btagSFReader.getVariableAt( jetIndex ) );
			jet->setBTagSFUp( btagSFUpReader.getVariableAt( jetIndex ) );
			jet->setBTagSFDown( btagSFDownReader.getVariableAt( jetIndex ) );
		}

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
	JECReader.initialise();
	JECUncReader.initialise();
	L1OffJECReader.initialise();
	L2L3ResJECReader.initialise();
	L2RelJECReader.initialise();
	L3AbsJECReader.initialise();
	pxReader.initialise();
	pyReader.initialise();
	pzReader.initialise();
	energyRawReader.initialise();
	pxRawReader.initialise();
	pyRawReader.initialise();
	pzRawReader.initialise();
	massReader.initialise();
	chargeReader.initialise();

	// matchedGeneratedJetEnergyReader.initialiseBlindly();
	// matchedGeneratedJetPxReader.initialiseBlindly();
	// matchedGeneratedJetPyReader.initialiseBlindly();
	// matchedGeneratedJetPzReader.initialiseBlindly();

	// matchedPartonEnergyReader.initialise();
	// matchedPartonPxReader.initialise();
	// matchedPartonPyReader.initialise();
	// matchedPartonPzReader.initialise();
	// matchedPartonPdgIdReader.initialise();

	if (usedAlgorithm == JetAlgorithm::Calo_AntiKT_Cone05) {
		emfReader.initialise();
		n90HitsReader.initialise();
		fHPDReader.initialise();
	}

	btagCSVv2Reader.initialise();
	isBtagReader.initialise();
	btagSFReader.initialise();
	btagSFUpReader.initialise();
	btagSFDownReader.initialise();

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
