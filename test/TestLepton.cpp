/*
 * TestLepton.cpp
 *
 *  Created on: 11 Mar 2012
 *      Author: kreczko
 */

#include "TestLepton.h"
#include "TestObjectFactory.h"
using namespace BAT;

TestLepton::TestLepton() :
		muon_(new Muon(50, 40, 30, 0)) {
	muon_->setEcalIsolation(3, 0.3);
	muon_->setEcalIsolation(4, 0.4);
	muon_->setEcalIsolation(5, 0.5);
	muon_->setHcalIsolation(3, 0.3);
	muon_->setHcalIsolation(4, 0.4);
	muon_->setHcalIsolation(5, 0.5);
	muon_->setTrackerIsolation(3, 0.3);
	muon_->setTrackerIsolation(4, 0.4);
	muon_->setTrackerIsolation(5, 0.5);

	muon_->setPFGammaIsolation(3, 0.3);
	muon_->setPFGammaIsolation(4, 0.4);
	muon_->setPFGammaIsolation(5, 0.5);

	muon_->setPFChargedHadronIsolation(3, 0.3);
	muon_->setPFChargedHadronIsolation(4, 0.4);
	muon_->setPFChargedHadronIsolation(5, 0.5);

	muon_->setPFNeutralHadronIsolation(3, 0.3);
	muon_->setPFNeutralHadronIsolation(4, 0.4);
	muon_->setPFNeutralHadronIsolation(5, 0.5);

	muon_->setDirectionalIsolation(3, 0.2);
	muon_->setDirectionalIsolation(4, 0.3);

}

TestLepton::~TestLepton() {
}

void TestLepton::testEcalIsolation03() {
	muon_->setEcalIsolation(1234., 0.3);
	ASSERT_EQUAL(1234., muon_->ecalIsolation());
}

void TestLepton::testHcalIsolation03() {
	muon_->setHcalIsolation(1234., 0.3);
	ASSERT_EQUAL(1234., muon_->hcalIsolation());
}

void TestLepton::testTrackerIsolation03() {
	muon_->setTrackerIsolation(1234., 0.3);
	ASSERT_EQUAL(1234., muon_->trackerIsolation());
}

void TestLepton::testEcalIsolation04() {
	muon_->setEcalIsolation(1234., 0.4);
	ASSERT_EQUAL(1234., muon_->ecalIsolation(0.4));
}

void TestLepton::testHcalIsolation04() {
	muon_->setHcalIsolation(1234., 0.4);
	ASSERT_EQUAL(1234., muon_->hcalIsolation(0.4));
}

void TestLepton::testTrackerIsolation04() {
	muon_->setTrackerIsolation(1234., 0.4);
	ASSERT_EQUAL(1234., muon_->trackerIsolation(0.4));
}

void TestLepton::testEcalIsolation05() {
	muon_->setEcalIsolation(1234., 0.5);
	ASSERT_EQUAL(1234., muon_->ecalIsolation(0.5));
}

void TestLepton::testHcalIsolation05() {
	muon_->setHcalIsolation(1234., 0.5);
	ASSERT_EQUAL(1234., muon_->hcalIsolation(0.5));
}

void TestLepton::testTrackerIsolation05() {
	muon_->setTrackerIsolation(1234., 0.5);
	ASSERT_EQUAL(1234., muon_->trackerIsolation(0.5));
}

void TestLepton::testRelativeIsolation03() {
	float relativeIsolation = (muon_->ecalIsolation(0.3) + muon_->hcalIsolation(0.3) + muon_->trackerIsolation(0.3))
			/ muon_->pt();
	ASSERT_EQUAL(relativeIsolation, muon_->relativeIsolation(0.3));
}

void TestLepton::testRelativeIsolation04() {
	float relativeIsolation = (muon_->ecalIsolation(0.4) + muon_->hcalIsolation(0.4) + muon_->trackerIsolation(0.4))
			/ muon_->pt();
	ASSERT_EQUAL(relativeIsolation, muon_->relativeIsolation(0.4));
}

void TestLepton::testRelativeIsolation05() {
	float relativeIsolation = (muon_->ecalIsolation(0.5) + muon_->hcalIsolation(0.5) + muon_->trackerIsolation(0.5))
			/ muon_->pt();
	ASSERT_EQUAL(relativeIsolation, muon_->relativeIsolation(0.5));
}

void TestLepton::testPFGammaIsolation03() {
	muon_->setPFGammaIsolation(1234., 0.3);
	ASSERT_EQUAL(1234., muon_->PFGammaIsolation(0.3));
}

void TestLepton::testPFNeutralHadronIsolation03() {
	muon_->setPFNeutralHadronIsolation(1234., 0.3);
	ASSERT_EQUAL(1234., muon_->PFNeutralHadronIsolation(0.3));
}

void TestLepton::testPFChargedHadronIsolation03() {
	muon_->setPFChargedHadronIsolation(1234., 0.3);
	ASSERT_EQUAL(1234., muon_->PFChargedHadronIsolation(0.3));
}

void TestLepton::testPFGammaIsolation04() {
	muon_->setPFGammaIsolation(1234., 0.4);
	ASSERT_EQUAL(1234., muon_->PFGammaIsolation(0.4));
}

void TestLepton::testPFNeutralHadronIsolation04() {
	muon_->setPFNeutralHadronIsolation(1234., 0.4);
	ASSERT_EQUAL(1234., muon_->PFNeutralHadronIsolation(0.4));
}

void TestLepton::testPFChargedHadronIsolation04() {
	muon_->setPFChargedHadronIsolation(1234., 0.4);
	ASSERT_EQUAL(1234., muon_->PFChargedHadronIsolation(0.4));
}

