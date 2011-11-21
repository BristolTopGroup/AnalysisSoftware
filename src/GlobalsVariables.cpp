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

double Globals::luminosity = 3934.085;
unsigned long Globals::printEveryXEvents = 100000;
std::string Globals::selectedEventsOutputfile = "selectedEvents.txt";
std::string Globals::selectedEventsWithMetaDataOutputfile = "selectedEventsWithMetaData.txt";
PileUpReweightingMethod::value Globals::pileUpReweightingMethod = PileUpReweightingMethod::averagePileUp;
boost::shared_ptr<TH1D> Globals::estimatedPileup = boost::shared_ptr<TH1D>(new TH1D("tmp", "tmp", 25, 0, 25));

//kinematic fit
std::string Globals::TQAFPath = "";
bool Globals::useHitFit = false;
bool Globals::produceFitterASCIIoutput = false;

//electrons
double Globals::maxElectronPFIsolation = 0.1;
double Globals::maxElectronRelativeIsolation = 0.1;
ElectronAlgorithm::value Globals::electronAlgorithm = ElectronAlgorithm::ParticleFlow;
ElectronID::value Globals::electronID = ElectronID::CiCHyperTight1MC;
double Globals::MaxAbsoluteElectronEta = 2.5;
double Globals::minElectronET = 30;

//muons
double Globals::maxMuonRelativeIsolation = 0.1;
double Globals::maxMuonPFIsolation = 0.1;
MuonAlgorithm::value Globals::muonAlgorithm = MuonAlgorithm::ParticleFlow;
double Globals::maxAbsoluteMuonEta = 2.1;
double Globals::minMuonPt = 20.;

//jets
JetAlgorithm::value Globals::jetAlgorithm = JetAlgorithm::PF2PAT;
BtagAlgorithm::value Globals::btagAlgorithm = BtagAlgorithm::SimpleSecondaryVertexHighEffBTag;
BtagAlgorithm::workingPoint Globals::btagWorkingPoint = BtagAlgorithm::MEDIUM;
double Globals::maxAbsoluteJetEta = 2.4;
double Globals::minJetPt = 30.;//GeV

//Level7 JEC arrays
boost::array< boost::shared_ptr<TF1>, 12 > Globals::bL7Corrections = {{
		boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1,
		boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1}};

boost::array< boost::shared_ptr<TF1>, 12 > Globals::lightL7Corrections = {{
		boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1,
		boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1, boosted_TF1}};

//MET
METAlgorithm::value Globals::metAlgorithm = METAlgorithm::ParticleFlowMET;
double Globals::METCut = 20.; //GeV
}
