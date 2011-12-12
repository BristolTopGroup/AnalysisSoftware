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

namespace AnalysisReference{
enum value{
	Ele30_TriPFJet30,
	Ele30_QuadPFJet30,
	Ele30_PFJet70_PFJet50_PFJet30,
	Ele30_PFJet70_PFJet50_PFJet30_PFJet30,
	NUMBER_OF_TRIGGEREFFICIENCY_CASES
};
}
class HLTriggerAnalyser : public BasicAnalyser {
private:
	void analyseTrigger(bool passesPreCondition, bool passesTrigger, std::string histFolder, const JetPointer jet,
			int prescale);

	void analyseTriggerEfficiency(AnalysisReference::value analysis, std::string triggerName, bool passesTrigger, const TopPairEventCandidate& ttbarEvent);
	double weight;

public:
	HLTriggerAnalyser(boost::shared_ptr<HistogramManager> histMan);
	virtual ~HLTriggerAnalyser();
	void analyse(const TopPairEventCandidate& ttbarEvent);
	void createHistograms();

};
}
#endif /* HLTRIGGERANALYSER_H_ */
