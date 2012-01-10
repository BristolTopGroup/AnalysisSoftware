/*
 * METAnalyser.h
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#ifndef METANALYSER_H_
#define METANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class METAnalyser: public BAT::BasicAnalyser {
public:
	METAnalyser(HistogramManagerPtr histMan);
	virtual ~METAnalyser();

	void analyse(const TopPairEventCandidate& ttbarEvent);
	void createHistograms();
};

} /* namespace BAT */
#endif /* METANALYSER_H_ */
