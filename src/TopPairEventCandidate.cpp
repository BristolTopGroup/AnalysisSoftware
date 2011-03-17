/*
 * TopPairEventCandidate.cpp
 *
 *  Created on: 9 Jul 2010
 *      Author: kreczko
 */

#include "../interface/TopPairEventCandidate.h"
#include <iostream>
#include <iomanip>
using namespace std;

namespace BAT {

double const TopPairEventCandidate::matched_angle = 0.945666;
double const TopPairEventCandidate::matched_angle_sigma = 0.311091;
double const TopPairEventCandidate::matched_leptonic_top_mass = 169.0;
double const TopPairEventCandidate::matched_leptonic_top_mass_sigma = 16.3;
double const TopPairEventCandidate::matched_hadronic_W_mass = 83.;
double const TopPairEventCandidate::matched_hadronic_W_mass_sigma = 10.8995;
double const TopPairEventCandidate::matched_hadronic_top_mass = 174.7;
double const TopPairEventCandidate::matched_hadronic_top_mass_sigma = 14.6;
double const TopPairEventCandidate::matched_ptratio = 0.18552;
double const TopPairEventCandidate::matched_ptratio_sigma = 0.401973;
double const TopPairEventCandidate::matched_pt_ttbarSystem = 0.;
double const TopPairEventCandidate::matched_pt_ttbarSystem_sigma = 50.;
double const TopPairEventCandidate::matched_HTSystem = 1;
double const TopPairEventCandidate::matched_HTSystem_sigma = 0.1;
double const TopPairEventCandidate::W_mass = 80.389;

NeutrinoSelectionCriterion::value TopPairEventCandidate::usedNeutrinoSelection = NeutrinoSelectionCriterion::chi2;

TopPairEventCandidate::TopPairEventCandidate() :
    Event(),
    electronFromW(),
    leptonicBJet(),
    hadronicBJet(),
    jet1FromW(),
    jet2FromW(),
    neutrino1(),
    neutrino2(),
    leptonicW1(),
    leptonicW2(),
    hadronicW(),
    leptonicTop1(),
    leptonicTop2(),
    hadronicTop(),
    selectedNeutrino(0),
    currentSelectedNeutrino(0),
    hadronicBIndex(0),
    leptonicBIndex(0),
    jet1FromWIndex(0),
    jet2FromWIndex(0),
    doneReconstruction(false),
    conversionTagger(new ConversionTagger()),
    doneConversionTagging(false),
    solutions(),
    compareSolutions(){
}

TopPairEventCandidate::TopPairEventCandidate(const Event& event) :
    Event(event),
    electronFromW(),
    leptonicBJet(),
    hadronicBJet(),
    jet1FromW(),
    jet2FromW(),
    neutrino1(),
    neutrino2(),
    leptonicW1(),
    leptonicW2(),
    hadronicW(),
    leptonicTop1(),
    leptonicTop2(),
    hadronicTop(),
    selectedNeutrino(0),
    currentSelectedNeutrino(0),
    hadronicBIndex(0),
    leptonicBIndex(0),
    jet1FromWIndex(0),
    jet2FromWIndex(0),
    doneReconstruction(false),
    conversionTagger(new ConversionTagger()),
    doneConversionTagging(false) {

}

TopPairEventCandidate::~TopPairEventCandidate() {
}

bool TopPairEventCandidate::passesScrapingFilter() const {
    if (tracks.size() > 10) {
        if (numberOfHighPurityTracks / (1.0 * tracks.size()) > 0.25)
            return true;
        else
            return false;
    } else
        return isBeamScraping == false;
}

bool TopPairEventCandidate::passesHighLevelTrigger() const {
    if (isRealData()) {
        if (runNumber < 140041)
            return HLT(HLTriggers::HLT_Ele10_LW_L1R);
        else if (runNumber >= 140041 && runNumber <= 143962)
            return HLT(HLTriggers::HLT_Ele15_SW_L1R);
        else if (runNumber > 143962 && runNumber <= 146427)
            return HLT(HLTriggers::HLT_Ele15_SW_CaloEleId_L1R);
        else if (runNumber > 146427 && runNumber <= 147116)
            return HLT(HLTriggers::HLT_Ele17_SW_CaloEleId_L1R);
        else if (runNumber > 147116 && runNumber <= 148818)
            return HLT(HLTriggers::HLT_Ele17_SW_TightEleId_L1R);
        else if (runNumber >= 148819 && runNumber < 149181)
            return HLT(HLTriggers::HLT_Ele22_SW_TighterEleId_L1R_v2);
        else if(runNumber >= 149181)
            return HLT(HLTriggers::HLT_Ele22_SW_TighterEleId_L1R_v3);
        else
            return false;
    }
    else // do not use HLT for MC
        return true;
}

bool TopPairEventCandidate::hasOneGoodPrimaryVertex() const {
    return PrimaryVertex()->isGood();
}

bool TopPairEventCandidate::hasOnlyOneGoodIsolatedElectron() const {
    if(Event::usePFIsolation)
        return goodPFIsolatedElectrons.size() == 1;
    else
        return goodIsolatedElectrons.size() == 1;
}

bool TopPairEventCandidate::isolatedElectronDoesNotComeFromConversion() const {
    bool passConversion = false;
    if (Event::usePFIsolation) {
        if (goodPFIsolatedElectrons.size() > 0)
            passConversion = goodPFIsolatedElectrons.front()->isFromConversion() == false;
    } else {
        if (goodIsolatedElectrons.size() > 0)
            passConversion = goodIsolatedElectrons.front()->isFromConversion() == false;
    }

    return passConversion;
}

bool TopPairEventCandidate::isolatedElectronNotTaggedAsFromConversion() const {
    bool passConversion = false;
    ElectronPointer electron;
    if (Event::usePFIsolation) {
        if (goodPFIsolatedElectrons.size() > 0)
            electron = goodPFIsolatedElectrons.front();
    } else {
        if (goodIsolatedElectrons.size() > 0)
            electron = goodIsolatedElectrons.front();
    }
    if (electron != 0) {
        if (useCustomConversionTagger) {
            conversionTagger->calculateConversionVariables(electron, tracks, 3.8, 0.45);
            passConversion = conversionTagger->isFromConversion(0.02, 0.02) == false;
        } else {
            passConversion = electron->isTaggedAsConversion(0.02, 0.02) == false;
        }
    }

    return passConversion;
}

bool TopPairEventCandidate::hasNoIsolatedMuon() const {
    return goodIsolatedMuons.size() == 0;
}

bool TopPairEventCandidate::hasAtLeastOneGoodJet() const {
    return goodJets.size() >= 1;
}

bool TopPairEventCandidate::hasAtLeastTwoGoodJets() const {
    return goodJets.size() >= 2;
}

bool TopPairEventCandidate::hasAtLeastThreeGoodJets() const {
    return goodJets.size() >= 3;
}

bool TopPairEventCandidate::hasAtLeastFourGoodJets() const {
    return goodJets.size() >= 4;
}

bool TopPairEventCandidate::isNotAZBosonEvent() const {
    float invariantMass = 0;
    bool isZEvent = false;
    ElectronPointer isoElectron;

    if (Event::usePFIsolation && goodPFIsolatedElectrons.size() > 0)
        isoElectron = goodPFIsolatedElectrons.front();
    else if (goodIsolatedElectrons.size() > 0)
        isoElectron = goodIsolatedElectrons.front();

    if (isoElectron != NULL && allElectrons.size() > 1) {
        for (unsigned int index = 0; index < allElectrons.size(); ++index) {
            const ElectronPointer looseElectron = allElectrons.at(index);
            bool passLooseIso = false;

            if (Event::usePFIsolation)
                passLooseIso = looseElectron->isLoose() && looseElectron->pfIsolation() < 1.;
            else
                passLooseIso = looseElectron->isLoose() && looseElectron->relativeIsolation() < 1.;

            if (passLooseIso)
                invariantMass = isoElectron->invariantMass(looseElectron);
            else
                invariantMass = 0;

            bool passesLowerLimit = invariantMass > 76;
            bool passesUpperLimit = invariantMass < 106;
            if (passesLowerLimit && passesUpperLimit)
                isZEvent = true;
        }

    }


    return isZEvent == false;
}

bool TopPairEventCandidate::passesFullTTbarEPlusJetSelection() const {
    unsigned int newstep = (int) TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 1;
    return passesSelectionStepUpTo((TTbarEPlusJetsSelection::Step) newstep);
}

bool TopPairEventCandidate::passesSelectionStepUpTo(enum TTbarEPlusJetsSelection::Step step) const {
    if (step == TTbarEPlusJetsSelection::FilterOutScraping)
        return passesSelectionStep(step);
    else {
        unsigned int newstep = (int) step - 1;
        return passesSelectionStep(step) && passesSelectionStepUpTo((TTbarEPlusJetsSelection::Step) newstep);
    }
}

bool TopPairEventCandidate::passesNMinus1(enum TTbarEPlusJetsSelection::Step omitted) const {
    bool passes(true);

    for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
        if (cut == (unsigned int) omitted)
            continue;
        passes = passes && passesSelectionStep((TTbarEPlusJetsSelection::Step) cut);
    }
    return passes;
}

