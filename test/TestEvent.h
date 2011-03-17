#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>

#include "../interface/Event.h"

using namespace BAT;

struct TestEvent {
    Event ttbarEvent, goodZEvent, poorZEvent, DiJetEvent, DiJetEventWithConversion, muonEvent;
    Event emptyEvent;
    ElectronPointer goodIsolatedElectron, goodIsolatedElectron2;
    ElectronPointer goodLooseElectron;
    ElectronPointer badEtElectron;
    ElectronPointer electronFromConversion;
    JetPointer goodJet, goodBJet, badJet, goodJetCloseToElectron;
    VertexPointer goodVertex;
    VertexPointer badVertex;
    MuonPointer goodIsolatedMuon;
    MuonPointer badMuon;
    METPointer met;

    TestEvent() :
    ttbarEvent(),
    goodZEvent(),
    poorZEvent(),
    DiJetEvent(),
    DiJetEventWithConversion(),
    muonEvent(),
    emptyEvent(),
    goodIsolatedElectron(TestObjectFactory::goodIsolatedElectron()),
    goodIsolatedElectron2(TestObjectFactory::goodIsolatedElectron2()),
    goodLooseElectron(TestObjectFactory::goodLooseElectron()),
    badEtElectron(TestObjectFactory::badEtElectron()),
    electronFromConversion(TestObjectFactory::electronWithMissingInnerLayerHit()),
    goodJet(TestObjectFactory::goodCaloJet()),
    goodBJet(TestObjectFactory::goodCaloBJetSSVHEM()),
    badJet(TestObjectFactory::badEMFCaloJet()),
    goodJetCloseToElectron(TestObjectFactory::goodCaloJetCloseToCaloElectron()),
    goodVertex(TestObjectFactory::goodVertex()),
    badVertex(TestObjectFactory::badFakeVertex()),
    goodIsolatedMuon(TestObjectFactory::goodIsolatedMuon()),
    badMuon(TestObjectFactory::badNonGlobalMuon()),
    met(TestObjectFactory::goodMET()) {

        setUpTTbarEvent();
        setUpGoodZEvent();
        setUpPoorZEvent();
        setUpDiJetEvent();
        setUpMuonEvent();
    }

private:

    void setUpTTbarEvent() {
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        ttbarEvent.setVertices(vertices);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(badEtElectron);
        ttbarEvent.setElectrons(electrons);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        jets.push_back(goodBJet);
        jets.push_back(badJet);
        ttbarEvent.setJets(jets);

        MuonCollection muons;
        muons.push_back(badMuon);
        ttbarEvent.setMuons(muons);
        ttbarEvent.setMET(met);
    }

    void setUpGoodZEvent() {
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        goodZEvent.setVertices(vertices);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(goodIsolatedElectron2);
        electrons.push_back(badEtElectron);
        goodZEvent.setElectrons(electrons);

        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        goodZEvent.setJets(jets);

    }

    void setUpPoorZEvent() {
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        poorZEvent.setVertices(vertices);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(goodLooseElectron);
        electrons.push_back(badEtElectron);
        poorZEvent.setElectrons(electrons);

        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        poorZEvent.setJets(jets);

    }

    void setUpDiJetEvent() {
        VertexCollection vertices;
        vertices.push_back(badVertex);
        DiJetEvent.setVertices(vertices);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        DiJetEvent.setJets(jets);

    }

    void setUpDiJetEventWithConversion() {
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        DiJetEventWithConversion.setVertices(vertices);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        DiJetEventWithConversion.setJets(jets);

        ElectronCollection electrons;
        electrons.push_back(electronFromConversion);
        DiJetEventWithConversion.setElectrons(electrons);
    }

    void setUpMuonEvent() {
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        muonEvent.setVertices(vertices);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(badEtElectron);
        muonEvent.setElectrons(electrons);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        jets.push_back(goodBJet);
        jets.push_back(badJet);
        muonEvent.setJets(jets);
        MuonCollection muons;
        muons.push_back(goodIsolatedMuon);
        muonEvent.setMuons(muons);
    }

public:
    void testNumberOfElectrons() {
        ASSERT_EQUAL(2, ttbarEvent.Electrons().size());
    }

    void testNumberOfGoodElectrons() {
        ASSERT_EQUAL(1, ttbarEvent.GoodElectrons().size());
    }

    void testNumberOfGoodIsolatedElectrons() {
        ASSERT_EQUAL(1, ttbarEvent.GoodIsolatedElectrons().size());
    }

    void testNumberOfJets() {
        ASSERT_EQUAL(5, ttbarEvent.Jets().size());
    }

    void testNumberOfGoodJets() {
        ASSERT_EQUAL(4, ttbarEvent.GoodJets().size());
    }

