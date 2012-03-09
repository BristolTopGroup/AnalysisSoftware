#ifndef TESTTOPPAIREVENTCANDIDATE_H_
#define TESTTOPPAIREVENTCANDIDATE_H_
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>
#include "../interface/Event.h"
#include "../interface/OnlyForTests/DummyTTbarEvent.h"

using namespace BAT;
using namespace std;

struct TestTopPairEventCandidate {
    DummyTTbarEvent ttbarEvent, goodZEvent, poorZEvent, DiJetEvent, DiJetEventWithConversion, muonEvent;
    DummyTTbarEvent emptyEvent, customEvent;

    ElectronPointer goodIsolatedElectron, goodIsolatedElectron2;
    ElectronPointer goodLooseElectron;
    ElectronPointer badElectron;
    ElectronPointer electronFromConversion;
    JetPointer goodJet, goodBJet, badJet, goodJetCloseToElectron;
    VertexPointer goodVertex;
    VertexPointer badVertex;
    MuonPointer goodIsolatedMuon;
    MuonPointer badMuon;
    METPointer met;
    TrackCollection lessThan10Tracks;
    TrackCollection moreThan10TracksHighPurity;
    TrackCollection moreThan10TracksLowPurity;
    TrackCollection moreThan10TracksMixedPurity_H;
    TrackCollection moreThan10TracksMixedPurity_L;
    boost::shared_ptr<std::vector<int> > HLTs;

    TestTopPairEventCandidate();

private:

    void setUpTTbarEvent();
    void setUpGoodZEvent();
    void setUpPoorZEvent();
    void setUpDiJetEvent();
    void setUpDiJetEventWithConversion();
    void setUpMuonEvent();
    void setUpTracks();
    void setUpCustomEvent();
public:
    void testEventConstructor();
    void testPassesHLT();
    void testDoesHaveGoodPV();
    void testDoesntHaveGoodPV();
    void testHasOnlyOneGoodIsolatedElectron();
    void testHasNotOnlyOneGoodIsolatedElectron();
    void testIsolatedElectronNotFromConversion();
    void testIsolatedElectronFromConversion();
    void testNoElectronInEventReturnsFalse();
    void testHasAtLeastOneGoodJet();
    void testHasNoGoodJets();
    void testHasAtLeastTwoGoodjet();
    void testHasNoTwoGoodJets();
    void testHasAtLeastThreeGoodJets();
    void testHasNoThreeGoodJets();
    void testHasAtFourThreeGoodJets();
    void testHasNoFourThreeGoodJets();
    void testTTbarIsNotAZBosonEvent();
    void testGoodZIsAZBosonEvent();
    void testPoorZIsAZBosonEvent();
    void testDiJetIsNotAZBosonEvent();
    void testTTbarEventPassesMuonVeto();
    void testMuonEventDoesnPassMuonVeto();
    void testEventPasses0Step();
    void testEventPasses0StepWithLowNumberOfTracks();
    void testEventPasses0StepWithMixedTracks_H();
    void testEventPasses0StepWithMixedTracks_L();
    void testEventFails0StepWithLowPurityTracks();
    void testEventPasses1stStep();
    void testEventPasses2ndStep();
    void testEventPasses3ndStep();
    void testEventPasses4thStep();
    void testEventPasses5thStep();
    void testEventPasses6thStep();
    void testEventPasses7thStep();
    void testTTbarEventPassesNStep();
    void testPoorZEventPassesUpToStep();
    void testPoorZEventDoesntPassUpToStep();
    void testPassesFullTTbarSelection();
    void testFailsFullTTbarSelection();
    void testComputeNeutrinoPzsWithoutMETThrowsExpeption();
    void testComputeNeutrinoPzsIsolatedElectronThrowsExpeption();
    void testComputeNeutrinoPzs();
    void testReconstructTopEventUsingChiWithNotEnoughJetsThrowsException();
    void testNMinus1CutsPositive();
    void testNMinus1CutsNegative();
};

extern cute::suite make_suite_TestTopPairEventCandidate();
#endif
