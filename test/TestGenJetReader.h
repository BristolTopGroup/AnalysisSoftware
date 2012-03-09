#ifndef TESTGENJETREADER_h
#define TESTGENJETREADER_h
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../interface/Readers/GenJetReader.h"

using namespace BAT;

struct TestGenJetReader {
private:
	boost::shared_ptr<TChain> input;
	boost::scoped_ptr<GenJetReader> reader;
	JetCollection genJets;
	JetPointer firstJet;

public:
	TestGenJetReader();
	void testReadJetsSize();
	void testReadFirstJetEnergy();
	void testReadFirstJetEMF();
};
extern cute::suite make_suite_TestGenJetReader();
#endif
