#include "TestMuon.h"

using namespace BAT;

TestMuon::TestMuon() :
		testMuon(new Muon(50., 50., 0., 0.)) {
	testMuon->setNormalisedChi2(54);
	testMuon->setNumberOfMatchedStations(2);
	testMuon->setNumberOfMatches(3);
	testMuon->setNumberOfValidHits(1);
	testMuon->setNumberOfValidMuonHits(23);
	testMuon->setNumberOfValidPixelHits(56);
	testMuon->setTrackerLayersWithMeasurement(111);
	testMuon->setPixelLayersWithMeasurement(222);
	testMuon->setZDistanceToPrimaryVertex(0.5);
	testMuon->makeGlobal(true);
	testMuon->setTrackerMuon(true);
}

void TestMuon::testNormalisedChi2() {
	ASSERT_EQUAL(54, testMuon->normChi2());
}

void TestMuon::testNumberOfMatchedStations() {
	ASSERT_EQUAL(2, testMuon->numberOfMatchedStations());
}

void TestMuon::testNumberOfMatches() {
	ASSERT_EQUAL(3, testMuon->numberOfMatches());
}

void TestMuon::testNumberOfValidHits() {
	ASSERT_EQUAL(1, testMuon->numberOfValidHits());
}

void TestMuon::testNumberOfValidMuonHits() {
	ASSERT_EQUAL(23, testMuon->numberOfValidMuonHits());
}

void TestMuon::testNumberOfValidPixelHits() {
	ASSERT_EQUAL(56, testMuon->numberOfValidPixelHits());
}

void TestMuon::testTrackerLayersWithMeasurement() {
	ASSERT_EQUAL(111, testMuon->trackerLayersWithMeasurement());
}

void TestMuon::testPixelLayersWithMeasurement() {
	ASSERT_EQUAL(222, testMuon->pixelLayersWithMeasurement());
}

void TestMuon::testZDistanceToPrimaryVertex() {
	ASSERT_EQUAL(0.5, testMuon->ZDistanceToPrimaryVertex());
}

void TestMuon::testGlobalMuon() {
	ASSERT_EQUAL(true, testMuon->isGlobal());
}

void TestMuon::testPFMuon() {
	ASSERT_EQUAL(true, testMuon->isPFMuon());
}

void TestMuon::testTrackerMuon() {
	ASSERT_EQUAL(true, testMuon->isTracker());
}

cute::suite make_suite_TestMuon() {
	cute::suite s;
	s.push_back(CUTE_SMEMFUN(TestMuon, testNormalisedChi2));
	s.push_back(CUTE_SMEMFUN(TestMuon, testNumberOfMatchedStations));
	s.push_back(CUTE_SMEMFUN(TestMuon, testNumberOfMatches));
	s.push_back(CUTE_SMEMFUN(TestMuon, testNumberOfValidHits));
	s.push_back(CUTE_SMEMFUN(TestMuon, testNumberOfValidMuonHits));
	s.push_back(CUTE_SMEMFUN(TestMuon, testNumberOfValidPixelHits));
	s.push_back(CUTE_SMEMFUN(TestMuon, testTrackerLayersWithMeasurement));
	s.push_back(CUTE_SMEMFUN(TestMuon, testPixelLayersWithMeasurement));
	s.push_back(CUTE_SMEMFUN(TestMuon, testZDistanceToPrimaryVertex));

	s.push_back(CUTE_SMEMFUN(TestMuon, testGlobalMuon));
	s.push_back(CUTE_SMEMFUN(TestMuon, testPFMuon));
	s.push_back(CUTE_SMEMFUN(TestMuon, testTrackerMuon));

	return s;
}
