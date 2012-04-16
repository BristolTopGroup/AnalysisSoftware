/*
 * BJetAnalyser.h
 *
 *  Created on: 3 Jul 2011
 *      Author: kreczko
 */

#ifndef BJETANALYSER_H_
#define BJETANALYSER_H_
#include "BasicAnalyser.h"
#include <boost/scoped_ptr.hpp>

namespace BAT {

class BJetAnalyser: public BasicAnalyser {
public:
	BJetAnalyser(HistogramManagerPtr, std::string histogramFolder = "BJetAnalysis");
	virtual ~BJetAnalyser();

	void analyse(const EventPtr);
	void createHistograms();

};

typedef boost::scoped_ptr<BAT::BJetAnalyser> BJetAnalyserLocalPtr;
}

#endif /* BJETANALYSER_H_ */
