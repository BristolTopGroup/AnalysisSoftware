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
	boost::scoped_ptr<GenJetReader> reader;
	JetCollection genJets;
	JetPointer firstJet;

public:
	TestGenJetReader() :
		input(new TChain(NTupleEventReader::EVENT_CHAIN)),
		reader(new GenJetReader(input)),
		genJets(),
		firstJet() {
		input->Add(InputFile::ttbar);
		input->SetBranchStatus("*", 0);
		reader->initialise();
		input->GetEntry(1);
		genJets = reader->getGenJets();
		firstJet = genJets.front();
	}

	void testReadJetsSize() {
		ASSERT_EQUAL(10, genJets.size());
	}

	void testReadFirstJetEnergy() {
		ASSERT_EQUAL_DELTA(238.579, firstJet->energy(), 0.001);
	}

	void testReadFirstJetEMF() {
		ASSERT_EQUAL_DELTA(0.191873, firstJet->emf(), 0.00001);
	}

};
extern cute::suite make_suite_TestGenJetReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestGenJetReader, testReadJetsSize));
	s.push_back(CUTE_SMEMFUN(TestGenJetReader, testReadFirstJetEnergy));
	s.push_back(CUTE_SMEMFUN(TestGenJetReader, testReadFirstJetEMF));

	return s;
}
