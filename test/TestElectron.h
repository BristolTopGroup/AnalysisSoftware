#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/RecoObjects/Electron.h"
#include "../interface/VBTF_ElectronID.h"
#include "TestObjectFactory.h"

using namespace BAT;

struct TestElectron {
private:
    double isolatedElectronMaximalRelativeIsolation;
    double goodElectronMaximalAbsoluteEta;
    double goodElectronMinimalEt;
    double goodElectronMaximalDistanceFromInteractionPoint;
    double MaximalNumberOfMissingInnerLayerHitsBeforeCalledConversion;
    double looseElectronMaximalAbsoluteEta;
    double looseElectronMinimalEt;
    double looseIsolatedElectronMaximalRelativeIsolation;

    float invalidSwissCross;
    ElectronPointer isolatedElectron;
    ElectronPointer goodElectron;
    ElectronPointer badEtElectron;
    ElectronPointer badEtaElectron;
    ElectronPointer badInCrackElectron;
    ElectronPointer badD0Electron;
    ElectronPointer badElectronFromConversion;
    ElectronPointer looseElectron;
    ElectronPointer badLooseElectronNoID;
    ElectronPointer badElectronNoID;

public:
    TestElectron() :
        isolatedElectronMaximalRelativeIsolation(0.1),
        goodElectronMaximalAbsoluteEta(2.1),
        goodElectronMinimalEt(20.),
        goodElectronMaximalDistanceFromInteractionPoint(0.02),
        MaximalNumberOfMissingInnerLayerHitsBeforeCalledConversion(0),
        looseElectronMaximalAbsoluteEta(2.5),
        looseElectronMinimalEt(25),
        looseIsolatedElectronMaximalRelativeIsolation(1.0),
        invalidSwissCross(2),
        isolatedElectron(TestObjectFactory::goodIsolatedElectron()),
        goodElectron(TestObjectFactory::goodCaloElectron()),
        badEtElectron(new Electron(20., 10., 0., 5.)),
        badEtaElectron(new Electron(400., 50., 50., 380)),
        badInCrackElectron(new Electron(400., 50., 50., 0)),
        badD0Electron(new Electron(40., 5., 5., 0.)),
        badElectronFromConversion(new Electron(400., 50., 50., 380)),
        looseElectron(new Electron(40., 20., 20., 0.)),
        badLooseElectronNoID(new Electron(30., 20., 5., 2.)),
        badElectronNoID(new Electron(30., 20., 5., 2.)) {
//        setElectronConditions();
        setBadEtElectron();
        setBadEtaElectron();
        setBadD0Electron();
        setBadInCrackElectron();
        setElectronFromConversion();
        setLooseElectron();
        setBadLooseElectronNoID();
        setBadElectronNoID();
    }

private:
//    void setElectronConditions() {
//        isolatedElectronMaximalRelativeIsolation = 0.1;
//        goodElectronMaximalAbsoluteEta = 2.1;
//        goodElectronMinimalEt = 20.;
//        goodElectronMaximalDistanceFromInteractionPoint = 0.02;
//        MaximalNumberOfMissingInnerLayerHitsBeforeCalledConversion = 0;
//        looseElectronMaximalAbsoluteEta = 2.5;
//        looseElectronMinimalEt = 25;
//        looseIsolatedElectronMaximalRelativeIsolation = 1.0;
//    }

    void setBadEtElectron() {
        badEtElectron->setD0(0.01);
        badEtElectron->setSuperClusterEta(1);
        assert(fabs(badEtElectron->eta()) < goodElectronMaximalAbsoluteEta);
        assert(fabs(badEtElectron->d0()) < goodElectronMaximalDistanceFromInteractionPoint);
        //and fails the selected
        assert(badEtElectron->et() < goodElectronMinimalEt);
    }

    void setBadEtaElectron() {
        badEtaElectron->setSuperClusterEta(2.6);
        badEtaElectron->setD0(0.001);
        //make sure it passes all other requirements
        assert(badEtaElectron->et() > goodElectronMinimalEt);
        assert(fabs(badEtaElectron->d0()) < goodElectronMaximalDistanceFromInteractionPoint);
        //and fails the selected
        assert(fabs(badEtaElectron->superClusterEta()) > goodElectronMaximalAbsoluteEta);
    }

    void setBadD0Electron() {
        badD0Electron->setD0(300.);
        //make sure it passes all other requirements
        assert(badD0Electron->et() > goodElectronMinimalEt);
        assert(fabs(badD0Electron->eta()) < goodElectronMaximalAbsoluteEta);
        //and fails the selected
        assert(fabs(badD0Electron->d0()) > goodElectronMaximalDistanceFromInteractionPoint);
    }

