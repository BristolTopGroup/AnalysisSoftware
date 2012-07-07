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
	TTJets,
	Wjets,
	W1Jet,
	W2Jets,
	W3Jets,
	W4Jets,
	Zjets,
	QCD_BCtoE_Pt20to30,
	QCD_BCtoE_Pt30to80,
	QCD_BCtoE_Pt80to170,
	QCD_EMEnriched_Pt20to30,
	QCD_EMEnriched_Pt30to80,
	QCD_EMEnriched_Pt80to170,
	QCD_MuEnrichedPt15_Pt20,
	PhotonJets_Pt40to100,
	PhotonJets_Pt100to200,
	PhotonJets_Pt200toInf,
	VQQ,
	singleTop_And_W, //
	singleTopTChannel,
	singleTopSChannel,
	singleAntiTop_And_W,
	singleAntiTopTChannel,
	singleAntiTopSChannel,
	Zprime_M500GeV_W5GeV,
	Zprime_M500GeV_W50GeV,
	Zprime_M750GeV_W7500MeV,
	Zprime_M1TeV_W10GeV,
	Zprime_M1TeV_W100GeV,
	Zprime_M1250GeV_W12500MeV,
	Zprime_M1500GeV_W15GeV,
	Zprime_M1500GeV_W150GeV,
	Zprime_M2TeV_W20GeV,
	Zprime_M2TeV_W200GeV,
	Zprime_M3TeV_W30GeV,
	Zprime_M3TeV_W300GeV,
	Zprime_M4TeV_W40GeV,
	Zprime_M4TeV_W400GeV,
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
	NUMBER_OF_DATA_TYPES
};

const boost::array<std::string, DataType::NUMBER_OF_DATA_TYPES> names = { {
//
		"ElectronHad",//
		"MuHad", //
		"TTJet", //
		"WJetsToLNu", //
		"W1Jet", //
		"W2Jets", //
		"W3Jets", //
		"W4Jets", //
		"DYJetsToLL", //
		"QCD_Pt-20to30_BCtoE", //
		"QCD_Pt-30to80_BCtoE", //
		"QCD_Pt-80to170_BCtoE", //
		"QCD_Pt-20to30_EMEnriched", //
		"QCD_Pt-30to80_EMEnriched", //
		"QCD_Pt-80to170_EMEnriched", //10
		"QCD_Pt-20_MuEnrichedPt-15", //
		"GJets_HT-40To100", //
		"GJets_HT-100To200", //
		"GJets_HT-200", //
		"VqqJets", //
		"T_tW-channel", //
		"T_t-channel", //
		"T_s-channel", //
		"Tbar_tW-channel", //
		"Tbar_t-channel", //20
		"Tbar_s-channel", //
		"Zprime_M500GeV_W5GeV", //
		"Zprime_M500GeV_W50GeV", //
		"Zprime_M750GeV_W7500MeV", //
		"Zprime_M1000GeV_W10GeV", //
		"Zprime_M1000GeV_W100GeV", //
		"Zprime_M1250GeV_W12500MeV", //
		"Zprime_M1500GeV_W15GeV", //
		"Zprime_M1500GeV_W150GeV", //
		"Zprime_M2000GeV_W20GeV", //
		"Zprime_M2000GeV_W200GeV", //
		"Zprime_M3000GeV_W30GeV", //
		"Zprime_M3000GeV_W300GeV", //30
		"Zprime_M4000GeV_W40GeV", //
		"Zprime_M4000GeV_W400GeV", //
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
		"TTbarInclWIncl" //
		} };
}

}

#endif /* ENUMERATORS_H_ */
