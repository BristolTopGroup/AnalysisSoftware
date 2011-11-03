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
#include "../interface/ReconstructionModules/ChiSquaredBasedTopPairReconstruction.h"
#include "../interface/LumiReWeighting.h"
#include "../interface/GlobalVariables.h"

using namespace reweight;
using namespace BAT;
using namespace std;
//float Analysis::luminosity = 882.;

void Analysis::analyze() {
	PoissonMeanShifter PShiftUp = PoissonMeanShifter(0.6); // PU-systematic
	PoissonMeanShifter PShiftDown = PoissonMeanShifter(-0.6); // PU-systematic

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
        //                doSynchExercise();
        hltriggerAnalyser->analyse(ttbarCandidate);
        electronAnalyser->analyse(ttbarCandidate);
        mttbarAnalyser->analyse(ttbarCandidate);
        doPileUpStudy();
        doTTbarCutFlow();
        doDiElectronAnalysis();
        doTTBarAnalysis();
        doNotePlots();
        doQCDStudy();
        doJetAnalysis();
        if (currentEvent.getDataType() == DataType::ttbar)
        	MonteCarloAnalyser->analyse(ttbarCandidate);

        if (useHitFit) {
            if (ttbarCandidate.passesFullTTbarEPlusJetSelection()){
            	hitfitAnalyser->analyse(ttbarCandidate);
            }
        }

//        if(currentEvent.getDataType() == DataType::DATA)
//            eventCheck[currentEvent.runnumber()].push_back(currentEvent.eventnumber());
//        checkForBrokenEvents();
    }
//    checkForDuplicatedEvents();
//    printInterestingEvents();
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
    if(!currentEvent.isRealData()){
        pileUpWeight = weights.reweightPileUp(currentEvent.numberOfGeneratedPileUpVertices());
        weight *= pileUpWeight;

    }
    currentEvent.setEventWeight(weight);
    ttbarCandidate.setEventWeight(weight);


    histMan->setCurrentDataType(ttbarCandidate.getDataType());
    histMan->setCurrentJetBin(currentEvent.GoodJets().size());
    histMan->setCurrentBJetBin(currentEvent.GoodBJets().size());
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
    if (ttbarCandidate.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::ConversionFinder)) {
        cout << ttbarCandidate.runnumber() << ":" << ttbarCandidate.eventnumber() << ":" << endl;//electron->et() << endl;
        if (ttbarCandidate.eventnumber() == 450622) {
            ttbarCandidate.inspect();
        }
    }
}

void Analysis::doTTbarCutFlow() {
    for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
        if (ttbarCandidate.passesEPlusJetsSelectionStep((TTbarEPlusJetsSelection::Step) cut)) {
            ++ePlusJetsSingleCuts[cut];
            ePlusJetsSingleCutsPerFile[eventReader->getCurrentFile()][cut]++;
        }

        if (ttbarCandidate.passesEPlusJetsSelectionStepUpTo((TTbarEPlusJetsSelection::Step) cut)) {
            ePlusJetsCutflow[cut] += 1;
            ePlusJetsCutflowPerFile[eventReader->getCurrentFile()][cut]++;
            unsigned int njet = ttbarCandidate.GoodJets().size();
            if (njet >= JetBin::NUMBER_OF_JET_BINS)
                njet = JetBin::NUMBER_OF_JET_BINS - 1;
            ePlusJetsCutflowPerSample.increase(ttbarCandidate.getDataType(), cut, njet, weight);
        }
    }

    for (unsigned int cut = 0; cut < TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
		if (ttbarCandidate.passesMuPlusJetsSelectionStep((TTbarMuPlusJetsSelection::Step) cut)) {
			++muPlusJetsSingleCuts[cut];
//			ePlusJetsSingleCutsPerFile[eventReader->getCurrentFile()][cut]++;
		}

		if (ttbarCandidate.passesMuPlusJetsSelectionStepUpTo((TTbarMuPlusJetsSelection::Step) cut)) {
			muPlusJetsCutFlow[cut] += 1;
//			ePlusJetsCutflowPerFile[eventReader->getCurrentFile()][cut]++;
			unsigned int njet = ttbarCandidate.GoodJets().size();
			if (njet >= JetBin::NUMBER_OF_JET_BINS)
				njet = JetBin::NUMBER_OF_JET_BINS - 1;
			muPlusJetsCutflowPerSample.increase(ttbarCandidate.getDataType(), cut, njet, weight);
		}
	}
}

void Analysis::doJetAnalysis() {
    histMan->setCurrentCollection("jetStudy");
    if (ttbarCandidate.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex)) {
        for(unsigned short jetIndex = 0; jetIndex < ttbarCandidate.Jets().size(); ++jetIndex)
            histMan->H1D_BJetBinned("AllJetMass")->Fill(ttbarCandidate.Jets().at(jetIndex)->mass());

        for (unsigned short jetIndex = 0; jetIndex < ttbarCandidate.GoodJets().size(); ++jetIndex) {
            double jetMass = ttbarCandidate.Jets().at(jetIndex)->mass();
            histMan->H1D_BJetBinned("AllGoodJetMass")->Fill(jetMass, weight);

            if (ttbarCandidate.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastOneGoodJets))
                histMan->H1D_BJetBinned("GoodJetMass_atLeastOneJets")->Fill(jetMass, weight);

            if (ttbarCandidate.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastTwoGoodJets))
                histMan->H1D_BJetBinned("GoodJetMass_atLeastTwoJets")->Fill(jetMass, weight);

            if (ttbarCandidate.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastThreeGoodJets))
                histMan->H1D_BJetBinned("GoodJetMass_atLeastThreeJets")->Fill(jetMass, weight);

            if (ttbarCandidate.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets))
                histMan->H1D_BJetBinned("GoodJetMass_atLeastFourJets")->Fill(jetMass, weight);
        }
    }
}

void Analysis::doDiElectronAnalysis() {
    histMan->setCurrentCollection("diElectronAnalysis");
    ElectronCollection electrons = currentEvent.GoodElectrons();
    if (electrons.size() == 2) {
        ElectronPointer leadingElectron = electrons.front();
        ElectronPointer secondElectron = electrons.at(1);
        histMan->H1D_JetBinned("diElectronMass")->Fill(leadingElectron->invariantMass(secondElectron), weight);
    }

    ElectronCollection isolatedElectrons = currentEvent.GoodPFIsolatedElectrons();
    if (isolatedElectrons.size() == 2) {
		ElectronPointer leadingElectron = isolatedElectrons.front();
		ElectronPointer secondElectron = isolatedElectrons.at(1);
		histMan->H1D_JetBinned("diElectronMass_iso")->Fill(leadingElectron->invariantMass(secondElectron), weight);
	}
}

