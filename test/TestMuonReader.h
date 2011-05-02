#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "TChain.h"
#include "../interface/Readers/MuonReader.h"
#include "InputFiles.h"

using namespace BAT;

struct TestMuonReader {
private:
	boost::shared_ptr<TChain> input;
	boost::scoped_ptr<MuonReader> reader;
	MuonCollection muons;
	MuonPointer leadingMuon;

public:
	TestMuonReader() :
		input(new TChain(NTupleEventReader::EVENT_CHAIN)),
		reader(new MuonReader(input)),
		muons(),
		leadingMuon() {
		input->Add(InputFile::ttbar);
		input->SetBranchStatus("*", 0);
		reader->initialise();
		unsigned int counter = 0;
		while(muons.size() == 0){
		    input->GetEntry(counter);
            muons = reader->getMuons();
            ++counter;
		}

		assert(muons.size() > 0);
		leadingMuon = muons.front();
	}

	void testNumberOfMuons() {
        ASSERT_EQUAL(2, muons.size());
    }

	void testLeadingMuonEnergy(){
	    ASSERT_EQUAL_DELTA(14.1981, leadingMuon->energy(), 0.0001);
	}

	void testLeadingMuonPx(){
	    ASSERT_EQUAL_DELTA(11.3069, leadingMuon->px(), 0.0001);
	}

	void testLeadingMuonPy(){
	    ASSERT_EQUAL_DELTA(-5.54134, leadingMuon->py(), 0.0001);
	}

    void testLeadingMuonPz() {
        ASSERT_EQUAL_DELTA(6.55911, leadingMuon->pz(), 0.0001);
    }

    void testLeadingMuonRelativeIsolation() {
        ASSERT_EQUAL_DELTA(3.03852, leadingMuon->relativeIsolation(), 0.00001);
    }

    void testLeadingMuonIsGlobal() {
        ASSERT_EQUAL(true, leadingMuon->isGlobal());
    }
};

extern cute::suite make_suite_TestMuonReader() {
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
