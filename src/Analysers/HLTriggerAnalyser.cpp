/*
 * HLTriggerAnalyser.cpp
 *
 *  Created on: Jun 21, 2011
 *      Author: lkreczko
 */

#include "../../interface/Analysers/HLTriggerAnalyser.h"
#include <iostream>
#include <math.h>
namespace BAT {

HLTriggerAnalyser::HLTriggerAnalyser(boost::shared_ptr<HistogramManager> histMan) :
		BasicAnalyser(histMan), //
		weight(1.), //
		triggerEfficiencies()//
{
}

HLTriggerAnalyser::~HLTriggerAnalyser() {

}

void HLTriggerAnalyser::analyse(const TopPairEventCandidate& ttbarEvent) {
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

	bool isData(ttbarEvent.isRealData());
	weight = ttbarEvent.weight();

	ElectronCollection electrons = ttbarEvent.Electrons();
	ElectronCollection goodElectrons;

	for (unsigned int index = 0; index < electrons.size(); ++index) {
		if (electrons.at(index)->isGood(ElectronID::SimpleCutBasedWP70) && electrons.at(index)->et() > 45) {
			//Et > 35GeV, d0 < 0.02cm, |z-vertex.z} < 1cm, Electron ID WP 70 (no isolation)
			// get on the plateau of the electron turn-on curve
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
	JetCollection jets = ttbarEvent.Jets();

	JetCollection cleanedJets;

	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet = jets.at(index);
		bool noElectronInVicinity = !jet->isWithinDeltaR(0.3, mostIsolatedElectron);
		bool passesEta = fabs(jet->eta()) < 2.4;
		if (noElectronInVicinity && passesEta)
			cleanedJets.push_back(jet);
	}

	if (!cleanedJets.size() > 0)
		return; //need at least one jet to continue

	//set jet binning for HLT analysis
	histMan->setCurrentJetBin(cleanedJets.size());
	bool passesPreCondition(false);
	bool passesTrigger(false);
	std::string histFolder("");
	int prescale(1);

	if (cleanedJets.size() > 0) { // exactly one jet
		const JetPointer jet1(cleanedJets.front());
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30";
		if (isData) {
			passesPreCondition = (ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT)
					|| ttbarEvent.HLT(HLTriggers::HLT_Ele25_WP80_PFMT40)
					|| ttbarEvent.HLT(HLTriggers::HLT_Ele27_WP70_PFMT40_PFMHT20)) && ttbarEvent.runnumber() <= 178380;
		} else {
			//no pre-trigger for MC as triggers here are not prescaled
			passesPreCondition = true;
		}

		passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30);
		prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30);
		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet1, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 1 jet
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30";

