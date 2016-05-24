/*
 * Jet.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef JET_H_
#define JET_H_
#include "Particle.h"
#include "MCParticle.h"
#include "TTGenInfo.h"
#include "TF1.h"
#include "TEfficiency.h"
#include <vector>
#include <string>
#include <boost/array.hpp>

namespace BAT {

class Jet;
typedef boost::shared_ptr<Jet> JetPointer;
typedef std::vector<JetPointer> JetCollection;

namespace BtagAlgorithm {
enum value {
	CombinedSecondaryVertexV2,	
	NUMBER_OF_BTAGALGORITHMS
};

enum workingPoint {
	LOOSE, MEDIUM, TIGHT
};
const boost::array<std::string, BtagAlgorithm::NUMBER_OF_BTAGALGORITHMS> names = { {
		"CombinedSecondaryVertexV2"//
	}}
	;
}

namespace JetAlgorithm {
enum value {
	Calo_AntiKT_Cone05, CA08PF, PF2PAT, NUMBER_OF_JETALGORITHMS
};
//for nTuple branch names
const boost::array<std::string, JetAlgorithm::NUMBER_OF_JETALGORITHMS> prefixes = { { "goodPatJets", "goodPatJetsCA8PF",
		"goodPatJetsPFlow" } };

const boost::array<std::string, JetAlgorithm::NUMBER_OF_JETALGORITHMS> names = { { "CaloJets", "CA08PFJets",
		"PF2PATJets" } };
}

class Jet: public Particle {
public:

	Jet();
	Jet(const Particle& particle);
	Jet(double energy, double px, double py, double pz);
	virtual ~Jet();
	bool isGood() const;
	bool isBJet(BtagAlgorithm::value type,
			BtagAlgorithm::workingPoint wp) const;
	bool isBJet() const;
	JetAlgorithm::value getUsedAlgorithm() const;
	double getBTagSF( int systematic = 0 ) const;

	void set_matched_generated_jet(const ParticlePointer matchedgeneratedjet);
	const ParticlePointer matched_generated_jet();

	void set_matched_parton(const MCParticlePointer matchedparton);
	const MCParticlePointer matched_parton();

	void set_ttbar_decay_parton( TTPartons::partonType ttbarDecayParton );
	const TTPartons::partonType ttbar_decay_parton();

	void set_unsmeared_jet(const ParticlePointer unsmearedjet);
	const ParticlePointer unsmeared_jet();
	static const ParticlePointer smear_jet(const ParticlePointer jet, const ParticlePointer gen_jet, int jet_smearing_systematic);
//	const ParticlePointer smeared_Jet();

	void set_raw_jet( const ParticlePointer rawJet );
	const ParticlePointer raw_jet();

	double emf() const;
	double n90Hits() const;
	double fHPD() const;
	double NOD() const;
	double CEF() const;
	double NHF() const;
	double NEF() const;
	double CHF() const;
	double NCH() const;
	double btagSSVHE() const;
	double getBJetL7EtCorrection() const;
	double getLightJetL7EtCorrection() const;
	//Quarks: d=1, u=2, s=3, c=4, b=5 (antimatter with flipped sign)
	// gluons = 21
	int partonFlavour() const;
	int hadronFlavour() const;
	double EnergyRaw() const;
	double PxRaw() const;
	double PyRaw() const;
	double PzRaw() const;
	double JEC() const;
	double JECUnc() const;
	double L1OffJEC() const;
	double L2L3ResJEC() const;
	double L2RelJEC() const;
	double L3AbsJEC() const;
	double getBTagDiscriminator(BtagAlgorithm::value type) const;

	void setEnergyRaw(double energy);
	void setPxRaw(double px);
	void setPyRaw(double py);
	void setPzRaw(double pz);
	void setJEC(double JEC);	
	void setJECUnc(double JECUnc);
	void setL1OffJEC(double JEC);
	void setL2L3ResJEC(double JEC);
	void setL2RelJEC(double JEC);
	void setL3AbsJEC(double JEC);
	void setUsedAlgorithm(JetAlgorithm::value algo);
	void setEMF(double emf);
	void setN90Hits(int n90Hits);
	void setFHPD(double fHPD);
	void setDiscriminatorForBtagType(double discriminator, BtagAlgorithm::value type);
	void setIsBJet( bool isBJet );
	void setBTagSF( double sf );
	void setBTagSFUp( double sf );
	void setBTagSFDown( double sf );
	void setNOD(int nod);
	void setCEF(double cef);
	void setNHF(double nhf);
	void setNEF(double nef);
	void setCHF(double chf);
	void setNCH(double nch);
//    void setRho(double rho);
	void setPartonFlavour(int);
	void setHadronFlavour(int);

	bool FirstDataLoosePFJetID() const;
	bool FirstDataLooseCaloJetID() const;

	double getEfficiencyCorrection( int scale_factor_systematic ) const;

private:
	JetAlgorithm::value usedAlgorithm;
	double electromagneticFraction;
	int numberOfRecHitsContaining90PercentOfTheJetEnergy;
	double fractionOfEnergyIntheHottestHPDReadout;

	std::vector<double> btag_discriminators;
	bool isBJet_;
	double btagSF_;
	double btagSF_up_;
	double btagSF_down_;

	//	double btagInData;
	int numberOfDaughters;
	double chargedEmEnergyFraction, neutralHadronEnergyFraction, neutralEmEnergyFraction;
	double chargedHadronEnergyFraction, chargedMultiplicity;
	double energyRaw_, pxRaw_, pyRaw_, pzRaw_, JEC_, JECUncertainty_;
	double l1OffJEC, l2l3ResJEC, l2RelJEC, l3AbsJEC;
	int partonFlavour_, hadronFlavour_;

	double energy_unsmeared;
	double pt_unsmeared;
	double px_unsmeared;
	double py_unsmeared;
	double pz_unsmeared;
	double phi_unsmeared;
	double eta_unsmeared;

	double energy_smeared;
	double pt_smeared;
	double px_smeared;
	double py_smeared;
	double pz_smeared;
	double phi_smeared;
	double eta_smeared;

	ParticlePointer matchedGeneratedJet;
	MCParticlePointer matchedParton;
	ParticlePointer unsmearedJet;
	ParticlePointer smearedJet;
	ParticlePointer rawJet_;

	TTPartons::partonType ttbar_decay_parton_;
};

}

#endif /* JET_H_ */
