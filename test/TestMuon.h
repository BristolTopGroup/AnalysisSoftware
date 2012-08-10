#ifndef TESTMUON_h
#define TESTMUON_h
#include "cute/cute.h"
#include "cute/cute_suite.h"
#include "../interface/RecoObjects/Muon.h"

using namespace BAT;

struct TestMuon {
private:

	MuonPointer testMuon;

public:
	TestMuon();

	void testNormalisedChi2();
	void testNumberOfMatchedStations();
	void testNumberOfMatches();
	void testNumberOfValidHits();
	void testNumberOfValidMuonHits();
	void testNumberOfValidPixelHits();
	void testTrackerLayersWithMeasurement();
	void testPixelLayersWithMeasurement();
	void testZDistanceToPrimaryVertex();
	void testGlobalMuon();
	void testPFMuon();
	void testTrackerMuon();

};

extern cute::suite make_suite_TestMuon();
#endif
