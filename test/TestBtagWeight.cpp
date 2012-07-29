/*
 * TestBtagWeight.cpp
 *
 *  Created on: Jul 27, 2012
 *      Author: lkreczko
 */

#include "TestBtagWeight.h"

using namespace BAT;

TestBtagWeight::TestBtagWeight() {

}

TestBtagWeight::~TestBtagWeight() {
}
void TestBtagWeight::test0Bjets0Tags4Jets() {
	double effb(0.6), effc(0.08), effl(0.01);
	double sfb(0.95), sfc(0.95), sfl(1.1);
	unsigned int minTags(0), maxTags(10), nTags(0);
	unsigned int N_B(0), N_C(0), N_L(4);
	BTagWeight b(minTags, maxTags);

	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	ASSERT_EQUAL_DELTA(1., weight, 0.0001);
}

void TestBtagWeight::test1Bjets1Tags1Jet() {
	double effb(0.6), effc(0.08), effl(0.01);
	double sfb(0.95), sfc(0.95), sfl(1.1);
	unsigned int minTags(1), maxTags(1), nTags(1);
	unsigned int N_B(1), N_C(0), N_L(0);
	BTagWeight b(minTags, maxTags);

	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	ASSERT_EQUAL_DELTA(sfb, weight, 0.0001);
}

void TestBtagWeight::test1Bjets0Tags1Jet() {
	double effb(0.6), effc(0.08), effl(0.01);
	double sfb(0.95), sfc(0.95), sfl(1.1);
	unsigned int minTags(0), maxTags(0), nTags(0);
	unsigned int N_B(1), N_C(0), N_L(0);
	BTagWeight b(minTags, maxTags);

	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	ASSERT_EQUAL_DELTA(1 - sfb, weight, 0.0001);
}

void TestBtagWeight::test2Bjets2Tags2Jets() {
	double effb(0.6), effc(0.08), effl(0.01);
	double sfb(0.95), sfc(0.95), sfl(1.1);
	unsigned int minTags(2), maxTags(2), nTags(2);
	unsigned int N_B(2), N_C(0), N_L(0);
	BTagWeight b(minTags, maxTags);

	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	ASSERT_EQUAL_DELTA(pow(sfb,2), weight, 0.001);
}

void TestBtagWeight::test0Bjets1Tags4Jets() {
	double effb(0.6), effc(0.08), effl(0.01);
	double sfb(0.95), sfc(0.95), sfl(1.1);
	unsigned int minTags(1), maxTags(2), nTags(1);
	unsigned int N_B(0), N_C(0), N_L(4);
	BTagWeight b(minTags, maxTags);

	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	ASSERT_EQUAL_DELTA(1-pow(1-effl*sfl,4), weight, 0.00001);
}

void TestBtagWeight::test1Bjets1Tags4Jets() {
	double effb(0.6), effc(0.08), effl(0.01);
	double sfb(0.95), sfc(0.95), sfl(1.1);
	unsigned int minTags(1), maxTags(20), nTags(1);
	unsigned int N_B(1), N_C(0), N_L(3);
	BTagWeight b(minTags, maxTags);

	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	ASSERT_EQUAL_DELTA(1-pow(1-effl*sfl,4), weight, 0.00001);
}

cute::suite make_suite_TestBtagWeight() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test0Bjets0Tags4Jets));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test1Bjets1Tags1Jet));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test1Bjets0Tags1Jet));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test2Bjets2Tags2Jets));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test0Bjets1Tags4Jets));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test1Bjets1Tags4Jets));
	return s;
}
