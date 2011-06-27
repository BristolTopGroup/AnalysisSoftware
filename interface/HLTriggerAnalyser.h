/*
 * HLTriggerAnalyser.h
 *
 *  Created on: Jun 21, 2011
 *      Author: lkreczko
 */

#ifndef HLTRIGGERANALYSER_H_
#define HLTRIGGERANALYSER_H_
#include <boost/shared_ptr.hpp>
#include <vector>
#include <boost/array.hpp>
#include "../interface/TopPairEventCandidate.h"
#include "../interface/HighLevelTriggers.h"
#include "../interface/HistHelpers/HistogramManager.h"

namespace BAT {
class HLTriggerAnalyser {
private:
//	boost::array<unsigned long, HLTriggers::NUMBER_OF_TRIGGERS> triggerCounts;
	boost::shared_ptr<HistogramManager> histMan;
//	bool createdHistograms;

	void analyseTrigger(bool passesPreTrigger, bool passesTrigger, std::string histFolder, const JetPointer jet, int prescale);

public:
	HLTriggerAnalyser(boost::shared_ptr<HistogramManager> histMan);
	~HLTriggerAnalyser();
//	void increaseTriggerCount(HLTriggers::value trigger);
	void analyse(const TopPairEventCandidate& ttbarEvent);
	void createHistograms();

};
}
#endif /* HLTRIGGERANALYSER_H_ */