bool TopPairEventCandidate::passesSelectionStep(enum TTbarEPlusJetsSelection::Step step) const {
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

bool TopPairEventCandidate::passesRelIsoSelection() const{
    bool passesFirst3 = passesSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex);
        bool passGoodElectrons = goodElectrons.size() > 0 && goodIsolatedElectrons.size() < 2;
        bool passesBothIsolationvetos = false;
        if (passGoodElectrons) {
            const ElectronPointer electron = MostIsolatedElectron();
            if (electron->isGood()) {
                if (useCustomConversionTagger) {
                    conversionTagger->calculateConversionVariables(electron, tracks, 3.8, 0.45);
                    passesBothIsolationvetos = electron->isFromConversion() == false && conversionTagger->isFromConversion(
                            0.02, 0.02) == false;
                }
                else{
                    passesBothIsolationvetos = electron->isFromConversion() == false && electron->isTaggedAsConversion(
                            0.02, 0.02) == false;
                }
            }

        }
        bool muonVeto = hasNoIsolatedMuon();
        bool Zveto = isNotAZBosonEvent();
        return passesFirst3 && passGoodElectrons && passesBothIsolationvetos && muonVeto && Zveto;
}

bool TopPairEventCandidate::passesRelIsoControlSelection() const{
    bool passesFirst3 = passesSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex);
       bool passGoodElectrons = allElectrons.size() > 0 && goodIsolatedElectrons.size() < 2;
       bool passesBothIsolationvetos = false;
       if (passGoodElectrons) {
           const ElectronPointer electron = MostIsolatedElectron();
           if (electron->isQCDElectron()) {
               if (useCustomConversionTagger) {
                   conversionTagger->calculateConversionVariables(electron, tracks, 3.8, 0.45);
                   passesBothIsolationvetos = electron->isFromConversion() == false && conversionTagger->isFromConversion(
                           0.02, 0.02) == false;
               } else {
                   passesBothIsolationvetos = electron->isFromConversion() == false && electron->isTaggedAsConversion(
                           0.02, 0.02) == false;
               }
           }

       }
       bool muonVeto = hasNoIsolatedMuon();
       bool Zveto = isNotAZBosonEvent();
       return passesFirst3 && passGoodElectrons && passesBothIsolationvetos && muonVeto && Zveto;
}

