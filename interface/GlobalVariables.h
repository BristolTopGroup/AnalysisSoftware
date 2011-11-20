/*
 * GlobalVariables.h
 *
 *  Created on: 20 Oct 2011
 *      Author: kreczko
 */

#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_
#include "RecoObjects/Jet.h"
#include "RecoObjects/Electron.h"
#include "RecoObjects/Muon.h"
#include "RecoObjects/MET.h"
#include "TF1.h"

namespace BAT {
struct Globals {

//general
static std::string TQAFPath;
static double luminosity;
static bool useHitFit;
static bool produceFitterASCIIoutput;

//electrons
static double maxElectronPFIsolation;
static double maxElectronRelativeIsolation;
static ElectronAlgorithm::value electronAlgorithm;
static ElectronID::value electronID;
static double minElectronET;
static double MaxAbsoluteElectronEta;

//muons
static double maxMuonPFIsolation;
static double maxMuonRelativeIsolation;
static MuonAlgorithm::value muonAlgorithm;
static double maxAbsoluteMuonEta;
static double minMuonPt;

//jets
static JetAlgorithm::value jetAlgorithm;
static BtagAlgorithm::value btagAlgorithm;
static BtagAlgorithm::workingPoint btagWorkingPoint;
static boost::array< boost::shared_ptr<TF1>, 12 > bL7Corrections;
static boost::array< boost::shared_ptr<TF1>, 12 > lightL7Corrections;

//good jet definition
static double maxAbsoluteJetEta;
static double minJetPt;

//MET
static METAlgorithm::value metAlgorithm;
static double METCut;
};

}

#endif /* GLOBALVARIABLES_H_ */
