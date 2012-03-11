/*
 * TestLepton.h
 *
 *  Created on: 11 Mar 2012
 *      Author: kreczko
 */

#ifndef TESTLEPTON_H_
#define TESTLEPTON_H_
//lepton is an abstract class, using Muon to test it!
#include "../interface/RecoObjects/Muon.h"
#include "cute/cute.h"
#include "cute/cute_suite.h"

class TestLepton {
public:
	TestLepton();
	virtual ~TestLepton();

	void testEcalIsolation03();
	void testHcalIsolation03();
	void testTrackerIsolation03();
	void testEcalIsolation04();
	void testHcalIsolation04();
	void testTrackerIsolation04();
	void testEcalIsolation05();
	void testHcalIsolation05();
	void testTrackerIsolation05();
	void testRelativeIsolation03();
	void testRelativeIsolation04();
	void testRelativeIsolation05();

	void testPFGammaIsolation03();
	void testPFNeutralHadronIsolation03();
	void testPFChargedHadronIsolation03();
	void testPFGammaIsolation04();
	void testPFNeutralHadronIsolation04();
	void testPFChargedHadronIsolation04();
	void testPFGammaIsolation05();
	void testPFNeutralHadronIsolation05();
	void testPFChargedHadronIsolation05();
	void testPFRelativeIsolation03();
	void testPFRelativeIsolation04();
	void testPFRelativeIsolation05();

	void testDirectionalIsolation02();
	void testDirectionalIsolation03();
	void testDirectionalIsolationWithGaussianFallOff02();
	void testDirectionalIsolationWithGaussianFallOff03();

	void testPFIsolationWithGaussianFallOff02();
	void testPFIsolationWithGaussianFallOff03();

private:
	BAT::MuonPointer muon_;
};

extern cute::suite make_suite_TestLepton();
#endif /* TESTLEPTON_H_ */
