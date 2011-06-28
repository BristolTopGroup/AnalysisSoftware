/*
 * HLTriggerAnalyser.cpp
 *
 *  Created on: Jun 21, 2011
 *      Author: lkreczko
 */

#include "../interface/HLTriggerAnalyser.h"
#include <iostream>
namespace BAT {

HLTriggerAnalyser::HLTriggerAnalyser(boost::shared_ptr<HistogramManager> histMan) :
	//	triggerCounts(),
			histMan(histMan) {
	//	createHistograms();
}

HLTriggerAnalyser::~HLTriggerAnalyser() {

}

//void HLTriggerAnalyser::increaseTriggerCount(HLTriggers::value trigger) {
//	triggerCounts.at(trigger)++;
//}

void HLTriggerAnalyser::analyse(const TopPairEventCandidate& ttbarEvent) {
	//do analysis

	//require tight and isolated electron
	if ((ttbarEvent.GoodPFIsolatedElectrons().size() > 0 ||
			ttbarEvent.GoodIsolatedElectrons().size() > 0)
			&& (ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT) ||
					ttbarEvent.HLT(HLTriggers::HLT_Ele25_WP80_PFMT40) ||
					ttbarEvent.HLT(HLTriggers::HLT_Ele27_WP70_PFMT40_PFMHT20))) {
		//all jets in the v2 nTuples have the following cuts applied:
		//pt > 20 & abs(eta) < 2.5 && loose ID
		const JetCollection& jets = ttbarEvent.Jets();

		const ElectronCollection& PFelectrons(ttbarEvent.GoodPFIsolatedElectrons());
		const ElectronCollection& electrons(ttbarEvent.GoodIsolatedElectrons());
		JetCollection cleanedJets(jets);

		for (unsigned int jetIndex = 0; jetIndex < cleanedJets.size(); ++jetIndex) {
			if (PFelectrons.size() == 0) {
				for (unsigned int electronIndex = 0; electronIndex < electrons.size(); ++electronIndex) {
					if (cleanedJets.at(jetIndex)->isWithinDeltaR(0.3, electrons.at(electronIndex))) {
						cleanedJets.erase(cleanedJets.begin() + jetIndex);
						--jetIndex;
						break;
					}
				}
			} else {
				for (unsigned int electronIndex = 0; electronIndex < PFelectrons.size(); ++electronIndex) {
					if (cleanedJets.at(jetIndex)->isWithinDeltaR(0.3, PFelectrons.at(electronIndex))) {
						cleanedJets.erase(cleanedJets.begin() + jetIndex);
						--jetIndex;
						break;
					}
				}
			}
		}

		bool passesPreTrigger(false);
		bool passesTrigger(false);
		std::string histFolder("");
		int prescale(1);

		if (cleanedJets.size() > 0) {
			const JetPointer jet1(cleanedJets.front());
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30";
			passesPreTrigger = true;//no pre-trigger other than the EWK triggers
			passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30);
			prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30);
			analyseTrigger(passesPreTrigger, passesTrigger, histFolder, jet1, prescale);

			if (ttbarEvent.runnumber() > 165970) {
				histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30";
				passesPreTrigger = true;//no pre-trigger other than the EWK triggers
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
				prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
				analyseTrigger(passesPreTrigger, passesTrigger, histFolder, jet1, prescale);

			}
		}
		if (cleanedJets.size() > 1) {
			const JetPointer jet2(cleanedJets.at(1));
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30";
			passesPreTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30);
			passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30) || ttbarEvent.HLT(
					HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30);
			if (ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30))
				prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30);

			if (ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30))
				prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30);

			analyseTrigger(passesPreTrigger, passesTrigger, histFolder, jet2, prescale);

			if (ttbarEvent.runnumber() > 165970) {
				histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30";
				passesPreTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);
				prescale
						= ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);
				analyseTrigger(passesPreTrigger, passesTrigger, histFolder, jet2, prescale);
			}
		}

		if (cleanedJets.size() > 2) {
			const JetPointer jet3(cleanedJets.at(2));
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30";
			passesPreTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30) || ttbarEvent.HLT(
					HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30);
			passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30) || ttbarEvent.HLT(
					HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);

			if (ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30))
				prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);

			if (ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30))
				prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);

			analyseTrigger(passesPreTrigger, passesTrigger, histFolder, jet3, prescale);

			if (ttbarEvent.runnumber() > 165970) {
				histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30";
				passesPreTrigger
						= ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
				prescale = ttbarEvent.HLTPrescale(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
				analyseTrigger(passesPreTrigger, passesTrigger, histFolder, jet3, prescale);
			}
		}

		if (cleanedJets.size() > 3 && ttbarEvent.runnumber() > 165970) {//run 165970, first occurance of the QuadJet trigger
			const JetPointer jet4(cleanedJets.at(3));
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30";
			passesPreTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30) || ttbarEvent.HLT(
					HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);
			passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30);
			prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30);
			analyseTrigger(passesPreTrigger, passesTrigger, histFolder, jet4, prescale);

			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30";
			passesPreTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
			passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30);
			prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30);
			analyseTrigger(passesPreTrigger, passesTrigger, histFolder, jet4, prescale);
		}
	}
}

