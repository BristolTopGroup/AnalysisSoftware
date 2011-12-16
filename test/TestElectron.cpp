#include "TestElectron.h"

TestElectron::TestElectron() :
		isolatedElectron(TestObjectFactory::goodIsolatedElectron()), //
		goodElectron(TestObjectFactory::goodCaloElectron()), //
		badEtElectron(TestObjectFactory::badEtElectron()), //
		badEtaElectron(TestObjectFactory::badEtaElectron()), //
		badInCrackElectron(TestObjectFactory::badInCrackElectron()), //
		badD0Electron(TestObjectFactory::badD0Electron()), //
		badElectronWithMissingInnerLayerHit(TestObjectFactory::electronWithMissingInnerLayerHit()), //
		badElectronWithPartnerTrack(TestObjectFactory::electronWithMatchingPartnerTrack()), //
		looseElectron(TestObjectFactory::goodLooseElectron()), //
		badLooseElectronNoID(TestObjectFactory::badLooseElectronNoID()), //
		badElectronNoID(TestObjectFactory::badElectronNoID()) //
{

}

void TestElectron::testEcalIsolation() {
	ASSERT_EQUAL_DELTA(0.3, isolatedElectron->ecalIsolation(), 0.01);
}

void TestElectron::testHcalIsolation() {
	ASSERT_EQUAL_DELTA(0.5, isolatedElectron->hcalIsolation(), 0.01);
}

void TestElectron::testTrackerIsolation() {
	ASSERT_EQUAL_DELTA(0.4, isolatedElectron->trackerIsolation(), 0.01);
}

void TestElectron::testBadEtElectron() {
	ASSERT(badEtElectron->isGood(false) == false);
}

void TestElectron::testBadEtaElectron() {
	ASSERT(badEtaElectron->isGood(false) == false);
}

void TestElectron::testBadInCrackElectron() {
	ASSERT(badInCrackElectron->isGood(false) == false);
}

void TestElectron::testElectronFromConversion() {
	ASSERT(badElectronWithMissingInnerLayerHit->isFromConversion());
}

void TestElectron::testElectronFromConversionWithPartnerTrack() {
	ASSERT(badElectronWithPartnerTrack->isTaggedAsConversion(0.2,0.2));
}

void TestElectron::testBadD0Electron() {
	ASSERT(badD0Electron->isGood(false) == false);
}

void TestElectron::testLooseElectron() {
	ASSERT_EQUAL(true, badD0Electron->isLoose());
	ASSERT_EQUAL(true, looseElectron->isLoose());
}

void TestElectron::testRelativeIsolation() {
	float isoSum = isolatedElectron->trackerIsolation() + isolatedElectron->ecalIsolation()
			+ isolatedElectron->hcalIsolation();
	ASSERT_EQUAL_DELTA(isoSum / isolatedElectron->et(), isolatedElectron->relativeIsolation(), 0.01);
	ASSERT(isolatedElectron->relativeIsolation() < 0.1);
}

void TestElectron::testGoodElectron() {
	ASSERT(goodElectron->isGood(ElectronID::CiCHyperTight1MC));
}

void TestElectron::testBadLooseElectronNoID() {
	ASSERT(badLooseElectronNoID->isLoose() == false);
}

void TestElectron::testBadElectronNoID() {
	ASSERT(badElectronNoID->isGood(false) == false);
}

