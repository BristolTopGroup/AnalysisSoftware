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
	ndofReader(input, "Vertex.NDF"),
	zReader(input, "Vertex.Z"),
	rhoReader(input, "Vertex.Rho"),
	isfakeReader(input, "Vertex.IsFake"),
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
        vertex->setDegreesOfFreedom(ndofReader.getVariableAt(0));
        vertex->setFake(isfakeReader.getBoolVariableAt(0));
        vertex->setRho(rhoReader.getVariableAt(0));
        vertex->setZPosition(zReader.getVariableAt(0));
        vertices.push_back(vertex);
    }
}

VertexReader::~VertexReader() {
}

}
