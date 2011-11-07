/*
 * TestEventWeightProvider.h
 *
 *  Created on: 16 Aug 2010
 *      Author: kreczko
 */

#include "cute/cute.h"
#include "cute/cute_suite.h"
#include "../interface/EventWeightProvider.h"
#include "../interface/DataTypes.h"

using namespace BAT;

struct TestEventWeightProvider {
private:
    float lumi;
    EventWeightProvider xSectionProvider;

public:
    TestEventWeightProvider() :
        lumi(20),
        xSectionProvider(lumi, 7, "Pileup_2011_to_172802_LP_LumiScale.root") {
    }

    ~TestEventWeightProvider() {

    }

    void testGetWeightData() {
        ASSERT_EQUAL(1, xSectionProvider.getWeight(DataType::ElectronHad));
        ASSERT_EQUAL(1, xSectionProvider.getWeight(DataType::MuHad));
    }

    void testGetWeightTtbar() {
        ASSERT_EQUAL_DELTA(157.5 * lumi/3634668, xSectionProvider.getWeight(DataType::ttbar), 0.00000001);
    }

};

extern cute::suite make_suite_TestEventWeightProvider() {
    cute::suite s;

    s.push_back(CUTE_SMEMFUN(TestEventWeightProvider, testGetWeightData));
    s.push_back(CUTE_SMEMFUN(TestEventWeightProvider, testGetWeightTtbar));

    return s;
}

