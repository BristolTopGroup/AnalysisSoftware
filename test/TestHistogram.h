/*
 * TestHistogram.h
 *
 *  Created on: 28 Mar 2012
 *      Author: kreczko
 */

#ifndef TESTHISTOGRAM_H_
#define TESTHISTOGRAM_H_
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/HistHelpers/Histogram.h"

class TestHistogram {
public:
	TestHistogram();
	virtual ~TestHistogram();

	void test1DConstructor();
	void test2DConstructor();
	void test3DConstructor();

	void testFill1D();
	void testFill2D();
	void testFill3D();

	void testFill1DWithWeight();
	void testFill2DWithWeight();
	void testFill3DWithWeight();

private:
	BAT::HistogramPtr hist1D_, hist2D_, hist3D_;
};

extern cute::suite make_suite_TestHistogram();

#endif /* TESTHISTOGRAM_H_ */
