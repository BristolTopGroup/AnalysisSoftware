/*
 * JetAnalyser.h
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#ifndef JETANALYSER_H_
#define JETANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class JetAnalyser: public BAT::BasicAnalyser {
public:
	JetAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "JetAnalysis");
	virtual ~JetAnalyser();

	void analyse(const EventPtr);
//	void analyseJets(JetCollection jets, double weight);
	void createHistograms();
};

typedef boost::scoped_ptr<JetAnalyser> JetAnalyserLocalPtr;

} /* namespace BAT */
#endif /* JETANALYSER_H_ */
