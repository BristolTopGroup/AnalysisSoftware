#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../interface/Readers/GenJetReader.h"
#include "InputFiles.h"
using namespace BAT;

struct TestGenJetReader {
private:
	boost::shared_ptr<TChain> input;
	boost::scoped_ptr<JetReader> reader;
	JetCollection genJets;
	JetPointer firstJet;

public:
	TestGenJetReader() :
		input(new TChain(NTupleEventReader::EVENT_CHAIN)),
		reader(new JetReader(input)),
		genJets(),
		firstJet() {
		input->Add(InputFile::ttbar);
		input->SetBranchStatus("*", 0);
		reader->initialise();
		input->GetEntry(1);
		genJets = reader->getJets();
		firstJet = genJets.front();
	}

	void testReadJetsSize() {
		ASSERT_EQUAL(8, genJets.size());
	}

	void testReadFirstJetEnergy() {
		ASSERT_EQUAL_DELTA(212.538, firstJet->energy(), 0.001);
	}

	void testReadFirstJetEMF() {
		ASSERT_EQUAL_DELTA(0.496578, firstJet->emf(), 0.00001);
	}

	void testReadFirstJetn90Hits() {
		ASSERT_EQUAL_DELTA(24, firstJet->n90Hits(), 0.1);
	}

	void testReadFirstJetfHPD() {
		ASSERT_EQUAL_DELTA(0.243706, firstJet->fHPD(), 0.00001);
	}

};
extern cute::suite make_suite_TestGenJetReader() {
	cute::suite s;
	s.push_back(CUTE_SMEMFUN(TestGenJetReader, testReadJetsSize));
	s.push_back(CUTE_SMEMFUN(TestGenJetReader, testReadFirstJetEnergy));
	s.push_back(CUTE_SMEMFUN(TestGenJetReader, testReadFirstJetEMF));
	s.push_back(CUTE_SMEMFUN(TestGenJetReader, testReadFirstJetn90Hits));
	s.push_back(CUTE_SMEMFUN(TestGenJetReader, testReadFirstJetfHPD));

	return s;
}
