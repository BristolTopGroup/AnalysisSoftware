/*
 * HLTriggerQCDAnalyser.h
 *
 *  Created on: 25 May 2012
 *      Author: kreczko
 */

#ifndef HLTRIGGERQCDANALYSER_H_
#define HLTRIGGERQCDANALYSER_H_
#include "BasicAnalyser.h"
#include "../Event.h"
#include "../HighLevelTriggers.h"
#include "ElectronAnalyser.h"

namespace BAT {

class HLTriggerQCDAnalyser: public BasicAnalyser {
public:
	HLTriggerQCDAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "HLTQCDStudy");
	virtual ~HLTriggerQCDAnalyser();
	void analyse(const EventPtr);
	void createHistograms();
private:
	ElectronAnalyserLocalPtr eleAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_,
			eleAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_, eleAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_,
			eleAnalyser_CaloIdVT_TrkIdT_;
};

} /* namespace BAT */
#endif /* HLTRIGGERQCDANALYSER_H_ */
