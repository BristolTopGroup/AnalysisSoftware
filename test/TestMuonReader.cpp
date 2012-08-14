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
	ASSERT_EQUAL_DELTA(46.6053, leadingMuon->energy(), 0.0001);
}

void TestMuonReader::testLeadingMuonPx() {
	ASSERT_EQUAL_DELTA(-10.782, leadingMuon->px(), 0.0001);
}

void TestMuonReader::testLeadingMuonPy() {
	ASSERT_EQUAL_DELTA(31.1576, leadingMuon->py(), 0.0001);
}

void TestMuonReader::testLeadingMuonPz() {
	ASSERT_EQUAL_DELTA(-32.9392, leadingMuon->pz(), 0.0001);
}

void TestMuonReader::testLeadingMuonRelativeIsolation() {
	ASSERT_EQUAL_DELTA(1.56243, leadingMuon->relativeIsolation(), 0.00001);
}

void TestMuonReader::testNormalisedChi2() {
	ASSERT_EQUAL_DELTA(1.65566, leadingMuon->normChi2(), 0.000001);
}

void TestMuonReader::testNumberOfMatchedStations() {
	ASSERT_EQUAL(3, leadingMuon->numberOfMatchedStations());
}

void TestMuonReader::testNumberOfMatches() {
	ASSERT_EQUAL(3, leadingMuon->numberOfMatches());
}

void TestMuonReader::testNumberOfValidHits() {
	ASSERT_EQUAL(18, leadingMuon->numberOfValidHits());
}

void TestMuonReader::testNumberOfValidMuonHits() {
	ASSERT_EQUAL(34, leadingMuon->numberOfValidMuonHits());
}

void TestMuonReader::testNumberOfValidPixelHits() {
	if (Globals::NTupleVersion >= 8)
		ASSERT_EQUAL(3, leadingMuon->numberOfValidPixelHits());
}

void TestMuonReader::testTrackerLayersWithMeasurement() {
	if (Globals::NTupleVersion >= 8)
		ASSERT_EQUAL(13, leadingMuon->trackerLayersWithMeasurement());
}

void TestMuonReader::testPixelLayersWithMeasurement() {
	ASSERT_EQUAL(3, leadingMuon->pixelLayersWithMeasurement());
}

void TestMuonReader::testZDistanceToPrimaryVertex() {
	ASSERT_EQUAL_DELTA(-0.0106531, leadingMuon->ZDistanceToPrimaryVertex(), 0.00000001);
}

void TestMuonReader::testGlobalMuon() {
	ASSERT_EQUAL(true, leadingMuon->isGlobal());
}

void TestMuonReader::testPFMuon() {
	if (Globals::NTupleVersion >= 8)
		ASSERT_EQUAL(true, leadingMuon->isPFMuon());
}

void TestMuonReader::testTrackerMuon() {
	ASSERT_EQUAL(true, leadingMuon->isTracker());
}

cute::suite make_suite_TestMuonReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestMuonReader, testNumberOfMuons));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testLeadingMuonEnergy));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testLeadingMuonPx));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testLeadingMuonPy));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testLeadingMuonPz));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testLeadingMuonRelativeIsolation));

	s.push_back(CUTE_SMEMFUN(TestMuonReader, testNormalisedChi2));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testNumberOfMatchedStations));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testNumberOfMatches));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testNumberOfValidHits));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testNumberOfValidMuonHits));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testNumberOfValidPixelHits));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testTrackerLayersWithMeasurement));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testPixelLayersWithMeasurement));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testZDistanceToPrimaryVertex));

	s.push_back(CUTE_SMEMFUN(TestMuonReader, testGlobalMuon));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testPFMuon));
	s.push_back(CUTE_SMEMFUN(TestMuonReader, testTrackerMuon));

	return s;
}
