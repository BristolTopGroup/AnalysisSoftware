#include "TestEvent.h"
#include "TestObjectFactory.h"

using namespace BAT;

TestEvent::TestEvent() :
		ttbarEvent(), //
		goodZEvent(), //
		poorZEvent(), //
		DiJetEvent(), //
		DiJetEventWithConversion(), //
		muonEvent(), //
		emptyEvent(), //
		goodIsolatedElectron(TestObjectFactory::goodIsolatedElectron()), //
		goodIsolatedElectron2(TestObjectFactory::goodIsolatedElectron2()), //
		goodLooseElectron(TestObjectFactory::goodLooseElectron()), //
		badEtElectron(TestObjectFactory::badEtElectron()), //
		electronFromConversion(TestObjectFactory::electronWithMissingInnerLayerHit()), //
		goodJet(TestObjectFactory::goodCaloJet()), //
		goodBJet(TestObjectFactory::goodCaloBJetSSVHEM()), //
		badJet(TestObjectFactory::badEMFCaloJet()), //
		goodJetCloseToElectron(TestObjectFactory::goodCaloJetCloseToCaloElectron()), //
		goodVertex(TestObjectFactory::goodVertex()), //
		badVertex(TestObjectFactory::badFakeVertex()), //
		goodIsolatedMuon(TestObjectFactory::goodIsolatedMuon()), //
		badMuon(TestObjectFactory::badNonGlobalMuon()), //
		met(TestObjectFactory::goodMET()) //
{

	setUpTTbarEvent();
	setUpGoodZEvent();
	setUpPoorZEvent();
	setUpDiJetEvent();
	setUpMuonEvent();
}

void TestEvent::setUpTTbarEvent() {
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
	METCollection mets;
	mets.resize(METAlgorithm::NUMBER_OF_METALGORITHMS);
	mets.at(METAlgorithm::patMETsPFlow) = met;

	ttbarEvent.setMETs(mets);
}

void TestEvent::setUpGoodZEvent() {
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

void TestEvent::setUpPoorZEvent() {
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

void TestEvent::setUpDiJetEvent() {
	VertexCollection vertices;
	vertices.push_back(badVertex);
	DiJetEvent.setVertices(vertices);
	JetCollection jets;
	jets.push_back(goodJet);
	jets.push_back(goodJet);
	DiJetEvent.setJets(jets);

}

void TestEvent::setUpDiJetEventWithConversion() {
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

void TestEvent::setUpMuonEvent() {
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

void TestEvent::testNumberOfElectrons() {
	ASSERT_EQUAL(2, ttbarEvent.Electrons().size());
}

void TestEvent::testNumberOfJets() {
	ASSERT_EQUAL(5, ttbarEvent.Jets().size());
}

void TestEvent::testRunNumber() {
	ttbarEvent.setRunNumber(42);
	ASSERT_EQUAL(42, ttbarEvent.runnumber());
}

void TestEvent::testEventNumber() {
	ttbarEvent.setEventNumber(42);
	ASSERT_EQUAL(42, ttbarEvent.eventnumber());
}

void TestEvent::testLocalEventNumber() {
	ttbarEvent.setLocalEventNumber(42);
	ASSERT_EQUAL(42, ttbarEvent.localnumber());
}

void TestEvent::testLumiBlock() {
	ttbarEvent.setLumiBlock(42);
	ASSERT_EQUAL(42, ttbarEvent.lumiblock());
}

void TestEvent::testEventWeight() {
	ttbarEvent.setEventWeight(1.152);
	ASSERT_EQUAL_DELTA(1.152, ttbarEvent.weight(), 0.001);
}

void TestEvent::testGetMostIsolatedElectron() {
	ASSERT_EQUAL_DELTA(goodIsolatedElectron2->relativeIsolation(),
			goodZEvent.MostIsolatedElectron(goodZEvent.Electrons())->relativeIsolation(), 0.001);
}

void TestEvent::testGoodJetCleaning() {
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
}

void TestEvent::testGoodJetCleaningNoGoodElectrons() {
	Event event = Event();
	VertexCollection vertices;
	vertices.push_back(goodVertex);
	event.setVertices(vertices);
	ElectronCollection electrons;
	electrons.push_back(badEtElectron);
	event.setElectrons(electrons);
	JetCollection jets;
	jets.push_back(goodJet);
	jets.push_back(goodJet);
	jets.push_back(goodBJet);
	jets.push_back(goodBJet);
	jets.push_back(goodJetCloseToElectron);
	event.setJets(jets);
	ASSERT_EQUAL(jets.size(), event.Jets().size());
}

void TestEvent::testGoodJetCleaningNoElectrons() {
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
}

void TestEvent::testGoodJetCleaningNoGoodJets() {
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
}

void TestEvent::testMET() {
	ASSERT_EQUAL(50, ttbarEvent.MET()->et());
}

cute::suite make_suite_TestEvent() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestEvent, testNumberOfElectrons));
	s.push_back(CUTE_SMEMFUN(TestEvent, testNumberOfJets));

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
