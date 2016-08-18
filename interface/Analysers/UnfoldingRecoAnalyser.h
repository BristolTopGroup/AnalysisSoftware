/*
 * UnfoldingRecoAnalyser.h
 *
 *  Created on: 24 Feb 2015
 *      Author: ejclemen
 */

#ifndef UnfoldingRecoAnalyser_H_
#define UnfoldingRecoAnalyser_H_

#include "BasicAnalyser.h"

namespace BAT {

class UnfoldingRecoAnalyser: public BAT::BasicAnalyser {
public:
	UnfoldingRecoAnalyser(TreeManagerPtr treeMan, std::string histogramFolder = "TTbar_plus_X_analysis/Unfolding");
	virtual ~UnfoldingRecoAnalyser();
	void analyse(const EventPtr);

	void createTrees();

protected:
	void fillHistograms(std::string subcollection, std::string suffix = "");
};
typedef boost::scoped_ptr<UnfoldingRecoAnalyser> UnfoldingRecoAnalyserLocalPtr;
typedef boost::shared_ptr<UnfoldingRecoAnalyser> UnfoldingRecoAnalyserPtr;
} /* namespace BAT */
#endif /* UnfoldingRecoAnalyser_H_ */
