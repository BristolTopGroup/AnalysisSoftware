/*
 * TTbar_plus_X_analyser.h
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
#include "WAnalyser.h"
#include "HitFitAnalyser.h"
#include "LikelihoodRecoAnalyser.h"
#include "BTagEfficiencyAnalyser.h"
#include "PileupAnalyser.h"

#include "Binned_variable_analyser.h"

#include <string>

namespace BAT {

class TTbar_plus_X_analyser: public BAT::BasicAnalyser {
public:
	TTbar_plus_X_analyser(HistogramManagerPtr histMan, std::string histogramFolder = "TTbar_plus_X_analysis");
	TTbar_plus_X_analyser(HistogramManagerPtr histMan, TreeManagerPtr treeMan, std::string histogramFolder = "TTbar_plus_X_analysis");
	virtual ~TTbar_plus_X_analyser();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();
	void ePlusJetsQcdAnalysis(const EventPtr);
	void muPlusJetsQcdAnalysis(const EventPtr);
	void ePlusJetsSignalAnalysis(const EventPtr);
	void muPlusJetsSignalAnalysis(const EventPtr);

private:
	/**
	 * Analysers
	 */
	//signal regions
	METAnalyserLocalPtr metAnalyserEPlusJetsRefSelection_, metAnalyserMuPlusJetsRefSelection_;
	ElectronAnalyserLocalPtr electronAnalyserRefSelection_;
	MuonAnalyserLocalPtr muonAnalyserRefSelection_;
	BasicAnalyserLocalPtr vertexAnalyserEPlusJetsRefSelection_, vertexAnalyserMuPlusJetsRefSelection_;

	//QCD region Non-isolated electrons
	METAnalyserLocalPtr metAnalyserqcdNonIsoElectronSelection_, metAnalyserqcdNonIsoMuonSelection_;
	ElectronAnalyserLocalPtr qcdNonIsoElectronAnalyser_;
	MuonAnalyserLocalPtr qcdNonIsoMuonAnalyser_;
	//QCD region electrons from conversions
	METAnalyserLocalPtr metAnalyserqcdConversionSelection_;
	ElectronAnalyserLocalPtr qcdConversionsElectronAnalyser_;

	JetAnalyserLocalPtr jetAnalyserEPlusJetsRefSelection_, jetAnalyserMuPlusJetsRefSelection_;
	JetAnalyserLocalPtr jetAnalyserEPlusJetsQCDNonIsoSelection_, jetAnalyserEPlusJetsConversionSelection_, jetAnalyserMuPlusJetsQCDNonIsoSelection_;

	// W simple reco analyser
	WAnalyserLocalPtr wAnalyserEPlusJetsRefSelection_, wAnalyserMuPlusJetsRefSelection_;

	// HitFit Analyser
	HitFitAnalyserLocalPtr hitFitAnalyserEPlusJetsRefSelection_;
	HitFitAnalyserLocalPtr hitFitAnalyserMuPlusJetsRefSelection_;
	HitFitAnalyserLocalPtr hitFitAnalyserEPlusJetsQCDSelection_;
	HitFitAnalyserLocalPtr hitFitAnalyserEPlusJetsConversionSelection_;
	HitFitAnalyserLocalPtr hitFitAnalyserMuPlusJetsQCDSelection_;

	// Likelihood ttbar reco analyser
	LikelihoodRecoAnalyserLocalPtr likelihoodRecoAnalyserEPlusJetsRefSelection_;
	LikelihoodRecoAnalyserLocalPtr likelihoodRecoAnalyserMuPlusJetsRefSelection_;

	// BTag Efficiency
	BTagEffAnalyserLocalPtr BTagEffAnalyserEPlusJetsRefSelection_;
	BTagEffAnalyserLocalPtr BTagEffAnalyserMuPlusJetsRefSelection_;

	// Pileup Analyser
	PileupAnalyserLocalPtr PileupAnalyserEPlusJetsRefSelection_;
	PileupAnalyserLocalPtr PileupAnalyserMuPlusJetsRefSelection_;
};

typedef boost::scoped_ptr<BAT::TTbar_plus_X_analyser> TTbar_plus_X_analyser_local_ptr;
} /* namespace BAT */
#endif /* TTBARPLUSMETANALYSER_H_ */
