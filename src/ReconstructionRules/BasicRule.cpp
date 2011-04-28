/*
 * BasicRule.cpp
 *
 *  Created on: 15 Apr 2011
 *      Author: kreczko
 */

#include "../../interface/ReconstructionRules/BasicRule.h"
namespace BAT {
BasicRule::BasicRule() {

}

BasicRule::~BasicRule() {
}

double BasicRule::softGlobalCondition(const TtbarHypothesisPointer hypo) const {
    return 0;
}

bool BasicRule::hardGlobalCondition(const TtbarHypothesisPointer hypo) const {
    return true;
}

double BasicRule::softHadronicBJetCondition(const JetPointer jet) const {
    return 0;
}

bool BasicRule::hardHadronicBJetCondition(const JetPointer jet) const {
    return 0;
}

double BasicRule::softLeptonicBJetCondition(const JetPointer jet) const {
    return 0;
}

bool BasicRule::hardLeptonicBJetCondition(const JetPointer jet) const {
    return 0;
}

double BasicRule::softHadronicJetFromWCondition(const JetPointer jet) const {
    return 0;
}
bool BasicRule::hardHadronicJetFromWCondition(const JetPointer jet) const {
    return true;
}
}
