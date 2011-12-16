#ifndef TESTELECTRON_H_
#define TESTELECTRON_H_
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/RecoObjects/Electron.h"
#include "TestObjectFactory.h"
using namespace BAT;

struct TestElectron {
private:
	ElectronPointer isolatedElectron;
	ElectronPointer goodElectron;
	ElectronPointer badEtElectron;
	ElectronPointer badEtaElectron;
	ElectronPointer badInCrackElectron;
	ElectronPointer badD0Electron;
	ElectronPointer badElectronWithMissingInnerLayerHit;
	ElectronPointer badElectronWithPartnerTrack;
	ElectronPointer looseElectron;
	ElectronPointer badLooseElectronNoID;
	ElectronPointer badElectronNoID;

public:
	TestElectron();
	~TestElectron() {
	}
	;

public:
	void testEcalIsolation();
	void testHcalIsolation();
	void testTrackerIsolation();
	void testBadEtElectron();

	void testBadEtaElectron();

	void testBadInCrackElectron();

	void testElectronFromConversion();
	void testElectronFromConversionWithPartnerTrack();
	void testBadD0Electron();
	void testLooseElectron();
	void testRelativeIsolation();
	void testGoodElectron();
	void testBadLooseElectronNoID();
	void testBadElectronNoID();
	void testVBTFW70Barrel();
	void testVBTFW70BarrelNegativeValues();
	void testVBTFW70BarrelFailSigmaIEtaIEta();
	void testVBTFW70BarrelNegativeSigmaIEtaIEta();
	void testVBTFW70BarrelFailDPhiIn();
	void testVBTFW70BarrelFailNegativeDPhiIn();
	void testVBTFW70BarrelFailDEtaIn();
	void testVBTFW70BarrelFailNegativeDEtaIn();
	void testVBTFW70BarrelFailHadOverEm();
	void testVBTFW70BarrelNegativeHadOverEm();
	void testVBTFW70Endcap();
	void testVBTFW70EndcapNegativeValues();
	void testVBTFW70EndcapFailSigmaIEtaIEta();
	void testVBTFW70EndcapNegativeSigmaIEtaIEta();
	void testVBTFW70EndcapFailDPhiIn();
	void testVBTFW70EndcapFailNegativeDPhiIn();
	void testVBTFW70EndcapDEtaIn();
	void testVBTFW70EndcapNegativeDEtaIn();
	void testVBTFW70EndcapFailHadOverEm();
	void testVBTFW70EndcapNegativeHadOverEm();
	void testVBTFW95Barrel();
	void testVBTFW95BarrelNegativeValues();
	void testVBTFW95BarrelFailSigmaIEtaIEta();
	void testVBTFW95BarrelNegativeSigmaIEtaIEta();
	void testVBTFW95BarrelFailDPhiIn();
	void testVBTFW95BarrelFailNegativeDPhiIn();
	void testVBTFW95BarrelFailDEtaIn();
	void testVBTFW95BarrelFailNegativeDEtaIn();
	void testVBTFW95BarrelFailHadOverEm();
	void testVBTFW95BarrelNegativeHadOverEm();
	void testVBTFW95Endcap();
	void testVBTFW95EndcapNegativeValues();
	void testVBTFW95EndcapFailSigmaIEtaIEta();
	void testVBTFW95EndcapNegativeSigmaIEtaIEta();
	void testVBTFW95EndcapFailDPhiIn();
	void testVBTFW95EndcapFailNegativeDPhiIn();
	void testVBTFW95EndcapDEtaIn();
	void testVBTFW95EndcapNegativeDEtaIn();
	void testVBTFW95EndcapFailHadOverEm();
	void testVBTFW95EndcapNegativeHadOverEm();
	void testElectronCharge();
	void testElectronChargeInCopy();
	void testElectronInCollection();
	void testElectronInSTDCollection();
	void testElectronSetMass();
	void testGSFTrack();
	void testCiCElectronIDVeryLooseMC();
	void testCiCElectronIDLooseMC();
	void testCiCElectronIDMediumMC();
	void testCiCElectronIDTightMC();
	void testCiCElectronIDSuperTightMC();
	void testCiCElectronIDHyperTight1MC();
	void testCiCElectronIDHyperTight2MC();
	void testCiCElectronIDHyperTight3MC();
	void testCiCElectronIDHyperTight4MC();
	void testCiCElectronIDNoID();
	void testCiCElectronIDMoreThanOneID();
};

extern cute::suite make_suite_TestElectron();

#endif
