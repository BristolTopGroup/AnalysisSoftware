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
#include "BasicAnalyser.h"

namespace BAT {
class HLTriggerAnalyser : public BasicAnalyser {
private:
	void analyseTrigger(bool passesPreTrigger, bool passesTrigger, std::string histFolder, const JetPointer jet,
			int prescale);

public:
	HLTriggerAnalyser(boost::shared_ptr<HistogramManager> histMan);
	virtual ~HLTriggerAnalyser();
	void analyse(const TopPairEventCandidate& ttbarEvent);
	void createHistograms();

};
}
#endif /* HLTRIGGERANALYSER_H_ */
