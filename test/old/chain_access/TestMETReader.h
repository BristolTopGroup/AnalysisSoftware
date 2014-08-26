#ifndef TESTMETREADER_H_
#define TESTMETREADER_H_

#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/Readers/METReader.h"
#include "../interface/RecoObjects/MET.h"
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "TChain.h"


struct TestMETReader {
private:
	boost::shared_ptr<TChain> input;
	boost::scoped_ptr<BAT::METReader> PFMETreader;
	BAT::METPointer pfmet;

public:
	TestMETReader();
	void testPFMET();
	void testPFMETRAW();
	void testPFSignificance();
	void testPFSumET();
};

extern cute::suite make_suite_TestMETReader();

#endif
