#ifndef TESTVARIABLERADER_h
#define TESTVARIABLERADER_h
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "TChain.h"
#include "TString.h"
#include <boost/scoped_ptr.hpp>

#include "../interface/Readers/VariableReader.h"

struct TestVariableReader {
private:
	TString invalidEmptyVariableName, invalidNotAvailableVariableName, runNumber, energyForEachElectron;
	TChainPointer input;
	boost::scoped_ptr<BAT::VariableReader<unsigned int> > singleVariableReader;
	boost::scoped_ptr<BAT::VariableReader<MultiDoublePointer> > multipleVariableReader;
	boost::scoped_ptr<BAT::VariableReader<int> > invalidEmptyVariableVariableReader, invalidnNotAvailableVariableReader;

public:
	TestVariableReader();
	void testReadSingleVariable();
	void testReadMultipleVariable();
	void testInvalidVariableThrowsException();
	void testInvalidEmptyVariableThrowsException();
};

extern cute::suite make_suite_TestVariableReader();

#endif

