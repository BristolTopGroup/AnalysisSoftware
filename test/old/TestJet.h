#ifndef TESTJET_h
#define TESTJET_h
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/RecoObjects/Jet.h"

using namespace BAT;

struct TestJet {
    JetPointer goodJet;
    JetPointer goodBJet;
    JetPointer badEtaJet;
    JetPointer badEtJet;
    JetPointer badEMFJet;
    JetPointer badn90HitsJet;
    JetPointer badfHPDJet;

    TestJet();

public:

    /**
     * tests for isGood are done at creation level!
     */
    void testGoodJet();
    void testBadEtaJet();
    void testBadEtJet();
    void testBadEMFJet();
    void testBadN90HitsJet();
    void testBadfHPDJet();
    void testUsedJetAlgorithm();
    void testJetEMF();
    void testJetN90Hits();
    void testJetFHPD();
    void testPositiveBtag();
    void testNegativeBtag();
};

extern cute::suite make_suite_TestJet();
#endif
