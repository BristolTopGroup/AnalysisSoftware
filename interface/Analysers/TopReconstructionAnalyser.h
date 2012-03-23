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
	TopReconstructionAnalyser(HistogramManagerPtr);
	virtual ~TopReconstructionAnalyser();

	void analyse(const EventPtr);
	void createHistograms();

};

}

#endif /* TTBARANALYSER_H_ */
