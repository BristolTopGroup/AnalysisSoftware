/*
 * TTGenInfo.h
 *
 *  Created on: Feb 06, 2015
 *      Author: ejclemen
 */

#ifndef TTGenInfo_H_
#define TTGenInfo_H_
#include "Particle.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace BAT {
class TTGenInfo;
typedef boost::shared_ptr<TTGenInfo> TTGenInfoPointer;

namespace TTPartons {
enum partonType {
    Undefined,
    Lepton,
    Neutrino,
    Quark,
    QuarkBar,
    LepB,
    HadB
};
}

class TTGenInfo {

public:
    TTGenInfo();
    TTGenInfo(ParticlePointer quark, ParticlePointer quarkBar, int quarkGenJetIndex, int quarkBarGenJetIndex ) ;
    virtual ~TTGenInfo();

    const ParticlePointer getQuark();
    const ParticlePointer getQuarkBar();

    const int getQuarkGenJetIndex();
    const int getQuarkBarGenJetIndex();
private:
    ParticlePointer quark_;
    ParticlePointer quarkBar_;

    int quarkGenJetIndex_;
    int quarkBarGenJetIndex_;
};

}

#endif /* TTGenInfo_H_ */
