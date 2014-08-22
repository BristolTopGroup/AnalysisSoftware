#ifndef TESTROOT_h
#define TESTROOT_h
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>

#include "TChain.h"
#include "TString.h"

struct TestROOT {
private:
	TString invalidBranch;
	TString energyForEachElectron;

	boost::scoped_ptr<TChain> input;
public:
	TestROOT();
	void testInvalidTBranch();
	void testValidBranch();

};

extern cute::suite make_suite_ROOTLearnTests();
#endif
