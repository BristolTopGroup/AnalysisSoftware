#include "TestMuon.h"
#include "TestObjectFactory.h"

using namespace BAT;

TestMuon::TestMuon() :
		goodNonIsolatedMuon(TestObjectFactory::goodNonIsolatedMuon()), //
		goodIsolatedMuon(TestObjectFactory::goodIsolatedMuon()), //
		nonGlobalMuon(TestObjectFactory::badNonGlobalMuon()), //
		badMuonLowPt(TestObjectFactory::badPtMuon()), //
		badEtaMuon(TestObjectFactory::badEtaMuon()) {
}

void TestMuon::testGlobalMuon() {
	ASSERT_EQUAL(true, goodNonIsolatedMuon->isGlobal());
	goodNonIsolatedMuon->makeGlobal(false);
	ASSERT_EQUAL(false, goodNonIsolatedMuon->isGlobal());
}

//void TestMuon::testEcalIsolation03() {
//	goodNonIsolatedMuon->setEcalIsolation(1234., 0.3);
//	ASSERT_EQUAL(1234., goodNonIsolatedMuon->ecalIsolation());
//}
//
//void TestMuon::testHcalIsolation03() {
//	goodNonIsolatedMuon->setHcalIsolation(1234., 0.3);
//	ASSERT_EQUAL(1234., goodNonIsolatedMuon->hcalIsolation());
//}
//
//void TestMuon::testTrackerIsolation03() {
//	goodNonIsolatedMuon->setTrackerIsolation(1234., 0.3);
//	ASSERT_EQUAL(1234., goodNonIsolatedMuon->trackerIsolation());
//}
//
//void TestMuon::testEcalIsolation04() {
//	goodNonIsolatedMuon->setEcalIsolation(1234., 0.4);
//	ASSERT_EQUAL(1234., goodNonIsolatedMuon->ecalIsolation(0.4));
//}
//
//void TestMuon::testHcalIsolation04() {
//	goodNonIsolatedMuon->setHcalIsolation(1234., 0.4);
//	ASSERT_EQUAL(1234., goodNonIsolatedMuon->hcalIsolation(0.4));
//}
//
//void TestMuon::testTrackerIsolation04() {
//	goodNonIsolatedMuon->setTrackerIsolation(1234., 0.4);
//	ASSERT_EQUAL(1234., goodNonIsolatedMuon->trackerIsolation(0.4));
//}
//
//void TestMuon::testEcalIsolation05() {
//	goodNonIsolatedMuon->setEcalIsolation(1234., 0.5);
//	ASSERT_EQUAL(1234., goodNonIsolatedMuon->ecalIsolation(0.5));
//}
//
//void TestMuon::testHcalIsolation05() {
//	goodNonIsolatedMuon->setHcalIsolation(1234., 0.5);
//	ASSERT_EQUAL(1234., goodNonIsolatedMuon->hcalIsolation(0.5));
//}
//
//void TestMuon::testTrackerIsolation05() {
//	goodNonIsolatedMuon->setTrackerIsolation(1234., 0.5);
//	ASSERT_EQUAL(1234., goodNonIsolatedMuon->trackerIsolation(0.5));
//}
//
//void TestMuon::testRelativeIsolation() {
//	float relativeIsolation = (goodNonIsolatedMuon->ecalIsolation() + goodNonIsolatedMuon->hcalIsolation()
//			+ goodNonIsolatedMuon->trackerIsolation()) / goodNonIsolatedMuon->pt();
//	ASSERT_EQUAL(relativeIsolation, goodNonIsolatedMuon->relativeIsolation());
//}

//void TestMuon::testIsIsolated() {
//	ASSERT_EQUAL(true, goodIsolatedMuon->relativeIsolation() < 0.1);
//}
//
//void TestMuon::testIsNotIsolated() {
//	ASSERT_EQUAL(false, goodNonIsolatedMuon->relativeIsolation() < 0.1);
//}
//
//void TestMuon::testIsGoodMuon() {
//	ASSERT_EQUAL(true, goodNonIsolatedMuon->isGood());
//	ASSERT_EQUAL(true, goodIsolatedMuon->isGood());
//}
//
//void TestMuon::testNonGlobalMuonIsGood() {
//	ASSERT_EQUAL(false, nonGlobalMuon->isGood());
//}
//
//void TestMuon::testBadMuonLowPtIsGood() {
//	ASSERT_EQUAL(false, badMuonLowPt->isGood());
//}
//
//void TestMuon::testBadEtaMuonIsGood() {
//	ASSERT_EQUAL(false, badEtaMuon->isGood());
//}

cute::suite make_suite_TestMuon() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestMuon, testGlobalMuon));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testEcalIsolation03));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testHcalIsolation03));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testTrackerIsolation03));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testEcalIsolation04));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testHcalIsolation04));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testTrackerIsolation04));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testEcalIsolation05));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testHcalIsolation05));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testTrackerIsolation05));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testRelativeIsolation));
//
//	s.push_back(CUTE_SMEMFUN(TestMuon, testIsIsolated));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testIsNotIsolated));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testIsGoodMuon));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testNonGlobalMuonIsGood));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testBadMuonLowPtIsGood));
//	s.push_back(CUTE_SMEMFUN(TestMuon, testBadEtaMuonIsGood));

	return s;
}