    void setBadInCrackElectron() {
        badInCrackElectron->setSuperClusterEta(1.5);
        badInCrackElectron->setD0_wrtBeamSpot(0.0001);
        //make sure it passes all other requirements
        assert(badInCrackElectron->et() > goodElectronMinimalEt);
        assert(fabs(badInCrackElectron->d0_wrtBeamSpot()) < goodElectronMaximalDistanceFromInteractionPoint);
        //and fails the selected
        assert(fabs(badInCrackElectron->eta()) < goodElectronMaximalAbsoluteEta);
        assert(badInCrackElectron->isInCrack());
    }

    void setElectronFromConversion() {
        //        badElectronFromConversion = ElectronPointer(new Electron(400., 50., 50., 380));
        badElectronFromConversion->setNumberOfMissingInnerLayerHits(1);
    }

    void setLooseElectron() {
        //        looseElectron = Electron(40., 20., 20., 0.);
        //        looseElectron->setRobustLooseID(true);
        looseElectron->setSigmaIEtaIEta(VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion - 0.1
                * VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion);
        looseElectron->setDPhiIn(VBTF_W95::MaximalDPhiIn_BarrelRegion - 0.1 * VBTF_W95::MaximalDPhiIn_BarrelRegion);
        looseElectron->setDEtaIn(VBTF_W95::MaximalDEtaIn_BarrelRegion - 0.1 * VBTF_W95::MaximalDEtaIn_BarrelRegion);
        looseElectron->setHadOverEm(VBTF_W95::MaximalHadOverEm_BarrelRegion - 0.1
                * VBTF_W95::MaximalHadOverEm_BarrelRegion);
        looseElectron->setSuperClusterEta(1);
        looseElectron->setEcalIsolation(0);
        looseElectron->setTrackerIsolation(0);
        looseElectron->setHcalIsolation(0);
    }

    void setBadLooseElectronNoID() {
        //        badLooseElectronNoID = Electron(30., 20., 5., 2.);
        badLooseElectronNoID->setHcalIsolation(0.5);
        badLooseElectronNoID->setEcalIsolation(0.3);
        badLooseElectronNoID->setTrackerIsolation(0.4);
        badLooseElectronNoID->setSigmaIEtaIEta(VBTF_W95::MaximalDEtaIn_BarrelRegion + 2);
    }

