/*
 * LikelihoodRecoAnalyser.h
 *
 *  Created on: Mey 13, 2015
 *      Author: ejclemen
 */

#ifndef LIKELIHOODRECOANALYSER_H_
#define LIKELIHOODRECOANALYSER_H_
#include <fstream>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "../../interface/TtbarHypothesis.h"
#include "../../interface/ReconstructionModules/LikelihoodBasedTopPairReconstruction.h"
#include "BasicAnalyser.h"


namespace BAT {

class LikelihoodRecoAnalyser: public BasicAnalyser {
private:
	bool isElectronChannel_;

private:

public:
	LikelihoodRecoAnalyser(TreeManagerPtr treeMan, const bool isElectronChannel, std::string histogramFolder = "likelihoodReco" );
	virtual ~LikelihoodRecoAnalyser();

	void analyse(const EventPtr);
	void analyse(const EventPtr, const JetCollection jets, const JetCollection bjets, const LeptonPointer selectedLepton, const METPointer met );
	void createTrees();	
};
typedef boost::scoped_ptr<LikelihoodRecoAnalyser> LikelihoodRecoAnalyserLocalPtr;
typedef boost::shared_ptr<LikelihoodRecoAnalyser> LikelihoodRecoAnalyserPtr;
}

#endif /* LIKELIHOODRECOANALYSER_H_ */
