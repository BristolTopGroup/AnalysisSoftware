/*
 * TopReconstructionAnalyser.h
 *
 *  Created on: Nov 24, 2011
 *      Author: lkreczko
 */

#ifndef TOPRECONTRUCTIONANALYSER_H_
#define TOPRECONTRUCTIONANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class TopReconstructionAnalyser: public BAT::BasicAnalyser {
public:
	TopReconstructionAnalyser(boost::shared_ptr<HistogramManager>);
	virtual ~TopReconstructionAnalyser();

	void analyse(const TopPairEventCandidate& ttbarEvent);
	void createHistograms();

//	std::vector<InterestingEvent> interestingEvents;
};

}

#endif /* TTBARANALYSER_H_ */
