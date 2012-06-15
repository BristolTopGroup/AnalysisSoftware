/*
 * HLTriggerAnalyser.cpp
 *
 *  Created on: Jun 21, 2011
 *      Author: lkreczko
 */

#include "../../interface/Analysers/HLTriggerTurnOnAnalyser.h"
#include <iostream>
#include <math.h>
namespace BAT {

HLTriggerTurnOnAnalyser::HLTriggerTurnOnAnalyser(boost::shared_ptr<HistogramManager> histMan,
		std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		weight(1.), //
		triggerEfficiencies(), //
		topEplusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection()) {
}

HLTriggerTurnOnAnalyser::~HLTriggerTurnOnAnalyser() {

}

void HLTriggerTurnOnAnalyser::analyse(const EventPtr event) {
//	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));
	/*
	 * triggers use (calo + tracker) electrons with ID looser than WP 80
	 * early triggers don't require isolation
	 *
	 * Trigger analysis:
	 * require at least one good WP 70 (2010 analysis) electron
	 * clean jets against most isolated electron canditate
	 *
	 *
	 */

	bool isData(event->isRealData());
	weight = event->weight();
	unsigned int currentJetBin = histMan_->getCurrentJetBin();
	unsigned int currentBJetBin = histMan_->getCurrentBJetBin();

	ElectronCollection electrons = event->Electrons();
	ElectronCollection goodElectrons;

	for (unsigned int index = 0; index < electrons.size(); ++index) {
		const ElectronPointer electron(electrons.at(index));
		bool passesID = electron->passesElectronID(ElectronID::SimpleCutBasedWP70);
			bool passesEt = electron->et() > 45;
			bool passesEta = fabs(electron->eta()) < 2.5 && !electron->isInCrack();
			bool passesD0 = fabs(electron->d0()) < 0.02; //cm
		//	//since H/E is used at trigger level, use the same cut here:
			bool passesHOverE = electron->HadOverEm() < 0.05; // same for EE and EB
		if (passesEt && passesEta && passesD0 && passesID && passesHOverE) {
			goodElectrons.push_back(electrons.at(index));
		}
	}

	if (!goodElectrons.size() > 0)
		return; //need at least one good electron to continue

	ElectronPointer mostIsolatedElectron = goodElectrons.front();

	//start with next electron
	for (unsigned int index = 1; index < goodElectrons.size(); ++index) {
		if (mostIsolatedElectron->relativeIsolation() > goodElectrons.at(index)->relativeIsolation())
			mostIsolatedElectron = goodElectrons.at(index);
	}

	//all jets in the v2 nTuples have the following cuts applied:
	//pt > 20 & abs(eta) < 2.5 && loose ID
	JetCollection jets = event->Jets();

	JetCollection cleanedJets, cleanedBJets;

	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet = jets.at(index);
		bool noElectronInVicinity = !jet->isWithinDeltaR(0.3, mostIsolatedElectron);
		bool passesEta = fabs(jet->eta()) < 2.4;
		bool passesBTag = jet->isBJet(BtagAlgorithm::CombinedSecondaryVertex, BtagAlgorithm::MEDIUM);
		if (noElectronInVicinity && passesEta) {
			cleanedJets.push_back(jet);
			if (passesBTag)
				cleanedBJets.push_back(jet);
		}

	}

	if (!cleanedJets.size() > 0)
		return; //need at least one jet to continue

	//set jet binning for HLT analysis
	histMan_->setCurrentJetBin(cleanedJets.size());
	histMan_->setCurrentBJetBin(cleanedBJets.size());
	bool passesPreCondition(false);
	bool passesTrigger(false);
	std::string histFolder("");
	int prescale(1);

	if (cleanedJets.size() > 0) { // exactly one jet
		const JetPointer jet1(cleanedJets.front());
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30";
		if (isData) {
			passesPreCondition = (event->HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT))
					&& event->runnumber() <= 178380;
		} else {
			//no pre-trigger for MC as triggers here are not prescaled
			passesPreCondition = true;
		}

		passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30);
		prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30);
		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet1, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 1 jet
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30";

			if (isData) {
				passesPreCondition = event->runnumber() > 165970
						&& (event->HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT));
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
			} else {
				//emulating trigger for MC, not necessary for Fall11 MC
				passesPreCondition = true;
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
				prescale = 1;
			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet1, prescale);
		}

	}

	if (cleanedJets.size() > 1) {
		const JetPointer jet2(cleanedJets.at(1));
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30";
		passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30);
		if (isData)
			passesPreCondition = passesPreCondition && event->runnumber() <= 178380;
		passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30)
				|| event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30);

		if (event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30))
			prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30);

		if (event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30))
			prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30);

		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet2, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 2 jets
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30";
			if (isData) {
				passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
				passesPreCondition = passesPreCondition && event->runnumber() > 165970 && event->runnumber() <= 178380;
			} else {
				passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);
			}

			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet2, prescale);
		}
	}

	if (cleanedJets.size() > 2) { //TODO: if nJets > 3, 4th jet > 30 GeV?
		const JetPointer jet3(cleanedJets.at(2));
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30";

		passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30)
				|| event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30);
		if (isData)
			passesPreCondition = passesPreCondition && event->runnumber() <= 165633;

		passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30)
				|| event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);

		if (event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30))
			prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);

		if (event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30))
			prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);

		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet3, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 3 jets
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30";

			if (isData) {
				passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);
				passesPreCondition = passesPreCondition && event->runnumber() > 165970 && event->runnumber() <= 178380;
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);

			} else {
				passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet3, prescale);
		}
	}

	if (cleanedJets.size() > 3) {
		const JetPointer jet4(cleanedJets.at(3));
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30";
		if (isData) {
			//run 165970, first occurance of the QuadJet trigger
			passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30)
					&& event->runnumber() > 165970 && event->runnumber() <= 178380;

		} else {
			prescale = 1;
			passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);
			//no emulation so far
			passesTrigger = false;
		}

		passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30);
		prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30);

		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet4, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 4 jets
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30";
			if (isData) {
				passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
				passesPreCondition = passesPreCondition && event->runnumber() > 165970 && event->runnumber() <= 178380;
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30);
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30);

			} else {
				passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30);
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30);
			}

			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet4, prescale);
		}
	}

	//PF jet triggers
	if (cleanedJets.size() > 0) { // exactly one jet
		const JetPointer jet1(cleanedJets.front());
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30";
		if (isData) {
			passesPreCondition = (event->HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT))
					&& event->runnumber() > 178380;
			passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30);
			prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30);
		} else {
			//no pre-trigger for MC as triggers here are not prescaled
			passesPreCondition = true;
			passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30);
			prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30);
		}

		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet1, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 1 jet
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30";

			if (isData) {
				passesPreCondition = event->runnumber() > 178380
						&& (event->HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT));
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30);
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30);
			} else {
				passesPreCondition = true;
				//emulating trigger for MC
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet1, prescale);
		}

	}

	if (cleanedJets.size() > 1) {
		const JetPointer jet2(cleanedJets.at(1));
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30";
		if (isData) {
			passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30);
			passesPreCondition = passesPreCondition && event->runnumber() > 178380;
			passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30);
			prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30);
		} else {
			passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30);
			passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30);
			prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30);
		}

		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet2, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 2 jets
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30";

			if (isData) {
				passesPreCondition = event->runnumber() > 178380
						&& event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30);
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30);
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30);
			} else {

				passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);

			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet2, prescale);
		}
	}

	if (cleanedJets.size() > 2) {
		const JetPointer jet3(cleanedJets.at(2));
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30";

		if (isData) {
			passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30);
			passesPreCondition = passesPreCondition && event->runnumber() > 178380;
			passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30);
			prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30);
		} else {
			passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30);
			passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);
			prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);
		}

		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet3, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 3 jets
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30";

			if (isData) {
				passesPreCondition = event->runnumber() > 178380
						&& event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30);
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
			} else {
				passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);

			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet3, prescale);
		}
	}

	if (cleanedJets.size() > 3) {
		const JetPointer jet4(cleanedJets.at(3));
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30";
		if (isData) {
			//run 165970, first occurance of the QuadJet trigger
			passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30)
					&& event->runnumber() > 178380;
			passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30);
			prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30);
		} else {
			prescale = 1;
			passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);
			passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30);
		}
		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet4, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 4 jets
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30";
			if (isData) {
				passesPreCondition = event->HLT(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30);
				prescale = event->HLTPrescale(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30);
			} else {
				passesPreCondition = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
				passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30);
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30);

			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet4, prescale);
		}
	}
