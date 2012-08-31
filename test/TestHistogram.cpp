/*
 * TestHistogram.cpp
 *
 *  Created on: 28 Mar 2012
 *      Author: kreczko
 */

#include "TestHistogram.h"
using namespace BAT;

TestHistogram::TestHistogram() :
		hist1D_(new Histogram("Name", "Titles", 100, 0, 10)), //
		hist2D_(new Histogram("Name", "Titles", 100, 0, 10, 50, -3, 3)), //
		hist3D_(new Histogram("Name", "Titles", 100, 0, 10, 50, -3, 3, 20, 0, 4)) {

}

TestHistogram::~TestHistogram() {
}

void TestHistogram::test1DConstructor() {

	ASSERT_EQUAL(100, hist1D_->GetNumberOfXBins());
	ASSERT_EQUAL(0, hist1D_->GetMinX());
	ASSERT_EQUAL(10, hist1D_->GetMaxX());

	ASSERT_EQUAL(0, hist1D_->GetNumberOfYBins());
	ASSERT_EQUAL(0, hist1D_->GetMinY());
	ASSERT_EQUAL(0, hist1D_->GetMaxY());

	ASSERT_EQUAL(0, hist1D_->GetNumberOfZBins());
	ASSERT_EQUAL(0, hist1D_->GetMinZ());
	ASSERT_EQUAL(0, hist1D_->GetMaxZ());

	ASSERT_EQUAL("Name", hist1D_->GetName());
	ASSERT_EQUAL("Titles", hist1D_->GetTitle());
}

void TestHistogram::test2DConstructor() {

	ASSERT_EQUAL(100, hist2D_->GetNumberOfXBins());
	ASSERT_EQUAL(0, hist2D_->GetMinX());
	ASSERT_EQUAL(10, hist2D_->GetMaxX());

	ASSERT_EQUAL(50, hist2D_->GetNumberOfYBins());
	ASSERT_EQUAL(-3, hist2D_->GetMinY());
	ASSERT_EQUAL(3, hist2D_->GetMaxY());

	ASSERT_EQUAL(0, hist2D_->GetNumberOfZBins());
	ASSERT_EQUAL(0, hist2D_->GetMinZ());
	ASSERT_EQUAL(0, hist2D_->GetMaxZ());

	ASSERT_EQUAL("Name", hist2D_->GetName());
	ASSERT_EQUAL("Titles", hist2D_->GetTitle());
}

void TestHistogram::test3DConstructor() {

	ASSERT_EQUAL(100, hist3D_->GetNumberOfXBins());
	ASSERT_EQUAL(0, hist3D_->GetMinX());
	ASSERT_EQUAL(10, hist3D_->GetMaxX());

	ASSERT_EQUAL(50, hist3D_->GetNumberOfYBins());
	ASSERT_EQUAL(-3, hist3D_->GetMinY());
	ASSERT_EQUAL(3, hist3D_->GetMaxY());

	ASSERT_EQUAL(20, hist3D_->GetNumberOfZBins());
	ASSERT_EQUAL(0, hist3D_->GetMinZ());
	ASSERT_EQUAL(4, hist3D_->GetMaxZ());

	ASSERT_EQUAL("Name", hist3D_->GetName());
	ASSERT_EQUAL("Titles", hist3D_->GetTitle());
}

void TestHistogram::testFill1D() {
	hist1D_->Fill(5);
	ASSERT_EQUAL(1, hist1D_->GetEntries());
	ASSERT_EQUAL(1, hist1D_->GetBinContent(50));
}

cute::suite make_suite_TestHistogram() {
	cute::suite s;
	s.push_back(CUTE_SMEMFUN(TestHistogram, test1DConstructor));
	s.push_back(CUTE_SMEMFUN(TestHistogram, test2DConstructor));
	s.push_back(CUTE_SMEMFUN(TestHistogram, test3DConstructor));
	return s;
}