void HLTriggerAnalyser::analyseTrigger(bool passesPreTrigger, bool passesTrigger, std::string histFolder,
		const JetPointer jet, int prescale) {
	if (passesPreTrigger) {
		histMan->setCurrentCollection(histFolder);

		histMan->H1D("jet_pt_visited")->Fill(jet->pt());
		histMan->H1D("jet_eta_visited")->Fill(jet->eta());
		histMan->H1D("jet_phi_visited")->Fill(jet->phi());

		if (passesTrigger) {
			histMan->H1D("jet_pt_fired")->Fill(jet->pt(), prescale);
			histMan->H1D("jet_eta_fired")->Fill(jet->eta(), prescale);
			histMan->H1D("jet_phi_fired")->Fill(jet->phi(), prescale);
		}
	}

}

void HLTriggerAnalyser::createHistograms() {
	//switch to jet binned!!!!!!!!

	std::cout << "creating HLTStudy histograms" << std::endl;
	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30");
	// ele tight ID + 1 jet
	histMan->addH1D("jet_pt_visited", "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D("jet_eta_visited", "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D("jet_phi_visited", "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D("jet_pt_fired", "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D("jet_eta_fired", "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D("jet_phi_fired", "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet phi (fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30");
	// ele tight ID + 2 jet
	histMan->addH1D("jet_pt_visited", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D("jet_eta_visited", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D("jet_phi_visited", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D("jet_pt_fired", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D("jet_eta_fired", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D("jet_phi_fired", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet phi (fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30");
	// ele tight ID + 3 jet
	histMan->addH1D("jet_pt_visited", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D("jet_eta_visited", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D("jet_phi_visited", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D("jet_pt_fired", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D("jet_eta_fired", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D("jet_phi_fired", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet phi (fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30");
	// ele tight ID + 4 jet
	histMan->addH1D("jet_pt_visited", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D("jet_eta_visited", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D("jet_phi_visited", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D("jet_pt_fired", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D("jet_eta_fired", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D("jet_phi_fired", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet phi (fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30");
	// ele tight ID + 1 jet
	histMan->addH1D("jet_pt_visited", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet pt (visited)", 200,
			0, 200);
	histMan->addH1D("jet_eta_visited", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet eta (visited)", 80,
			-4, 4);
	histMan->addH1D("jet_phi_visited", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet phi (visited)", 80,
			-4, 4);

	histMan->addH1D("jet_pt_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet pt (fired)", 200, 0,
			200);
	histMan->addH1D("jet_eta_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet eta (fired)", 80, -4,
			4);
	histMan->addH1D("jet_phi_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet phi (fired)", 80, -4,
			4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30");
	// ele tight ID + 2 jet
	histMan->addH1D("jet_pt_visited", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet pt (visited)",
			200, 0, 200);
	histMan->addH1D("jet_eta_visited", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet eta (visited)",
			80, -4, 4);
	histMan->addH1D("jet_phi_visited", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet phi (visited)",
			80, -4, 4);

	histMan->addH1D("jet_pt_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet pt (fired)", 200, 0,
			200);
	histMan->addH1D("jet_eta_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet eta (fired)", 80,
			-4, 4);
	histMan->addH1D("jet_phi_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet phi (fired)", 80,
			-4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30");
	// ele tight ID + 3 jet
	histMan->addH1D("jet_pt_visited", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet pt (visited)",
			200, 0, 200);
	histMan->addH1D("jet_eta_visited", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet eta (visited)",
			80, -4, 4);
	histMan->addH1D("jet_phi_visited", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet phi (visited)",
			80, -4, 4);

	histMan->addH1D("jet_pt_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet pt (fired)", 200,
			0, 200);
	histMan->addH1D("jet_eta_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet eta (fired)", 80,
			-4, 4);
	histMan->addH1D("jet_phi_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet phi (fired)", 80,
			-4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30");
	// ele tight ID + 4 jet
	histMan->addH1D("jet_pt_visited", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet pt (visited)",
			200, 0, 200);
	histMan->addH1D("jet_eta_visited", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet eta (visited)",
			80, -4, 4);
	histMan->addH1D("jet_phi_visited", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet phi (visited)",
			80, -4, 4);

	histMan->addH1D("jet_pt_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet pt (fired)", 200,
			0, 200);
	histMan->addH1D("jet_eta_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet eta (fired)", 80,
			-4, 4);
	histMan->addH1D("jet_phi_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet phi (fired)", 80,
			-4, 4);

	//	createdHistograms = true;
}

}