bool TopPairEventCandidate::passesPFIsoSelection() const{
    bool passesFirst3 = passesSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex);
        bool passGoodElectrons = goodElectrons.size() > 0 && goodPFIsolatedElectrons.size() < 2;
        bool passesBothIsolationvetos = false;
        if (passGoodElectrons) {
            const ElectronPointer electron = MostPFIsolatedElectron();
            if (electron->isGood()) {
                if (useCustomConversionTagger) {
                    conversionTagger->calculateConversionVariables(electron, tracks, 3.8, 0.45);
                    passesBothIsolationvetos = electron->isFromConversion() == false && conversionTagger->isFromConversion(
                            0.02, 0.02) == false;
                }
                else{
                    passesBothIsolationvetos = electron->isFromConversion() == false && electron->isTaggedAsConversion(
                            0.02, 0.02) == false;
                }
            }

        }
        bool muonVeto = hasNoIsolatedMuon();
        bool Zveto = isNotAZBosonEvent();
        return passesFirst3 && passGoodElectrons && passesBothIsolationvetos && muonVeto && Zveto;
}

bool TopPairEventCandidate::passesPFIsoControlSelection() const{
    bool passesFirst3 = passesSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex);
       bool passGoodElectrons = allElectrons.size() > 0 && goodPFIsolatedElectrons.size() < 2;
       bool passesBothIsolationvetos = false;
       if (passGoodElectrons) {
           const ElectronPointer electron = MostPFIsolatedElectron();
           if (electron->isQCDElectron()) {
               if (useCustomConversionTagger) {
                   conversionTagger->calculateConversionVariables(electron, tracks, 3.8, 0.45);
                   passesBothIsolationvetos = electron->isFromConversion() == false && conversionTagger->isFromConversion(
                           0.02, 0.02) == false;
               } else {
                   passesBothIsolationvetos = electron->isFromConversion() == false && electron->isTaggedAsConversion(
                           0.02, 0.02) == false;
               }
           }

       }
       bool muonVeto = hasNoIsolatedMuon();
       bool Zveto = isNotAZBosonEvent();
       return passesFirst3 && passGoodElectrons && passesBothIsolationvetos && muonVeto && Zveto;
}

bool TopPairEventCandidate::passesConversionSelection() const {
    bool passesFirst6 = passesSelectionStepUpTo(TTbarEPlusJetsSelection::Zveto);
    bool isConversion1 = isolatedElectronDoesNotComeFromConversion() == false;
    bool isConversion2 = isolatedElectronNotTaggedAsFromConversion() == false;
    bool atLeast4Jets = hasAtLeastFourGoodJets();
    return passesFirst6 && (isConversion1 || isConversion2) && atLeast4Jets;
}

bool TopPairEventCandidate::passesAntiIsolationSelection() const {
    //require at least one good electron and no isolated good electrons
    if (!(goodElectrons.size() > 0 && goodIsolatedElectrons.size() == 0))
            return false;

    bool passesFirst3 = passesSelectionStep(TTbarEPlusJetsSelection::GoodPrimaryvertex);


    bool muonVeto = passesSelectionStep(TTbarEPlusJetsSelection::LooseMuonVeto);
    bool zveto = passesSelectionStep(TTbarEPlusJetsSelection::Zveto);
    bool conversionVeto = (goodElectrons.front()->isFromConversion() || goodElectrons.front()->isTaggedAsConversion(
            0.2, 0.2)) == false;
    bool jets = hasAtLeastFourGoodJets();
    return passesFirst3 && muonVeto && zveto && conversionVeto && jets;
}