void Analysis::doTTBarAnalysis() {
    histMan->setCurrentCollection("topReconstruction");
    if (ttbarCandidate.passesNMinus1(TTbarEPlusJetsSelection::AtLeastFourGoodJets)) {
        histMan->H1D("numberOfJets")->Fill(ttbarCandidate.GoodJets().size());
    }

    if (ttbarCandidate.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex)
            && ttbarCandidate.looseMuonVeto() && ttbarCandidate.electronPlusJetsZVeto()
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
                histMan->H1D("electronD0")->Fill(electron->d0(), weight);
            }
        }
    }
    if (ttbarCandidate.passesFullTTbarEPlusJetSelection()) {
    	ChiSquaredBasedTopPairReconstruction reco(ttbarCandidate.GoodPFIsolatedElectrons().front(), ttbarCandidate.MET(), ttbarCandidate.GoodJets());
        histMan->H1D("numberOfBJets")->Fill(ttbarCandidate.GoodBJets().size(), weight);
        try {
            if (Event::usePFIsolation)
                ttbarCandidate.reconstructTTbar(ttbarCandidate.GoodPFIsolatedElectrons().front());
            else
                ttbarCandidate.reconstructTTbar(ttbarCandidate.GoodIsolatedElectrons().front());
        } catch (ReconstructionException &e) {
            cout << e.what() << endl;
            return;
        }
        vector<TtbarHypothesisPointer> solutions = ttbarCandidate.Solutions();
        const ParticlePointer resonance = ttbarCandidate.getResonance();
        double mttbar = ttbarCandidate.mttbar();
//        mttbar = reco.getBestSolution()->resonance->mass();

        ParticlePointer leadingTop, nextToLeadingTop;

        if (ttbarCandidate.getHadronicTop()->pt() > ttbarCandidate.getLeptonicTop()->pt()) {
            leadingTop = ttbarCandidate.getHadronicTop();
            nextToLeadingTop = ttbarCandidate.getLeptonicTop();
        } else {
            leadingTop = ttbarCandidate.getLeptonicTop();
            nextToLeadingTop = ttbarCandidate.getHadronicTop();
        }
        double angleTops = leadingTop->angle(nextToLeadingTop);
        histMan->H1D_BJetBinned("angleTops")->Fill(angleTops, weight);
        histMan->H2D_BJetBinned("angleTops_vs_mttbar")->Fill(mttbar, angleTops, weight);
        histMan->H1D_BJetBinned("mLeptonicTop")->Fill(ttbarCandidate.getLeptonicTop()->mass(), weight);
        histMan->H1D_BJetBinned("mHadronicTop")->Fill(ttbarCandidate.getHadronicTop()->mass(), weight);
        histMan->H1D_BJetBinned("mAllTop")->Fill(ttbarCandidate.getLeptonicTop()->mass(), weight);
        histMan->H1D_BJetBinned("mAllTop")->Fill(ttbarCandidate.getHadronicTop()->mass(), weight);

        histMan->H1D_BJetBinned("MET")->Fill(ttbarCandidate.MET()->et(), weight);
        histMan->H2D_BJetBinned("METvsMttbar")->Fill(mttbar, ttbarCandidate.MET()->et(), weight);
        histMan->H1D_BJetBinned("HT")->Fill(ttbarCandidate.fullHT(), weight);
        histMan->H2D_BJetBinned("HTvsMttbar")->Fill(mttbar, ttbarCandidate.fullHT(), weight);
        histMan->H1D_BJetBinned("mtW")->Fill(ttbarCandidate.transverseWmass(ttbarCandidate.getElectronFromWDecay()), weight);

        histMan->H1D_BJetBinned("m3")->Fill(ttbarCandidate.M3(), weight);


//        histMan->H1D("mttbar")->Fill(mttbar, weight);
        histMan->H1D_BJetBinned("mttbar")->Fill(mttbar, weight);

        histMan->H1D_BJetBinned("ttbar_pt")->Fill(resonance->pt(), weight);

        histMan->H1D_BJetBinned("ttbar_px")->Fill(resonance->px(), weight);
        histMan->H1D_BJetBinned("ttbar_py")->Fill(resonance->py(), weight);
        histMan->H1D_BJetBinned("ttbar_pz")->Fill(resonance->pz(), weight);

        histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar")->Fill(mttbar, resonance->pt(), weight);


        histMan->H1D("electron_et")->Fill(ttbarCandidate.getElectronFromWDecay()->et(), weight);
        histMan->H1D_BJetBinned("neutrino_pz")->Fill(ttbarCandidate.getNeutrinoFromWDecay()->pz(), weight);

        histMan->H1D_BJetBinned("pt_leadingTop")->Fill(leadingTop->pt(), weight);
        histMan->H1D_BJetBinned("pt_NextToLeadingTop")->Fill(nextToLeadingTop->pt(), weight);
        histMan->H2D_BJetBinned("pt_leadingTop_vs_mttbar")->Fill(mttbar, leadingTop->pt(), weight);
        histMan->H2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar")->Fill(mttbar, nextToLeadingTop->pt(), weight);

        if (ttbarCandidate.passesMETCut()) {
            histMan->H1D_BJetBinned("angleTops_withMETCut")->Fill(angleTops, weight);
            histMan->H2D_BJetBinned("angleTops_vs_mttbar_withMETCut")->Fill(mttbar, angleTops, weight);
            histMan->H1D_BJetBinned("mttbar_withMETCut")->Fill(mttbar, weight);
            histMan->H1D_BJetBinned("ttbar_pt_withMETCut")->Fill(resonance->pt(), weight);
            histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_withMETCut")->Fill(mttbar, resonance->pt(), weight);

            histMan->H1D_BJetBinned("pt_leadingTop_withMETCut")->Fill(leadingTop->pt(), weight);
            histMan->H1D_BJetBinned("pt_NextToLeadingTop_withMETCut")->Fill(nextToLeadingTop->pt(), weight);
            histMan->H2D_BJetBinned("pt_leadingTop_vs_mttbar_withMETCut")->Fill(mttbar, leadingTop->pt(), weight);
            histMan->H2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withMETCut")->Fill(mttbar, nextToLeadingTop->pt(), weight);

            if (ttbarCandidate.passesAsymmetricElectronCleanedJetCuts()) {
                histMan->H1D_BJetBinned("angleTops_withMETAndAsymJets")->Fill(angleTops, weight);
                histMan->H2D_BJetBinned("angleTops_vs_mttbar_withMETAndAsymJets")->Fill(mttbar, angleTops, weight);
                histMan->H1D_BJetBinned("mttbar_withMETAndAsymJets")->Fill(mttbar, weight);
                histMan->H1D_BJetBinned("ttbar_pt_withMETAndAsymJets")->Fill(resonance->pt(), weight);

                histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_withMETAndAsymJets")->Fill(mttbar, resonance->pt(), weight);

                histMan->H1D_BJetBinned("pt_leadingTop_withMETAndAsymJets")->Fill(leadingTop->pt(), weight);
                histMan->H1D_BJetBinned("pt_NextToLeadingTop_withMETAndAsymJets")->Fill(nextToLeadingTop->pt(), weight);
                histMan->H2D_BJetBinned("pt_leadingTop_vs_mttbar_withMETAndAsymJets")->Fill(mttbar, leadingTop->pt(), weight);
                histMan->H2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withMETAndAsymJets")->Fill(mttbar, nextToLeadingTop->pt(), weight);
                histMan->H1D_BJetBinned("HT_withMETAndAsymJets")->Fill(ttbarCandidate.fullHT(), weight);
                histMan->H1D_BJetBinned("mLeptonicTop_withMETAndAsymJets")->Fill(
                        ttbarCandidate.getLeptonicTop()->mass(), weight);
                histMan->H1D_BJetBinned("mHadronicTop_withMETAndAsymJets")->Fill(
                        ttbarCandidate.getHadronicTop()->mass(), weight);
                histMan->H1D_BJetBinned("mAllTop_withMETAndAsymJets")->Fill(ttbarCandidate.getLeptonicTop()->mass(),
                        weight);
                histMan->H1D_BJetBinned("mAllTop_withMETAndAsymJets")->Fill(ttbarCandidate.getHadronicTop()->mass(),
                        weight);
                histMan->H1D_BJetBinned("m3_withMETAndAsymJets")->Fill(ttbarCandidate.M3(), weight);
                histMan->H1D_BJetBinned("MET_withMETAndAsymJets")->Fill(ttbarCandidate.MET()->et(), weight);
                histMan->H1D_BJetBinned("mtW_withMETAndAsymJets")->Fill(
                        ttbarCandidate.transverseWmass(ttbarCandidate.getElectronFromWDecay()), weight);
            }
        }

        if (ttbarCandidate.passesAsymmetricElectronCleanedJetCuts()) {
            histMan->H1D_BJetBinned("angleTops_withAsymJetsCut")->Fill(angleTops, weight);
            histMan->H2D_BJetBinned("angleTops_vs_mttbar_withAsymJetsCut")->Fill(mttbar, angleTops, weight);
            histMan->H1D_BJetBinned("mttbar_withAsymJetsCut")->Fill(mttbar, weight);
            histMan->H1D_BJetBinned("ttbar_pt_withAsymJetsCut")->Fill(resonance->pt(), weight);

            histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_withAsymJetsCut")->Fill(mttbar, resonance->pt(), weight);

            histMan->H1D_BJetBinned("pt_leadingTop_withAsymJetsCut")->Fill(leadingTop->pt(), weight);
            histMan->H1D_BJetBinned("pt_NextToLeadingTop_withAsymJetsCut")->Fill(nextToLeadingTop->pt(), weight);
            histMan->H2D_BJetBinned("pt_leadingTop_vs_mttbar_withAsymJetsCut")->Fill(mttbar, leadingTop->pt(), weight);
            histMan->H2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withAsymJetsCut")->Fill(mttbar, nextToLeadingTop->pt(), weight);


        }
        unsigned int numberOfSolutions(solutions.size());
        for (unsigned int solutionIndex = 0; solutionIndex < solutions.size(); ++solutionIndex) {
            histMan->H1D_BJetBinned("mttbar_allSolutions")->Fill(solutions.at(solutionIndex)->resonance->mass(), weight/numberOfSolutions);
            histMan->H1D_BJetBinned("ttbar_pt_allSolutions")->Fill(solutions.at(solutionIndex)->resonance->pt(), weight/numberOfSolutions);
            histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions")->Fill(
                    solutions.at(solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(),
                    weight/numberOfSolutions);

//            if (solutionIndex == 1) {
//                histMan->H1D_BJetBinned("mttbar_2ndSolution")->Fill(solutions.at(solutionIndex)->resonance->mass(),
//                        weight);
//
//                histMan->H1D_BJetBinned("ttbar_pt_2ndSolution")->Fill(solutions.at(solutionIndex)->resonance->pt(),
//                        weight);
//
//                histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_2ndSolution")->Fill(
//                        solutions.at(solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(),
//                        weight);
//
//            }
//            if (solutionIndex == 2) {
//                histMan->H1D_BJetBinned("mttbar_3rdSolution")->Fill(solutions.at(solutionIndex)->resonance->mass(),
//                        weight);
//
//                histMan->H1D_BJetBinned("ttbar_pt_3rdSolution")->Fill(solutions.at(solutionIndex)->resonance->pt(),
//                        weight);
//
//                histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_3rdSolution")->Fill(
//                        solutions.at(solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(),
//                        weight);
//            }

            if (ttbarCandidate.passesMETCut()) {
                histMan->H1D_BJetBinned("mttbar_allSolutions_withMETCut")->Fill(
                        solutions.at(solutionIndex)->resonance->mass(), weight/numberOfSolutions);
                histMan->H1D_BJetBinned("ttbar_pt_allSolutions_withMETCut")->Fill(
                        solutions.at(solutionIndex)->resonance->pt(), weight/numberOfSolutions);
                histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withMETCut")->Fill(
                        solutions.at(solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(),
                        weight/numberOfSolutions);
//                if (solutionIndex == 1) {
//                    histMan->H1D_BJetBinned("mttbar_2ndSolution_withMETCut")->Fill(
//                            solutions.at(solutionIndex)->resonance->mass(), weight);
//                    histMan->H1D_BJetBinned("ttbar_pt_2ndSolution_withMETCut")->Fill(
//                            solutions.at(solutionIndex)->resonance->pt(), weight);
//                    histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_2ndSolution_withMETCut")->Fill(solutions.at(
//                            solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);
//                }
//                if (solutionIndex == 2) {
//                    histMan->H1D_BJetBinned("mttbar_3rdSolution_withMETCut")->Fill(
//                            solutions.at(solutionIndex)->resonance->mass(), weight);
//                    histMan->H1D_BJetBinned("ttbar_pt_3rdSolution_withMETCut")->Fill(
//                            solutions.at(solutionIndex)->resonance->pt(), weight);
//                    histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_3rdSolution_withMETCut")->Fill(solutions.at(
//                            solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);
//                }

                if (ttbarCandidate.passesAsymmetricElectronCleanedJetCuts()) {
                    histMan->H1D_BJetBinned("mttbar_allSolutions_withMETAndAsymJets")->Fill(
                            solutions.at(solutionIndex)->resonance->mass(), weight/numberOfSolutions);
                    histMan->H1D_BJetBinned("ttbar_pt_allSolutions_withMETAndAsymJets")->Fill(
                            solutions.at(solutionIndex)->resonance->pt(), weight/numberOfSolutions);
                    histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withMETAndAsymJets")->Fill(solutions.at(
                            solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight/numberOfSolutions);
                }
            }

//            if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
//                histMan->H1D_BJetBinned("mttbar_allSolutions_withAsymJetsCut")->Fill(
//                        solutions.at(solutionIndex)->resonance->mass(), weight/numberOfSolutions);
//                histMan->H1D_BJetBinned("ttbar_pt_allSolutions_withAsymJetsCut")->Fill(
//                        solutions.at(solutionIndex)->resonance->pt(), weight/numberOfSolutions);
//                histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withAsymJetsCut")->Fill(solutions.at(
//                        solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight/numberOfSolutions);
//
////                if (ttbarCandidate.MET()->et() > 20) {
////
////                    if (solutionIndex == 1) {
////                        histMan->H1D_BJetBinned("mttbar_2ndSolution_withMETAndAsymJets")->Fill(solutions.at(
////                                solutionIndex)->resonance->mass(), weight);
////
////                        histMan->H1D_BJetBinned("ttbar_pt_2ndSolution_withMETAndAsymJets")->Fill(solutions.at(
////                                solutionIndex)->resonance->pt(), weight);
////                        histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_2ndSolution_withMETAndAsymJets")->Fill(solutions.at(
////                                solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);
////                    }
////
////                    if (solutionIndex == 2) {
////                        histMan->H1D_BJetBinned("mttbar_3rdSolution_withMETAndAsymJets")->Fill(solutions.at(
////                                solutionIndex)->resonance->mass(), weight);
////
////                        histMan->H1D_BJetBinned("ttbar_pt_3rdSolution_withMETAndAsymJets")->Fill(solutions.at(
////                                solutionIndex)->resonance->pt(), weight);
////                        histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_3rdSolution_withMETAndAsymJets")->Fill(solutions.at(
////                                solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);
////                    }
////                }
//
////                if (solutionIndex == 1) {
////                    histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_2ndSolution_withAsymJetsCut")->Fill(solutions.at(
////                            solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);
////
//////                    histMan->H1D_BJetBinned("mttbar_2ndSolution_withAsymJetsCut")->Fill(
//////                            solutions.at(solutionIndex)->resonance->mass(), weight);
//////                    histMan->H1D_BJetBinned("mttbar_2ndSolution_withAsymJetsCut")->Fill(
//////                            solutions.at(solutionIndex)->resonance->mass(), weight);
////                }
////
////                if (solutionIndex == 2) {
////                    histMan->H2D_BJetBinned("ttbar_pt_vs_mttbar_3rdSolution_withAsymJetsCut")->Fill(solutions.at(
////                            solutionIndex)->resonance->mass(), solutions.at(solutionIndex)->resonance->pt(), weight);
////                    histMan->H1D_BJetBinned("ttbar_pt_3rdSolution_withAsymJetsCut")->Fill(
////                            solutions.at(solutionIndex)->resonance->pt(), weight);
////
////                    histMan->H1D_BJetBinned("ttbar_pt_3rdSolution_withAsymJetsCut")->Fill(
////                            solutions.at(solutionIndex)->resonance->pt(), weight);
////                }
//
//            }

            //            cout << "total Chi2 = " << solutions.at(solutionIndex)->totalChi2;
            //            cout << ", mass = " << solutions.at(solutionIndex)->resonance->mass() << endl;
        }

        histMan->setCurrentCollection("topReconstruction/backgroundShape");
        if (ttbarCandidate.MET()->et() < 20) {
            histMan->H1D_BJetBinned("mttbar_QCDEnriched")->Fill(mttbar, weight);
            histMan->H1D_BJetBinned("ttbar_pt_QCDEnriched")->Fill(resonance->pt());
        }



