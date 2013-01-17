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
	Wjets,
	W1Jet,
	W2Jets,
	W3Jets,
	W4Jets,
	Zjets,
	//2012 DY samples
	Z1Jet,
	Z2Jets,
	Z3Jets,
	Z4Jets,
	QCD_BCtoE_Pt20to30,
	QCD_BCtoE_Pt30to80,
	QCD_BCtoE_Pt80to170,
	QCD_EMEnriched_Pt20to30,
	QCD_EMEnriched_Pt30to80,
	QCD_EMEnriched_Pt80to170,
	QCD_MuEnrichedPt15_Pt20,
	//2012 QCD samples
	QCD_BCtoE_Pt20_30,
	QCD_BCtoE_Pt30_80,
	QCD_BCtoE_Pt80_170,
	QCD_BCtoE_Pt170_250,
	QCD_BCtoE_Pt250_350,
	QCD_BCtoE_Pt350,
	QCD_EMEnriched_Pt20_30,
	QCD_EMEnriched_Pt30_80,
	QCD_EMEnriched_Pt80_170,
	QCD_EMEnriched_Pt170_250,
	QCD_EMEnriched_Pt250_350,
	QCD_EMEnriched_Pt350,
	QCD_MuEnrichedPt15_Pt_20, 
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
	PhotonJets_Pt40to100,
	PhotonJets_Pt100to200,
	PhotonJets_Pt200toInf,
	//2012 PJ samples
	PhotonJets_Pt200to400,
	PhotonJets_Pt400toInf,
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
		"SingleElectron",//
		"SingleMu",//
		"TTJet", //
		"WJetsToLNu", //
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
		"QCD_Pt-20to30_BCtoE", //
		"QCD_Pt-30to80_BCtoE", //
		"QCD_Pt-80to170_BCtoE", //
		"QCD_Pt-20to30_EMEnriched", //
		"QCD_Pt-30to80_EMEnriched", //
		"QCD_Pt-80to170_EMEnriched", //10
		"QCD_Pt-20_MuEnrichedPt-15", //
		//2012 QCD samples
		"QCD_Pt_20_30_BCtoE", //
		"QCD_Pt_30_80_BCtoE", //
		"QCD_Pt_80_170_BCtoE", //
		"QCD_Pt_170_250_BCtoE", //
		"QCD_Pt_250_350_BCtoE", //
		"QCD_Pt_350_BCtoE", //
		"QCD_Pt_20_30_EMEnriched", //
		"QCD_Pt_30_80_EMEnriched", //
		"QCD_Pt_80_170_EMEnriched", //
		"QCD_Pt_170_250_EMEnriched", //
		"QCD_Pt_250_350_EMEnriched", //
		"QCD_Pt_350_EMEnriched", //
		"QCD_Pt_20_MuEnrichedPt_15", //
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
		"GJets_HT-40To100", //
		"GJets_HT-100To200", //
		"GJets_HT-200ToInf", //
		//2012 PJ samples
		"GJets_HT-200To400", //
		"GJets_HT-400ToInf", //
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