void TopPairEventCandidate::reconstructUsingTopMassDifference(ElectronPointer electron) {
    if (goodJets.size() < 4)
      throw ReconstructionException("Not enough jets available to reconstruct top event using Mass Equality method.");
    electronFromW = electron;
    selectedNeutrino = 0;
    currentSelectedNeutrino = 0;
    reconstructNeutrinos();

    double chosen_TopMassDifference(9999999.);

    for (unsigned short hadBindex = 0; hadBindex < goodJets.size(); ++hadBindex) {
      for (unsigned short lepBindex = 0; lepBindex < goodJets.size(); ++lepBindex) {
        if (lepBindex == hadBindex)
          continue;
        for (unsigned short jet1Index = 0; jet1Index < goodJets.size(); ++jet1Index) {
          if (jet1Index == lepBindex || jet1Index == hadBindex)
            continue;
          for (unsigned short jet2Index = 0; jet2Index < goodJets.size(); ++jet2Index) {
            if (jet2Index == jet1Index || jet2Index == lepBindex || jet2Index == hadBindex)
              continue;
            hadronicBJet = goodJets.at(hadBindex);
            leptonicBJet = goodJets.at(lepBindex);
            jet1FromW = goodJets.at(jet1Index);
            jet2FromW = goodJets.at(jet2Index);

            leptonicW1 = ParticlePointer(new Particle(*neutrino1 + *electronFromW));
            leptonicW2 = ParticlePointer(new Particle(*neutrino2 + *electronFromW));
            hadronicW = ParticlePointer(new Particle(*jet1FromW + *jet2FromW));
            leptonicTop1 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW1));
            leptonicTop2 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW2));
            hadronicTop = ParticlePointer(new Particle(*hadronicBJet + *hadronicW));
            selectNeutrinoSolution();
            double TopMassDifference = calculateTopMassDifference(currentSelectedNeutrino);
            if (TopMassDifference < chosen_TopMassDifference) {
              hadronicBIndex = hadBindex;
              leptonicBIndex = lepBindex;
              jet1FromWIndex = jet1Index;
              jet2FromWIndex = jet2Index;
              chosen_TopMassDifference = TopMassDifference;
              selectedNeutrino = currentSelectedNeutrino;
            }
          }
        }
      }
    }

    hadronicBJet = goodJets.at(hadronicBIndex);
    leptonicBJet = goodJets.at(leptonicBIndex);
    jet1FromW = goodJets.at(jet1FromWIndex);
    jet2FromW = goodJets.at(jet2FromWIndex);
    leptonicW1 = ParticlePointer(new Particle(*neutrino1 + *electronFromW));
    leptonicW2 = ParticlePointer(new Particle(*neutrino2 + *electronFromW));
    hadronicW = ParticlePointer(new Particle(*jet1FromW + *jet2FromW));
    leptonicTop1 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW1));
    leptonicTop2 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW2));
    hadronicTop = ParticlePointer(new Particle(*hadronicBJet + *hadronicW));
    if (selectedNeutrino == 1)
      ttbarResonance = ParticlePointer(new Particle(*leptonicTop1 + *hadronicTop));
    else
      ttbarResonance = ParticlePointer(new Particle(*leptonicTop2 + *hadronicTop));

    doneReconstruction = true;

}

void TopPairEventCandidate::reconstructUsingChi2(ElectronPointer electron) {
    if (goodJets.size() < 4)
        throw ReconstructionException("Not enough jets available to reconstruct top event using Chi2 method.");
    electronFromW = electron;
    selectedNeutrino = 0;
    currentSelectedNeutrino = 0;
    reconstructNeutrinos();

    double chosen_Chi2Total(9999999.);

    for (unsigned short hadBindex = 0; hadBindex < goodJets.size(); ++hadBindex) {
        for (unsigned short lepBindex = 0; lepBindex < goodJets.size(); ++lepBindex) {
            if (lepBindex == hadBindex)
                continue;
            for (unsigned short jet1Index = 0; jet1Index < goodJets.size(); ++jet1Index) {
                if (jet1Index == lepBindex || jet1Index == hadBindex)
                    continue;
                for (unsigned short jet2Index = 0; jet2Index < goodJets.size(); ++jet2Index) {
                    if (jet2Index == jet1Index || jet2Index == lepBindex || jet2Index == hadBindex)
                        continue;
                    hadronicBJet = goodJets.at(hadBindex);
                    leptonicBJet = goodJets.at(lepBindex);
                    jet1FromW = goodJets.at(jet1Index);
                    jet2FromW = goodJets.at(jet2Index);

                    leptonicW1 = ParticlePointer(new Particle(*neutrino1 + *electronFromW));
                    leptonicW2 = ParticlePointer(new Particle(*neutrino2 + *electronFromW));
                    hadronicW = ParticlePointer(new Particle(*jet1FromW + *jet2FromW));
                    leptonicTop1 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW1));
                    leptonicTop2 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW2));
                    hadronicTop = ParticlePointer(new Particle(*hadronicBJet + *hadronicW));
                    fillHypotheses();
                    selectNeutrinoSolution();
                    double chi2 = getTotalChi2(currentSelectedNeutrino);
                    if (chi2 < chosen_Chi2Total) {
                        hadronicBIndex = hadBindex;
                        leptonicBIndex = lepBindex;
                        jet1FromWIndex = jet1Index;
                        jet2FromWIndex = jet2Index;
                        chosen_Chi2Total = chi2;
                        selectedNeutrino = currentSelectedNeutrino;
                    }
                }
            }
        }
    }
    std::sort(solutions.begin(), solutions.end(), compareSolutions);
    hadronicBJet = goodJets.at(hadronicBIndex);
    leptonicBJet = goodJets.at(leptonicBIndex);
    jet1FromW = goodJets.at(jet1FromWIndex);
    jet2FromW = goodJets.at(jet2FromWIndex);
    leptonicW1 = ParticlePointer(new Particle(*neutrino1 + *electronFromW));
    leptonicW2 = ParticlePointer(new Particle(*neutrino2 + *electronFromW));
    hadronicW = ParticlePointer(new Particle(*jet1FromW + *jet2FromW));
    leptonicTop1 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW1));
    leptonicTop2 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW2));
    hadronicTop = ParticlePointer(new Particle(*hadronicBJet + *hadronicW));
    if (selectedNeutrino == 1)
        ttbarResonance = ParticlePointer(new Particle(*leptonicTop1 + *hadronicTop));
    else
        ttbarResonance = ParticlePointer(new Particle(*leptonicTop2 + *hadronicTop));

    doneReconstruction = true;
}

