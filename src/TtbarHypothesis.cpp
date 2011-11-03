/*
 * TtbarHypothesis.cpp
 *
 *  Created on: Dec 4, 2010
 *      Author: lkreczko
 */

#include "../interface/TtbarHypothesis.h"
#include "../interface/RecontructionModules/ReconstructionException.h"

namespace BAT {

TtbarHypothesis::TtbarHypothesis() :
	totalChi2(99999.), 
	leptonicChi2(99999.), 
	hadronicChi2(99999.), 
	globalChi2(99999.), 
	discriminator(999999),
	hadronicTop(), 
	leptonicTop(),
	leptonicW(), 
	hadronicW(), 
	resonance(), 
	neutrinoFromW(), 
	leptonicBjet(), 
	hadronicBJet(), 
	jet1FromW(),
	jet2FromW(), 
	leptonFromW(), 
	met() {

}

TtbarHypothesis::TtbarHypothesis( const LeptonPointer& elec, const ParticlePointer& neut,
				    const JetPointer& lepBJet,   const JetPointer& hadBJet,
				    const JetPointer& hadWJet1,  const JetPointer& hadWJet2 ) :
    totalChi2(99999.),
    leptonicChi2(99999.),
    hadronicChi2(99999.),
    globalChi2(99999.),
    hadronicTop(),
    leptonicTop(),
    leptonicW(new Particle(*elec + *neut)),
    hadronicW(new Particle(*hadWJet1 + *hadWJet2)),
    resonance(),
    neutrinoFromW(neut),
    leptonicBjet(lepBJet),
    hadronicBJet(hadBJet),
    jet1FromW(hadWJet1),
    jet2FromW(hadWJet2),
    leptonFromW(elec),
    met(new MET(neut->px(), neut->py()))
  {

}

TtbarHypothesis::~TtbarHypothesis() {

}

bool TtbarHypothesis::isValid() const{
	bool hasObjects = leptonFromW != 0 && neutrinoFromW != 0 && jet1FromW != 0 && jet2FromW != 0 && hadronicBJet != 0 && leptonicBjet;
	return hasObjects;

}

bool TtbarHypothesis::isPhysical() const{
	bool hasPhysicalSolution = leptonicTop->mass() > 0 && leptonicW->mass() > 0
				&& hadronicW->mass() > 0 && hadronicTop->mass() > 0;
	return hasPhysicalSolution;
}

void TtbarHypothesis::combineReconstructedObjects() {
	if(isValid()){
		leptonicW =  ParticlePointer(new Particle(*neutrinoFromW + *leptonFromW));
		if(jet1FromW != jet2FromW)
			hadronicW =  ParticlePointer(new Particle(*jet1FromW + *jet2FromW));
		else
			hadronicW = jet1FromW;

		leptonicTop = ParticlePointer(new Particle(*leptonicBjet + *leptonicW));
		hadronicTop = ParticlePointer(new Particle(*hadronicBJet + *hadronicW));

		resonance = ParticlePointer(new Particle(*leptonicTop + *hadronicTop));

	}
	else{
		throwDetailedException();
	}
}

void TtbarHypothesis::throwDetailedException() const {
	std::string msg = "TTbar Hypothesis not filled properly: \n";
	if (leptonFromW == 0)
		msg += "Electron from W: not filled \n";
	else
		msg += "Electron from W: filled \n";

	if (neutrinoFromW == 0)
		msg += "Neutrino from W: not filled \n";
	else
		msg += "Neutrino from W: filled \n";

	if (jet1FromW == 0)
		msg += "Jet 1 from W: not filled \n";
	else
		msg += "Jet 1 from W: filled \n";

	if (jet2FromW == 0)
		msg += "Jet 2 from W: not filled \n";
	else
		msg += "Jet 2 from W: filled \n";

	if (leptonicBjet == 0)
		msg += "Leptonic b-jet: not filled \n";
	else
		msg += "Leptonic b-jet: filled \n";

	if (hadronicBJet == 0)
		msg += "Hadronic b-jet: not filled \n";
	else
		msg += "Hadronic b-jet: filled \n";

	throw ReconstructionException(msg);
}

double TtbarHypothesis::M3() const {
	double m3(0), max_et(0);
	JetCollection mcJets;
	mcJets.clear();
	mcJets.push_back(jet1FromW);
	mcJets.push_back(jet2FromW);
	mcJets.push_back(leptonicBjet);
	mcJets.push_back(hadronicBJet);
	if (mcJets.size() >= 3) {
		for (unsigned int index1 = 0; index1 < mcJets.size() - 2; ++index1) {
			for (unsigned int index2 = index1 + 1; index2 < mcJets.size() - 1;
					++index2) {
				for (unsigned int index3 = index2 + 1; index3 < mcJets.size();
						++index3) {
					FourVector m3Vector(
							mcJets.at(index1)->getFourVector()
									+ mcJets.at(index2)->getFourVector()
									+ mcJets.at(index3)->getFourVector());
					double currentEt = m3Vector.Et();
					if (currentEt > max_et) {
						max_et = currentEt;
						m3 = m3Vector.M();
					}
				}
			}
		}
	}

	return m3;
}

double TtbarHypothesis::sumPt() const {
    return leptonicBjet->pt() + hadronicBJet->pt() + jet1FromW->pt() + jet2FromW->pt();
}

double TtbarHypothesis::PtTtbarSystem() const {
    return resonance->pt();
}
//bool TtbarHypothesis::operator <(const TtbarHypothesis& hyp) const {
//	return totalChi2 < hyp.totalChi2;
//}
//
//bool TtbarHypothesis::operator ==(const TtbarHypothesis& hyp) const {
//	return totalChi2 == hyp.totalChi2;
//}

} // namespace BAT
