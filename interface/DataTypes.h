/*
 * Enumerators.h
 *
 *  Created on: Jun 30, 2010
 *      Author: lkreczko
 */

#ifndef ENUMERATORS_H_
#define ENUMERATORS_H_
#include <string>
#include <boost/array.hpp>

namespace BAT {
namespace DataType {
enum value {
	ElectronHad,
	MuHad,
	SingleElectron,
	SingleMu,
	TTJets,
	TTJets_Madgraph_PU30bx50,
	TTJets_Madgraph_PU4bx50,
	TT_Pythia8,
	TT_Pythia8_PU30bx50,
	TT_Pythia8_PU40bx25,

	WJets,
	W1Jet,
	W2Jets,
	W3Jets,
	W4Jets,
	Zjets,
	// 2012 DY samples
	Z1Jet,
	Z2Jets,
	Z3Jets,
	Z4Jets,

	// PHYS14 QCD samples
	// BC->E
	QCD_BCtoE_Pt20_30,
	QCD_BCtoE_Pt30_80,
	QCD_BCtoE_Pt80_170,
	QCD_BCtoE_Pt170,
	// EM Enriched
	QCD_EMEnriched_Pt20_30,
	QCD_EMEnriched_Pt30_80,
	QCD_EMEnriched_Pt80_170,
	QCD_EMEnriched_Pt170_250,
	QCD_EMEnriched_Pt250_350,
	QCD_EMEnriched_Pt_350, //

	QCD_MuEnriched_20ToInf,
	QCD_MuEnriched_30to50,
	QCD_MuEnriched_50to80,
	QCD_MuEnriched_80to120,

	QCD_MuEnrichedPt5_Pt15to20,
	QCD_MuEnrichedPt5_Pt20to30,
	QCD_MuEnrichedPt5_Pt30to50,
	QCD_MuEnrichedPt5_Pt50to80,
	QCD_MuEnrichedPt5_Pt80to120,
	QCD_MuEnrichedPt5_Pt120to170,
	QCD_MuEnrichedPt5_Pt170o300,
	QCD_MuEnrichedPt5_Pt300to470,
	QCD_MuEnrichedPt5_Pt470to600,
	QCD_MuEnrichedPt5_Pt600to800,
	QCD_MuEnrichedPt5_Pt800to1000,
	QCD_MuEnrichedPt5_Pt1000,
	//PHYS14 PJ samples
	PhotonJets_Pt100to200,
	PhotonJets_Pt200to400,
	PhotonJets_Pt400to600,
	PhotonJets_Pt600toInf,
	
	VQQ,
	singleTop_And_W, //
	singleTopTChannel,
	singleTopSChannel,
	singleAntiTop_And_W,
	singleAntiTopTChannel,
	singleAntiTopSChannel,
	TTJets_mcatnlo,
	TTJets_powheg,
	WWtoAnything,
	WZtoAnything,
	ZZtoAnything,
	ttbar161,
	ttbar163,
	ttbar166,
	ttbar169,
	ttbar175,
	ttbar178,
	ttbar181,
	ttbar184,
	TTJets_matchingdown,
	TTJets_matchingup,
	TTJets_scaledown,
	TTJets_scaleup,
	WJets_matchingdown,
	WJets_matchingup,
	WJets_scaledown,
	WJets_scaleup,
	ZJets_matchingdown,
	ZJets_matchingup,
	ZJets_scaledown,
	ZJets_scaleup,
	TTJetsPlusZ,
	TTbarPlusW,
	TTH_Inclusive_M125,
	NUMBER_OF_DATA_TYPES
};

const boost::array<std::string, DataType::NUMBER_OF_DATA_TYPES> names = { {
//
		"ElectronHad",//
		"MuHad", //
		"SingleElectron",//
		"SingleMu",//
		"TTJet", //
		"TTJets_Madgraph_PU30bx50", //
		"TTJets_Madgraph_PU4bx50", //
		"TT_Pythia8", //
		"TT_Pythia8_PU30bx50", //
		"TT_Pythia8_PU40bx25", //

		"WJets", //
		"W1Jet", //
		"W2Jets", //
		"W3Jets", //
		"W4Jets", //
		"DYJetsToLL", //
		//2012 DY samples
		"DY1JetsToLL", //
		"DY2JetsToLL", //
		"DY3JetsToLL", //
		"DY4JetsToLL", //

		//PHYS14 QCD samples
		"QCD_bcToE_20to30", //
		"QCD_bcToE_30to80", //
		"QCD_bcToE_80to170", //
		"QCD_bcToE_170toInf", //
		"QCD_EMEnriched_20to30", //
		"QCD_EMEnriched_30to80", //
		"QCD_EMEnriched_80to170", //
		"QCD_EMEnriched_170to250", //
		"QCD_EMEnriched_250to350", //
		"QCD_EMEnriched_350", //

		"QCD_MuEnriched_20toInf", //
		"QCD_MuEnriched_30to50", //
		"QCD_MuEnriched_50to80", //
		"QCD_MuEnriched_80to120", //

		//2011 & 2012 Muon QCD samples
		"QCD_Pt-15to20_MuEnrichedPt5", //
		"QCD_Pt-20to30_MuEnrichedPt5", //
		"QCD_Pt-30to50_MuEnrichedPt5", //
		"QCD_Pt-50to80_MuEnrichedPt5", //
		"QCD_Pt-80to120_MuEnrichedPt5", //
		"QCD_Pt-120to170_MuEnrichedPt5", //
		"QCD_Pt-170to300_MuEnrichedPt5", //
		"QCD_Pt-300to470_MuEnrichedPt5", //
		"QCD_Pt-470to600_MuEnrichedPt5", //
		"QCD_Pt-600to800_MuEnrichedPt5", //
		"QCD_Pt-800to1000_MuEnrichedPt5", //
		"QCD_Pt-1000_MuEnrichedPt5", //
		//PHYS14 PJ samples
		"GJets_100to200", //
		"GJets_200to400", //
		"GJets_400to600", //
		"GJets_600toInf", //

		"VqqJets", //
		"T_tw", //
		"TToLeptons_t", //
		"TToLeptons_s", //
		"Tbar_tw", //
		"TBarToLeptons_t", //20
		"TBarToLeptons_s", //
		"TTJets-mcatnlo", //
		"TTJets-powheg", //
		"WWtoAnything", //
		"WZtoAnything", //
		"ZZtoAnything", //
		"TTJets161", //
		"TTJets163", //
		"TTJets166", //
		"TTJets169", //
		"TTJets175", //
		"TTJets178", //
		"TTJets181", //
		"TTJets184", //
		"TTJets-matchingdown", //
		"TTJets-matchingup", //
		"TTJets-scaledown", //
		"TTJets-scaleup", //
		"WJets-matchingdown", //
		"WJets-matchingup", //
		"WJets-scaledown", //
		"WJets-scaleup", //
		"ZJets-matchingdown",//
		"ZJets-matchingup",//
		"ZJets-scaledown",//
		"ZJets-scaleup",//
		"TTbarZIncl", //
		"TTbarInclWIncl", //
		"TTH_Inclusive_M-125"
		} };
}

}

#endif /* ENUMERATORS_H_ */
