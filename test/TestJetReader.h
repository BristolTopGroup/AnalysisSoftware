#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../interface/Readers/JetReader.h"
#include "InputFiles.h"
using namespace BAT;

struct TestJetReader {
private:
	boost::shared_ptr<TChain> input;
	boost::scoped_ptr<JetReader> reader, PFreader;
	JetCollection jets, pfjets;
	JetPointer firstJet, firstPFJet;

public:
	TestJetReader() :
		input(new TChain(NTupleEventReader::EVENT_CHAIN)),
		reader(new JetReader(input)),
		PFreader(new JetReader(input, JetAlgorithm::CA08PF)),
		jets(),
		pfjets(),
		firstJet(),
		firstPFJet() {
		input->Add(InputFile::ttbar);
		input->SetBranchStatus("*", 0);
		reader->initialise();
		PFreader->initialise();
		input->GetEntry(1);
		jets = reader->getJets();
		pfjets = PFreader->getJets();
		firstJet = jets.front();
		firstPFJet = pfjets.front();
	}

	void testReadJetsSize() {
		ASSERT_EQUAL(10, jets.size());
	}

	void testReadFirstJetEnergy() {
		ASSERT_EQUAL_DELTA(138.88, firstJet->energy(), 0.001);
	}

	void testReadFirstJetEMF() {
		ASSERT_EQUAL_DELTA(0.568266, firstJet->emf(), 0.00001);
	}

	void testReadFirstJetn90Hits() {
		ASSERT_EQUAL_DELTA(37, firstJet->n90Hits(), 0.1);
	}

	void testReadFirstJetfHPD() {
		ASSERT_EQUAL_DELTA(0.262745, firstJet->fHPD(), 0.00001);
	}

	void testReadPFJetsSize() {
		ASSERT_EQUAL(12, pfjets.size());
	}

	void testReadFirstPFJetEnergy() {
		ASSERT_EQUAL_DELTA(139.809, firstPFJet->energy(), 0.001);
	}

	void testReadFirstPFJetEMF() {
		ASSERT_EQUAL_DELTA(0, firstPFJet->emf(), 0.00001);
	}

	void testReadFirstPFJetn90Hits() {
		ASSERT_EQUAL_DELTA(0, firstPFJet->n90Hits(), 0.1);
	}

	void testReadFirstPFJetfHPD() {
		ASSERT_EQUAL_DELTA(0, firstPFJet->fHPD(), 0.00001);
	}

	void testUsedAlgorithmDefault(){
		ASSERT_EQUAL(JetAlgorithm::Calo_AntiKT_Cone05, firstJet->getUsedAlgorithm());
	}

	void testUsedAlgorithmPFJet(){
		ASSERT_EQUAL(JetAlgorithm::CA08PF, firstPFJet->getUsedAlgorithm());
	}

};
extern cute::suite make_suite_TestJetReader() {
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