void TestLepton::testPFGammaIsolation05() {
	muon_->setPFGammaIsolation(1234., 0.5);
	ASSERT_EQUAL(1234., muon_->PFGammaIsolation(0.5));
}

void TestLepton::testPFNeutralHadronIsolation05() {
	muon_->setPFNeutralHadronIsolation(1234., 0.5);
	ASSERT_EQUAL(1234., muon_->PFNeutralHadronIsolation(0.5));
}

void TestLepton::testPFChargedHadronIsolation05() {
	muon_->setPFChargedHadronIsolation(1234., 0.5);
	ASSERT_EQUAL(1234., muon_->PFChargedHadronIsolation(0.5));
}

void TestLepton::testPFRelativeIsolation03() {
	float relativeIsolation = (muon_->PFGammaIsolation(0.3) + muon_->PFNeutralHadronIsolation(0.3)
			+ muon_->PFChargedHadronIsolation(0.3)) / muon_->pt();
	ASSERT_EQUAL(relativeIsolation, muon_->relativeIsolation(0.3));
}

void TestLepton::testPFRelativeIsolation04() {
	float relativeIsolation = (muon_->PFGammaIsolation(0.4) + muon_->PFNeutralHadronIsolation(0.4)
			+ muon_->PFChargedHadronIsolation(0.4)) / muon_->pt();
	ASSERT_EQUAL(relativeIsolation, muon_->relativeIsolation(0.4));
}

void TestLepton::testPFRelativeIsolation05() {
	float relativeIsolation = (muon_->PFGammaIsolation(0.5) + muon_->PFNeutralHadronIsolation(0.5)
			+ muon_->PFChargedHadronIsolation(0.5)) / muon_->pt();
	ASSERT_EQUAL(relativeIsolation, muon_->pfRelativeIsolation(0.5));
}

void TestLepton::testDirectionalIsolation02() {
	muon_->setDirectionalIsolation(1234., 0.2);
	ASSERT_EQUAL(1234., muon_->directionalIsolation(0.2));
}

void TestLepton::testDirectionalIsolation03() {
	muon_->setDirectionalIsolation(1234., 0.3);
	ASSERT_EQUAL(1234., muon_->directionalIsolation(0.3));
}

void TestLepton::testDirectionalIsolationWithGaussianFallOff02() {
	muon_->setDirectionalIsolationWithGaussianFallOff(1234., 0.2);
	ASSERT_EQUAL(1234., muon_->directionalIsolationWithGaussianFallOff(0.2));
}

void TestLepton::testDirectionalIsolationWithGaussianFallOff03() {
	muon_->setDirectionalIsolationWithGaussianFallOff(1234., 0.3);
	ASSERT_EQUAL(1234., muon_->directionalIsolationWithGaussianFallOff(0.3));
}

void TestLepton::testPFIsolationWithGaussianFallOff02() {
	muon_->setPFIsolationWithGaussianFallOff(1234., 0.2);
	ASSERT_EQUAL(1234., muon_->pfIsolationWithGaussianFallOff(0.2));
}

void TestLepton::testPFIsolationWithGaussianFallOff03() {
	muon_->setPFIsolationWithGaussianFallOff(1234., 0.3);
	ASSERT_EQUAL(1234., muon_->pfIsolationWithGaussianFallOff(0.3));
}
cute::suite make_suite_TestLepton() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestLepton, testEcalIsolation03));
	s.push_back(CUTE_SMEMFUN(TestLepton, testHcalIsolation03));
	s.push_back(CUTE_SMEMFUN(TestLepton, testTrackerIsolation03));
	s.push_back(CUTE_SMEMFUN(TestLepton, testEcalIsolation04));
	s.push_back(CUTE_SMEMFUN(TestLepton, testHcalIsolation04));
	s.push_back(CUTE_SMEMFUN(TestLepton, testTrackerIsolation04));
	s.push_back(CUTE_SMEMFUN(TestLepton, testEcalIsolation05));
	s.push_back(CUTE_SMEMFUN(TestLepton, testHcalIsolation05));
	s.push_back(CUTE_SMEMFUN(TestLepton, testTrackerIsolation05));
	s.push_back(CUTE_SMEMFUN(TestLepton, testRelativeIsolation03));
	s.push_back(CUTE_SMEMFUN(TestLepton, testRelativeIsolation04));
	s.push_back(CUTE_SMEMFUN(TestLepton, testRelativeIsolation05));

	s.push_back(CUTE_SMEMFUN(TestLepton, testPFGammaIsolation03));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFNeutralHadronIsolation03));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFChargedHadronIsolation03));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFGammaIsolation04));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFNeutralHadronIsolation04));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFChargedHadronIsolation04));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFGammaIsolation05));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFNeutralHadronIsolation05));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFChargedHadronIsolation05));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFRelativeIsolation03));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFRelativeIsolation04));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFRelativeIsolation05));

	s.push_back(CUTE_SMEMFUN(TestLepton, testDirectionalIsolation02));
	s.push_back(CUTE_SMEMFUN(TestLepton, testDirectionalIsolation03));
	s.push_back(CUTE_SMEMFUN(TestLepton, testDirectionalIsolationWithGaussianFallOff02));
	s.push_back(CUTE_SMEMFUN(TestLepton, testDirectionalIsolationWithGaussianFallOff03));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFIsolationWithGaussianFallOff02));
	s.push_back(CUTE_SMEMFUN(TestLepton, testPFIsolationWithGaussianFallOff03));

//

	return s;
}
