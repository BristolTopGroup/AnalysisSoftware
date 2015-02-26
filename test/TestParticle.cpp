/*
 * TestParticle.cpp
 *
 *  Created on: 26 Feb 2015
 *      Author: kreczko
 */

#include <boost/test/unit_test.hpp>
#include "../interface/RecoObjects/Particle.h"

using namespace BAT;

struct TestParticleSetup {
	TestParticleSetup() :
					particle1(20., 10., 0., 0.),
					particle2(20., -10., 0., 0.),
					zparticle1(100., 79., -13., -5.),
					zparticle2(100., 99., 13., 5.) {
		particle1.setD0(180.);
		particle1.setCharge(0.5);
	}

	~TestParticleSetup() {

	}

	Particle particle1, particle2, zparticle1, zparticle2;
};

BOOST_AUTO_TEST_SUITE (TestParticle)
BOOST_FIXTURE_TEST_CASE(test_get_energy, TestParticleSetup) {
	BOOST_CHECK_EQUAL(particle1.energy(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_get_px, TestParticleSetup) {
	BOOST_CHECK_EQUAL(particle1.px(), 10);
}

BOOST_FIXTURE_TEST_CASE(test_get_py, TestParticleSetup) {
	BOOST_CHECK_EQUAL(particle1.py(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_get_pz, TestParticleSetup) {
	BOOST_CHECK_EQUAL(particle1.pz(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_get_d0, TestParticleSetup) {
	BOOST_CHECK_EQUAL(particle1.d0(), 180.);
}

BOOST_FIXTURE_TEST_CASE(test_get_mass, TestParticleSetup) {
	BOOST_CHECK_CLOSE(particle1.mass(), 17.32, 0.01);
}

BOOST_FIXTURE_TEST_CASE(test_get_charge, TestParticleSetup) {
	BOOST_CHECK_EQUAL(particle1.charge(), 0.5);
}

BOOST_FIXTURE_TEST_CASE(test_operator_plus, TestParticleSetup) {
	Particle p = zparticle1 + zparticle2;
	BOOST_CHECK_EQUAL(p.px(), zparticle1.px() + zparticle2.px());
	BOOST_CHECK_EQUAL(p.py(), zparticle1.py() + zparticle2.py());
	BOOST_CHECK_EQUAL(p.pz(), zparticle1.pz() + zparticle2.pz());
	double e_squared = pow(zparticle1.energy() + zparticle2.energy(), 2);
	double p_squared = pow(zparticle1.px() + zparticle2.px(), 2);
	double mass = sqrt(e_squared - p_squared);
	BOOST_CHECK_EQUAL(p.mass(), mass);
}

BOOST_FIXTURE_TEST_CASE(test_operator_equals, TestParticleSetup) {
	Particle p = particle1;
	BOOST_CHECK_EQUAL(p.px(), particle1.px() );
	BOOST_CHECK_EQUAL(p.py(), particle1.py());
	BOOST_CHECK_EQUAL(p.pz(), particle1.pz());
	BOOST_CHECK_EQUAL(p.energy(), particle1.energy());

	BOOST_CHECK_EQUAL(p.mass(), particle1.mass());
	BOOST_CHECK_EQUAL(p.charge(), particle1.charge());
	BOOST_CHECK_EQUAL(p.d0(), particle1.d0());
}

BOOST_FIXTURE_TEST_CASE(test_operator_plus_equals, TestParticleSetup) {
	Particle p = zparticle1;
	p +=  zparticle2;
	BOOST_CHECK_EQUAL(p.px(), zparticle1.px() + zparticle2.px());
	BOOST_CHECK_EQUAL(p.py(), zparticle1.py() + zparticle2.py());
	BOOST_CHECK_EQUAL(p.pz(), zparticle1.pz() + zparticle2.pz());
	double e_squared = pow(zparticle1.energy() + zparticle2.energy(), 2);
	double p_squared = pow(zparticle1.px() + zparticle2.px(), 2);
	double mass = sqrt(e_squared - p_squared);
	BOOST_CHECK_EQUAL(p.mass(), mass);
}


BOOST_AUTO_TEST_SUITE_END()
