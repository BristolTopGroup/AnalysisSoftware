/*
 * PrimaryVertexReader.cpp
 *
 *  Created on: 25 Jul 2010
 *      Author: kreczko
 */

#include "../../interface/Readers/VertexReader.h"

namespace BAT {

VertexReader::VertexReader() :
	ndofReader(),
	zReader(),
	rhoReader(),
	isfakeReader(),
	vertices() {

}

VertexReader::VertexReader(TChainPointer input) :
	ndofReader(input, "goodOfflinePrimaryVertices.NDF"),
	zReader(input, "goodOfflinePrimaryVertices.Z"),
	rhoReader(input, "goodOfflinePrimaryVertices.Rho"),
	isfakeReader(input, "goodOfflinePrimaryVertices.IsFake"),
	vertices() {

}

void VertexReader::initialise() {
    ndofReader.initialise();
    zReader.initialise();
    rhoReader.initialise();
    isfakeReader.initialise();
}

const VertexCollection& VertexReader::getVertices() {
    if (vertices.empty() == false)
        vertices.clear();
    readVertices();
    return vertices;
}

void VertexReader::readVertices() {
    for (unsigned int index = 0; index < ndofReader.size(); index++) {
        VertexPointer vertex(new Vertex());
        vertex->setDegreesOfFreedom(ndofReader.getVariableAt(index));
        vertex->setFake(isfakeReader.getBoolVariableAt(index));
        vertex->setRho(rhoReader.getVariableAt(index));
        vertex->setZPosition(zReader.getVariableAt(index));
        vertices.push_back(vertex);
    }
}

VertexReader::~VertexReader() {
}

}