			if (isData) {
				passesPreCondition = ttbarEvent.runnumber() > 165970
						&& (ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT)
								|| ttbarEvent.HLT(HLTriggers::HLT_Ele25_WP80_PFMT40)
								|| ttbarEvent.HLT(HLTriggers::HLT_Ele27_WP70_PFMT40_PFMHT20));
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
				prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
			} else {
				passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
				//emulating trigger for MC
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30)
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
				prescale = 1;
			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet1, prescale);
		}

	}

	if (cleanedJets.size() > 1) {
		const JetPointer jet2(cleanedJets.at(1));
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30";
		passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30);
		if (isData)
			passesPreCondition = passesPreCondition && ttbarEvent.runnumber() <= 178380;
		passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30)
				|| ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30);

		if (ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30))
			prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30);

		if (ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30))
			prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30);

		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet2, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 2 jets
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30";

			if (isData) {
				passesPreCondition = ttbarEvent.runnumber() > 165970 && ttbarEvent.runnumber() <= 178380
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30);
				prescale = ttbarEvent.HLTPrescale(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);
			} else {
				passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30)
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
				prescale = 1;
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30)
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet2, prescale);
		}
	}

	if (cleanedJets.size() > 2) {
		const JetPointer jet3(cleanedJets.at(2));
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30";

		passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30)
				|| ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30);
		if (isData)
			passesPreCondition = passesPreCondition && ttbarEvent.runnumber() <= 165633;

		passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30)
				|| ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);

		if (ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30))
			prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);

		if (ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30))
			prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);

		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet3, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 3 jets
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30";

			if (isData) {
				passesPreCondition = ttbarEvent.runnumber() > 165970 && ttbarEvent.runnumber() <= 178380
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30);
				prescale = ttbarEvent.HLTPrescale(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
			} else {
				passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30)
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
				prescale = 1;
				//emulate trigger
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30)
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet3, prescale);
		}
	}

	if (cleanedJets.size() > 3) {
		const JetPointer jet4(cleanedJets.at(3));
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30";
		if (isData) {
			//run 165970, first occurance of the QuadJet trigger
			passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30)
					&& ttbarEvent.runnumber() > 165970 && ttbarEvent.runnumber() <= 178380;
			passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30);
			prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30);
		} else {
			prescale = 1;
			passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);
			//no emulation so far
			passesTrigger = false;
		}
		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet4, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 4 jets
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30";
			if (isData) {
				passesPreCondition = ttbarEvent.HLT(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30);
				prescale = ttbarEvent.HLTPrescale(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30);
			} else {
				prescale = 1;
				passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30)
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
				//no emulation so far
				passesTrigger = false;
			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet4, prescale);
		}
	}

	//PF jet triggers
	if (cleanedJets.size() > 0) { // exactly one jet
		const JetPointer jet1(cleanedJets.front());
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30";
		if (isData) {
			passesPreCondition = (ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT)
					|| ttbarEvent.HLT(HLTriggers::HLT_Ele25_WP80_PFMT40)
					|| ttbarEvent.HLT(HLTriggers::HLT_Ele27_WP70_PFMT40_PFMHT20)) && ttbarEvent.runnumber() > 178380;
		} else {
			//no pre-trigger for MC as triggers here are not prescaled
			passesPreCondition = true;
		}

		passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30);
		prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30);
		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet1, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 1 jet
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30";

			if (isData) {
				passesPreCondition = ttbarEvent.runnumber() > 178380
						&& (ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT)
								|| ttbarEvent.HLT(HLTriggers::HLT_Ele25_WP80_PFMT40)
								|| ttbarEvent.HLT(HLTriggers::HLT_Ele27_WP70_PFMT40_PFMHT20));
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30);
				prescale = ttbarEvent.HLTPrescale(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30);
			} else {
				passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
				//emulating trigger for MC
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30)
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
				prescale = 1;
			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet1, prescale);
		}

	}

	if (cleanedJets.size() > 1) {
		const JetPointer jet2(cleanedJets.at(1));
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30";
		passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30);
		if (isData)
			passesPreCondition = passesPreCondition && ttbarEvent.runnumber() > 178380;
		passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30);

		if (ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30))
			prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30);

		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet2, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 2 jets
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30";

			if (isData) {
				passesPreCondition = ttbarEvent.runnumber() > 178380
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30);
				prescale = ttbarEvent.HLTPrescale(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30);
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30);
			} else {
				passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30)
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
				prescale = 1;
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30)
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet2, prescale);
		}
	}

	if (cleanedJets.size() > 2) {
		const JetPointer jet3(cleanedJets.at(2));
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30";

		passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30);
		if (isData)
			passesPreCondition = passesPreCondition && ttbarEvent.runnumber() > 178380;

		passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30);

		if (ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30))
			prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30);

		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet3, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 3 jets
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30";

			if (isData) {
				passesPreCondition = ttbarEvent.runnumber() > 178380
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30);
				prescale = ttbarEvent.HLTPrescale(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
				passesTrigger = ttbarEvent.HLT(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
			} else {
				passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralDiJet30)
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
				prescale = 1;
				//emulate trigger
				passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30)
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet3, prescale);
		}
	}

	if (cleanedJets.size() > 3) {
		const JetPointer jet4(cleanedJets.at(3));
		histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30";
		if (isData) {
			//run 165970, first occurance of the QuadJet trigger
			passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30)
					&& ttbarEvent.runnumber() > 178380;
			passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30);
			prescale = ttbarEvent.HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30);
		} else {
			prescale = 1;
			passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);
			//no emulation so far
			passesTrigger = false;
		}
		analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet4, prescale);

		if (mostIsolatedElectron->relativeIsolation() < 0.1) {
			//isoEle + 4 jets
			histFolder = "HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30";
			if (isData) {
				passesPreCondition = ttbarEvent.HLT(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
				passesTrigger = ttbarEvent.HLT(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30);
				prescale = ttbarEvent.HLTPrescale(
						HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30);
			} else {
				prescale = 1;
				passesPreCondition = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30)
						&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);
				//no emulation so far
				passesTrigger = false;
			}
			analyseTrigger(passesPreCondition, passesTrigger, histFolder, jet4, prescale);
		}
	}
	//restore normal jet binning
	histMan->setCurrentJetBin(ttbarEvent.GoodJets().size());

	passesTrigger = false;

	//same trigger for MC and data
	passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30)
			|| ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);

	if ((isData && ttbarEvent.runnumber() <= 178380) || !isData) {
		for (unsigned int analysis = AnalysisReference::Ele30_TriPFJet30;
				analysis < AnalysisReference::NUMBER_OF_TRIGGEREFFICIENCY_CASES; analysis++) {
			//only in the range it was used as a signal trigger
			if ((isData && ttbarEvent.runnumber() > 160404 && ttbarEvent.runnumber() <= 165633) || !isData)
				analyseTriggerEfficiency((AnalysisReference::value) analysis,
						"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30", passesTrigger, ttbarEvent);
		}

		if (isData)
			passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
		else
			passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30)
					&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);

		for (unsigned int analysis = AnalysisReference::Ele30_TriPFJet30;
				analysis < AnalysisReference::NUMBER_OF_TRIGGEREFFICIENCY_CASES; analysis++) {
			if ((isData && ttbarEvent.runnumber() > 165633 && ttbarEvent.runnumber() <= 178380) || !isData)
				analyseTriggerEfficiency((AnalysisReference::value) analysis,
						"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30", passesTrigger, ttbarEvent);
		}
	}
	if ((isData && ttbarEvent.runnumber() > 178380) || !isData) {
		//PF jet triggers
		passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30);

		for (unsigned int analysis = AnalysisReference::Ele30_TriPFJet30;
				analysis < AnalysisReference::NUMBER_OF_TRIGGEREFFICIENCY_CASES; analysis++) {
			analyseTriggerEfficiency((AnalysisReference::value) analysis, "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30",
					passesTrigger, ttbarEvent);
		}

		if (isData)
			passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
		else
			passesTrigger = ttbarEvent.HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30)
					&& ttbarEvent.HLT(HLTriggers::HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT);

		for (unsigned int analysis = AnalysisReference::Ele30_TriPFJet30;
				analysis < AnalysisReference::NUMBER_OF_TRIGGEREFFICIENCY_CASES; analysis++) {
			analyseTriggerEfficiency((AnalysisReference::value) analysis,
					"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30", passesTrigger, ttbarEvent);
		}
	}

}

