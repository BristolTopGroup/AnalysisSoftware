/*
 * TestEventWeightProvider.h
 *
 *  Created on: 16 Aug 2010
 *      Author: kreczko
 */

#include "TestEventWeightProvider.h"
#include "../interface/GlobalVariables.h"

using namespace BAT;

TestEventWeightProvider::TestEventWeightProvider() :
		lumi(5000), //
		xSectionProvider("python/DataSetInfo.py") {
	Globals::luminosity = lumi;
}

TestEventWeightProvider::~TestEventWeightProvider() {

}

void TestEventWeightProvider::testGetWeightData() {
	ASSERT_EQUAL(1, xSectionProvider.getWeight(DataType::ElectronHad));
	ASSERT_EQUAL(1, xSectionProvider.getWeight(DataType::MuHad));
}

void TestEventWeightProvider::testGetWeightTtbar() {
	ASSERT_EQUAL_DELTA(157.5 * lumi/59414270, xSectionProvider.getWeight(DataType::TTJets), 0.00000001);
}

cute::suite make_suite_TestEventWeightProvider() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestEventWeightProvider, testGetWeightData));
	s.push_back(CUTE_SMEMFUN(TestEventWeightProvider, testGetWeightTtbar));

	return s;
}

