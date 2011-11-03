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
    TestHistogramManager() :
        man(),
        lumi(1.5),
        expectedTtbarFile("ttjet_1.5pb_CaloElectron_CaloJets_CaloMET.root"),
        expectedDataFile("data_1.5pb_CaloElectron_CaloJets_CaloMET.root") {
        man.enableDebugMode(true);
        boost::array<bool, DataType::NUMBER_OF_DATA_TYPES> seenDataTypes = boost::array<bool,
                DataType::NUMBER_OF_DATA_TYPES>();

        seenDataTypes[DataType::DATA] = true;
        seenDataTypes[DataType::ttbar] = true;

//        man.setCurrentLumi(lumi);
        man.prepareForSeenDataTypes(seenDataTypes);
    }

    ~TestHistogramManager() {
        if (boost::filesystem::exists(expectedDataFile))
            boost::filesystem::remove(expectedDataFile);
        if (boost::filesystem::exists(expectedTtbarFile))
            boost::filesystem::remove(expectedTtbarFile);
    }

    void testPreparationData() {
        man.addH1D("myHist", "Testing", 10, 0., 1);
        man.setCurrentDataType(DataType::DATA);
        man.setCurrentCollection("");
        ASSERT(man.H1D("myHist") != 0);
    }

    void testPreparationTtbar() {
        man.addH1D("myHist", "Testing", 10, 0., 1);
        man.setCurrentDataType(DataType::ttbar);
        man.setCurrentCollection("");
        ASSERT(man.H1D("myHist") != 0);
    }

    void testDataFile() {
        man.writeToDisk();
        ASSERT(boost::filesystem::exists(expectedDataFile));
    }

    void testPreparationData2D() {
        man.addH2D("myHist2D", "Testing", 10, 0., 1, 10, 0., 1);
        man.setCurrentDataType(DataType::DATA);
        man.setCurrentCollection("");
        ASSERT(man.H2D("myHist2D") != 0);
    }

    void testPreparationDatajetBinned() {
        man.addH1D_JetBinned("myHistJeted", "Testing", 10, 0., 1);
        man.setCurrentDataType(DataType::DATA);
        man.setCurrentJetBin(0);
        man.setCurrentCollection("");
        ASSERT(man.H1D_JetBinned("myHistJeted") != 0);
    }

    void testNumberOf1DJetHistsInFile() {
        man.addH1D_JetBinned("myHistJeted", "Testing", 10, 0., 1);
        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT_EQUAL(JetBin::NUMBER_OF_JET_BINS + JetBinSummed::NUMBER_OF_SUMMED_JET_BINS, file->GetNkeys());
    }

    void testNumberOf2DJetHistsInFile() {
        man.addH2D_JetBinned("myHistJeted", "Testing", 10, 0., 1, 10, 0., 1);
        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT_EQUAL(JetBin::NUMBER_OF_JET_BINS + JetBinSummed::NUMBER_OF_SUMMED_JET_BINS, file->GetNkeys());
    }

    void testNumberOf1DBJetHistsInFile() {
        man.addH1D_BJetBinned("myHist", "Testing", 10, 0., 1);
        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT_EQUAL(BJetBin::NUMBER_OF_BJET_BINS + BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS, file->GetNkeys());
    }

    void testNumberOf2DBJetHistsInFile() {
        man.addH2D_BJetBinned("myHist", "Testing", 10, 0., 1, 10, 0., 1);
        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT_EQUAL(BJetBin::NUMBER_OF_BJET_BINS + BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS, file->GetNkeys());
    }

    void testJetBinnedHistInFile() {
        man.addH1D_JetBinned("myHistJeted", "Testing", 10, 0., 1);
        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT(file->Get("myHistJeted_0jet") != 0);
    }

    void test1DCollection() {
        man.add1DCollection("1D");
        man.setCurrent1DCollection("1D");
        man.addH1D("myHist", "Testing", 10, 0., 1);

        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT(file->Get("1D/myHist") != 0);
    }

    void testAdd1DBjetBinnedCollection() {
        man.add1DBJetCollection("1DBjet");
        man.setCurrent1DBJetCollection("1DBjet");
        man.addH1D_BJetBinned("myHist", "Testing", 10, 0., 1);

        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT(file->Get("1DBjet/myHist_0btag") != 0);
    }

    void testAdd1DJetBinnedCollection() {
        man.add1DBJetCollection("1Djet");
        man.setCurrent1DJetCollection("1Djet");
        man.addH1D_JetBinned("myHist", "Testing", 10, 0., 1);

        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT(file->Get("1Djet/myHist_0jet") != 0);
    }

    void test2DCollection() {
        man.add2DCollection("2D");
        man.setCurrent2DCollection("2D");
        man.addH2D("myHist", "Testing", 10, 0., 1, 10, 0.1, 1);

        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT(file->Get("2D/myHist") != 0);
    }

    void testAdd2DBjetBinnedCollection() {
        man.add2DBJetCollection("2DBjet");
        man.setCurrent2DBJetCollection("2DBjet");
        man.addH2D_BJetBinned("myHist", "Testing", 10, 0., 1, 10, 0.1, 1);

        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT(file->Get("2DBjet/myHist_0btag") != 0);
    }

    void testAdd2DJetBinnedCollection() {
        man.add2DBJetCollection("2Djet");
        man.setCurrent2DJetCollection("2Djet");
        man.addH2D_JetBinned("myHist", "Testing", 10, 0., 1, 10, 0.1, 1);

        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT(file->Get("2Djet/myHist_0jet") != 0);
    }

    void testAddCollection(){
        man.addCollection("test");
        man.setCurrentCollection("test");
        man.addH1D("myHist", "Testing", 10, 0., 1);
        man.addH1D_JetBinned("myHist", "Testing", 10, 0., 1);
        man.addH1D_BJetBinned("myHist", "Testing", 10, 0., 1);

        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT(file->Get("test/myHist") != 0);
        ASSERT(file->Get("test/myHist_0jet") != 0);
        ASSERT(file->Get("test/myHist_0btag") != 0);
    }


    void testAddCollectionWithoutAdd(){
        man.setCurrentCollection("test");
        man.addH1D("myHist", "Testing", 10, 0., 1);
        man.addH1D_JetBinned("myHist", "Testing", 10, 0., 1);
        man.addH1D_BJetBinned("myHist", "Testing", 10, 0., 1);

        man.writeToDisk();
        boost::shared_ptr<TFile> file(new TFile(expectedDataFile.c_str()));
        ASSERT(file->Get("test/myHist") != 0);
        ASSERT(file->Get("test/myHist_0jet") != 0);
        ASSERT(file->Get("test/myHist_0btag") != 0);
    }

    void throwExceptionWhenAccessingNonExistentHistIn1DCollection() {
        man.setCurrentCollection("test");
        ASSERT_THROWS(man.H1D("tfdgsdf")->Fill(12), HistogramAccessException);
    }

    void throwExceptionWhenAccessingNonExistentHistIn1DBJetCollection() {
        man.setCurrentCollection("test");
        ASSERT_THROWS(man.H1D_BJetBinned("tfdgsdf")->Fill(12), HistogramAccessException);
    }

    void throwExceptionWhenAccessingNonExistentHistIn1DJetCollection() {
        man.setCurrentCollection("test");
        ASSERT_THROWS(man.H1D_JetBinned("tfdgsdf")->Fill(12), HistogramAccessException);
    }

    void throwExceptionWhenAccessingNonExistentHistIn2DCollection() {
        man.setCurrentCollection("test");
        ASSERT_THROWS(man.H2D("tfdgsdf")->Fill(12), HistogramAccessException);
    }

    void throwExceptionWhenAccessingNonExistentHistIn2DBJetCollection() {
        man.setCurrentCollection("test");
        ASSERT_THROWS(man.H2D_BJetBinned("tfdgsdf")->Fill(12), HistogramAccessException);
    }

    void throwExceptionWhenAccessingNonExistentHistIn2DJetCollection() {
        man.setCurrentCollection("test");
        ASSERT_THROWS(man.H2D_JetBinned("tfdgsdf")->Fill(12), HistogramAccessException);
    }


};

extern cute::suite make_suite_TestHistogramManager() {
    cute::suite s;

    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testPreparationData));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testPreparationTtbar));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testDataFile));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testPreparationData2D));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testPreparationDatajetBinned));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testNumberOf1DJetHistsInFile));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testNumberOf2DJetHistsInFile));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testNumberOf1DBJetHistsInFile));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testNumberOf2DBJetHistsInFile));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testJetBinnedHistInFile));

    s.push_back(CUTE_SMEMFUN(TestHistogramManager, test1DCollection));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testAdd1DBjetBinnedCollection));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testAdd1DJetBinnedCollection));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, test2DCollection));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testAdd2DBjetBinnedCollection));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testAdd2DJetBinnedCollection));

    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testAddCollection));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, testAddCollectionWithoutAdd));

    s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn1DCollection));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn1DBJetCollection));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn1DJetCollection));

    s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn2DCollection));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn2DBJetCollection));
    s.push_back(CUTE_SMEMFUN(TestHistogramManager, throwExceptionWhenAccessingNonExistentHistIn2DJetCollection));

    return s;
}