void HLTriggerAnalyser::analyseTrigger(bool passesPreCondition, bool passesTrigger, std::string histFolder,
		const JetPointer jet, int prescale) {
	if (passesPreCondition) {
		histMan->setCurrentCollection(histFolder);

		histMan->H1D_JetBinned("jet_pt_visited")->Fill(jet->pt(), weight);
		histMan->H1D_JetBinned("jet_eta_visited")->Fill(jet->eta(), weight);
		histMan->H1D_JetBinned("jet_phi_visited")->Fill(jet->phi(), weight);

		if (jet->pt() > 45.) {
			histMan->H1D_JetBinned("jet_eta_PtGT45_visited")->Fill(jet->eta(), weight);
			histMan->H1D_JetBinned("jet_phi_PtGT45_visited")->Fill(jet->phi(), weight);
		}

		if (passesTrigger) {
			histMan->H1D_JetBinned("jet_pt_fired")->Fill(jet->pt(), prescale * weight);
			histMan->H1D_JetBinned("jet_eta_fired")->Fill(jet->eta(), prescale * weight);
			histMan->H1D_JetBinned("jet_phi_fired")->Fill(jet->phi(), prescale * weight);

			if (jet->pt() > 45.) {
				histMan->H1D_JetBinned("jet_eta_PtGT45_fired")->Fill(jet->eta(), weight);
				histMan->H1D_JetBinned("jet_phi_PtGT45_fired")->Fill(jet->phi(), weight);
			}
		}
	}

}

