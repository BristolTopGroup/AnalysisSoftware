/*
 * JetReader.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef JETREADER_H_
#define JETREADER_H_
#include "VariableReader.h"
#include "../RecoObjects/Jet.h"

namespace BAT {

class JetReader {
public:
	JetReader();
	JetReader(TChainPointer,
			JetAlgorithm::value algo = JetAlgorithm::Calo_AntiKT_Cone05);
	virtual ~JetReader();
	virtual const JetCollection& getJets();
	virtual void initialise();

protected:
	VariableReader<MultiDoublePointer> energyReader;
	VariableReader<MultiDoublePointer> JECUncReader;
	VariableReader<MultiDoublePointer> L1OffJECReader;
	VariableReader<MultiDoublePointer> L2L3ResJECReader;
	VariableReader<MultiDoublePointer> L2RelJECReader;
	VariableReader<MultiDoublePointer> L3AbsJECReader;
	VariableReader<MultiDoublePointer> pxReader;
	VariableReader<MultiDoublePointer> pyReader;
	VariableReader<MultiDoublePointer> pzReader;
	VariableReader<MultiDoublePointer> pxRawReader;
	VariableReader<MultiDoublePointer> pyRawReader;
	VariableReader<MultiDoublePointer> pzRawReader;
	VariableReader<MultiDoublePointer> massReader;
	VariableReader<MultiDoublePointer> chargeReader;

	VariableReader<MultiDoublePointer> matchedGeneratedJetEnergyReader;
	VariableReader<MultiDoublePointer> matchedGeneratedJetPxReader;
	VariableReader<MultiDoublePointer> matchedGeneratedJetPyReader;
	VariableReader<MultiDoublePointer> matchedGeneratedJetPzReader;
	VariableReader<MultiDoublePointer> jetSmearingUncertaintyReader;

	VariableReader<MultiDoublePointer> emfReader;
	VariableReader<unsigned int> numberOfJetsReader;
	VariableReader<MultiIntPointer> n90HitsReader;
	VariableReader<MultiDoublePointer> fHPDReader;

	VariableReader<MultiIntPointer> NODReader;
	VariableReader<MultiDoublePointer> CEFReader;
	VariableReader<MultiDoublePointer> NHFReader;
	VariableReader<MultiDoublePointer> NEFReader;
	VariableReader<MultiDoublePointer> CHFReader;
	VariableReader<MultiIntPointer> NCHReader;

	VariableReader<MultiDoublePointer> btagCombinedSecondaryVertexReader;
	VariableReader<MultiDoublePointer> btagCombinedSecondaryVertexMVAReader;
	VariableReader<MultiDoublePointer> btagJetBProbabilityReader;
	VariableReader<MultiDoublePointer> btagJetProbabilityReader;
	VariableReader<MultiDoublePointer> btagSimpleSecondaryVertexHEReader;
	VariableReader<MultiDoublePointer> btagSimpleSecondaryVertexHPReader;
	VariableReader<MultiDoublePointer> btagSoftElectronByIP3dReader;
	VariableReader<MultiDoublePointer> btagSoftElectronByPtReader;
	VariableReader<MultiDoublePointer> btagSoftMuonReader;
	VariableReader<MultiDoublePointer> btagSoftMuonByIP3dReader;
	VariableReader<MultiDoublePointer> btagSoftMuonByPtReader;
	VariableReader<MultiDoublePointer> btagTrackCountingHighPurityReader;
	VariableReader<MultiDoublePointer> btagTrackCountingHighEfficiencyReader;

	VariableReader<MultiIntPointer> PartonFlavour;



	JetCollection jets;
	JetAlgorithm::value usedAlgorithm;
	virtual void readJets();

};

}

#endif /* JETREADER_H_ */
