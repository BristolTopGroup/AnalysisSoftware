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
#include "METAnalyser.h"
#include "../Selections/QCDNonIsolatedElectronSelection.h"
#include "../Selections/QCDAntiIDEPlusJetsSelection.h"
#include "../Selections/TopPairEPlusJetsReferenceSelection.h"

namespace BAT {

class HLTriggerQCDAnalyser: public BasicAnalyser {
public:
	HLTriggerQCDAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "HLTQCDStudy", bool studyExclusiveEvents = false);
	virtual ~HLTriggerQCDAnalyser();
	void analyse(const EventPtr);
	void createHistograms();
private:
	bool passesNonIsoWithoutBtagAndHLT(const EventPtr event) const;
	bool passesAntiIDWithoutBtagAndHLT(const EventPtr event) const;
	bool passesSignalSelectionWithoutBtagAndHLT(const EventPtr event) const;
	bool passesTriggerAnalysisSelection(const EventPtr event) const;

	bool studyExclusiveEvents_;
	ElectronAnalyserLocalPtr eleAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_,
			eleAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_, eleAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_,
			eleAnalyser_CaloIdVT_TrkIdT_;
	METAnalyserLocalPtr metNonIsoRegionAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_,
			metNonIsoRegionAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_, metNonIsoRegionAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_,
			metNonIsoRegionAnalyser_CaloIdVT_TrkIdT_;
	METAnalyserLocalPtr metAntiIDRegionAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_,
			metAntiIDRegionAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_, metAntiIDRegionAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_,
			metAntiIDRegionAnalyser_CaloIdVT_TrkIdT_;
	QCDNonIsolatedElectronSelectionPointer qcdNonIsoSelection_;
	unsigned long QCDNonIsoRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_,
			QCDNonIsoRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_,
			QCDNonIsoRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_, QCDNonIsoRegionCount_CaloIdVT_TrkIdT_;

	QCDAntiIDEPlusJetsSelectionPointer qcdAntiIDSelection_;
	unsigned long QCDAntiIDRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_,
			QCDAntiIDRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_,
			QCDAntiIDRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_, QCDAntiIDRegionCount_CaloIdVT_TrkIdT_;

	TopPairEPlusJetsReferenceSelectionPointer topSignalSelection_;
	unsigned long TopSignalRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_,
			TopSignalRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_,
			TopSignalRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_, TopSignalRegionCount_CaloIdVT_TrkIdT_;
	unsigned long TopSignal_TwoBtagsRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_,
			TopSignal_TwoBtagsRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_,
			TopSignal_TwoBtagsRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_,
			TopSignal_TwoBtagsRegionCount_CaloIdVT_TrkIdT_;

};

} /* namespace BAT */
#endif /* HLTRIGGERQCDANALYSER_H_ */
