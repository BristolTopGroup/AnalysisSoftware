#include "TestVertexReader.h"
#include "../interface/Readers/NTupleEventReader.h"
#include "InputFiles.h"

using namespace BAT;

TestVertexReader::TestVertexReader() :
		input(new TChain(NTupleEventReader::EVENT_CHAIN)), //
		reader(new VertexReader(input)), //
		vertex() //
{
	input->Add(InputFile::TTJets);
	input->SetBranchStatus("*", 0);
	reader->initialise();
	input->GetEntry(1);
	vertex = reader->getVertices().front();
}

void TestVertexReader::testVertexZPosition() {
	ASSERT_EQUAL_DELTA(5.19014, vertex->absoluteZPosition(), 0.00001);
}

void TestVertexReader::testVertexRho() {
	ASSERT_EQUAL_DELTA(0.462303, vertex->absoluteRho(), 0.000001);
}

void TestVertexReader::testVertexIsFake() {
	ASSERT(vertex->isFake() == false);
}

void TestVertexReader::testVertexNDOF() {
	ASSERT_EQUAL(69, vertex->ndof());
}

cute::suite make_suite_TestVertexReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestVertexReader, testVertexZPosition));
	s.push_back(CUTE_SMEMFUN(TestVertexReader, testVertexRho));
	s.push_back(CUTE_SMEMFUN(TestVertexReader, testVertexIsFake));
	s.push_back(CUTE_SMEMFUN(TestVertexReader, testVertexNDOF));

	return s;
}
