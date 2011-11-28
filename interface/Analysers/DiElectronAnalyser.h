/*
 * DiElectronAnalyser.h
 *
 *  Created on: Nov 24, 2011
 *      Author: lkreczko
 */

#ifndef DIELECTRONANALYSER_H_
#define DIELECTRONANALYSER_H_

#include "BasicAnalyser.h"
#include <boost/shared_ptr.hpp>
#include "../../interface/TopPairEventCandidate.h"
#include "../../interface/HistHelpers/HistogramManager.h"

namespace BAT {

class DiElectronAnalyser: public BAT::BasicAnalyser {
public:
	DiElectronAnalyser(boost::shared_ptr<HistogramManager> histMan);
	virtual ~DiElectronAnalyser();

    void analyse(const TopPairEventCandidate& ttbarEvent);
    void createHistograms();
};

}

#endif /* DIELECTRONANALYSER_H_ */
