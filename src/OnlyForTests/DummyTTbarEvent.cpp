/*
 * DummyTTbarEvent.cpp
 *
 *  Created on: 25 Aug 2010
 *      Author: kreczko
 */

#include "../../interface/OnlyForTests/DummyTTbarEvent.h"
#include <iostream>

using namespace std;
namespace BAT {

DummyTTbarEvent::DummyTTbarEvent() :
    BAT::TopPairEventCandidate(), 
    passScraping(false), 
    passHLT(false), 
    passPV(false),
    passElectronCut(false),
    passConversion(false), 
    passConversionPartnerTrack(false), 
    passesMuon(false), 
    passes1JetCut(false),
    passes2JetCut(false),
    passes3JetCut(false),
    passes4JetCut(false),
    passesZveto(false), 
    useCustomReturnValues(false) {

}

DummyTTbarEvent::~DummyTTbarEvent() {
}

void DummyTTbarEvent::setElectronFromW(ElectronPointer electron) {
    electronFromW = electron;
}

bool DummyTTbarEvent::passesScrapingFilter() const {
    if (useCustomReturnValues)
        return passScraping;
    else
        return TopPairEventCandidate::passesScrapingFilter();
}

bool DummyTTbarEvent::passesHighLevelTrigger() const {
    if (useCustomReturnValues)
        return passHLT;
    else
        return TopPairEventCandidate::passesHighLevelTrigger();
}

bool DummyTTbarEvent::hasOneGoodPrimaryVertex() const {
    if (useCustomReturnValues)
        return passPV;
    else
        return TopPairEventCandidate::hasOneGoodPrimaryVertex();
}

bool DummyTTbarEvent::hasOnlyOneGoodIsolatedElectron() const {
    if (useCustomReturnValues)
        return passElectronCut;
    else
        return TopPairEventCandidate::hasOnlyOneGoodIsolatedElectron();
}

bool DummyTTbarEvent::isolatedElectronDoesNotComeFromConversion() const {
    if (useCustomReturnValues)
        return passConversion;
    else
        return TopPairEventCandidate::isolatedElectronDoesNotComeFromConversion();
}

bool DummyTTbarEvent::isolatedElectronNotTaggedAsFromConversion() const {
    if (useCustomReturnValues)
        return passConversionPartnerTrack;
    else
        return TopPairEventCandidate::isolatedElectronNotTaggedAsFromConversion();
}

bool DummyTTbarEvent::hasNoIsolatedMuon() const {
    if (useCustomReturnValues)
        return passesMuon;
    else
        return TopPairEventCandidate::hasNoIsolatedMuon();
}
bool DummyTTbarEvent::hasAtLeastOneGoodJet() const {
    if (useCustomReturnValues)
        return passes1JetCut;
    else
        return TopPairEventCandidate::hasAtLeastOneGoodJet();
}

bool DummyTTbarEvent::hasAtLeastTwoGoodJets() const {
    if (useCustomReturnValues)
        return passes2JetCut;
    else
        return TopPairEventCandidate::hasAtLeastTwoGoodJets();
}

bool DummyTTbarEvent::hasAtLeastThreeGoodJets() const {
    if (useCustomReturnValues)
        return passes3JetCut;
    else
        return TopPairEventCandidate::hasAtLeastThreeGoodJets();
}

bool DummyTTbarEvent::hasAtLeastFourGoodJets() const {
    if (useCustomReturnValues)
        return passes4JetCut;
    else
        return TopPairEventCandidate::hasAtLeastFourGoodJets();
}

bool DummyTTbarEvent::isNotAZBosonEvent() const {
    if (useCustomReturnValues)
        return passesZveto;
    else
        return TopPairEventCandidate::isNotAZBosonEvent();
}

bool DummyTTbarEvent::passesNMinus1(enum TTbarEPlusJetsSelection::Step omitted) const {
    bool passes(true);

    for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
        if (cut == (unsigned int) omitted)
            continue;
        passes = passes && passesSelectionStep((TTbarEPlusJetsSelection::Step) cut);
    }
    return passes;
}

bool DummyTTbarEvent::passesSelectionStep(enum TTbarEPlusJetsSelection::Step step) const {
    switch (step) {
        case TTbarEPlusJetsSelection::FilterOutScraping:
            return passesScrapingFilter();
        case TTbarEPlusJetsSelection::HighLevelTrigger:
            return passesHighLevelTrigger();
        case TTbarEPlusJetsSelection::GoodPrimaryvertex:
            return hasOneGoodPrimaryVertex();
        case TTbarEPlusJetsSelection::OneIsolatedElectron:
            return hasOnlyOneGoodIsolatedElectron();
        case TTbarEPlusJetsSelection::ConversionRejection:
            return isolatedElectronDoesNotComeFromConversion();
        case TTbarEPlusJetsSelection::ConversionFinder:
            return isolatedElectronNotTaggedAsFromConversion();
        case TTbarEPlusJetsSelection::LooseMuonVeto:
            return hasNoIsolatedMuon();
        case TTbarEPlusJetsSelection::AtLeastOneGoodJets:
            return hasAtLeastOneGoodJet();
        case TTbarEPlusJetsSelection::AtLeastTwoGoodJets:
            return hasAtLeastTwoGoodJets();
        case TTbarEPlusJetsSelection::AtLeastThreeGoodJets:
            return hasAtLeastThreeGoodJets();
        case TTbarEPlusJetsSelection::AtLeastFourGoodJets:
            return hasAtLeastFourGoodJets();
        case TTbarEPlusJetsSelection::Zveto:
            return isNotAZBosonEvent();
        default:
            return false;
    }
}

}
