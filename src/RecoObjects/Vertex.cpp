/*
 * PrimaryVertex.cpp
 *
 *  Created on: 25 Jul 2010
 *      Author: kreczko
 */

#include "../../interface/RecoObjects/Vertex.h"
#include <math.h>
namespace BAT {

Vertex::Vertex() :
	is_fake(false),
	degreesOfFreedom(0),
	z_position(0),
	rho(0) {

}

Vertex::~Vertex() {
}

void Vertex::setDegreesOfFreedom(unsigned int ndof) {
	degreesOfFreedom = ndof;
}

void Vertex::setZPosition(double z) {
	z_position = z;
}

void Vertex::setRho(double rho) {
	this->rho = rho;
}

void Vertex::setFake(bool fake) {
	is_fake = fake;
}

unsigned int Vertex::ndof() const {
	return degreesOfFreedom;
}

double Vertex::absoluteZPosition() const {
	return fabs(z_position);
}

double Vertex::absoluteRho() const {
	return fabs(rho);
}

bool Vertex::isFake() const {
	return is_fake;
}

bool Vertex::isGood() const {
	bool passesNDOF = ndof() >= 4;
	bool passesZ = absoluteZPosition() <= 24;//cm
	bool passesRho = absoluteRho() <= 2.0;
	bool isNotFake = isFake() == false;
	return passesNDOF && passesZ && passesRho && isNotFake;
}

double Vertex::z() const{
    return z_position;
}
}
