/*
 * PseudoTopAnalyser.h
 *
 *  Created on: 24 Feb 2015
 *      Author: ejclemen
 */

#ifndef PseudoTopAnalyser_H_
#define PseudoTopAnalyser_H_

#include "BasicAnalyser.h"

namespace BAT {

class PseudoTopAnalyser: public BAT::BasicAnalyser {
public:
	PseudoTopAnalyser(HistogramManagerPtr histMan, TreeManagerPtr treeMan, std::string histogramFolder = "TTbar_plus_X_analysis/PseudoTop");
	virtual ~PseudoTopAnalyser();
	void analyse(const EventPtr);

	void createHistograms();
	void createTrees();

protected:
	void fillHistograms(std::string subcollection, std::string suffix = "");
};
typedef boost::scoped_ptr<PseudoTopAnalyser> PseudoTopAnalyserLocalPtr;
typedef boost::shared_ptr<PseudoTopAnalyser> PseudoTopAnalyserPtr;
} /* namespace BAT */
#endif /* PseudoTopAnalyser_H_ */
