#ifndef TESTEVENT_h
#define TESTEVENT_h
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

	TestEvent();

private:

	void setUpTTbarEvent();
	void setUpGoodZEvent();
	void setUpPoorZEvent();
	void setUpDiJetEvent();
	void setUpDiJetEventWithConversion();
	void setUpMuonEvent();
public:
	void testNumberOfElectrons();
	void testNumberOfGoodElectrons();
	void testNumberOfGoodIsolatedElectrons();
	void testNumberOfJets();
	void testNumberOfGoodJets();
	void testRunNumber();
	void testEventNumber();
	void testLocalEventNumber();
	void testLumiBlock();
	void testEventWeight();
	void testGetMostIsolatedElectron();
	void testGoodJetCleaning();
	void testGoodJetCleaningNoGoodElectrons();
	void testGoodJetCleaningNoElectrons();
	void testGoodJetCleaningNoGoodJets();
	void testMET();

};

extern cute::suite make_suite_TestEvent();
#endif
