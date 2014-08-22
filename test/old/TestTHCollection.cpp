/*
 * TestTH1Collection.h
 *
 *  Created on: 9 Aug 2010
 *      Author: kreczko
 */

#include "TestTHCollection.h"
#include <boost/filesystem.hpp>

using namespace BAT;

TestTHCollection::TestTHCollection() :
		histFile(new TFile("testTHCollection.root", "RECREATE")), collection(), collectionWithPath("mc/ttbar"), collection2D() {
}

TestTHCollection::~TestTHCollection() {
	if (boost::filesystem::exists("testTHCollection.root")) {
		boost::filesystem::remove("testTHCollection.root");
	}
}

void TestTHCollection::testInitialSize() {
	ASSERT_EQUAL(0, collection.size());
}

void TestTHCollection::testSize() {
	collection.add("test", "test", 100, 0, 100);
	ASSERT_EQUAL(1, collection.size());
}

void TestTHCollection::test2DSize() {
	collection2D.add("test", "test", 100, 0, 100, 50, 0, 50);
	ASSERT_EQUAL(1, collection2D.size());
}

void TestTHCollection::testInterference() {
	collection.add("test", "test", 100, 0, 100);
	collectionWithPath.add("test", "test", 100, 0, 100);
	collection.get("test")->Fill(1);
	collectionWithPath.get("test")->Fill(1);
	ASSERT_EQUAL(1, collectionWithPath.get("test")->GetEntries());
}

void TestTHCollection::testWriteInterference() {
	collection.add("test", "test", 100, 0, 100);
	collectionWithPath.add("test", "test", 100, 0, 100);
	collection.get("test")->Fill(1);
	collectionWithPath.get("test")->Fill(1);
	collection.writeToFile(histFile);
	collectionWithPath.writeToFile(histFile);
	ASSERT(histFile->Get("test") != 0);
	ASSERT(histFile->Get("mc/ttbar/test") != 0);
}

void TestTHCollection::testWriteFileWithoutFolder() {
	collection.add("test", "test", 100, 0, 100);
	collection.writeToFile(histFile);
	ASSERT(histFile->Get("test") != 0);
}

void TestTHCollection::testWriteFileWithFolderFirstDirectory() {
	collectionWithPath.add("test", "test", 100, 0, 100);
	collectionWithPath.writeToFile(histFile);
	ASSERT(histFile->Get("mc") != 0);

}

void TestTHCollection::testWriteFileWithFolderSubDirectory() {
	collectionWithPath.add("test", "test", 100, 0, 100);
	collectionWithPath.writeToFile(histFile);
	ASSERT(histFile->Get("mc/ttbar") != 0);
}

void TestTHCollection::testWriteFileWithFolder() {
	collectionWithPath.add("test", "test", 100, 0, 100);
	collectionWithPath.writeToFile(histFile);
	ASSERT(histFile->Get("mc/ttbar/test") != 0);
}

void TestTHCollection::testPrefix() {
	collectionWithPath.add("test", "test", 100, 0, 100);
	collectionWithPath.setPrefix("first");
	collectionWithPath.writeToFile(histFile);
	ASSERT(histFile->Get("mc/ttbar/first_test") != 0);
}

void TestTHCollection::testSuffix() {
	collectionWithPath.add("test", "test", 100, 0, 100);
	collectionWithPath.setSuffix("old");
	collectionWithPath.writeToFile(histFile);
	ASSERT(histFile->Get("mc/ttbar/test_old") != 0);
}

void TestTHCollection::testPrefixAndSuffix() {
	collectionWithPath.add("test", "test", 100, 0, 100);
	collectionWithPath.setPrefix("first");
	collectionWithPath.setSuffix("old");
	collectionWithPath.writeToFile(histFile);
	ASSERT(histFile->Get("mc/ttbar/first_test_old") != 0);
}

void TestTHCollection::containsHistogram() {
	collectionWithPath.add("test", "test", 100, 0, 100);
	ASSERT(collectionWithPath.contains("test"));
}

void TestTHCollection::doesNotContainHistogram() {
	ASSERT(collectionWithPath.contains("sdfsdf") == false);
}

cute::suite make_suite_TestTHCollection() {
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

	s.push_back(CUTE_SMEMFUN(TestTHCollection, containsHistogram));
	s.push_back(CUTE_SMEMFUN(TestTHCollection, doesNotContainHistogram));

	return s;
}

