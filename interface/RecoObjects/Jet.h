/*
 * Jet.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef JET_H_
#define JET_H_
#include "../Taggers/BJetTagger.h"
#include "Particle.h"
#include <vector>
namespace BAT {

class Jet: public Particle {
public:

	Jet();
	Jet(const Particle& particle);
	Jet(float energy, float px, float py, float pz);
	virtual ~Jet();
	bool isGood() const;
	bool isBJetAccordingToBtagAlgorithm(BJetTagger::Algorithm btag) const;
	bool isBJetInData() const;
	JetAlgorithm::value getUsedAlgorithm() const;
	float emf() const;
	float n90Hits() const;
	float fHPD() const;
	float NOD() const;
	float CEF() const;
    float NHF() const;
    float NEF() const;
    float CHF() const;
    float NCH() const;

	void setUsedAlgorithm(JetAlgorithm::value algo);
	void setEMF(float emf);
	void setN90Hits(int n90Hits);
	void setFHPD(float fHPD);
	void setDiscriminatorForBtagType(float discriminator, BJetTagger::Algorithm type);
	void setBtagForData(float btag);
	void setNOD(int nod);
	void setCEF(float cef);
	void setNHF(float nhf);
	void setNEF(float nef);
	void setCHF(float chf);
	void setNCH(float nch);
private:
	JetAlgorithm::value usedAlgorithm;
	float electromagneticFraction;
	int numberOfRecHitsContaining90PercentOfTheJetEnergy;
	float fractionOfEnergyIntheHottestHPDReadout;
	std::vector<float> btag_discriminators;
	float btagInData;
	int numberOfDaughters;
	float chargedEmEnergyFraction, neutralHadronEnergyFraction, neutralEmEnergyFraction;
	float chargedHadronEnergyFraction, chargedMultiplicity;
};
typedef boost::shared_ptr<Jet> JetPointer;
typedef std::vector<JetPointer> JetCollection;
}

#endif /* JET_H_ */
