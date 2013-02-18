/*
 * EventCountAnalyser.h
 *
 *  Created on: 24 Mar 2011
 *      Author: kreczko
 */

#ifndef EVENTCOUNTANALYSER_H_
#define EVENTCOUNTANALYSER_H_

#include "BasicAnalyser.h"
#include "../Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../Selections/TopPairEplusJetsRefAsymJetsSelection.h"
#include "../Selections/TopPairEPlusJetsRefAsymJetsMETSelection.h"
#include "../Selections/TopPairMuPlusJetsReferenceSelection.h"
#include "../Selections/TopPairMuPlusJetsReferenceSelection2011.h"
//QCD selections
#include "../Selections/QCDNonIsolatedElectronSelection.h"
#include "../Selections/QCDNonIsolatedMuonSelection.h"
#include "../Selections/QCDConversionsSelection.h"
#include "../Selections/QCDPFRelIsoEPlusJetsSelection.h"
#include "../Selections/QCDPFRelIsoMuPlusJetsSelection.h"
//QCD selections with AsymJets
#include "../Selections/QCDNonIsolatedElectronAsymJetsSelection.h"
#include "../Selections/QCDPFRelIsoEPlusAsymJetsSelection.h"
#include "../Selections/QCDConversionsAsymJetsSelection.h"
//QCD selections with AsymJets & MET cuts
#include "../Selections/QCDNonIsolatedElectronAsymJetsMETSelection.h"
#include "../Selections/QCDPFRelIsoEPlusAsymJetsMET.h"
#include "../Selections/QCDConversionsAsymJetsMETSelection.h"
namespace BAT {

class EventCountAnalyser: public BAT::BasicAnalyser {
public:
	EventCountAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "EventCount");
	virtual ~EventCountAnalyser();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();
	void setHistogramLabels();

	void topEPlusJetsReferenceSelection(const EventPtr event);
	void topEPlusJetsReferenceSelectionUnweighted(const EventPtr event);
	void topMuPlusJetsReferenceSelection(const EventPtr event);
	void topMuPlusJetsReferenceSelectionUnweighted(const EventPtr event);
	void topMuPlusJetsReferenceSelection2011(const EventPtr event);
	void topEplusJetsPlusMETSelection(const EventPtr event);
	void topEplusJetsZprimeSelection(const EventPtr event);

	void qcdSelections(const EventPtr event);
	void qcdMuPlusJetsSelections(const EventPtr event);
	void qcdAsymJetsSelections(const EventPtr event);
	void qcdAsymJetsMETSelections(const EventPtr event);
	void qcdNonIsoTriggerSelections(const EventPtr event);
	void qcdNonIsoTriggerAsymJetsSelections(const EventPtr event);
	void qcdNonIsoTriggerAsymJetsMETSelections(const EventPtr event);
	void individualCuts(const EventPtr event);

private:
	SelectionPointer topEplusJetsRefSelection_, topEplusAsymJetsSelection_, topEplusAsymJetsMETSelection_, topMuPlusJetsRefSelection_, topMuPlusJetsRefSelection2011_;

	//QCD selections with respect to reference selection
	SelectionPointer qcdNonIsoElectronSelection_, qcdNonIsoElectronNonIsoTriggerSelection_, qcdNonIsoMuonSelection_;
	SelectionPointer qcdConversionSelection_;
	SelectionPointer qcdPFRelIsoEPlusJetsSelection_, qcdPFRelIsoNonIsoTriggerSelection_, qcdPFRelIsoMuPlusJetsSelection_;
	//QCD selections with respect to reference selection + asymmetric jet cuts
	SelectionPointer qcdNonIsoElectronAsymJetsSelection_, qcdNonIsoElectronAsymJetsNonIsoTriggerSelection_;
	SelectionPointer qcdConversionAsymJetsSelection_;
	SelectionPointer qcdPFRelIsoAsymJetsSelection_, qcdPFRelIsoNonIsoTriggerAsymJetsSelection_;
	//QCD selections with respect to reference selection + MET + asymmetric jet cuts
	SelectionPointer qcdNonIsoElectronAsymJetsMETSelection_, qcdNonIsoElectronAsymJetsMETNonIsoTriggerSelection_;
	SelectionPointer qcdConversionAsymJetsMETSelection_;
	SelectionPointer qcdPFRelIsoAsymJetsMETSelection_, qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_;
};

} /* namespace BAT */
#endif /* EVENTCOUNTANALYSER_H_ */

