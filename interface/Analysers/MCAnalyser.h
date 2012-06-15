/*
 * MCAnalyser.h
 *
 *  Created on: Jul 5, 2011
 *      Author: phzss
 */

#ifndef MCANALYSER_H_
#define MCANALYSER_H_
#include <boost/shared_ptr.hpp>
#include "../Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../../interface/HistHelpers/HistogramManager.h"
#include "BasicAnalyser.h"
#include "../../interface/TtbarHypothesis.h"

namespace BAT {

class MCAnalyser : public BasicAnalyser{
public:
    MCAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "MCStudy");
    virtual ~MCAnalyser();

    void analyse(const EventPtr);
    void createHistograms();

    double topMass() const;
    Decay::value decay() const;
    TtbarHypothesis GetMCTTbarHypothesis() const;

private:
    TtbarHypothesis mcEvent;
    TopPairEPlusJetsReferenceSelectionPointer topEplusJetsRefSelection_;
};

}

#endif /* MCANALYSER_H_ */
