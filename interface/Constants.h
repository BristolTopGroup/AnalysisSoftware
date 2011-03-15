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
    DATA,
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
    //    QCD_ALL,
    //    singleTop_ALL,
    NUMBER_OF_DATA_TYPES
};

const boost::array<std::string, DataType::NUMBER_OF_DATA_TYPES> names = { {
        "data",
        "ttjet",
        "wj",
        "zj",
        "bce1",
        "bce2",
        "bce3",
        "enri1",
        "enri2",
        "enri3",//10
        "pj1",
        "pj2",
        "pj3",
        "VqqJets",
        "tW",
        "tchan",
        "schan",
        "Zprime_M500GeV_W5GeV",
        "Zprime_M500GeV_W50GeV",
        "Zprime_M750GeV_W7500MeV",//20
        "Zprime_M1TeV_W10GeV",
        "Zprime_M1TeV_W100GeV",
        "Zprime_M1250GeV_W12500MeV",
        "Zprime_M1500GeV_W15GeV",
        "Zprime_M1500GeV_W150GeV",
        "Zprime_M2TeV_W20GeV",
        "Zprime_M2TeV_W200GeV",
        "Zprime_M3TeV_W30GeV",
        "Zprime_M3TeV_W300GeV",
        "Zprime_M4TeV_W40GeV",//30
        "Zprime_M4TeV_W400GeV" } };
}

namespace NeutrinoSelectionCriterion {
enum value {
    pzClosestToLepton,
    mostCentral,
    pzClosestToLeptonOrMostcentralIfAbove300,
    largestValueOfCosine,
    chi2,
    TopMassDifference
};
}

namespace HLT {
enum trigger {

};
}

namespace JetBin {
enum value {
    NoJet,
    OneJet,
    TwoJets,
    ThreeJets,
    FourOrMoreJets,
    NUMBER_OF_JET_BINS
};

const boost::array<std::string, JetBin::NUMBER_OF_JET_BINS> names = {
        { "0jet", "1jet", "2jets", "3jets", "4orMoreJets"
        } };
}

namespace JetBinSummed {
enum value {
    allJet,
    OneOrMoreJets,
    TwoOrMoreJets,
    ThreeOrMoreJets,
    NUMBER_OF_SUMMED_JET_BINS
};

const boost::array<std::string, JetBinSummed::NUMBER_OF_SUMMED_JET_BINS> names = { {
        "0orMoreJets",
        "1orMoreJets",
        "2orMoreJets",
        "3orMoreJets" } };
}

namespace BJetBin {
enum value {
    NoBtag,
    OneBTag,
    TwoBtags,
    ThreeBtags,
    FourOrMoreBTags,
    NUMBER_OF_BJET_BINS
};

const boost::array<std::string, BJetBin::NUMBER_OF_BJET_BINS> names = { {
        "0btag",
        "1btag",
        "2btags",
        "3btags",
        "4orMoreBtags" } };
}

namespace BJetBinSummed {
enum value {
    allBtags,
    OneOrMoreBTags,
    TwoOrMoreBTags,
    ThreeOrMoreBTags,
    NUMBER_OF_SUMMED_BJET_BINS
};

const boost::array<std::string, BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS> names = { {
        "0orMoreBtag",
        "1orMoreBtag",
        "2orMoreBtags",
        "3orMoreBtags" } };
}

//namespace JetAlgorithm {
//enum value {
//    Calo_AntiKT_Cone05,
//    JPT_AntiKt_ConeDR05,
//    ParticleFlow,
//    PF2PAT,
//    NUMBER_OF_JETALGORITHMS
//};
////for nTuple branch names
//const boost::array<std::string, JetAlgorithm::NUMBER_OF_JETALGORITHMS> prefixes = { {
//        "CaloJet",
//        "",
//        "PFJet",
//        "PF2PATJet" } };
//
//const boost::array<std::string, JetAlgorithm::NUMBER_OF_JETALGORITHMS> names = { {
//        "CaloJets",
//        "JPT",
//        "PFJets",
//        "PF2PATJets" } };
//}

namespace METAlgorithm {
enum value {
    Calo,
    tcMET,
    ParticleFlowMET,
    NUMBER_OF_METALGORITHMS
};

const boost::array<std::string, METAlgorithm::NUMBER_OF_METALGORITHMS> prefixes = { {
        "CaloMET",
        "TCMET",
        "PFMET"  } };

const boost::array<std::string, METAlgorithm::NUMBER_OF_METALGORITHMS> names = { {
        "CaloMET",
        "TCMET",
        "PFMET" } };
}

