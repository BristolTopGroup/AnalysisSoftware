#ifndef TESTMUONREADER_h
#define TESTMUONREADER_h
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "TChain.h"
#include "../interface/Readers/MuonReader.h"

using namespace BAT;

struct TestMuonReader {
private:
	boost::shared_ptr<TChain> input;
	boost::scoped_ptr<MuonReader> reader;
	MuonCollection muons;
	MuonPointer leadingMuon;

public:
	TestMuonReader();
	void testNumberOfMuons();
	void testLeadingMuonEnergy();
	void testLeadingMuonPx();
	void testLeadingMuonPy();
    void testLeadingMuonPz();
    void testLeadingMuonRelativeIsolation();
    void testLeadingMuonIsGlobal();
};

extern cute::suite make_suite_TestMuonReader();
#endif
