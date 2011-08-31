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
#include <string>
#include <boost/array.hpp>

namespace BAT {

namespace METAlgorithm {
enum value {
    Calo,
    tcMET,
    ParticleFlowMET,
    NUMBER_OF_METALGORITHMS
};

const boost::array<std::string, METAlgorithm::NUMBER_OF_METALGORITHMS> prefixes = { {
        "patMETs",
        "TCMET",
        "patMETsPFlow"  } };

const boost::array<std::string, METAlgorithm::NUMBER_OF_METALGORITHMS> names = { {
        "CaloMET",
        "TCMET",
        "PFMET" } };
}

class MET: public Particle {
public:
	MET();
	MET(double ex, double ey);
	virtual ~MET();
	bool isGood() const;

	METAlgorithm::value getUsedAlgorithm() const;
	double significance() const;
	double sumET() const;

	void setUsedAlgorithm(METAlgorithm::value algo);
	void setSignificance(double significance);
	void setSumET(double sumET);
private:
	METAlgorithm::value usedAlgorithm;
	double significance_, sumET_;
};

typedef boost::shared_ptr<MET> METPointer;

}

#endif /* MET_H_ */
