#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/RecoObjects/Jet.h"
using namespace BAT;

struct TestJet {
	Jet goodJet;
	Jet badEtaJet;
	Jet badEtJet;
	Jet badEMFJet;
	Jet badn90HitsJet;
	Jet badfHPDJet;

	TestJet() :
				goodJet(290., 200., 200., 0.),
				badEtaJet(2900., 200., 0., 2000.),
				badEtJet(20., 20., 20., 0.),
				badEMFJet(goodJet),
				badn90HitsJet(goodJet),
				badfHPDJet(goodJet),
				goodJetMaximalAbsoluteEta(2.4),
				goodJetMinimalPt(30.0),
				goodJetMinimalElectromagneticFraction(0.01),
				goodJetMinimalNumberOfRecHitsContaining90PercentOfTheJetEnergy(
						1),
				goodJetMaximalFractionOfEnergyIntheHottestHPDReadout(0.98) {
		setUpGoodJet();
		setUpBadEtaJet();
		setUpBadEtJet();
		setUpBadEMFJet();
		setUpBadn90HitsJet();
		setUpBadfHPDJet();
	}
private:

	double goodJetMaximalAbsoluteEta;
	double goodJetMinimalPt;
	double goodJetMinimalElectromagneticFraction;
	double goodJetMinimalNumberOfRecHitsContaining90PercentOfTheJetEnergy;
	double goodJetMaximalFractionOfEnergyIntheHottestHPDReadout;

	void setUpJet(Jet& jet) {
		jet.setEMF(0.1);
		jet.setN90Hits(2.);
		jet.setFHPD(0.5);
	}

	void setUpGoodJet() {
		setUpJet(goodJet);
	}

	void setUpBadEtaJet() {
		setUpJet(badEtaJet);
		assert(badEtaJet.et() > goodJetMinimalPt);
		assert(badEtaJet.eta() > goodJetMaximalAbsoluteEta);
	}

	void setUpBadEtJet() {
		setUpJet(badEtJet);
		assert(fabs(badEtJet.eta()) < goodJetMaximalAbsoluteEta);
	}

	void setUpBadEMFJet() {
		setUpJet(badEMFJet);
		badEMFJet.setEMF(goodJetMinimalElectromagneticFraction - 0.1);
	}

	void setUpBadn90HitsJet() {
		setUpJet(badn90HitsJet);
		badn90HitsJet.setN90Hits(
				goodJetMinimalNumberOfRecHitsContaining90PercentOfTheJetEnergy
						- 1);
	}

	void setUpBadfHPDJet() {
		setUpJet(badfHPDJet);
		badfHPDJet.setFHPD(
				goodJetMaximalFractionOfEnergyIntheHottestHPDReadout + 0.01);
	}
public:
	void testGoodJet() {
		ASSERT(goodJet.isGood());
	}

	void testBadEtaJet() {
		ASSERT(badEtaJet.isGood() == false);
	}

	void testBadEtJet() {
		ASSERT(badEtJet.isGood() == false);
	}

	void testBadEMFJet() {
		ASSERT(badEMFJet.isGood() == false);
	}

	void testBadN90HitsJet() {
		ASSERT(badn90HitsJet.isGood() == false);
	}

	void testBadfHPDJet() {
		ASSERT(badfHPDJet.isGood() == false);
	}

	void testUsedJetAlgorithm() {
		ASSERT_EQUAL(JetAlgorithm::Calo_AntiKT_Cone05,
				goodJet.getUsedAlgorithm());
		goodJet.setUsedAlgorithm(JetAlgorithm::ParticleFlow);
		ASSERT_EQUAL(JetAlgorithm::ParticleFlow, goodJet.getUsedAlgorithm());
	}

	void testJetEMF() {
		goodJet.setEMF(2304.23);
		ASSERT_EQUAL_DELTA(2304.23, goodJet.emf(), 0.01);
	}

	void testJetN90Hits() {
		goodJet.setN90Hits(234.);
		ASSERT_EQUAL_DELTA(234., goodJet.n90Hits(), 0.1);
	}

	void testJetFHPD() {
		goodJet.setFHPD(0.444);
		ASSERT_EQUAL_DELTA(0.444, goodJet.fHPD(), 0.001);
	}

	void testPositiveBtag() {
		goodJet.setDiscriminatorForBtagType(1.7 + 0.1,
				BtagAlgorithm::SimpleSecondaryVertexHighEffBTag);
		ASSERT(goodJet.isBJet(BtagAlgorithm::SimpleSecondaryVertexHighEffBTag));
	}

	void testNegativeBtag() {
		goodJet.setDiscriminatorForBtagType(1.7 - 0.1,
				BtagAlgorithm::SimpleSecondaryVertexHighEffBTag);
		ASSERT(goodJet.isBJet(BtagAlgorithm::SimpleSecondaryVertexHighEffBTag) == false);
	}

};

extern cute::suite make_suite_TestJet() {
	cute::suite s;
	s.push_back(CUTE_SMEMFUN(TestJet, testGoodJet));
	s.push_back(CUTE_SMEMFUN(TestJet, testBadEtaJet));
	s.push_back(CUTE_SMEMFUN(TestJet, testBadEtJet));
	s.push_back(CUTE_SMEMFUN(TestJet, testBadEMFJet));
	s.push_back(CUTE_SMEMFUN(TestJet, testBadN90HitsJet));
	s.push_back(CUTE_SMEMFUN(TestJet, testBadfHPDJet));

	s.push_back(CUTE_SMEMFUN(TestJet, testUsedJetAlgorithm));
	s.push_back(CUTE_SMEMFUN(TestJet, testJetEMF));
	s.push_back(CUTE_SMEMFUN(TestJet, testJetN90Hits));
	s.push_back(CUTE_SMEMFUN(TestJet, testJetFHPD));

	s.push_back(CUTE_SMEMFUN(TestJet, testPositiveBtag));
	s.push_back(CUTE_SMEMFUN(TestJet, testNegativeBtag));
	return s;
}
