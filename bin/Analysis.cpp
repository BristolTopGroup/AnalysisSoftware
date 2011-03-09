/*
 * Analysis.cpp
 *
 *  Created on: 12 Jul 2010
 *      Author: kreczko
 */

#include "Analysis.h"
#include "TROOT.h"
#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <boost/array.hpp>
#include "../interface/EventCounter.h"
#include <cmath>
#include <math.h>
#include "../interface/Printers/EventTablePrinter.h"

using namespace BAT;
using namespace std;
float Analysis::luminosity = 36.145;

void Analysis::analyze() {
    createHistograms();
    cout << "detected samples:" << endl;
    for (unsigned int sample = 0; sample < DataType::NUMBER_OF_DATA_TYPES; ++sample) {
        if (eventReader->getSeenDatatypes()[sample])
            cout << DataType::names[sample] << endl;
    }
    while (eventReader->hasNextEvent()) {
        initiateEvent();
        printNumberOfProccessedEventsEvery(100000);
        inspectEvents();
        //        doEcalSpikeAnalysis();
        //                doSynchExercise();
//        if (currentEvent.GoodElectrons().size() > 0) {
//            cout << "PFIsolation: " << currentEvent.GoodElectrons().front()->pfIsolation();
//            cout << " RelIso: " << currentEvent.GoodElectrons().front()->relativeIsolation() << endl;
//        }
        doTTbarCutFlow();
        doDiElectronAnalysis();
        doTTBarAnalysis();
        doNotePlots();
        doQCDStudy();
//        if(currentEvent.getDataType() == DataType::DATA)
//            eventCheck[currentEvent.runnumber()].push_back(currentEvent.eventnumber());
//        checkForBrokenEvents();
    }
//    checkForDuplicatedEvents();
    printInterestingEvents();
    printSummary();
}

void Analysis::printNumberOfProccessedEventsEvery(unsigned long printEvery) {
    unsigned long eventIndex = eventReader->getNumberOfProccessedEvents();
    if (eventIndex % printEvery == 0 || eventIndex == 1) {
        cout << "Analysing event no " << eventIndex << ", sample: " << DataType::names[currentEvent.getDataType()]
                << endl;
        cout << "File: " << eventReader->getCurrentFile() << endl;
    }

}

void Analysis::initiateEvent() {
    currentEvent = eventReader->getNextEvent();
    ttbarCandidate = TopPairEventCandidate(currentEvent);
    weight = weights.getWeight(currentEvent.getDataType());
    histMan.setCurrentDataType(ttbarCandidate.getDataType());
    histMan.setCurrentJetBin(currentEvent.GoodJets().size());
    histMan.setCurrentBJetBin(currentEvent.GoodBJets().size());
}

void Analysis::inspectEvents() {
    std::vector<InterestingEvent> eventsToInspect;

    for (unsigned int index = 0; index < eventsToInspect.size(); ++index) {
        if ((ttbarCandidate.runnumber() == eventsToInspect.at(index).runNumber && ttbarCandidate.eventnumber()
                == eventsToInspect.at(index).eventNumber)) {
            cout << "file: " << eventReader->getCurrentFile() << endl;
            ttbarCandidate.inspect();
        }
    }

}

void Analysis::doSynchExercise() {
    if (ttbarCandidate.passesSelectionStepUpTo(TTbarEPlusJetsSelection::ConversionFinder)) {
        cout << ttbarCandidate.runnumber() << ":" << ttbarCandidate.eventnumber() << ":" << endl;//electron->et() << endl;
        if (ttbarCandidate.eventnumber() == 450622) {
            ttbarCandidate.inspect();
        }
    }
}

void Analysis::doTTbarCutFlow() {
    for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
        if (ttbarCandidate.passesSelectionStep((TTbarEPlusJetsSelection::Step) cut)) {
            ++singleCuts[cut];
            singleCutsPerFile[eventReader->getCurrentFile()][cut]++;
        }

        if (ttbarCandidate.passesSelectionStepUpTo((TTbarEPlusJetsSelection::Step) cut)) {
            cutflow[cut] += 1;
            cutflowPerFile[eventReader->getCurrentFile()][cut]++;
            unsigned int njet = ttbarCandidate.GoodJets().size();
            if (njet >= JetBin::NUMBER_OF_JET_BINS)
                njet = JetBin::NUMBER_OF_JET_BINS - 1;
            cutflowPerSample.increase(ttbarCandidate.getDataType(), cut, njet, weight);
        }
    }
}
void Analysis::doDiElectronAnalysis() {
    ElectronCollection electrons = currentEvent.GoodElectrons();
    if (electrons.size() == 2) {
        ElectronPointer leadingElectron = electrons.front();
        ElectronPointer secondElectron = electrons.at(1);
        histMan.H1D_JetBinned("diElectronMass")->Fill(leadingElectron->invariantMass(secondElectron), weight);
    }
}

