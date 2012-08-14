/*
 * HighLevelTriggers.h
 *
 *  Created on: 17 Mar 2011
 *      Author: kreczko
 */

#ifndef HIGHLEVELTRIGGERS_H_
#define HIGHLEVELTRIGGERS_H_

//nTuple_V7
namespace HLTriggers {
enum value {
//ElectronHad PD
	HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30,
	HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30,
	HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30,
	//renaming for 1E33
	HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30,
	HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30,
	HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30,
	//new iso triggers:
	HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30,
	HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30,
	HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30,
	HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30,
	//SingleElectron PD
	HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT,
	HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT,
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
	HLT_IsoMu17,
	//MuHad
	HLT_Mu17_CentralJet30,
	HLT_Mu17_DiCentralJet30,
	HLT_Mu17_TriCentralJet30,
	//2E33
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
	//========== 2012 data ==============
	//data parking
	HLT_Ele27_WP80,
	HLT_IsoMu20_eta2p1, //needed for trigger efficiency
	//5E33 menu new triggers (rest is included in 2011 menu)
	HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet50_40_30,
	HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentrlJet50_40_30, //new backup trigger
	//
	HLT_Mu20_eta2p1_TriCentralPFJet30,
	HLT_Mu20_eta2p1_TriCentralPFJet50_40_30,
	HLT_IsoMu20_eta2p1_CentralPFJet30,
	HLT_IsoMu20_eta2p1_DiCentralPFJet30,
	HLT_IsoMu20_eta2p1_TriCentralPFJet30,
	HLT_IsoMu20_eta2p1_TriCentralPFJet50_40_30,
	//7E33 menu
	HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30,
	HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet50_40_30,
	HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFNoPUJet30,
	HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFNoPUJet30,
	HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30,
	HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet50_40_30,
	//
	HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30, //anti-ID trigger DEV
	HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30, //stop signal trigger
	HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet50_40_30, //stop signal trigger (backup)
	//
	HLT_Mu20_eta2p1_TriCentralPFNoPUJet30,
	HLT_Mu20_eta2p1_TriCentralPFNoPUJet50_40_30,
	HLT_IsoMu20_eta2p1_CentralPFNoPUJet30,
	HLT_IsoMu20_eta2p1_DiCentralPFNoPUJet30,
	HLT_IsoMu20_eta2p1_TriCentralPFNoPUJet30,
	HLT_IsoMu20_eta2p1_TriCentralPFNoPUJet50_40_30,
	//
	HLT_Mu17_eta2p1_TriCentralPFNoPUJet30,
	HLT_Mu17_eta2p1_TriCentralPFNoPUJet50_40_30,
	HLT_IsoMu17_eta2p1_CentralPFNoPUJet30,
	HLT_IsoMu17_eta2p1_DiCentralPFNoPUJet30,
	HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30,
	HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet50_40_30,
//triggers with lower 3rd jet threshold to combat missing JEC
	HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_30_20,
	HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30_30_20,
	HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20,
	HLT_Mu17_eta2p1_TriCentralPFNoPUJet30_30_20,
//new for 2011 SingleMu analysis
	HLT_IsoMu24,
	HLT_IsoMu24_eta2p1,
	NUMBER_OF_TRIGGERS
};

const boost::array<std::string, NUMBER_OF_TRIGGERS> names = { {
//ElectronHad PD
		"HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30",//
		"HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30", //
		"HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30", //
		//renaming for 1E33
		"HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30",//
		"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30", //
		"HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30", //
		//new iso triggers:
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30",//
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30", //
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30", //
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30", //
		//SingleElectron PD
		"HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT",//
		"HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT", //
		//5E33 change to PF jets @ "HLT
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30",//
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30", //
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30", //
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30", //
		//control triggers
		"HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30",//
		"HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30", //
		"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30", //
		"HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30", //
		//Muon Triggers
		//SingleMuon - last unprescaled in 1E33 menu
		"HLT_IsoMu17",//
		//MuHad
		"HLT_Mu17_CentralJet30",//
		"HLT_Mu17_DiCentralJet30", //
		"HLT_Mu17_TriCentralJet30", //
		//2E33
		"HLT_IsoMu17_CentralJet30",//
		"HLT_IsoMu17_DiCentralJet30", //
		"HLT_IsoMu17_QuadCentralJet30", //
		"HLT_IsoMu17_TriCentralJet30", //
		//3E33 - added eta requirement @ L1
		"HLT_IsoMu17_eta2p1_CentralJet30_BTagIP",//
		"HLT_IsoMu17_eta2p1_CentralJet30", //
		"HLT_IsoMu17_eta2p1_DiCentralJet30", //
		"HLT_IsoMu17_eta2p1_QuadCentralJet30", //
		"HLT_IsoMu17_eta2p1_TriCentralJet30", //
		//5E33 change to PF jets @ "HLT
		"HLT_IsoMu17_eta2p1_CentralPFJet30",//
		"HLT_IsoMu17_eta2p1_DiCentralPFJet30", //
		"HLT_IsoMu17_eta2p1_TriCentralPFJet30", //
		"HLT_IsoMu17_eta2p1_QuadCentralPFJet30", //
		//control triggers
		"HLT_Mu17_eta2p1_CentralPFJet30",//
		"HLT_Mu17_eta2p1_DiCentralPFJet30", //
		"HLT_Mu17_eta2p1_TriCentralPFJet30", //
		"HLT_Mu17_eta2p1_QuadCentralPFJet30", //
		//========== 2012 data ==============
		//data parking
		"HLT_Ele27_WP80",//
		"HLT_IsoMu20_eta2p1", ////needed for trigger efficiency
		//5E33 menu new triggers (rest is included in 2011 menu)
		"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet50_40_30",//
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentrlJet50_40_30", //new backup trigger
		//
		"HLT_Mu20_eta2p1_TriCentralPFJet30",//
		"HLT_Mu20_eta2p1_TriCentralPFJet50_40_30", //
		"HLT_IsoMu20_eta2p1_CentralPFJet30", //
		"HLT_IsoMu20_eta2p1_DiCentralPFJet30", //
		"HLT_IsoMu20_eta2p1_TriCentralPFJet30", //
		"HLT_IsoMu20_eta2p1_TriCentralPFJet50_40_30", //
		//7E33 menu
		"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30",//
		"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet50_40_30", //
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFNoPUJet30", //
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFNoPUJet30", //
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30", //
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet50_40_30", //
		//
		"HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30",//anti-ID trigger DEV
		"HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30", //stop signal trigger
		"HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet50_40_30", //stop signal trigger (backup)
		//
		"HLT_Mu20_eta2p1_TriCentralPFNoPUJet30",//
		"HLT_Mu20_eta2p1_TriCentralPFNoPUJet50_40_30", //
		"HLT_IsoMu20_eta2p1_CentralPFNoPUJet30", //
		"HLT_IsoMu20_eta2p1_DiCentralPFNoPUJet30", //
		"HLT_IsoMu20_eta2p1_TriCentralPFNoPUJet30", //
		"HLT_IsoMu20_eta2p1_TriCentralPFNoPUJet50_40_30", //
		//
		"HLT_Mu17_eta2p1_TriCentralPFNoPUJet30",//
		"HLT_Mu17_eta2p1_TriCentralPFNoPUJet50_40_30", //
		"HLT_IsoMu17_eta2p1_CentralPFNoPUJet30", //
		"HLT_IsoMu17_eta2p1_DiCentralPFNoPUJet30", //
		"HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30", //
		"HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet50_40_30", //
//triggers with lower 3rd jet threshold to combat missing JEC
		"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_30_20",//
		"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30_30_20",//
		"HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20",//
		"HLT_Mu17_eta2p1_TriCentralPFNoPUJet30_30_20",//
		"HLT_IsoMu24",//
		"HLT_IsoMu24_eta2p1" } };
}
#endif /* HIGHLEVELTRIGGERS_H_ */
