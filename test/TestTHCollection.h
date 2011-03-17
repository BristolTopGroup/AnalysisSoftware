/*
 * TestTH1Collection.h
 *
 *  Created on: 9 Aug 2010
 *      Author: kreczko
 */

#include "cute/cute.h"
#include "cute/cute_suite.h"
#include "../interface/HistHelpers/THCollection.h"
#include "TH1F.h"
#include "TFile.h"
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

using namespace BAT;

struct TestTHCollection {
private:
    boost::shared_ptr<TFile> histFile;
    TH1Collection collection;
    TH1Collection collectionWithPath;
    TH2Collection collection2D;

public:
    TestTHCollection() :
        histFile(new TFile("testTHCollection.root", "RECREATE")), collection(), collectionWithPath("mc/ttbar"),
                collection2D() {
    }

    ~TestTHCollection() {
        if (boost::filesystem::exists("testTHCollection.root")) {
            boost::filesystem::remove("testTHCollection.root");
        }
    }

    void testInitialSize() {
        ASSERT_EQUAL(0, collection.size());
    }

    void testSize() {
        collection.add("test", "test", 100, 0, 100);
        ASSERT_EQUAL(1, collection.size());
    }

    void test2DSize() {
        collection2D.add("test", "test", 100, 0, 100, 50, 0, 50);
        ASSERT_EQUAL(1, collection2D.size());
    }

    void testInterference() {
        collection.add("test", "test", 100, 0, 100);
        collectionWithPath.add("test", "test", 100, 0, 100);
        collection.get("test")->Fill(1);
        collectionWithPath.get("test")->Fill(1);
        ASSERT_EQUAL(1, collectionWithPath.get("test")->GetEntries());
    }

    void testWriteInterference() {
        collection.add("test", "test", 100, 0, 100);
        collectionWithPath.add("test", "test", 100, 0, 100);
        collection.get("test")->Fill(1);
        collectionWithPath.get("test")->Fill(1);
        collection.writeToFile(histFile);
        collectionWithPath.writeToFile(histFile);
        ASSERT(histFile->Get("test") != 0);
        ASSERT(histFile->Get("mc/ttbar/test") != 0);
    }

    void testWriteFileWithoutFolder() {
        collection.add("test", "test", 100, 0, 100);
        collection.writeToFile(histFile);
        ASSERT(histFile->Get("test") != 0);
    }

    void testWriteFileWithFolderFirstDirectory() {
        collectionWithPath.add("test", "test", 100, 0, 100);
        collectionWithPath.writeToFile(histFile);
        ASSERT(histFile->Get("mc") != 0);

    }

    void testWriteFileWithFolderSubDirectory() {
        collectionWithPath.add("test", "test", 100, 0, 100);
        collectionWithPath.writeToFile(histFile);
        ASSERT(histFile->Get("mc/ttbar") != 0);
    }

    void testWriteFileWithFolder() {
        collectionWithPath.add("test", "test", 100, 0, 100);
        collectionWithPath.writeToFile(histFile);
        ASSERT(histFile->Get("mc/ttbar/test") != 0);
    }

    void testPrefix() {
        collectionWithPath.add("test", "test", 100, 0, 100);
        collectionWithPath.setPrefix("first");
        collectionWithPath.writeToFile(histFile);
        ASSERT(histFile->Get("mc/ttbar/first_test") != 0);
    }

    void testSuffix() {
        collectionWithPath.add("test", "test", 100, 0, 100);
        collectionWithPath.setSuffix("old");
        collectionWithPath.writeToFile(histFile);
        ASSERT(histFile->Get("mc/ttbar/test_old") != 0);
    }

    void testPrefixAndSuffix() {
        collectionWithPath.add("test", "test", 100, 0, 100);
        collectionWithPath.setPrefix("first");
        collectionWithPath.setSuffix("old");
        collectionWithPath.writeToFile(histFile);
        ASSERT(histFile->Get("mc/ttbar/first_test_old") != 0);
    }

};

extern cute::suite make_suite_TestTHCollection() {
    cute::suite s;

    s.push_back(CUTE_SMEMFUN(TestTHCollection, testInitialSize));
    s.push_back(CUTE_SMEMFUN(TestTHCollection, testSize));
    s.push_back(CUTE_SMEMFUN(TestTHCollection, test2DSize));
    s.push_back(CUTE_SMEMFUN(TestTHCollection, testInterference));
    s.push_back(CUTE_SMEMFUN(TestTHCollection, testWriteInterference));
    s.push_back(CUTE_SMEMFUN(TestTHCollection, testWriteFileWithoutFolder));
    s.push_back(CUTE_SMEMFUN(TestTHCollection, testWriteFileWithFolderFirstDirectory));
    s.push_back(CUTE_SMEMFUN(TestTHCollection, testWriteFileWithFolderSubDirectory));
    s.push_back(CUTE_SMEMFUN(TestTHCollection, testWriteFileWithFolder));
    s.push_back(CUTE_SMEMFUN(TestTHCollection, testPrefix));
    s.push_back(CUTE_SMEMFUN(TestTHCollection, testSuffix));
    s.push_back(CUTE_SMEMFUN(TestTHCollection, testPrefixAndSuffix));

    return s;
}

