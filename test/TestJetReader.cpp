#include "TestJetReader.h"
#include "../interface/Readers/NTupleEventReader.h"

TestJetReader::TestJetReader() : //
		input(new TChain(NTupleEventReader::EVENT_CHAIN)), //
		reader(new JetReader(input, JetAlgorithm::PF2PAT)), //
		jets(), //
		firstJet() {
	input->Add(InputFile::TTJets);
	input->SetBranchStatus("*", 0);
	reader->initialise();
	input->GetEntry(1);
	jets = reader->getJets();
	firstJet = jets.front();
}

void TestJetReader::testReadJetsSize() {
	ASSERT_EQUAL(3, jets.size());
}

void TestJetReader::testReadFirstJetEnergy() {
	ASSERT_EQUAL_DELTA(234.424, firstJet->energy(), 0.001);
}

void TestJetReader::testReadFirstJetEMF() {
	if (firstJet->getUsedAlgorithm() == JetAlgorithm::Calo_AntiKT_Cone05)
		ASSERT_EQUAL_DELTA(0.963491, firstJet->emf(), 0.00001);
	else //PF Jets
		ASSERT_EQUAL_DELTA(0, firstJet->emf(), 0.00001);
}

void TestJetReader::testReadFirstJetn90Hits() {
	if (firstJet->getUsedAlgorithm() == JetAlgorithm::Calo_AntiKT_Cone05)
		ASSERT_EQUAL_DELTA(57, firstJet->n90Hits(), 0.1);
	else //PF Jets
		ASSERT_EQUAL_DELTA(0, firstJet->n90Hits(), 0.1);
}

void TestJetReader::testReadFirstJetfHPD() {
	if (firstJet->getUsedAlgorithm() == JetAlgorithm::Calo_AntiKT_Cone05)
		ASSERT_EQUAL_DELTA(0.0126929, firstJet->fHPD(), 0.00001);
	else //PF Jets
		ASSERT_EQUAL_DELTA(0, firstJet->fHPD(), 0.00001);
}

void TestJetReader::testUsedAlgorithmDefault() {
	if (firstJet->getUsedAlgorithm() == JetAlgorithm::PF2PAT || firstJet->getUsedAlgorithm() == JetAlgorithm::CA08PF)
		ASSERT_EQUAL(JetAlgorithm::PF2PAT, firstJet->getUsedAlgorithm());
	else
	ASSERT(true);
}

cute::suite make_suite_TestJetReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadJetsSize));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadFirstJetEnergy));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadFirstJetEMF));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadFirstJetn90Hits));
	s.push_back(CUTE_SMEMFUN(TestJetReader, testReadFirstJetfHPD));

	s.push_back(CUTE_SMEMFUN(TestJetReader, testUsedAlgorithmDefault));

	return s;
}
