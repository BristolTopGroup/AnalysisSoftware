#include "TestVariableReader.h"
#include "../interface/Readers/NTupleEventReader.h"
#include "InputFiles.h"

using namespace BAT;

TestVariableReader::TestVariableReader() :
		invalidEmptyVariableName(""), //
		invalidNotAvailableVariableName("thisIsNotInTheFile"), //
		runNumber("Event.Run"), //
		energyForEachElectron("selectedPatElectronsLoosePFlow.Energy"), //
		input(new TChain(NTupleEventReader::EVENT_CHAIN)), //
		singleVariableReader(new VariableReader<unsigned int>(input, runNumber)), //
		multipleVariableReader(new VariableReader<MultiDoublePointer>(input, energyForEachElectron)), //
		invalidEmptyVariableVariableReader(new VariableReader<int>(input, invalidEmptyVariableName)), //
		invalidnNotAvailableVariableReader(new VariableReader<int>(input, invalidNotAvailableVariableName)) //
{
	input->Add(InputFile::TTJets);
	input->SetBranchStatus("*", 0);
	singleVariableReader->initialise();
	multipleVariableReader->initialise();
	input->LoadTree(1);
	input->GetEntry(1);
	unsigned int entry(1);
	while(multipleVariableReader->size() == 0){
		++entry;
		input->GetEntry(entry);
	}
}

void TestVariableReader::testReadSingleVariable() {
	ASSERT_EQUAL(1, singleVariableReader->getVariable());
}

void TestVariableReader::testReadMultipleVariable() {
	ASSERT_EQUAL_DELTA(257.856, multipleVariableReader->getVariableAt(0), 0.001);
}

void TestVariableReader::testInvalidVariableThrowsException() {
	ASSERT_THROWS(invalidnNotAvailableVariableReader->initialise(), VariableNotFoundException);
}

void TestVariableReader::testInvalidEmptyVariableThrowsException() {
	ASSERT_THROWS(invalidEmptyVariableVariableReader->initialise(), VariableNotFoundException);
}

cute::suite make_suite_TestVariableReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestVariableReader,testReadSingleVariable));
	s.push_back(CUTE_SMEMFUN(TestVariableReader,testReadMultipleVariable));
	s.push_back(CUTE_SMEMFUN(TestVariableReader,testInvalidVariableThrowsException));
	s.push_back(CUTE_SMEMFUN(TestVariableReader,testInvalidEmptyVariableThrowsException));

	return s;
}