namespace ElectronAlgorithm {
enum value {
    Calo,
    ParticleFlow,
    NUMBER_OF_ELECTRONALGORITHMS
};

const boost::array<std::string, ElectronAlgorithm::NUMBER_OF_ELECTRONALGORITHMS> prefixes = { {
        "Electron",
        "PFElectron"  } };

const boost::array<std::string, ElectronAlgorithm::NUMBER_OF_ELECTRONALGORITHMS> names = { {
        "CaloElectron",
        "PFElectron"} };
}

namespace ElectronIsolation{
enum value {
    RelativeIsolation,
    ParticleFlowIsolation,
    NUMBER_OF_ELECTRONISOLATIONS
};
}


namespace MuonAlgorithm {
enum value {
    Default,
    ParticleFlow,
    NUMBER_OF_MUONALGORITHMS
};

const boost::array<std::string, MuonAlgorithm::NUMBER_OF_MUONALGORITHMS> prefixes = { {
        "Muon",
        "PFMuon"  } };

const boost::array<std::string, MuonAlgorithm::NUMBER_OF_MUONALGORITHMS> names = { {
        "Muon",
        "PFMuon"} };
}

namespace HLTriggers{
enum value {
////////////////////////////
                                    //// 2E31 + 6E31
                                    ////////////////////////////
                                    //Electron
                                    HLT_DoubleEle10_SW_L1R, //0
                                    HLT_Ele12_SW_TightEleIdIsol_L1R, //1
                                    HLT_Ele12_SW_TightEleIdIsol_NoDEtaInEE_L1R, //2
                                    HLT_Ele17_SW_CaloEleId_L1R, //3
                                    HLT_Ele17_SW_EleId_L1R, //4
                                    HLT_Ele17_SW_LooseEleId_L1R, //5
                                    HLT_Ele22_SW_CaloEleId_L1R, //6
                                    HLT_Ele40_SW_L1R, //7
                                    HLT_DoubleEle15_SW_L1R_v1, //8
                                    HLT_Ele10_MET45_v1, //9
                                    HLT_Ele12_SW_TighterEleIdIsol_L1R_v1, //10
                                    HLT_Ele17_SW_TightCaloEleId_SC8HE_L1R_v1, //11
                                    HLT_Ele17_SW_TightEleIdIsol_L1R_v1, //12
                                    HLT_Ele17_SW_TightEleId_L1R, //13
                                    HLT_Ele17_SW_TighterEleIdIsol_L1R_v1, //14
                                    HLT_Ele17_SW_TighterEleId_L1R_v1, //15
                                    HLT_Ele27_SW_TightCaloEleIdTrack_L1R_v1, //16
                                    HLT_Ele32_SW_TightCaloEleIdTrack_L1R_v1, //17
                                    //Photon
                                    HLT_DoublePhoton17_L1R, //18
                                    HLT_DoublePhoton5_CEP_L1R, //19
                                    HLT_Photon20_Cleaned_L1R, //20
                                    HLT_Photon30_Cleaned_L1R, //21
                                    HLT_Photon50_NoHE_Cleaned_L1R, //22
                                    HLT_Photon100_NoHE_Cleaned_L1R_v1, //23
                                    HLT_Photon17_SC17HE_L1R_v1, //24
                                    HLT_Photon30_Isol_EBOnly_Cleaned_L1R_v1, //25
                                    HLT_Photon35_Isol_Cleaned_L1R_v1, //26
                                    HLT_Photon50_Cleaned_L1R_v1, //27
                                    HLT_Photon70_NoHE_Cleaned_L1R_v1, //28
                                    //Jet
                                    HLT_DiJetAve30U, //29
                                    HLT_Jet30U, //30
                                    HLT_Jet50U, //31
                                    HLT_Jet70U, //32
                                    HLT_Jet100U, //33
                                    HLT_Jet70U_v2, //34
                                    HLT_Jet100U_v2, //35
                                    HLT_Jet140U_v1, //36
                                    //METFwd
                                    HLT_MET45, //37
                                    HLT_MET65, //38
                                    HLT_MET100, //39
                                    HLT_MET80_v1, //40
                                    HLT_MET100_v2, //41
                                    HLT_MET45_HT100U_v1, //42
                                    HLT_MET45_HT120U_v1, //43
                                    HLT_HT100U, //44
                                    HLT_DoubleJet15U_ForwardBackward, //45
                                    HLT_DoubleJet25U_ForwardBackward, //46
                                    //BTau
                                    HLT_BTagMu_Jet20U, //47
                                    HLT_DoubleIsoTau15_OneLeg_Trk5, //48
                                    HLT_DoubleIsoTau15_Trk5, //49
                                    HLT_SingleIsoTau20_Trk15_MET20, //50
                                    HLT_SingleIsoTau20_Trk5_MET20, //51
                                    HLT_SingleIsoTau30_Trk5_L120or30, //52
                                    HLT_SingleIsoTau30_Trk5_MET20, //53
                                    HLT_BTagMu_DiJet20U_v1, //54
                                    HLT_SingleIsoTau30_Trk5_v2, //55
                                    //Mu
                                    HLT_DoubleMu3, //56
                                    HLT_Mu3, //57
                                    HLT_Mu5, //58
                                    HLT_Mu7, //59
                                    HLT_Mu9, //60
                                    HLT_Mu11, //61
                                    HLT_IsoMu9, //62
                                    HLT_Mu20_NoVertex, //63
                                    HLT_DoubleMu5_v1, //64
                                    HLT_Mu13_v1, //65
                                    HLT_Mu15_v1, //66
                                    HLT_Mu5_Ele9_v1, //67
                                    HLT_Mu5_HT50U_v1, //68
                                    HLT_Mu5_HT70U_v1, //69
                                    HLT_Mu5_Jet35U_v1, //70
                                    HLT_Mu5_Jet50U_v1, //71
                                    HLT_Mu5_MET45_v1, //72
                                    HLT_Mu5_Photon11_Cleaned_L1R_v1, //73
                                    //MinimumBias
                                    HLT_ZeroBias, //74
                                    HLT_L1Tech_BSC_minBias, //75
                                    HLT_L1Tech_BSC_halo_forPhysicsBackground, //76
                                    HLT_L1Tech_HCAL_HF, //77
                                    HLT_L1_BPTX, //78
                                    HLT_L1_BPTX_MinusOnly, //79
                                    HLT_L1_BPTX_PlusOnly, //80
                                    HLT_StoppedHSCP, //81
                                    HLT_StoppedHSCP_v2, //82
                                    //Cosmics
                                    HLT_L1Tech_BSC_halo, //83
                                    HLT_TrackerCosmics, //84
                                    ////////////////////////////
                                    //// 2E32
                                    ////////////////////////////
                                    //Electron
                                    HLT_DoubleEle17_SW_L1R_v1, //85
                                    HLT_DoubleEle8_SW_HT70U_L1R_v1, //86
                                    HLT_Ele10_SW_EleId_HT70U_L1R_v1, //87
                                    HLT_Ele10_SW_HT100U_L1R_v1, //88
                                    HLT_Ele10_SW_HT70U_L1R_v1, //89
                                    HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1, //90
                                    HLT_Ele17_SW_TighterEleIdIsol_L1R_v2, //91
                                    HLT_Ele22_SW_TighterCaloIdIsol_L1R_v1, //92
                                    HLT_Ele22_SW_TighterEleId_L1R_v2, //93
                                    HLT_Ele32_SW_TighterEleId_L1R_v2, //94
                                    HLT_IsoEle12_PFTau15_v1, //95
                                    //Photon
                                    HLT_DoublePhoton17_SingleIsol_L1R_v1, //96
                                    HLT_DoublePhoton22_L1R_v1, //97
                                    HLT_DoublePhoton5_CEP_L1R_v3, //98
                                    HLT_Photon110_NoHE_Cleaned_L1R_v1, //99
                                    HLT_Photon17_Isol_SC17HE_L1R_v1, //100
                                    HLT_Photon22_SC22HE_L1R_v1, //101
                                    HLT_Photon40_CaloId_Cleaned_L1R_v1, //102
                                    HLT_Photon40_Isol_Cleaned_L1R_v1, //103
//                                    HLT_Photon50_Cleaned_L1R_v2, //104 this trigger is already included (#27)
                                    HLT_Photon70_Cleaned_L1R_v1, //104
                                    //Mu
                                    HLT_IsoMu13_v3, //105
                                    HLT_Mu17_v1, //106
                                    HLT_Mu21_v1, //107
                                    HLT_Mu8_Ele8_v1, //108
                                    HLT_Mu5_HT100U_v3, //109
                                    HLT_Mu5_HT70U_v3, //110
                                    //Jet
                                    HLT_Jet50U_v3, //111
                                    HLT_Jet140U_v3, //112
                                    //METFwd
                                    HLT_MET100_v3, //113
                                    HLT_MET120_v3, //114
                                    HLT_MET45_DiJet30U_v3, //115
                                    ////////////////////////////
                                    //// Others (from Sam, W analysis)
                                    ////////////////////////////
                                    HLT_Ele10_LW_L1R, //116
                                    HLT_Ele15_SW_L1R, //117
                                    HLT_Ele15_SW_EleId_L1R, //118
                                    HLT_Ele20_SW_L1R, //119
                                    HLT_Ele15_SW_CaloEleId_L1R, //120
                                    HLT_Ele22_SW_TighterEleId_L1R_v3, //121
                                    NUMBER_OF_HLTS
};
}


}

#endif /* ENUMERATORS_H_ */
