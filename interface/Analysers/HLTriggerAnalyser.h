/*
 * HLTriggerAnalyser.h
 *
 *  Created on: Jun 21, 2011
 *      Author: lkreczko
 */

#ifndef HLTRIGGERANALYSER_H_
#define HLTRIGGERANALYSER_H_
#include <boost/shared_ptr.hpp>
#include "../../interface/TopPairEventCandidate.h"
#include "../../interface/HistHelpers/HistogramManager.h"
#include "../../interface/HighLevelTriggers.h"

namespace BAT {
class HLTriggerAnalyser {
private:
	boost::shared_ptr<HistogramManager> histMan;

	void analyseTrigger(bool passesPreTrigger, bool passesTrigger, std::string histFolder, const JetPointer jet,
			int prescale);

public:
	HLTriggerAnalyser(boost::shared_ptr<HistogramManager> histMan);
	~HLTriggerAnalyser();
	void analyse(const TopPairEventCandidate& ttbarEvent);
	void createHistograms();

};
}
#endif /* HLTRIGGERANALYSER_H_ */
