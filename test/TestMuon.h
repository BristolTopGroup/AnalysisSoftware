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
	TestMuon() :
		goodNonIsolatedMuon(TestObjectFactory::goodNonIsolatedMuon()),
		goodIsolatedMuon(TestObjectFactory::goodIsolatedMuon()),
		nonGlobalMuon(TestObjectFactory::badNonGlobalMuon()),
		badMuonLowPt(TestObjectFactory::badPtMuon()),
		badEtaMuon(TestObjectFactory::badEtaMuon()) {
	}

	void testGlobalMuon() {
		ASSERT_EQUAL(true, goodNonIsolatedMuon->isGlobal());
		goodNonIsolatedMuon->makeGlobal(false);
		ASSERT_EQUAL(false, goodNonIsolatedMuon->isGlobal());
	}

	void testEcalIsolation() {
		goodNonIsolatedMuon->setEcalIsolation(1234.);
		ASSERT_EQUAL(1234., goodNonIsolatedMuon->ecalIsolation());
	}

	void testHcalIsolation() {
		goodNonIsolatedMuon->setHcalIsolation(1234.);
		ASSERT_EQUAL(1234., goodNonIsolatedMuon->hcalIsolation());
	}

	void testTrackerIsolation() {
		goodNonIsolatedMuon->setTrackerIsolation(1234.);
		ASSERT_EQUAL(1234., goodNonIsolatedMuon->trackerIsolation());
	}

	void testRelativeIsolation() {
		float relativeIsolation = (goodNonIsolatedMuon->ecalIsolation() + goodNonIsolatedMuon->hcalIsolation()
				+ goodNonIsolatedMuon->trackerIsolation()) / goodNonIsolatedMuon->pt();
		ASSERT_EQUAL(relativeIsolation, goodNonIsolatedMuon->relativeIsolation());
	}

	void testIsIsolated() {
		ASSERT_EQUAL(true, goodIsolatedMuon->isIsolated());
	}

	void testIsNotIsolated() {
		ASSERT_EQUAL(false, goodNonIsolatedMuon->isIsolated());
	}

	void testIsGoodMuon() {
		ASSERT_EQUAL(true, goodNonIsolatedMuon->isGood());
		ASSERT_EQUAL(true, goodIsolatedMuon->isGood());
	}

	void testNonGlobalMuonIsGood() {
		ASSERT_EQUAL(false, nonGlobalMuon->isGood());
	}

	void testBadMuonLowPtIsGood() {
		ASSERT_EQUAL(false, badMuonLowPt->isGood());
	}

	void testBadEtaMuonIsGood() {
		ASSERT_EQUAL(false, badEtaMuon->isGood());
	}
};

extern cute::suite make_suite_TestMuon() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestMuon, testGlobalMuon));
	s.push_back(CUTE_SMEMFUN(TestMuon, testEcalIsolation));
	s.push_back(CUTE_SMEMFUN(TestMuon, testHcalIsolation));
	s.push_back(CUTE_SMEMFUN(TestMuon, testTrackerIsolation));
	s.push_back(CUTE_SMEMFUN(TestMuon, testRelativeIsolation));

	s.push_back(CUTE_SMEMFUN(TestMuon, testIsIsolated));
	s.push_back(CUTE_SMEMFUN(TestMuon, testIsNotIsolated));
	s.push_back(CUTE_SMEMFUN(TestMuon, testIsGoodMuon));
	s.push_back(CUTE_SMEMFUN(TestMuon, testNonGlobalMuonIsGood));
	s.push_back(CUTE_SMEMFUN(TestMuon, testBadMuonLowPtIsGood));
	s.push_back(CUTE_SMEMFUN(TestMuon, testBadEtaMuonIsGood));

	return s;
}
