/*
 * GlobalVariables.h
 *
 *  Created on: 20 Oct 2011
 *      Author: kreczko
 */

#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_
#include "EventWeightProvider.h"
#include "RecoObjects/Jet.h"
#include "RecoObjects/Electron.h"
#include "RecoObjects/Muon.h"
#include "RecoObjects/MET.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TEfficiency.h"
#include <string>


namespace BAT {
struct Globals {

	//general
	static unsigned long maxEvents;
	static double luminosity;
	static unsigned short energyInTeV;
	static unsigned long printEveryXEvents;
	static std::string selectedEventsOutputfile;
	static std::string selectedEventsWithMetaDataOutputfile;
	static PileUpReweightingMethod::value pileUpReweightingMethod;
	static boost::shared_ptr<TH1D> estimatedPileup, estimatedPileup_up, estimatedPileup_down;
	static unsigned int NTupleVersion;
	static bool verbose;
	static std::string sample;

	//Kinematic fit
	static std::string TQAFPath;
	static bool useHitFit;
	static bool produceFitterASCIIoutput;

	// Likelihood ttbar reco
	static boost::shared_ptr<TH1F> csvCorrectPermHistogram;
	static boost::shared_ptr<TH1F> csvIncorrectPermHistogram;
	static boost::shared_ptr<TH1F> NuChiCorrectPermHistogram;
	static boost::shared_ptr<TH1F> NuChiIncorrectPermHistogram;
	static boost::shared_ptr<TH2F> HadronicRecoCorrectPermHistogram;
	static boost::shared_ptr<TH2F> HadronicRecoIncorrectPermHistogram;
	static boost::shared_ptr<TH2F> LeptonicRecoCorrectPermHistogram;
	static boost::shared_ptr<TH2F> LeptonicRecoIncorrectPermHistogram;

	// BTagging efficiencies
	static boost::shared_ptr<TH2F> bQuarkJet;
	static boost::shared_ptr<TH2F> cQuarkJet;
	static boost::shared_ptr<TH2F> udsgQuarkJet;

	static boost::shared_ptr<TH2F> bQuarkJet_PowhegPythia8;
	static boost::shared_ptr<TH2F> cQuarkJet_PowhegPythia8;
	static boost::shared_ptr<TH2F> udsgQuarkJet_PowhegPythia8;

	//electrons
	static ElectronAlgorithm::value electronAlgorithm;
	static int ElectronScaleFactorSystematic;
	static boost::shared_ptr<TH2F> electronTriggerScaleFactorsHistogram;
	static boost::shared_ptr<TH2F> electronIdScaleFactorsHistogram;
	static boost::shared_ptr<TH2F> electronIdScaleFactorsHistogram_etaBins;
	static boost::shared_ptr<TH2F> electronRecoScaleFactorsHistogram;

	//muons
	static MuonAlgorithm::value muonAlgorithm;
	static int MuonScaleFactorSystematic;
	static boost::shared_ptr<TH2F> muonIdScaleFactorsHistogram;
	static boost::shared_ptr<TH2F> muonIsoScaleFactorsHistogram;
	static boost::shared_ptr<TH2F> muonTriggerScaleFactorsHistogram;
	static boost::shared_ptr<TH1F> muonTrackingScaleFactorsHistogram;
	static boost::shared_ptr<TH1F> muonIdScaleFactorsHistogram_etaBins;
	static boost::shared_ptr<TH1F> muonIsoScaleFactorsHistogram_etaBins;
	static boost::shared_ptr<TH1F> muonTriggerScaleFactorsHistogram_etaBins;

	//jets
	static JetAlgorithm::value jetAlgorithm;
	static boost::shared_ptr<TEfficiency> hadronTriggerLegEfficiencyHistogram_nonIsoJets;
	static boost::shared_ptr<TEfficiency> hadronTriggerLegEfficiencyHistogram_isoJets;
	static boost::shared_ptr<TEfficiency> hadronTriggerLegEfficiencyHistogram_isoPFJets;
	static boost::array<boost::shared_ptr<TF1>, 12> bL7Corrections;
	static boost::array<boost::shared_ptr<TF1>, 12> lightL7Corrections;
	static int JESsystematic;
	static int BJetSystematic;
	static int LightJetSystematic;
	static int JetSmearingSystematic;
	static bool applyJetSmearing;

	//MET
	static METAlgorithm::value metAlgorithm;
	static bool applySysShiftMetCorrection;
	static bool applyType0MetCorrection;
	static bool applyType1MetCorrection;

	//top pt reweighting
	static bool applyTopPtReweighting;

	//miscellaneous
	static std::string custom_file_suffix;
	static unsigned int pdfWeightNumber;

	static std::string treePrefix_;
};

}

#endif /* GLOBALVARIABLES_H_ */
