/*
 * TTbarPlusMETAnalyser.h
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#ifndef TTBARPLUSMETANALYSER_H_
#define TTBARPLUSMETANALYSER_H_

#include "BasicAnalyser.h"
#include "METAnalyser.h"
#include "ElectronAnalyser.h"
#include "VertexAnalyser.h"
#include "../Selections/BasicSelection.h"

#include <string>

namespace BAT {

class TTbarPlusMETAnalyser: public BAT::BasicAnalyser {
public:
	TTbarPlusMETAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "TTbarEplusJetsPlusMetAnalysis");
	virtual ~TTbarPlusMETAnalyser();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();
	void qcdAnalysis(const EventPtr);
	void qcdAnalysisAsymJets(const EventPtr event);
	void qcdAnalysisAsymJetsMET(const EventPtr event);
	void signalAnalysis(const EventPtr);

private:
	//signal selections
	SelectionPointer topEplusJetsRefSelection_, topEplusAsymJetsSelection_, topEplusAsymJetsMETSelection_;
	//QCD selections with respect to reference selection
	SelectionPointer qcdNonIsoElectronSelection_, qcdNonIsoElectronNonIsoTriggerSelection_;
	SelectionPointer qcdConversionSelection_;
	SelectionPointer qcdPFRelIsoSelection_, qcdPFRelIsoNonIsoTriggerSelection_;
	//QCD selections with respect to reference selection + asymmetric jet cuts
	SelectionPointer qcdNonIsoElectronAsymJetsSelection_, qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_;
	SelectionPointer qcdConversionAsymJetsSelection_;
	SelectionPointer qcdPFRelIsoAsymJetsSelection_, qcdPFRelIsoNonIsoTriggerAsymJetsSelection_;
	//QCD selections with respect to reference selection + MET + asymmetric jet cuts
	SelectionPointer qcdNonIsoElectronAsymJetsMETSelection_, qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_;
	SelectionPointer qcdConversionAsymJetsMETSelection_;
	SelectionPointer qcdPFRelIsoAsymJetsMETSelection_, qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_;

	/**
	 * Analysers
	 */
	//signal regions
	METAnalyserLocalPtr metAnalyserRefSelection_, metAnalyserRefAsymJetsMETSelection_, metAnalyserRefAsymJetsSelection_;
	ElectronAnalyserLocalPtr electronAnalyserRefSelection_, electronAnalyserRefAsymJetsMETSelection_, electronAnalyserRefAsymJetsSelection_;
	BasicAnalyserLocalPtr vertexAnalyserRefSelection_;

	//QCD region
	METAnalyserLocalPtr metAnalyserqcdNonIsoSelection_, metAnalyserqcdNonIsoNonIsoTriggerSelection_;
	ElectronAnalyserLocalPtr qcdNonIsoElectronAnalyser_, qcdNonIsoNonIsoTriggerElectronAnalyser_;
	METAnalyserLocalPtr metAnalyserqcdConversionSelection_;
	ElectronAnalyserLocalPtr qcdConversionsElectronAnalyser_;
	ElectronAnalyserLocalPtr qcdEPlusjetsPFRelIsoElectronAnalyser_, qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_;
	//QCD region with asym. jets
	METAnalyserLocalPtr metAnalyserqcdNonIsoAsymJetsSelection_, metAnalyserqcdNonIsoNonIsoTriggerAsymJetsSelection_;
	ElectronAnalyserLocalPtr qcdNonIsoElectronAnalyserAsymJets_, qcdNonIsoNonIsoTriggerElectronAnalyserAsymJets_;
	METAnalyserLocalPtr metAnalyserqcdConversionAsymJetsSelection_;
	ElectronAnalyserLocalPtr qcdConversionsElectronAnalyserAsymJets_;
	ElectronAnalyserLocalPtr qcdEPlusjetsPFRelIsoElectronAnalyserAsymJets_,
			qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJets_;
	//QCD region with asym. jets + MET
	METAnalyserLocalPtr metAnalyserqcdNonIsoAsymJetsMETSelection_, metAnalyserqcdNonIsoNonIsoTriggerAsymJetsMETSelection_;
	ElectronAnalyserLocalPtr qcdNonIsoElectronAnalyserAsymJetsMET_, qcdNonIsoNonIsoTriggerElectronAnalyserAsymJetsMET_;
	METAnalyserLocalPtr metAnalyserqcdConversionAsymJetsMETSelection_;
	ElectronAnalyserLocalPtr qcdConversionsElectronAnalyserAsymJetsMET_;
	ElectronAnalyserLocalPtr qcdEPlusjetsPFRelIsoElectronAnalyserAsymJetsMET_,
			qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJetsMET_;
	std::vector<double> metBins_;
	std::vector<ElectronAnalyserPtr> binnedElectronAnalysers_;
	std::vector<ElectronAnalyserPtr> qcdConversionBinnedElectronAnalysers_;
	std::vector<ElectronAnalyserPtr> qcdNonIsoBinnedElectronAnalysers_;

};

typedef boost::scoped_ptr<BAT::TTbarPlusMETAnalyser> TTbarPlusMETAnalyserLocalPtr;
} /* namespace BAT */
#endif /* TTBARPLUSMETANALYSER_H_ */
