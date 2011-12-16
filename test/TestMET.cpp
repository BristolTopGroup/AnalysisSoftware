#include "TestMET.h"
#include "TestObjectFactory.h"

using namespace BAT;

TestMET::TestMET() :
		goodMET(TestObjectFactory::goodMET()), //
		badMET(TestObjectFactory::badMET()) //
{
}

void TestMET::testStandardConstructorEt() {
	ASSERT_EQUAL(50, goodMET->et());
}

void TestMET::testStandardConstructorPt() {
	ASSERT_EQUAL(50, goodMET->pt());
}

void TestMET::testStandardConstructorPx() {
	ASSERT_EQUAL(40, goodMET->px());
}
void TestMET::testStandardConstructorPy() {
	ASSERT_EQUAL(30, goodMET->py());
}

void TestMET::testStandardConstructorPz() {
	ASSERT_EQUAL(0, goodMET->pz());
}

void TestMET::testStandardConstructorMass() {
	ASSERT_EQUAL(0, goodMET->mass());
}

void TestMET::testGoodMETIsGood() {
	ASSERT_EQUAL(true, goodMET->isGood());
}

void TestMET::testBadMETNoGood() {
	ASSERT_EQUAL(false, badMET->isGood());
}

void TestMET::testSetAlgorithm() {
	goodMET->setUsedAlgorithm(METAlgorithm::ParticleFlowMET);
	ASSERT_EQUAL(METAlgorithm::ParticleFlowMET, goodMET->getUsedAlgorithm());
}

void TestMET::testStandardAlgorithm() {
	ASSERT_EQUAL(METAlgorithm::Calo, goodMET->getUsedAlgorithm());
}

void TestMET::testSumET() {
	ASSERT_EQUAL(20*goodMET->et(), goodMET->sumET());
}

void TestMET::testSignificance() {
	ASSERT_EQUAL(50, goodMET->significance());
}

cute::suite make_suite_TestMET() {
    cute::suite s;

    s.push_back(CUTE_SMEMFUN(TestMET,testStandardConstructorEt));
    s.push_back(CUTE_SMEMFUN(TestMET,testStandardConstructorPt));
    s.push_back(CUTE_SMEMFUN(TestMET,testStandardConstructorPx));
    s.push_back(CUTE_SMEMFUN(TestMET,testStandardConstructorPy));
    s.push_back(CUTE_SMEMFUN(TestMET,testStandardConstructorPz));
    s.push_back(CUTE_SMEMFUN(TestMET,testStandardConstructorMass));
    s.push_back(CUTE_SMEMFUN(TestMET,testGoodMETIsGood));
    s.push_back(CUTE_SMEMFUN(TestMET,testBadMETNoGood));
    s.push_back(CUTE_SMEMFUN(TestMET,testSetAlgorithm));
    s.push_back(CUTE_SMEMFUN(TestMET,testStandardAlgorithm));
    s.push_back(CUTE_SMEMFUN(TestMET,testSumET));
    s.push_back(CUTE_SMEMFUN(TestMET,testSignificance));

    return s;
}
