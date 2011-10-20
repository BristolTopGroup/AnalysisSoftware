/*
 * HighLevelTriggers.h
 *
 *  Created on: 17 Mar 2011
 *      Author: kreczko
 */

#ifndef HIGHLEVELTRIGGERS_H_
#define HIGHLEVELTRIGGERS_H_

//namespace HLTriggers{
//enum value{
////2010 data RunA + RunB
//    HLT_Ele10_LW_L1R, //0
//    HLT_Ele15_SW_L1R, //1
//    HLT_Ele15_SW_CaloEleId_L1R, //2
//    HLT_Ele17_SW_CaloEleId_L1R, //3
//    HLT_Ele17_SW_EleId_L1R, //4
//    HLT_Ele17_SW_LooseEleId_L1R, //5
//    HLT_Ele17_SW_TightEleIdIsol_L1R_v1, //6
//    HLT_Ele17_SW_TightEleId_L1R, //7
//    HLT_Ele17_SW_TighterEleIdIsol_L1R_v1, //8
//    HLT_Ele17_SW_TighterEleId_L1R_v1, //9
//    HLT_Ele22_SW_TighterEleId_L1R, //10
//    HLT_Ele27_SW_TightCaloEleIdTrack_L1R_v1, //11
//    //========== 2011 data ==============
//    //ElectronHad PD
//    HLT_Ele25_CaloIdVT_TrkIdT_CentralJet40_BTagIP, //12
//    HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30, //13
//    HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30, //14
//    HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30, //15
//    //renaming for 1E33
//    HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30_BTagIP, //16
//    HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30,//17
//    HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30, //18
//    HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30, //19
//    //new iso triggers:
//    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_BTagIP,//20
//    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30,//21
//    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30,//22
//    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30,//23
//    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30,//24
//    //Higgs trigger
//    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20,//25
//    //EWK triggers
//    HLT_Ele25_WP80_PFMT40,//26
//    HLT_Ele27_WP70_PFMT40_PFMHT20,//27
//
//
//    //SingleElectron PD
//    HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, //28
//    HLT_Ele25_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL,//29
//    HLT_Ele32_CaloIdVL_CaloIsoVL_TrkIdVL_TrkIsoVL,//30
//    HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT,//31
//    NUMBER_OF_TRIGGERS
//};
//}

//nTuple_V4
namespace HLTriggers{
enum value{
//========== 2011 data ==============
    //ElectronHad PD
    HLT_Ele25_CaloIdVT_TrkIdT_CentralJet40_BTagIP, //12
    HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30, //13
    HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30, //14
    HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30, //15
    //renaming for 1E33
    HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30_BTagIP, //16
    HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30,//17
    HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30, //18
    HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30, //19
    //new iso triggers:
    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_BTagIP,//20
    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30,//21
    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30,//22
    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30,//23
    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30,//24
    //Higgs trigger
    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20,//25
    //EWK triggers
    HLT_Ele25_WP80_PFMT40,//26
    HLT_Ele27_WP70_PFMT40_PFMHT20,//27
    //SingleElectron PD
    HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, //28
    HLT_Ele25_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL,//29
    HLT_Ele32_CaloIdVL_CaloIsoVL_TrkIdVL_TrkIsoVL,//30
    HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT,//31
    //5E33 change to PF jets @ HLT
    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30,
    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30,
    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30,
    HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30,
    //control triggers
    HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30,
    HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30,
    HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30,
    HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30,
    //Muon Triggers
    //SingleMuon - last unprescaled in 1E33 menu
    HLT_Mu15,
    HLT_Mu20,
    HLT_IsoMu17,
    HLT_IsoMu15,
    HLT_IsoMu24,
    HLT_IsoMu24_eta2p1,
    HLT_IsoMu30_eta2p1,
    //MuHad
    HLT_IsoMu17_CentralJet40_BTagIP, //32
    HLT_Mu17_CentralJet30, //33
    HLT_Mu17_CentralJet40_BTagIP, //34
    HLT_Mu17_DiCentralJet30, //35
    HLT_Mu17_TriCentralJet30, //36
    //2E33
    HLT_IsoMu17_CentralJet30_BTagIP,
    HLT_IsoMu17_CentralJet30,
    HLT_IsoMu17_DiCentralJet30,
    HLT_IsoMu17_QuadCentralJet30,
    HLT_IsoMu17_TriCentralJet30,
    //3E33 - added eta requirement @ L1
    HLT_IsoMu17_eta2p1_CentralJet30_BTagIP,
    HLT_IsoMu17_eta2p1_CentralJet30,
    HLT_IsoMu17_eta2p1_DiCentralJet30,
    HLT_IsoMu17_eta2p1_QuadCentralJet30,
    HLT_IsoMu17_eta2p1_TriCentralJet30,
    //5E33 change to PF jets @ HLT
    HLT_IsoMu17_eta2p1_CentralPFJet30,
    HLT_IsoMu17_eta2p1_DiCentralPFJet30,
    HLT_IsoMu17_eta2p1_TriCentralPFJet30,
    HLT_IsoMu17_eta2p1_QuadCentralPFJet30,
    //control triggers
    HLT_Mu17_eta2p1_CentralPFJet30,
    HLT_Mu17_eta2p1_DiCentralPFJet30,
    HLT_Mu17_eta2p1_TriCentralPFJet30,
    HLT_Mu17_eta2p1_QuadCentralPFJet30,
    NUMBER_OF_TRIGGERS
};
}
#endif /* HIGHLEVELTRIGGERS_H_ */