void TestElectron::testVBTFW70Barrel() {
	Electron electron = Electron(50, 40, 30, 0);
	electron.setSigmaIEtaIEta(0.01 - 0.01 * 0.01);
	electron.setDPhiIn(0.03 - 0.01 * 0.03);
	electron.setDEtaIn(0.004 - 0.01 * 0.004);
	electron.setHadOverEm(0.025 - 0.01 * 0.025);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70BarrelNegativeValues() {
	Electron electron = Electron(50, 40, 30, 0);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.03 + 0.01 * 0.03);
	electron.setDEtaIn(-0.004 + 0.01 * 0.004);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70BarrelFailSigmaIEtaIEta() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.03 + 0.01 * 0.03);
	electron.setDEtaIn(-0.004 + 0.01 * 0.004);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70BarrelNegativeSigmaIEtaIEta() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 - 0.01 * 0.01);
	electron.setDPhiIn(-0.03 + 0.01 * 0.03);
	electron.setDEtaIn(-0.004 + 0.01 * 0.004);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70BarrelFailDPhiIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(0.03 + 0.01 * 0.03);
	electron.setDEtaIn(-0.004 + 0.01 * 0.004);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70BarrelFailNegativeDPhiIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.03 - 0.01 * 0.03);
	electron.setDEtaIn(-0.004 + 0.01 * 0.004);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70BarrelFailDEtaIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.03 + 0.01 * 0.03);
	electron.setDEtaIn(0.004 + 0.01 * 0.004);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70BarrelFailNegativeDEtaIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.03 + 0.01 * 0.03);
	electron.setDEtaIn(-0.004 - 0.01 * 0.004);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70BarrelFailHadOverEm() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.03 + 0.01 * 0.03);
	electron.setDEtaIn(-0.004 + 0.01 * 0.004);
	electron.setHadOverEm(0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70BarrelNegativeHadOverEm() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.03 + 0.01 * 0.03);
	electron.setDEtaIn(-0.004 + 0.01 * 0.004);
	electron.setHadOverEm(-0.025 - 0.01 * 0.025);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70Endcap() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(0.03 - 0.01 * 0.03);
	electron.setDPhiIn(0.02 - 0.01 * 0.02);
	electron.setDEtaIn(0.005 - 0.01 * 0.005);
	electron.setHadOverEm(0.025 - 0.01 * 0.025);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70EndcapNegativeValues() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.02 + 0.01 * 0.02);
	electron.setDEtaIn(-0.005 + 0.01 * 0.005);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70EndcapFailSigmaIEtaIEta() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.02 + 0.01 * 0.02);
	electron.setDEtaIn(-0.005 + 0.01 * 0.005);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70EndcapNegativeSigmaIEtaIEta() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 - 0.01 * 0.03);
	electron.setDPhiIn(-0.02 + 0.01 * 0.02);
	electron.setDEtaIn(-0.005 + 0.01 * 0.005);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70EndcapFailDPhiIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(0.02 + 0.01 * 0.02);
	electron.setDEtaIn(-0.005 + 0.01 * 0.005);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70EndcapFailNegativeDPhiIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.02 - 0.01 * 0.02);
	electron.setDEtaIn(-0.005 + 0.01 * 0.005);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70EndcapDEtaIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.02 + 0.01 * 0.02);
	electron.setDEtaIn(0.005 - 0.01 * 0.005);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	//expect true, dEtaIn cut is not used in the endcap at the moment
	ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70EndcapNegativeDEtaIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.02 + 0.01 * 0.02);
	electron.setDEtaIn(-0.005 + 0.01 * 0.005);
	electron.setHadOverEm(-0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	//expect true, dEtaIn cut is not used in the endcap at the moment
	ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70EndcapFailHadOverEm() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.02 + 0.01 * 0.02);
	electron.setDEtaIn(-0.005 + 0.01 * 0.005);
	electron.setHadOverEm(0.025 + 0.01 * 0.025);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW70EndcapNegativeHadOverEm() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.02 + 0.01 * 0.02);
	electron.setDEtaIn(-0.005 + 0.01 * 0.005);
	electron.setHadOverEm(-0.025 - 0.01 * 0.025);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
}

