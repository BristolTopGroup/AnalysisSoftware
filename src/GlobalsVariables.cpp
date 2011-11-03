/*
 * Globals.cpp
 *
 *  Created on: 20 Oct 2011
 *      Author: kreczko
 */

#include "../interface/GlobalVariables.h"

namespace BAT {

//general
std::string Globals::TQAFPath = "";
double Globals::luminosity = 3934.085;

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
//MET
METAlgorithm::value Globals::metAlgorithm = METAlgorithm::ParticleFlowMET;
double Globals::METCut = 20.; //GeV
}
