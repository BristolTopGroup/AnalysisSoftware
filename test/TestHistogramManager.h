/*
 * TestTH1Collection.h
 *
 *  Created on: 9 Aug 2010
 *      Author: kreczko
 */

#include "cute/cute.h"
#include "cute/cute_suite.h"
#include "../interface/HistHelpers/HistogramManager.h"
#include <boost/array.hpp>
#include "../interface/DataTypes.h"
#include <boost/filesystem.hpp>

using namespace BAT;

struct TestHistogramManager {
private:
	HistogramManager man;
	float lumi;
	string expectedTtbarFile, expectedDataFile;
public:
	TestHistogramManager();
	~TestHistogramManager();

	void testPreparationData();

	void testPreparationTtbar();
	void testDataFile();
	void testPreparationData2D();
	void testPreparationDatajetBinned();
	void testPreparationDataBjetBinned();

	void testNumberOf1DJetHistsInFile();
	void testNumberOf2DJetHistsInFile();
	void testNumberOf3DJetHistsInFile();

	void testNumberOf1DBJetHistsInFile();
	void testNumberOf2DBJetHistsInFile();
	void testNumberOf3DBJetHistsInFile();

	void testJetBinnedHistInFile();
	void testAddCollection();
	void testAddCollectionWithoutAdd();
	void throwExceptionWhenAccessingNonExistentHistIn1DCollection();
	void throwExceptionWhenAccessingNonExistentHistIn1DBJetCollection();
	void throwExceptionWhenAccessingNonExistentHistIn1DJetCollection();
	void throwExceptionWhenAccessingNonExistentHistIn2DCollection();
	void throwExceptionWhenAccessingNonExistentHistIn2DBJetCollection();
	void throwExceptionWhenAccessingNonExistentHistIn2DJetCollection();
	void throwExceptionWhenAccessingNonExistentHistIn3DCollection();
	void throwExceptionWhenAccessingNonExistentHistIn3DBJetCollection();
	void throwExceptionWhenAccessingNonExistentHistIn3DJetCollection();

};

extern cute::suite make_suite_TestHistogramManager();