void Analysis::doTTBarAnalysis() {
    if (ttbarCandidate.passesNMinus1(TTbarEPlusJetsSelection::AtLeastFourGoodJets)) {
        histMan.H1D("numberOfJets")->Fill(ttbarCandidate.GoodJets().size());
    }

    if (ttbarCandidate.passesSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex)
            && ttbarCandidate.hasNoIsolatedMuon() && ttbarCandidate.isNotAZBosonEvent()
            && ttbarCandidate.hasAtLeastFourGoodJets()) {
        const ElectronCollection coll = ttbarCandidate.Electrons();
        for (unsigned int index = 0; index < coll.size(); ++index) {
            const ElectronPointer electron = coll.at(index);
            bool passesEt = electron->et() > 30;
            bool passesEta = fabs(electron->superClusterEta()) < 2.5
                    && electron->isInCrack() == false;
            bool passesID = electron->VBTF_W70_ElectronID();
            //            bool isNotEcalSpike = electron->isEcalSpike() == false;
            bool noConversion = electron->isFromConversion() == false;
            if (passesEt && passesEta && passesID && noConversion) {
                histMan.H1D("electronD0")->Fill(electron->d0(), weight);
            }
        }
    }
    if (ttbarCandidate.passesFullTTbarEPlusJetSelection()) {
        histMan.H1D("numberOfBJets")->Fill(ttbarCandidate.GoodBJets().size(), weight);
        if(Event::usePFIsolation)
            ttbarCandidate.reconstructUsingChi2(ttbarCandidate.GoodPFIsolatedElectrons().front());
        else
            ttbarCandidate.reconstructUsingChi2(ttbarCandidate.GoodIsolatedElectrons().front());

        vector<TtbarHypothesisPointer> solutions = ttbarCandidate.Solutions();
        const ParticlePointer resonance = ttbarCandidate.getResonance();
        double mttbar = ttbarCandidate.mttbar();

        ParticlePointer leadingTop, nextToLeadingTop;

        if (ttbarCandidate.getHadronicTop()->pt() > ttbarCandidate.getLeptonicTop()->pt()) {
            leadingTop = ttbarCandidate.getHadronicTop();
            nextToLeadingTop = ttbarCandidate.getLeptonicTop();
        } else {
            leadingTop = ttbarCandidate.getLeptonicTop();
            nextToLeadingTop = ttbarCandidate.getHadronicTop();
        }
        double angleTops = leadingTop->angle(nextToLeadingTop);
        histMan.H1D_BJetBinned("angleTops")->Fill(angleTops, weight);
        histMan.H2D_BJetBinned("angleTops_vs_mttbar")->Fill(mttbar, angleTops, weight);
        histMan.H1D_BJetBinned("mLeptonicTop")->Fill(ttbarCandidate.getLeptonicTop()->mass(), weight);
        histMan.H1D_BJetBinned("mHadronicTop")->Fill(ttbarCandidate.getHadronicTop()->mass(), weight);
        histMan.H1D_BJetBinned("mAllTop")->Fill(ttbarCandidate.getLeptonicTop()->mass(), weight);
        histMan.H1D_BJetBinned("mAllTop")->Fill(ttbarCandidate.getHadronicTop()->mass(), weight);

        histMan.H1D_BJetBinned("MET")->Fill(ttbarCandidate.MET()->et(), weight);
        histMan.H2D_BJetBinned("METvsMttbar")->Fill(mttbar, ttbarCandidate.MET()->et(), weight);
        histMan.H1D_BJetBinned("HT")->Fill(ttbarCandidate.fullHT(), weight);
        histMan.H2D_BJetBinned("HTvsMttbar")->Fill(mttbar, ttbarCandidate.fullHT(), weight);
        histMan.H1D_BJetBinned("leadingJetMass")->Fill(ttbarCandidate.GoodJets().front()->mass(), weight);
        if (Event::usePFIsolation)
            histMan.H1D_BJetBinned("mtW")->Fill(ttbarCandidate.transverseWmass(
                    ttbarCandidate.GoodPFIsolatedElectrons().front()), weight);
        else
            histMan.H1D_BJetBinned("mtW")->Fill(ttbarCandidate.transverseWmass(
                    ttbarCandidate.GoodIsolatedElectrons().front()), weight);
        histMan.H1D_BJetBinned("m3")->Fill(ttbarCandidate.M3(), weight);


//        histMan.H1D("mttbar")->Fill(mttbar, weight);
        histMan.H1D_BJetBinned("mttbar")->Fill(mttbar, weight);

        histMan.H1D_BJetBinned("ttbar_pt")->Fill(resonance->pt(), weight);

        histMan.H1D_BJetBinned("ttbar_px")->Fill(resonance->px(), weight);
        histMan.H1D_BJetBinned("ttbar_py")->Fill(resonance->py(), weight);
        histMan.H1D_BJetBinned("ttbar_pz")->Fill(resonance->pz(), weight);

        histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar")->Fill(mttbar, resonance->pt(), weight);


        histMan.H1D("electron_et")->Fill(ttbarCandidate.getElectronFromWDecay()->et(), weight);
        histMan.H1D_BJetBinned("neutrino_pz")->Fill(ttbarCandidate.getNeutrinoFromWDecay()->pz(), weight);

        histMan.H1D_BJetBinned("pt_leadingTop")->Fill(leadingTop->pt(), weight);
        histMan.H1D_BJetBinned("pt_NextToLeadingTop")->Fill(nextToLeadingTop->pt(), weight);
        histMan.H2D_BJetBinned("pt_leadingTop_vs_mttbar")->Fill(mttbar, leadingTop->pt(), weight);
        histMan.H2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar")->Fill(mttbar, nextToLeadingTop->pt(), weight);

        if (ttbarCandidate.MET()->pt() > 20) {
            histMan.H1D_BJetBinned("angleTops_withMETCut")->Fill(angleTops, weight);
            histMan.H2D_BJetBinned("angleTops_vs_mttbar_withMETCut")->Fill(mttbar, angleTops, weight);
            histMan.H1D_BJetBinned("mttbar_withMETCut")->Fill(mttbar, weight);
            histMan.H1D_BJetBinned("ttbar_pt_withMETCut")->Fill(resonance->pt(), weight);
            histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_withMETCut")->Fill(mttbar, resonance->pt(), weight);

            histMan.H1D_BJetBinned("pt_leadingTop_withMETCut")->Fill(leadingTop->pt(), weight);
            histMan.H1D_BJetBinned("pt_NextToLeadingTop_withMETCut")->Fill(nextToLeadingTop->pt(), weight);
            histMan.H2D_BJetBinned("pt_leadingTop_vs_mttbar_withMETCut")->Fill(mttbar, leadingTop->pt(), weight);
            histMan.H2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withMETCut")->Fill(mttbar, nextToLeadingTop->pt(), weight);

            if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                histMan.H1D_BJetBinned("angleTops_withMETAndAsymJets")->Fill(angleTops, weight);
                histMan.H2D_BJetBinned("angleTops_vs_mttbar_withMETAndAsymJets")->Fill(mttbar, angleTops, weight);
                histMan.H1D_BJetBinned("mttbar_withMETAndAsymJets")->Fill(mttbar, weight);
                histMan.H1D_BJetBinned("ttbar_pt_withMETAndAsymJets")->Fill(resonance->pt(), weight);

                histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_withMETAndAsymJets")->Fill(mttbar, resonance->pt(), weight);

                histMan.H1D_BJetBinned("pt_leadingTop_withMETAndAsymJets")->Fill(leadingTop->pt(), weight);
                histMan.H1D_BJetBinned("pt_NextToLeadingTop_withMETAndAsymJets")->Fill(nextToLeadingTop->pt(), weight);
                histMan.H2D_BJetBinned("pt_leadingTop_vs_mttbar_withMETAndAsymJets")->Fill(mttbar, leadingTop->pt(), weight);
                histMan.H2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withMETAndAsymJets")->Fill(mttbar, nextToLeadingTop->pt(), weight);
            }
        }

        if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
            histMan.H1D_BJetBinned("angleTops_withAsymJetsCut")->Fill(angleTops, weight);
            histMan.H2D_BJetBinned("angleTops_vs_mttbar_withAsymJetsCut")->Fill(mttbar, angleTops, weight);
            histMan.H1D_BJetBinned("mttbar_withAsymJetsCut")->Fill(mttbar, weight);
            histMan.H1D_BJetBinned("ttbar_pt_withAsymJetsCut")->Fill(resonance->pt(), weight);

            histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_withAsymJetsCut")->Fill(mttbar, resonance->pt(), weight);

            histMan.H1D_BJetBinned("pt_leadingTop_withAsymJetsCut")->Fill(leadingTop->pt(), weight);
            histMan.H1D_BJetBinned("pt_NextToLeadingTop_withAsymJetsCut")->Fill(nextToLeadingTop->pt(), weight);
            histMan.H2D_BJetBinned("pt_leadingTop_vs_mttbar_withAsymJetsCut")->Fill(mttbar, leadingTop->pt(), weight);
            histMan.H2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withAsymJetsCut")->Fill(mttbar, nextToLeadingTop->pt(), weight);
        }

        for (unsigned int solutionIndex = 0; solutionIndex < solutions.size(); ++solutionIndex) {
            histMan.H1D_BJetBinned("mttbar_allSolutions")->Fill(solutions.at(solutionIndex)->resonance->mass(), weight);
            histMan.H1D_BJetBinned("ttbar_pt_allSolutions")->Fill(solutions.at(solutionIndex)->resonance->pt(), weight);
            histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions")->Fill(
                    solutions.at(solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(),
                    weight);
            if (solutionIndex == 1) {
                histMan.H1D_BJetBinned("mttbar_2ndSolution")->Fill(solutions.at(solutionIndex)->resonance->mass(),
                        weight);

                histMan.H1D_BJetBinned("ttbar_pt_2ndSolution")->Fill(solutions.at(solutionIndex)->resonance->pt(),
                        weight);

                histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_2ndSolution")->Fill(
                        solutions.at(solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(),
                        weight);

            }
            if (solutionIndex == 2) {
                histMan.H1D_BJetBinned("mttbar_3rdSolution")->Fill(solutions.at(solutionIndex)->resonance->mass(),
                        weight);

                histMan.H1D_BJetBinned("ttbar_pt_3rdSolution")->Fill(solutions.at(solutionIndex)->resonance->pt(),
                        weight);

                histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_3rdSolution")->Fill(
                        solutions.at(solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(),
                        weight);
            }

            if (ttbarCandidate.MET()->pt() > 20) {
                histMan.H1D_BJetBinned("mttbar_allSolutions_withMETCut")->Fill(
                        solutions.at(solutionIndex)->resonance->mass(), weight);
                histMan.H1D_BJetBinned("ttbar_pt_allSolutions_withMETCut")->Fill(
                        solutions.at(solutionIndex)->resonance->pt(), weight);
                histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withMETCut")->Fill(
                        solutions.at(solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(),
                        weight);
                if (solutionIndex == 1) {
                    histMan.H1D_BJetBinned("mttbar_2ndSolution_withMETCut")->Fill(
                            solutions.at(solutionIndex)->resonance->mass(), weight);
                    histMan.H1D_BJetBinned("ttbar_pt_2ndSolution_withMETCut")->Fill(
                            solutions.at(solutionIndex)->resonance->pt(), weight);
                    histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_2ndSolution_withMETCut")->Fill(solutions.at(
                            solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);
                }
                if (solutionIndex == 2) {
                    histMan.H1D_BJetBinned("mttbar_3rdSolution_withMETCut")->Fill(
                            solutions.at(solutionIndex)->resonance->mass(), weight);
                    histMan.H1D_BJetBinned("ttbar_pt_3rdSolution_withMETCut")->Fill(
                            solutions.at(solutionIndex)->resonance->pt(), weight);
                    histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_3rdSolution_withMETCut")->Fill(solutions.at(
                            solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);
                }

                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                    histMan.H1D_BJetBinned("mttbar_allSolutions_withMETAndAsymJets")->Fill(
                            solutions.at(solutionIndex)->resonance->mass(), weight);
                    histMan.H1D_BJetBinned("ttbar_pt_allSolutions_withMETAndAsymJets")->Fill(
                            solutions.at(solutionIndex)->resonance->pt(), weight);
                    histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withMETAndAsymJets")->Fill(solutions.at(
                            solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);
                }
            }

            if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                histMan.H1D_BJetBinned("mttbar_allSolutions_withAsymJetsCut")->Fill(
                        solutions.at(solutionIndex)->resonance->mass(), weight);
                histMan.H1D_BJetBinned("ttbar_pt_allSolutions_withAsymJetsCut")->Fill(
                        solutions.at(solutionIndex)->resonance->pt(), weight);
                histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withAsymJetsCut")->Fill(solutions.at(
                        solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);

                if (ttbarCandidate.MET()->et() > 20) {

                    if (solutionIndex == 1) {
                        histMan.H1D_BJetBinned("mttbar_2ndSolution_withMETAndAsymJets")->Fill(solutions.at(
                                solutionIndex)->resonance->mass(), weight);

                        histMan.H1D_BJetBinned("ttbar_pt_2ndSolution_withMETAndAsymJets")->Fill(solutions.at(
                                solutionIndex)->resonance->pt(), weight);
                        histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_2ndSolution_withMETAndAsymJets")->Fill(solutions.at(
                                solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);
                    }

                    if (solutionIndex == 2) {
                        histMan.H1D_BJetBinned("mttbar_3rdSolution_withMETAndAsymJets")->Fill(solutions.at(
                                solutionIndex)->resonance->mass(), weight);

                        histMan.H1D_BJetBinned("ttbar_pt_3rdSolution_withMETAndAsymJets")->Fill(solutions.at(
                                solutionIndex)->resonance->pt(), weight);
                        histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_3rdSolution_withMETAndAsymJets")->Fill(solutions.at(
                                solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);
                    }
                }

                if (solutionIndex == 1) {
                    histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_2ndSolution_withAsymJetsCut")->Fill(solutions.at(
                            solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);

                    histMan.H1D_BJetBinned("mttbar_2ndSolution_withAsymJetsCut")->Fill(
                            solutions.at(solutionIndex)->resonance->mass(), weight);
                    histMan.H1D_BJetBinned("mttbar_2ndSolution_withAsymJetsCut")->Fill(
                            solutions.at(solutionIndex)->resonance->mass(), weight);
                }

                if (solutionIndex == 2) {
                    histMan.H2D_BJetBinned("ttbar_pt_vs_mttbar_3rdSolution_withAsymJetsCut")->Fill(solutions.at(
                            solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);
                    histMan.H1D_BJetBinned("ttbar_pt_3rdSolution_withAsymJetsCut")->Fill(
                            solutions.at(solutionIndex)->resonance->pt(), weight);

                    histMan.H1D_BJetBinned("ttbar_pt_3rdSolution_withAsymJetsCut")->Fill(
                            solutions.at(solutionIndex)->resonance->pt(), weight);
                }

            }

            //            cout << "total Chi2 = " << solutions.at(solutionIndex)->totalChi2;
            //            cout << ", mass = " << solutions.at(solutionIndex)->resonance->mass() << endl;
        }

        if (ttbarCandidate.MET()->et() < 20) {
            histMan.H1D_BJetBinned("mttbar_QCDEnriched")->Fill(mttbar, weight);
            histMan.H1D_BJetBinned("ttbar_pt_QCDEnriched")->Fill(resonance->pt());
        }