//	//restore normal jet binning
//	histMan_->setCurrentJetBin(ttbarEvent.GoodJets().size());

	passesTrigger = false;

	//same trigger for MC and data
	passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30)
			|| event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);

	if ((isData && event->runnumber() <= 178380) || !isData) {
		for (unsigned int analysis = AnalysisReference::Ele30_TriPFJet30;
				analysis < AnalysisReference::NUMBER_OF_TRIGGEREFFICIENCY_CASES; analysis++) {
			//only in the range it was used as a signal trigger
			if ((isData && event->runnumber() > 160404 && event->runnumber() <= 165633) || !isData)
				analyseTriggerEfficiency((AnalysisReference::value) analysis,
						"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30", passesTrigger, event);
		}

		passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);

		for (unsigned int analysis = AnalysisReference::Ele30_TriPFJet30;
				analysis < AnalysisReference::NUMBER_OF_TRIGGEREFFICIENCY_CASES; analysis++) {
			if ((isData && event->runnumber() > 165633 && event->runnumber() <= 178380) || !isData)
				analyseTriggerEfficiency((AnalysisReference::value) analysis,
						"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30", passesTrigger, event);
		}
	}
	if ((isData && event->runnumber() > 178380) || !isData) {
		//PF jet triggers
		passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30);

		for (unsigned int analysis = AnalysisReference::Ele30_TriPFJet30;
				analysis < AnalysisReference::NUMBER_OF_TRIGGEREFFICIENCY_CASES; analysis++) {
			analyseTriggerEfficiency((AnalysisReference::value) analysis, "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30",
					passesTrigger, event);
		}

		if (isData)
			passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
		else {
			//Fall11
			passesTrigger = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
			//Summer12
		}

		for (unsigned int analysis = AnalysisReference::Ele30_TriPFJet30;
				analysis < AnalysisReference::NUMBER_OF_TRIGGEREFFICIENCY_CASES; analysis++) {
			analyseTriggerEfficiency((AnalysisReference::value) analysis,
					"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30", passesTrigger, event);
		}
	}
	//restore normal jet binning
	histMan_->setCurrentJetBin(currentJetBin);
	histMan_->setCurrentBJetBin(currentBJetBin);

}
//TODO: JetPointer -> JetCollection, add uint nthJet
// remove passesPreCondition and add it before the execution of the function
//Create trigger object (not here) containing
// triggerResult, trigger prescale
void HLTriggerTurnOnAnalyser::analyseTrigger(bool passesPreCondition, bool passesTrigger, std::string histFolder,
		const JetPointer jet, int prescale) {
	if (passesPreCondition) {
		histMan_->setCurrentHistogramFolder(histFolder);

		histMan_->H1D_JetBinned("jet_pt_visited")->Fill(jet->pt(), weight);
		histMan_->H1D_JetBinned("jet_eta_visited")->Fill(jet->eta(), weight);
		histMan_->H1D_JetBinned("jet_phi_visited")->Fill(jet->phi(), weight);
		if (histMan_->getCurrentJetBin() == 3) {
			histMan_->H1D_BJetBinned("jet_pt_visited_3jets")->Fill(jet->pt(), weight);
			histMan_->H1D_BJetBinned("jet_eta_visited_3jets")->Fill(jet->eta(), weight);
			histMan_->H1D_BJetBinned("jet_phi_visited_3jets")->Fill(jet->phi(), weight);
		}

		if (histMan_->getCurrentJetBin() > 3) {
			histMan_->H1D_BJetBinned("jet_pt_visited")->Fill(jet->pt(), weight);
			histMan_->H1D_BJetBinned("jet_eta_visited")->Fill(jet->eta(), weight);
			histMan_->H1D_BJetBinned("jet_phi_visited")->Fill(jet->phi(), weight);
		}

		if (jet->pt() > 30.) {
			histMan_->H1D_JetBinned("jet_eta_PtGT30_visited")->Fill(jet->eta(), weight);
			histMan_->H1D_JetBinned("jet_phi_PtGT30_visited")->Fill(jet->phi(), weight);
		}

		if (jet->pt() > 35.) {
			histMan_->H1D_JetBinned("jet_eta_PtGT35_visited")->Fill(jet->eta(), weight);
			histMan_->H1D_JetBinned("jet_phi_PtGT35_visited")->Fill(jet->phi(), weight);
		}

		if (jet->pt() > 40.) {
			histMan_->H1D_JetBinned("jet_eta_PtGT40_visited")->Fill(jet->eta(), weight);
			histMan_->H1D_JetBinned("jet_phi_PtGT40_visited")->Fill(jet->phi(), weight);
		}
		if (jet->pt() > 45.) {
			histMan_->H1D_JetBinned("jet_eta_PtGT45_visited")->Fill(jet->eta(), weight);
			histMan_->H1D_JetBinned("jet_phi_PtGT45_visited")->Fill(jet->phi(), weight);
		}

		if (passesTrigger) {
			histMan_->H1D_JetBinned("jet_pt_fired")->Fill(jet->pt(), prescale * weight);
			histMan_->H1D_JetBinned("jet_eta_fired")->Fill(jet->eta(), prescale * weight);
			histMan_->H1D_JetBinned("jet_phi_fired")->Fill(jet->phi(), prescale * weight);
			if (histMan_->getCurrentJetBin() == 3) {
				histMan_->H1D_BJetBinned("jet_pt_fired_3jets")->Fill(jet->pt(), prescale * weight);
				histMan_->H1D_BJetBinned("jet_eta_fired_3jets")->Fill(jet->eta(), prescale * weight);
				histMan_->H1D_BJetBinned("jet_phi_fired_3jets")->Fill(jet->phi(), prescale * weight);
			}
			if (histMan_->getCurrentJetBin() > 3) {
				histMan_->H1D_BJetBinned("jet_pt_fired")->Fill(jet->pt(), prescale * weight);
				histMan_->H1D_BJetBinned("jet_eta_fired")->Fill(jet->eta(), prescale * weight);
				histMan_->H1D_BJetBinned("jet_phi_fired")->Fill(jet->phi(), prescale * weight);
			}
			if (jet->pt() > 30.) {
				histMan_->H1D_JetBinned("jet_eta_PtGT30_fired")->Fill(jet->eta(), weight);
				histMan_->H1D_JetBinned("jet_phi_PtGT30_fired")->Fill(jet->phi(), weight);
			}
			if (jet->pt() > 35.) {
				histMan_->H1D_JetBinned("jet_eta_PtGT35_fired")->Fill(jet->eta(), weight);
				histMan_->H1D_JetBinned("jet_phi_PtGT35_fired")->Fill(jet->phi(), weight);
			}
			if (jet->pt() > 40.) {
				histMan_->H1D_JetBinned("jet_eta_PtGT40_fired")->Fill(jet->eta(), weight);
				histMan_->H1D_JetBinned("jet_phi_PtGT40_fired")->Fill(jet->phi(), weight);
			}

			if (jet->pt() > 45.) {
				histMan_->H1D_JetBinned("jet_eta_PtGT45_fired")->Fill(jet->eta(), weight);
				histMan_->H1D_JetBinned("jet_phi_PtGT45_fired")->Fill(jet->phi(), weight);
			}
		}
	}

}

