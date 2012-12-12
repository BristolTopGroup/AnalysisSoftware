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
#include "MuonAnalyser.h"
#include "VertexAnalyser.h"
#include "JetAnalyser.h"
#include "../Selections/BasicSelection.h"
#include "../../interface/Selections/QCDNoIsoNoIDSelection.h"

#include <string>

namespace BAT {

class TTbarPlusMETAnalyser: public BAT::BasicAnalyser {
public:
	TTbarPlusMETAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "TTbarPlusMetAnalysis");
	virtual ~TTbarPlusMETAnalyser();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();
	void ePlusJetsQcdAnalysis(const EventPtr);
	void muPlusJetsQcdAnalysis(const EventPtr);
	void ePlusJetsSignalAnalysis(const EventPtr);
	void muPlusJetsSignalAnalysis(const EventPtr);

private:
	//signal selections
	SelectionPointer topEplusJetsRefSelection_, topMuplusJetsRefSelection_;
	//QCD selections with respect to reference selection
	SelectionPointer qcdNonIsoElectronSelection_, qcdNonIsoElectronNonIsoTriggerSelection_, qcdNonIsoMuonSelection_;
	SelectionPointer qcdConversionSelection_;
	SelectionPointer qcdPFRelIsoEPlusJetsSelection_, qcdPFRelIsoEPlusNonIsoTriggerSelection_,
			qcdPFRelIsoMuPlusJetsSelection_, qcdNoIsolationMuonSelection_;
	SelectionPointer qcdAntiIDSelection_;
	QCDNoIsoNoIDSelectionPointer qcdNoIsoNoIDSelection_;

	/**
	 * Analysers
	 */
	//signal regions
	METAnalyserLocalPtr metAnalyserEPlusJetsRefSelection_, metAnalyserMuPlusJetsRefSelection_;
	ElectronAnalyserLocalPtr electronAnalyserRefSelection_;
	MuonAnalyserLocalPtr muonAnalyserRefSelection_;
	BasicAnalyserLocalPtr vertexAnalyserEPlusJetsRefSelection_, vertexAnalyserMuPlusJetsRefSelection_;

	//QCD region Non-isolated electrons
	METAnalyserLocalPtr metAnalyserqcdNonIsoElectronSelection_, metAnalyserqcdNonIsoElectronNonIsoTriggerSelection_,
			metAnalyserqcdNonIsoMuonSelection_;
	ElectronAnalyserLocalPtr qcdNonIsoElectronAnalyser_, qcdNonIsoNonIsoTriggerElectronAnalyser_;
	MuonAnalyserLocalPtr qcdNonIsoMuonAnalyser_;
	//QCD region electrons from conversions
	METAnalyserLocalPtr metAnalyserqcdConversionSelection_;
	ElectronAnalyserLocalPtr qcdConversionsElectronAnalyser_;
	//No iso
	ElectronAnalyserLocalPtr qcdEPlusjetsPFRelIsoElectronAnalyser_, qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_;
	MuonAnalyserLocalPtr qcdMuPlusjetsPFRelIsoMuonAnalyser_;
	MuonAnalyserLocalPtr qcdNoIsolationMuonAnalyser_;
	//Anti ID
	METAnalyserLocalPtr metAnalyserqcdAntiIDSelection_;
	ElectronAnalyserLocalPtr qcdAntiIDElectronAnalyser_;
	//Anti ID
	METAnalyserLocalPtr metAnalyserqcdNoIsoNoIDSelection_;
	ElectronAnalyserLocalPtr qcdNoIsoNoIDElectronAnalyser_;

	std::vector<double> metBins_;
	std::vector<ElectronAnalyserPtr> binnedElectronAnalysers_;
	std::vector<MuonAnalyserPtr> binnedMuonAnalysers_;
	std::vector<ElectronAnalyserPtr> qcdConversionBinnedElectronAnalysers_;
	std::vector<ElectronAnalyserPtr> qcdNonIsoBinnedElectronAnalysers_;
	std::vector<ElectronAnalyserPtr> qcdPFRelIsoBinnedElectronAnalysers_;
	std::vector<ElectronAnalyserPtr> qcdAntiIDBinnedElectronAnalysers_;
	std::vector<ElectronAnalyserPtr> qcdNoIsoNoIDBinnedElectronAnalysers_;
	std::vector<MuonAnalyserPtr> qcdNonIsoBinnedMuonAnalysers_;
	std::vector<MuonAnalyserPtr> qcdPFRelIsoBinnedMuonAnalysers_;
//	JetAnalyserLocalPtr jetAnalyserEPlusJetsRefSelection_noBtagWeights_, jetAnalyserMuPlusJetsRefSelection_noBtagWeights_;
	JetAnalyserLocalPtr jetAnalyserEPlusJetsRefSelection_, jetAnalyserMuPlusJetsRefSelection_;
};

typedef boost::scoped_ptr<BAT::TTbarPlusMETAnalyser> TTbarPlusMETAnalyserLocalPtr;
} /* namespace BAT */
#endif /* TTBARPLUSMETANALYSER_H_ */
