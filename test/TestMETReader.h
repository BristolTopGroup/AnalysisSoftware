#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/Readers/METReader.h"
#include "../interface/RecoObjects/MET.h"
#include "InputFiles.h"
using namespace BAT;

struct TestMETReader {
private:
	boost::shared_ptr<TChain> input;
	boost::scoped_ptr<METReader> reader, PFreader;
	METPointer met, pfmet;

public:
	TestMETReader() :
		input(new TChain(NTupleEventReader::EVENT_CHAIN)),
				reader(new METReader(input)),
				PFreader(new METReader(input, METAlgorithm::ParticleFlowMET)),
				met(), pfmet() {
		input->Add(InputFile::TTJets);
		input->SetBranchStatus("*", 0);
		reader->initialise();
		PFreader->initialise();
		input->GetEntry(0);
		met = reader->getMET();
		pfmet = PFreader->getMET();
	}

	void testMET() {
		ASSERT_EQUAL_DELTA(58.6601, met->et(), 0.0001);
	}

	void testPFMET() {
		ASSERT_EQUAL_DELTA(44.6193, pfmet->et(), 0.0001);
	}

	void testAreMETsGood() {
		ASSERT_EQUAL(true, met->isGood());
		ASSERT_EQUAL(true, pfmet->isGood());
	}

	void testCaloSignificance() {
		ASSERT_EQUAL(-1, met->significance());
	}

	void testPFSignificance() {
		ASSERT_EQUAL_DELTA(5, pfmet->significance(), 0.0001);
	}

	void testCaloSumET() {
		ASSERT_EQUAL_DELTA(44.6193, met->sumET(), 0.0001);
	}

	void testPFSumET() {
		ASSERT_EQUAL_DELTA(44.6193, pfmet->sumET(), 0.0001);
	}

};

extern cute::suite make_suite_TestMETReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestMETReader, testMET));
	s.push_back(CUTE_SMEMFUN(TestMETReader, testPFMET));
	s.push_back(CUTE_SMEMFUN(TestMETReader, testAreMETsGood));
	s.push_back(CUTE_SMEMFUN(TestMETReader, testCaloSignificance));
	s.push_back(CUTE_SMEMFUN(TestMETReader, testPFSignificance));
	s.push_back(CUTE_SMEMFUN(TestMETReader, testCaloSumET));
	s.push_back(CUTE_SMEMFUN(TestMETReader, testPFSumET));

	return s;
}