void HLTriggerTurnOnAnalyser::analyseTriggerEfficiency(AnalysisReference::value analysis, std::string triggerName,
		bool passesTrigger, const EventPtr event) {

	double triggerResult = passesTrigger ? 1.0 : 0.;

	if (std::isnan(triggerResult) || std::isinf(triggerResult))
		cout << "Trigger result is infinite or not a number" << endl;

	if (std::isnan(weight) || std::isinf(weight))
		cout << "Weight is infinite or not a number" << endl;

	histMan_->setCurrentHistogramFolder("HLTStudy/" + triggerName + "/TriggerEfficiency");
	bool passesCuts = true;
	//all common cuts except HLT
	for (unsigned int cut = TTbarEPlusJetsReferenceSelection::OneIsolatedElectron;
			cut <= TTbarEPlusJetsReferenceSelection::AtLeastThreeGoodJets; cut++) {
		passesCuts = passesCuts
				&& topEplusJetsRefSelection_->passesSelectionStep(event, (TTbarEPlusJetsReferenceSelection::Step) cut);
	}
	bool passesRequired = false;

	switch (analysis) {
	case AnalysisReference::Ele30_TriPFJet30:
		//require == 3 jets
		passesRequired = !topEplusJetsRefSelection_->passesSelectionStep(event, TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets);
		if (passesRequired && passesCuts)
			histMan_->H1D("Ele30_TriPFJet30")->Fill(triggerResult, weight);
		break;
	case AnalysisReference::Ele30_QuadPFJet30:
		passesRequired = topEplusJetsRefSelection_->passesSelectionStep(event, TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets);
		if (passesRequired && passesCuts)
			histMan_->H1D("Ele30_QuadPFJet30")->Fill(triggerResult, weight);
		break;

//	case AnalysisReference::Ele30_PFJet70_PFJet50_PFJet30:
//		//require == 3 jets
//		passesRequired = !topEplusJetsRefSelection_->passesSelectionStep(event, TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets);
//		//require  >= 1 jet above 70GeV and >=2 jets above 50GeV
//		passesRequired = passesRequired
//				&& event->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AsymmetricJetCuts);
//		if (passesRequired && passesCuts)
//			histMan_->H1D("Ele30_PFJet70_PFJet50_PFJet30")->Fill(triggerResult, weight);
//		break;

//	case AnalysisReference::Ele30_PFJet70_PFJet50_PFJet30_PFJet30:
//		passesRequired = event->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AtLeastFourGoodJets);
//		//require  >= 1 jet above 70GeV and >=2 jets above 50GeV
//		passesRequired = passesRequired
//				&& event->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AsymmetricJetCuts);
//		if (passesRequired && passesCuts)
//			histMan_->H1D("Ele30_PFJet70_PFJet50_PFJet30_PFJet30")->Fill(triggerResult, weight);
//		break;

	default:
		return;
	}

}

