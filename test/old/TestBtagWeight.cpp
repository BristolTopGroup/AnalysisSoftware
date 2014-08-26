/*
 * TestBtagWeight.cpp
 *
 *  Created on: Jul 27, 2012
 *      Author: lkreczko
 */

#include "TestBtagWeight.h"
#include "../interface/BTagWeight2011.h"
#include "../interface/RecoObjects/Jet.h"
#include <vector>
#include <functional>
#include <numeric>

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
	BTagWeight2011 b = BTagWeight2011();
	b.setNumberOfBtags(minTags, maxTags);

	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	ASSERT_EQUAL_DELTA(1., weight, 0.0001);
}

void TestBtagWeight::test1Bjets1Tags1Jet() {
	double effb(0.6), effc(0.08), effl(0.01);
	double sfb(0.95), sfc(0.95), sfl(1.1);
	unsigned int minTags(1), maxTags(1), nTags(1);
	unsigned int N_B(1), N_C(0), N_L(0);
	BTagWeight2011 b = BTagWeight2011();
	b.setNumberOfBtags(minTags, maxTags);

	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	ASSERT_EQUAL_DELTA(sfb, weight, 0.0001);
}

void TestBtagWeight::test1Bjets0Tags1Jet() {
	double effb(0.6), effc(0.08), effl(0.01);
	double sfb(0.95), sfc(0.95), sfl(1.1);
	unsigned int minTags(1), maxTags(1), nTags(1);
	unsigned int N_B(1), N_C(0), N_L(0);
	BTagWeight2011 b = BTagWeight2011();
	b.setNumberOfBtags(minTags, maxTags);

	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	ASSERT_EQUAL_DELTA(1 - sfb, 1 - weight, 0.0001);
}

void TestBtagWeight::test1Bjets0Tags1Jet_vec() {
	double sfb(0.95);
	unsigned int minTags(0), maxTags(1), nTags(1);
	BTagWeight2011 b = BTagWeight2011();
	b.setNumberOfBtags(minTags, maxTags);

	std::vector<double> weights = b.weights(sfb, nTags);
	ASSERT_EQUAL(nTags + 1, weights.size());
	ASSERT_EQUAL_DELTA(1-pow(sfb,1), weights.at(0), 0.0001);
	ASSERT_EQUAL_DELTA(sfb, weights.at(1), 0.0001);
}

void TestBtagWeight::test0Tag_vec() {
	double sfb(0.95);
	unsigned int minTags(0), maxTags(1), nTags(0);
	BTagWeight2011 b = BTagWeight2011();
	b.setNumberOfBtags(minTags, maxTags);

	std::vector<double> weights = b.weights(sfb, nTags);
	ASSERT_EQUAL(nTags + 1, weights.size());
	ASSERT_EQUAL_DELTA(1, weights.at(0), 0.0001);
	ASSERT_EQUAL_DELTA(1, std::accumulate(weights.begin(), weights.end(), 0.0), 0.0001);
}

void TestBtagWeight::test1Tag_vec() {
	double sfb(0.95);
	unsigned int minTags(0), maxTags(1), nTags(1);
	BTagWeight2011 b = BTagWeight2011();
	b.setNumberOfBtags(minTags, maxTags);

	std::vector<double> weights = b.weights(sfb, nTags);
	ASSERT_EQUAL(nTags + 1, weights.size());
	ASSERT_EQUAL_DELTA(pow(1-sfb,1), weights.at(0), 0.0001);
	ASSERT_EQUAL_DELTA(sfb, weights.at(1), 0.0001);
	ASSERT_EQUAL_DELTA(1, std::accumulate(weights.begin(), weights.end(), 0.0), 0.0001);
}

void TestBtagWeight::test2Tag_vec() {
	unsigned int minTags(0), maxTags(1), nTags(2);
	double sfb(0.95);

	BTagWeight2011 b = BTagWeight2011();
	b.setNumberOfBtags(minTags, maxTags);

	std::vector<double> weights = b.weights(sfb, nTags);
	ASSERT_EQUAL(nTags + 1, weights.size());
	ASSERT_EQUAL_DELTA(pow(1-sfb,2), weights.at(0), 0.0001);
	ASSERT_EQUAL_DELTA(sfb*(1-sfb)*2, weights.at(1), 0.0001);
	ASSERT_EQUAL_DELTA(pow(sfb, 2), weights.at(2), 0.0001);
	ASSERT_EQUAL_DELTA(1, std::accumulate(weights.begin(), weights.end(), 0.0), 0.0001);
}

void TestBtagWeight::test2Bjets2Tags2Jets() {
	double effb(0.6), effc(0.08), effl(0.01);
	double sfb(0.95), sfc(0.95), sfl(1.1);
	unsigned int minTags(2), maxTags(2), nTags(2);
	unsigned int N_B(2), N_C(0), N_L(0);
	BTagWeight2011 b = BTagWeight2011();
	b.setNumberOfBtags(minTags, maxTags);

	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	ASSERT_EQUAL_DELTA(pow(sfb,2), weight, 0.001);
}

