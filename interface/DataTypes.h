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
	// MuHad,
	SingleElectron,
	SingleMuon,

	// SingleElectron_PromptReco_RunD,
	// SingleElectron_ReReco_RunD,
	// SingleElectron_ReReco_RunC,
	// SingleMuon_PromptReco_RunD,
	// SingleMuon_ReReco_RunD,
	// SingleMuon_ReReco_RunC,

	TTJets_PowhegPythia8,
	TTJets_PowhegPythia8_scaledown,
	TTJets_PowhegPythia8_scaleup,
	TTJets_PowhegPythia8_mtop1695,
	TTJets_PowhegPythia8_mtop1755,

	TTJets_amcatnloFXFX,
	// TTJets_amcatnloFXFX_scaledown,
	// TTJets_amcatnloFXFX_scaleup,
	TTJets_amcatnloFXFX_mtop1695,
	TTJets_amcatnloFXFX_mtop1755,

	TTJets_madgraphMLM,

	// TTJets_powhegPythia6,
	// TTJets_PowhegHerwigpp,

	TTJets_amcatnloHerwigpp,

	// TTJets_synch,
	DYJetsToLL_M50,
	WJetsToLNu,
	// TToLeptons_s,
	// TBarToLeptons_s,
	// TBarToLeptons_t,
	TToLeptons_t,
	T_tw,
	Tbar_tw,
	GJets_15to6000,
	QCD_EMEnriched_15to20,
	QCD_EMEnriched_20to30,
	QCD_EMEnriched_30to50,
	QCD_EMEnriched_50to80,
	QCD_EMEnriched_80to120,
	QCD_EMEnriched_120to170,
	QCD_EMEnriched_170to300,
	QCD_EMEnriched_300toInf,
	QCD_bcToE_15to20,
	QCD_bcToE_20to30,
	QCD_bcToE_30to80,
	QCD_bcToE_80to170,
	QCD_bcToE_170to250,
	QCD_bcToE_250toInf,
	QCD_MuEnriched_15to20,
	QCD_MuEnriched_20to30,
	QCD_MuEnriched_30to50,
	QCD_MuEnriched_50to80,
	QCD_MuEnriched_80to120,
	QCD_MuEnriched_120to170,
	QCD_MuEnriched_170to300,
	QCD_MuEnriched_300to470,
	QCD_MuEnriched_470to600,
	QCD_MuEnriched_600to800,
	QCD_MuEnriched_800to1000,
	QCD_MuEnriched_1000toInf,

	NUMBER_OF_DATA_TYPES
};

const boost::array<std::string, DataType::NUMBER_OF_DATA_TYPES> names = { {
//
		"ElectronHad",//
		// "MuHad", //

		"SingleElectron",
		"SingleMuon",

		// "SingleElectron_PromptReco_RunD",
		// "SingleElectron_ReReco_RunD",
		// "SingleElectron_ReReco_RunC",
		// "SingleMuon_PromptReco_RunD",
		// "SingleMuon_ReReco_RunD",
		// "SingleMuon_ReReco_RunC",

		"TTJets_PowhegPythia8",
		"TTJets_PowhegPythia8_scaledown",
		"TTJets_PowhegPythia8_scaleup",
		"TTJets_PowhegPythia8_mtop1695",
		"TTJets_PowhegPythia8_mtop1755",

		"TTJets_amcatnloFXFX",
		// "TTJets_amcatnloFXFX_scaledown",
		// "TTJets_amcatnloFXFX_scaleup",
		"TTJets_amcatnloFXFX_mtop1695",
		"TTJets_amcatnloFXFX_mtop1755",

		"TTJets_madgraphMLM",
		// "TTJets_powhegPythia6",
		// "TTJets_PowhegHerwigpp",
		"TTJets_amcatnloHerwigpp",

		// "TTJets_synch",

		"DYJetsToLL_M50",
		"WJetsToLNu",
		// "TToLeptons_s",
		// "TBarToLeptons_s",
		// "TBarToLeptons_t",
		"TToLeptons_t",
		"T_tw",
		"Tbar_tw",
		"GJets_15to6000",
		"QCD_EMEnriched_15to20",
		"QCD_EMEnriched_20to30",
		"QCD_EMEnriched_30to50",
		"QCD_EMEnriched_50to80",
		"QCD_EMEnriched_80to120",
		"QCD_EMEnriched_120to170",
		"QCD_EMEnriched_170to300",
		"QCD_EMEnriched_300toInf",
		"QCD_bcToE_15to20",
		"QCD_bcToE_20to30",
		"QCD_bcToE_30to80",
		"QCD_bcToE_80to170",
		"QCD_bcToE_170to250",
		"QCD_bcToE_250toInf",
		"QCD_MuEnriched_15to20",
		"QCD_MuEnriched_20to30",
		"QCD_MuEnriched_30to50",
		"QCD_MuEnriched_50to80",
		"QCD_MuEnriched_80to120",
		"QCD_MuEnriched_120to170",
		"QCD_MuEnriched_170to300",
		"QCD_MuEnriched_300to470",
		"QCD_MuEnriched_470to600",
		"QCD_MuEnriched_600to800",
		"QCD_MuEnriched_800to1000",
		"QCD_MuEnriched_1000toInf"
		} };

DataType::value getDataType(const std::string filename);
}

}

#endif /* ENUMERATORS_H_ */