void TopPairEventCandidate::reconstructNeutrinos() {
    boost::array<double, 2> neutrinoPzs = computeNeutrinoPz();
    double energy1 = sqrt(met->et() * met->et() + neutrinoPzs.at(0) * neutrinoPzs.at(0));
    double energy2 = sqrt(met->et() * met->et() + neutrinoPzs.at(1) * neutrinoPzs.at(1));
    neutrino1 = ParticlePointer(new Particle(energy1, met->px(), met->py(), neutrinoPzs.at(0)));
    neutrino2 = ParticlePointer(new Particle(energy2, met->px(), met->py(), neutrinoPzs.at(1)));

    if (isnan(neutrino1->energy()) && isnan(neutrino2->energy()))
        throw ReconstructionException("No physical neutrino solution found");
    else if (isnan(neutrino1->energy()))
        neutrino1 = neutrino2;
    else if (isnan(neutrino2->energy()))
        neutrino2 = neutrino1;
}

const boost::array<double, 2> TopPairEventCandidate::computeNeutrinoPz() {
    if (electronFromW == 0)
        throw ReconstructionException("Could not reconstruct neutrinos: no isolated electrons found");
    if (met->energy() == 0)
        throw ReconstructionException("Could not reconstruct neutrinos: no MET found");
    boost::array<double, 2> neutrinoPzs;
    //    const ElectronPointer electron = goodIsolatedElectrons.front();

    double pz1(0), pz2(0);
    //    double M_W = 80.389;
    double M_e = 0.0005;
    double ee = electronFromW->energy();
    double pxe = electronFromW->px();
    double pye = electronFromW->py();
    double pze = electronFromW->pz();
    double pxnu = met->px();
    double pynu = met->py();

    double a = W_mass * W_mass - M_e * M_e + 2.0 * pxe * pxnu + 2.0 * pye * pynu;
    double A = 4.0 * (ee * ee - pze * pze);
    double B = -4.0 * a * pze;
    double C = 4.0 * ee * ee * (pxnu * pxnu + pynu * pynu) - a * a;

    double tmproot = B * B - 4.0 * A * C;
    if (tmproot < 0) {
        pz1 = pz2 = -B / (2 * A);
    } else {
        pz1 = (-B + TMath::Sqrt(tmproot)) / (2.0 * A);
        pz2 = (-B - TMath::Sqrt(tmproot)) / (2.0 * A);

    }
    neutrinoPzs[0] = pz1;
    neutrinoPzs[1] = pz2;
    return neutrinoPzs;
}

//bool TopPairEventCandidate::hasNextJetCombination(){
//    unsigned short njets = goodJets.size();
//    return (hadronicBIndex == njets && leptonicBJet == njets-1 && jet1FormWIndex == njets -2 && jet2);
//}
//
//void TopPairEventCandidate::selectNextJetCombination(){
//
//}

void TopPairEventCandidate::fillHypotheses() {
	TtbarHypothesisPointer hypothesis1(fillHypothesis(1));
	TtbarHypothesisPointer hypothesis2(fillHypothesis(2));
	solutions.push_back(hypothesis1);
	solutions.push_back(hypothesis2);

}

