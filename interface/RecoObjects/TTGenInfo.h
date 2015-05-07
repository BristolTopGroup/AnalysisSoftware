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
    TTGenInfo(ParticlePointer quark, ParticlePointer quarkBar, ParticlePointer leptonicB, ParticlePointer hadronicB, ParticlePointer singlelepton, ParticlePointer singleneutrino, int quarkGenJetIndex, int quarkBarGenJetIndex, int leptonicBGenJetIndex, int hadronicBGenJetIndex ) ;
    virtual ~TTGenInfo();

    const ParticlePointer getQuark();
    const ParticlePointer getQuarkBar();

    const ParticlePointer getleptonicB();
    const ParticlePointer gethadronicB();

    const ParticlePointer getsinglelepton();
    const ParticlePointer getsingleneutrino();

    const int getQuarkGenJetIndex();
    const int getQuarkBarGenJetIndex();

    const int getleptonicBGenJetIndex();
    const int gethadronicBGenJetIndex();

private:
    ParticlePointer quark_;
    ParticlePointer quarkBar_;

    ParticlePointer leptonicB_;
    ParticlePointer hadronicB_;

    ParticlePointer singlelepton_;
    ParticlePointer singleneutrino_;

    int quarkGenJetIndex_;
    int quarkBarGenJetIndex_;

    int leptonicBGenJetIndex_;
    int hadronicBGenJetIndex_;
};

}

#endif /* TTGenInfo_H_ */
