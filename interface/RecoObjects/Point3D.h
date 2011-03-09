/*
 * Vertex.h
 *
 *  Created on: 2 Jul 2010
 *      Author: kreczko
 */

#ifndef Point3D_H_
#define Point3D_H_

namespace BAT {

class Point3D {
public:
	float x,y,z;
	Point3D();
	Point3D(float x, float y, float z);
	virtual ~Point3D();
};

typedef Point3D Vertex;
}

#endif /* VERTEX_H_ */