//void TestBtagWeight::test2Bjets1Tags2Jets() {
//	double effb(0.6), effc(0.08), effl(0.01);
//	double sfb(0.95), sfc(0.95), sfl(1.1);
//	unsigned int minTags(1), maxTags(1), nTags(1);
//	unsigned int N_B(2), N_C(0), N_L(0);
//	BTagWeight2011 b = BTagWeight2011();
//	b.setNumberOfBtags(minTags, maxTags);
//
//	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
//	ASSERT_EQUAL_DELTA(sfb*(1-sfb)*2, weight, 0.001);
//}

//void TestBtagWeight::test2Bjets0Tags2Jets() {
//	double effb(0.6), effc(0.08), effl(0.01);
//	double sfb(0.95), sfc(0.95), sfl(1.1);
//	unsigned int minTags(0), maxTags(0), nTags(0);
//	unsigned int N_B(2), N_C(0), N_L(0);
//	BTagWeight2011 b = BTagWeight2011();
//	b.setNumberOfBtags(minTags, maxTags);
//
//	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
//	ASSERT_EQUAL_DELTA(pow(1-sfb, 2), weight, 0.001);
//}

void TestBtagWeight::test0Bjets1Tags4Jets() {
	double effb(0.6), effc(0.08), effl(0.01);
	double sfb(0.95), sfc(0.95), sfl(1.1);
	unsigned int minTags(1), maxTags(20), nTags(1);
	unsigned int N_B(0), N_C(0), N_L(4);
	BTagWeight2011 b = BTagWeight2011();
	b.setNumberOfBtags(minTags, maxTags);

	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	ASSERT_EQUAL_DELTA((1-pow(1-effl*sfl,4))/(1-pow(1-effl,4)), weight, 0.00001);
}

//void TestBtagWeight::test1Bjets1Tags4Jets() {
//	double effb(0.6), effc(0.08), effl(0.01);
//	double sfb(0.95), sfc(0.95), sfl(1.1);
//	unsigned int minTags(1), maxTags(20), nTags(1);
//	unsigned int N_B(1), N_C(0), N_L(3);
//	BTagWeight2011 b = BTagWeight2011();
//	b.setNumberOfBtags(minTags, maxTags);
//
//	double weight = b.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
//	ASSERT_EQUAL_DELTA(1-pow(1-effl*sfl,4), weight, 0.00001);
//}

void TestBtagWeight::testAnalysisCase() {
	double effb(0.6), effc(0.08), effl(0.01);
	double sfb(0.95), sfc(0.95), sfl(1.1);
	unsigned int nTags(4);
	unsigned int N_B(4), N_C(0), N_L(0);

	BTagWeight2011 b_0 = BTagWeight2011();
	BTagWeight2011 b_1 = BTagWeight2011();
	BTagWeight2011 b_2 = BTagWeight2011();
	BTagWeight2011 b_3 = BTagWeight2011();
	BTagWeight2011 b_4 = BTagWeight2011();
	b_0.setNumberOfBtags(0, 20);
	b_1.setNumberOfBtags(1, 20);
	b_2.setNumberOfBtags(2, 20);
	b_3.setNumberOfBtags(3, 20);
	b_4.setNumberOfBtags(4, 20);

	double weight_0 = b_0.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	double weight_1 = b_1.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	double weight_2 = b_2.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	double weight_3 = b_3.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);
	double weight_4 = b_4.weight(N_B, N_C, N_L, effb, effc, effl, sfb, sfc, sfl, nTags);

	double weight_3btag = weight_3 - weight_4;
	double weight_2btag = weight_2 - weight_3;
	double weight_1btag = weight_1 - weight_2;
	double weight_0btag = weight_0 - weight_1;
	double sum = weight_0btag + weight_1btag + weight_2btag + weight_3btag + weight_4;

	ASSERT_EQUAL_DELTA(1, weight_0, 0.00001);
//	ASSERT_EQUAL_DELTA(pow(1-sfb,4), weight_0btag, 0.00001);
	ASSERT_EQUAL_DELTA(weight_2, weight_2btag + weight_3btag + weight_4, 0.00001);
	ASSERT_EQUAL_DELTA(1, sum, 0.00001);
}

void TestBtagWeight::testAnalysisCaseWithJetInput() {

	JetCollection jets;
	JetPointer jet(new Jet(50,50,0,0));
	jet->setPartonFlavour(5);//bjet
	jets.push_back(jet);
	jets.push_back(jet);
	jets.push_back(jet);
	jets.push_back(jet);

	std::vector<double> weights = BjetWeights2011(jets, 4);

	ASSERT_EQUAL_DELTA(1, std::accumulate(weights.begin(), weights.end(), 0.0), 0.0001);
}
cute::suite make_suite_TestBtagWeight() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test0Bjets0Tags4Jets));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test1Bjets1Tags1Jet));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test1Bjets0Tags1Jet));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test1Bjets0Tags1Jet_vec));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test0Tag_vec));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test1Tag_vec));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test2Tag_vec));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test2Bjets2Tags2Jets));
//	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test2Bjets1Tags2Jets));
//	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test2Bjets0Tags2Jets));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test0Bjets1Tags4Jets));
//	s.push_back(CUTE_SMEMFUN(TestBtagWeight, test1Bjets1Tags4Jets));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, testAnalysisCase));
	s.push_back(CUTE_SMEMFUN(TestBtagWeight, testAnalysisCaseWithJetInput));
	return s;
}
