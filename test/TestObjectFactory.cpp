/*
 * TestObjectFactory.cpp
 *
 *  Created on: 28 Sep 2010
 *      Author: kreczko
 */

#include "TestObjectFactory.h"
#include <iostream>

using namespace std;
using namespace BAT;

TestObjectFactory::TestObjectFactory() {

}

TestObjectFactory::~TestObjectFactory() {
}

PrimaryVertexPointer TestObjectFactory::goodPrimaryVertex(){
    PrimaryVertexPointer pv = PrimaryVertexPointer(new PrimaryVertex());
    pv->setFake(false);
    pv->setDegreesOfFreedom(10);
    pv->setRho(1.0);
    pv->setZPosition(20);
    assert(pv->isGood());
    return pv;
}

PrimaryVertexPointer TestObjectFactory::badFakePrimaryVertex(){
    PrimaryVertexPointer pv = PrimaryVertexPointer(new PrimaryVertex());
    pv->setFake(true);
    pv->setDegreesOfFreedom(10);
    pv->setRho(1.0);
    pv->setZPosition(20);
    assert(pv->isGood() == false);
    return pv;
}

ElectronPointer TestObjectFactory::goodCaloElectron() {
    ElectronPointer electron = ElectronPointer(new Electron(100., 99., 13., 5.));
    electron->setUsedAlgorithm(ElectronAlgorithm::Calo);
    electron->setSuperClusterEta(0);
    electron->setD0_wrtBeamSpot(0);
    PrimaryVertexPointer pv = TestObjectFactory::goodPrimaryVertex();
    // electron vertex == primary vertex
//    electron->setElectronVertexZPosition(pv->z());
    electron->setZDistanceToPrimaryVertex(0);

    electron->setDEtaIn(0);
    electron->setDPhiIn(0);
    electron->setHadOverEm(0);
    electron->setSigmaIEtaIEta(0);

    electron->setNumberOfMissingInnerLayerHits(0);
    electron->setDCotThetaToNextTrack(0.5);
    electron->setDistToNextTrack(0.5);
    if (electron->isGood() == false) {
        cout << "Et " << electron->et() << endl;
        cout << "Eta " << electron->eta() << endl;
        cout << "VBTF 70 " << electron->VBTF_W70_ElectronID() << endl;
        cout << "d0 " << electron->d0_wrtBeamSpot() << endl;
        cout << "Et " << electron->et() << endl;
    }
    assert(electron->isGood());
    assert(electron->isFromConversion() == false);
    assert(electron->isIsolated() == false);
    return electron;
}

ElectronPointer TestObjectFactory::goodIsolatedElectron() {
    ElectronPointer electron = TestObjectFactory::goodCaloElectron();
    electron->setTrackerIsolation(0.4);
    electron->setEcalIsolation(0.3);
    electron->setHcalIsolation(0.5);

    assert(electron->isIsolated());
    assert(electron->isGood());
    assert(electron->isFromConversion() == false);
    assert(electron->isTaggedAsConversion(0.02,0.02) == false);
    return electron;
}

ElectronPointer TestObjectFactory::goodIsolatedElectron2() {
    ElectronPointer electron = TestObjectFactory::goodIsolatedElectron();
    ElectronPointer electron2 = TestObjectFactory::goodIsolatedElectron();
    FourVector vec = electron->getFourVector();
    vec.SetPx(vec.Px() - 20);
    vec.SetPy(-vec.Py());
    vec.SetPz(-vec.Pz());
    electron2->setFourVector(vec);
    //Z mass requirement
    assert(electron->invariantMass(electron2) > 80);
    assert(electron->invariantMass(electron2) < 100);
    assert(electron2->isIsolated());
    assert(electron2->isGood());
    return electron2;
}

