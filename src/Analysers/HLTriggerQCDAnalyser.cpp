/*
 * HLTriggerQCDAnalyser.cpp
 *
 *  Created on: 25 May 2012
 *      Author: kreczko
 */

#include "../../interface/Analysers/HLTriggerQCDAnalyser.h"

namespace BAT {
void HLTriggerQCDAnalyser::analyse(const EventPtr event) {

	//only do this analysis for runs above 193834 as previous runs don't have all triggers
	if (!(event->runnumber() >= 193834 && event->runnumber() <= 194076))
		return;

	if (event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30)) {
		int prescale = event->HLTPrescale(
				(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30));
		eleAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->setPrescale(prescale);
		eleAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->analyse(event);
	}

	if (event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30)) {
		int prescale = event->HLTPrescale(
				(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30));
		eleAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->setPrescale(prescale);
		eleAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->analyse(event);
	}

	if (event->HLT(HLTriggers::HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30)) {
		int prescale = event->HLTPrescale(
				(HLTriggers::HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30));
		eleAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->setPrescale(prescale);
		eleAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->analyse(event);
	}

	if (event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30)) {
		int prescale = event->HLTPrescale((HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30));
		eleAnalyser_CaloIdVT_TrkIdT_->setPrescale(prescale);
		eleAnalyser_CaloIdVT_TrkIdT_->analyse(event);
	}
}

void HLTriggerQCDAnalyser::createHistograms() {
	eleAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->createHistograms();
	eleAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->createHistograms();
	eleAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->createHistograms();
	eleAnalyser_CaloIdVT_TrkIdT_->createHistograms();
}

HLTriggerQCDAnalyser::HLTriggerQCDAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		eleAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30")), //
		eleAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30")), //
		eleAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30")), //
		eleAnalyser_CaloIdVT_TrkIdT_(
				new ElectronAnalyser(histMan, histogramFolder + "/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30")) {

}

HLTriggerQCDAnalyser::~HLTriggerQCDAnalyser() {
}

} /* namespace BAT */
