/*
 * MCAnalyser.h
 *
 *  Created on: Jul 5, 2011
 *      Author: phzss
 */

#ifndef MCANALYSER_H_
#define MCANALYSER_H_
#include <boost/shared_ptr.hpp>
#include "../../interface/TopPairEventCandidate.h"
#include "../../interface/HistHelpers/HistogramManager.h"
#include "BasicAnalyser.h"

namespace BAT {

class MCAnalyser : public BasicAnalyser{
public:
    MCAnalyser(boost::shared_ptr<HistogramManager> histMan);
    virtual ~MCAnalyser();

    void analyse(const TopPairEventCandidate& ttbarEvent);
    void createHistograms();
};

}

#endif /* MCANALYSER_H_ */