void HLTriggerTurnOnAnalyser::createHistograms() {

	createHistograms("HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30");
	createHistograms("HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30");
	createHistograms("HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30");
	createHistograms("HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30");

	createHistograms("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30");
	createHistograms("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30");
	createHistograms("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30");
	createHistograms("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30");

	createHistograms("HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30");
	createHistograms("HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30");
	createHistograms("HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30");
	createHistograms("HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30");

	createHistograms("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30");
	createHistograms("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30");
	createHistograms("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30");
	createHistograms("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30");

}

void HLTriggerTurnOnAnalyser::createHistograms(std::string trigger) {
	histMan_->setCurrentHistogramFolder("HLTStudy/" + trigger);

	//kinematic distributions for HLT path when visited
	histMan_->addH1D_JetBinned("jet_pt_visited", trigger + " jet pt (visited)", 200, 0, 200);
	histMan_->addH1D_JetBinned("jet_eta_visited", trigger + " jet eta (visited)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_phi_visited", trigger + " jet phi (visited)", 80, -4, 4);
	//kinematic distributions for HLT path when visited and fired
	histMan_->addH1D_JetBinned("jet_pt_fired", trigger + " jet pt (fired)", 200, 0, 200);
	histMan_->addH1D_JetBinned("jet_eta_fired", trigger + " jet eta (fired)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_phi_fired", trigger + " jet phi (fired)", 80, -4, 4);

	//pre-plateau regions in 5 GeV steps
	histMan_->addH1D_JetBinned("jet_eta_PtGT30_visited", trigger + " jet eta (jet pT > 30 GeV, visited)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_phi_PtGT30_visited", trigger + " jet phi (jet pT > 30 GeV, visited)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_eta_PtGT30_fired", trigger + " jet eta (jet pT > 30 GeV, fired)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_phi_PtGT30_fired", trigger + " jet phi (jet pT > 30 GeV, fired)", 80, -4, 4);

	histMan_->addH1D_JetBinned("jet_eta_PtGT35_visited", trigger + " jet eta (jet pT > 35 GeV, visited)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_phi_PtGT35_visited", trigger + " jet phi (jet pT > 35 GeV, visited)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_eta_PtGT35_fired", trigger + " jet eta (jet pT > 35 GeV, fired)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_phi_PtGT35_fired", trigger + " jet phi (jet pT > 35 GeV, fired)", 80, -4, 4);

	histMan_->addH1D_JetBinned("jet_eta_PtGT40_visited", trigger + " jet eta (jet pT > 40 GeV, visited)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_phi_PtGT40_visited", trigger + " jet phi (jet pT > 40 GeV, visited)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_eta_PtGT40_fired", trigger + " jet eta (jet pT > 40 GeV, fired)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_phi_PtGT40_fired", trigger + " jet phi (jet pT > 40 GeV, fired)", 80, -4, 4);

	//plateau region
	histMan_->addH1D_JetBinned("jet_eta_PtGT45_visited", trigger + " jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_phi_PtGT45_visited", trigger + " jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_eta_PtGT45_fired", trigger + " jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan_->addH1D_JetBinned("jet_phi_PtGT45_fired", trigger + " jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

//	histMan_->setCurrentCollection("HLTStudy/" + trigger + "/FlavourDependence");
	//kinematic distributions for HLT path when visited
	histMan_->addH1D_BJetBinned("jet_pt_visited", trigger + " jet pt (visited, >=4 jets)", 200, 0, 200);
	histMan_->addH1D_BJetBinned("jet_eta_visited", trigger + " jet eta (visited, >=4 jets)", 80, -4, 4);
	histMan_->addH1D_BJetBinned("jet_phi_visited", trigger + " jet phi (visited, >=4 jets)", 80, -4, 4);
	//kinematic distributions for HLT path when visited and fired
	histMan_->addH1D_BJetBinned("jet_pt_fired", trigger + " jet pt (fired, >=4 jets)", 200, 0, 200);
	histMan_->addH1D_BJetBinned("jet_eta_fired", trigger + " jet eta (fired, >=4 jets)", 80, -4, 4);
	histMan_->addH1D_BJetBinned("jet_phi_fired", trigger + " jet phi (fired, >=4 jets)", 80, -4, 4);

	histMan_->addH1D_BJetBinned("jet_pt_visited_3jets", trigger + " jet pt (visited, ==3 jets)", 200, 0, 200);
	histMan_->addH1D_BJetBinned("jet_eta_visited_3jets", trigger + " jet eta (visited, ==3 jets)", 80, -4, 4);
	histMan_->addH1D_BJetBinned("jet_phi_visited_3jets", trigger + " jet phi (visited, ==3 jets)", 80, -4, 4);
	//kinematic distributions for HLT path when visited and fired
	histMan_->addH1D_BJetBinned("jet_pt_fired_3jets", trigger + " jet pt (fired, ==3 jets) ", 200, 0, 200);
	histMan_->addH1D_BJetBinned("jet_eta_fired_3jets", trigger + " jet eta (fired, ==3 jets)", 80, -4, 4);
	histMan_->addH1D_BJetBinned("jet_phi_fired_3jets", trigger + " jet phi (fired, ==3 jets)", 80, -4, 4);
	//signal trigger efficiencies
	if (trigger == "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30"
			|| trigger == "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30"
			|| trigger == "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30"
			|| trigger == "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30"
			|| trigger == "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30"
			|| trigger == "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30"
			|| trigger == "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30"
			|| trigger == "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30") {

		histMan_->setCurrentHistogramFolder("HLTStudy/" + trigger + "/TriggerEfficiency");
		histMan_->addH1D("Ele30_TriPFJet30", "Signal trigger effiency for Ele30 + 3x PFJet30", 2, -0.5, 1.5);
		histMan_->addH1D("Ele30_QuadPFJet30", "Signal trigger effiency for Ele30 + 4x PFJet30", 2, -0.5, 1.5);
		histMan_->addH1D("Ele30_PFJet70_PFJet50_PFJet30", "Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30",
				2, -0.5, 1.5);
		histMan_->addH1D("Ele30_PFJet70_PFJet50_PFJet30_PFJet30",
				"Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30_PFJet30", 2, -0.5, 1.5);

	}
}

}