//        cout << "Number of solutions: " << solutions.size() << " (" << ttbarCandidate.GoodJets().size() << " jets)"
//                << endl;
//        cout << "First solution, compare to chosen one" << endl;
//        cout << solutions.front()->totalChi2 << ", " << ttbarCandidate.getTotalChi2() << endl;

//        if (mttbar != mttbar) {//isnan
//            ttbarCandidate.inspectReconstructedEvent();
//        }
        if (ttbarCandidate.isRealData() && mttbar > 800 && ttbarCandidate.passesEPlusJetsSelectionStepUpTo(
                TTbarEPlusJetsSelection::AsymmetricJetCuts)) {
            cout << "run " << ttbarCandidate.runnumber() << ", event " << ttbarCandidate.eventnumber() << ", lumi "
                    << ttbarCandidate.lumiblock();
            cout << ", top pair invariant mass = " << mttbar << " GeV" << endl;
            interestingEvents.push_back(InterestingEvent(ttbarCandidate, eventReader->getCurrentFile()));

//            if (resonance->pt() > 200) {
//                cout << "top pair pt = " << resonance->pt() << " GeV" << endl;
//                ttbarCandidate.inspect();
//                ttbarCandidate.inspectReconstructedEvent();
//            }
        }

    }

    //for 3 jets only
    histMan->setCurrentCollection("topReconstruction");
    if (ttbarCandidate.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastTwoGoodJets)
            && ttbarCandidate.hasExactlyThreeGoodJets()) {
        try {
            if (Event::usePFIsolation)
                ttbarCandidate.reconstructTTbarFrom3Jets(ttbarCandidate.GoodPFIsolatedElectrons().front());
            else
                ttbarCandidate.reconstructTTbarFrom3Jets(ttbarCandidate.GoodIsolatedElectrons().front());
        } catch (ReconstructionException &e) {
            cout << e.what() << endl;
            return;
        }
        const ParticlePointer resonance = ttbarCandidate.getResonance();
        double mttbar = resonance->mass();
        histMan->H1D_BJetBinned("mttbar_3jets")->Fill(mttbar, weight);
        if (ttbarCandidate.MET()->pt() > 20) {
            if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                histMan->H1D_BJetBinned("mttbar_3jets_withMETAndAsymJets")->Fill(mttbar, weight);

            }
        }

    }
}

