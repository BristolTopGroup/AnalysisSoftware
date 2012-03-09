#ifndef TESTVERTEXREADER_h
#define TESTVERTEXREADER_h
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/shared_ptr.hpp>

#include "../interface/Readers/VertexReader.h"
#include "TChain.h"


struct TestVertexReader {
	boost::shared_ptr<TChain> input;
	boost::shared_ptr<BAT::VertexReader> reader;
	BAT::VertexPointer vertex;

	TestVertexReader();
	void testVertexZPosition();
	void testVertexRho();
	void testVertexIsFake();
	void testVertexNDOF();
};

extern cute::suite make_suite_TestVertexReader();

#endif
