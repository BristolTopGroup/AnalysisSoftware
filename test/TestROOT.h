#include <iostream>

#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>

#include "TChain.h"
#include "TString.h"



struct TestROOT {
private:
	TString invalidBranch;
	//select two related variables
//	TString numberOfElectrons;
	TString energyForEachElectron;

	boost::scoped_ptr<TChain> input;
public:
	TestROOT() :
		invalidBranch("this is not in the chain"),
//		numberOfElectrons("Nels"),
		energyForEachElectron("Electron.Energy"),
		input(new TChain(NTupleEventReader::EVENT_CHAIN)) {
		input->Add(InputFile::ttbar);

		input->GetEntries();
		input->SetBranchStatus("*", 0);
		input->SetBranchStatus(energyForEachElectron, 1);
	}
	void testInvalidTBranch() {
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

	void testValidBranch(){
		ASSERT(input->GetBranch(energyForEachElectron) != NULL);
	}


};

extern cute::suite make_suite_ROOTLearnTests() {
		cute::suite s;
		s.push_back(CUTE_SMEMFUN(TestROOT,testInvalidTBranch));
		s.push_back(CUTE_SMEMFUN(TestROOT,testValidBranch));
		return s;
	}
