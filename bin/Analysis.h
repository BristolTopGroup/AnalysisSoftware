/*
 * Analysis.h
 *
 *  Created on: 12 Jul 2010
 *      Author: kreczko
 */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/unordered_map.hpp>
#include "../interface/Readers/NTupleEventReader.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TDirectory.h"
#include "../interface/Event.h"
#include "../interface/TopPairEventCandidate.h"
#include <vector>
#include <utility>
#include "../interface/EventWeightProvider.h"
#include <iostream>
#include <string>
#include "../interface/HistHelpers/HistogramManager.h"
#include "../interface/EventCounter.h"
#include "../interface/RecoObjects/Particle.h"
#include "../interface/Analysers/HLTriggerAnalyser.h"
#include "../interface/Analysers/ElectronAnalyser.h"
#include "../interface/Analysers/MTtbarAnalyser.h"
#include "../interface/Analysers/MCAnalyser.h"
#include "../interface/Analysers/HitFitAnalyser.h"

struct InterestingEvent {
    InterestingEvent(unsigned long run, unsigned long event, std::string file) :
        candidate(), runNumber(run), eventNumber(event), fileName(file) {

    }

    InterestingEvent(BAT::TopPairEventCandidate cand, std::string file) :
        candidate(cand), runNumber(cand.runnumber()), eventNumber(cand.eventnumber()), fileName(file) {

    }
    ~InterestingEvent() {

    }
    BAT::TopPairEventCandidate candidate;
    unsigned long runNumber, eventNumber;
    std::string fileName;

    void print() {
        std::cout << "run " << candidate.runnumber() << ", event " << candidate.eventnumber() << " (Mttbar: "
                << candidate.mttbar() << ")" << std::endl;
        std::cout << "located in: " << fileName << std::endl << std::endl;
    }
};
typedef boost::array<unsigned long, BAT::TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS> ePlusJetscutarray;
typedef boost::array<unsigned long, BAT::TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS> muPlusJetscutarray;
typedef boost::unordered_map<std::string, ePlusJetscutarray> cutmap;

class Analysis {
private:
    boost::scoped_ptr<BAT::NTupleEventReader> eventReader;
    BAT::Event currentEvent;
    BAT::TopPairEventCandidate ttbarCandidate;
    boost::shared_ptr<BAT::HistogramManager> histMan;
    ePlusJetscutarray ePlusJetsCutflow;
    ePlusJetscutarray ePlusJetsSingleCuts;
    cutmap ePlusJetsCutflowPerFile;
    cutmap ePlusJetsSingleCutsPerFile;

    muPlusJetscutarray muPlusJetsCutFlow;
    muPlusJetscutarray muPlusJetsSingleCuts;
    std::vector<InterestingEvent> interestingEvents, brokenEvents;
    std::map<unsigned long, std::vector<unsigned long> > eventCheck;
    BAT::EventWeightProvider weights;
    float weight, pileUpWeight;
    BAT::Counter ePlusJetsCutflowPerSample, muPlusJetsCutflowPerSample;
    boost::scoped_ptr<BAT::HLTriggerAnalyser> hltriggerAnalyser;
    boost::scoped_ptr<BAT::ElectronAnalyser> electronAnalyser;
    boost::scoped_ptr<BAT::MTtbarAnalyser> mttbarAnalyser;
    boost::scoped_ptr<BAT::MCAnalyser> MonteCarloAnalyser;
    boost::scoped_ptr<BAT::HitFitAnalyser> hitfitAnalyser;

public:
//    static float luminosity;
    Analysis(std::string fileForPileUpReweighting);
    virtual ~Analysis();
    void analyze();
    void addInputFile(const char * fileName);
    void setMaximalNumberOfEvents(long maxEvents);
    void setUsedNeutrinoSelectionForTopPairReconstruction(BAT::NeutrinoSelectionCriterion::value selection);
    void setUsedTTbarReconstructionCriterion(BAT::TTbarReconstructionCriterion::value selection);
    bool useHitFit;

    static void usePFIsolation(bool use){
        BAT::Event::usePFIsolation = use;
    }

    static void useCustomConversionTagger(bool use){
        BAT::TopPairEventCandidate::useCustomConversionTagger = use;
        //custom conversion tagger needs track information
        BAT::NTupleEventReader::loadTracks = use;
    }

    unsigned long getNumberOfProccessedEvents() const;

private:
    void printNumberOfProccessedEventsEvery(unsigned long printEvery);
    void initiateEvent();
    void doDiElectronAnalysis();
    void doTTBarAnalysis();
    void doTTbarCutFlow();
    void doSynchExercise();
    void printInterestingEvents();
    void printSummary();
    void inspectEvents();
    void createHistograms();
    void doNotePlots();
    void doQCDStudy();
    void checkForDuplicatedEvents();
    void checkForBrokenEvents();
    void doJetAnalysis();
    void doPileUpStudy();
    void doTriggerAnalysis();
};

#endif /* ANALYSIS_H_ */