void Analysis::doNotePlots() {
    histMan->setCurrentCollection("QCDStudy");
    if (ttbarCandidate.GoodElectrons().size() >= 1 && ttbarCandidate.Jets().size() >= 2) {
        const ElectronCollection electrons = ttbarCandidate.GoodElectrons();
        ElectronCollection nonConversionElectrons;
        for (unsigned int index = 0; index < electrons.size(); ++index) {
            const ElectronPointer electron = electrons.at(index);
            if (electron->isFromConversion() == false && electron->isTaggedAsConversion(0.02,0.02) == false) {
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
                unsigned int closestID = electron->getClosestJetIndex(goodjets);
                float minDR = electron->deltaR(goodjets.at(closestID));
                float ptRel = electron->relativePtTo(goodjets.at(closestID));
                histMan->H2D("ptRel_vs_DRmin")->Fill(minDR, ptRel, weight);
                if (ttbarCandidate.MET()->et() < 20 && ttbarCandidate.transverseWmass(electron) < 35) {
                    histMan->H1D("DRmin_QCDenriched")->Fill(minDR, weight);
                    histMan->H1D("ptRel_QCDenriched")->Fill(ptRel, weight);
                } else if (ttbarCandidate.MET()->et() > 30 && ttbarCandidate.transverseWmass(electron) > 50) {
                    histMan->H1D("DRmin_WZenriched")->Fill(minDR, weight);
                    histMan->H1D("ptRel_WZenriched")->Fill(ptRel, weight);
                }
            }
        }

    }
}

