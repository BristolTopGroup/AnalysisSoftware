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
	WAnalyser(TreeManagerPtr treeMan, std::string histogramFolder = "WAnalysis");
	virtual ~WAnalyser();
	void analyse(const EventPtr);
	void analyseHadronicW(const EventPtr, const JetCollection jets, const JetCollection bjets, std::string folder);
	void createTrees( std::string folder);

protected:
};
typedef boost::scoped_ptr<WAnalyser> WAnalyserLocalPtr;
typedef boost::shared_ptr<WAnalyser> WAnalyserPtr;
} /* namespace BAT */
#endif /* WANALYSER_H_ */