//        cout << "Number of solutions: " << solutions.size() << " (" << ttbarCandidate.GoodJets().size() << " jets)"
//                << endl;
//        cout << "First solution, compare to chosen one" << endl;
//        cout << solutions.front()->totalChi2 << ", " << ttbarCandidate.getTotalChi2() << endl;

        if (mttbar != mttbar) {//isnan
            ttbarCandidate.inspectReconstructedEvent();
        }
        if (ttbarCandidate.isRealData()) {
            cout << "run " << ttbarCandidate.runnumber() << ", event " << ttbarCandidate.eventnumber() << ", lumi "
                    << ttbarCandidate.lumiblock();
            cout << ", top pair invariant mass = " << mttbar << " GeV" << endl;
            interestingEvents.push_back(InterestingEvent(ttbarCandidate, eventReader->getCurrentFile()));

            if (resonance->pt() > 100) {
                cout << "top pair pt = " << resonance->pt() << " GeV" << endl;
                ttbarCandidate.inspect();
                ttbarCandidate.inspectReconstructedEvent();
            }
        }

    }
}

void Analysis::doNotePlots() {
    if (ttbarCandidate.GoodElectrons().size() >= 1 && ttbarCandidate.Jets().size() >= 2) {
        const ElectronCollection electrons = ttbarCandidate.GoodElectrons();
        ElectronCollection nonConversionElectrons;
        for (unsigned int index = 0; index < electrons.size(); ++index) {
            const ElectronPointer electron = electrons.at(index);
            if (electron->isFromConversion() == false && electron->isTaggedAsConversion(0.02,0.02) == false) {
//                ConversionTagger tagger = ConversionTagger();
//                tagger.calculateConversionVariables(electron, ttbarCandidate.Tracks(), 3.8, 0.45);
//                if (tagger.isFromConversion(0.02, 0.02) == false)
                    nonConversionElectrons.push_back(electron);
            }
        }
        if (nonConversionElectrons.size() == 1) {
            const ElectronPointer electron = nonConversionElectrons.front();
            JetCollection goodjets;
            for (unsigned index = 0; index < ttbarCandidate.Jets().size(); ++index) {
                if (ttbarCandidate.Jets().at(index)->isGood())
                    goodjets.push_back(ttbarCandidate.Jets().at(index));
            }
            if (goodjets.size() >= 2) {
                unsigned int closestID = electron->getClosestJetID(goodjets);
                float minDR = electron->deltaR(goodjets.at(closestID));
                float ptRel = electron->relativePtTo(goodjets.at(closestID));
                histMan.H2D("ptRel_vs_DRmin")->Fill(minDR, ptRel, weight);
                if (ttbarCandidate.MET()->et() < 20 && ttbarCandidate.transverseWmass(electron) < 35) {
                    histMan.H1D("DRmin_QCDenriched")->Fill(minDR, weight);
                    histMan.H1D("ptRel_QCDenriched")->Fill(ptRel, weight);
                } else if (ttbarCandidate.MET()->et() > 30 && ttbarCandidate.transverseWmass(electron) > 50) {
                    histMan.H1D("DRmin_WZenriched")->Fill(minDR, weight);
                    histMan.H1D("ptRel_WZenriched")->Fill(ptRel, weight);
                }
            }
        }

    }
}

