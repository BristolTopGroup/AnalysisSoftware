/*
 * Vertex.cpp
 *
 *  Created on: 2 Jul 2010
 *      Author: kreczko
 */

#include "../../interface/RecoObjects/Point3D.h"

namespace BAT {

Point3D::Point3D() :
	x(0), y(0), z(0) {

}

Point3D::Point3D(float x, float y, float z) :
	x(x), y(y), z(z) {

}

Point3D::~Point3D() {
}

}
