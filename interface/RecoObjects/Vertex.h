/*
 * PrimaryVertex.h
 *
 *  Created on: 25 Jul 2010
 *      Author: kreczko
 */

#ifndef PRIMARYVERTEX_H_
#define PRIMARYVERTEX_H_
#include <boost/shared_ptr.hpp>

namespace BAT {

class Vertex {
public:
//	static unsigned int goodVertexMinimalNumberOfDegreesOfFreedom;
//	static float goodVertexMaximalAbsoluteZPosition;
//	static float goodVertexMaximalAbsoluteRho;
//
//	static void resetSelectionValues() {
//	    PrimaryVertex::goodVertexMinimalNumberOfDegreesOfFreedom = 4;
//	    PrimaryVertex::goodVertexMaximalAbsoluteZPosition = 24;
//	    PrimaryVertex::goodVertexMaximalAbsoluteRho = 2.0;
//	}

	Vertex();
	virtual ~Vertex();
	bool isGood() const;
	bool isFake() const;
	unsigned int ndof() const;
	float absoluteZPosition() const;
	float absoluteRho() const;
	float z() const;
	void setFake(bool fake);
	void setDegreesOfFreedom(unsigned int ndof);
	void setZPosition(float z);
	void setRho(float rho);
private:
	bool is_fake;
	unsigned int degreesOfFreedom;
	float z_position;
	float rho;

};
typedef boost::shared_ptr<Vertex> VertexPointer;

}

#endif /* PRIMARYVERTEX_H_ */
