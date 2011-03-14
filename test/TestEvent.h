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
    ElectronPointer badElectron;
    ElectronPointer electronFromConversion;
    JetPointer goodJet, goodBJet, badJet, goodJetCloseToElectron;
    VertexPointer goodVertex;
    VertexPointer badVertex;
    Muon goodIsolatedMuon;
    Muon badMuon;
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
    goodLooseElectron(new Electron(100., 79., -13., -5.)),
    badElectron(new Electron(20, 14., 15., 0)),
    electronFromConversion(new Electron(*goodIsolatedElectron)),
    goodJet(new Jet(100, 13, 99, 5)),
    goodBJet(new Jet(*goodJet)),
    badJet(new Jet(20, 19, 0, 0)),
    goodJetCloseToElectron(new Jet(100., 98., 13., 5.)),
    goodVertex(TestObjectFactory::goodPrimaryVertex()),
    badVertex(TestObjectFactory::badFakePrimaryVertex()),
    goodIsolatedMuon(100., 99., 13., 5.),
    badMuon(100., 99., 13., 5.),
    met(new MET(40, 30)) {
        setUpGoodLooseElectron();
        setUpBadElectron();
        setUpGoodIsolatedElectronFromConversion();
        setUpGoodJet();
        setUpGoodBJet();
        setUpGoodJetCloseToElectron();
        setUpIsolatedGoodMuon();
        setUpBadMuon();

        setUpTTbarEvent();
        setUpGoodZEvent();
        setUpPoorZEvent();
        setUpDiJetEvent();
        setUpMuonEvent();
    }

private:

    void setUpBadElectron() {
        badElectron->setHcalIsolation(4);
        badElectron->setEcalIsolation(44);
        badElectron->setTrackerIsolation(1);
    }

    void setUpGoodLooseElectron() {
        goodLooseElectron->setHcalIsolation(5);
        goodLooseElectron->setEcalIsolation(3);
        goodLooseElectron->setTrackerIsolation(4);
        goodLooseElectron->setSigmaIEtaIEta(0.009);
		goodLooseElectron->setDPhiIn(0.7);
		goodLooseElectron->setDEtaIn(0.006);
		goodLooseElectron->setHadOverEm(0.14);
        goodLooseElectron->setRobustLooseID(true);
        goodLooseElectron->setNumberOfMissingInnerLayerHits(0);
        goodLooseElectron->setD0(0);
        goodLooseElectron->setSuperClusterEta(0);
        assert(goodLooseElectron->isGood() == false);
        assert(goodLooseElectron->isLoose());
    }

    void setUpGoodIsolatedElectronFromConversion() {
        electronFromConversion->setHcalIsolation(0.5);
        electronFromConversion->setEcalIsolation(0.3);
        electronFromConversion->setTrackerIsolation(0.4);
        electronFromConversion->setNumberOfMissingInnerLayerHits(3);
    }
    void setUpGoodJet() {
        goodJet->setEMF(0.2);
        goodJet->setFHPD(0.5);
        goodJet->setN90Hits(2);
    }

    void setUpGoodBJet() {
        goodBJet->setEMF(0.2);
        goodBJet->setFHPD(0.5);
        goodBJet->setN90Hits(2);
        goodBJet->setDiscriminatorForBtagType(2.5, BJetTagger::SimpleSecondaryVertex);
    }

    void setUpGoodJetCloseToElectron() {
        goodJetCloseToElectron->setEMF(0.2);
        goodJetCloseToElectron->setFHPD(0.5);
        goodJetCloseToElectron->setN90Hits(2);
    }


    void setUpBadVertex() {
        badVertex->setDegreesOfFreedom(4);
        badVertex->setFake(true);
        badVertex->setRho(2.0);
        badVertex->setZPosition(24);
    }

    void setUpIsolatedGoodMuon() {
        goodIsolatedMuon.makeGlobal(true);
        goodIsolatedMuon.setEcalIsolation(1);
        goodIsolatedMuon.setHcalIsolation(1);
        goodIsolatedMuon.setTrackerIsolation(1);
    }

    void setUpBadMuon() {
        badMuon.makeGlobal(false);
    }

    void setUpTTbarEvent() {
        ttbarEvent.setPrimaryVertex(goodVertex);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(badElectron);
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
        goodZEvent.setPrimaryVertex(goodVertex);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(goodIsolatedElectron2);
        electrons.push_back(badElectron);
        goodZEvent.setElectrons(electrons);

        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        goodZEvent.setJets(jets);

    }

    void setUpPoorZEvent() {
        poorZEvent.setPrimaryVertex(goodVertex);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(goodLooseElectron);
        electrons.push_back(badElectron);
        poorZEvent.setElectrons(electrons);

        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        poorZEvent.setJets(jets);

    }

    void setUpDiJetEvent() {
        DiJetEvent.setPrimaryVertex(badVertex);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        DiJetEvent.setJets(jets);

    }

    void setUpDiJetEventWithConversion() {
        DiJetEventWithConversion.setPrimaryVertex(goodVertex);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        DiJetEventWithConversion.setJets(jets);

        ElectronCollection electrons;
        electrons.push_back(electronFromConversion);
        DiJetEventWithConversion.setElectrons(electrons);
    }

    void setUpMuonEvent() {
        muonEvent.setPrimaryVertex(goodVertex);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(badElectron);
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
        event.setPrimaryVertex(goodVertex);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(badElectron);
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
        event.setPrimaryVertex(goodVertex);
        ElectronCollection electrons;
        goodIsolatedElectron->setSigmaIEtaIEta(0.01 + 2);
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(badElectron);
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
        event.setPrimaryVertex(goodVertex);
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
        event.setPrimaryVertex(goodVertex);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(badElectron);
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
