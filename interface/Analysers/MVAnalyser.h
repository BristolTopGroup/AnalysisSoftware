/*
 * MVAnalyser.h
 *
 *  Created on: Nov 29, 2011
 *      Author: lkreczko
 */

#ifndef MVANALYSER_H_
#define MVANALYSER_H_

#include "BasicAnalyser.h"
#include <boost/shared_ptr.hpp>

namespace BAT {

class MVAnalyser: public BAT::BasicAnalyser {
public:
	MVAnalyser(HistogramManagerPtr histMan);
	virtual ~MVAnalyser();

	void analyse(const TopPairEventCandidate& event);
	void createHistograms();
};

typedef boost::shared_ptr<MVAnalyser> MVAnalyserPtr;

}

#endif /* MVANALYSER_H_ */
