#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/Filter.h"
#include "../interface/Taggers/BJetTagger.h"
#include "../interface/RecoObjects/Jet.h"
#include "../interface/RecoObjects/Electron.h"
#include "../interface/RecoObjects/MET.h"
#include "../interface/RecoObjects/PrimaryVertex.h"
#include <boost/scoped_ptr.hpp>

using namespace BAT;

struct TestFilter {
private:
    boost::scoped_ptr<Filter> filter;
public:
    TestFilter() :
        filter(Filter::makeTopPairEPlusJetsFilter()) {

    }

    void testSetGoodVertexMinimalNumberOfDegreesOfFreedom() {
        ASSERT_EQUAL(4, PrimaryVertex::goodVertexMinimalNumberOfDegreesOfFreedom);
        filter->setGoodVertexMinimalNumberOfDegreesOfFreedom(5);
        ASSERT_EQUAL(5, PrimaryVertex::goodVertexMinimalNumberOfDegreesOfFreedom);
    }

    void testSetGoodVertexMaximalAbsoluteZPosition() {
        ASSERT_EQUAL(24, PrimaryVertex::goodVertexMaximalAbsoluteZPosition);
        filter->setGoodVertexMaximalAbsoluteZPosition(20);
        ASSERT_EQUAL(20, PrimaryVertex::goodVertexMaximalAbsoluteZPosition);
    }


    void testSetGoodVertexMaximalRho() {
        ASSERT_EQUAL(2.0, PrimaryVertex::goodVertexMaximalAbsoluteRho);
        filter->setGoodVertexMaximalRho(2.5);
        ASSERT_EQUAL(2.5, PrimaryVertex::goodVertexMaximalAbsoluteRho);
    }

    void testSetGoodJetMinimalPt() {
        ASSERT_EQUAL_DELTA(30.0, Jet::goodJetMinimalPt, 0.01);
        filter->setGoodJetMinimalPt(50.5);
        ASSERT_EQUAL_DELTA(50.5, Jet::goodJetMinimalPt, 0.01);
    }

    void testSetGoodJetMaximalAbsoluteEta() {
        ASSERT_EQUAL_DELTA(2.4, Jet::goodJetMaximalAbsoluteEta, 0.01);
        filter->setGoodJetMaximalAbsoluteEta(1.4);
        ASSERT_EQUAL_DELTA(1.4, Jet::goodJetMaximalAbsoluteEta, 0.01);
    }

    void testSetGoodJetMinimalElectromagneticFraction() {
        ASSERT_EQUAL_DELTA(0.01, Jet::goodJetMinimalElectromagneticFraction, 0.001);
        filter->setGoodJetMinimalElectromagneticFraction(0.9);
        ASSERT_EQUAL_DELTA(0.9, Jet::goodJetMinimalElectromagneticFraction, 0.01);
    }

    void testSetGoodJetMinimalNumberOfRecHitsContaining90PercentOfTheJetEnergy() {
        ASSERT_EQUAL_DELTA(1., Jet::goodJetMinimalNumberOfRecHitsContaining90PercentOfTheJetEnergy, 0.001);
        filter->setGoodJetMinimalNumberOfRecHitsContaining90PercentOfTheJetEnergy(0.9);
        ASSERT_EQUAL_DELTA(0.9, Jet::goodJetMinimalNumberOfRecHitsContaining90PercentOfTheJetEnergy, 0.01);
    }

    void testSetGoodJetMaximalFractionOfEnergyIntheHottestHPDReadout() {
        ASSERT_EQUAL_DELTA(0.98, Jet::goodJetMaximalFractionOfEnergyIntheHottestHPDReadout, 0.001);
        filter->setGoodJetMaximalFractionOfEnergyIntheHottestHPDReadout(1.9);
        ASSERT_EQUAL_DELTA(1.9, Jet::goodJetMaximalFractionOfEnergyIntheHottestHPDReadout, 0.01);
    }

    void testSetGoodElectronMinimalEt() {
        ASSERT_EQUAL_DELTA(30.0, Electron::goodElectronMinimalEt, 0.01);
        filter->setGoodElectronMinimalEt(50.5);
        ASSERT_EQUAL_DELTA(50.5, Electron::goodElectronMinimalEt, 0.01);
    }

    void testSetGoodElectronMaximalAbsoluteEta() {
        ASSERT_EQUAL_DELTA(2.5, Electron::goodElectronMaximalAbsoluteEta, 0.01);
        filter->setGoodElectronMaximalAbsoluteEta(1.442);
        ASSERT_EQUAL_DELTA(1.442, Electron::goodElectronMaximalAbsoluteEta, 0.01);
    }

