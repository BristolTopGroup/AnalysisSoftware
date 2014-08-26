#ifndef TESTPARTICLE_h
#define TESTPARTICLE_h
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/RecoObjects/Particle.h"

using namespace BAT;

struct TestParticle {
private:
    ParticlePointer particle1, particle2, particle3, particleInBarrelRegion, particleInCrack, particleInEndcap;
    ParticlePointer zParticle1, zParticle2;
    FourVector combinedVector;

public:
    TestParticle();

    void testAsignOperator();
    void testPlusOperatorCorrectFourvector();
    void testPlusOperatorCorrectMass();
    void testDeltaEta();
    void testDeltaPhi();
    void testDeltaR();
    void testIsWithinDR();
    void testIsNotWithinDR();
    void testStandardConstructor();
    void testFourvectorConstructor();
    void testParticleConstructor();
    void testSetMass();
    void testDistanceFromInteractionPointAliasD0();
    void testGetMassFromEnergyAndMomentumIfEquals0();
    void testParticleIsInBarrelRegion();
    void testParticleInEndcap();
    void testParticleInCrack();
    void testInvarianMassOfTwoParticles();
    void testInvarianMassOfOneParticles();
    void testRelativePtParallelParticles();
    void testRelativePtOrthogonalParticles();
    void testGetClosest();
    void testGetClosest2();
    void testCharge();
    void testChargeInCollection();
    void testAngle();
};

extern cute::suite make_suite_TestParticle();
#endif
