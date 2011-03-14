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
//	virtual const JetCollection& getGenJets();
	virtual void initialise();

protected:
	VariableReader<MultiDoublePointer> energyReader;
	VariableReader<MultiDoublePointer> pxReader;
	VariableReader<MultiDoublePointer> pyReader;
	VariableReader<MultiDoublePointer> pzReader;
	VariableReader<MultiDoublePointer> massReader;
	VariableReader<MultiDoublePointer> chargeReader;

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

	VariableReader<MultiDoublePointer> btagSimpleSecondaryVertexReader;
	VariableReader<MultiDoublePointer> btagTrackCountingHighPurityReader;
	VariableReader<MultiDoublePointer> btagTrackCountingHighEfficiencyReader;

//	VariableReader<MultiDoublePointer> genEnergyReader;
//	VariableReader<MultiDoublePointer> genPxReader;
//	VariableReader<MultiDoublePointer> genPyReader;
//	VariableReader<MultiDoublePointer> genPzReader;
//	VariableReader<MultiDoublePointer> genMassReader;
//	VariableReader<MultiDoublePointer> genChargeReader;
//	VariableReader<MultiDoublePointer> genEmfReader;

	JetCollection jets;
//	JetCollection genJets;
	JetAlgorithm::value usedAlgorithm;
	virtual void readJets();
//	virtual void readGenJets();

};

}

#endif /* JETREADER_H_ */
