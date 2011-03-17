#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/shared_ptr.hpp>

#include "../interface/Readers/VertexReader.h"
#include "TChain.h"

using namespace BAT;

struct TestVertexReader {
	boost::shared_ptr<TChain> input;
	boost::shared_ptr<VertexReader> reader;
	VertexPointer vertex;

	TestVertexReader() :
		input(new TChain(NTupleEventReader::EVENT_CHAIN)),
		reader(new VertexReader(input)),
		vertex() {
		input->Add(InputFile::ttbar);
		input->SetBranchStatus("*", 0);
		reader->initialise();
		input->GetEntry(1);
		vertex = reader->getVertices().front();
	}

	void testVertexZPosition() {
		ASSERT_EQUAL_DELTA(1.86712, vertex->absoluteZPosition(), 0.00001);
	}

	void testVertexRho(){
		ASSERT_EQUAL_DELTA(0.461491, vertex->absoluteRho(), 0.000001);
	}

	void testVertexIsFake(){
		ASSERT(vertex->isFake() == false);
	}

	void testVertexNDOF(){
		ASSERT_EQUAL(111, vertex->ndof());
	}
};

cute::suite make_suite_TestVertexReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestVertexReader, testVertexZPosition));
	s.push_back(CUTE_SMEMFUN(TestVertexReader, testVertexRho));
	s.push_back(CUTE_SMEMFUN(TestVertexReader, testVertexIsFake));
	s.push_back(CUTE_SMEMFUN(TestVertexReader, testVertexNDOF));

	return s;
}
