/*
 * TestStringOperations.h
 *
 *  Created on: 11 Aug 2010
 *      Author: kreczko
 */

#ifndef TESTSTRINGOPERATION_h
#define TESTSTRINGOPERATION_h
#include "cute/cute.h"
#include "cute/cute_suite.h"

struct TestStringOperations {
public:
    TestStringOperations();
    ~TestStringOperations();

    void testGetDirectoriesFromSimplePath();
    void testGetDirectoriesFromSimplePathWithEndingSlash();
    void testGetDirectoriesFromNestedPath();
    void testGetDirectoriesFromEmptyPath();
    void testGetDirectoriesFromEmptyPathWithSlash();
};
extern cute::suite make_suite_TestStringOperations();
#endif
