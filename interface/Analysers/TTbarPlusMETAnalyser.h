/*
 * TTbarPlusMETAnalyser.h
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#ifndef TTBARPLUSMETANALYSER_H_
#define TTBARPLUSMETANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class TTbarPlusMETAnalyser: public BAT::BasicAnalyser {
public:
	TTbarPlusMETAnalyser(HistogramManagerPtr);
	virtual ~TTbarPlusMETAnalyser();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();
};

typedef boost::scoped_ptr<BAT::TTbarPlusMETAnalyser> TTbarPlusMETAnalyserLocalPtr;
} /* namespace BAT */
#endif /* TTBARPLUSMETANALYSER_H_ */
