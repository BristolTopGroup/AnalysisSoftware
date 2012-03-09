#include <iostream>

#include "TestROOT.h"
#include "../interface/Readers/NTupleEventReader.h"
#include "InputFiles.h"

using namespace BAT;

TestROOT::TestROOT() :
		invalidBranch("this is not in the chain"), //
		energyForEachElectron("selectedPatElectronsLoosePFlow.Energy"), //
		input(new TChain(NTupleEventReader::EVENT_CHAIN)) {
	input->Add(InputFile::TTJets);

	input->GetEntries();
	input->SetBranchStatus("*", 0);
	input->SetBranchStatus(energyForEachElectron, 1);
}
void TestROOT::testInvalidTBranch() {
	try {
		TBranch* br = input->GetBranch(invalidBranch);
		if (br) {
			input->SetBranchStatus(invalidBranch, 1);
		}
	} catch (...) {
		std::cout << "exception occurred" << std::endl;
	}
	ASSERT(input->GetBranch(invalidBranch) == NULL);
}

void TestROOT::testValidBranch() {
	ASSERT(input->GetBranch(energyForEachElectron) != NULL);
}

cute::suite make_suite_ROOTLearnTests() {
	cute::suite s;
	s.push_back(CUTE_SMEMFUN(TestROOT,testInvalidTBranch));
	s.push_back(CUTE_SMEMFUN(TestROOT,testValidBranch));
	return s;
}
