#ifndef TESTMUON_h
#define TESTMUON_h
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/RecoObjects/Muon.h"
#include "TestObjectFactory.h"

using namespace BAT;

struct TestMuon {
private:

	MuonPointer goodNonIsolatedMuon;
	MuonPointer goodIsolatedMuon;
	MuonPointer nonGlobalMuon;
	MuonPointer badMuonLowPt;
	MuonPointer badEtaMuon;

public:
	TestMuon() ;
	void testGlobalMuon();
	void testEcalIsolation();
	void testHcalIsolation();
	void testTrackerIsolation();
	void testRelativeIsolation();
	void testIsIsolated();
	void testIsNotIsolated();
	void testIsGoodMuon();
	void testNonGlobalMuonIsGood();
	void testBadMuonLowPtIsGood();
	void testBadEtaMuonIsGood();
};

extern cute::suite make_suite_TestMuon();
#endif
