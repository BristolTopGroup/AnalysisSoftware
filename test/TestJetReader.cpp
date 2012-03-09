#include "TestJetReader.h"
#include "../interface/Readers/NTupleEventReader.h"

TestJetReader::TestJetReader() : //
		input(new TChain(NTupleEventReader::EVENT_CHAIN)), //
		reader(new JetReader(input, JetAlgorithm::PF2PAT)), //
		PFCA08Jetreader(new JetReader(input, JetAlgorithm::CA08PF)), //
		jets(), //
		pfCA08jets(), //
		firstJet(), //
		firstPFJet() {
	input->Add(InputFile::TTJets);
	input->SetBranchStatus("*", 0);
	reader->initialise();
	PFCA08Jetreader->initialise();
	input->GetEntry(1);
	jets = reader->getJets();
	pfCA08jets = PFCA08Jetreader->getJets();
	firstJet = jets.front();
	firstPFJet = pfCA08jets.front();
}

void TestJetReader::testReadJetsSize() {
	ASSERT_EQUAL(7, jets.size());
}

void TestJetReader::testReadFirstJetEnergy() {
	ASSERT_EQUAL_DELTA(684.132, firstJet->energy(), 0.001);
}

void TestJetReader::testReadFirstJetEMF() {
	if (firstJet->getUsedAlgorithm() == JetAlgorithm::Calo_AntiKT_Cone05)
		ASSERT_EQUAL_DELTA(0.963491, firstJet->emf(), 0.00001);
	else //PF Jets don't have this value set, skip test
	ASSERT(true);
}

void TestJetReader::testReadFirstJetn90Hits() {
	if (firstJet->getUsedAlgorithm() == JetAlgorithm::Calo_AntiKT_Cone05)
		ASSERT_EQUAL_DELTA(57, firstJet->n90Hits(), 0.1);
	else //PF Jets don't have this value set, skip test
	ASSERT(true);
}

void TestJetReader::testReadFirstJetfHPD() {
	if (firstJet->getUsedAlgorithm() == JetAlgorithm::Calo_AntiKT_Cone05)
		ASSERT_EQUAL_DELTA(0.0126929, firstJet->fHPD(), 0.00001);
	else //PF Jets don't have this value set, skip test
	ASSERT(true);
}

void TestJetReader::testReadPFJetsSize() {
	ASSERT_EQUAL(6, pfCA08jets.size());
}

void TestJetReader::testReadFirstPFJetEnergy() {
	ASSERT_EQUAL_DELTA(761.373, firstPFJet->energy(), 0.001);
}

void TestJetReader::testReadFirstPFJetEMF() {
	ASSERT_EQUAL_DELTA(0, firstPFJet->emf(), 0.00001);
}

void TestJetReader::testReadFirstPFJetn90Hits() {
	ASSERT_EQUAL_DELTA(0, firstPFJet->n90Hits(), 0.1);
}

void TestJetReader::testReadFirstPFJetfHPD() {
	ASSERT_EQUAL_DELTA(0, firstPFJet->fHPD(), 0.00001);
}

void TestJetReader::testUsedAlgorithmDefault() {
	ASSERT_EQUAL(JetAlgorithm::PF2PAT, firstJet->getUsedAlgorithm());
}

void TestJetReader::testUsedAlgorithmPFJet() {
	ASSERT_EQUAL(JetAlgorithm::CA08PF, firstPFJet->getUsedAlgorithm());
}

cute::suite make_suite_TestJetReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadJetsSize));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadFirstJetEnergy));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadFirstJetEMF));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadFirstJetn90Hits));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadFirstJetfHPD));

	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadPFJetsSize));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadFirstPFJetEnergy));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadFirstPFJetEMF));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadFirstPFJetn90Hits));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadFirstPFJetfHPD));

	s.push_back(CUTE_SMEMFUN(TestJetReader, testUsedAlgorithmDefault));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testUsedAlgorithmPFJet));

	return s;
}
