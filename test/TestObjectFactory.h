/*
 * TestObjectFactory.h
 *
 *  Created on: 28 Sep 2010
 *      Author: kreczko
 */

#ifndef TESTOBJECTFACTORY_H_
#define TESTOBJECTFACTORY_H_
#include "../interface/Event.h"
#include "../interface/RecoObjects/PrimaryVertex.h"
#include "../interface/RecoObjects/Jet.h"
#include "../interface/RecoObjects/Electron.h"
#include "../interface/RecoObjects/Track.h"


class TestObjectFactory {
public:
    TestObjectFactory();
    virtual ~TestObjectFactory();
    static BAT::PrimaryVertexPointer goodPrimaryVertex();
    static BAT::PrimaryVertexPointer badFakePrimaryVertex();
    static BAT::TrackPointer highPurityTrack();
    static BAT::TrackPointer lowPurityTrack();
    static BAT::ElectronPointer goodCaloElectron();
    static BAT::ElectronPointer goodPFElectron();
    static BAT::ElectronPointer goodIsolatedElectron();
    static BAT::ElectronPointer goodIsolatedElectron2();
    static BAT::ElectronPointer goodLooseElectron();
    static BAT::ElectronPointer badElectron();
    static BAT::ElectronPointer electronWithMissingInnerLayerHit();
    static BAT::ElectronPointer electronFromConversion();
    static BAT::JetPointer goodJet();
    static BAT::JetPointer goodBJet();
    static BAT::JetPointer badJet();


};

#endif /* TESTOBJECTFACTORY_H_ */