    void setBadElectronNoID() {
        //        badElectronNoID = Electron(30., 20., 5., 2.);
        badElectronNoID->setHcalIsolation(0.5);
        badElectronNoID->setEcalIsolation(0.3);
        badElectronNoID->setTrackerIsolation(0.4);
        badElectronNoID->setSigmaIEtaIEta(VBTF_W70::MaximalDEtaIn_BarrelRegion + 2);
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
        ASSERT(badElectronFromConversion->isFromConversion());
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
        electron.setSigmaIEtaIEta(VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion - 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(VBTF_W70::MaximalDPhiIn_BarrelRegion - 0.01 * VBTF_W70::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(VBTF_W70::MaximalDEtaIn_BarrelRegion - 0.01 * VBTF_W70::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(VBTF_W70::MaximalHadOverEm_BarrelRegion - 0.01 * VBTF_W70::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelNegativeValues() {
        Electron electron = Electron(50, 40, 30, 0);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W70::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelFailSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W70::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelNegativeSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion - 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W70::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelFailDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(VBTF_W70::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W70::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelFailNegativeDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_BarrelRegion - 0.01 * VBTF_W70::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W70::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelFailDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(VBTF_W70::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W70::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelFailNegativeDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_BarrelRegion - 0.01 * VBTF_W70::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W70::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelFailHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(VBTF_W70::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W70::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70BarrelNegativeHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W70::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_BarrelRegion - 0.01 * VBTF_W70::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70Endcap() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion - 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(VBTF_W70::MaximalDPhiIn_EndcapRegion - 0.01 * VBTF_W70::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(VBTF_W70::MaximalDEtaIn_EndcapRegion - 0.01 * VBTF_W70::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(VBTF_W70::MaximalHadOverEm_EndcapRegion - 0.01 * VBTF_W70::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapNegativeValues() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W70::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapFailSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W70::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapNegativeSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion - 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W70::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapFailDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(VBTF_W70::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W70::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapFailNegativeDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_EndcapRegion - 0.01 * VBTF_W70::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W70::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(VBTF_W70::MaximalDEtaIn_EndcapRegion - 0.01 * VBTF_W70::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W70::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        //expect true, dEtaIn cut is not used in the endcap at the moment
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapNegativeDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W70::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        //expect true, dEtaIn cut is not used in the endcap at the moment
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapFailHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(VBTF_W70::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W70::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(false, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW70EndcapNegativeHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W70::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W70::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W70::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W70::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W70::MaximalHadOverEm_EndcapRegion - 0.01 * VBTF_W70::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(true, electron.VBTF_W70_ElectronID());
    }

    void testVBTFW95Barrel() {
        Electron electron = Electron(50, 40, 30, 0);
        electron.setSigmaIEtaIEta(VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion - 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(VBTF_W95::MaximalDPhiIn_BarrelRegion - 0.01 * VBTF_W95::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(VBTF_W95::MaximalDEtaIn_BarrelRegion - 0.01 * VBTF_W95::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(VBTF_W95::MaximalHadOverEm_BarrelRegion - 0.01 * VBTF_W95::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelNegativeValues() {
        Electron electron = Electron(50, 40, 30, 0);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W95::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelFailSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W95::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelNegativeSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion - 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W95::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelFailDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(VBTF_W95::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W95::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelFailNegativeDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_BarrelRegion - 0.01 * VBTF_W95::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W95::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelFailDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(VBTF_W95::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W95::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelFailNegativeDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_BarrelRegion - 0.01 * VBTF_W95::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W95::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelFailHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(VBTF_W95::MaximalHadOverEm_BarrelRegion + 0.01 * VBTF_W95::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95BarrelNegativeHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_BarrelRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDPhiIn_BarrelRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_BarrelRegion + 0.01 * VBTF_W95::MaximalDEtaIn_BarrelRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_BarrelRegion - 0.01 * VBTF_W95::MaximalHadOverEm_BarrelRegion);
        electron.setSuperClusterEta(0);
        assert(electron.isInBarrelRegion());
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95Endcap() {
        Electron electron = Electron(50, 3, 0, 49);
        electron.setSigmaIEtaIEta(VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion - 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(VBTF_W95::MaximalDPhiIn_EndcapRegion - 0.01 * VBTF_W95::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(VBTF_W95::MaximalDEtaIn_EndcapRegion - 0.01 * VBTF_W95::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(VBTF_W95::MaximalHadOverEm_EndcapRegion - 0.01 * VBTF_W95::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.4);

        assert(electron.isInEndCapRegion());
        assert(electron.sigmaIEtaIEta() < VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        assert(fabs(electron.dPhiIn()) < VBTF_W95::MaximalDPhiIn_EndcapRegion);
        assert(fabs(electron.dEtaIn()) < VBTF_W95::MaximalDEtaIn_EndcapRegion);
        assert(electron.HadOverEm() < VBTF_W95::MaximalHadOverEm_EndcapRegion);
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95EndcapNegativeValues() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W95::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.4);

        assert(electron.sigmaIEtaIEta() < VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        assert(fabs(electron.dPhiIn()) < VBTF_W95::MaximalDPhiIn_EndcapRegion);
        assert(fabs(electron.dEtaIn()) < VBTF_W95::MaximalDEtaIn_EndcapRegion);
        assert(electron.HadOverEm() < VBTF_W95::MaximalHadOverEm_EndcapRegion);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95EndcapFailSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W95::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.4);
        assert(electron.isInEndCapRegion());
        assert(electron.sigmaIEtaIEta() > VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        assert(fabs(electron.dPhiIn()) < VBTF_W95::MaximalDPhiIn_EndcapRegion);
        assert(fabs(electron.dEtaIn()) < VBTF_W95::MaximalDEtaIn_EndcapRegion);
        assert(electron.HadOverEm() < VBTF_W95::MaximalHadOverEm_EndcapRegion);
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95EndcapNegativeSigmaIEtaIEta() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion - 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W95::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        assert(electron.sigmaIEtaIEta() < VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        assert(fabs(electron.dPhiIn()) < VBTF_W95::MaximalDPhiIn_EndcapRegion);
        assert(fabs(electron.dEtaIn()) < VBTF_W95::MaximalDEtaIn_EndcapRegion);
        assert(electron.HadOverEm() < VBTF_W95::MaximalHadOverEm_EndcapRegion);
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95EndcapFailDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(VBTF_W95::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W95::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.sigmaIEtaIEta() < VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        assert(fabs(electron.dPhiIn()) > VBTF_W95::MaximalDPhiIn_EndcapRegion);
        assert(fabs(electron.dEtaIn()) < VBTF_W95::MaximalDEtaIn_EndcapRegion);
        assert(electron.HadOverEm() < VBTF_W95::MaximalHadOverEm_EndcapRegion);
        assert(electron.isInEndCapRegion());
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95EndcapFailNegativeDPhiIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_EndcapRegion - 0.01 * VBTF_W95::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W95::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        assert(electron.sigmaIEtaIEta() < VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        assert(fabs(electron.dPhiIn()) > VBTF_W95::MaximalDPhiIn_EndcapRegion);
        assert(fabs(electron.dEtaIn()) < VBTF_W95::MaximalDEtaIn_EndcapRegion);
        assert(electron.HadOverEm() < VBTF_W95::MaximalHadOverEm_EndcapRegion);
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95EndcapDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(VBTF_W95::MaximalDEtaIn_EndcapRegion - 0.01 * VBTF_W95::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W95::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        assert(electron.sigmaIEtaIEta() < VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        assert(fabs(electron.dPhiIn()) < VBTF_W95::MaximalDPhiIn_EndcapRegion);
        assert(fabs(electron.dEtaIn()) < VBTF_W95::MaximalDEtaIn_EndcapRegion);
        assert(electron.HadOverEm() < VBTF_W95::MaximalHadOverEm_EndcapRegion);
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95EndcapNegativeDEtaIn() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W95::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        assert(electron.sigmaIEtaIEta() < VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        assert(fabs(electron.dPhiIn()) < VBTF_W95::MaximalDPhiIn_EndcapRegion);
        assert(fabs(electron.dEtaIn()) < VBTF_W95::MaximalDEtaIn_EndcapRegion);
        assert(electron.HadOverEm() < VBTF_W95::MaximalHadOverEm_EndcapRegion);
        //expect true, dEtaIn cut is not used in the endcap at the moment
        ASSERT_EQUAL(true, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95EndcapFailHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(VBTF_W95::MaximalHadOverEm_EndcapRegion + 0.01 * VBTF_W95::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        assert(electron.sigmaIEtaIEta() < VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        assert(fabs(electron.dPhiIn()) < VBTF_W95::MaximalDPhiIn_EndcapRegion);
        assert(fabs(electron.dEtaIn()) < VBTF_W95::MaximalDEtaIn_EndcapRegion);
        assert(electron.HadOverEm() > VBTF_W95::MaximalHadOverEm_EndcapRegion);
        ASSERT_EQUAL(false, electron.VBTF_W95_ElectronID());
    }

    void testVBTFW95EndcapNegativeHadOverEm() {
        Electron electron = Electron(50, 3, 0, 40);
        electron.setSigmaIEtaIEta(-VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion + 0.01
                * VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        electron.setDPhiIn(-VBTF_W95::MaximalDPhiIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDPhiIn_EndcapRegion);
        electron.setDEtaIn(-VBTF_W95::MaximalDEtaIn_EndcapRegion + 0.01 * VBTF_W95::MaximalDEtaIn_EndcapRegion);
        electron.setHadOverEm(-VBTF_W95::MaximalHadOverEm_EndcapRegion - 0.01 * VBTF_W95::MaximalHadOverEm_EndcapRegion);
        electron.setSuperClusterEta(2.6);
        assert(electron.isInEndCapRegion());
        assert(electron.sigmaIEtaIEta() < VBTF_W95::MaximalSigmaIEtaIEta_EndcapRegion);
        assert(fabs(electron.dPhiIn()) < VBTF_W95::MaximalDPhiIn_EndcapRegion);
        assert(fabs(electron.dEtaIn()) < VBTF_W95::MaximalDEtaIn_EndcapRegion);
        assert(electron.HadOverEm() < VBTF_W95::MaximalHadOverEm_EndcapRegion);
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

    //    void testSwissCrossBarrel() {
    //        goodElectron->setSuperClusterEta(1);
    //        goodElectron->setSwissCross(invalidSwissCross);
    //        goodElectron->setIsEcalDriven(true);
    //        ASSERT_EQUAL(true, goodElectron->isEcalSpike());
    //        ASSERT_EQUAL(false, goodElectron->isGood());
    //    }
    //
    //    void testSwissCrossEndcap() {
    //        goodElectron->setSuperClusterEta(2);
    //        goodElectron->setSwissCross(invalidSwissCross);
    //        goodElectron->setIsEcalDriven(true);
    //        ASSERT_EQUAL(false, goodElectron->isEcalSpike());
    //        ASSERT_EQUAL(true, goodElectron->isGood());
    //    }
    //
    //    void testSwissCrossNotEcalDriven() {
    //        goodElectron->setSuperClusterEta(2);
    //        goodElectron->setSwissCross(invalidSwissCross);
    //        goodElectron->setIsEcalDriven(false);
    //        ASSERT_EQUAL(false, goodElectron->isEcalSpike());
    //        ASSERT_EQUAL(true, goodElectron->isGood());
    //    }

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
