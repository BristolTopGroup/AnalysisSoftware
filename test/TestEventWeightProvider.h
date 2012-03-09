/*
 * TestEventWeightProvider.h
 *
 *  Created on: 16 Aug 2010
 *      Author: kreczko
 */

#ifndef TESTEVENTWEIGHTPROVIDER_h
#define  TESTEVENTWEIGHTPROVIDER_h
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
    TestEventWeightProvider();
    ~TestEventWeightProvider();

    void testGetWeightData();
    void testGetWeightTtbar();
};

extern cute::suite make_suite_TestEventWeightProvider();

#endif