const TtbarHypothesisPointer TopPairEventCandidate::fillHypothesis(unsigned short int neutrinoSolution) {
	TtbarHypothesisPointer hypothesis(new TtbarHypothesis());
	hypothesis->electronFromW = electronFromW;
	hypothesis->leptonicBjet = leptonicBJet;
	hypothesis->hadronicBJet = hadronicBJet;
	hypothesis->jet1FromW = jet1FromW;
	hypothesis->jet2FromW = jet2FromW;
	hypothesis->hadronicW = hadronicW;
	hypothesis->hadronicTop = hadronicTop;
	hypothesis->hadronicChi2 = getHadronicChi2();
	if(neutrinoSolution == 1) {
		hypothesis->neutrinoFromW = neutrino1;
		hypothesis->leptonicW = leptonicW1;
		hypothesis->leptonicTop = leptonicTop1;
	}
	else {
		hypothesis->neutrinoFromW = neutrino2;
		hypothesis->leptonicW = leptonicW2;
		hypothesis->leptonicTop = leptonicTop2;
	}

	hypothesis->totalChi2 = getTotalChi2(neutrinoSolution);
	hypothesis->globalChi2 = getGlobalChi2(neutrinoSolution);
	hypothesis->leptonicChi2 = getLeptonicChi2(neutrinoSolution);
	ParticlePointer resonance(new Particle(*hypothesis->leptonicTop + *hypothesis->hadronicTop));
	hypothesis->resonance = resonance;
	return hypothesis;
}

void TopPairEventCandidate::selectNeutrinoSolution() {

    if (leptonicTop1->mass() < 0 && leptonicTop2->mass() < 0) {
        inspectReconstructedEvent();
        throw ReconstructionException("No valid neutrino solution found");
    } else if (leptonicTop1->mass() < 0 && leptonicTop2->mass() > 0) {
        currentSelectedNeutrino = 2;
    } else if (leptonicTop1->mass() > 0 && leptonicTop2->mass() < 0) {
        currentSelectedNeutrino = 1;
    } else {// both solutions give positive mass
        switch (usedNeutrinoSelection) {
        case NeutrinoSelectionCriterion::TopMassDifference:
            fabs(leptonicTop1->mass()-hadronicTop->mass()) < fabs(leptonicTop2->mass()-hadronicTop->mass()) ?
                      currentSelectedNeutrino = 1 : currentSelectedNeutrino = 2;
            break;
        case NeutrinoSelectionCriterion::chi2:
            getTotalChi2(1) < getTotalChi2(2) ? currentSelectedNeutrino = 1 : currentSelectedNeutrino = 2;
            break;

        case NeutrinoSelectionCriterion::pzClosestToLepton:
            fabs(neutrino1->pz() - electronFromW->pz()) < fabs(neutrino2->pz()
                    - electronFromW->pz()) ? currentSelectedNeutrino = 1 : currentSelectedNeutrino = 2;
            break;

        case NeutrinoSelectionCriterion::mostCentral:
            fabs(neutrino1->pz()) < fabs(neutrino2->pz()) ? currentSelectedNeutrino = 1 : currentSelectedNeutrino = 2;
            break;

        case NeutrinoSelectionCriterion::pzClosestToLeptonOrMostcentralIfAbove300:
            fabs(neutrino1->pz() - electronFromW->pz()) < fabs(neutrino2->pz()
                    - electronFromW->pz()) ? currentSelectedNeutrino = 1 : currentSelectedNeutrino = 2;
            if (fabs(neutrino1->pz()) > 300 || fabs(neutrino2->pz()) > 300)
                fabs(neutrino1->pz()) < fabs(neutrino2->pz()) ? currentSelectedNeutrino = 1 : currentSelectedNeutrino
                        = 2;
            break;

        case NeutrinoSelectionCriterion::largestValueOfCosine:
            TVector3 p3W, p3e;
            //TODO clean up
            p3W = leptonicW1->getFourVector().Vect();
            p3e = electronFromW->getFourVector().Vect();

            double sinthcm1 = 2. * (p3e.Perp(p3W)) / W_mass;
            p3W = leptonicW2->getFourVector().Vect();
            double sinthcm2 = 2. * (p3e.Perp(p3W)) / W_mass;

            double costhcm1 = TMath::Sqrt(1. - sinthcm1 * sinthcm1);
            double costhcm2 = TMath::Sqrt(1. - sinthcm2 * sinthcm2);
            costhcm1 > costhcm2 ? currentSelectedNeutrino = 1 : currentSelectedNeutrino = 2;
            break;

        }
    }

}

double TopPairEventCandidate::calculateTopMassDifference(unsigned short int neutrinoSolution) const {

  double LeptonicTop1MassDifference = fabs(leptonicTop1->mass()-hadronicTop->mass());
  double LeptonicTop2MassDifference = fabs(leptonicTop2->mass()-hadronicTop->mass());

  if (neutrinoSolution == 1)
    return LeptonicTop1MassDifference;
  else
    return LeptonicTop2MassDifference;

}

double TopPairEventCandidate::getTotalChi2() {
    double totalChi2(9999999);
    double firstTotalChi2 = getTotalChi2(1);
    double secondTotalChi2 = getTotalChi2(2);
    selectedNeutrino == 1 ? totalChi2 = firstTotalChi2 : totalChi2 = secondTotalChi2;
    //    if (firstTotalChi2 < secondTotalChi2) {
    //        selectedNeutrino = 1;
    //        totalChi2 = firstTotalChi2;
    //    } else {
    //        selectedNeutrino = 2;
    //        totalChi2 = secondTotalChi2;
    //    }
    return totalChi2;
}

