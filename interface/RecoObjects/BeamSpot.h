/*
 * BeamSpot.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef BEAMSPOT_H_
#define BEAMSPOT_H_
#include "Point3D.h"

namespace BAT {

class BeamSpot :public Point3D {
public:
	BeamSpot();
	virtual ~BeamSpot();
};

}

#endif /* BEAMSPOT_H_ */
