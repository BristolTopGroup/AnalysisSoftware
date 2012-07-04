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
	static boost::shared_ptr<TH1D> estimatedPileup;
	static unsigned int NTupleVersion;

	//Kinematic fit
	static std::string TQAFPath;
	static bool useHitFit;
	static bool produceFitterASCIIoutput;

	//electrons
//	static double maxElectronPFIsolation;
//	static double maxElectronLoosePFIsolation;
//	static double maxElectronRelativeIsolation;
	static ElectronAlgorithm::value electronAlgorithm;
//	static ElectronID::value electronID;
//	static double minElectronET;
//	static double MaxAbsoluteElectronEta;
//	static double electronIsolationCone;
//
//	//muons
//	static double maxMuonPFIsolation;
//	static double maxMuonRelativeIsolation;
	static MuonAlgorithm::value muonAlgorithm;
//	static double maxAbsoluteMuonEta;
//	static double minMuonPt;
//	static double muonIsolationCone;
//
//	//jets
	static JetAlgorithm::value jetAlgorithm;
//	static BtagAlgorithm::value btagAlgorithm;
//	static BtagAlgorithm::workingPoint btagWorkingPoint;
	static boost::array<boost::shared_ptr<TF1>, 12> bL7Corrections;
	static boost::array<boost::shared_ptr<TF1>, 12> lightL7Corrections;
	static int JESsystematic;

	//good jet definition
//	static double maxAbsoluteJetEta;
//	static double minJetPt;

	//MET
	static METAlgorithm::value metAlgorithm;
//	static double METCut;
};

}

#endif /* GLOBALVARIABLES_H_ */
