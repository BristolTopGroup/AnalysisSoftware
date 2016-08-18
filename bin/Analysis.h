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
#include "../interface/TreeHelpers/TreeManager.h"
#include "../interface/EventCounter.h"
#include "../interface/RecoObjects/Particle.h"
//Analysers
#include "../interface/Analysers/TTbar_plus_X_analyser.h"
#include "../interface/TopPairEventCandidate.h"
#include "../interface/Analysers/PseudoTopAnalyser.h"

// #include "../interface/Analysers/VertexAnalyser.h"
#include "../interface/Analysers/UnfoldingRecoAnalyser.h"
#include "../interface/Analysers/PartonAnalyser.h"
#include "../interface/Analysers/LikelihoodInputAnalyser.h"

class Analysis {
private:
    boost::scoped_ptr<BAT::NTupleEventReader> eventReader;
    BAT::EventPtr currentEvent;
    boost::shared_ptr<BAT::TreeManager> treeMan;
    std::vector<BAT::InterestingEvent> interestingEvents;
    std::map<unsigned long, std::vector<unsigned long> > eventCheck;
    boost::shared_ptr<BAT::EventWeightProvider> weights;
    float weight, pileUpWeight;

    BAT::TTbar_plus_X_analyser_local_ptr ttbar_plus_X_analyser_;
    // boost::scoped_ptr<BAT::VertexAnalyser> vertexAnalyser;

    BAT::PseudoTopAnalyserLocalPtr pseudoTopAnalyser_;
    BAT::UnfoldingRecoAnalyserLocalPtr unfoldingRecoAnalyser_;
    BAT::PartonAnalyserLocalPtr partonAnalyser_;

    BAT::LikelihoodInputAnalyserLocalPtr likelihoodInputAnalyser_;
    std::string sampleName;
    bool passMETFilter;
    ifstream metfiles;
    std::vector<unsigned long> eventInfo;
public:
    Analysis(std::string fileForPileUpReweighting);
    virtual ~Analysis();
    void analyse();
    void addInputFile(const char * fileName);
    void setMaximalNumberOfEvents(long maxEvents);

    unsigned long getNumberOfProccessedEvents() const;

    void finishAnalysis();
    bool passMETFilters(string datatype);

private:
    void printNumberOfProccessedEventsEvery(unsigned long printEvery);
    void initiateEvent();
    void printInterestingEvents();
    void printSummary();
    void inspectEvents();
    void createHistograms();
    void checkForDuplicatedEvents();
};

#endif /* ANALYSIS_H_ */
