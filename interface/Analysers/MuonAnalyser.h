/*
 * MuonAnalyser.h
 *  Created on: 1 Nov 2011
 *  Last modified on: 10 Aug 2012
 *      Author: kreczko
 */

#ifndef MUONANALYSER_H_
#define MUONANALYSER_H_
#include "BasicAnalyser.h"
#include <boost/shared_ptr.hpp>

namespace BAT {

class MuonAnalyser: public BasicAnalyser {
public:
	MuonAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "MuonAnalysis",
			bool singleMuonOnly = false);
	virtual ~MuonAnalyser();

	/**
	 * Analyses all muons in an event
	 */
	void analyse(const EventPtr);
	/**
	 * Analyses a single muon
	 */
	void analyseMuon(const MuonPointer muon, double weight);
	void createHistograms();

	void useTTbarPlusMETSetup(bool use);
private:
	bool singleMuonOnly_;
	bool ttbarPlusMETAnalysisSetup_;
};
typedef boost::scoped_ptr<MuonAnalyser> MuonAnalyserLocalPtr;
typedef boost::shared_ptr<MuonAnalyser> MuonAnalyserPtr;
}

#endif /* MUONANALYSER_H_ */
