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

double const TestObjectFactory::goodVertexMaximalAbsoluteRho = 2.0;
double const TestObjectFactory::goodVertexMinimalNumberOfDegreesOfFreedom = 4;
double const TestObjectFactory::goodVertexMaximalAbsoluteZPosition = 24.;//cm

double const TestObjectFactory::isolatedElectronMaximalRelativeIsolation = 0.1;
double const TestObjectFactory::goodElectronMaximalAbsoluteEta = 2.1;
double const TestObjectFactory::goodElectronMinimalEt = 30.;
double const TestObjectFactory::goodElectronMaximalDistanceFromInteractionPoint = 0.02;//cm
double const TestObjectFactory::MaximalNumberOfMissingInnerLayerHitsBeforeCalledConversion = 0;
double const TestObjectFactory::MinimalDistanceToNextTrackBeforeCalledConversion = 0.2;
double const TestObjectFactory::MinimalDCotToNextTrackBeforeCalledConversion = 0.2;
double const TestObjectFactory::looseElectronMaximalAbsoluteEta = 2.5;
double const TestObjectFactory::looseElectronMinimalEt = 25.;
double const TestObjectFactory::looseIsolatedElectronMaximalRelativeIsolation = 1.0;

double const TestObjectFactory::goodJetMaximalAbsoluteEta = 2.4;
double const TestObjectFactory::goodJetMinimalPt = 30;
double const TestObjectFactory::goodJetMinimalElectromagneticFraction = 0.01;
double const TestObjectFactory::goodJetMinimalNumberOfRecHitsContaining90PercentOfTheJetEnergy = 1;
double const TestObjectFactory::goodJetMaximalFractionOfEnergyIntheHottestHPDReadout = 0.98;

double const TestObjectFactory::goodMuonMinimalPt = 15.;
double const TestObjectFactory::goodMuonMaximalAbsoluteEta = 2.5;
double const TestObjectFactory::isolatedMuonMaximalRelativeIsolation = 0.2;

TestObjectFactory::TestObjectFactory() {

}

TestObjectFactory::~TestObjectFactory() {
}

VertexPointer TestObjectFactory::goodVertex() {
    VertexPointer pv = VertexPointer(new Vertex());

    pv->setFake(false);
    pv->setDegreesOfFreedom(TestObjectFactory::goodVertexMinimalNumberOfDegreesOfFreedom);
    pv->setRho(TestObjectFactory::goodVertexMaximalAbsoluteRho * 0.9);
    pv->setZPosition(TestObjectFactory::goodVertexMaximalAbsoluteZPosition);

    assert(pv->isGood());

    return pv;
}

VertexPointer TestObjectFactory::badFakeVertex() {
    VertexPointer pv(TestObjectFactory::goodVertex());

    pv->setFake(true);

    assert(pv->isGood() == false);

    return pv;
}

VertexPointer TestObjectFactory::badNDOFVertex() {
    VertexPointer pv(TestObjectFactory::goodVertex());

    pv->setDegreesOfFreedom(TestObjectFactory::goodVertexMinimalNumberOfDegreesOfFreedom - 1);

    assert(pv->isGood() == false);

    return pv;
}

VertexPointer TestObjectFactory::badRhoVertex() {
    VertexPointer pv(TestObjectFactory::goodVertex());

    pv->setRho(TestObjectFactory::goodVertexMaximalAbsoluteRho * 1.1);

    assert(pv->isGood() == false);

    return pv;
}

VertexPointer TestObjectFactory::badZPositionVertex() {
    VertexPointer pv(TestObjectFactory::goodVertex());

    pv->setZPosition(TestObjectFactory::goodVertexMaximalAbsoluteZPosition * 1.1);

    assert(pv->isGood() == false);

    return pv;
}

