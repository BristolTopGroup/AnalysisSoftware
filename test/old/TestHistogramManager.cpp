/*
 * TestTH1Collection.h
 *
 *  Created on: 9 Aug 2010
 *      Author: kreczko
 */

#include "TestHistogramManager.h"
#include <boost/array.hpp>
#include "../interface/DataTypes.h"
#include <boost/filesystem.hpp>
#include "../interface/GlobalVariables.h"

using namespace BAT;

TestHistogramManager::TestHistogramManager() :
		man(), //
		lumi(1.5), //
		expectedTtbarFile("TTJet_1.5pb_PFElectron_PFMuon_PF2PATJets_patType1CorrectedPFMet.root"), //
		expectedDataFile("ElectronHad_1.5pb_PFElectron_PFMuon_PF2PATJets_patType1CorrectedPFMet.root") {
	man.enableDebugMode(true);
	boost::array<bool, DataType::NUMBER_OF_DATA_TYPES> seenDataTypes =
			boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>();

	seenDataTypes[DataType::ElectronHad] = true;
	seenDataTypes[DataType::TTJets] = true;
	Globals::luminosity = lumi;
//        man.setCurrentLumi(lumi);
	man.prepareForSeenDataTypes(seenDataTypes);
}

TestHistogramManager::~TestHistogramManager() {
	if (boost::filesystem::exists(expectedDataFile))
		boost::filesystem::remove(expectedDataFile);
	if (boost::filesystem::exists(expectedTtbarFile))
		boost::filesystem::remove(expectedTtbarFile);
}

void TestHistogramManager::testPreparationData() {
	man.addH1D("myHist", "Testing", 10, 0., 1);
	man.setCurrentDataType(DataType::ElectronHad);
	man.setCurrentHistogramFolder("");
	ASSERT(man.H1D("myHist") != 0);
}

void TestHistogramManager::testPreparationTtbar() {
	man.addH1D("myHist", "Testing", 10, 0., 1);
	man.setCurrentDataType(DataType::TTJets);
	man.setCurrentHistogramFolder("");
	ASSERT(man.H1D("myHist") != 0);
}

void TestHistogramManager::testDataFile() {
	man.writeToDisk();
	ASSERT(boost::filesystem::exists(expectedDataFile));
}

void TestHistogramManager::testPreparationData2D() {
	man.addH2D("myHist2D", "Testing", 10, 0., 1, 10, 0., 1);
	man.setCurrentDataType(DataType::ElectronHad);
	man.setCurrentHistogramFolder("");
	ASSERT(man.H2D("myHist2D") != 0);
}

void TestHistogramManager::testPreparationDatajetBinned() {
	man.addH1D_JetBinned("myHistJeted", "Testing", 10, 0., 1);
	man.setCurrentDataType(DataType::ElectronHad);
	man.setCurrentJetBin(0);
	man.setCurrentHistogramFolder("");
	ASSERT(man.H1D_JetBinned("myHistJeted") != 0);
}

void TestHistogramManager::testPreparationDataBjetBinned() {
	man.addH1D_BJetBinned("myHistJeted", "Testing", 10, 0., 1);
	man.setCurrentDataType(DataType::ElectronHad);
	man.setCurrentBJetBin(0);
	man.setCurrentHistogramFolder("");
	ASSERT(man.H1D_BJetBinned("myHistJeted") != 0);
}

void TestHistogramManager::testNumberOf1DJetHistsInFile() {
	man.addH1D_JetBinned("myHistJeted", "Testing", 10, 0., 1);
	man.writeToDisk();
	boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
	ASSERT_EQUAL(JetBin::NUMBER_OF_JET_BINS + JetBinSummed::NUMBER_OF_SUMMED_JET_BINS, file->GetNkeys());
}

void TestHistogramManager::testNumberOf2DJetHistsInFile() {
	man.addH2D_JetBinned("myHistJeted", "Testing", 10, 0., 1, 10, 0., 1);
	man.writeToDisk();
	boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
	ASSERT_EQUAL(JetBin::NUMBER_OF_JET_BINS + JetBinSummed::NUMBER_OF_SUMMED_JET_BINS, file->GetNkeys());
}

void TestHistogramManager::testNumberOf3DJetHistsInFile() {
	man.addH3D_JetBinned("myHistJeted", "Testing", 10, 0., 1, 10, 0., 1, 10, 0., 1);
	man.writeToDisk();
	boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
	ASSERT_EQUAL(JetBin::NUMBER_OF_JET_BINS + JetBinSummed::NUMBER_OF_SUMMED_JET_BINS, file->GetNkeys());
}

void TestHistogramManager::testNumberOf1DBJetHistsInFile() {
	man.addH1D_BJetBinned("myHist", "Testing", 10, 0., 1);
	man.writeToDisk();
	boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
	ASSERT_EQUAL(BJetBin::NUMBER_OF_BJET_BINS + BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS, file->GetNkeys());
}

void TestHistogramManager::testNumberOf2DBJetHistsInFile() {
	man.addH2D_BJetBinned("myHist", "Testing", 10, 0., 1, 10, 0., 1);
	man.writeToDisk();
	boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
	ASSERT_EQUAL(BJetBin::NUMBER_OF_BJET_BINS + BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS, file->GetNkeys());
}

