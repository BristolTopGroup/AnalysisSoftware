/*
 * MVAnalyser.h
 *
 *  Created on: Nov 29, 2011
 *      Author: lkreczko
 */

#ifndef MVANALYSER_H_
#define MVANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class MVAnalyser: public BAT::BasicAnalyser {
public:
	MVAnalyser(HistogramManagerPtr histMan);
	virtual ~MVAnalyser();

	void analyse(const TopPairEventCandidate& event);
	void createHistograms();
};

}

#endif /* MVANALYSER_H_ */
