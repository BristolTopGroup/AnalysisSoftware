#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/RecoObjects/MET.h"
#include "TestObjectFactory.h"

using namespace BAT;

struct TestMET {
private:
    METPointer goodMET, badMET;
public:
    TestMET() :
        goodMET(TestObjectFactory::goodMET()),
        badMET(TestObjectFactory::badMET()) {
    }

    void testStandardConstructorEt() {
        ASSERT_EQUAL(50, goodMET->et());
    }

    void testStandardConstructorPt() {
        ASSERT_EQUAL(50, goodMET->pt());
    }

    void testStandardConstructorPx() {
        ASSERT_EQUAL(40, goodMET->px());
    }
    void testStandardConstructorPy() {
        ASSERT_EQUAL(30, goodMET->py());
    }

    void testStandardConstructorPz() {
        ASSERT_EQUAL(0, goodMET->pz());
    }

    void testStandardConstructorMass() {
        ASSERT_EQUAL(0, goodMET->mass());
    }

    void testGoodMETIsGood() {
        ASSERT_EQUAL(true, goodMET->isGood());
    }

    void testBadMETNoGood() {
        ASSERT_EQUAL(false, badMET->isGood());
    }

    void testSetAlgorithm() {
        goodMET->setUsedAlgorithm(METAlgorithm::ParticleFlowMET);
        ASSERT_EQUAL(METAlgorithm::ParticleFlowMET, goodMET->getUsedAlgorithm());
    }

    void testStandardAlgorithm() {
        ASSERT_EQUAL(METAlgorithm::Calo, goodMET->getUsedAlgorithm());
    }
};

extern cute::suite make_suite_TestMET() {
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

    return s;
}
