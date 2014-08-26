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
	ASSERT_EQUAL(2, electrons.size());
}

void TestElectronReader::testReadFirstElectronEnergy() {
	ASSERT_EQUAL_DELTA(257.856, firstElectron->energy(), 0.001);
}

void TestElectronReader::testReadFirstElectronIsIsolated() {
	ASSERT_EQUAL(true, firstElectron->relativeIsolation() < 0.1);
}

void TestElectronReader::testFirstElectronSigmaEtaEta() {
	ASSERT_EQUAL_DELTA(0.0211602, firstElectron->sigmaIEtaIEta(), 0.0000001);
}

void TestElectronReader::testFirstElectronDPhiIn() {
	ASSERT_EQUAL_DELTA(-0.00968266, firstElectron->dPhiIn(), 0.0000001);
}

void TestElectronReader::testFirstElectronDEtaIn() {
	ASSERT_EQUAL_DELTA(-0.00388098, firstElectron->dEtaIn(), 0.0000001);
}

void TestElectronReader::testFirstElectronHadOverEm() {
	ASSERT_EQUAL_DELTA(0, firstElectron->HadOverEm(), 0.000001);
}

void TestElectronReader::testFirstElectronCharge() {
	ASSERT_EQUAL(1, fabs(firstElectron->charge()));
}

void TestElectronReader::testFirstElectronD0() {
	ASSERT_EQUAL_DELTA(0.0127306, firstElectron->d0(), 0.0000001);
}

void TestElectronReader::testShFracInnerHits() {
	ASSERT_EQUAL(0, firstElectron->shFracInnerLayer());
}

void TestElectronReader::testPFRelIso03() {
	ASSERT_EQUAL_DELTA(0.0137179, firstElectron->pfRelativeIsolation(0.3), 0.0000001);
}

void TestElectronReader::testPFRelIso04() {
	ASSERT_EQUAL_DELTA(0.022701, firstElectron->pfRelativeIsolation(0.4), 0.0000001);
}

void TestElectronReader::testPFRelIso05() {
	ASSERT_EQUAL_DELTA(0.022701, firstElectron->pfRelativeIsolation(0.5), 0.0000001);
}

void TestElectronReader::testPFRelIso03FallOff() {
	ASSERT_EQUAL_DELTA(65.9962, firstElectron->pfIsolationWithGaussianFallOff(0.3), 0.0001);
}

void TestElectronReader::testDirectionalRelIso02() {
	ASSERT_EQUAL_DELTA(0.0, firstElectron->directionalIsolation(0.2), 0.0000001);
}

void TestElectronReader::testDirectionalRelIso03() {
	ASSERT_EQUAL_DELTA(1.32731, firstElectron->directionalIsolation(0.3), 0.00001);
}

void TestElectronReader::testDirectionalRelIso02FallOff() {
	ASSERT_EQUAL_DELTA(2.26685, firstElectron->directionalIsolationWithGaussianFallOff(0.2), 0.00001);
}

void TestElectronReader::testDirectionalRelIso03FallOff() {
	ASSERT_EQUAL_DELTA(2.8498, firstElectron->directionalIsolationWithGaussianFallOff(0.3), 0.00001);
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

	s.push_back(CUTE_SMEMFUN(TestElectronReader, testPFRelIso03));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testPFRelIso04));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testPFRelIso05));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testPFRelIso03FallOff));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testDirectionalRelIso02));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testDirectionalRelIso03));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testDirectionalRelIso02FallOff));
	s.push_back(CUTE_SMEMFUN(TestElectronReader, testDirectionalRelIso03FallOff));

	return s;
}
