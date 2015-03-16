/*
 * Analysis.h
 *
 *  Created on: 12 Jul 2010
 *      Author: kreczko
 */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_
#include <assert.h>
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
#include "../interface/TreeHelpers/TreeManager.h"
#include "../interface/EventCounter.h"
#include "../interface/RecoObjects/Particle.h"
//Analysers
#include "../interface/Analysers/BJetAnalyser.h"
#include "../interface/Analysers/ElectronAnalyser.h"
#include "../interface/Analysers/JetAnalyser.h"
#include "../interface/Analysers/METAnalyser.h"
#include "../interface/Analysers/MuonAnalyser.h"
#include "../interface/Analysers/TTbar_plus_X_analyser.h"
#include "../interface/Analysers/VertexAnalyser.h"
#include "../interface/TopPairEventCandidate.h"

#include "../interface/Analysers/PseudoTopAnalyser.h"
#include "../interface/Analysers/UnfoldingRecoAnalyser.h"

class Analysis {
private:
    boost::scoped_ptr<BAT::NTupleEventReader> eventReader;
    BAT::EventPtr currentEvent;
    boost::shared_ptr<BAT::HistogramManager> histMan;
    boost::shared_ptr<BAT::TreeManager> treeMan;
    std::vector<BAT::InterestingEvent> interestingEvents, brokenEvents;
    std::map<unsigned long, std::vector<unsigned long> > eventCheck;
    boost::shared_ptr<BAT::EventWeightProvider> weights;
    float weight, pileUpWeight;

    boost::scoped_ptr<BAT::ElectronAnalyser> electronAnalyser;
    boost::scoped_ptr<BAT::JetAnalyser> jetAnalyser;
    boost::scoped_ptr<BAT::METAnalyser> metAnalyser;
    boost::scoped_ptr<BAT::MuonAnalyser> muonAnalyser;
    BAT::TTbar_plus_X_analyser_local_ptr ttbar_plus_X_analyser_;
    boost::scoped_ptr<BAT::VertexAnalyser> vertexAnalyser;

    BAT::PseudoTopAnalyserLocalPtr pseudoTopAnalyser_;
    BAT::UnfoldingRecoAnalyserLocalPtr unfoldingRecoAnalyser_;

public:
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
