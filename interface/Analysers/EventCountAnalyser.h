/*
 * EventCountAnalyser.h
 *
 *  Created on: 24 Mar 2012
 *      Author: kreczko
 */

#ifndef EVENTCOUNTANALYSER_H_
#define EVENTCOUNTANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class EventCountAnalyser: public BAT::BasicAnalyser {
public:
	EventCountAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "EventCount");
	virtual ~EventCountAnalyser();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();
};

} /* namespace BAT */
#endif /* EVENTCOUNTANALYSER_H_ */
