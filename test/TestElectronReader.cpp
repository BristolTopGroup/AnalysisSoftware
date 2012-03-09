#include "TestElectronReader.h"
#include "InputFiles.h"
#include <iostream>

using namespace BAT;

TestElectronReader::TestElectronReader() :
		input(new TChain(NTupleEventReader::EVENT_CHAIN)), //
		electronReader(new ElectronReader(input, Globals::electronAlgorithm)), //
		electrons(), //
		firstElectron() {

	input->Add(InputFile::TTJets);

	input->LoadTree(1);
	input->SetBranchStatus("*", 0);
	electronReader->initialise();
	input->GetEntry(1);
	electrons = electronReader->getElectrons();
	unsigned int entry = 1;
	while (electrons.size() == 0) {
		++entry;
		input->GetEntry(entry);
		electrons = electronReader->getElectrons();
	}
	assert(electrons.size() > 0);
	firstElectron = electrons.front();
}

void TestElectronReader::testReadElectronsSize() {
	ASSERT_EQUAL(1, electrons.size());
}

void TestElectronReader::testReadFirstElectronEnergy() {
	ASSERT_EQUAL_DELTA(50.2523, firstElectron->energy(), 0.001);
}

void TestElectronReader::testReadFirstElectronIsIsolated() {
	ASSERT_EQUAL(true, firstElectron->relativeIsolation() < 0.1);
}

void TestElectronReader::testFirstElectronSigmaEtaEta() {
	ASSERT_EQUAL_DELTA(0.0089932, firstElectron->sigmaIEtaIEta(), 0.0000001);
}

void TestElectronReader::testFirstElectronDPhiIn() {
	ASSERT_EQUAL_DELTA(-0.00186157, firstElectron->dPhiIn(), 0.0000001);
}

void TestElectronReader::testFirstElectronDEtaIn() {
	ASSERT_EQUAL_DELTA(-0.00117373, firstElectron->dEtaIn(), 0.0000001);
}

void TestElectronReader::testFirstElectronHadOverEm() {
	ASSERT_EQUAL_DELTA(0, firstElectron->HadOverEm(), 0.000001);
}

void TestElectronReader::testFirstElectronCharge() {
	ASSERT_EQUAL(1, fabs(firstElectron->charge()));
}

void TestElectronReader::testFirstElectronD0() {
	ASSERT_EQUAL_DELTA(0.00216157, firstElectron->d0(), 0.0000001);
}

void TestElectronReader::testShFracInnerHits() {
	ASSERT_EQUAL(0, firstElectron->shFracInnerLayer());
}

void TestElectronReader::testCiCElectronIDVeryLooseMC() {
	CiCElectronID::value IDunderTest = CiCElectronID::eidVeryLooseMC;
	ASSERT(firstElectron->CiC_ElectronID(IDunderTest));
}

void TestElectronReader::testCiCElectronIDLooseMC() {
	CiCElectronID::value IDunderTest = CiCElectronID::eidLooseMC;
	ASSERT(firstElectron->CiC_ElectronID(IDunderTest));
}

void TestElectronReader::testCiCElectronIDMediumMC() {
	CiCElectronID::value IDunderTest = CiCElectronID::eidMediumMC;
	ASSERT(firstElectron->CiC_ElectronID(IDunderTest));
}

void TestElectronReader::testCiCElectronIDTightMC() {
	CiCElectronID::value IDunderTest = CiCElectronID::eidTightMC;
	ASSERT(firstElectron->CiC_ElectronID(IDunderTest));
}

void TestElectronReader::testCiCElectronIDSuperTightMC() {
	CiCElectronID::value IDunderTest = CiCElectronID::eidSuperTightMC;
	ASSERT(firstElectron->CiC_ElectronID(IDunderTest));
}

void TestElectronReader::testCiCElectronIDHyperTight1MC() {
	CiCElectronID::value IDunderTest = CiCElectronID::eidHyperTight1MC;
	ASSERT(firstElectron->CiC_ElectronID(IDunderTest));
}

void TestElectronReader::testCiCElectronIDHyperTight2MC() {
	CiCElectronID::value IDunderTest = CiCElectronID::eidHyperTight2MC;
	ASSERT(firstElectron->CiC_ElectronID(IDunderTest));
}

void TestElectronReader::testCiCElectronIDHyperTight3MC() {
	CiCElectronID::value IDunderTest = CiCElectronID::eidHyperTight3MC;
	ASSERT(firstElectron->CiC_ElectronID(IDunderTest));
}

void TestElectronReader::testCiCElectronIDHyperTight4MC() {
	CiCElectronID::value IDunderTest = CiCElectronID::eidHyperTight4MC;
	ASSERT(firstElectron->CiC_ElectronID(IDunderTest));
}
cute::suite make_suite_TestElectronReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestElectronReader, testReadElectronsSize));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testReadFirstElectronEnergy));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testReadFirstElectronIsIsolated));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testFirstElectronSigmaEtaEta));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testFirstElectronDPhiIn));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testFirstElectronDEtaIn));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testFirstElectronHadOverEm));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testFirstElectronCharge));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testFirstElectronD0));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testShFracInnerHits));

//    s.push_back(CUTE_SMEMFUN(TestElectronReader, testGSFTrack));
//    s.push_back(CUTE_SMEMFUN(TestElectronReader, testGSFTrackPhi));
//    s.push_back(CUTE_SMEMFUN(TestElectronReader, testGSFTrackEta));
//    s.push_back(CUTE_SMEMFUN(TestElectronReader, testGSFTrackPt));
//    s.push_back(CUTE_SMEMFUN(TestElectronReader, testGSFTrackTheta));

//    s.push_back(CUTE_SMEMFUN(TestElectronReader, testClosestCTFTrackID));
//    s.push_back(CUTE_SMEMFUN(TestElectronReader, testTrackd0));

	s.push_back(CUTE_SMEMFUN(TestElectronReader, testCiCElectronIDVeryLooseMC));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testCiCElectronIDLooseMC));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testCiCElectronIDMediumMC));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testCiCElectronIDTightMC));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testCiCElectronIDSuperTightMC));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testCiCElectronIDHyperTight1MC));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testCiCElectronIDHyperTight2MC));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testCiCElectronIDHyperTight3MC));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testCiCElectronIDHyperTight4MC));

	return s;
}