void TestElectron::testVBTFW95Barrel() {
	Electron electron = Electron(50, 40, 30, 0);
	electron.setSigmaIEtaIEta(0.01 - 0.01 * 0.01);
	electron.setDPhiIn(0.8 - 0.01 * 0.8);
	electron.setDEtaIn(0.007 - 0.01 * 0.007);
	electron.setHadOverEm(0.15 - 0.01 * 0.15);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95BarrelNegativeValues() {
	Electron electron = Electron(50, 40, 30, 0);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.8 + 0.01 * 0.8);
	electron.setDEtaIn(-0.007 + 0.01 * 0.007);
	electron.setHadOverEm(-0.15 + 0.01 * 0.15);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95BarrelFailSigmaIEtaIEta() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.8 + 0.01 * 0.8);
	electron.setDEtaIn(-0.007 + 0.01 * 0.007);
	electron.setHadOverEm(-0.15 + 0.01 * 0.15);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95BarrelNegativeSigmaIEtaIEta() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 - 0.01 * 0.01);
	electron.setDPhiIn(-0.8 + 0.01 * 0.8);
	electron.setDEtaIn(-0.007 + 0.01 * 0.007);
	electron.setHadOverEm(-0.15 + 0.01 * 0.15);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95BarrelFailDPhiIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(0.8 + 0.01 * 0.8);
	electron.setDEtaIn(-0.007 + 0.01 * 0.007);
	electron.setHadOverEm(-0.15 + 0.01 * 0.15);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95BarrelFailNegativeDPhiIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.8 - 0.01 * 0.8);
	electron.setDEtaIn(-0.007 + 0.01 * 0.007);
	electron.setHadOverEm(-0.15 + 0.01 * 0.15);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95BarrelFailDEtaIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.8 + 0.01 * 0.8);
	electron.setDEtaIn(0.007 + 0.01 * 0.007);
	electron.setHadOverEm(-0.15 + 0.01 * 0.15);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95BarrelFailNegativeDEtaIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.8 + 0.01 * 0.8);
	electron.setDEtaIn(-0.007 - 0.01 * 0.007);
	electron.setHadOverEm(-0.15 + 0.01 * 0.15);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95BarrelFailHadOverEm() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.8 + 0.01 * 0.8);
	electron.setDEtaIn(-0.007 + 0.01 * 0.007);
	electron.setHadOverEm(0.15 + 0.01 * 0.15);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95BarrelNegativeHadOverEm() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.01 + 0.01 * 0.01);
	electron.setDPhiIn(-0.8 + 0.01 * 0.8);
	electron.setDEtaIn(-0.007 + 0.01 * 0.007);
	electron.setHadOverEm(-0.15 - 0.01 * 0.15);
	electron.setSuperClusterEta(0);
	assert(electron.isInBarrelRegion());
	ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95Endcap() {
	Electron electron = Electron(50, 3, 0, 49);
	electron.setSigmaIEtaIEta(0.03 - 0.01 * 0.03);
	electron.setDPhiIn(0.7 - 0.01 * 0.7);
	electron.setDEtaIn(0.01 - 0.01 * 0.01);
	electron.setHadOverEm(0.07 - 0.01 * 0.07);
	electron.setSuperClusterEta(2.4);

	assert(electron.isInEndCapRegion());
	assert(electron.sigmaIEtaIEta() < 0.03);
	assert(fabs(electron.dPhiIn()) < 0.7);
	assert(fabs(electron.dEtaIn()) < 0.01);
	assert(electron.HadOverEm() < 0.07);
	ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95EndcapNegativeValues() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.7 + 0.01 * 0.7);
	electron.setDEtaIn(-0.01 + 0.01 * 0.01);
	electron.setHadOverEm(-0.07 + 0.01 * 0.07);
	electron.setSuperClusterEta(2.4);

	assert(electron.sigmaIEtaIEta() < 0.03);
	assert(fabs(electron.dPhiIn()) < 0.7);
	assert(fabs(electron.dEtaIn()) < 0.01);
	assert(electron.HadOverEm() < 0.07);
	assert(electron.isInEndCapRegion());
	ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95EndcapFailSigmaIEtaIEta() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.7 + 0.01 * 0.7);
	electron.setDEtaIn(-0.01 + 0.01 * 0.01);
	electron.setHadOverEm(-0.07 + 0.01 * 0.07);
	electron.setSuperClusterEta(2.4);
	assert(electron.isInEndCapRegion());
	assert(electron.sigmaIEtaIEta() > 0.03);
	assert(fabs(electron.dPhiIn()) < 0.7);
	assert(fabs(electron.dEtaIn()) < 0.01);
	assert(electron.HadOverEm() < 0.07);
	ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95EndcapNegativeSigmaIEtaIEta() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 - 0.01 * 0.03);
	electron.setDPhiIn(-0.7 + 0.01 * 0.7);
	electron.setDEtaIn(-0.01 + 0.01 * 0.01);
	electron.setHadOverEm(-0.07 + 0.01 * 0.07);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	assert(electron.sigmaIEtaIEta() < 0.03);
	assert(fabs(electron.dPhiIn()) < 0.7);
	assert(fabs(electron.dEtaIn()) < 0.01);
	assert(electron.HadOverEm() < 0.07);
	ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95EndcapFailDPhiIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(0.7 + 0.01 * 0.7);
	electron.setDEtaIn(-0.01 + 0.01 * 0.01);
	electron.setHadOverEm(-0.07 + 0.01 * 0.07);
	electron.setSuperClusterEta(2.6);
	assert(electron.sigmaIEtaIEta() < 0.03);
	assert(fabs(electron.dPhiIn()) > 0.7);
	assert(fabs(electron.dEtaIn()) < 0.01);
	assert(electron.HadOverEm() < 0.07);
	assert(electron.isInEndCapRegion());
	ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95EndcapFailNegativeDPhiIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.7 - 0.01 * 0.7);
	electron.setDEtaIn(-0.01 + 0.01 * 0.01);
	electron.setHadOverEm(-0.07 + 0.01 * 0.07);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	assert(electron.sigmaIEtaIEta() < 0.03);
	assert(fabs(electron.dPhiIn()) > 0.7);
	assert(fabs(electron.dEtaIn()) < 0.01);
	assert(electron.HadOverEm() < 0.07);
	ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95EndcapDEtaIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.7 + 0.01 * 0.7);
	electron.setDEtaIn(0.01 - 0.01 * 0.01);
	electron.setHadOverEm(-0.07 + 0.01 * 0.07);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	assert(electron.sigmaIEtaIEta() < 0.03);
	assert(fabs(electron.dPhiIn()) < 0.7);
	assert(fabs(electron.dEtaIn()) < 0.01);
	assert(electron.HadOverEm() < 0.07);
	ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95EndcapNegativeDEtaIn() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.7 + 0.01 * 0.7);
	electron.setDEtaIn(-0.01 + 0.01 * 0.01);
	electron.setHadOverEm(-0.07 + 0.01 * 0.07);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	assert(electron.sigmaIEtaIEta() < 0.03);
	assert(fabs(electron.dPhiIn()) < 0.7);
	assert(fabs(electron.dEtaIn()) < 0.01);
	assert(electron.HadOverEm() < 0.07);
	//expect true, dEtaIn cut is not used in the endcap at the moment
	ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95EndcapFailHadOverEm() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.7 + 0.01 * 0.7);
	electron.setDEtaIn(-0.01 + 0.01 * 0.01);
	electron.setHadOverEm(0.07 + 0.01 * 0.07);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	assert(electron.sigmaIEtaIEta() < 0.03);
	assert(fabs(electron.dPhiIn()) < 0.7);
	assert(fabs(electron.dEtaIn()) < 0.01);
	assert(electron.HadOverEm() > 0.07);
	ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
}

