#include "TestMETReader.h"
#include "../interface/Readers/NTupleEventReader.h"
#include "InputFiles.h"

using namespace BAT;

TestMETReader::TestMETReader() :
		input(new TChain(NTupleEventReader::EVENT_CHAIN)), //
		PFMETreader(new METReader(input, METAlgorithm::ParticleFlowMET)), //
		pfmet() //
{
	input->Add(InputFile::TTJets);
	input->SetBranchStatus("*", 0);
	PFMETreader->initialise();
	input->GetEntry(0);
	pfmet = PFMETreader->getMET();
}

void TestMETReader::testPFMET() {
	ASSERT_EQUAL_DELTA(32.0708, pfmet->et(), 0.0001);
}

void TestMETReader::testPFMETRAW() {
	ASSERT_EQUAL_DELTA(7.69286, pfmet->et(), 0.0001);
}

void TestMETReader::testPFSignificance() {
	ASSERT_EQUAL_DELTA(14.231, pfmet->significance(), 0.0001);
}

void TestMETReader::testPFSumET() {
	ASSERT_EQUAL_DELTA(440.114, pfmet->sumET(), 0.001);
}

cute::suite make_suite_TestMETReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestMETReader, testPFMET));
	s.push_back(CUTE_SMEMFUN(TestMETReader, testPFSignificance));
	s.push_back(CUTE_SMEMFUN(TestMETReader, testPFSumET));

	return s;
}