    void testRunNumber() {
        ttbarEvent.setRunNumber(42);
        ASSERT_EQUAL(42, ttbarEvent.runnumber());
    }

    void testEventNumber() {
        ttbarEvent.setEventNumber(42);
        ASSERT_EQUAL(42, ttbarEvent.eventnumber());
    }

    void testLocalEventNumber() {
        ttbarEvent.setLocalEventNumber(42);
        ASSERT_EQUAL(42, ttbarEvent.localnumber());
    }

    void testLumiBlock() {
        ttbarEvent.setLumiBlock(42);
        ASSERT_EQUAL(42, ttbarEvent.lumiblock());
    }

    void testEventWeight() {
        ttbarEvent.setEventWeight(1.152);
        ASSERT_EQUAL_DELTA(1.152, ttbarEvent.weight(), 0.001);
    }

    void testGetMostIsolatedElectron() {
        ASSERT_EQUAL_DELTA(goodIsolatedElectron2->relativeIsolation(),
                goodZEvent.MostIsolatedElectron()->relativeIsolation(), 0.001);
    }

    void testGoodJetCleaning() {
        Event event = Event();
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        event.setVertices(vertices);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(badEtElectron);
        event.setElectrons(electrons);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        jets.push_back(goodBJet);
        jets.push_back(goodJetCloseToElectron);
        event.setJets(jets);
        ASSERT_EQUAL(5, event.Jets().size());
        ASSERT_EQUAL(4, event.GoodJets().size());
    }

    void testGoodJetCleaningNoGoodElectrons() {
        Event event = Event();
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        event.setVertices(vertices);
        ElectronCollection electrons;
        goodIsolatedElectron->setSigmaIEtaIEta(0.01 + 2);
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(badEtElectron);
        event.setElectrons(electrons);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        jets.push_back(goodBJet);
        jets.push_back(goodJetCloseToElectron);
        event.setJets(jets);
        ASSERT_EQUAL(0, event.GoodElectrons().size());
        ASSERT_EQUAL(0, event.GoodIsolatedElectrons().size());
        ASSERT_EQUAL(jets.size(), event.Jets().size());
        ASSERT_EQUAL(jets.size() - 1, event.GoodJets().size());
    }

    void testGoodJetCleaningNoElectrons() {
        Event event = Event();
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        event.setVertices(vertices);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        jets.push_back(goodBJet);
        jets.push_back(goodJetCloseToElectron);
        event.setJets(jets);
        ASSERT_EQUAL(jets.size(), event.Jets().size());
        ASSERT_EQUAL(jets.size(), event.GoodJets().size());
    }

    void testGoodJetCleaningNoGoodJets() {
        Event event = Event();
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        event.setVertices(vertices);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(badEtElectron);
        event.setElectrons(electrons);
        JetCollection jets;
        jets.push_back(badJet);
        event.setJets(jets);
        ASSERT_EQUAL(jets.size(), event.Jets().size());
        ASSERT_EQUAL(0, event.GoodJets().size());
    }

    void testMET() {
        ASSERT_EQUAL(50, ttbarEvent.MET()->et());
    }

};

extern cute::suite make_suite_TestEvent() {
    cute::suite s;

    s.push_back(CUTE_SMEMFUN(TestEvent, testNumberOfElectrons));
    s.push_back(CUTE_SMEMFUN(TestEvent, testNumberOfGoodElectrons));
    s.push_back(CUTE_SMEMFUN(TestEvent, testNumberOfGoodIsolatedElectrons));
    s.push_back(CUTE_SMEMFUN(TestEvent, testNumberOfJets));
    s.push_back(CUTE_SMEMFUN(TestEvent, testNumberOfGoodJets));

    s.push_back(CUTE_SMEMFUN(TestEvent, testRunNumber));
    s.push_back(CUTE_SMEMFUN(TestEvent, testEventNumber));
    s.push_back(CUTE_SMEMFUN(TestEvent, testLocalEventNumber));
    s.push_back(CUTE_SMEMFUN(TestEvent, testLumiBlock));
    s.push_back(CUTE_SMEMFUN(TestEvent, testEventWeight));

    s.push_back(CUTE_SMEMFUN(TestEvent, testGetMostIsolatedElectron));

    s.push_back(CUTE_SMEMFUN(TestEvent, testGoodJetCleaning));
    s.push_back(CUTE_SMEMFUN(TestEvent, testGoodJetCleaningNoGoodElectrons));
    s.push_back(CUTE_SMEMFUN(TestEvent, testGoodJetCleaningNoElectrons));
    s.push_back(CUTE_SMEMFUN(TestEvent, testGoodJetCleaningNoGoodJets));
    s.push_back(CUTE_SMEMFUN(TestEvent, testMET));

    return s;
}
