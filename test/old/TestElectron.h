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

	void testRelativeIsolation();
	void testVBTFWP70Barrel();
	void testVBTFWP70BarrelNegativeValues();
	void testVBTFWP70BarrelFailSigmaIEtaIEta();
	void testVBTFWP70BarrelNegativeSigmaIEtaIEta();
	void testVBTFWP70BarrelFailDPhiIn();
	void testVBTFWP70BarrelFailNegativeDPhiIn();
	void testVBTFWP70BarrelFailDEtaIn();
	void testVBTFWP70BarrelFailNegativeDEtaIn();
	void testVBTFWP70BarrelFailHadOverEm();
	void testVBTFWP70BarrelNegativeHadOverEm();
	void testVBTFWP70Endcap();
	void testVBTFWP70EndcapNegativeValues();
	void testVBTFWP70EndcapFailSigmaIEtaIEta();
	void testVBTFWP70EndcapNegativeSigmaIEtaIEta();
	void testVBTFWP70EndcapFailDPhiIn();
	void testVBTFWP70EndcapFailNegativeDPhiIn();
	void testVBTFWP70EndcapDEtaIn();
	void testVBTFWP70EndcapNegativeDEtaIn();
	void testVBTFWP70EndcapFailHadOverEm();
	void testVBTFWP70EndcapNegativeHadOverEm();
	void testVBTFWP95Barrel();
	void testVBTFWP95BarrelNegativeValues();
	void testVBTFWP95BarrelFailSigmaIEtaIEta();
	void testVBTFWP95BarrelNegativeSigmaIEtaIEta();
	void testVBTFWP95BarrelFailDPhiIn();
	void testVBTFWP95BarrelFailNegativeDPhiIn();
	void testVBTFWP95BarrelFailDEtaIn();
	void testVBTFWP95BarrelFailNegativeDEtaIn();
	void testVBTFWP95BarrelFailHadOverEm();
	void testVBTFWP95BarrelNegativeHadOverEm();
	void testVBTFWP95Endcap();
	void testVBTFWP95EndcapNegativeValues();
	void testVBTFWP95EndcapFailSigmaIEtaIEta();
	void testVBTFWP95EndcapNegativeSigmaIEtaIEta();
	void testVBTFWP95EndcapFailDPhiIn();
	void testVBTFWP95EndcapFailNegativeDPhiIn();
	void testVBTFWP95EndcapDEtaIn();
	void testVBTFWP95EndcapNegativeDEtaIn();
	void testVBTFWP95EndcapFailHadOverEm();
	void testVBTFWP95EndcapNegativeHadOverEm();
	void testElectronCharge();
	void testElectronChargeInCopy();
	void testElectronInCollection();
	void testElectronInSTDCollection();
	void testElectronSetMass();
	void testGSFTrack();
};

extern cute::suite make_suite_TestElectron();

#endif
