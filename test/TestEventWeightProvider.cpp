/*
 * TestEventWeightProvider.h
 *
 *  Created on: 16 Aug 2010
 *      Author: kreczko
 */

#include "TestEventWeightProvider.h"

using namespace BAT;

TestEventWeightProvider::TestEventWeightProvider() :
		lumi(20), xSectionProvider(lumi, 7, "Pileup_2011_to_172802_LP_LumiScale.root") {
}

TestEventWeightProvider::~TestEventWeightProvider() {

}

void TestEventWeightProvider::testGetWeightData() {
	ASSERT_EQUAL(1, xSectionProvider.getWeight(DataType::ElectronHad));
	ASSERT_EQUAL(1, xSectionProvider.getWeight(DataType::MuHad));
}

void TestEventWeightProvider::testGetWeightTtbar() {
	ASSERT_EQUAL_DELTA(157.5 * lumi/3701947, xSectionProvider.getWeight(DataType::ttbar), 0.00000001);
}

cute::suite make_suite_TestEventWeightProvider() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestEventWeightProvider, testGetWeightData));
	s.push_back(CUTE_SMEMFUN(TestEventWeightProvider, testGetWeightTtbar));

	return s;
}

