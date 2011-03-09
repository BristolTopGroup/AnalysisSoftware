/*
 * MET.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef MET_H_
#define MET_H_
#include "Particle.h"
#include <boost/shared_ptr.hpp>
#include "../Constants.h"

namespace BAT {

class MET: public Particle {
public:
	MET();
	MET(float ex, float ey);
	virtual ~MET();
	bool isGood() const;
	void setUsedAlgorithm(METAlgorithm::value algo);
	METAlgorithm::value getUsedAlgorithm() const;
private:
	METAlgorithm::value usedAlgorithm;
};

typedef boost::shared_ptr<MET> METPointer;

}

#endif /* MET_H_ */