void TestElectron::testVBTFW95EndcapNegativeHadOverEm() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setSigmaIEtaIEta(-0.03 + 0.01 * 0.03);
	electron.setDPhiIn(-0.7 + 0.01 * 0.7);
	electron.setDEtaIn(-0.01 + 0.01 * 0.01);
	electron.setHadOverEm(-0.07 - 0.01 * 0.07);
	electron.setSuperClusterEta(2.6);
	assert(electron.isInEndCapRegion());
	assert(electron.sigmaIEtaIEta() < 0.03);
	assert(fabs(electron.dPhiIn()) < 0.7);
	assert(fabs(electron.dEtaIn()) < 0.01);
	assert(electron.HadOverEm() < 0.07);
	ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
}

void TestElectron::testElectronCharge() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setCharge(2);
	ASSERT_EQUAL(2, electron.charge());
}

void TestElectron::testElectronChargeInCopy() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setCharge(2);
	Electron copyElectron(electron);
	ASSERT_EQUAL(2, copyElectron.charge());
}

void TestElectron::testElectronInCollection() {
	ElectronCollection coll;
	ElectronPointer electron(new Electron(50, 3, 0, 40));
	ElectronPointer electron2(new Electron(50, 3, 0, 40));
	electron->setCharge(2);
	electron2->setCharge(2);
	coll.push_back(electron);
	coll.push_back(electron2);
	ASSERT_EQUAL(2, coll.front()->charge());
}

