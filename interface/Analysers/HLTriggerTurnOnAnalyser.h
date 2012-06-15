/*
 * HLTriggerAnalyser.h
 *
 *  Created on: Jun 21, 2011
 *      Author: lkreczko
 */

#ifndef HLTRIGGERANALYSER_H_
#define HLTRIGGERANALYSER_H_
#include <boost/shared_ptr.hpp>
#include "../Event.h"
#include "../HistHelpers/HistogramManager.h"
#include "../HighLevelTriggers.h"
#include "BasicAnalyser.h"
#include "boost/array.hpp"
#include "../DataTypes.h"
#include "Efficiency.h"
#include "../Selections/TopPairEPlusJetsReferenceSelection.h"

namespace BAT {

namespace AnalysisReference {
enum value {
	Ele30_TriPFJet30,
	Ele30_QuadPFJet30,
//	Ele30_PFJet70_PFJet50_PFJet30,
//	Ele30_PFJet70_PFJet50_PFJet30_PFJet30,
	NUMBER_OF_TRIGGEREFFICIENCY_CASES
};
}
class HLTriggerTurnOnAnalyser: public BasicAnalyser {

public:
	HLTriggerTurnOnAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "HLTStudy");
	virtual ~HLTriggerTurnOnAnalyser();
	void analyse(const EventPtr);
	void createHistograms();
	void createHistograms(std::string trigger);

private:
	void analyseTrigger(bool passesPreCondition, bool passesTrigger, std::string histFolder, const JetPointer jet,
			int prescale);

	void analyseTriggerEfficiency(AnalysisReference::value analysis, std::string triggerName, bool passesTrigger,
			const EventPtr event);
	double weight;
	//for trigger efficiency
	boost::array<Efficiency, AnalysisReference::NUMBER_OF_TRIGGEREFFICIENCY_CASES> triggerEfficiencies;
	TopPairEPlusJetsReferenceSelectionPointer topEplusJetsRefSelection_;
};

}
#endif /* HLTRIGGERANALYSER_H_ */
