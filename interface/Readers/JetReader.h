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
#include <string>


namespace BAT {
	
class JetReaderBase {
public:
	JetReaderBase();
	JetReaderBase(TChainPointer, const TString &fieldPrefix,
		JetAlgorithm::value algo = JetAlgorithm::Calo_AntiKT_Cone05);
	virtual ~JetReaderBase();
	virtual const JetCollection& getJets();
	virtual void initialise();
protected:
	VariableReader<MultiDoublePointer> energyReader;
	VariableReader<MultiDoublePointer> pxReader;
	VariableReader<MultiDoublePointer> pyReader;
	VariableReader<MultiDoublePointer> pzReader;
	VariableReader<MultiDoublePointer> massReader;

	JetCollection jets;
	JetAlgorithm::value usedAlgorithm;
	virtual void readJets();
	virtual void readJet(JetPointer &jet, unsigned int jetIndex) = 0;
};


class JetReader : public JetReaderBase {
public:
//	const static std::string algorithmPrefixes[JetAlgorithm::NUMBER_OF_JETALGORITHMS];
	
	JetReader();
	JetReader(TChainPointer, JetAlgorithm::value algo = JetAlgorithm::Calo_AntiKT_Cone05);
	virtual ~JetReader();
	virtual void initialise();
	
protected:
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
//	VariableReader<MultiDoublePointer> btagSimpleSecondaryVertexReaderData;
	VariableReader<MultiDoublePointer> btagTrackCountingHighPurityReader;
	VariableReader<MultiDoublePointer> btagTrackCountingHighEfficiencyReader;
	
	virtual void readJet(JetPointer &jet, unsigned int jetIndex);
};


class GenJetReader : public JetReaderBase {
public:
	GenJetReader();
	GenJetReader(TChainPointer, JetAlgorithm::value algo = JetAlgorithm::Calo_AntiKT_Cone05);
	virtual ~GenJetReader();
	virtual void initialise();
	
protected:
	VariableReader<MultiDoublePointer> emfReader;
	VariableReader<MultiDoublePointer> chargeReader;
	
	virtual void readJet(JetPointer &jet, unsigned int jetIndex);
};
}

#endif /* JETREADER_H_ */
