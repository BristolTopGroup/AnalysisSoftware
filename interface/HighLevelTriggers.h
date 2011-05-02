/*
 * HighLevelTriggers.h
 *
 *  Created on: 17 Mar 2011
 *      Author: kreczko
 */

#ifndef HIGHLEVELTRIGGERS_H_
#define HIGHLEVELTRIGGERS_H_

namespace HLTriggers{
enum value{
//2010 data RunA + RunB
    HLT_Ele10_LW_L1R, //0
    HLT_Ele15_SW_L1R, //1
    HLT_Ele15_SW_CaloEleId_L1R, //2
    HLT_Ele17_SW_CaloEleId_L1R, //3
    HLT_Ele17_SW_EleId_L1R, //4
    HLT_Ele17_SW_LooseEleId_L1R, //5
    HLT_Ele17_SW_TightEleIdIsol_L1R_v1, //6
    HLT_Ele17_SW_TightEleId_L1R, //7
    HLT_Ele17_SW_TighterEleIdIsol_L1R_v1, //8
    HLT_Ele17_SW_TighterEleId_L1R_v1, //9
    HLT_Ele22_SW_TighterEleId_L1R_v2, //10
    HLT_Ele22_SW_TighterEleId_L1R_v3, //11
    HLT_Ele27_SW_TightCaloEleIdTrack_L1R_v1, //12
    //2011 data
    HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30_v1, //13
    HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30_v1, //14
    HLT_Ele25_CaloIdVT_TrkIdT_CentralJet40_BTagIP_v1, //15
    HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30_v1, //16
    HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30_v2, //17
    HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30_v2, //18
    HLT_Ele25_CaloIdVT_TrkIdT_CentralJet40_BTagIP_v2, //19
    HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30_v2, //20
    //additional triggers to recover boosted top
    HLT_Ele10_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_HT200_v2, //21
    HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1, //22
    HLT_Ele45_CaloIdVT_TrkIdT_v1, //23
    HLT_Ele10_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_HT200_v3, //24
    HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2, //25
    HLT_Ele45_CaloIdVT_TrkIdT_v2, //26
};
}
#endif /* HIGHLEVELTRIGGERS_H_ */
