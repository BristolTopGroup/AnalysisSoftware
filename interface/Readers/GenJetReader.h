/*
 * GenJetReader.h
 *
 *  Created on: 14 Mar 2011
 *      Author: kreczko
 */

#ifndef GENJETREADER_H_
#define GENJETREADER_H_
#include "VariableReader.h"
#include "../RecoObjects/Jet.h"

namespace BAT {

class GenJetReader {
public:
	GenJetReader();
	GenJetReader(TChainPointer);
	virtual ~GenJetReader();
	virtual const JetCollection& getGenJets();
	virtual void initialise();

protected:
	VariableReader<MultiDoublePointer> genEnergyReader;
	VariableReader<MultiDoublePointer> genPxReader;
	VariableReader<MultiDoublePointer> genPyReader;
	VariableReader<MultiDoublePointer> genPzReader;
	VariableReader<MultiDoublePointer> genMassReader;
	VariableReader<MultiDoublePointer> genChargeReader;
	VariableReader<MultiDoublePointer> genEmfReader;

	JetCollection genJets;
	virtual void readGenJets();

};

}

#endif /* GENJETREADER_H_ */
