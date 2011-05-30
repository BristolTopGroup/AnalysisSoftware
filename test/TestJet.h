#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/RecoObjects/Jet.h"
#include "TestObjectFactory.h"

using namespace BAT;

struct TestJet {
    JetPointer goodJet;
    JetPointer goodBJet;
    JetPointer badEtaJet;
    JetPointer badEtJet;
    JetPointer badEMFJet;
    JetPointer badn90HitsJet;
    JetPointer badfHPDJet;

    TestJet() :
        goodJet(TestObjectFactory::goodCaloJet()),
        goodBJet(TestObjectFactory::goodCaloBJetSSVHEM()),
        badEtaJet(TestObjectFactory::badEtaCaloJet()),
        badEtJet(TestObjectFactory::badEtCaloJet()),
        badEMFJet(TestObjectFactory::badEMFCaloJet()),
        badn90HitsJet(TestObjectFactory::badN90CaloJet()),
        badfHPDJet(TestObjectFactory::badfHPDCaloJet()) {
    }

public:

    /**
     * tests for isGood are done at creation level!
     */
    void testGoodJet() {
        ASSERT(goodJet->isGood());
    }

    void testBadEtaJet() {
        ASSERT(badEtaJet->isGood() == false);
    }

    void testBadEtJet() {
        ASSERT(badEtJet->isGood() == false);
    }

    void testBadEMFJet() {
        ASSERT(badEMFJet->isGood() == false);
    }

    void testBadN90HitsJet() {
        ASSERT(badn90HitsJet->isGood() == false);
    }

    void testBadfHPDJet() {
        ASSERT(badfHPDJet->isGood() == false);
    }

    void testUsedJetAlgorithm() {
        ASSERT_EQUAL(JetAlgorithm::Calo_AntiKT_Cone05, goodJet->getUsedAlgorithm());
        goodJet->setUsedAlgorithm(JetAlgorithm::CA08PF);
        ASSERT_EQUAL(JetAlgorithm::CA08PF, goodJet->getUsedAlgorithm());
    }

    void testJetEMF() {
        goodJet->setEMF(2304.23);
        ASSERT_EQUAL_DELTA(2304.23, goodJet->emf(), 0.01);
    }

    void testJetN90Hits() {
        goodJet->setN90Hits(234.);
        ASSERT_EQUAL_DELTA(234., goodJet->n90Hits(), 0.1);
    }

    void testJetFHPD() {
        goodJet->setFHPD(0.444);
        ASSERT_EQUAL_DELTA(0.444, goodJet->fHPD(), 0.001);
    }

    void testPositiveBtag() {
        ASSERT(goodBJet->isBJet(BtagAlgorithm::SimpleSecondaryVertexHighEffBTag));
    }

    void testNegativeBtag() {
        ASSERT(goodJet->isBJet(BtagAlgorithm::SimpleSecondaryVertexHighEffBTag) == false);
    }

};

extern cute::suite make_suite_TestJet() {
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