    void testSetGoodElectronMaximalRelativeIsolation() {
        ASSERT_EQUAL_DELTA(0.1, Electron::isolatedElectronMaximalRelativeIsolation, 0.01);
        filter->setIsolatedElectronMaximalRelativeIsolation(0.05);
        ASSERT_EQUAL_DELTA(0.05, Electron::isolatedElectronMaximalRelativeIsolation, 0.005);
    }

    void testElectronConversionVeto() {
        ASSERT_EQUAL(0, Electron::MaximalNumberOfMissingInnerLayerHitsBeforeCalledConversion);
        filter->setElectronMaximalNumberOfMissingInnerLayerHitsBeforeCalledConversion(2);
        ASSERT_EQUAL(2, Electron::MaximalNumberOfMissingInnerLayerHitsBeforeCalledConversion);
    }

    void testSetLooseElectronMinimalEt() {
        ASSERT_EQUAL_DELTA(20, Electron::looseElectronMinimalEt, 0.1);
        filter->setLooseElectronMinimalEt(55);
        ASSERT_EQUAL_DELTA(55, Electron::looseElectronMinimalEt, 0.1);
    }

    void testSetLooseElectronMaximalRelativeIsolation() {
        ASSERT_EQUAL_DELTA(1., Electron::looseIsolatedElectronMaximalRelativeIsolation, 0.01);
        filter->setLooseIsolatedElectronMaximalRelativeIsolation(1.442);
        ASSERT_EQUAL_DELTA(1.442, Electron::looseIsolatedElectronMaximalRelativeIsolation, 0.01);
    }

    void testSetLooseElectronMaximalAbsoluteEta() {
        ASSERT_EQUAL_DELTA(2.5, Electron::looseElectronMaximalAbsoluteEta, 0.01);
        filter->setLooseElectronMaximalAbsoluteEta(1.442);
        ASSERT_EQUAL_DELTA(1.442, Electron::looseElectronMaximalAbsoluteEta, 0.01);
    }
    void testSetGoodElectronMaximalDistanceFromInteractionPoint() {
        ASSERT_EQUAL_DELTA(0.02, Electron::goodElectronMaximalDistanceFromInteractionPoint, 0.1);
        filter->setGoodElectronMaximalDistanceFromInteractionPoint(100);
        ASSERT_EQUAL_DELTA(100, Electron::goodElectronMaximalDistanceFromInteractionPoint, 0.1);
    }

    void testSetGoodMETMinimalEt() {
        ASSERT_EQUAL_DELTA(20, MET::goodMETMinimalEt, 0.1);
        filter->setGoodMETMinimalEt(30);
        ASSERT_EQUAL_DELTA(30, MET::goodMETMinimalEt, 0.1);
    }

    void testSetGoodMuonMinimalPt() {
        ASSERT_EQUAL_DELTA(10., Muon::goodMuonMinimalPt, 0.001);
        filter->setGoodMuonMinimalPt(20);
        ASSERT_EQUAL_DELTA(20., Muon::goodMuonMinimalPt, 0.001);
    }

    void testSetGoodMuonMaximalAbsoluteEta() {
        ASSERT_EQUAL_DELTA(2.5, Muon::goodMuonMaximalAbsoluteEta, 0.001);
        filter->setGoodMuonMaximalAbsoluteEta(20);
        ASSERT_EQUAL_DELTA(20., Muon::goodMuonMaximalAbsoluteEta, 0.001);
    }

    void testSetIsolatedMuonMinimalRelativeIsolation() {
        ASSERT_EQUAL_DELTA(0.2, Muon::isolatedMuonMaximalRelativeIsolation, 0.001);
        filter->setIsolatedMuonMinimalRelativeIsolation(11);
        ASSERT_EQUAL_DELTA(11., Muon::isolatedMuonMaximalRelativeIsolation, 0.001);
    }

};
extern cute::suite make_suite_TestFilter() {
    cute::suite s;
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodVertexMinimalNumberOfDegreesOfFreedom));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodVertexMaximalAbsoluteZPosition));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodVertexMaximalRho));

    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodJetMinimalPt));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodJetMaximalAbsoluteEta));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodJetMinimalElectromagneticFraction));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodElectronMinimalEt));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodElectronMaximalAbsoluteEta));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodElectronMaximalRelativeIsolation));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodElectronMaximalDistanceFromInteractionPoint));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodMETMinimalEt));
    s.push_back(CUTE_SMEMFUN(TestFilter, testElectronConversionVeto));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetLooseElectronMinimalEt));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetLooseElectronMaximalRelativeIsolation));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetLooseElectronMaximalAbsoluteEta));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodJetMinimalNumberOfRecHitsContaining90PercentOfTheJetEnergy));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodJetMaximalFractionOfEnergyIntheHottestHPDReadout));

    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodMuonMinimalPt));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetGoodMuonMaximalAbsoluteEta));
    s.push_back(CUTE_SMEMFUN(TestFilter, testSetIsolatedMuonMinimalRelativeIsolation));

    return s;
}
