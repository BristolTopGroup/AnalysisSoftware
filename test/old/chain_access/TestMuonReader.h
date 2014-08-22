#ifndef TESTMUONREADER_h
#define TESTMUONREADER_h
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "TChain.h"
#include "../interface/Readers/MuonReader.h"

using namespace BAT;

struct TestMuonReader {
private:
	boost::shared_ptr<TChain> input;
	boost::scoped_ptr<MuonReader> reader;
	MuonCollection muons;
	MuonPointer leadingMuon;

public:
	TestMuonReader();
	void testNumberOfMuons();
	void testLeadingMuonEnergy();
	void testLeadingMuonPx();
	void testLeadingMuonPy();
	void testLeadingMuonPz();
	void testLeadingMuonRelativeIsolation();

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

extern cute::suite make_suite_TestMuonReader();
#endif
