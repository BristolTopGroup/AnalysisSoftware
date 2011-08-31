/*
 * PrimaryVertex.h
 *
 *  Created on: 25 Jul 2010
 *      Author: kreczko
 */

#ifndef PRIMARYVERTEX_H_
#define PRIMARYVERTEX_H_
#include <boost/shared_ptr.hpp>
#include <vector>

namespace BAT {

class Vertex {
public:

	Vertex();
	virtual ~Vertex();
	bool isGood() const;
	bool isFake() const;
	unsigned int ndof() const;
	double absoluteZPosition() const;
	double absoluteRho() const;
	double z() const;
	void setFake(bool fake);
	void setDegreesOfFreedom(unsigned int ndof);
	void setZPosition(double z);
	void setRho(double rho);
private:
	bool is_fake;
	unsigned int degreesOfFreedom;
	double z_position;
	double rho;

};
typedef boost::shared_ptr<Vertex> VertexPointer;
typedef std::vector<VertexPointer> VertexCollection;

}

#endif /* PRIMARYVERTEX_H_ */
