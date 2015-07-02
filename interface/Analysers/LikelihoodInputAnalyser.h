/*
 * LikelihoodInputAnalyser.h
 *
 *  Created on: May 5, 2015
 *      Author: ejclemen
 */

#ifndef LIKELIHOODINPUTANALYSER_H_
#define LIKELIHOODINPUTANALYSER_H_
#include <fstream>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "../../interface/TtbarHypothesis.h"
#include "BasicAnalyser.h"
#include "../Rochester/NeutrinoSolver.h"

namespace BAT {

class LikelihoodInputAnalyser: public BasicAnalyser {

public:
	LikelihoodInputAnalyser(HistogramManagerPtr histMan, TreeManagerPtr treeMan, std::string histogramFolder = "likelihood" );
	virtual ~LikelihoodInputAnalyser();

	void analyse(const EventPtr);
	void createHistograms();
	void createTrees();
};
typedef boost::scoped_ptr<LikelihoodInputAnalyser> LikelihoodInputAnalyserLocalPtr;
typedef boost::shared_ptr<LikelihoodInputAnalyser> LikelihoodInputAnalyserPtr;
}

#endif /* LIKELIHOODINPUTANALYSER_H_ */