void TestHistogramManager::testNumberOf3DBJetHistsInFile() {
	man.addH3D_BJetBinned("myHist", "Testing", 10, 0., 1, 10, 0., 1, 10, 0., 1);
	man.writeToDisk();
	boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
	ASSERT_EQUAL(BJetBin::NUMBER_OF_BJET_BINS + BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS, file->GetNkeys());
}

void TestHistogramManager::testJetBinnedHistInFile() {
	man.addH1D_JetBinned("myHistJeted", "Testing", 10, 0., 1);
	man.writeToDisk();
	boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
	ASSERT(file->Get("myHistJeted_0jet") != 0);
}

void TestHistogramManager::testAddCollection() {
	man.addHistogramFolder("test");
	man.setCurrentHistogramFolder("test");
	man.addH1D("myHist", "Testing", 10, 0., 1);
	man.addH1D_JetBinned("myHist", "Testing", 10, 0., 1);
	man.addH1D_BJetBinned("myHist", "Testing", 10, 0., 1);

	man.writeToDisk();
	boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
	ASSERT(file->Get("test/myHist") != 0);
	ASSERT(file->Get("test/myHist_0jet") != 0);
	ASSERT(file->Get("test/myHist_0btag") != 0);
}

void TestHistogramManager::testAddCollectionWithoutAdd() {
	man.setCurrentHistogramFolder("test");
	man.addH1D("myHist", "Testing", 10, 0., 1);
	man.addH1D_JetBinned("myHist", "Testing", 10, 0., 1);
	man.addH1D_BJetBinned("myHist", "Testing", 10, 0., 1);

	man.writeToDisk();
	boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
	ASSERT(file->Get("test/myHist") != 0);
	ASSERT(file->Get("test/myHist_0jet") != 0);
	ASSERT(file->Get("test/myHist_0btag") != 0);
}

void TestHistogramManager::throwExceptionWhenAccessingNonExistentHistIn1DCollection() {
	man.setCurrentHistogramFolder("test");
	ASSERT_THROWS(man.H1D("tfdgsdf")->Fill(12), HistogramAccessException);
}

void TestHistogramManager::throwExceptionWhenAccessingNonExistentHistIn1DBJetCollection() {
	man.setCurrentHistogramFolder("test");
	ASSERT_THROWS(man.H1D_BJetBinned("tfdgsdf")->Fill(12), HistogramAccessException);
}

void TestHistogramManager::throwExceptionWhenAccessingNonExistentHistIn1DJetCollection() {
	man.setCurrentHistogramFolder("test");
	ASSERT_THROWS(man.H1D_JetBinned("tfdgsdf")->Fill(12), HistogramAccessException);
}

void TestHistogramManager::throwExceptionWhenAccessingNonExistentHistIn2DCollection() {
	man.setCurrentHistogramFolder("test");
	ASSERT_THROWS(man.H2D("tfdgsdf")->Fill(12,12), HistogramAccessException);
}

void TestHistogramManager::throwExceptionWhenAccessingNonExistentHistIn2DBJetCollection() {
	man.setCurrentHistogramFolder("test");
	ASSERT_THROWS(man.H2D_BJetBinned("tfdgsdf")->Fill(12,12), HistogramAccessException);
}

void TestHistogramManager::throwExceptionWhenAccessingNonExistentHistIn2DJetCollection() {
	man.setCurrentHistogramFolder("test");
	ASSERT_THROWS(man.H2D_JetBinned("tfdgsdf")->Fill(12,12), HistogramAccessException);
}

void TestHistogramManager::throwExceptionWhenAccessingNonExistentHistIn3DCollection() {
	man.setCurrentHistogramFolder("test");
	ASSERT_THROWS(man.H3D("tfdgsdf")->Fill(12,12,12), HistogramAccessException);
}

void TestHistogramManager::throwExceptionWhenAccessingNonExistentHistIn3DBJetCollection() {
	man.setCurrentHistogramFolder("test");
	ASSERT_THROWS(man.H3D_BJetBinned("tfdgsdf")->Fill(12,12,12), HistogramAccessException);
}

void TestHistogramManager::throwExceptionWhenAccessingNonExistentHistIn3DJetCollection() {
	man.setCurrentHistogramFolder("test");
	ASSERT_THROWS(man.H3D_JetBinned("tfdgsdf")->Fill(12,12,12), HistogramAccessException);
}

extern cute::suite make_suite_TestHistogramManager() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testPreparationData));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testPreparationTtbar));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testDataFile));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testPreparationData2D));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testPreparationDatajetBinned));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testPreparationDataBjetBinned));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testNumberOf1DJetHistsInFile));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testNumberOf2DJetHistsInFile));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testNumberOf3DJetHistsInFile));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testNumberOf1DBJetHistsInFile));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testNumberOf2DBJetHistsInFile));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testNumberOf3DBJetHistsInFile));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testJetBinnedHistInFile));

	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testAddCollection));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, testAddCollectionWithoutAdd));

	s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn1DCollection));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn1DBJetCollection));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn1DJetCollection));

	s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn2DCollection));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn2DBJetCollection));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn2DJetCollection));

	s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn3DCollection));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn3DBJetCollection));
	s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn3DJetCollection));

	return s;
}

