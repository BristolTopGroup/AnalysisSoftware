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
    TestElectron() :
        isolatedElectron(TestObjectFactory::goodIsolatedElectron()),
        goodElectron(TestObjectFactory::goodCaloElectron()),
        badEtElectron(TestObjectFactory::badEtElectron()),
        badEtaElectron(TestObjectFactory::badEtaElectron()),
        badInCrackElectron(TestObjectFactory::badInCrackElectron()),
        badD0Electron(TestObjectFactory::badD0Electron()),
        badElectronWithMissingInnerLayerHit(TestObjectFactory::electronWithMissingInnerLayerHit()),
        badElectronWithPartnerTrack(TestObjectFactory::electronWithMatchingPartnerTrack()),
        looseElectron(TestObjectFactory::goodLooseElectron()),
        badLooseElectronNoID(TestObjectFactory::badLooseElectronNoID()),
        badElectronNoID(TestObjectFactory::badElectronNoID()) {
    }

public:
    void testEcalIsolation() {
        ASSERT_EQUAL_DELTA(0.3, isolatedElectron->ecalIsolation(), 0.01);
    }

    void testHcalIsolation() {
        ASSERT_EQUAL_DELTA(0.5, isolatedElectron->hcalIsolation(), 0.01);
    }

    void testTrackerIsolation() {
        ASSERT_EQUAL_DELTA(0.4, isolatedElectron->trackerIsolation(), 0.01);
    }

    void testBadEtElectron() {
        ASSERT(badEtElectron->isGood() == false);
    }

    void testBadEtaElectron() {
        ASSERT(badEtaElectron->isGood() == false);
    }

    void testBadInCrackElectron() {
        ASSERT(badInCrackElectron->isGood() == false);
    }

    void testElectronFromConversion() {
        ASSERT(badElectronWithMissingInnerLayerHit->isFromConversion());
    }

    void testElectronFromConversionWithPartnerTrack() {
        ASSERT(badElectronWithPartnerTrack->isTaggedAsConversion(0.2,0.2));
    }

    void testBadD0Electron() {
        ASSERT(badD0Electron->isGood() == false);
    }

    void testLooseElectron() {
        ASSERT_EQUAL(true, badD0Electron->isLoose());
        ASSERT_EQUAL(true, looseElectron->isLoose());
    }

    void testRelativeIsolation() {
        float isoSum = isolatedElectron->trackerIsolation() + isolatedElectron->ecalIsolation()
                + isolatedElectron->hcalIsolation();
        ASSERT_EQUAL_DELTA(isoSum / isolatedElectron->et(), isolatedElectron->relativeIsolation(), 0.01);
        ASSERT(isolatedElectron->isIsolated());
    }

    void testGoodElectron() {
        ASSERT(goodElectron->isGood());
    }

    void testBadLooseElectronNoID() {
        ASSERT(badLooseElectronNoID->isLoose() == false);
    }

    void testBadElectronNoID() {
        ASSERT(badElectronNoID->isGood() == false);
    }

    void testVBTFW70Barrel() {
        Electron electron = Electron(50, 40, 30, 0);
        electron.setSigmaIEtaIEta(0.01 - 0.01
                * 0.01);
        electron.setDPhiIn(0.03 - 0.01 * 0.03);
        electron.setDEtaIn(0.004 - 0.01 * 0.004);
        electron.setHadOverEm(0.025 - 0.01 * 0.025);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelNegativeValues() {
        Electron electron = Electron(50, 40, 30, 0);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.03 + 0.01 * 0.03);
        electron.setDEtaIn(-0.004 + 0.01 * 0.004);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelFailSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.03 + 0.01 * 0.03);
        electron.setDEtaIn(-0.004 + 0.01 * 0.004);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelNegativeSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 - 0.01
                * 0.01);
        electron.setDPhiIn(-0.03 + 0.01 * 0.03);
        electron.setDEtaIn(-0.004 + 0.01 * 0.004);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelFailDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(0.03 + 0.01 * 0.03);
        electron.setDEtaIn(-0.004 + 0.01 * 0.004);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelFailNegativeDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.03 - 0.01 * 0.03);
        electron.setDEtaIn(-0.004 + 0.01 * 0.004);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelFailDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.03 + 0.01 * 0.03);
        electron.setDEtaIn(0.004 + 0.01 * 0.004);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelFailNegativeDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.03 + 0.01 * 0.03);
        electron.setDEtaIn(-0.004 - 0.01 * 0.004);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelFailHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.03 + 0.01 * 0.03);
        electron.setDEtaIn(-0.004 + 0.01 * 0.004);
        electron.setHadOverEm(0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelNegativeHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.03 + 0.01 * 0.03);
        electron.setDEtaIn(-0.004 + 0.01 * 0.004);
        electron.setHadOverEm(-0.025 - 0.01 * 0.025);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70Endcap() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(0.03 - 0.01
                * 0.03);
        electron.setDPhiIn(0.02 - 0.01 * 0.02);
        electron.setDEtaIn(0.005 - 0.01 * 0.005);
        electron.setHadOverEm(0.025 - 0.01 * 0.025);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapNegativeValues() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
        electron.setDPhiIn(-0.02 + 0.01 * 0.02);
        electron.setDEtaIn(-0.005 + 0.01 * 0.005);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapFailSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(0.03 + 0.01
                * 0.03);
        electron.setDPhiIn(-0.02 + 0.01 * 0.02);
        electron.setDEtaIn(-0.005 + 0.01 * 0.005);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapNegativeSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 - 0.01
                * 0.03);
        electron.setDPhiIn(-0.02 + 0.01 * 0.02);
        electron.setDEtaIn(-0.005 + 0.01 * 0.005);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapFailDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
        electron.setDPhiIn(0.02 + 0.01 * 0.02);
        electron.setDEtaIn(-0.005 + 0.01 * 0.005);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapFailNegativeDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
        electron.setDPhiIn(-0.02 - 0.01 * 0.02);
        electron.setDEtaIn(-0.005 + 0.01 * 0.005);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
        electron.setDPhiIn(-0.02 + 0.01 * 0.02);
        electron.setDEtaIn(0.005 - 0.01 * 0.005);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        //expect true, dEtaIn cut is not used in the endcap at the moment
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapNegativeDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
        electron.setDPhiIn(-0.02 + 0.01 * 0.02);
        electron.setDEtaIn(-0.005 + 0.01 * 0.005);
        electron.setHadOverEm(-0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        //expect true, dEtaIn cut is not used in the endcap at the moment
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapFailHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
        electron.setDPhiIn(-0.02 + 0.01 * 0.02);
        electron.setDEtaIn(-0.005 + 0.01 * 0.005);
        electron.setHadOverEm(0.025 + 0.01 * 0.025);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapNegativeHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
        electron.setDPhiIn(-0.02 + 0.01 * 0.02);
        electron.setDEtaIn(-0.005 + 0.01 * 0.005);
        electron.setHadOverEm(-0.025 - 0.01 * 0.025);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW95Barrel() {
        Electron electron = Electron(50, 40, 30, 0);
        electron.setSigmaIEtaIEta(0.01 - 0.01
                * 0.01);
        electron.setDPhiIn(0.8 - 0.01 * 0.8);
        electron.setDEtaIn(0.007 - 0.01 * 0.007);
        electron.setHadOverEm(0.15 - 0.01 * 0.15);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelNegativeValues() {
        Electron electron = Electron(50, 40, 30, 0);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.8 + 0.01 * 0.8);
        electron.setDEtaIn(-0.007 + 0.01 * 0.007);
        electron.setHadOverEm(-0.15 + 0.01 * 0.15);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelFailSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.8 + 0.01 * 0.8);
        electron.setDEtaIn(-0.007 + 0.01 * 0.007);
        electron.setHadOverEm(-0.15 + 0.01 * 0.15);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelNegativeSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 - 0.01
                * 0.01);
        electron.setDPhiIn(-0.8 + 0.01 * 0.8);
        electron.setDEtaIn(-0.007 + 0.01 * 0.007);
        electron.setHadOverEm(-0.15 + 0.01 * 0.15);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelFailDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(0.8 + 0.01 * 0.8);
        electron.setDEtaIn(-0.007 + 0.01 * 0.007);
        electron.setHadOverEm(-0.15 + 0.01 * 0.15);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelFailNegativeDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.8 - 0.01 * 0.8);
        electron.setDEtaIn(-0.007 + 0.01 * 0.007);
        electron.setHadOverEm(-0.15 + 0.01 * 0.15);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelFailDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.8 + 0.01 * 0.8);
        electron.setDEtaIn(0.007 + 0.01 * 0.007);
        electron.setHadOverEm(-0.15 + 0.01 * 0.15);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelFailNegativeDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.8 + 0.01 * 0.8);
        electron.setDEtaIn(-0.007 - 0.01 * 0.007);
        electron.setHadOverEm(-0.15 + 0.01 * 0.15);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelFailHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.8 + 0.01 * 0.8);
        electron.setDEtaIn(-0.007 + 0.01 * 0.007);
        electron.setHadOverEm(0.15 + 0.01 * 0.15);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelNegativeHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.01 + 0.01
                * 0.01);
        electron.setDPhiIn(-0.8 + 0.01 * 0.8);
        electron.setDEtaIn(-0.007 + 0.01 * 0.007);
        electron.setHadOverEm(-0.15 - 0.01 * 0.15);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95Endcap() {
        Electron electron = Electron(50, 3, 0, 49);
        electron.setSigmaIEtaIEta(0.03 - 0.01
                * 0.03);
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

    void testVBTFW95EndcapNegativeValues() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
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

    void testVBTFW95EndcapFailSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(0.03 + 0.01
                * 0.03);
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

    void testVBTFW95EndcapNegativeSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 - 0.01
                * 0.03);
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

    void testVBTFW95EndcapFailDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
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

    void testVBTFW95EndcapFailNegativeDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
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

    void testVBTFW95EndcapDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
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

    void testVBTFW95EndcapNegativeDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
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

    void testVBTFW95EndcapFailHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
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

    void testVBTFW95EndcapNegativeHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-0.03 + 0.01
                * 0.03);
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

    void testElectronCharge() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setCharge(2);
        ASSERT_EQUAL(2, electron.charge());
    }

    void testElectronChargeInCopy() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setCharge(2);
        Electron copyElectron(electron);
        ASSERT_EQUAL(2, copyElectron.charge());
    }

    void testElectronInCollection() {
        ElectronCollection coll;
        ElectronPointer electron(new Electron(50, 3, 0, 40));
        ElectronPointer electron2(new Electron(50, 3, 0, 40));
        electron->setCharge(2);
        electron2->setCharge(2);
        coll.push_back(electron);
        coll.push_back(electron2);
        ASSERT_EQUAL(2, coll.front()->charge());
    }

    void testElectronInSTDCollection() {
        std::vector<Electron> coll;
        Electron electron(50, 3, 0, 40);
        Electron electron2(50, 3, 0, 40);
        electron.setCharge(2);
        electron2.setCharge(2);
        coll.push_back(electron);
        coll.push_back(electron2);
        ASSERT_EQUAL(2, coll.front().charge());
    }

    void testElectronSetMass() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setMass(500);
        ASSERT_EQUAL(500, electron.mass());
    }

    void testGSFTrack() {
        TrackPointer track = TrackPointer(new Track(20, 20, 0, 0));
        goodElectron->setGSFTrack(track);
        ASSERT_EQUAL(track, goodElectron->GSFTrack());
    }

};
extern cute::suite make_suite_TestElectron() {
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
    //    s.push_back(CUTE_SMEMFUN(TestElectron, testSwissCrossBarrel));
    //    s.push_back(CUTE_SMEMFUN(TestElectron, testSwissCrossEndcap));
    //    s.push_back(CUTE_SMEMFUN(TestElectron, testSwissCrossNotEcalDriven));
    s.push_back(CUTE_SMEMFUN(TestElectron, testGSFTrack));
    return s;
}
