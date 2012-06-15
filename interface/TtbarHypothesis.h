/*
 * TTbarHypothesis.h
 *
 *  Created on: Dec 4, 2010
 *      Author: lkreczko
 */

#ifndef TTBARHYPOTHESIS_H_
#define TTBARHYPOTHESIS_H_
#include "RecoObjects/Electron.h"
#include "RecoObjects/Jet.h"
#include "RecoObjects/MET.h"
#include "RecoObjects/Particle.h"

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace BAT {
namespace Decay{
enum value{
	unknown,
	electronPlusJets,
	LeptonPlusJets,
	fullyHadronic,
	fullyLeptonic
};
}

class TtbarHypothesis {
public:
	TtbarHypothesis();

	TtbarHypothesis( const LeptonPointer& elec, const ParticlePointer& neut,
		const JetPointer& lepBJet,   const JetPointer& hadBJet,
		const JetPointer& hadWJet1,  const JetPointer& hadWJet2 );

	virtual ~TtbarHypothesis();
	double totalChi2, leptonicChi2, hadronicChi2, globalChi2, discriminator;
	ParticlePointer hadronicTop, leptonicTop, leptonicW, hadronicW, resonance, neutrinoFromW;
	JetPointer leptonicBjet, hadronicBJet, jet1FromW, jet2FromW;
	LeptonPointer leptonFromW;
	METPointer met;
	Decay::value decayChannel;

	double M3() const;
	static double M3(const JetCollection jets);

	double sumPt() const;
	double PtTtbarSystem() const;

	bool isValid() const;
	bool isPhysical() const;

	void combineReconstructedObjects();

	void throwDetailedException() const;

//	bool operator==(const TtbarHypothesis& hyp) const;
//	bool operator<(const TtbarHypothesis& hyp) const;

};

typedef boost::shared_ptr<TtbarHypothesis> TtbarHypothesisPointer;
typedef std::vector<TtbarHypothesisPointer> TtbarHypothesisCollection;

struct compare_totalChi2 {
    bool operator ()(TtbarHypothesisPointer lhs, TtbarHypothesisPointer rhs) {
        return lhs->totalChi2 < rhs->totalChi2;
    }

    bool operator ()(TtbarHypothesis lhs, TtbarHypothesis rhs) {
        return lhs.totalChi2 < rhs.totalChi2;
    }
};

struct SmallerDiscriminatorIsBetter {
    bool operator ()(TtbarHypothesisPointer lhs, TtbarHypothesisPointer rhs) {
        return lhs->discriminator < rhs->discriminator;
    }

    bool operator ()(TtbarHypothesis lhs, TtbarHypothesis rhs) {
        return lhs.discriminator < rhs.discriminator;
    }
};
} // namespace BAT

#endif /* TTBARHYPOTHESIS_H_ */
