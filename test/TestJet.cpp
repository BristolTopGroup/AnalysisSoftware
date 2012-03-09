#include "TestJet.h"

#include "TestObjectFactory.h"

using namespace BAT;

TestJet::TestJet() :
		goodJet(TestObjectFactory::goodCaloJet()), //
		goodBJet(TestObjectFactory::goodCaloBJetSSVHEM()), //
		badEtaJet(TestObjectFactory::badEtaCaloJet()), //
		badEtJet(TestObjectFactory::badEtCaloJet()), //
		badEMFJet(TestObjectFactory::badEMFCaloJet()), //
		badn90HitsJet(TestObjectFactory::badN90CaloJet()), //
		badfHPDJet(TestObjectFactory::badfHPDCaloJet()) {
}

/**
 * tests for isGood are done at creation level!
 */
void TestJet::testGoodJet() {
	ASSERT(goodJet->isGood());
}

void TestJet::testBadEtaJet() {
	ASSERT(badEtaJet->isGood() == false);
}

void TestJet::testBadEtJet() {
	ASSERT(badEtJet->isGood() == false);
}

void TestJet::testBadEMFJet() {
	ASSERT(badEMFJet->isGood() == false);
}

void TestJet::testBadN90HitsJet() {
	ASSERT(badn90HitsJet->isGood() == false);
}

void TestJet::testBadfHPDJet() {
	ASSERT(badfHPDJet->isGood() == false);
}

void TestJet::testUsedJetAlgorithm() {
	ASSERT_EQUAL(JetAlgorithm::Calo_AntiKT_Cone05, goodJet->getUsedAlgorithm());
	goodJet->setUsedAlgorithm(JetAlgorithm::CA08PF);
	ASSERT_EQUAL(JetAlgorithm::CA08PF, goodJet->getUsedAlgorithm());
}

void TestJet::testJetEMF() {
	goodJet->setEMF(2304.23);
	ASSERT_EQUAL_DELTA(2304.23, goodJet->emf(), 0.01);
}

void TestJet::testJetN90Hits() {
	goodJet->setN90Hits(234.);
	ASSERT_EQUAL_DELTA(234., goodJet->n90Hits(), 0.1);
}

void TestJet::testJetFHPD() {
	goodJet->setFHPD(0.444);
	ASSERT_EQUAL_DELTA(0.444, goodJet->fHPD(), 0.001);
}

void TestJet::testPositiveBtag() {
	ASSERT(goodBJet->isBJet(BtagAlgorithm::SimpleSecondaryVertexHighEfficiency));
}

void TestJet::testNegativeBtag() {
	ASSERT(goodJet->isBJet(BtagAlgorithm::SimpleSecondaryVertexHighEfficiency) == false);
}

cute::suite make_suite_TestJet() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestJet, testGoodJet));
	s.push_back(CUTE_SMEMFUN(TestJet, testBadEtaJet));
	s.push_back(CUTE_SMEMFUN(TestJet, testBadEtJet));
	s.push_back(CUTE_SMEMFUN(TestJet, testBadEMFJet));
	s.push_back(CUTE_SMEMFUN(TestJet, testBadN90HitsJet));
	s.push_back(CUTE_SMEMFUN(TestJet, testBadfHPDJet));

	s.push_back(CUTE_SMEMFUN(TestJet, testUsedJetAlgorithm));
	s.push_back(CUTE_SMEMFUN(TestJet, testJetEMF));
	s.push_back(CUTE_SMEMFUN(TestJet, testJetN90Hits));
	s.push_back(CUTE_SMEMFUN(TestJet, testJetFHPD));

	s.push_back(CUTE_SMEMFUN(TestJet, testPositiveBtag));
	s.push_back(CUTE_SMEMFUN(TestJet, testNegativeBtag));

	return s;
}