void TestElectron::testElectronInSTDCollection() {
	std::vector<Electron> coll;
	Electron electron(50, 3, 0, 40);
	Electron electron2(50, 3, 0, 40);
	electron.setCharge(2);
	electron2.setCharge(2);
	coll.push_back(electron);
	coll.push_back(electron2);
	ASSERT_EQUAL(2, coll.front().charge());
}

void TestElectron::testElectronSetMass() {
	Electron electron = Electron(50, 3, 0, 40);
	electron.setMass(500);
	ASSERT_EQUAL(500, electron.mass());
}

void TestElectron::testGSFTrack() {
	TrackPointer track = TrackPointer(new Track(20, 20, 0, 0));
	goodElectron->setGSFTrack(track);
	ASSERT_EQUAL(track, goodElectron->GSFTrack());
}

void TestElectron::testCiCElectronIDVeryLooseMC() {
	int passId = 0;
	CiCElectronID::value IDunderTest = (CiCElectronID::value) ElectronID::CiCVeryLooseMC;
	passId = passId | 1 << (int) IDunderTest;

	goodElectron->setCompressedCiCElectronID(passId);
	ASSERT(goodElectron->CiC_ElectronID(IDunderTest));
}

void TestElectron::testCiCElectronIDLooseMC() {
	int passId = 0;
	CiCElectronID::value IDunderTest = CiCElectronID::eidLooseMC;
	passId = passId | 1 << (int) IDunderTest;

	goodElectron->setCompressedCiCElectronID(passId);
	ASSERT(goodElectron->CiC_ElectronID(IDunderTest));
}

void TestElectron::testCiCElectronIDMediumMC() {
	int passId = 0;
	CiCElectronID::value IDunderTest = CiCElectronID::eidMediumMC;
	passId = passId | 1 << (int) IDunderTest;

	goodElectron->setCompressedCiCElectronID(passId);
	ASSERT(goodElectron->CiC_ElectronID(IDunderTest));
}

void TestElectron::testCiCElectronIDTightMC() {
	int passId = 0;
	CiCElectronID::value IDunderTest = CiCElectronID::eidTightMC;
	passId = passId | 1 << (int) IDunderTest;

	goodElectron->setCompressedCiCElectronID(passId);
	ASSERT(goodElectron->CiC_ElectronID(IDunderTest));
}

void TestElectron::testCiCElectronIDSuperTightMC() {
	int passId = 0;
	CiCElectronID::value IDunderTest = CiCElectronID::eidSuperTightMC;
	passId = passId | 1 << (int) IDunderTest;

	goodElectron->setCompressedCiCElectronID(passId);
	ASSERT(goodElectron->CiC_ElectronID(IDunderTest));
}

