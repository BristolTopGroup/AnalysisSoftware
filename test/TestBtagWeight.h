/*
 * TestBtagWeight.h
 *
 *  Created on: Jul 27, 2012
 *      Author: lkreczko
 */

#ifndef TESTBTAGWEIGHT_H_
#define TESTBTAGWEIGHT_H_
#include "../interface/BTagWeight.h"
#include "cute/cute.h"
#include "cute/cute_suite.h"

class TestBtagWeight {
public:
	TestBtagWeight();
	virtual ~TestBtagWeight();
	void test0Bjets0Tags4Jets();
	void test1Bjets1Tags1Jet();
	void test1Bjets0Tags1Jet();
	void test1Bjets0Tags1Jet_vec();
	void test2Bjets2Tags2Jets();
	void test0Bjets1Tags4Jets();
	void test1Bjets1Tags4Jets();
	void test0Tag_vec();
	void test1Tag_vec();
	void test2Tag_vec();
};

extern cute::suite make_suite_TestBtagWeight();

#endif /* TESTBTAGWEIGHT_H_ */
