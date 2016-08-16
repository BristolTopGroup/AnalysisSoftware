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
	virtual void createTrees();
	void createCommonTrees( std::string folder );
	void createCommonNoBSelectionTrees( std::string folder );
	void fillCommonTrees(const EventPtr event,  const unsigned int selection, std::string folder );
	void fillCommonTreesNoBSelection(const EventPtr event,  const unsigned int selectionCriteria, std::string folder );
	void fillLeptonEfficiencyCorrectionBranches( const EventPtr event, const unsigned int selectionCriteria, const LeptonPointer signalLepton );	
	virtual void createHistograms() {};
	void ePlusJetsQcdAnalysis(const EventPtr);
	void muPlusJetsQcdAnalysis(const EventPtr);
	void ePlusJetsSignalAnalysis(const EventPtr);
	void muPlusJetsSignalAnalysis(const EventPtr);

private:
	/**
	 * Analysers
	 */
	BasicAnalyserLocalPtr vertexAnalyserEPlusJetsRefSelection_, vertexAnalyserMuPlusJetsRefSelection_;
	
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
