/*
 * ElectronAnalyser.h
 *
 *  Created on: 3 Jul 2011
 *      Author: kreczko
 */

#ifndef ELECTRONANALYSER_H_
#define ELECTRONANALYSER_H_
#include "BasicAnalyser.h"
#include <boost/shared_ptr.hpp>

namespace BAT {

class ElectronAnalyser: public BasicAnalyser {
public:
	ElectronAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "ElectronAnalysis",
			bool singleElectronOnly = false);
	virtual ~ElectronAnalyser();

	/**
	 * Analyses all electrons in an event
	 */
	void analyse(const EventPtr);
	/**
	 * Analyses a single electron
	 */
	void analyseElectron(const ElectronPointer electron, double weight);
	void createHistograms();

	void useTTbarPlusMETSetup(bool use);
private:
	bool singleElectronOnly_;
	bool ttbarPlusMETAnalysisSetup_;
};
typedef boost::scoped_ptr<ElectronAnalyser> ElectronAnalyserLocalPtr;
typedef boost::shared_ptr<ElectronAnalyser> ElectronAnalyserPtr;
}

#endif /* ELECTRONANALYSER_H_ */
