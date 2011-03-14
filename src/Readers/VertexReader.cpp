/*
 * PrimaryVertexReader.cpp
 *
 *  Created on: 25 Jul 2010
 *      Author: kreczko
 */

#include "../../interface/Readers/VertexReader.h"

namespace BAT {

VertexReader::VertexReader() :
	ndofReader(), zReader(), rhoReader(), isfakeReader(), vertex() {

}

VertexReader::VertexReader(TChainPointer input) :
	ndofReader(input, "Vertex.NDF"),
	zReader(input, "Vertex.Z"),
	rhoReader(input, "Vertex.Rho"),
	isfakeReader(input, "Vertex.IsFake"),
	vertex() {

}

void VertexReader::initialise() {
	ndofReader.initialise();
	zReader.initialise();
	rhoReader.initialise();
	isfakeReader.initialise();
}

const VertexPointer VertexReader::getVertex() {
	readVertex();
	return vertex;
}

void VertexReader::readVertex() {
	vertex = VertexPointer(new Vertex());
	vertex->setDegreesOfFreedom(ndofReader.getVariableAt(0));
	vertex->setFake(isfakeReader.getBoolVariableAt(0));
	vertex->setRho(rhoReader.getVariableAt(0));
	vertex->setZPosition(zReader.getVariableAt(0));
}
VertexReader::~VertexReader() {
}

}
