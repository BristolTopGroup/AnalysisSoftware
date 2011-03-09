/*
 * PrimaryVertexReader.cpp
 *
 *  Created on: 25 Jul 2010
 *      Author: kreczko
 */

#include "../../interface/Readers/PrimaryVertexReader.h"

namespace BAT {

PrimaryVertexReader::PrimaryVertexReader() :
	ndofReader(), zReader(), rhoReader(), isfakeReader(), vertex() {

}

PrimaryVertexReader::PrimaryVertexReader(TChainPointer input) :
	ndofReader(input, "Vertex.NDF"),
	zReader(input, "Vertex.Z"),
	rhoReader(input, "Vertex.Rho"),
	isfakeReader(input, "Vertex.IsFake"),
	vertex() {

}

void PrimaryVertexReader::initialise() {
	ndofReader.initialise();
	zReader.initialise();
	rhoReader.initialise();
	isfakeReader.initialise();
}

const PrimaryVertexPointer PrimaryVertexReader::getVertex() {
	readVertex();
	return vertex;
}

void PrimaryVertexReader::readVertex() {
	vertex = PrimaryVertexPointer(new PrimaryVertex());
	vertex->setDegreesOfFreedom(ndofReader.getVariableAt(0));
	vertex->setFake(isfakeReader.getBoolVariableAt(0));
	vertex->setRho(rhoReader.getVariableAt(0));
	vertex->setZPosition(zReader.getVariableAt(0));
}
PrimaryVertexReader::~PrimaryVertexReader() {
}

}
