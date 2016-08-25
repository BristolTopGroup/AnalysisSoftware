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
	SingleMuon_Run2016B,
	SingleMuon_Run2016C,
	SingleMuon_Run2016D,
	SingleMuon_Run2016E,

	TTJets_PowhegPythia8,
	TTJets_PowhegPythia8_scaledown,
	TTJets_PowhegPythia8_scaleup,
	TTJets_PowhegPythia8_mtop1695,
	TTJets_PowhegPythia8_mtop1755,

	TTJets_amcatnloFXFX,
	TTJets_madgraphMLM,
	TTJets_PowhegHerwigpp,
	TTJets_amcatnloHerwigpp,

	// TTJets_synch,
	DYJetsToLL_M50,
	WJetsToLNu,
	// TToLeptons_s,
	// TBarToLeptons_s,
	// TBarToLeptons_t,
	ST_t,
	STbar_t,
	ST_s,
	ST_tW,
	STbar_tW,
	// GJets_100to200,
	// GJets_200to400,
	// GJets_400to600,
	// GJets_600toInf,
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
		"SingleMuon_Run2016B",
		"SingleMuon_Run2016C",
		"SingleMuon_Run2016D",
		"SingleMuon_Run2016E",

		"TTJets_PowhegPythia8",
		"TTJets_PowhegPythia8_scaledown",
		"TTJets_PowhegPythia8_scaleup",
		"TTJets_PowhegPythia8_mtop1695",
		"TTJets_PowhegPythia8_mtop1755",

		"TTJets_amcatnloFXFX",
		// "TTJets_amcatnloFXFX_scaledown",
		// "TTJets_amcatnloFXFX_scaleup",
		// "TTJets_amcatnloFXFX_mtop1695",
		// "TTJets_amcatnloFXFX_mtop1755",

		"TTJets_madgraphMLM",
		"TTJets_PowhegHerwigpp",
		"TTJets_amcatnloHerwigpp",

		// "TTJets_synch",

		"DYJetsToLL_M50",
		"WJetsToLNu",
		"ST_t",
		"STbar_t",
		"ST_s",
		"ST_tW",
		"STbar_tW",
		// "GJets_100to200",
		// "GJets_200to400",
		// "GJets_400to600",
		// "GJets_600toInf",
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
