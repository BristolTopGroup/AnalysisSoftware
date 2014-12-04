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
unsigned int Globals::NTupleVersion = 6;
bool Globals::verbose = true;

//kinematic fit
std::string Globals::TQAFPath = "";
bool Globals::useHitFit = false;
bool Globals::produceFitterASCIIoutput = false;

//electrons
ElectronAlgorithm::value Globals::electronAlgorithm = ElectronAlgorithm::ParticleFlow;
int Globals::ElectronScaleFactorSystematic = 0;

//muons
MuonAlgorithm::value Globals::muonAlgorithm = MuonAlgorithm::ParticleFlow;
int Globals::MuonScaleFactorSystematic = 0;
boost::shared_ptr<TH2F> Globals::muonIdIsoScaleFactorsHistogram = boost::shared_ptr<TH2F>(new TH2F());
boost::shared_ptr<TH3F> Globals::muonTriggerScaleFactorsHistogram = boost::shared_ptr<TH3F>(new TH3F());

//jets
JetAlgorithm::value Globals::jetAlgorithm = JetAlgorithm::PF2PAT;
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
METAlgorithm::value Globals::metAlgorithm = METAlgorithm::patType1CorrectedPFMet;
bool Globals::applySysShiftMetCorrection = false;
bool Globals::applyType0MetCorrection = false;
bool Globals::applyType1MetCorrection = false;

//top pt reweighting
bool Globals::applyTopPtReweighting = true;

std::string Globals::custom_file_suffix = "";
unsigned int Globals::pdfWeightNumber = 0;

}