void Analysis::doQCDStudy() {
    if (ttbarCandidate.passesRelIsoSelection()) {
        const ElectronPointer electron = ttbarCandidate.MostIsolatedElectron();
        histMan.H1D_JetBinned("QCDest_CombRelIso")->Fill(electron->relativeIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() >= 1)
            histMan.H1D_JetBinned("QCDest_CombRelIso_1btag")->Fill(electron->relativeIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() >= 2)
            histMan.H1D_JetBinned("QCDest_CombRelIso_2btag")->Fill(electron->relativeIsolation(), weight);
    }

    if (ttbarCandidate.passesPFIsoSelection() && NTupleEventReader::electronAlgorithm
            == ElectronAlgorithm::ParticleFlow) {
        const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron();
        histMan.H1D_JetBinned("QCDest_PFIsolation")->Fill(electron->pfIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() >= 1)
            histMan.H1D_JetBinned("QCDest_PFIsolation_1btag")->Fill(electron->pfIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() >= 2)
            histMan.H1D_JetBinned("QCDest_PFIsolation_2btag")->Fill(electron->pfIsolation(), weight);

        if (ttbarCandidate.MET()->pt() > 20) {
            histMan.H1D_JetBinned("QCDest_PFIsolation_WithMETCut")->Fill(electron->pfIsolation(), weight);

            if (ttbarCandidate.GoodJets().size() >= 2) {
                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                    histMan.H1D_JetBinned("QCDest_PFIsolation_WithMETCutAndAsymJetCuts")->Fill(electron->pfIsolation(),
                            weight);
                }
            }
        }

        if (ttbarCandidate.GoodJets().size() >= 2) {
            if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                histMan.H1D_JetBinned("QCDest_PFIsolation_WithAsymJetCuts")->Fill(electron->pfIsolation(), weight);

            }
        }
    }


    if (ttbarCandidate.passesRelIsoControlSelection()) {
        const ElectronPointer electron = ttbarCandidate.MostIsolatedElectron();
        histMan.H1D_JetBinned("QCDest_CombRelIso_controlRegion")->Fill(electron->relativeIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() >= 1)
            histMan.H1D_JetBinned("QCDest_CombRelIso_controlRegion_1btag")->Fill(electron->relativeIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() >= 2)
            histMan.H1D_JetBinned("QCDest_CombRelIso_controlRegion_2btag")->Fill(electron->relativeIsolation(), weight);

        if (NTupleEventReader::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
            const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron();
            histMan.H1D_JetBinned("QCDest_PFIsolation_controlRegion2")->Fill(electron->pfIsolation(), weight);

            if (ttbarCandidate.GoodBJets().size() >= 1)
                histMan.H1D_JetBinned("QCDest_PFIsolation_controlRegion2_1btag")->Fill(electron->pfIsolation(), weight);

            if (ttbarCandidate.GoodBJets().size() >= 2)
                histMan.H1D_JetBinned("QCDest_PFIsolation_controlRegion2_2btag")->Fill(electron->pfIsolation(), weight);

            if (ttbarCandidate.MET()->pt() > 20) {
                histMan.H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCut")->Fill(electron->pfIsolation(), weight);

                if (ttbarCandidate.GoodJets().size() >= 2) {
                    if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                        histMan.H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCutAndAsymJetCuts")->Fill(
                                electron->pfIsolation(), weight);
                    }
                }
            }

            if (ttbarCandidate.GoodJets().size() >= 2) {
                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                    histMan.H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithAsymJetCuts")->Fill(electron->pfIsolation(), weight);

                }
            }
        }

    }

    if (ttbarCandidate.passesPFIsoControlSelection() && NTupleEventReader::electronAlgorithm
            == ElectronAlgorithm::ParticleFlow) {
        const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron();
        histMan.H1D_JetBinned("QCDest_PFIsolation_controlRegion")->Fill(electron->pfIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() >= 1)
            histMan.H1D_JetBinned("QCDest_PFIsolation_controlRegion_1btag")->Fill(electron->pfIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() >= 2)
            histMan.H1D_JetBinned("QCDest_PFIsolation_controlRegion_2btag")->Fill(electron->pfIsolation(), weight);
    }

    if (ttbarCandidate.passesRelIsoSelection() && ttbarCandidate.hasAtLeastFourGoodJets()) {
        const ElectronPointer electron = ttbarCandidate.MostIsolatedElectron(false);
        if (electron->isIsolated() == false && !isnan(electron->relativeIsolation()) && !isinf(
                electron->relativeIsolation())) {
            try {
                ttbarCandidate.reconstructUsingChi2(electron);
                const ParticlePointer resonance = ttbarCandidate.getResonance();
                histMan.H1D_BJetBinned("mttbar_controlRegion")->Fill(resonance->mass(), weight);
                if (ttbarCandidate.MET()->pt() > 20) {
                    histMan.H1D_BJetBinned("mttbar_controlRegion_withMETCut")->Fill(resonance->mass(), weight);
                    if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
                        histMan.H1D_BJetBinned("mttbar_controlRegion_withMETAndAsymJets")->Fill(resonance->mass(),
                                weight);
                }

                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
                    histMan.H1D_BJetBinned("mttbar_controlRegion_withAsymJetsCut")->Fill(resonance->mass(), weight);

            } catch (ReconstructionException& e) {
                cout << "Could not reconstruct event: " << e.what() << endl;
            }
        }
    }

    if (ttbarCandidate.passesConversionSelection()) {
        ElectronPointer electron;
        if (Event::usePFIsolation)
            electron = currentEvent.GoodPFIsolatedElectrons().front();
        else
            electron = currentEvent.GoodIsolatedElectrons().front();
        try {
            ttbarCandidate.reconstructUsingChi2(electron);
            const ParticlePointer resonance = ttbarCandidate.getResonance();
            histMan.H1D_BJetBinned("mttbar_conversions")->Fill(resonance->mass(), weight);
            if (ttbarCandidate.MET()->pt() > 20) {
                histMan.H1D_BJetBinned("mttbar_conversions_withMETCut")->Fill(resonance->mass(), weight);
                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
                    histMan.H1D_BJetBinned("mttbar_conversions_withMETAndAsymJets")->Fill(resonance->mass(), weight);
            }
            if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
                histMan.H1D_BJetBinned("mttbar_conversions_withAsymJetsCut")->Fill(resonance->mass(), weight);
        } catch (ReconstructionException& e) {
            cout << "Could not reconstruct event: " << e.what() << endl;
        }

    }

    if (ttbarCandidate.Electrons().size() > 0 && ttbarCandidate.GoodPFIsolatedElectrons().size() < 2
            && NTupleEventReader::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
        const ElectronPointer electron = ttbarCandidate.MostIsolatedElectron(true);
        histMan.H1D_JetBinned("MostPFIsolatedElectron_dPhiIn")->Fill(electron->dPhiIn(), weight);
        histMan.H1D_JetBinned("MostPFIsolatedElectron_dEtaIn")->Fill(electron->dEtaIn(), weight);
    }

    if (ttbarCandidate.passesAntiIsolationSelection()) {
        ElectronPointer electron = ttbarCandidate.GoodElectrons().front();
        try {
            ttbarCandidate.reconstructUsingChi2(electron);
            float mttbar = ttbarCandidate.mttbar();
            histMan.H1D_BJetBinned("mttbar_antiIsolated")->Fill(mttbar, weight);
            if (ttbarCandidate.MET()->pt() > 20) {
                histMan.H1D_BJetBinned("mttbar_antiIsolated_withMETCut")->Fill(mttbar, weight);
                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                    histMan.H1D_BJetBinned("mttbar_antiIsolated_withMETAndAsymJets")->Fill(mttbar, weight);
                }
            }
            if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                histMan.H1D_BJetBinned("mttbar_antiIsolated_withAsymJetsCut")->Fill(mttbar, weight);
            }

        } catch (ReconstructionException& e) {
            cout << "Could not reconstruct event: " << e.what() << endl;
        }
    }

}