void TestElectron::testCiCElectronIDHyperTight1MC() {
	int passId = 0;
	CiCElectronID::value IDunderTest = CiCElectronID::eidHyperTight1MC;
	passId = passId | 1 << (int) IDunderTest;

	goodElectron->setCompressedCiCElectronID(passId);
	ASSERT(goodElectron->CiC_ElectronID(IDunderTest));
}

void TestElectron::testCiCElectronIDHyperTight2MC() {
	int passId = 0;
	CiCElectronID::value IDunderTest = CiCElectronID::eidHyperTight2MC;
	passId = passId | 1 << (int) IDunderTest;

	goodElectron->setCompressedCiCElectronID(passId);
	ASSERT(goodElectron->CiC_ElectronID(IDunderTest));
}

void TestElectron::testCiCElectronIDHyperTight3MC() {
	int passId = 0;
	CiCElectronID::value IDunderTest = CiCElectronID::eidHyperTight3MC;
	passId = passId | 1 << (int) IDunderTest;

	goodElectron->setCompressedCiCElectronID(passId);
	ASSERT(goodElectron->CiC_ElectronID(IDunderTest));
}

void TestElectron::testCiCElectronIDHyperTight4MC() {
	int passId = 0;
	CiCElectronID::value IDunderTest = CiCElectronID::eidHyperTight4MC;
	passId = passId | 1 << (int) IDunderTest;
	goodElectron->setCompressedCiCElectronID(passId);
	ASSERT(goodElectron->CiC_ElectronID(IDunderTest));
	ASSERT(!goodElectron->CiC_ElectronID(CiCElectronID::eidHyperTight3MC));
}

void TestElectron::testCiCElectronIDNoID() {
	int passId = 0;
	goodElectron->setCompressedCiCElectronID(passId);
	//if passId = 0 all IDs should be false
	for (unsigned int id = 0; id < CiCElectronID::NUMBER_OF_CiCIds; ++id)
		ASSERT(!goodElectron->CiC_ElectronID((CiCElectronID::value) id));
}

void TestElectron::testCiCElectronIDMoreThanOneID() {
	int passId = 0;
	goodElectron->setCompressedCiCElectronID(passId);
	CiCElectronID::value IDunderTest = CiCElectronID::eidHyperTight3MC;
	CiCElectronID::value otherIDunderTest = CiCElectronID::eidHyperTight4MC;
	passId = passId | 1 << (int) IDunderTest;
	passId = passId | 1 << (int) otherIDunderTest;

	goodElectron->setCompressedCiCElectronID(passId);

	for (unsigned int id = 0; id < CiCElectronID::NUMBER_OF_CiCIds; ++id) {
		CiCElectronID::value testId = (CiCElectronID::value) id;
		if (testId == IDunderTest || testId == otherIDunderTest) {
			ASSERT(goodElectron->CiC_ElectronID(testId));
		} else {
			ASSERT(!goodElectron->CiC_ElectronID(testId));
		}
	}

}

