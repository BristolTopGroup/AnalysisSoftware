/*
 * TTbarMCEvent.h
 *
 *  Created on: 8 Jun 2011
 *      Author: kreczko
 */

#ifndef TTBARMCEVENT_H_
#define TTBARMCEVENT_H_

#include "TopPairEventCandidate.h"
#include "RecoObjects/Electron.h"

namespace BAT {

class TTbarMCEvent: public BAT::TopPairEventCandidate {
public:
    TTbarMCEvent();
    virtual ~TTbarMCEvent();

    //genJets
    const JetPointer BGenJetFromLeptonicTop() const;
    const JetPointer BGenJetFromHadronicTop() const;
    const JetPointer GenJet1FromW() const;
    const JetPointer GenJet2FromW() const;

    //genParticles
    const ParticlePointer MCTopQuark() const;
    const ParticlePointer MCAntiTopQuark() const;

    const ParticlePointer MCLeptonicW() const;
    const ParticlePointer MCHadronicW() const;

    const ParticlePointer MCBQuarkFromLeptonicTop() const;
    const ParticlePointer MCBQuarkFromHadronicTop() const;

    const ParticlePointer MCElectron() const;
    const ParticlePointer MCNeutrino() const;

    const ParticlePointer MCQuark1FromW() const;
    const ParticlePointer MCQuark2FromW() const;

    //add reconstruction
    void performStandardMatching(float maxDRBetweenGenJetandParticle, float maxDRBetweenGenAndRecoJet);
    void performStandardMatchingWithOptimisation(float maxDRBetweenGenJetandParticle, float maxDRBetweenGenAndRecoJet);
    const TtbarHypothesis& performCustomMatching(const JetCollection & customJets,
            float maxDRBetweenCustomJetandParticle);
    const TtbarHypothesis& performCustomMatchingWithOptimisation(const JetCollection & customJets,
            float maxDRBetweenCustomJetandParticle);
    void fillMCEvent();
private:
    void performGenParticleToGenJetMatching(float maxDRBetweenGenJetandParticle);
    void performGenJetsToRecoJetMatching(float maxDRBetweenGenAndRecoJet);

    ParticlePointer mcTopQuark, mcAntitopQuark, mcBQuarkFromHadronicTop, mcBQuarkFromLeptonicTop;
    ParticlePointer mcNeutrino, mcElectron, mcQuark1FromW, mcQuark2FromW, mcLeptonicW, mcHadronicW;
};
}
#endif /* TTBARMCEVENT_H_ */
