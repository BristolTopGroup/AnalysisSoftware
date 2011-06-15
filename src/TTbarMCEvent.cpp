/*
 * TTbarMCEvent.cpp
 *
 *  Created on: 8 Jun 2011
 *      Author: kreczko
 */

#include "../interface/TTbarMCEvent.h"
#include <iostream>

namespace BAT {
TTbarMCEvent::TTbarMCEvent():
        TopPairEventCandidate(),
        mcTopQuark(),
        mcAntitopQuark(),
        mcBQuarkFromHadronicTop(),
        mcBQuarkFromLeptonicTop(),
        mcNeutrino(),
        mcElectron(),
        mcQuark1FromW(),
        mcQuark2FromW(),
        mcLeptonicW(),
        mcHadronicW(){

}

TTbarMCEvent::~TTbarMCEvent() {
}

void TTbarMCEvent::performStandardMatching(float maxDRBetweenGenJetandParticle, float maxDRBetweenGenAndRecoJet) {
    performGenParticleToGenJetMatching(maxDRBetweenGenJetandParticle);
    performGenJetsToRecoJetMatching(maxDRBetweenGenAndRecoJet);
}

void TTbarMCEvent::performGenParticleToGenJetMatching(float maxDRBetweenGenJetandParticle){

}

void TTbarMCEvent::performGenJetsToRecoJetMatching(float maxDRBetweenGenAndRecoJet) {

}

void TTbarMCEvent::fillMCEvent() {
    for (MCParticleCollection::const_iterator mc_particle = genParticles.begin(); mc_particle != genParticles.end(); ++mc_particle) {
        const MCParticlePointer particle(*mc_particle);

        if(particle->status() != 3)
            continue;
    }
}

}
