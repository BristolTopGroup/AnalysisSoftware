#include <vector>
#include <string>

#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "TChain.h"
#include "TString.h"

#include "../interface/Readers/VariableReader.h"



using namespace ROOT;
using namespace BAT;
using namespace std;

struct TestVariableReader {
private:
	TString invalidEmptyVariableName, invalidNotAvailableVariableName, runNumber, energyForEachElectron;
	TChainPointer input;
	boost::scoped_ptr<VariableReader<unsigned int> > singleVariableReader;
	boost::scoped_ptr<VariableReader<MultiDoublePointer> > multipleVariableReader;
	boost::scoped_ptr<VariableReader<int> > invalidEmptyVariableVariableReader, invalidnNotAvailableVariableReader;

public:
	TestVariableReader() :
		invalidEmptyVariableName(""),
		invalidNotAvailableVariableName("thisIsNotInTheFile"),
		runNumber("run"),
		energyForEachElectron("Electron.Energy"),
		input(new TChain(NTupleEventReader::EVENT_CHAIN)),
		singleVariableReader(new VariableReader<unsigned int>::VariableReader(input, runNumber)),
		multipleVariableReader(new VariableReader<MultiDoublePointer>::VariableReader(input,
						energyForEachElectron)),
		invalidEmptyVariableVariableReader(new VariableReader<int>::VariableReader(input, invalidEmptyVariableName)),
		invalidnNotAvailableVariableReader(new VariableReader<int>::VariableReader(input, invalidNotAvailableVariableName)) 
		{
		input->Add(InputFile::ttbar);
		input->SetBranchStatus("*", 0);
		singleVariableReader->initialise();
		multipleVariableReader->initialise();
		input->LoadTree(1);
		input->GetEntry(1);
	}

	void testReadSingleVariable() {
		ASSERT_EQUAL(1, singleVariableReader->getVariable());
	}

	void testReadMultipleVariable() {
		ASSERT_EQUAL_DELTA(108.714, multipleVariableReader->getVariableAt(0), 0.001);
		ASSERT_EQUAL(singleVariableReader->getVariable(), multipleVariableReader->size());
	}

	void testInvalidVariableThrowsException() {
		ASSERT_THROWS(invalidnNotAvailableVariableReader->initialise(), VariableNotFoundException);
	}

	void testInvalidEmptyVariableThrowsException() {
		ASSERT_THROWS(invalidEmptyVariableVariableReader->initialise(), VariableNotFoundException);
	}

};

cute::suite make_suite_TestVariableReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestVariableReader,testReadSingleVariable));
	s.push_back(CUTE_SMEMFUN(TestVariableReader,testReadMultipleVariable));
	s.push_back(CUTE_SMEMFUN(TestVariableReader,testInvalidVariableThrowsException));
	s.push_back(CUTE_SMEMFUN(TestVariableReader,testInvalidEmptyVariableThrowsException));

	return s;
}