void HLTriggerAnalyser::analyseTriggerEfficiency(AnalysisReference::value analysis, std::string triggerName,
		bool passesTrigger, const TopPairEventCandidate& ttbarEvent) {


	double triggerResult = passesTrigger? 1.0: 0.;

	if (isnan(triggerResult) || isinf(triggerResult))
		cout << "Trigger result is infinite or not a number" << endl;

	if (isnan(weight) || isinf(weight))
		cout << "Weight is infinite or not a number" << endl;


	histMan->setCurrentCollection("HLTStudy/" + triggerName + "/TriggerEfficiency");
	bool passesCuts = true;
	//all common cuts except HLT
	for (unsigned int cut = TTbarEPlusJetsSelection::GoodPrimaryvertex;
			cut <= TTbarEPlusJetsSelection::AtLeastThreeGoodJets; cut++) {
		passesCuts = passesCuts && ttbarEvent.passesEPlusJetsSelectionStep((TTbarEPlusJetsSelection::Step) cut);
	}
	bool passesRequired = false;

	switch (analysis) {
	case AnalysisReference::Ele30_TriPFJet30:
		//require == 3 jets
		passesRequired = !ttbarEvent.passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AtLeastFourGoodJets);
		if (passesRequired && passesCuts)
			histMan->H1D("Ele30_TriPFJet30")->Fill(triggerResult, weight);
		break;
	case AnalysisReference::Ele30_QuadPFJet30:
		passesRequired = ttbarEvent.passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AtLeastFourGoodJets);
		if (passesRequired && passesCuts)
			histMan->H1D("Ele30_QuadPFJet30")->Fill(triggerResult, weight);
		break;

	case AnalysisReference::Ele30_PFJet70_PFJet50_PFJet30:
		//require == 3 jets
		passesRequired = !ttbarEvent.passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AtLeastFourGoodJets);
		//require  >= 1 jet above 70GeV and >=2 jets above 50GeV
		passesRequired = passesRequired
				&& ttbarEvent.passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AsymmetricJetCuts);
		if (passesRequired && passesCuts)
			histMan->H1D("Ele30_PFJet70_PFJet50_PFJet30")->Fill(triggerResult, weight);
		break;

	case AnalysisReference::Ele30_PFJet70_PFJet50_PFJet30_PFJet30:
		passesRequired = ttbarEvent.passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AtLeastFourGoodJets);
		//require  >= 1 jet above 70GeV and >=2 jets above 50GeV
		passesRequired = passesRequired
				&& ttbarEvent.passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AsymmetricJetCuts);
		if (passesRequired && passesCuts)
			histMan->H1D("Ele30_PFJet70_PFJet50_PFJet30_PFJet30")->Fill(triggerResult, weight);
		break;

	default:
		return;
	}

}

