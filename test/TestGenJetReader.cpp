#include "TestGenJetReader.h"

#include "InputFiles.h"
#include "../interface/Readers/NTupleEventReader.h"

using namespace BAT;

TestGenJetReader::TestGenJetReader() :
		input(new TChain(NTupleEventReader::EVENT_CHAIN)), //
		reader(new GenJetReader(input)), //
		genJets(), //
		firstJet() {
	input->Add(InputFile::TTJets);
	input->SetBranchStatus("*", 0);
	reader->initialise();
	input->GetEntry(1);
	genJets = reader->getGenJets();
	firstJet = genJets.front();
}

void TestGenJetReader::testReadJetsSize() {
	ASSERT_EQUAL(8, genJets.size());
}

void TestGenJetReader::testReadFirstJetEnergy() {
	ASSERT_EQUAL_DELTA(642.435, firstJet->energy(), 0.001);
}

void TestGenJetReader::testReadFirstJetEMF() {
	ASSERT_EQUAL_DELTA(0.578493, firstJet->emf(), 0.00001);
}

cute::suite make_suite_TestGenJetReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestGenJetReader, testReadJetsSize));
	s.push_back(CUTE_SMEMFUN(TestGenJetReader, testReadFirstJetEnergy));
	s.push_back(CUTE_SMEMFUN(TestGenJetReader, testReadFirstJetEMF));

	return s;
}
