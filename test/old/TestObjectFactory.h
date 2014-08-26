/*
 * TestObjectFactory.h
 *
 *  Created on: 28 Sep 2010
 *      Author: kreczko
 */

#ifndef TESTOBJECTFACTORY_H_
#define TESTOBJECTFACTORY_H_
#include "../interface/Event.h"
#include "../interface/RecoObjects/Vertex.h"
#include "../interface/RecoObjects/Jet.h"
#include "../interface/RecoObjects/Electron.h"
#include "../interface/RecoObjects/Track.h"
#include "../interface/RecoObjects/Muon.h"

class TestObjectFactory {
private:
    static const double goodVertexMaximalAbsoluteRho;
    static const double goodVertexMinimalNumberOfDegreesOfFreedom;
    static const double goodVertexMaximalAbsoluteZPosition;

    static const double isolatedElectronMaximalRelativeIsolation;
    static const double goodElectronMaximalAbsoluteEta;
    static const double goodElectronMinimalEt;
    static const double goodElectronMaximalDistanceFromInteractionPoint;
    static const double MaximalNumberOfMissingInnerLayerHitsBeforeCalledConversion;
    static const double MinimalDistanceToNextTrackBeforeCalledConversion;
    static const double MinimalDCotToNextTrackBeforeCalledConversion;
    static const double looseElectronMaximalAbsoluteEta;
    static const double looseElectronMinimalEt;
    static const double looseIsolatedElectronMaximalRelativeIsolation;

    static const double goodJetMaximalAbsoluteEta;
    static const double goodJetMinimalPt;
    static const double goodJetMinimalElectromagneticFraction;
    static const double goodJetMinimalNumberOfRecHitsContaining90PercentOfTheJetEnergy;
    static const double goodJetMaximalFractionOfEnergyIntheHottestHPDReadout;

    static const double goodMuonMinimalPt;
    static const double goodMuonMaximalAbsoluteEta;
    static const double isolatedMuonMaximalRelativeIsolation;
public:
    TestObjectFactory();
    virtual ~TestObjectFactory();
    static BAT::VertexPointer goodVertex();
    static BAT::VertexPointer badNDOFVertex();
    static BAT::VertexPointer badZPositionVertex();
    static BAT::VertexPointer badRhoVertex();
    static BAT::VertexPointer badFakeVertex();

    static BAT::TrackPointer highPurityTrack();
    static BAT::TrackPointer lowPurityTrack();

    static BAT::ElectronPointer goodCaloElectron();
    static BAT::ElectronPointer goodPFElectron();
    static BAT::ElectronPointer goodIsolatedElectron();
    static BAT::ElectronPointer goodIsolatedElectron2();
    static BAT::ElectronPointer goodLooseElectron();

    static BAT::ElectronPointer badLooseElectronNoID();
    static BAT::ElectronPointer badElectronNoID();
    static BAT::ElectronPointer badEtElectron();
    static BAT::ElectronPointer badEtaElectron();
    static BAT::ElectronPointer badInCrackElectron();
    static BAT::ElectronPointer badD0Electron();
    static BAT::ElectronPointer electronWithMissingInnerLayerHit();
    static BAT::ElectronPointer electronWithMatchingPartnerTrack();

    static BAT::JetPointer goodCaloJet();
    static BAT::JetPointer goodCaloJetCloseToCaloElectron();
    static BAT::JetPointer goodCaloBJetSSVHEM();

    static BAT::JetPointer badEMFCaloJet();
    static BAT::JetPointer badN90CaloJet();
    static BAT::JetPointer badfHPDCaloJet();
    static BAT::JetPointer badEtCaloJet();
    static BAT::JetPointer badEtaCaloJet();

    static BAT::MuonPointer goodIsolatedMuon();
    static BAT::MuonPointer goodNonIsolatedMuon();
    static BAT::MuonPointer badNonGlobalMuon();
    static BAT::MuonPointer badPtMuon();
    static BAT::MuonPointer badEtaMuon();

    static BAT::METPointer goodMET();
    static BAT::METPointer badMET();

};

#endif /* TESTOBJECTFACTORY_H_ */