void HLTriggerAnalyser::createHistograms() {

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30");
	// ele tight ID + 1 jet
	histMan->addH1D_JetBinned("jet_pt_visited", "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_visited", "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_visited", "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D_JetBinned("jet_pt_fired", "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired", "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired", "HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet phi (fired)", 80, -4, 4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30");
	// ele tight ID + 2 jet
	histMan->addH1D_JetBinned("jet_pt_visited", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet pt (visited)", 200, 0,
			200);
	histMan->addH1D_JetBinned("jet_eta_visited", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet eta (visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_phi_visited", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet phi (visited)", 80, -4,
			4);

	histMan->addH1D_JetBinned("jet_pt_fired", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet phi (fired)", 80, -4, 4);
	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30");
	// ele tight ID + 3 jet
	histMan->addH1D_JetBinned("jet_pt_visited", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet pt (visited)", 200, 0,
			200);
	histMan->addH1D_JetBinned("jet_eta_visited", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet eta (visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_phi_visited", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet phi (visited)", 80, -4,
			4);

	histMan->addH1D_JetBinned("jet_pt_fired", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet phi (fired)", 80, -4, 4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30");
	// ele tight ID + 4 jet
	histMan->addH1D_JetBinned("jet_pt_visited", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet pt (visited)", 200, 0,
			200);
	histMan->addH1D_JetBinned("jet_eta_visited", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet eta (visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_phi_visited", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet phi (visited)", 80, -4,
			4);

	histMan->addH1D_JetBinned("jet_pt_fired", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet phi (fired)", 80, -4, 4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30");
	// ele tight ID + 1 jet
	histMan->addH1D_JetBinned("jet_pt_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D_JetBinned("jet_pt_fired", "HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet pt (fired)",
			200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet phi (fired)", 80, -4, 4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30");
	// ele tight ID + 2 jet
	histMan->addH1D_JetBinned("jet_pt_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D_JetBinned("jet_pt_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet phi (fired)", 80, -4, 4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30");
	// ele tight ID + 3 jet
	histMan->addH1D_JetBinned("jet_pt_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D_JetBinned("jet_pt_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet phi (fired)", 80, -4, 4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30");
	// ele tight ID + 4 jet
	histMan->addH1D_JetBinned("jet_pt_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D_JetBinned("jet_pt_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet phi (fired)", 80, -4, 4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	//PF Jet triggers
	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30");
	// ele tight ID + 1 jet
	histMan->addH1D_JetBinned("jet_pt_visited", "HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30 jet pt (visited)", 200, 0,
			200);
	histMan->addH1D_JetBinned("jet_eta_visited", "HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30 jet eta (visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_phi_visited", "HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30 jet phi (visited)", 80, -4,
			4);

	histMan->addH1D_JetBinned("jet_pt_fired", "HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired", "HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired", "HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30 jet phi (fired)", 80, -4, 4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_CentralPFJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30");
	// ele tight ID + 2 jet
	histMan->addH1D_JetBinned("jet_pt_visited", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30 jet pt (visited)", 200, 0,
			200);
	histMan->addH1D_JetBinned("jet_eta_visited", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30 jet eta (visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_phi_visited", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30 jet phi (visited)", 80, -4,
			4);

	histMan->addH1D_JetBinned("jet_pt_fired", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired", "HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30 jet phi (fired)", 80, -4, 4);
	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_DiCentralPFJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30");
	// ele tight ID + 3 jet
	histMan->addH1D_JetBinned("jet_pt_visited", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30 jet pt (visited)", 200, 0,
			200);
	histMan->addH1D_JetBinned("jet_eta_visited", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30 jet eta (visited)", 80,
			-4, 4);
	histMan->addH1D_JetBinned("jet_phi_visited", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30 jet phi (visited)", 80,
			-4, 4);

	histMan->addH1D_JetBinned("jet_pt_fired", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30 jet pt (fired)", 200, 0,
			200);
	histMan->addH1D_JetBinned("jet_eta_fired", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30 jet eta (fired)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_phi_fired", "HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30 jet phi (fired)", 80, -4,
			4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30");
	// ele tight ID + 4 jet
	histMan->addH1D_JetBinned("jet_pt_visited", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30 jet pt (visited)", 200, 0,
			200);
	histMan->addH1D_JetBinned("jet_eta_visited", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30 jet eta (visited)", 80,
			-4, 4);
	histMan->addH1D_JetBinned("jet_phi_visited", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30 jet phi (visited)", 80,
			-4, 4);

	histMan->addH1D_JetBinned("jet_pt_fired", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30 jet pt (fired)", 200, 0,
			200);
	histMan->addH1D_JetBinned("jet_eta_fired", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30 jet eta (fired)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_phi_fired", "HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30 jet phi (fired)", 80, -4,
			4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30");
	// ele tight ID + 1 jet
	histMan->addH1D_JetBinned("jet_pt_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D_JetBinned("jet_pt_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30 jet phi (fired)", 80, -4, 4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30");
	// ele tight ID + 2 jet
	histMan->addH1D_JetBinned("jet_pt_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D_JetBinned("jet_pt_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30 jet phi (fired)", 80, -4, 4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralPFJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30");
	// ele tight ID + 3 jet
	histMan->addH1D_JetBinned("jet_pt_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D_JetBinned("jet_pt_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30 jet phi (fired)", 80, -4, 4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4, 4);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30");
	// ele tight ID + 4 jet
	histMan->addH1D_JetBinned("jet_pt_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30 jet pt (visited)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30 jet eta (visited)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30 jet phi (visited)", 80, -4, 4);

	histMan->addH1D_JetBinned("jet_pt_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30 jet pt (fired)", 200, 0, 200);
	histMan->addH1D_JetBinned("jet_eta_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30 jet eta (fired)", 80, -4, 4);
	histMan->addH1D_JetBinned("jet_phi_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30 jet phi (fired)", 80, -4, 4);

	//plateau region
	histMan->addH1D_JetBinned("jet_eta_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30 jet eta (jet pT > 45 GeV, visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_visited",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30 jet phi (jet pT > 45 GeV, visited)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_eta_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30 jet eta (jet pT > 45 GeV, fired)", 80, -4,
			4);
	histMan->addH1D_JetBinned("jet_phi_PtGT45_fired",
			"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30 jet phi (jet pT > 45 GeV, fired)", 80, -4,
			4);

	//signal trigger efficiencies
	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30/TriggerEfficiency");
	histMan->addH1D("Ele30_TriPFJet30", "Signal trigger effiency for Ele30 + 3x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_QuadPFJet30", "Signal trigger effiency for Ele30 + 4x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30", "Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30", 2,
			-0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30_PFJet30",
			"Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30_PFJet30", 2, -0.5, 1.5);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30/TriggerEfficiency");
	histMan->addH1D("Ele30_TriPFJet30", "Signal trigger effiency for Ele30 + 3x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_QuadPFJet30", "Signal trigger effiency for Ele30 + 4x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30", "Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30", 2,
			-0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30_PFJet30",
			"Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30_PFJet30", 2, -0.5, 1.5);

	histMan->setCurrentCollection(
			"HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30/TriggerEfficiency");
	histMan->addH1D("Ele30_TriPFJet30", "Signal trigger effiency for Ele30 + 3x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_QuadPFJet30", "Signal trigger effiency for Ele30 + 4x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30", "Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30", 2,
			-0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30_PFJet30",
			"Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30_PFJet30", 2, -0.5, 1.5);

	histMan->setCurrentCollection(
			"HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30/TriggerEfficiency");

	histMan->addH1D("Ele30_TriPFJet30", "Signal trigger effiency for Ele30 + 3x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_QuadPFJet30", "Signal trigger effiency for Ele30 + 4x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30", "Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30", 2,
			-0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30_PFJet30",
			"Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30_PFJet30", 2, -0.5, 1.5);

	//signal PF trigger efficiencies
	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30/TriggerEfficiency");
	histMan->addH1D("Ele30_TriPFJet30", "Signal trigger effiency for Ele30 + 3x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_QuadPFJet30", "Signal trigger effiency for Ele30 + 4x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30", "Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30", 2,
			-0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30_PFJet30",
			"Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30_PFJet30", 2, -0.5, 1.5);

	histMan->setCurrentCollection("HLTStudy/HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralPFJet30/TriggerEfficiency");
	histMan->addH1D("Ele30_TriPFJet30", "Signal trigger effiency for Ele30 + 3x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_QuadPFJet30", "Signal trigger effiency for Ele30 + 4x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30", "Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30", 2,
			-0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30_PFJet30",
			"Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30_PFJet30", 2, -0.5, 1.5);

	histMan->setCurrentCollection(
			"HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30/TriggerEfficiency");
	histMan->addH1D("Ele30_TriPFJet30", "Signal trigger effiency for Ele30 + 3x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_QuadPFJet30", "Signal trigger effiency for Ele30 + 4x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30", "Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30", 2,
			-0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30_PFJet30",
			"Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30_PFJet30", 2, -0.5, 1.5);

	histMan->setCurrentCollection(
			"HLTStudy/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30/TriggerEfficiency");

	histMan->addH1D("Ele30_TriPFJet30", "Signal trigger effiency for Ele30 + 3x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_QuadPFJet30", "Signal trigger effiency for Ele30 + 4x PFJet30", 2, -0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30", "Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30", 2,
			-0.5, 1.5);
	histMan->addH1D("Ele30_PFJet70_PFJet50_PFJet30_PFJet30",
			"Signal trigger effiency for Ele30_PFJet70_PFJet50_PFJet30_PFJet30", 2, -0.5, 1.5);
}

}
