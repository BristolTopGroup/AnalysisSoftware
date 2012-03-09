#include "TestMuonReader.h"
#include "InputFiles.h"
#include "../interface/Readers/NTupleEventReader.h"

using namespace BAT;

TestMuonReader::TestMuonReader() :
		input(new TChain(NTupleEventReader::EVENT_CHAIN)), //
		reader(new MuonReader(input, MuonAlgorithm::ParticleFlow)), //
		muons(), //
		leadingMuon() //
{
	input->Add(InputFile::TTJets);
	input->SetBranchStatus("*", 0);
	reader->initialise();
	unsigned int counter = 0;
	while (muons.size() == 0) {
		input->GetEntry(counter);
		muons = reader->getMuons();
		++counter;
	}

	assert(muons.size() > 0);
	leadingMuon = muons.front();
}

void TestMuonReader::testNumberOfMuons() {
	ASSERT_EQUAL(1, muons.size());
}

void TestMuonReader::testLeadingMuonEnergy() {
	ASSERT_EQUAL_DELTA(69.8599, leadingMuon->energy(), 0.0001);
}

void TestMuonReader::testLeadingMuonPx() {
	ASSERT_EQUAL_DELTA(-1.87493, leadingMuon->px(), 0.0001);
}

void TestMuonReader::testLeadingMuonPy() {
	ASSERT_EQUAL_DELTA(-28.8254, leadingMuon->py(), 0.0001);
}

void TestMuonReader::testLeadingMuonPz() {
	ASSERT_EQUAL_DELTA(63.608, leadingMuon->pz(), 0.0001);
}

void TestMuonReader::testLeadingMuonRelativeIsolation() {
	ASSERT_EQUAL_DELTA(0.0931756, leadingMuon->relativeIsolation(), 0.00001);
}

void TestMuonReader::testLeadingMuonIsGlobal() {
	ASSERT_EQUAL(true, leadingMuon->isGlobal());
}

cute::suite make_suite_TestMuonReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestMuonReader, testNumberOfMuons));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testLeadingMuonEnergy));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testLeadingMuonPx));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testLeadingMuonPy));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testLeadingMuonPz));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testLeadingMuonRelativeIsolation));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testLeadingMuonIsGlobal));

	return s;
}