void Analysis::doQCDStudy() {
    histMan->setCurrentCollection("QCDStudy");
    if (ttbarCandidate.passesEPlusJetsRelIsoSelection()) {
        const ElectronPointer electron = ttbarCandidate.MostIsolatedElectron(ttbarCandidate.Electrons());
        histMan->H1D_JetBinned("QCDest_CombRelIso")->Fill(electron->relativeIsolation(), weight);

        if (ttbarCandidate.GoodJets().size() >= 4) {
            double rho = currentEvent.rho();
            histMan->H1D_BJetBinned("RelativeIsolation_PUCorrected")->Fill(electron->relativeIsolationPUCorrected(rho),
                    weight);
            if (ttbarCandidate.passesMETCut() && ttbarCandidate.passesAsymmetricElectronCleanedJetCuts()) {
                histMan->H1D_BJetBinned("RelativeIsolation_PUCorrected_WithMETCutAndAsymJetCuts")->Fill(
                        electron->relativeIsolationPUCorrected(rho), weight);
            }
        }


        if (ttbarCandidate.GoodBJets().size() >= 1)
            histMan->H1D_JetBinned("QCDest_CombRelIso_1btag")->Fill(electron->relativeIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() >= 2)
            histMan->H1D_JetBinned("QCDest_CombRelIso_2btag")->Fill(electron->relativeIsolation(), weight);
    }

    if (ttbarCandidate.passesEPlusJetsPFIsoSelection() && Globals::electronAlgorithm
            == ElectronAlgorithm::ParticleFlow) {
        const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
        histMan->H1D_JetBinned("QCDest_PFIsolation")->Fill(electron->pfIsolation(), weight);


        if (ttbarCandidate.GoodBJets().size() == 1){
        	histMan->H1D_JetBinned("QCDest_PFIsolation_1btag")->Fill(electron->pfIsolation(), weight);
        	if(ttbarCandidate.passesAsymmetricElectronCleanedJetCuts() && ttbarCandidate.passesMETCut()){
        		histMan->H1D_JetBinned("QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts")->Fill(electron->pfIsolation(), weight);
        	}
        }


        if (ttbarCandidate.GoodBJets().size() == 2) {
			histMan->H1D_JetBinned("QCDest_PFIsolation_2btag")->Fill(electron->pfIsolation(), weight);
			if (ttbarCandidate.passesAsymmetricElectronCleanedJetCuts() && ttbarCandidate.passesMETCut()) {
				histMan->H1D_JetBinned("QCDest_PFIsolation_2btag_WithMETCutAndAsymJetCuts")->Fill(
						electron->pfIsolation(), weight);
			}
		}


        if(ttbarCandidate.GoodJets().size() >= 4){
            histMan->H1D_BJetBinned("PFIsolation")->Fill(electron->pfIsolation(), weight);
        }

        if (ttbarCandidate.MET()->pt() > 20) {
            histMan->H1D_JetBinned("QCDest_PFIsolation_WithMETCut")->Fill(electron->pfIsolation(), weight);

            if (ttbarCandidate.GoodJets().size() >= 2) {
                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                    histMan->H1D_JetBinned("QCDest_PFIsolation_WithMETCutAndAsymJetCuts")->Fill(
                            electron->pfIsolation(), weight);

                    if(ttbarCandidate.hasExactlyThreeGoodJets())
                        histMan->H1D_BJetBinned("PFIsolation_3jets_WithMETCutAndAsymJetCuts")->Fill(electron->pfIsolation(),
                                weight);

                    if (ttbarCandidate.GoodJets().size() >= 4)
                        histMan->H1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts")->Fill(electron->pfIsolation(),
                                weight);
                }
            }
        }

    }


    if (ttbarCandidate.passesEPlusJetsRelIsoControlSelection()) {
        const ElectronPointer electron = ttbarCandidate.MostIsolatedElectron(ttbarCandidate.Electrons());
        histMan->H1D_JetBinned("QCDest_CombRelIso_controlRegion")->Fill(electron->relativeIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() >= 1)
            histMan->H1D_JetBinned("QCDest_CombRelIso_controlRegion_1btag")->Fill(electron->relativeIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() >= 2)
            histMan->H1D_JetBinned("QCDest_CombRelIso_controlRegion_2btag")->Fill(electron->relativeIsolation(), weight);

        if (Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
            const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
            histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2")->Fill(electron->pfIsolation(), weight);

            if (ttbarCandidate.GoodBJets().size() >= 1)
                histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_1btag")->Fill(electron->pfIsolation(), weight);

            if (ttbarCandidate.GoodBJets().size() >= 2)
                histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_2btag")->Fill(electron->pfIsolation(), weight);

            if (ttbarCandidate.MET()->pt() > 20) {
                histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCut")->Fill(electron->pfIsolation(), weight);

                if (ttbarCandidate.GoodJets().size() >= 2) {
                    if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                        histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCutAndAsymJetCuts")->Fill(
                                electron->pfIsolation(), weight);
                    }
                }
            }

            if (ttbarCandidate.GoodJets().size() >= 2) {
                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                    histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithAsymJetCuts")->Fill(electron->pfIsolation(), weight);

                }
            }
        }

    }

    if (ttbarCandidate.passesEPlusJEtsPFIsoControlSelection() && Globals::electronAlgorithm
            == ElectronAlgorithm::ParticleFlow) {
        const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
        histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion")->Fill(electron->pfIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() == 1)
            histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion_1btag")->Fill(electron->pfIsolation(), weight);

        if (ttbarCandidate.GoodBJets().size() == 2)
            histMan->H1D_JetBinned("QCDest_PFIsolation_controlRegion_2btag")->Fill(electron->pfIsolation(), weight);

    }

    if (ttbarCandidate.passesEPlusJEtsPFIsoControlSelection() && Globals::electronAlgorithm
            == ElectronAlgorithm::ParticleFlow && ttbarCandidate.hasAtLeastFourGoodJets()) {
        const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
        histMan->H1D_BJetBinned("PFIsolation_controlRegion")->Fill(electron->pfIsolation(), weight);
    }

    histMan->setCurrentCollection("topReconstruction/backgroundShape");
    if (ttbarCandidate.passesEPlusJetsRelIsoSelection() && ttbarCandidate.hasAtLeastFourGoodJets()) {
        const ElectronPointer electron = ttbarCandidate.MostIsolatedElectron(ttbarCandidate.Electrons());
        if (electron->relativeIsolation() > Globals::maxElectronRelativeIsolation && !isnan(electron->relativeIsolation()) && !isinf(
                electron->relativeIsolation())) {
            try {
                ttbarCandidate.reconstructTTbar(electron);
                const ParticlePointer resonance = ttbarCandidate.getResonance();
                histMan->H1D_BJetBinned("mttbar_controlRegion")->Fill(resonance->mass(), weight);
                if (ttbarCandidate.MET()->pt() > 20) {
                    histMan->H1D_BJetBinned("mttbar_controlRegion_withMETCut")->Fill(resonance->mass(), weight);
                    if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
                        histMan->H1D_BJetBinned("mttbar_controlRegion_withMETAndAsymJets")->Fill(resonance->mass(),
                                weight);
                }

                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
                    histMan->H1D_BJetBinned("mttbar_controlRegion_withAsymJetsCut")->Fill(resonance->mass(), weight);

            } catch (ReconstructionException& e) {
                cout << "Could not reconstruct event: " << e.what() << endl;
            }
        }
    }

    if (ttbarCandidate.passesConversionSelection() && ttbarCandidate.hasAtLeastFourGoodJets()) {
        ElectronPointer electron;
        if (Event::usePFIsolation)
            electron = currentEvent.GoodPFIsolatedElectrons().front();
        else
            electron = currentEvent.GoodIsolatedElectrons().front();
        try {
            ttbarCandidate.reconstructTTbar(electron);
            const ParticlePointer resonance = ttbarCandidate.getResonance();
            histMan->H1D_BJetBinned("mttbar_conversions")->Fill(resonance->mass(), weight);
            if (ttbarCandidate.MET()->pt() > 20) {
                histMan->H1D_BJetBinned("mttbar_conversions_withMETCut")->Fill(resonance->mass(), weight);
                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
                    histMan->H1D_BJetBinned("mttbar_conversions_withMETAndAsymJets")->Fill(resonance->mass(), weight);
            }
            if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
                histMan->H1D_BJetBinned("mttbar_conversions_withAsymJetsCut")->Fill(resonance->mass(), weight);
        } catch (ReconstructionException& e) {
            cout << "Could not reconstruct event: " << e.what() << endl;
        }

    }

    if(ttbarCandidate.passesConversionSelection() && ttbarCandidate.hasExactlyThreeGoodJets()){
        ElectronPointer electron;
        if (Event::usePFIsolation)
            electron = currentEvent.GoodPFIsolatedElectrons().front();
        else
            electron = currentEvent.GoodIsolatedElectrons().front();

        try {
            ttbarCandidate.reconstructTTbarFrom3Jets(electron);
            const ParticlePointer resonance = ttbarCandidate.getResonance();
            histMan->H1D_BJetBinned("mttbar_3jets_conversions")->Fill(resonance->mass(), weight);
            if (ttbarCandidate.MET()->pt() > 20) {
                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50)
                    histMan->H1D_BJetBinned("mttbar_3jets_conversions_withMETAndAsymJets")->Fill(resonance->mass(), weight);
            }
        } catch (ReconstructionException& e) {
            cout << "Could not reconstruct event: " << e.what() << endl;
        }
    }

    histMan->setCurrentCollection("QCDStudy");
    if (ttbarCandidate.Electrons().size() > 0 && ttbarCandidate.GoodPFIsolatedElectrons().size() < 2
            && Globals::electronAlgorithm == ElectronAlgorithm::ParticleFlow) {
        const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
        histMan->H1D_JetBinned("MostPFIsolatedElectron_dPhiIn")->Fill(electron->dPhiIn(), weight);
        histMan->H1D_JetBinned("MostPFIsolatedElectron_dEtaIn")->Fill(electron->dEtaIn(), weight);
    }

    histMan->setCurrentCollection("topReconstruction/backgroundShape");
    if (ttbarCandidate.passesEPlusJetsAntiIsolationSelection() && ttbarCandidate.hasAtLeastFourGoodJets()) {
        ElectronPointer electron = ttbarCandidate.GoodElectrons().front();
        try {
            ttbarCandidate.reconstructTTbar(electron);
            float mttbar = ttbarCandidate.mttbar();
            histMan->H1D_BJetBinned("mttbar_antiIsolated")->Fill(mttbar, weight);
            if (ttbarCandidate.MET()->pt() > 20) {
                histMan->H1D_BJetBinned("mttbar_antiIsolated_withMETCut")->Fill(mttbar, weight);
                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                    histMan->H1D_BJetBinned("mttbar_antiIsolated_withMETAndAsymJets")->Fill(mttbar, weight);
                }
            }
            if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                histMan->H1D_BJetBinned("mttbar_antiIsolated_withAsymJetsCut")->Fill(mttbar, weight);
            }

        } catch (ReconstructionException& e) {
            cout << "Could not reconstruct event: " << e.what() << endl;
        }
    }

    if (ttbarCandidate.passesEPlusJetsAntiIsolationSelection() && ttbarCandidate.hasExactlyThreeGoodJets()) {
        ElectronPointer electron = ttbarCandidate.GoodElectrons().front();
        try {
            ttbarCandidate.reconstructTTbarFrom3Jets(electron);
            float mttbar = ttbarCandidate.mttbar();
            histMan->H1D_BJetBinned("mttbar_3jets_antiIsolated")->Fill(mttbar, weight);
            if (ttbarCandidate.MET()->pt() > 20) {
                if (ttbarCandidate.GoodJets().front()->pt() > 70 && ttbarCandidate.GoodJets().at(1)->pt() > 50) {
                    histMan->H1D_BJetBinned("mttbar_3jets_antiIsolated_withMETAndAsymJets")->Fill(mttbar, weight);
                }
            }
        } catch (ReconstructionException& e) {
            cout << "Could not reconstruct event: " << e.what() << endl;
        }
    }

    if (ttbarCandidate.passedEPlusJetsAntiIDSelection() && ttbarCandidate.hasAtLeastFourGoodJets()) {
        const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
        try {
            ttbarCandidate.reconstructTTbar(electron);
            const ParticlePointer resonance = ttbarCandidate.getResonance();
            histMan->H1D_BJetBinned("mttbar_antiID")->Fill(resonance->mass(), weight);
            if (ttbarCandidate.passesMETCut() && ttbarCandidate.passesAsymmetricElectronCleanedJetCuts())
                histMan->H1D_BJetBinned("mttbar_antiID_withMETAndAsymJets")->Fill(resonance->mass(), weight);

        } catch (ReconstructionException& e) {
            cout << "Could not reconstruct event: " << e.what() << endl;
        }
    }

    if (ttbarCandidate.passedEPlusJetsAntiIDSelection() && ttbarCandidate.hasExactlyThreeGoodJets()) {
        const ElectronPointer electron = ttbarCandidate.MostPFIsolatedElectron(ttbarCandidate.Electrons());
        try {
            ttbarCandidate.reconstructTTbarFrom3Jets(electron);
            const ParticlePointer resonance = ttbarCandidate.getResonance();
            histMan->H1D_BJetBinned("mttbar_3jets_antiID")->Fill(resonance->mass(), weight);
            if (ttbarCandidate.passesMETCut() && ttbarCandidate.passesAsymmetricElectronCleanedJetCuts())
                histMan->H1D_BJetBinned("mttbar_3jets_antiID_withMETAndAsymJets")->Fill(resonance->mass(), weight);

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
    EventTablePrinter::printCutFlowLatexTable(ePlusJetsCutflowPerSample, TTbarEPlusJetsSelection::StringSteps,
			Globals::luminosity);
	EventTablePrinter::printUnweightedCutFlowLatexTable(ePlusJetsCutflowPerSample, TTbarEPlusJetsSelection::StringSteps,
			Globals::luminosity);

	EventTablePrinter::printCutFlowLatexTable(muPlusJetsCutflowPerSample, TTbarMuPlusJetsSelection::StringSteps,
			Globals::luminosity);
	EventTablePrinter::printUnweightedCutFlowLatexTable(muPlusJetsCutflowPerSample,
			TTbarMuPlusJetsSelection::StringSteps, Globals::luminosity);

    cout << "total number of processed events: " << eventReader->getNumberOfProccessedEvents() << endl;
    cout << endl;
    cout << "Electron + jets selection" << endl;
    for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
        cout << "Selection step '" << TTbarEPlusJetsSelection::StringSteps[cut] << "'" << endl;
        cout << "passed events (single cut): " << ePlusJetsSingleCuts.at(cut) << endl;
        if (cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS)
            cout << "passed events (up to this cut):" << ePlusJetsCutflow.at(cut) << endl;
        else
            cout << "passed events (full selection):" << ePlusJetsCutflow.at(cut) << endl;
        cout << endl;
    }

    cout << "Muon + jets selection" << endl;
    for (unsigned int cut = 0; cut < TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
		cout << "Selection step '" << TTbarMuPlusJetsSelection::StringSteps[cut] << "'" << endl;
		cout << "passed events (single cut): " << muPlusJetsSingleCuts.at(cut) << endl;

		if (cut < TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS)
			cout << "passed events (up to this cut):" << muPlusJetsCutFlow.at(cut) << endl;
		else
			cout << "passed events (full selection):" << muPlusJetsCutFlow.at(cut) << endl;
		cout << endl;
	}

    cout << "number of events without electrons: " << brokenEvents.size() << endl;
    cout << "number of events with too high pileup: " << weights.getNumberOfEventsWithTooHighPileUp() << endl;
}

