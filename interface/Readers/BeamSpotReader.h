/*
 * BeamSpotReader.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef BEAMSPOTREADER_H_
#define BEAMSPOTREADER_H_
#include "../RecoObjects/BeamSpot.h"

namespace BAT {

class BeamSpotReader {
public:
	BeamSpotReader();
	virtual ~BeamSpotReader();
	BeamSpot getBeamSpot();
};

}

#endif /* BEAMSPOTREADER_H_ */
