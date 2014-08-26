/*
 * TestDatasetInformation.h
 *
 *  Created on: 12 Mar 2012
 *      Author: kreczko
 */

#ifndef TESTMCINFORMATION_H_
#define TESTMCINFORMATION_H_
#include "../interface/Python/DatasetInformation.h"
#include "cute/cute.h"
#include "cute/cute_suite.h"

class TestDatasetInformation {
public:
	TestDatasetInformation();
	virtual ~TestDatasetInformation();

	void testCrossSectionTTJets();
	void testCrossSectionTTJetsFromArray();
	void testGetNumberOfProcessedEventsTTJets();
	void testGetNumberOfProcessedEventsTTJetsFromArray();
private:
	BAT::DatasetInformation datasetInfo_;

};

extern cute::suite make_suite_TestDatasetInformation();

#endif /* TESTMCINFORMATION_H_ */
