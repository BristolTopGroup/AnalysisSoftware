/*
 * PrimaryVertexReader.h
 *
 *  Created on: 25 Jul 2010
 *      Author: kreczko
 */

#ifndef PRIMARYVERTEXREADER_H_
#define PRIMARYVERTEXREADER_H_
#include "VariableReader.h"
#include "../RecoObjects/Vertex.h"
namespace BAT {

class VertexReader {
public:
	VertexReader();
	VertexReader(TChainPointer input);
	virtual ~VertexReader();
	const VertexCollection& getVertices();
	void initialise();
private:
	VariableReader<MultiDoublePointer> ndofReader;
	VariableReader<MultiDoublePointer> zReader;
	VariableReader<MultiDoublePointer> rhoReader;
	VariableReader<MultiBoolPointer> isfakeReader;

	VertexCollection vertices;

	void readVertices();
};

}

#endif /* PRIMARYVERTEXREADER_H_ */
