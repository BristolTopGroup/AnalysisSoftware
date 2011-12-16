#ifndef TESTMET_H_
#define TESTMET_H_

#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/RecoObjects/MET.h"

struct TestMET {
private:
	BAT::METPointer goodMET, badMET;
public:
	TestMET();
	void testStandardConstructorEt();
	void testStandardConstructorPt();
	void testStandardConstructorPx();
	void testStandardConstructorPy();
	void testStandardConstructorPz();
	void testStandardConstructorMass();
	void testGoodMETIsGood();
	void testBadMETNoGood();
	void testSetAlgorithm();
	void testStandardAlgorithm();
	void testSumET();
	void testSignificance();

	static cute::suite make_suite() {
		cute::suite s;

		s.push_back(CUTE_SMEMFUN(TestMET,testStandardConstructorEt));
		s.push_back(CUTE_SMEMFUN(TestMET,testStandardConstructorPt));
		s.push_back(CUTE_SMEMFUN(TestMET,testStandardConstructorPx));
		s.push_back(CUTE_SMEMFUN(TestMET,testStandardConstructorPy));
		s.push_back(CUTE_SMEMFUN(TestMET,testStandardConstructorPz));
		s.push_back(CUTE_SMEMFUN(TestMET,testStandardConstructorMass));
		s.push_back(CUTE_SMEMFUN(TestMET,testGoodMETIsGood));
		s.push_back(CUTE_SMEMFUN(TestMET,testBadMETNoGood));
		s.push_back(CUTE_SMEMFUN(TestMET,testSetAlgorithm));
		s.push_back(CUTE_SMEMFUN(TestMET,testStandardAlgorithm));
		s.push_back(CUTE_SMEMFUN(TestMET,testSumET));
		s.push_back(CUTE_SMEMFUN(TestMET,testSignificance));

		return s;
	}
};

extern cute::suite make_suite_TestMET();

#endif
