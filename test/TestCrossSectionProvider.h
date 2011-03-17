/*
 * TestCrossSectionProvider.h
 *
 *  Created on: 16 Aug 2010
 *      Author: kreczko
 */

#include "cute/cute.h"
#include "cute/cute_suite.h"
#include "../interface/CrossSections.h"
#include "../interface/DataTypes.h"

using namespace BAT;

struct TestCrossSectionProvider {
private:
    float lumi;
    CrossSectionProvider xSectionProvider;

public:
    TestCrossSectionProvider() :
        lumi(20), xSectionProvider(lumi) {
        xSectionProvider.useSkimEfficiency(false);
    }

    ~TestCrossSectionProvider() {

    }

    void testGetExpectedTTbarEvents() {
        ASSERT_EQUAL(157.5*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::ttbar));
    }

    void testGetExpectedWjetsEvents() {
        ASSERT_EQUAL(31314.*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::Wjets));
    }

    void testGetExpectedZjetsEvents() {
        ASSERT_EQUAL(3048.*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::Zjets));
    }

    void testGetExpectedQCD_EMEnriched_Pt20to30Events() {
        ASSERT_EQUAL(0.2355e9 * 0.0073*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::QCD_EMEnriched_Pt20to30));
    }

    void testGetExpectedQCD_EMEnriched_Pt30to80Events() {
        ASSERT_EQUAL(0.0593e9 * 0.059*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::QCD_EMEnriched_Pt30to80));
    }

    void testGetExpectedQCD_EMEnriched_Pt80to170Events() {
        ASSERT_EQUAL(0.906e6 * 0.148*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::QCD_EMEnriched_Pt80to170));
    }

    void testGetExpectedQCD_BCtoE_Pt20to30Events() {
        ASSERT_EQUAL(0.2355e9 * 0.00046*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::QCD_BCtoE_Pt20to30));
    }

    void testGetExpectedQCD_BCtoE_Pt30to80Events() {
        ASSERT_EQUAL(0.0593e9 * 0.00234*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::QCD_BCtoE_Pt30to80));
    }

    void testGetExpectedQCD_BCtoE_Pt80to170Events() {
        ASSERT_EQUAL(0.906e6 * 0.0104*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::QCD_BCtoE_Pt80to170));
    }

    void testGetExpectedVQQEvents() {
        ASSERT_EQUAL(36.*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::VQQ));
    }

    void testGetExpectedsingleTop_And_WEvents() {
        ASSERT_EQUAL(10.6*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::singleTop_And_W));
    }

    void testGetExpectedsingleTopTChannelEvents() {
        ASSERT_EQUAL_DELTA(21.53*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::singleTopTChannel), 0.1);
    }

    void testGetExpectedsingleTopSChannelEvents() {
        ASSERT_EQUAL(1.40*lumi, xSectionProvider.getExpectedNumberOfEvents(DataType::singleTopSChannel));
    }

    void testGetExpectedDataEvents() {
        ASSERT_EQUAL(0, xSectionProvider.getExpectedNumberOfEvents(DataType::DATA));
    }

    void testGetWeightData() {
        ASSERT_EQUAL(1, xSectionProvider.getWeight(DataType::DATA));
    }

    void testGetWeightTtbar() {
        ASSERT_EQUAL_DELTA(157.5 * lumi/1306182, xSectionProvider.getWeight(DataType::ttbar), 0.00000001);
    }

};

extern cute::suite make_suite_TestCrossSectionProvider() {
    cute::suite s;

    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedTTbarEvents));
    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedWjetsEvents));
    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedZjetsEvents));

    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedQCD_EMEnriched_Pt20to30Events));
    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedQCD_EMEnriched_Pt30to80Events));
    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedQCD_EMEnriched_Pt80to170Events));

    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedQCD_BCtoE_Pt20to30Events));
    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedQCD_BCtoE_Pt30to80Events));
    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedQCD_BCtoE_Pt80to170Events));

    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedVQQEvents));

    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedsingleTop_And_WEvents));
    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedsingleTopTChannelEvents));
    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedsingleTopSChannelEvents));

    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetExpectedDataEvents));
    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetWeightData));
    s.push_back(CUTE_SMEMFUN(TestCrossSectionProvider, testGetWeightTtbar));

    return s;
}

