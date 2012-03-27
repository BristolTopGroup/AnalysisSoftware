/*
 * TTbarPlusMETAnalyser.h
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#ifndef TTBARPLUSMETANALYSER_H_
#define TTBARPLUSMETANALYSER_H_

#include "BasicAnalyser.h"
#include "METAnalyser.h"
#include <string>

namespace BAT {

class TTbarPlusMETAnalyser: public BAT::BasicAnalyser {
public:
	TTbarPlusMETAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "TTbarPlusMetAnalysis");
	virtual ~TTbarPlusMETAnalyser();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();

private:
	BasicAnalyserLocalPtr metAnalyser;
};

typedef boost::scoped_ptr<BAT::TTbarPlusMETAnalyser> TTbarPlusMETAnalyserLocalPtr;
} /* namespace BAT */
#endif /* TTBARPLUSMETANALYSER_H_ */
