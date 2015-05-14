/*
 * HitFitAnalyser.h
 *
 *  Created on: Jul 5, 2011
 *      Author: senkin
 */

#ifndef HITFITANALYSER_H_
#define HITFITANALYSER_H_
#include <fstream>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "../../interface/TtbarHypothesis.h"
#include "../../interface/HistHelpers/HistogramManager.h"
#include "BasicAnalyser.h"

#include "TopQuarkAnalysis/TopHitFit/interface/RunHitFit.h"
#include "TopQuarkAnalysis/TopHitFit/interface/Top_Decaykin.h"
#include "TopQuarkAnalysis/TopHitFit/interface/LeptonTranslatorBase.h"
#include "TopQuarkAnalysis/TopHitFit/interface/JetTranslatorBase.h"
#include "TopQuarkAnalysis/TopHitFit/interface/METTranslatorBase.h"

typedef hitfit::RunHitFit<BAT::Electron, BAT::Muon, BAT::Jet, BAT::MET> BatHitFit;

namespace BAT {

class HitFitAnalyser: public BasicAnalyser {
private:

//    BAT::TtbarHypothesis trueFourVectors;
	BAT::TtbarHypothesis truthMatchEvent;
	BAT::JetCollection jetsForFitting;
	BAT::JetCollection bJetsForFitting;

	std::string FitterPath_;
	std::string hitfitDefault_;
	std::string hitfitElectronResolution_;
	std::string hitfitMuonResolution_;
	std::string hitfitUdscJetResolution_;
	std::string hitfitBJetResolution_;
	std::string hitfitMETResolution_;

	double hitfitLepWMass_;
	double hitfitHadWMass_;
	double hitfitTopMass_;
	double lepton_charge;
	bool do_MC_matching;
	bool allTTBarJetsPassedToFit_;
	bool allTTBarJetsPassSelection_;

	bool isElectronChannel_;

	hitfit::LeptonTranslatorBase<BAT::Electron> electronTranslator_;
	hitfit::LeptonTranslatorBase<BAT::Muon> muonTranslator_;
	hitfit::JetTranslatorBase<BAT::Jet> jetTranslator_;
	hitfit::METTranslatorBase<BAT::MET> metTranslator_;

	struct jetPtCompare {
		bool operator()(const BAT::ParticlePointer& i, const BAT::ParticlePointer& j) const {
			return i->pt() > j->pt();
		}
	};
	jetPtCompare jetPtComp;

private:
	FourVector fourVectorFromHitFit(const hitfit::Fourvec& v);
	BAT::TtbarHypothesis BatEvent(const hitfit::Lepjets_Event& ev, const EventPtr event);

public:
	HitFitAnalyser(HistogramManagerPtr histMan, TreeManagerPtr treeMan, const bool isElectronChannel, std::string histogramFolder = "hitfitStudy" );
	virtual ~HitFitAnalyser();

	void analyse(const EventPtr);
	BAT::TtbarHypothesis analyseAndReturn(const EventPtr, const JetCollection jets, const JetCollection bjets, const LeptonPointer selectedLepton );
	void setMCTTbarHypothesis(const TtbarHypothesis& mcEvent);
	void createHistograms();
	void createTrees();
	int positionOfLastTTBarJet(const JetCollection jets);
};
typedef boost::scoped_ptr<HitFitAnalyser> HitFitAnalyserLocalPtr;
typedef boost::shared_ptr<HitFitAnalyser> HitFitAnalyserPtr;
}

#endif /* HITFITANALYSER_H_ */
