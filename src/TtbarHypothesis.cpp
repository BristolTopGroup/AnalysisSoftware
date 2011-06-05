/*
 * TtbarHypothesis.cpp
 *
 *  Created on: Dec 4, 2010
 *      Author: lkreczko
 */

#include "../interface/TtbarHypothesis.h"

namespace BAT {

TtbarHypothesis::TtbarHypothesis() :
	totalChi2(99999.), 
	leptonicChi2(99999.), 
	hadronicChi2(99999.), 
	globalChi2(99999.), 
	disc(999999),
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
	electronFromW(), 
	met() {

}

TtbarHypothesis::~TtbarHypothesis() {

}

bool TtbarHypothesis::operator <(const TtbarHypothesis& hyp) const {
	return totalChi2 < hyp.totalChi2;
}

bool TtbarHypothesis::operator ==(const TtbarHypothesis& hyp) const {
	return totalChi2 == hyp.totalChi2;
}

} // namespace BAT