ElectronPointer TestObjectFactory::goodCaloElectron() {
    ElectronPointer electron = ElectronPointer(new Electron(100., 99., 13., 5.));
    electron->setUsedAlgorithm(ElectronAlgorithm::Calo);
    electron->setSuperClusterEta(0);
    electron->setD0_wrtBeamSpot(0);
    VertexPointer pv = TestObjectFactory::goodVertex();
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

ElectronPointer TestObjectFactory::badEtElectron() {
    ElectronPointer badEtElectron(new Electron(20., 10., 0., 5.));
    badEtElectron->setD0(0.01);
    badEtElectron->setSuperClusterEta(1);

    assert(fabs(badEtElectron->eta()) < TestObjectFactory::goodElectronMaximalAbsoluteEta);
    assert(fabs(badEtElectron->d0()) < TestObjectFactory::goodElectronMaximalDistanceFromInteractionPoint);
    //and fails the selected
    assert(badEtElectron->et() < TestObjectFactory::goodElectronMinimalEt);
    return badEtElectron;
}

ElectronPointer TestObjectFactory::badEtaElectron() {
    ElectronPointer badEtaElectron(new Electron(400., 50., 50., 380));
    badEtaElectron->setD0(0.01);
    badEtaElectron->setSuperClusterEta(1);

    assert(fabs(badEtaElectron->eta()) > TestObjectFactory::goodElectronMaximalAbsoluteEta);
    assert(fabs(badEtaElectron->d0()) < TestObjectFactory::goodElectronMaximalDistanceFromInteractionPoint);
    assert(badEtaElectron->et() > TestObjectFactory::goodElectronMinimalEt);

    return badEtaElectron;
}

ElectronPointer TestObjectFactory::badInCrackElectron() {
    ElectronPointer badInCrackElectron(TestObjectFactory::goodCaloElectron());
    badInCrackElectron->setSuperClusterEta(1.5);

    assert(badInCrackElectron->isInCrack());

    return badInCrackElectron;
}

ElectronPointer TestObjectFactory::badD0Electron() {
    ElectronPointer badD0Electron(TestObjectFactory::goodCaloElectron());
    badD0Electron->setD0_wrtBeamSpot(300.);
    badD0Electron->setD0(300.);

    assert(fabs(badD0Electron->d0()) > goodElectronMaximalDistanceFromInteractionPoint);
    assert(fabs(badD0Electron->d0_wrtBeamSpot()) > goodElectronMaximalDistanceFromInteractionPoint);

    return badD0Electron;
}

ElectronPointer TestObjectFactory::electronWithMissingInnerLayerHit() {
    ElectronPointer electronWithMissingInnerLayerHit(TestObjectFactory::goodCaloElectron());
    electronWithMissingInnerLayerHit->setNumberOfMissingInnerLayerHits(1);

    assert(electronWithMissingInnerLayerHit->innerLayerMissingHits() > MaximalNumberOfMissingInnerLayerHitsBeforeCalledConversion);
    return electronWithMissingInnerLayerHit;

}

ElectronPointer TestObjectFactory::electronWithMatchingPartnerTrack() {
    ElectronPointer electronWithMatchingPartnerTrack(TestObjectFactory::goodCaloElectron());
    electronWithMatchingPartnerTrack->setDCotThetaToNextTrack(0.1);
    electronWithMatchingPartnerTrack->setDistToNextTrack(0.01);

    assert(electronWithMatchingPartnerTrack->distToClosestTrack() < TestObjectFactory::MinimalDistanceToNextTrackBeforeCalledConversion);
    assert(electronWithMatchingPartnerTrack->dCotThetaToClosestTrack() < TestObjectFactory::MinimalDCotToNextTrackBeforeCalledConversion);

    return electronWithMatchingPartnerTrack;
}

ElectronPointer TestObjectFactory::goodLooseElectron() {
    ElectronPointer looseElectron(new Electron(100., 79., -13., -5.));
    looseElectron->setSigmaIEtaIEta(0.009);
    looseElectron->setDPhiIn(0.7);
    looseElectron->setDEtaIn(0.006);
    looseElectron->setHadOverEm(0.14);
    looseElectron->setSuperClusterEta(1);
    looseElectron->setEcalIsolation(0);
    looseElectron->setTrackerIsolation(0);
    looseElectron->setHcalIsolation(0);

    looseElectron->setNumberOfMissingInnerLayerHits(0);
    looseElectron->setD0(0);
    looseElectron->setSuperClusterEta(0);

    assert(looseElectron->isGood() == false);
    assert(looseElectron->isLoose());

    return looseElectron;
}

ElectronPointer TestObjectFactory::badLooseElectronNoID() {
    ElectronPointer badLooseElectronNoID(goodLooseElectron());
    badLooseElectronNoID->setHcalIsolation(0.5);
    badLooseElectronNoID->setEcalIsolation(0.3);
    badLooseElectronNoID->setTrackerIsolation(0.4);
    badLooseElectronNoID->setSigmaIEtaIEta(0.009 + 2);

    assert(badLooseElectronNoID->VBTF_W95_ElectronID() == false);

    return badLooseElectronNoID;
}

ElectronPointer TestObjectFactory::badElectronNoID() {
    ElectronPointer badElectronNoID(TestObjectFactory::goodCaloElectron());
    badElectronNoID->setHcalIsolation(0.5);
    badElectronNoID->setEcalIsolation(0.3);
    badElectronNoID->setTrackerIsolation(0.4);
    badElectronNoID->setSigmaIEtaIEta(0.009 + 2);

    assert(badElectronNoID->VBTF_W70_ElectronID() == false);

    return badElectronNoID;
}

JetPointer TestObjectFactory::goodCaloJet() {
    JetPointer goodJet(new Jet(290., 200., 200., 0.));
    goodJet->setEMF(0.1);
    goodJet->setN90Hits(2.);
    goodJet->setFHPD(0.5);

    assert(goodJet->isGood());

    return goodJet;
}

JetPointer TestObjectFactory::badEMFCaloJet() {
    JetPointer badEMFJet(TestObjectFactory::goodCaloJet());

    badEMFJet->setEMF(goodJetMinimalElectromagneticFraction - 0.1);

    assert(badEMFJet->isGood() == false);

    return badEMFJet;
}

JetPointer TestObjectFactory::badN90CaloJet() {
    JetPointer badN90Jet(TestObjectFactory::goodCaloJet());

    badN90Jet->setN90Hits(TestObjectFactory::goodJetMinimalNumberOfRecHitsContaining90PercentOfTheJetEnergy - 1);

    assert(badN90Jet->isGood() == false);

    return badN90Jet;
}

JetPointer TestObjectFactory::badfHPDCaloJet() {
    JetPointer badfHPDJet(TestObjectFactory::goodCaloJet());

    badfHPDJet->setFHPD(TestObjectFactory::goodJetMaximalFractionOfEnergyIntheHottestHPDReadout + 0.01);

    assert(badfHPDJet->isGood() == false);

    return badfHPDJet;
}

JetPointer TestObjectFactory::badEtCaloJet() {
    JetPointer badEtJet(TestObjectFactory::goodCaloJet());

    badEtJet->setFourVector(FourVector(20., 20., 0., 20));

    assert(badEtJet->isGood() == false);

    return badEtJet;
}

JetPointer TestObjectFactory::badEtaCaloJet() {
    JetPointer badEtJet(TestObjectFactory::goodCaloJet());

    badEtJet->setFourVector(FourVector(200., 0., 2000., 2900.));

    assert(badEtJet->isGood() == false);

    return badEtJet;
}

JetPointer TestObjectFactory::goodCaloBJetSSVHEM() {
    JetPointer goodBJet(TestObjectFactory::goodCaloJet());

    goodBJet->setDiscriminatorForBtagType(1.7 + 0.1, BtagAlgorithm::SimpleSecondaryVertexHighEffBTag);

    assert(goodBJet->isGood());
    assert(goodBJet->isBJet(BtagAlgorithm::SimpleSecondaryVertexHighEffBTag));

    return goodBJet;
}

/**
 * This depends on TestObjectFactory::goodCaloElectron()!!
 */
JetPointer TestObjectFactory::goodCaloJetCloseToCaloElectron() {
    JetPointer goodJet(TestObjectFactory::goodCaloJet());

    goodJet->setFourVector(FourVector(98., 13., 5., 100.));

    return goodJet;
}

MuonPointer TestObjectFactory::goodIsolatedMuon() {
    MuonPointer goodIsolatedMuon(new Muon(100., 99., 13., 5.));

    goodIsolatedMuon->makeGlobal(true);
    goodIsolatedMuon->setEcalIsolation(1);
    goodIsolatedMuon->setHcalIsolation(1);
    goodIsolatedMuon->setTrackerIsolation(1);

    assert(goodIsolatedMuon->isGood());
    assert(goodIsolatedMuon->isIsolated());

    return goodIsolatedMuon;
}

MuonPointer TestObjectFactory::goodNonIsolatedMuon() {
    MuonPointer goodNonIsoMuon(TestObjectFactory::goodIsolatedMuon());

    goodNonIsoMuon->setEcalIsolation(200);
    goodNonIsoMuon->setHcalIsolation(200);
    goodNonIsoMuon->setTrackerIsolation(200);

    assert(goodNonIsoMuon->isGood());
    assert(goodNonIsoMuon->isIsolated() == false);

    return goodNonIsoMuon;
}

MuonPointer TestObjectFactory::badNonGlobalMuon() {
    MuonPointer nonGlobalMuon(TestObjectFactory::goodIsolatedMuon());

    nonGlobalMuon->makeGlobal(false);

    assert(nonGlobalMuon->isGood() == false);

    return nonGlobalMuon;
}

MuonPointer TestObjectFactory::badPtMuon() {
    MuonPointer badMuonLowPt(new Muon(5., 4., 1., 1.));

    assert(badMuonLowPt->isGood() == false);

    return badMuonLowPt;
}

MuonPointer TestObjectFactory::badEtaMuon() {
    MuonPointer badEtaMu(new Muon(450., 50., 50., 444.));

    assert(badEtaMu->isGood() == false);

    return badEtaMu;
}

METPointer TestObjectFactory::goodMET() {
    METPointer goodMET(new MET(40, 30));

    assert(goodMET->isGood());

    return goodMET;
}

METPointer TestObjectFactory::badMET() {
    METPointer badMissingET(new MET(4, 3));

    assert(badMissingET->isGood() == false);

    return badMissingET;
}