cute::suite make_suite_TestElectron() {
	cute::suite s;
	s.push_back(CUTE_SMEMFUN(TestElectron, testRelativeIsolation));
	s.push_back(CUTE_SMEMFUN(TestElectron, testGoodElectron));
	s.push_back(CUTE_SMEMFUN(TestElectron, testBadEtElectron));
	s.push_back(CUTE_SMEMFUN(TestElectron, testBadEtaElectron));
	s.push_back(CUTE_SMEMFUN(TestElectron, testBadD0Electron));
	s.push_back(CUTE_SMEMFUN(TestElectron, testBadInCrackElectron));
	s.push_back(CUTE_SMEMFUN(TestElectron, testElectronFromConversion));
	s.push_back(CUTE_SMEMFUN(TestElectron, testElectronFromConversionWithPartnerTrack));
	s.push_back(CUTE_SMEMFUN(TestElectron, testEcalIsolation));
	s.push_back(CUTE_SMEMFUN(TestElectron, testHcalIsolation));
	s.push_back(CUTE_SMEMFUN(TestElectron, testTrackerIsolation));
	s.push_back(CUTE_SMEMFUN(TestElectron, testLooseElectron));
	s.push_back(CUTE_SMEMFUN(TestElectron, testBadLooseElectronNoID));
	s.push_back(CUTE_SMEMFUN(TestElectron, testBadElectronNoID));

	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70Barrel));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70BarrelNegativeValues));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70BarrelFailSigmaIEtaIEta));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70BarrelNegativeSigmaIEtaIEta));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70BarrelFailDPhiIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70BarrelFailNegativeDPhiIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70BarrelFailDEtaIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70BarrelFailNegativeDEtaIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70BarrelFailHadOverEm));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70BarrelNegativeHadOverEm));

	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70Endcap));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70EndcapNegativeValues));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70EndcapFailSigmaIEtaIEta));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70EndcapNegativeSigmaIEtaIEta));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70EndcapFailDPhiIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70EndcapFailNegativeDPhiIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70EndcapDEtaIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70EndcapNegativeDEtaIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70EndcapFailHadOverEm));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW70EndcapNegativeHadOverEm));

	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95Barrel));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95BarrelNegativeValues));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95BarrelFailSigmaIEtaIEta));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95BarrelNegativeSigmaIEtaIEta));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95BarrelFailDPhiIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95BarrelFailNegativeDPhiIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95BarrelFailDEtaIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95BarrelFailNegativeDEtaIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95BarrelFailHadOverEm));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95BarrelNegativeHadOverEm));

	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95Endcap));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95EndcapNegativeValues));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95EndcapFailSigmaIEtaIEta));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95EndcapNegativeSigmaIEtaIEta));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95EndcapFailDPhiIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95EndcapFailNegativeDPhiIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95EndcapDEtaIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95EndcapNegativeDEtaIn));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95EndcapFailHadOverEm));
	s.push_back(CUTE_SMEMFUN(TestElectron, testVBTFW95EndcapNegativeHadOverEm));

	s.push_back(CUTE_SMEMFUN(TestElectron, testElectronCharge));
	s.push_back(CUTE_SMEMFUN(TestElectron, testElectronChargeInCopy));
	s.push_back(CUTE_SMEMFUN(TestElectron, testElectronInCollection));
	s.push_back(CUTE_SMEMFUN(TestElectron, testElectronSetMass));
	s.push_back(CUTE_SMEMFUN(TestElectron, testElectronInSTDCollection));

	s.push_back(CUTE_SMEMFUN(TestElectron, testCiCElectronIDVeryLooseMC));
	s.push_back(CUTE_SMEMFUN(TestElectron, testCiCElectronIDLooseMC));
	s.push_back(CUTE_SMEMFUN(TestElectron, testCiCElectronIDMediumMC));
	s.push_back(CUTE_SMEMFUN(TestElectron, testCiCElectronIDTightMC));
	s.push_back(CUTE_SMEMFUN(TestElectron, testCiCElectronIDSuperTightMC));
	s.push_back(CUTE_SMEMFUN(TestElectron, testCiCElectronIDHyperTight1MC));
	s.push_back(CUTE_SMEMFUN(TestElectron, testCiCElectronIDHyperTight2MC));
	s.push_back(CUTE_SMEMFUN(TestElectron, testCiCElectronIDHyperTight3MC));
	s.push_back(CUTE_SMEMFUN(TestElectron, testCiCElectronIDHyperTight4MC));
	s.push_back(CUTE_SMEMFUN(TestElectron, testCiCElectronIDNoID));
	s.push_back(CUTE_SMEMFUN(TestElectron, testCiCElectronIDMoreThanOneID));

	s.push_back(CUTE_SMEMFUN(TestElectron, testGSFTrack));
	return s;
}
