/*
 * TestTH1Collection.h
 *
 *  Created on: 9 Aug 2010
 *      Author: kreczko
 */
#ifndef TESTTHCOLLECTION_h
#define TESTTHCOLLECTION_h
#include "cute/cute.h"
#include "cute/cute_suite.h"
#include "../interface/HistHelpers/THCollection.h"
#include "TH1F.h"
#include "TFile.h"
#include <boost/shared_ptr.hpp>

using namespace BAT;

struct TestTHCollection {
private:
    boost::shared_ptr<TFile> histFile;
    TH1Collection collection;
    TH1Collection collectionWithPath;
    TH2Collection collection2D;

public:
    TestTHCollection();
    ~TestTHCollection();

    void testInitialSize();
    void testSize();
    void test2DSize();
    void testInterference();
    void testWriteInterference();
    void testWriteFileWithoutFolder();
    void testWriteFileWithFolderFirstDirectory();
    void testWriteFileWithFolderSubDirectory();
    void testWriteFileWithFolder();
    void testPrefix();
    void testSuffix();
    void testPrefixAndSuffix();
    void containsHistogram();
    void doesNotContainHistogram();

};

extern cute::suite make_suite_TestTHCollection();
#endif

