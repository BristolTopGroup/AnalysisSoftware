/*
 * BasicRule.h
 *
 *  Created on: 15 Apr 2011
 *      Author: kreczko
 */

#ifndef BASICRULE_H_
#define BASICRULE_H_

#include "../RecoObjects/Jet.h"
#include "../TtbarHypothesis.h"
namespace BAT {

class BasicRule {
public:
    BasicRule();
    virtual ~BasicRule();

    double softLeptonicBJetCondition(const JetPointer jet) const;
    bool hardLeptonicBJetCondition(const JetPointer jet) const;

    double softHadronicBJetCondition(const JetPointer jet) const;
    bool hardHadronicBJetCondition(const JetPointer jet) const;

    double softHadronicJetFromWCondition(const JetPointer jet) const;
    bool hardHadronicJetFromWCondition(const JetPointer jet) const;

    double softGlobalCondition(const TtbarHypothesisPointer hypothesis) const;
    bool hardGlobalCondition(const TtbarHypothesisPointer hypothesis) const;

    bool meetsInitialConditaion() const;
};

typedef boost::shared_ptr<BasicRule> Rule;
}

#endif /* BASICRULE_H_ */