double TopPairEventCandidate::getTotalChi2(unsigned short int neutrinoSolution) const {
    return getLeptonicChi2(neutrinoSolution) + getHadronicChi2() + getGlobalChi2(neutrinoSolution);
}

double TopPairEventCandidate::getLeptonicChi2(unsigned short int neutrinoSolution) const {
    double topMass(0);
    double angle = leptonicBJet->angle(electronFromW);
    if (neutrinoSolution == 1)
        topMass = leptonicTop1->mass();
    else
        topMass = leptonicTop2->mass();

    return getLeptonicChi2(topMass, angle);
}

double TopPairEventCandidate::getLeptonicChi2(double topMass, double angle) const {
    double massDifference = TMath::Power(topMass - matched_leptonic_top_mass, 2);
    double massError = 2 * matched_leptonic_top_mass_sigma * matched_leptonic_top_mass_sigma;
    double massTerm = massDifference / massError;

    double angleDifference = TMath::Power(angle - matched_angle, 2);
    double angleError = 2 * matched_angle_sigma * matched_angle_sigma;
    double angleTerm = angleDifference / angleError;
    return 1 / sqrt(2) * (angleTerm + massTerm);
}

double TopPairEventCandidate::getHadronicChi2() const {
    double ptRatioDifference = TMath::Power(PtRatio() - matched_ptratio, 2);
    double ptRatioError = 2 * matched_ptratio_sigma * matched_ptratio_sigma;
    double ptRatioTerm = ptRatioDifference / ptRatioError;

    double WmassDifference = TMath::Power(hadronicW->mass() - matched_hadronic_W_mass, 2);
    double WmassError = 2 * matched_hadronic_W_mass_sigma * matched_hadronic_W_mass_sigma;
    double WmassTerm = WmassDifference / WmassError;

    double topMassDifference = TMath::Power(hadronicTop->mass() - matched_hadronic_top_mass, 2);
    double topMassError = 2 * matched_hadronic_top_mass_sigma * matched_hadronic_top_mass_sigma;
    double topMassTerm = topMassDifference / topMassError;
    return 1 / sqrt(3) * (topMassTerm + WmassTerm + ptRatioTerm);
    return 0;
}

double TopPairEventCandidate::PtRatio() const {
    return TMath::Log(hadronicTop->pt() / hadronicW->pt());
}

double TopPairEventCandidate::getGlobalChi2(unsigned short neutrinoSolution) const {
    double pttbar = PtTtbarSystem(neutrinoSolution);
    double pttbarDifference = TMath::Power(pttbar - matched_pt_ttbarSystem, 2);
    double pttbarError = (2 * matched_pt_ttbarSystem_sigma * matched_pt_ttbarSystem_sigma);
    double pttbarTerm = pttbarDifference / pttbarError;

    double htSystemDifference = TMath::Power(HTSystem() - matched_HTSystem, 2);
    double htSystemError = matched_HTSystem_sigma * matched_HTSystem_sigma * 2;
    double htSystemTerm = htSystemDifference / htSystemError;
    return 1 / sqrt(2) * (pttbarTerm + htSystemTerm);
}

double TopPairEventCandidate::PtTtbarSystem(unsigned short neutrinoSolution) const {
    ParticlePointer combined;
    if (neutrinoSolution == 1)
        combined = ParticlePointer(new Particle(*leptonicTop1 + *hadronicTop));
    else
        combined = ParticlePointer(new Particle(*leptonicTop2 + *hadronicTop));
    return combined->pt();
}

double TopPairEventCandidate::HT(unsigned short jetLimit) const {
    double HT(0);
    unsigned short limit = goodJets.size();
    if (limit > jetLimit + 1)
        limit = jetLimit + 1;

    for (unsigned short index = 0; index < limit; ++index)
        HT += goodJets.at(index)->pt();

    return HT;
}

double TopPairEventCandidate::HTSystem() const {
    return sumPt() / HT(8);
}

double TopPairEventCandidate::sumPt() const {
    return leptonicBJet->pt() + hadronicBJet->pt() + jet1FromW->pt() + jet2FromW->pt();
}

void TopPairEventCandidate::throwExpeptionIfNotReconstructed() const {
    if (doneReconstruction == false)
        throw ReconstructionException("Can't access reconstructed particles before reconstruction.");
}

const ElectronPointer TopPairEventCandidate::getElectronFromWDecay() const {
    return electronFromW;
}

const ParticlePointer TopPairEventCandidate::getNeutrinoFromWDecay() const {
    throwExpeptionIfNotReconstructed();
    if (selectedNeutrino == 1)
        return neutrino1;
    else
        return neutrino2;
}

const JetPointer TopPairEventCandidate::getHadronicBJet() const {
    throwExpeptionIfNotReconstructed();
    return hadronicBJet;
}

