/*
 * SelectionAnalyser.h
 *
 *  Created on: 15 Mar 2012
 *      Author: kreczko
 */

#ifndef SELECTIONANALYSER_H_
#define SELECTIONANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class SelectionAnalyser: public BAT::BasicAnalyser {
public:
	SelectionAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "SelectionAnalysis");
	virtual ~SelectionAnalyser();
	void analyse(const EventPtr);
	void createHistograms();

private:
	void analyseAngles(const EventPtr);
};

} /* namespace BAT */
#endif /* SELECTIONANALYSER_H_ */
