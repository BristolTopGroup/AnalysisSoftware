/*
 * WAnalyser.h
 *
 *  Created on: 12 Jan 2015
 *      Author: ejclemen
 */

#ifndef WANALYSER_H_
#define WANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class WAnalyser: public BAT::BasicAnalyser {
public:
	WAnalyser(HistogramManagerPtr histMan, TreeManagerPtr treeMan, std::string histogramFolder = "WAnalysis");
	virtual ~WAnalyser();
	void analyse(const EventPtr);
	void analyseHadronicW(const EventPtr, const JetCollection jets, const JetCollection bjets);
	void analyseHadronicW_partons(const EventPtr);

	void createHistograms();
	void createTrees();

protected:
	void fillHistograms(std::string subcollection, std::string suffix = "");
};
typedef boost::scoped_ptr<WAnalyser> WAnalyserLocalPtr;
typedef boost::shared_ptr<WAnalyser> WAnalyserPtr;
} /* namespace BAT */
#endif /* WANALYSER_H_ */
