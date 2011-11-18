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
    ttbar,
    Wjets,
    Zjets,
    QCD_BCtoE_Pt20to30,
    QCD_BCtoE_Pt30to80,
    QCD_BCtoE_Pt80to170,
    QCD_EMEnriched_Pt20to30,
    QCD_EMEnriched_Pt30to80,
    QCD_EMEnriched_Pt80to170,//10
    PhotonJets_Pt40to100,
    PhotonJets_Pt100to200,
    PhotonJets_Pt200toInf,
    VQQ,
    singleTop_And_W,
    singleTopTChannel,
    singleTopSChannel,
    singleAntiTop_And_W,
	singleAntiTopTChannel,
	singleAntiTopSChannel,
    Zprime_M500GeV_W5GeV,
    Zprime_M500GeV_W50GeV,
    Zprime_M750GeV_W7500MeV,//20
    Zprime_M1TeV_W10GeV,
    Zprime_M1TeV_W100GeV,
    Zprime_M1250GeV_W12500MeV,
    Zprime_M1500GeV_W15GeV,
    Zprime_M1500GeV_W150GeV,
    Zprime_M2TeV_W20GeV,
    Zprime_M2TeV_W200GeV,
    Zprime_M3TeV_W30GeV,
    Zprime_M3TeV_W300GeV,
    Zprime_M4TeV_W40GeV,//30
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
    NUMBER_OF_DATA_TYPES
};

const boost::array<std::string, DataType::NUMBER_OF_DATA_TYPES> names = { {
        "ElectronHad",
        "MuHad",
        "TTJet",
        "WJetsToLNu",
        "DYJetsToLL",
        "QCD_Pt-20to30_BCtoE",
        "QCD_Pt-30to80_BCtoE",
        "QCD_Pt-80to170_BCtoE",
        "QCD_Pt-20to30_EMEnriched",
        "QCD_Pt-30to80_EMEnriched",
        "QCD_Pt-80to170_EMEnriched",//10
        "GJets_TuneD6T_HT-40To100",
        "GJets_TuneD6T_HT-100To200",
        "GJets_TuneD6T_HT-200",
        "VqqJets",
        "T_TuneZ2_tW-channel",
        "T_TuneZ2_t-channel",
        "T_TuneZ2_s-channel",
		"Tbar_TuneZ2_tW-channel",
		"Tbar_TuneZ2_t-channel",
		"Tbar_TuneZ2_s-channel",//20
		"Zprime_M500GeV_W5GeV",
		"Zprime_M500GeV_W50GeV",
        "Zprime_M750GeV_W7500MeV",
        "Zprime_M1000GeV_W10GeV",
        "Zprime_M1000GeV_W100GeV",
        "Zprime_M1250GeV_W12500MeV",
        "Zprime_M1500GeV_W15GeV",
        "Zprime_M1500GeV_W150GeV",
        "Zprime_M2000GeV_W20GeV",
        "Zprime_M2000GeV_W200GeV",
        "Zprime_M3000GeV_W30GeV",
        "Zprime_M3000GeV_W300GeV",
        "Zprime_M4000GeV_W40GeV",//30
        "Zprime_M4000GeV_W400GeV",
        "WWtoAnything",
        "WZtoAnything",
        "ZZtoAnything",
        "TTJets161",
        "TTJets163",
        "TTJets166",
        "TTJets169",
        "TTJets175",
        "TTJets178",
        "TTJets181",
        "TTJets184"
} };
}

}

#endif /* ENUMERATORS_H_ */