void Analysis::printInterestingEvents() {
    cout << "Interesting events:" << endl;
    for (unsigned int index = 0; index < interestingEvents.size(); ++index) {
        interestingEvents.at(index).print();
    }
}

void Analysis::printSummary() {
    EventTablePrinter::printCutFlowLatexTable(cutflowPerSample);
    EventTablePrinter::printUnweightedCutFlowLatexTable(cutflowPerSample);

    cout << "total number of processed events: " << eventReader->getNumberOfProccessedEvents() << endl;
    cout << endl;
    for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
        cout << "Selection step '" << TTbarEPlusJetsSelection::StringSteps[cut] << "'" << endl;
        cout << "passed events (single cut): " << singleCuts.at(cut) << endl;
        if (cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS)
            cout << "passed events (up to this cut):" << cutflow.at(cut) << endl;
        else
            cout << "passed events (full selection):" << cutflow.at(cut) << endl;
        cout << endl;
    }

    cout << "number of events without electrons: " << brokenEvents.size() << endl;
}

void Analysis::createHistograms() {
    histMan.setCurrentLumi(Analysis::luminosity);
    histMan.prepareForSeenDataTypes(eventReader->getSeenDatatypes());
    histMan.addH1D("electron_et", "electron_et", 500, 0, 500);
    histMan.addH1D_JetBinned("MostPFIsolatedElectron_dPhiIn", "MostPFIsolatedElectron_dPhiIn", 50, 0, 0.1);
    histMan.addH1D_JetBinned("MostPFIsolatedElectron_dEtaIn", "MostPFIsolatedElectron_dEtaIn", 50, 0, 0.02 );
    histMan.addH1D_JetBinned("diElectronMass", "diElectronMass", 1000, 0, 1000);

    histMan.addH1D_BJetBinned("mttbar_conversions", "mttbar", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_antiIsolated", "mttbar", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_QCDEnriched", "mttbar", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_controlRegion", "mttbar", 5000, 0, 5000);

    histMan.addH1D_BJetBinned("mttbar_conversions_withMETCut", "mttbar", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_antiIsolated_withMETCut", "mttbar", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_controlRegion_withMETCut", "mttbar", 5000, 0, 5000);

    histMan.addH1D_BJetBinned("mttbar_conversions_withMETAndAsymJets", "mttbar", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_antiIsolated_withMETAndAsymJets", "mttbar", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_controlRegion_withMETAndAsymJets", "mttbar", 5000, 0, 5000);

    histMan.addH1D_BJetBinned("mttbar_conversions_withAsymJetsCut", "mttbar", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_antiIsolated_withAsymJetsCut", "mttbar", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_controlRegion_withAsymJetsCut", "mttbar", 5000, 0, 5000);

    histMan.addH1D_BJetBinned("mttbar", "mttbar", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_withMETCut", "mttbar_withMETCut", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_withMETAndAsymJets", "mttbar_withMETAndAsymJets", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_withAsymJetsCut", "mttbar_withAsymJetsCut", 5000, 0, 5000);
//
    histMan.addH1D_BJetBinned("mttbar_2ndSolution", "mttbar_2ndSolution", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_3rdSolution", "mttbar_3rdSolution", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_allSolutions", "mttbar_allSolutions", 5000, 0, 5000);
//
    histMan.addH1D_BJetBinned("mttbar_2ndSolution_withMETCut", "mttbar_2ndSolution", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_3rdSolution_withMETCut", "mttbar_3rdSolution", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_allSolutions_withMETCut", "mttbar_allSolutions", 5000, 0, 5000);

    histMan.addH1D_BJetBinned("mttbar_2ndSolution_withMETAndAsymJets", "mttbar_2ndSolution", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_3rdSolution_withMETAndAsymJets", "mttbar_3rdSolution", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_allSolutions_withMETAndAsymJets", "mttbar_allSolutions", 5000, 0, 5000);

    histMan.addH1D_BJetBinned("mttbar_2ndSolution_withAsymJetsCut", "mttbar_2ndSolution", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_3rdSolution_withAsymJetsCut", "mttbar_3rdSolution", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("mttbar_allSolutions_withAsymJetsCut", "mttbar_allSolutions", 5000, 0, 5000);

    histMan.addH1D_BJetBinned("mLeptonicTop", "mLeptonicTop", 500, 0, 500);
    histMan.addH1D_BJetBinned("mHadronicTop", "mHadronicTop", 500, 0, 500);
    histMan.addH1D_BJetBinned("mAllTop", "mAllTop", 500, 0, 500);
    histMan.addH1D_BJetBinned("m3", "m3", 5000, 0, 5000);
    histMan.addH1D_BJetBinned("ttbar_pt", "ttbar_pt", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_withMETCut", "ttbar_pt", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_withMETAndAsymJets", "ttbar_pt", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_withAsymJetsCut", "ttbar_pt", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_2ndSolution", "ttbar_pt_2ndSolution", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_3rdSolution", "ttbar_pt_3rdSolution", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_allSolutions", "ttbar_pt_allSolutions", 1000, 0, 1000);

    histMan.addH1D_BJetBinned("ttbar_pt_2ndSolution_withMETCut", "ttbar_pt_2ndSolution", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_3rdSolution_withMETCut", "ttbar_pt_3rdSolution", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_allSolutions_withMETCut", "ttbar_pt_allSolutions", 1000, 0, 1000);

    histMan.addH1D_BJetBinned("ttbar_pt_2ndSolution_withMETAndAsymJets", "ttbar_pt_2ndSolution", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_3rdSolution_withMETAndAsymJets", "ttbar_pt_3rdSolution", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_allSolutions_withMETAndAsymJets", "ttbar_pt_allSolutions", 1000, 0, 1000);

    histMan.addH1D_BJetBinned("ttbar_pt_2ndSolution_withAsymJetsCut", "ttbar_pt_2ndSolution", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_3rdSolution_withAsymJetsCut", "ttbar_pt_3rdSolution", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_allSolutions_withAsymJetsCut", "ttbar_pt_allSolutions", 1000, 0, 1000);

    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar", "ttbar_pt_vs_mttbar", 500, 0, 5000, 100, 0, 1000);
    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_2ndSolution", "ttbar_pt_vs_mttbar_2ndSolution", 500, 0, 5000, 500,
            0, 5000);
    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_3rdSolution", "ttbar_pt_vs_mttbar_3rdSolution", 500, 0, 5000, 500,
            0, 5000);
    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions", "ttbar_pt_vs_mttbar_allSolutions", 500, 0, 5000, 500,
            0, 5000);

    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_withMETCut", "ttbar_pt_vs_mttbar", 500, 0, 5000, 100, 0, 1000);
    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_2ndSolution_withMETCut", "ttbar_pt_vs_mttbar", 500, 0, 5000, 500,
            0, 5000);
    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_3rdSolution_withMETCut", "ttbar_pt_vs_mttbar", 500, 0, 5000, 500,
            0, 5000);
    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withMETCut", "ttbar_pt_vs_mttbar", 500, 0, 5000, 500,
            0, 5000);

    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_withMETAndAsymJets", "ttbar_pt_vs_mttbar", 500, 0, 5000, 500, 0,
            5000);
    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_2ndSolution_withMETAndAsymJets", "ttbar_pt_vs_mttbar", 500, 0, 5000,
            100, 0, 1000);
    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_3rdSolution_withMETAndAsymJets", "ttbar_pt_vs_mttbar", 500, 0, 5000,
            100, 0, 1000);
    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withMETAndAsymJets", "ttbar_pt_vs_mttbar", 500, 0,
            5000, 100, 0, 1000);

    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_withAsymJetsCut", "ttbar_pt_vs_mttbar", 500, 0, 5000, 100, 0, 1000);
    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_2ndSolution_withAsymJetsCut", "ttbar_pt_vs_mttbar", 500, 0, 5000,
            100, 0, 1000);
    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_3rdSolution_withAsymJetsCut", "ttbar_pt_vs_mttbar", 500, 0, 5000,
            100, 0, 1000);
    histMan.addH2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withAsymJetsCut", "ttbar_pt_vs_mttbar", 500, 0, 5000,
            100, 0, 1000);
