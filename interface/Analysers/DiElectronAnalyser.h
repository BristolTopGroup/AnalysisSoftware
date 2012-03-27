/*
 * DiElectronAnalyser.h
 *
 *  Created on: Nov 24, 2011
 *      Author: lkreczko
 */

#ifndef DIELECTRONANALYSER_H_
#define DIELECTRONANALYSER_H_

#include "BasicAnalyser.h"
#include "../../interface/TopPairEventCandidate.h"
#include "../../interface/HistHelpers/HistogramManager.h"

namespace BAT {

class DiElectronAnalyser: public BAT::BasicAnalyser {
public:
	DiElectronAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "DiElectronAnalysis");
	virtual ~DiElectronAnalyser();

    void analyse(const EventPtr);
    void createHistograms();
};

}

#endif /* DIELECTRONANALYSER_H_ */
