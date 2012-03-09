#ifndef TESTJETREADER_H_
#define TESTJETREADER_H_
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
	boost::scoped_ptr<JetReader> reader, PFCA08Jetreader;
	JetCollection jets, pfCA08jets;
	JetPointer firstJet, firstPFJet;

public:
	TestJetReader();

	void testReadJetsSize();
	void testReadFirstJetEnergy();
	void testReadFirstJetEMF();
	void testReadFirstJetn90Hits();
	void testReadFirstJetfHPD();
	void testReadPFJetsSize();
	void testReadFirstPFJetEnergy();
	void testReadFirstPFJetEMF();
	void testReadFirstPFJetn90Hits();
	void testReadFirstPFJetfHPD();
	void testUsedAlgorithmDefault();
	void testUsedAlgorithmPFJet();

};
extern cute::suite make_suite_TestJetReader();

#endif
