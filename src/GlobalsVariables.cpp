/*
 * Globals.cpp
 *
 *  Created on: 20 Oct 2011
 *      Author: kreczko
 */

#include "../interface/GlobalVariables.h"
#define boosted_TF1 boost::shared_ptr<TF1>(new TF1())

namespace BAT {

//general
unsigned long Globals::maxEvents = 0;

double Globals::luminosity = 19584.;
unsigned short Globals::energyInTeV = 8;
unsigned long Globals::printEveryXEvents = 100000;
std::string Globals::selectedEventsOutputfile = "selectedEvents.txt";
std::string Globals::selectedEventsWithMetaDataOutputfile = "selectedEventsWithMetaData.txt";
PileUpReweightingMethod::value Globals::pileUpReweightingMethod = PileUpReweightingMethod::averagePileUp;
boost::shared_ptr<TH1D> Globals::estimatedPileup = boost::shared_ptr<TH1D>(new TH1D("tmp", "tmp", 25, 0, 25));
unsigned int Globals::NTupleVersion = 0;
bool Globals::verbose = true;

//kinematic fit
std::string Globals::TQAFPath = "";
bool Globals::useHitFit = false;
bool Globals::produceFitterASCIIoutput = false;

// Likelihood ttbar reco
boost::shared_ptr<TH1F> Globals::csvCorrectPermHistogram = boost::shared_ptr<TH1F>(new TH1F());
boost::shared_ptr<TH1F> Globals::csvIncorrectPermHistogram = boost::shared_ptr<TH1F>(new TH1F());
boost::shared_ptr<TH1F> Globals::NuChiCorrectPermHistogram = boost::shared_ptr<TH1F>(new TH1F());
boost::shared_ptr<TH1F> Globals::NuChiIncorrectPermHistogram = boost::shared_ptr<TH1F>(new TH1F());

boost::shared_ptr<TH2F> Globals::HadronicRecoCorrectPermHistogram = boost::shared_ptr<TH2F>(new TH2F());
boost::shared_ptr<TH2F> Globals::HadronicRecoIncorrectPermHistogram = boost::shared_ptr<TH2F>(new TH2F());
boost::shared_ptr<TH2F> Globals::LeptonicRecoCorrectPermHistogram = boost::shared_ptr<TH2F>(new TH2F());
boost::shared_ptr<TH2F> Globals::LeptonicRecoIncorrectPermHistogram = boost::shared_ptr<TH2F>(new TH2F());

// BTagging efficiencies
boost::shared_ptr<TH2F> Globals::bQuarkJet = boost::shared_ptr<TH2F>(new TH2F());
boost::shared_ptr<TH2F> Globals::cQuarkJet = boost::shared_ptr<TH2F>(new TH2F());
boost::shared_ptr<TH2F> Globals::udsQuarkJet = boost::shared_ptr<TH2F>(new TH2F());
boost::shared_ptr<TH2F> Globals::gluonJet = boost::shared_ptr<TH2F>(new TH2F());



//electrons
ElectronAlgorithm::value Globals::electronAlgorithm = ElectronAlgorithm::ParticleFlow;
int Globals::ElectronScaleFactorSystematic = 0;
boost::shared_ptr<TH2D> Globals::electronIdIsoScaleFactorsHistogram = boost::shared_ptr<TH2D>(new TH2D());
boost::shared_ptr<TH1F> Globals::electronTriggerScaleFactorsHistogram = boost::shared_ptr<TH1F>(new TH1F());

//muons
MuonAlgorithm::value Globals::muonAlgorithm = MuonAlgorithm::ParticleFlow;
int Globals::MuonScaleFactorSystematic = 0;
boost::shared_ptr<TH2F> Globals::muonIdIsoScaleFactorsHistogram = boost::shared_ptr<TH2F>(new TH2F());
boost::shared_ptr<TH3F> Globals::muonTriggerScaleFactorsHistogram = boost::shared_ptr<TH3F>(new TH3F());

//jets
JetAlgorithm::value Globals::jetAlgorithm = JetAlgorithm::PF2PAT;
boost::shared_ptr<TEfficiency> Globals::hadronTriggerLegEfficiencyHistogram_nonIsoJets = boost::shared_ptr<TEfficiency>(new TEfficiency("nonIsoJets","nonIsoJets",1,0));
boost::shared_ptr<TEfficiency> Globals::hadronTriggerLegEfficiencyHistogram_isoJets = boost::shared_ptr<TEfficiency>(new TEfficiency("isoJets","isoJets",1,0));
boost::shared_ptr<TEfficiency> Globals::hadronTriggerLegEfficiencyHistogram_isoPFJets = boost::shared_ptr<TEfficiency>(new TEfficiency("isoPFJets","isoPFJets",1,0));
int Globals::JESsystematic = 0;
int Globals::BJetSystematic = 0;
int Globals::LightJetSystematic = 0;
int Globals::JetSmearingSystematic = 0;
bool Globals::applyJetSmearing = 0;

//Level7 JEC arrays (obsolete)
boost::array< boost::shared_ptr<TF1>, 12 > Globals::bL7Corrections = {{
		boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1,
		boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1}};

boost::array< boost::shared_ptr<TF1>, 12 > Globals::lightL7Corrections = {{
		boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1,
		boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1}};

//MET
METAlgorithm::value Globals::metAlgorithm = METAlgorithm::MET;
bool Globals::applySysShiftMetCorrection = false;
bool Globals::applyType0MetCorrection = false;
bool Globals::applyType1MetCorrection = false;

//top pt reweighting
bool Globals::applyTopPtReweighting = false;

std::string Globals::custom_file_suffix = "";
unsigned int Globals::pdfWeightNumber = 0;

std::string Globals::treePrefix_ = "";

}
