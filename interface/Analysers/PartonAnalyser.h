/*
 * PartonAnalyser.h
 *
 *  Created on: 24 Feb 2015
 *      Author: ejclemen
 */

#ifndef PartonAnalyser_H_
#define PartonAnalyser_H_

#include "BasicAnalyser.h"

namespace BAT {

class PartonAnalyser: public BAT::BasicAnalyser {
public:
	PartonAnalyser(HistogramManagerPtr histMan, TreeManagerPtr treeMan, std::string histogramFolder = "TTbar_plus_X_analysis/Unfolding");
	virtual ~PartonAnalyser();
	void analyse(const EventPtr);

	void createHistograms();
	void createTrees();

protected:
	void fillHistograms(std::string subcollection, std::string suffix = "");
};
typedef boost::scoped_ptr<PartonAnalyser> PartonAnalyserLocalPtr;
typedef boost::shared_ptr<PartonAnalyser> PartonAnalyserPtr;
} /* namespace BAT */
#endif /* PartonAnalyser_H_ */
