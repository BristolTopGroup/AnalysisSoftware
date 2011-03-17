/*
 * TestStringOperations.h
 *
 *  Created on: 11 Aug 2010
 *      Author: kreczko
 */

#include "cute/cute.h"
#include "cute/cute_suite.h"
#include "../interface/HistHelpers/StringOperations.h"

using namespace BAT;

struct TestStringOperations {
public:
    TestStringOperations() {

    }
    ~TestStringOperations() {

    }

    void testGetDirectoriesFromSimplePath() {
        ASSERT_EQUAL(1, getDirectoriesFromPath("dir").size());
        ASSERT_EQUAL("dir", getDirectoriesFromPath("dir").at(0));
    }

    void testGetDirectoriesFromSimplePathWithEndingSlash() {
        ASSERT_EQUAL(1, getDirectoriesFromPath("dir/").size());
        ASSERT_EQUAL("dir", getDirectoriesFromPath("dir/").at(0));
    }

    void testGetDirectoriesFromNestedPath() {
        ASSERT_EQUAL(2, getDirectoriesFromPath("dir/dir2").size());
        ASSERT_EQUAL("dir", getDirectoriesFromPath("dir/dir2").at(0));
        ASSERT_EQUAL("dir2", getDirectoriesFromPath("dir/dir2").at(1));
    }

    void testGetDirectoriesFromEmptyPath() {
        ASSERT_EQUAL(0, getDirectoriesFromPath("").size());
    }

    void testGetDirectoriesFromEmptyPathWithSlash() {
        ASSERT_EQUAL(0, getDirectoriesFromPath("/").size());
    }
};
extern cute::suite make_suite_TestStringOperations() {
    cute::suite s;

    s.push_back(CUTE_SMEMFUN(TestStringOperations, testGetDirectoriesFromSimplePath));
    s.push_back(CUTE_SMEMFUN(TestStringOperations, testGetDirectoriesFromSimplePathWithEndingSlash));
    s.push_back(CUTE_SMEMFUN(TestStringOperations, testGetDirectoriesFromNestedPath));
    s.push_back(CUTE_SMEMFUN(TestStringOperations, testGetDirectoriesFromEmptyPath));
    s.push_back(CUTE_SMEMFUN(TestStringOperations, testGetDirectoriesFromEmptyPathWithSlash));

    return s;
}