//
    histMan.addH1D_BJetBinned("ttbar_px", "ttbar_px", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_py", "ttbar_py", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pz", "ttbar_pz", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("ttbar_pt_QCDEnriched", "ttbar_pt", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("HT", "HT", 5000, 0, 5000);
    histMan.addH2D_BJetBinned("HTvsMttbar", "HT vs mttbar", 500, 0, 5000, 500, 0, 5000);
    histMan.addH1D("numberOfJets", "numberOfJets", 10, 0, 10);
    histMan.addH1D("numberOfBJets", "numberOfBJets", 10, 0, 10);
    histMan.addH1D_BJetBinned("MET", "MET", 200, 0, 1000);
    histMan.addH2D_BJetBinned("METvsMttbar", "MET vs mttbar", 500, 0, 5000, 200, 0, 1000);
    histMan.addH1D_BJetBinned("leadingJetMass", "leadingJetMass", 200, 0, 200);
    histMan.addH1D_BJetBinned("mtW", "mtW", 600, 0, 600);
    histMan.addH1D("electronD0", "electronD0", 1000, 0, 0.2);
    histMan.addH1D_BJetBinned("neutrino_pz", "neutrino_pz", 1000, -500, 500);
    histMan.addH2D("ptRel_vs_DRmin", "ptRel_vs_DRmin", 100, 0, 1, 300, 0, 300);
    histMan.addH1D("ptRel_QCDenriched", "ptRel_QCDenriched", 300, 0, 300);
    histMan.addH1D("DRmin_QCDenriched", "DRmin_QCDenriched", 100, 0, 1);
    histMan.addH1D("ptRel_WZenriched", "ptRel_WZenriched", 300, 0, 300);
    histMan.addH1D("DRmin_WZenriched", "DRmin_WZenriched", 100, 0, 1);
    histMan.addH1D_JetBinned("QCDest_CombRelIso", "RelIso", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_CombRelIso_controlRegion", "RelIso control region", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_CombRelIso_1btag", "RelIso (>=1 btag)", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_CombRelIso_controlRegion_1btag", "RelIso control region", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_CombRelIso_2btag", "RelIso (>=2 btag)", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_CombRelIso_controlRegion_2btag", "RelIso control region", 1000, 0, 10);

    histMan.addH1D_JetBinned("QCDest_PFIsolation", "PFIso", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_WithMETCut", "PFIso", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_WithMETCutAndAsymJetCuts", "PFIso", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_WithAsymJetCuts", "PFIso", 1000, 0, 10);

    histMan.addH1D_JetBinned("QCDest_PFIsolation_controlRegion", "PFIso control region", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_1btag", "PFIso (>=1 btag)", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_controlRegion_1btag", "PFIso control region", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_2btag", "PFIso (>=2 btag)", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_controlRegion_2btag", "PFIso control region", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithMETCut", "PFIso control region", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithMETCutAndAsymJetCuts", "PFIso control region",
            1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithAsymJetCuts", "PFIso control region", 1000, 0, 10);

    histMan.addH1D_JetBinned("QCDest_PFIsolation_controlRegion2", "PFIso control region", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCut", "PFIso control region", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCutAndAsymJetCuts", "PFIso control region", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithAsymJetCuts", "PFIso control region", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_1btag", "PFIso control region", 1000, 0, 10);
    histMan.addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_2btag", "PFIso control region", 1000, 0, 10);

    histMan.addH1D_BJetBinned("pt_leadingTop", "pt_leadingTop", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("pt_NextToLeadingTop", "pt_NextToLeadingTop", 1000, 0, 1000);
    histMan.addH2D_BJetBinned("pt_leadingTop_vs_mttbar", "pt_leadingTop_vs_mttbar", 500, 0, 5000, 100, 0, 1000);
    histMan.addH2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar", "pt_NextToLeadingTop_vs_mttbar", 500, 0, 5000, 100, 0,
            1000);
    histMan.addH1D_BJetBinned("pt_leadingTop_withMETCut", "pt_leadingTop_withMETCut", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("pt_NextToLeadingTop_withMETCut", "pt_NextToLeadingTop_withMETCut", 1000, 0, 1000);
    histMan.addH2D_BJetBinned("pt_leadingTop_vs_mttbar_withMETCut", "pt_leadingTop_vs_mttbar_withMETCut", 500, 0,
            5000, 100, 0, 1000);
    histMan.addH2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withMETCut", "pt_NextToLeadingTop_vs_mttbar_withMETCut",
            500, 0, 5000, 100, 0, 1000);

    histMan.addH1D_BJetBinned("pt_leadingTop_withMETAndAsymJets", "pt_leadingTop_withMETAndAsymJets", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("pt_NextToLeadingTop_withMETAndAsymJets", "pt_NextToLeadingTop_withMETAndAsymJets", 1000,
            0, 1000);
    histMan.addH2D_BJetBinned("pt_leadingTop_vs_mttbar_withMETAndAsymJets",
            "pt_leadingTop_vs_mttbar_withMETAndAsymJets", 500, 0, 5000, 100, 0, 1000);
    histMan.addH2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withMETAndAsymJets",
            "pt_NextToLeadingTop_vs_mttbar_withMETAndAsymJets", 500, 0, 5000, 100, 0, 1000);

    histMan.addH1D_BJetBinned("pt_leadingTop_withAsymJetsCut", "pt_leadingTop_withMETAndAsymJets", 1000, 0, 1000);
    histMan.addH1D_BJetBinned("pt_NextToLeadingTop_withAsymJetsCut", "pt_NextToLeadingTop_withMETAndAsymJets", 1000, 0,
            1000);
    histMan.addH2D_BJetBinned("pt_leadingTop_vs_mttbar_withAsymJetsCut", "pt_leadingTop_vs_mttbar_withAsymJetsCut",
            500, 0, 5000, 100, 0, 1000);
    histMan.addH2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withAsymJetsCut",
            "pt_NextToLeadingTop_vs_mttbar_withAsymJetsCut", 500, 0, 5000, 100, 0, 1000);

    histMan.addH1D_BJetBinned("angleTops", "angle between top quarks", 400, 0, 4);
    histMan.addH1D_BJetBinned("angleTops_withMETCut", "angle between top quarks", 400, 0, 4);
    histMan.addH1D_BJetBinned("angleTops_withMETAndAsymJets", "angle between top quarks", 400, 0, 4);
    histMan.addH1D_BJetBinned("angleTops_withAsymJetsCut", "angle between top quarks", 400, 0, 4);

    histMan.addH2D_BJetBinned("angleTops_vs_mttbar", "angleTops_vs_mttbar", 500, 0, 5000, 400, 0, 4);
    histMan.addH2D_BJetBinned("angleTops_vs_mttbar_withMETCut", "angleTops_vs_mttbar", 500, 0, 5000, 400, 0, 4);
    histMan.addH2D_BJetBinned("angleTops_vs_mttbar_withMETAndAsymJets", "angleTops_vs_mttbar", 500, 0, 5000, 400, 0, 4);
    histMan.addH2D_BJetBinned("angleTops_vs_mttbar_withAsymJetsCut", "angleTops_vs_mttbar", 500, 0, 5000, 400, 0, 4);

}

Analysis::Analysis() :
    eventReader(new NTupleEventReader()),
//    eventFilter(Filter::makeTopPairEPlusJetsFilter()),
    currentEvent(),
    ttbarCandidate(),
    histMan(),
    cutflow(),
    singleCuts(),
    cutflowPerFile(),
    singleCutsPerFile(),
    interestingEvents(),
    brokenEvents(),
    eventCheck(),
    weights(Analysis::luminosity/*current lumi*/),
    weight(0),
    cutflowPerSample(DataType::NUMBER_OF_DATA_TYPES, TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS,
                    JetBin::NUMBER_OF_JET_BINS) {
    //    outputfile->SetCompressionLevel(7);
    for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
        cutflow[cut] = 0;
        singleCuts[cut] = 0;
    }
}

Analysis::~Analysis() {
    histMan.writeToDisk();
}

void Analysis::addInputFile(const char* fileName) {
    eventReader->addInputFile(fileName);
}

void Analysis::setMaximalNumberOfEvents(long maxEvents) {
    if (maxEvents > 0) {
        eventReader->setMaximumNumberOfEvents(maxEvents);
    }
}

void Analysis::setUsedNeutrinoSelectionForTopPairReconstruction(NeutrinoSelectionCriterion::value selection) {
    TopPairEventCandidate::usedNeutrinoSelection = selection;
}

void Analysis::checkForDuplicatedEvents(){
    map<unsigned long, std::vector<unsigned long> >::const_iterator iter;
    std::vector<pair<unsigned long, unsigned long> > duplicateEvents;

    for(iter = eventCheck.begin(); iter != eventCheck.end(); ++iter){
        std::vector<unsigned long> events = (*iter).second;
        std::sort(events.begin(), events.end());
        for(unsigned long ev = 0; ev < events.size() -1; ++ev){
            if(events.at(ev) == events.at(ev +1)){
                duplicateEvents.push_back(make_pair((*iter).first, events.at(ev)));
            }
        }
    }

    if (duplicateEvents.size() > 0){
        cout << "found duplicate events" << endl;
        for(unsigned long ev = 0; ev < duplicateEvents.size() -1; ++ev){
            cout << "run: " << duplicateEvents.at(ev).first << " event: " << duplicateEvents.at(ev).second << endl;
        }
    }
}

void Analysis::checkForBrokenEvents(){
    if(ttbarCandidate.Electrons().size() == 0){
        brokenEvents.push_back(InterestingEvent(ttbarCandidate, eventReader->getCurrentFile()));
    }

    if(ttbarCandidate.eventnumber() == 1019245){
        cout << "broken event" << endl;
        ttbarCandidate.inspect();
    }
}