const JetPointer TopPairEventCandidate::getLeptonicBJet() const {
    throwExpeptionIfNotReconstructed();
    return leptonicBJet;
}

const JetPointer TopPairEventCandidate::getJet1FromHadronicW() const {
    throwExpeptionIfNotReconstructed();
    return jet1FromW;
}

const JetPointer TopPairEventCandidate::getJet2FromHadronicW() const {
    throwExpeptionIfNotReconstructed();
    return jet2FromW;
}

const ParticlePointer TopPairEventCandidate::getLeptonicTop() const {
    throwExpeptionIfNotReconstructed();
    if (selectedNeutrino == 1)
        return leptonicTop1;
    else
        return leptonicTop2;
}

const ParticlePointer TopPairEventCandidate::getHadronicTop() const {
    throwExpeptionIfNotReconstructed();
    return hadronicTop;
}

const ParticlePointer TopPairEventCandidate::getResonance() const {
    throwExpeptionIfNotReconstructed();
    return ttbarResonance;
}

double TopPairEventCandidate::M3() const {
    double m3(0), max_pt(0);
    if (goodJets.size() >= 3) {
        for (unsigned int index1 = 0; index1 < goodJets.size() - 2; ++index1) {
            for (unsigned int index2 = index1 + 1; index2 < goodJets.size() - 1; ++index2) {
                for (unsigned int index3 = index2 + 1; index3 < goodJets.size(); ++index3) {
                    FourVector m3Vector(goodJets.at(index1)->getFourVector() + goodJets.at(index2)->getFourVector()
                            + goodJets.at(index3)->getFourVector());
                    double currentPt = m3Vector.Pt();
                    if (currentPt > max_pt) {
                        max_pt = currentPt;
                        m3 = m3Vector.M();
                    }
                }
            }
        }
    }

    return m3;
}

double TopPairEventCandidate::mttbar() const {
    return getResonance()->mass();
}

const std::vector<TtbarHypothesisPointer>& TopPairEventCandidate::Solutions() const{
	return solutions;
}

void TopPairEventCandidate::inspectReconstructedEvent() const {
    cout << "run " << runNumber << ", event " << eventNumber << endl;
    cout << "leptonic b jet, goodJet index " << leptonicBIndex << endl;
    EventContentPrinter::printJet(leptonicBJet);

    cout << "electron from W" << endl;
    EventContentPrinter::printElectron(electronFromW);

    cout << "MET" << endl;
    EventContentPrinter::printParticle(met);
    cout << endl;

    cout << "reconstructed neutrino 1(selected: " << selectedNeutrino << ")" << endl;
    EventContentPrinter::printParticle(neutrino1);
    cout << endl;

    cout << "reconstructed neutrino 2(selected: " << selectedNeutrino << ")" << endl;
    EventContentPrinter::printParticle(neutrino2);
    cout << endl;

    cout << "leptonic W 1 (selected: " << selectedNeutrino << ")" << endl;
    EventContentPrinter::printParticle(leptonicW1);
    cout << endl;

    cout << "leptonic W 2 (selected: " << selectedNeutrino << ")" << endl;
    EventContentPrinter::printParticle(leptonicW2);
    cout << endl;

    cout << "leptonic top 1 (selected: " << selectedNeutrino << ")" << endl;
    EventContentPrinter::printParticle(leptonicTop1);
    cout << endl;

    cout << "leptonic top 2 (selected: " << selectedNeutrino << ")" << endl;
    EventContentPrinter::printParticle(leptonicTop2);
    cout << endl;

    cout << "hadronic b jet, goodJet index " << hadronicBIndex << endl;
    EventContentPrinter::printJet(hadronicBJet);

    cout << "jet1 from W, goodJet index " << jet1FromWIndex << endl;
    EventContentPrinter::printJet(jet1FromW);

    cout << "jet 2 from W, goodJet index " << jet2FromWIndex << endl;
    EventContentPrinter::printJet(jet2FromW);

    cout << "hadronic W" << endl;
    EventContentPrinter::printParticle(hadronicW);
    cout << endl;

    cout << "hadronic top" << endl;
    EventContentPrinter::printParticle(hadronicTop);
    cout << endl;
}

double TopPairEventCandidate::fullHT() const {
    double ht(met->pt());

    for (unsigned int index = 0; index < goodIsolatedElectrons.size(); ++index) {
        ht += goodIsolatedElectrons.at(index)->pt();
    }

    for (unsigned int index = 0; index < goodIsolatedMuons.size(); ++index) {
        ht += goodIsolatedMuons.at(index)->pt();
    }

    for (unsigned int index = 0; index < goodJets.size(); ++index) {
        ht += goodJets.at(index)->pt();
    }
    return ht;
}

double TopPairEventCandidate::transverseWmass(const ElectronPointer electron) const {
    double energySquared = pow(electron->et() + met->et(), 2);
    double momentumSquared = pow(electron->px() + met->px(), 2) + pow(electron->py() + met->py(), 2);
    double tMassSquared = energySquared - momentumSquared;

    if (tMassSquared > 0)
        return sqrt(tMassSquared);
    else
        return -1;
}

}
