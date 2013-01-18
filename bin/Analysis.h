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
#include <vector>
#include <utility>
#include "../interface/EventWeightProvider.h"
#include <iostream>
#include <string>
#include "../interface/HistHelpers/HistogramManager.h"
#include "../interface/EventCounter.h"
#include "../interface/RecoObjects/Particle.h"
//Analysers
#include "../interface/Analysers/ABCDMethodAnalyser.h"
#include "../interface/Analysers/DiElectronAnalyser.h"
#include "../interface/Analysers/BJetAnalyser.h"
#include "../interface/Analysers/ElectronAnalyser.h"
#include "../interface/Analysers/EventCountAnalyser.h"
//#include "../interface/Analysers/HitFitAnalyser.h"
#include "../interface/Analysers/HLTriggerTurnOnAnalyser.h"
#include "../interface/Analysers/HLTriggerQCDAnalyser.h"
#include "../interface/Analysers/JetAnalyser.h"
#include "../interface/Analysers/MCAnalyser.h"
#include "../interface/Analysers/METAnalyser.h"
#include "../interface/Analysers/MTtbarAnalyser.h"
#include "../interface/Analysers/MuonAnalyser.h"
#include "../interface/Analysers/MVAnalyser.h"
#include "../interface/Analysers/NeutrinoReconstructionAnalyser.h"
#include "../interface/Analysers/TTbar_plus_X_analyser.h"
#include "../interface/Analysers/VertexAnalyser.h"
#include "../interface/Analysers/DiffVariablesAnalyser.h"
#include "../interface/Analysers/BinningAnalyser.h"

//typedef boost::array<unsigned long, BAT::TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS> ePlusJetscutarray;
//typedef boost::array<unsigned long, BAT::TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS> muPlusJetscutarray;
//typedef boost::unordered_map<std::string, ePlusJetscutarray> cutmap;

class Analysis {
private:
    boost::scoped_ptr<BAT::NTupleEventReader> eventReader;
    BAT::EventPtr currentEvent;
    boost::shared_ptr<BAT::HistogramManager> histMan;
//    ePlusJetscutarray ePlusJetsCutflow;
//    ePlusJetscutarray ePlusJetsSingleCuts;
//    cutmap ePlusJetsCutflowPerFile;
//    cutmap ePlusJetsSingleCutsPerFile;
//
//    muPlusJetscutarray muPlusJetsCutFlow;
//    muPlusJetscutarray muPlusJetsSingleCuts;
    std::vector<BAT::InterestingEvent> interestingEvents, brokenEvents;
    std::map<unsigned long, std::vector<unsigned long> > eventCheck;
    boost::shared_ptr<BAT::EventWeightProvider> weights;
    float weight, pileUpWeight;
//    BAT::Counter ePlusJetsCutflowPerSample, muPlusJetsCutflowPerSample;

    BAT::ABCDMethodAnalyserLocalPtr abcdMethodAnalyser_;
    BAT::BJetAnalyserLocalPtr bjetAnalyser;
    boost::scoped_ptr<BAT::DiElectronAnalyser> diElectronAnalyser;
    boost::scoped_ptr<BAT::ElectronAnalyser> electronAnalyser;
    BAT::BasicAnalyserLocalPtr eventcountAnalyser;
//    boost::scoped_ptr<BAT::HitFitAnalyser> hitfitAnalyser;
    boost::scoped_ptr<BAT::HLTriggerTurnOnAnalyser> hltriggerAnalyser;
    BAT::BasicAnalyserLocalPtr hltriggerQCDAnalyserInclusive_, hltriggerQCDAnalyserExclusive_;
    boost::scoped_ptr<BAT::JetAnalyser> jetAnalyser;
    boost::scoped_ptr<BAT::MCAnalyser> mcAnalyser;
    boost::scoped_ptr<BAT::METAnalyser> metAnalyser;
    boost::scoped_ptr<BAT::MTtbarAnalyser> mttbarAnalyser;
    boost::scoped_ptr<BAT::MuonAnalyser> muonAnalyser;
    BAT::MVAnalyserPtr mvAnalyser;
    BAT::NeutrinoRecoAnalyserLocalPtr neutrinoRecoAnalyser;
    BAT::TTbar_plus_X_analyser_local_ptr ttbar_plus_X_analyser_;
    boost::scoped_ptr<BAT::VertexAnalyser> vertexAnalyser;
    boost::scoped_ptr<BAT::DiffVariablesAnalyser> diffVariablesAnalyser;
    boost::scoped_ptr<BAT::BinningAnalyser> binningAnalyser;

public:
//    static float luminosity;
    Analysis(std::string fileForPileUpReweighting);
    virtual ~Analysis();
    void analyse();
    void addInputFile(const char * fileName);
    void setMaximalNumberOfEvents(long maxEvents);

    unsigned long getNumberOfProccessedEvents() const;

    void finishAnalysis();

private:
    void printNumberOfProccessedEventsEvery(unsigned long printEvery);
    void initiateEvent();
    void printInterestingEvents();
    void printSummary();
    void inspectEvents();
    void createHistograms();
    void checkForDuplicatedEvents();
    void checkForBrokenEvents();
};

#endif /* ANALYSIS_H_ */
