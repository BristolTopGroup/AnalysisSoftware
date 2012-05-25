/*
 * TestDatasetInformation.cpp
 *
 *  Created on: 12 Mar 2012
 *      Author: kreczko
 */

#include "TestDatasetInformation.h"
#include "../interface/DataTypes.h"
#include <boost/array.hpp>

using namespace BAT;

TestDatasetInformation::TestDatasetInformation() :
		datasetInfo_("python/DataSetInfo.py") {

}

TestDatasetInformation::~TestDatasetInformation() {
}

void TestDatasetInformation::testCrossSectionTTJets() {
	ASSERT_EQUAL_DELTA(157.5, datasetInfo_.getCrossSection(DataType::TTJets), 0.1);
}

void TestDatasetInformation::testGetNumberOfProcessedEventsTTJets() {
	ASSERT_EQUAL(4438282, datasetInfo_.getNumberOfProcessedEvents(DataType::TTJets));
}

void TestDatasetInformation::testCrossSectionTTJetsFromArray() {
	boost::array<double, DataType::NUMBER_OF_DATA_TYPES> cross_sections = datasetInfo_.getCrossSections();
	ASSERT_EQUAL_DELTA(157.5, cross_sections.at(DataType::TTJets), 0.1);
}

void TestDatasetInformation::testGetNumberOfProcessedEventsTTJetsFromArray() {
	boost::array<unsigned long, DataType::NUMBER_OF_DATA_TYPES> numberOfProcessedEvents = datasetInfo_.getArrayOfProcessedEvents();
	ASSERT_EQUAL(4438282, numberOfProcessedEvents.at(DataType::TTJets));
}

cute::suite make_suite_TestDatasetInformation() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestDatasetInformation,testCrossSectionTTJets));
	s.push_back(CUTE_SMEMFUN(TestDatasetInformation,testGetNumberOfProcessedEventsTTJets));
	s.push_back(CUTE_SMEMFUN(TestDatasetInformation,testCrossSectionTTJetsFromArray));
	s.push_back(CUTE_SMEMFUN(TestDatasetInformation,testGetNumberOfProcessedEventsTTJetsFromArray));
	return s;
}