void Analysis::createHistograms() {
    histMan->prepareForSeenDataTypes(eventReader->getSeenDatatypes());

    //histograms for Pile-up
    histMan->setCurrentCollection("pileupStudy");
    histMan->addH1D("nVertex", "number of primary vertices", 51, 0, 50);
    histMan->addH1D("nVertex_reweighted", "number of primary vertices", 51, 0, 50);
    histMan->addH1D_BJetBinned("nVertex_reweighted_withMETAndAsymJets", "number of primary vertices", 51, 0, 50);
    histMan->addH1D_BJetBinned("nVertex_withMETAndAsymJets", "number of primary vertices", 51, 0, 50);
    //histograms for Jet study
    histMan->setCurrentCollection("jetStudy");
    histMan->addH1D_BJetBinned("AllJetMass", "AllJetMass", 500, 0, 500);
    histMan->addH1D_BJetBinned("AllGoodJetMass", "AllGoodJetMass", 500, 0, 500);
    histMan->addH1D_BJetBinned("GoodJetMass_atLeastOneJets", "GoodJetMass_atLeastOneJets", 500, 0, 500);
    histMan->addH1D_BJetBinned("GoodJetMass_atLeastTwoJets", "GoodJetMass_atLeastTwoJets", 500, 0, 500);
    histMan->addH1D_BJetBinned("GoodJetMass_atLeastThreeJets", "GoodJetMass_atLeastThreeJets", 500, 0, 500);
    histMan->addH1D_BJetBinned("GoodJetMass_atLeastFourJets", "GoodJetMass_atLeastFourJets", 500, 0, 500);

    histMan->setCurrentCollection("diElectronAnalysis");
    histMan->addH1D_JetBinned("diElectronMass", "diElectronMass", 7000, 0, 7000);
    histMan->addH1D_JetBinned("diElectronMass_iso", "diElectronMass (iso)", 7000, 0, 7000);

    histMan->setCurrentCollection("topReconstruction");
    histMan->addH1D("electron_et", "electron_et", 500, 0, 500);


    histMan->setCurrentCollection("topReconstruction/backgroundShape");
    histMan->addH1D_BJetBinned("mttbar_conversions", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_antiIsolated", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_QCDEnriched", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_controlRegion", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_antiID", "mttbar", 5000, 0, 5000);

    histMan->addH1D_BJetBinned("mttbar_3jets_conversions", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_3jets_antiIsolated", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_3jets_antiID", "mttbar", 5000, 0, 5000);

    histMan->addH1D_BJetBinned("mttbar_conversions_withMETCut", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_antiIsolated_withMETCut", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_controlRegion_withMETCut", "mttbar", 5000, 0, 5000);

    histMan->addH1D_BJetBinned("mttbar_conversions_withMETAndAsymJets", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_antiIsolated_withMETAndAsymJets", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_controlRegion_withMETAndAsymJets", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_antiID_withMETAndAsymJets", "mttbar", 5000, 0, 5000);

    histMan->addH1D_BJetBinned("mttbar_3jets_conversions_withMETAndAsymJets", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_3jets_antiIsolated_withMETAndAsymJets", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_3jets_antiID_withMETAndAsymJets", "mttbar", 5000, 0, 5000);

    histMan->addH1D_BJetBinned("mttbar_conversions_withAsymJetsCut", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_antiIsolated_withAsymJetsCut", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_controlRegion_withAsymJetsCut", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("ttbar_pt_QCDEnriched", "ttbar_pt", 1000, 0, 1000);

    histMan->setCurrentCollection("topReconstruction");
    histMan->addH1D_BJetBinned("mttbar", "mttbar", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_withMETCut", "mttbar_withMETCut", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_withMETAndAsymJets", "mttbar_withMETAndAsymJets", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_withAsymJetsCut", "mttbar_withAsymJetsCut", 5000, 0, 5000);

    histMan->addH1D_BJetBinned("mttbar_3jets", "mttbar_3jet", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_3jets_withMETAndAsymJets", "mttbar_3jet_withMETAndAsymJets", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_allSolutions", "mttbar_allSolutions", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_allSolutions_withMETCut", "mttbar_allSolutions", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mttbar_allSolutions_withMETAndAsymJets", "mttbar_allSolutions", 5000, 0, 5000);

    histMan->addH1D_BJetBinned("mttbar_allSolutions_withAsymJetsCut", "mttbar_allSolutions", 5000, 0, 5000);

    histMan->addH1D_BJetBinned("mLeptonicTop", "mLeptonicTop", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mHadronicTop", "mHadronicTop", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mAllTop", "mAllTop", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("m3", "m3", 5000, 0, 5000);

    histMan->addH1D_BJetBinned("mLeptonicTop_withMETAndAsymJets", "mLeptonicTop", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mHadronicTop_withMETAndAsymJets", "mHadronicTop", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("mAllTop_withMETAndAsymJets", "mAllTop", 5000, 0, 5000);
    histMan->addH1D_BJetBinned("m3_withMETAndAsymJets", "m3", 5000, 0, 5000);

    histMan->addH1D_BJetBinned("ttbar_pt", "ttbar_pt", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("ttbar_pt_withMETCut", "ttbar_pt", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("ttbar_pt_withMETAndAsymJets", "ttbar_pt", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("ttbar_pt_withAsymJetsCut", "ttbar_pt", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("ttbar_pt_allSolutions", "ttbar_pt_allSolutions", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("ttbar_pt_allSolutions_withMETCut", "ttbar_pt_allSolutions", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("ttbar_pt_allSolutions_withMETAndAsymJets", "ttbar_pt_allSolutions", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("ttbar_pt_allSolutions_withAsymJetsCut", "ttbar_pt_allSolutions", 1000, 0, 1000);

    histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar", "ttbar_pt_vs_mttbar", 100, 0, 5000, 40, 0, 1000);
    histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions", "ttbar_pt_vs_mttbar_allSolutions", 100, 0, 5000, 40,
            0, 1000);

    histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_withMETCut", "ttbar_pt_vs_mttbar", 100, 0, 5000, 40, 0, 1000);
    histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withMETCut", "ttbar_pt_vs_mttbar", 100, 0, 5000, 40,
            0, 1000);

    histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_withMETAndAsymJets", "ttbar_pt_vs_mttbar", 100, 0, 5000, 40, 0,
            1000);
    histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withMETAndAsymJets", "ttbar_pt_vs_mttbar", 100, 0,
            5000, 40, 0, 1000);

    histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_withAsymJetsCut", "ttbar_pt_vs_mttbar", 100, 0, 5000, 40, 0, 1000);
    histMan->addH2D_BJetBinned("ttbar_pt_vs_mttbar_allSolutions_withAsymJetsCut", "ttbar_pt_vs_mttbar", 100, 0, 5000,
            40, 0, 1000);
//
    histMan->addH1D_BJetBinned("ttbar_px", "ttbar_px", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("ttbar_py", "ttbar_py", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("ttbar_pz", "ttbar_pz", 1000, 0, 1000);

    histMan->addH1D_BJetBinned("HT", "HT", 5000, 0, 5000);
	histMan->addH1D_BJetBinned("HT_withMETAndAsymJets", "HT", 5000, 0, 5000);
	histMan->addH2D_BJetBinned("HTvsMttbar", "HT vs mttbar", 500, 0, 5000, 500, 0, 5000);
	histMan->addH1D("numberOfJets", "numberOfJets", 10, 0, 10);
	histMan->addH1D("numberOfBJets", "numberOfBJets", 10, 0, 10);
	histMan->addH1D_BJetBinned("MET", "MET", 200, 0, 1000);
	histMan->addH1D_BJetBinned("MET_withMETAndAsymJets", "MET", 200, 0, 1000);
	histMan->addH1D_BJetBinned("MET_withAsymJetsCut", "MET", 200, 0, 1000);
	histMan->addH2D_BJetBinned("METvsMttbar", "MET vs mttbar", 500, 0, 5000, 200, 0, 1000);
	histMan->addH1D_BJetBinned("mtW", "mtW", 600, 0, 600);
	histMan->addH1D_BJetBinned("mtW_withMETAndAsymJets", "mtW", 600, 0, 600);
	histMan->addH1D("electronD0", "electronD0", 1000, 0, 0.2);
	histMan->addH1D_BJetBinned("neutrino_pz", "neutrino_pz", 1000, -500, 500);

    histMan->setCurrentCollection("QCDStudy");
    histMan->addH1D_JetBinned("MostPFIsolatedElectron_dPhiIn", "MostPFIsolatedElectron_dPhiIn", 50, 0, 0.1);
    histMan->addH1D_JetBinned("MostPFIsolatedElectron_dEtaIn", "MostPFIsolatedElectron_dEtaIn", 50, 0, 0.02 );
    histMan->addH2D("ptRel_vs_DRmin", "ptRel_vs_DRmin", 100, 0, 1, 300, 0, 300);
    histMan->addH1D("ptRel_QCDenriched", "ptRel_QCDenriched", 300, 0, 300);
    histMan->addH1D("DRmin_QCDenriched", "DRmin_QCDenriched", 100, 0, 1);
    histMan->addH1D("ptRel_WZenriched", "ptRel_WZenriched", 300, 0, 300);
    histMan->addH1D("DRmin_WZenriched", "DRmin_WZenriched", 100, 0, 1);
    histMan->addH1D_JetBinned("QCDest_CombRelIso", "RelIso", 1000, 0, 10);
    histMan->addH1D_JetBinned("QCDest_CombRelIso_controlRegion", "RelIso control region", 1000, 0, 10);
    histMan->addH1D_JetBinned("QCDest_CombRelIso_1btag", "RelIso (>=1 btag)", 1000, 0, 10);
    histMan->addH1D_JetBinned("QCDest_CombRelIso_controlRegion_1btag", "RelIso control region", 1000, 0, 10);
    histMan->addH1D_JetBinned("QCDest_CombRelIso_2btag", "RelIso (>=2 btag)", 1000, 0, 10);
    histMan->addH1D_JetBinned("QCDest_CombRelIso_controlRegion_2btag", "RelIso control region", 1000, 0, 10);

    histMan->addH1D_JetBinned("QCDest_PFIsolation", "PFIso", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_WithMETCut", "PFIso", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);

    histMan->addH1D_BJetBinned("PFIsolation", "PFIso", 500, 0, 5);

    histMan->addH1D_BJetBinned("PFIsolation_controlRegion", "PFIso", 500, 0, 5);
    histMan->addH1D_BJetBinned("PFIsolation_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);
    histMan->addH1D_BJetBinned("PFIsolation_3jets_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);

    histMan->addH1D_BJetBinned("RelativeIsolation_PUCorrected", "PFIso", 500, 0, 5);
    histMan->addH1D_BJetBinned("RelativeIsolation_PUCorrected_WithMETCutAndAsymJetCuts", "PFIso", 500, 0, 5);

    //histMan->addH1D_JetBinned("QCDest_PFIsolation_WithAsymJetCuts", "PFIso", 1000, 0, 10);

    histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion", "PFIso control region", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_1btag", "PFIso (>=1 btag)", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts", "PFIso (>=1 btag)", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_1btag", "PFIso control region", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_2btag", "PFIso (>=2 btag)", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_2btag_WithMETCutAndAsymJetCuts", "PFIso (>=2 btag)", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_2btag", "PFIso control region", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithMETCut", "PFIso control region", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithMETCutAndAsymJetCuts", "PFIso control region",
            1000, 0, 10);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion_WithAsymJetCuts", "PFIso control region", 500, 0, 5);

    histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2", "PFIso control region", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCut", "PFIso control region", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithMETCutAndAsymJetCuts", "PFIso control region", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_WithAsymJetCuts", "PFIso control region", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_1btag", "PFIso control region", 500, 0, 5);
    histMan->addH1D_JetBinned("QCDest_PFIsolation_controlRegion2_2btag", "PFIso control region", 500, 0, 5);


    histMan->setCurrentCollection("topReconstruction");
    histMan->addH1D_BJetBinned("pt_leadingTop", "pt_leadingTop", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("pt_NextToLeadingTop", "pt_NextToLeadingTop", 1000, 0, 1000);
    histMan->addH2D_BJetBinned("pt_leadingTop_vs_mttbar", "pt_leadingTop_vs_mttbar", 100, 0, 5000, 40, 0, 1000);
    histMan->addH2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar", "pt_NextToLeadingTop_vs_mttbar", 100, 0, 5000, 40, 0,
            1000);
    histMan->addH1D_BJetBinned("pt_leadingTop_withMETCut", "pt_leadingTop_withMETCut", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("pt_NextToLeadingTop_withMETCut", "pt_NextToLeadingTop_withMETCut", 1000, 0, 1000);
    histMan->addH2D_BJetBinned("pt_leadingTop_vs_mttbar_withMETCut", "pt_leadingTop_vs_mttbar_withMETCut", 100, 0,
            5000, 40, 0, 1000);
    histMan->addH2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withMETCut", "pt_NextToLeadingTop_vs_mttbar_withMETCut",
            100, 0, 5000, 40, 0, 1000);

    histMan->addH1D_BJetBinned("pt_leadingTop_withMETAndAsymJets", "pt_leadingTop_withMETAndAsymJets", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("pt_NextToLeadingTop_withMETAndAsymJets", "pt_NextToLeadingTop_withMETAndAsymJets", 1000,
            0, 1000);
    histMan->addH2D_BJetBinned("pt_leadingTop_vs_mttbar_withMETAndAsymJets",
            "pt_leadingTop_vs_mttbar_withMETAndAsymJets", 100, 0, 5000, 40, 0, 1000);
    histMan->addH2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withMETAndAsymJets",
            "pt_NextToLeadingTop_vs_mttbar_withMETAndAsymJets", 100, 0, 5000, 40, 0, 1000);

    histMan->addH1D_BJetBinned("pt_leadingTop_withAsymJetsCut", "pt_leadingTop_withMETAndAsymJets", 1000, 0, 1000);
    histMan->addH1D_BJetBinned("pt_NextToLeadingTop_withAsymJetsCut", "pt_NextToLeadingTop_withMETAndAsymJets", 1000, 0,
            1000);
    histMan->addH2D_BJetBinned("pt_leadingTop_vs_mttbar_withAsymJetsCut", "pt_leadingTop_vs_mttbar_withAsymJetsCut",
            100, 0, 5000, 40, 0, 1000);
    histMan->addH2D_BJetBinned("pt_NextToLeadingTop_vs_mttbar_withAsymJetsCut",
            "pt_NextToLeadingTop_vs_mttbar_withAsymJetsCut", 100, 0, 5000, 40, 0, 1000);

    histMan->addH1D_BJetBinned("angleTops", "angle between top quarks", 400, 0, 4);
    histMan->addH1D_BJetBinned("angleTops_withMETCut", "angle between top quarks", 400, 0, 4);
    histMan->addH1D_BJetBinned("angleTops_withMETAndAsymJets", "angle between top quarks", 400, 0, 4);
    histMan->addH1D_BJetBinned("angleTops_withAsymJetsCut", "angle between top quarks", 400, 0, 4);

    histMan->addH2D_BJetBinned("angleTops_vs_mttbar", "angleTops_vs_mttbar", 100, 0, 5000, 40, 0, 4);
    histMan->addH2D_BJetBinned("angleTops_vs_mttbar_withMETCut", "angleTops_vs_mttbar", 100, 0, 5000, 40, 0, 4);
    histMan->addH2D_BJetBinned("angleTops_vs_mttbar_withMETAndAsymJets", "angleTops_vs_mttbar", 100, 0, 5000, 40, 0, 4);
    histMan->addH2D_BJetBinned("angleTops_vs_mttbar_withAsymJetsCut", "angleTops_vs_mttbar", 100, 0, 5000, 40, 0, 4);

    hltriggerAnalyser->createHistograms();
    electronAnalyser->createHistograms();
    mttbarAnalyser->createHistograms();
    MonteCarloAnalyser->createHistograms();
    hitfitAnalyser->createHistograms();
}

Analysis::Analysis(std::string fileForPileUpReweighting) :
    eventReader(new NTupleEventReader()),
    currentEvent(),
    ttbarCandidate(),
    histMan(new BAT::HistogramManager()),
    ePlusJetsCutflow(),
    ePlusJetsSingleCuts(),
    ePlusJetsCutflowPerFile(),
    ePlusJetsSingleCutsPerFile(),
    muPlusJetsCutFlow(),
    muPlusJetsSingleCuts(),
    interestingEvents(),
    brokenEvents(),
    eventCheck(),
    weights(Globals::luminosity/*current lumi*/, 7, fileForPileUpReweighting),
    weight(0),
    pileUpWeight(1),
    ePlusJetsCutflowPerSample(DataType::NUMBER_OF_DATA_TYPES, TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS,
                    JetBin::NUMBER_OF_JET_BINS),
    muPlusJetsCutflowPerSample(DataType::NUMBER_OF_DATA_TYPES, TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS,
                    JetBin::NUMBER_OF_JET_BINS),
    hltriggerAnalyser(new HLTriggerAnalyser(histMan)),
    electronAnalyser(new ElectronAnalyser(histMan)),
    mttbarAnalyser(new MTtbarAnalyser(histMan)),
    MonteCarloAnalyser(new MCAnalyser(histMan)),
    hitfitAnalyser(new HitFitAnalyser(histMan)){
    for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
		ePlusJetsCutflow[cut] = 0;
		ePlusJetsSingleCuts[cut] = 0;
	}

    for (unsigned int cut = 0; cut < TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
    	muPlusJetsCutFlow[cut] = 0;
		muPlusJetsSingleCuts[cut] = 0;
	}
	histMan->enableDebugMode(true);
}

Analysis::~Analysis() {
    histMan->writeToDisk();
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

void Analysis::setUsedTTbarReconstructionCriterion(TTbarReconstructionCriterion::value selection) {
	TopPairEventCandidate::usedTTbarReconstruction = selection;
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

void Analysis::doPileUpStudy() {
    histMan->setCurrentCollection("pileupStudy");
    if (pileUpWeight > 0) {
        histMan->H1D("nVertex")->Fill(currentEvent.Vertices().size(), weight/pileUpWeight);
        histMan->H1D("nVertex_reweighted")->Fill(currentEvent.Vertices().size(), weight);

        if (ttbarCandidate.passesFullTTbarEPlusJetSelection()) {
            if (ttbarCandidate.passesMETCut() && ttbarCandidate.passesAsymmetricElectronCleanedJetCuts()) {
                histMan->H1D_BJetBinned("nVertex_withMETAndAsymJets")->Fill(currentEvent.Vertices().size(), weight/pileUpWeight);
                histMan->H1D_BJetBinned("nVertex_reweighted_withMETAndAsymJets")->Fill(currentEvent.Vertices().size(), weight);
            }
        }
    }

}

unsigned long Analysis::getNumberOfProccessedEvents() const{
    return eventReader->getNumberOfProccessedEvents();
}

