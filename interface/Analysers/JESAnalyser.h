/*
 * JESAnalyser.h
 *
 *  Created on: 24 Feb 2015
 *      Author: doburns
 */

#ifndef JESAnalyser_H_
#define JESAnalyser_H_

#include "BasicAnalyser.h"

namespace BAT {

class JESAnalyser: public BAT::BasicAnalyser {
public:
	JESAnalyser(TreeManagerPtr treeMan, std::string histogramFolder = "TTbar_plus_X_analysis/JESAnalyser");
	virtual ~JESAnalyser();
	void analyse(const EventPtr);

	void createTrees();

protected:
	void fillHistograms(std::string subcollection, std::string suffix = "");
};
typedef boost::scoped_ptr<JESAnalyser> JESAnalyserLocalPtr;
typedef boost::shared_ptr<JESAnalyser> JESAnalyserPtr;
} /* namespace BAT */
#endif /* JESAnalyser_H_ */
