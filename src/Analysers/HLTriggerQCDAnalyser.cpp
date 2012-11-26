/*
 * HLTriggerQCDAnalyser.cpp
 *
 *  Created on: 25 May 2012
 *      Author: kreczko
 */

#include "../../interface/Analysers/HLTriggerQCDAnalyser.h"
#include <iostream>

using namespace std;

namespace BAT {
void HLTriggerQCDAnalyser::analyse(const EventPtr event) {
	unsigned long run(event->runnumber());
	//only do this analysis for runs above 193834 as previous runs don't have all triggers
	//after run 194225 the trigger has been changed to HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_30_20
	if (!(run >= 193834 && run <= 194225 && event->isRealData()))
		return;
//	if (!(run >= 194270 && event->isRealData()))
//		return;

	if (event->Electrons().size() == 0)
		return;

	const ElectronPointer mostEnergeticElectron(event->Electrons().front());

	bool passCaloIdVT_CaloIsoT_TrkIdT_TrkIsoT = event->HLT(
			HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30)
			|| event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_30_20);

	bool passCaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT = event->HLT(
			HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30);

	bool passCaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT = event->HLT(
			HLTriggers::HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30);

	bool passCaloIdVT_TrkIdT = event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30)
			|| event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30_30_20);

	if (passCaloIdVT_CaloIsoT_TrkIdT_TrkIsoT
			&& (!studyExclusiveEvents_
					|| (!passCaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT && !passCaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT
							&& !passCaloIdVT_TrkIdT))) {
		int prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30);
		if (run >= 194270)
		prescale = event->HLTPrescale(
				HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_30_20);

		eleAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->setPrescale(prescale);
		metNonIsoRegionAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->setPrescale(prescale);
		metAntiIDRegionAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->setPrescale(prescale);
		if (passesTriggerAnalysisSelection(event)) {
			eleAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->analyse(event);
			eleAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->analyseElectron(mostEnergeticElectron, event->weight());
		}

		if (passesNonIsoWithoutBtagAndHLT(event)) {
			QCDNonIsoRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_++;
			metNonIsoRegionAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->analyse(event, qcdNonIsoSelection_->signalLepton(event));
//			metNonIsoRegionAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->analyseTransverseMass(event,
//					qcdNonIsoSelection_->signalLepton(event));
		}

		if (passesAntiIDWithoutBtagAndHLT(event)) {
			QCDAntiIDRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_++;
			metAntiIDRegionAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->analyse(event,
					qcdAntiIDSelection_->signalLepton(event));
//			metAntiIDRegionAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->analyseTransverseMass(event,
//					qcdAntiIDSelection_->signalLepton(event));
		}
		if (passesSignalSelectionWithoutBtagAndHLT(event)) {
			TopSignalRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_++;
			if (topSignalSelection_->hasAtLeastTwoGoodBJets(event))
				TopSignal_TwoBtagsRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_++;
		}
	}

	if (passCaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT
			&& (!studyExclusiveEvents_
					|| (!passCaloIdVT_CaloIsoT_TrkIdT_TrkIsoT && !passCaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT
							&& !passCaloIdVT_TrkIdT))) {
		int prescale = event->HLTPrescale(
				(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30));
		eleAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->setPrescale(prescale);
		metNonIsoRegionAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->setPrescale(prescale);
		metAntiIDRegionAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->setPrescale(prescale);

		if (passesTriggerAnalysisSelection(event)) {
			eleAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->analyse(event);
			eleAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->analyseElectron(mostEnergeticElectron, event->weight());
		}

		if (passesNonIsoWithoutBtagAndHLT(event)) {
			QCDNonIsoRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_++;
			metNonIsoRegionAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->analyse(event,
					qcdNonIsoSelection_->signalLepton(event));
//			metNonIsoRegionAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->analyseTransverseMass(event,
//					qcdNonIsoSelection_->signalLepton(event));
		}
		if (passesAntiIDWithoutBtagAndHLT(event)) {
			QCDAntiIDRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_++;
			metAntiIDRegionAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->analyse(event,
					qcdAntiIDSelection_->signalLepton(event));
//			metAntiIDRegionAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->analyseTransverseMass(event,
//					qcdAntiIDSelection_->signalLepton(event));
		}

		if (passesSignalSelectionWithoutBtagAndHLT(event)) {
			TopSignalRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_++;
			if (topSignalSelection_->hasAtLeastTwoGoodBJets(event))
				TopSignal_TwoBtagsRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_++;
		}

	}

	if (passCaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT
			&& (!studyExclusiveEvents_
					|| (passCaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT && !passCaloIdVT_CaloIsoT_TrkIdT_TrkIsoT
							&& !passCaloIdVT_TrkIdT))) {
		int prescale = event->HLTPrescale(
				(HLTriggers::HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30));
		eleAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->setPrescale(prescale);
		metNonIsoRegionAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->setPrescale(prescale);
		metAntiIDRegionAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->setPrescale(prescale);
		if (passesTriggerAnalysisSelection(event)) {
			eleAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->analyse(event);
			eleAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->analyseElectron(mostEnergeticElectron, event->weight());
		}

		if (passesNonIsoWithoutBtagAndHLT(event)) {
			QCDNonIsoRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_++;
			metNonIsoRegionAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->analyse(event,
					qcdNonIsoSelection_->signalLepton(event));
//			metNonIsoRegionAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->analyseTransverseMass(event,
//					qcdNonIsoSelection_->signalLepton(event));
		}
		if (passesAntiIDWithoutBtagAndHLT(event)) {
			QCDAntiIDRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_++;
			metAntiIDRegionAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->analyse(event,
					qcdAntiIDSelection_->signalLepton(event));
//			metAntiIDRegionAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->analyseTransverseMass(event,
//					qcdAntiIDSelection_->signalLepton(event));
		}
		if (passesSignalSelectionWithoutBtagAndHLT(event)) {
			TopSignalRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_++;
			if (topSignalSelection_->hasAtLeastTwoGoodBJets(event))
				TopSignal_TwoBtagsRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_++;
		}
	}

	if (passCaloIdVT_TrkIdT
			&& (!studyExclusiveEvents_
					|| (!passCaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT && !passCaloIdVT_CaloIsoT_TrkIdT_TrkIsoT
							&& !passCaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT))) {
		int prescale = event->HLTPrescale((HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30));
		if (run >= 194270)
			prescale = event->HLTPrescale((HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30_30_20));
		eleAnalyser_CaloIdVT_TrkIdT_->setPrescale(prescale);
		metNonIsoRegionAnalyser_CaloIdVT_TrkIdT_->setPrescale(prescale);
		metAntiIDRegionAnalyser_CaloIdVT_TrkIdT_->setPrescale(prescale);

		if (passesTriggerAnalysisSelection(event)) {
			eleAnalyser_CaloIdVT_TrkIdT_->analyse(event);
			eleAnalyser_CaloIdVT_TrkIdT_->analyseElectron(mostEnergeticElectron, event->weight());
		}
		if (passesNonIsoWithoutBtagAndHLT(event)) {
			QCDNonIsoRegionCount_CaloIdVT_TrkIdT_++;
			metNonIsoRegionAnalyser_CaloIdVT_TrkIdT_->analyse(event,
					qcdNonIsoSelection_->signalLepton(event));
//			metNonIsoRegionAnalyser_CaloIdVT_TrkIdT_->analyseTransverseMass(event,
//					qcdNonIsoSelection_->signalLepton(event));
		}
		if (passesAntiIDWithoutBtagAndHLT(event)) {
			QCDAntiIDRegionCount_CaloIdVT_TrkIdT_++;
			metAntiIDRegionAnalyser_CaloIdVT_TrkIdT_->analyse(event,
					qcdAntiIDSelection_->signalLepton(event));
//			metAntiIDRegionAnalyser_CaloIdVT_TrkIdT_->analyseTransverseMass(event,
//					qcdAntiIDSelection_->signalLepton(event));
		}

		if (passesSignalSelectionWithoutBtagAndHLT(event)) {
			TopSignalRegionCount_CaloIdVT_TrkIdT_++;
			if (topSignalSelection_->hasAtLeastTwoGoodBJets(event))
				TopSignal_TwoBtagsRegionCount_CaloIdVT_TrkIdT_++;
		}
	}

}

void HLTriggerQCDAnalyser::createHistograms() {
	eleAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->createHistograms();
	eleAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->createHistograms();
	eleAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->createHistograms();
	eleAnalyser_CaloIdVT_TrkIdT_->createHistograms();
	metNonIsoRegionAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->createHistograms();
	metNonIsoRegionAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->createHistograms();
	metNonIsoRegionAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->createHistograms();
	metNonIsoRegionAnalyser_CaloIdVT_TrkIdT_->createHistograms();
	metAntiIDRegionAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_->createHistograms();
	metAntiIDRegionAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_->createHistograms();
	metAntiIDRegionAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_->createHistograms();
	metAntiIDRegionAnalyser_CaloIdVT_TrkIdT_->createHistograms();
}

HLTriggerQCDAnalyser::HLTriggerQCDAnalyser(HistogramManagerPtr histMan, std::string histogramFolder,
		bool studyExclusiveEvents) :
		BasicAnalyser(histMan, histogramFolder), //
		studyExclusiveEvents_(studyExclusiveEvents), eleAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30")), //
		eleAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30")), //
		eleAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30")), //
		eleAnalyser_CaloIdVT_TrkIdT_(
				new ElectronAnalyser(histMan, histogramFolder + "/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30")), //
		metNonIsoRegionAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_(
				new METAnalyser(histMan,
						histogramFolder
								+ "/QCDNonIso/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30")), //
		metNonIsoRegionAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_(
				new METAnalyser(histMan,
						histogramFolder
								+ "/QCDNonIso/HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30")), //
		metNonIsoRegionAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_(
				new METAnalyser(histMan,
						histogramFolder
								+ "/QCDNonIso/HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30")), //
		metNonIsoRegionAnalyser_CaloIdVT_TrkIdT_(
				new METAnalyser(histMan,
						histogramFolder + "/QCDNonIso/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30")), //
		metAntiIDRegionAnalyser_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_(
				new METAnalyser(histMan,
						histogramFolder
								+ "/QCDAntiID/HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30")), //
		metAntiIDRegionAnalyser_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_(
				new METAnalyser(histMan,
						histogramFolder
								+ "/QCDAntiID/HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30")), //
		metAntiIDRegionAnalyser_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_(
				new METAnalyser(histMan,
						histogramFolder
								+ "/QCDAntiID/HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30")), //
		metAntiIDRegionAnalyser_CaloIdVT_TrkIdT_(
				new METAnalyser(histMan,
						histogramFolder + "/QCDAntiID/HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30")), //
		qcdNonIsoSelection_(new QCDNonIsolatedElectronSelection()), //
		QCDNonIsoRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_(0), //
		QCDNonIsoRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_(0), //
		QCDNonIsoRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_(0), //
		QCDNonIsoRegionCount_CaloIdVT_TrkIdT_(0), //
		qcdAntiIDSelection_(new QCDAntiIDEPlusJetsSelection()), //
		QCDAntiIDRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_(0), //
		QCDAntiIDRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_(0), //
		QCDAntiIDRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_(0), //
		QCDAntiIDRegionCount_CaloIdVT_TrkIdT_(0), //
		topSignalSelection_(new TopPairEPlusJetsReferenceSelection()), //
		TopSignalRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_(0), //
		TopSignalRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_(0), //
		TopSignalRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_(0), //
		TopSignalRegionCount_CaloIdVT_TrkIdT_(0), //
		TopSignal_TwoBtagsRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_(0), //
		TopSignal_TwoBtagsRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_(0), //
		TopSignal_TwoBtagsRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_(0), //
		TopSignal_TwoBtagsRegionCount_CaloIdVT_TrkIdT_(0) {
	qcdAntiIDSelection_->useMVAID(false);
}

HLTriggerQCDAnalyser::~HLTriggerQCDAnalyser() {
//	cout << "QCD non-iso selection" << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30: "
//			<< QCDNonIsoRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_ << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30: "
//			<< QCDNonIsoRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_ << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30: "
//			<< QCDNonIsoRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_ << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30: " << QCDNonIsoRegionCount_CaloIdVT_TrkIdT_
//			<< endl;
//
//	cout << "QCD anti-id selection" << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30: "
//			<< QCDAntiIDRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_ << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30: "
//			<< QCDAntiIDRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_ << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30: "
//			<< QCDAntiIDRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_ << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30: " << QCDAntiIDRegionCount_CaloIdVT_TrkIdT_
//			<< endl;
//
//	cout << "Top signal selection" << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30: "
//			<< TopSignalRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_ << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30: "
//			<< TopSignalRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_ << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30: "
//			<< TopSignalRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_ << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30: " << TopSignalRegionCount_CaloIdVT_TrkIdT_
//			<< endl;
//
//	cout << "Top signal selection (>= 2 b-tags)" << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30: "
//			<< TopSignal_TwoBtagsRegionCount_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_ << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30: "
//			<< TopSignal_TwoBtagsRegionCount_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_ << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30: "
//			<< TopSignal_TwoBtagsRegionCount_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_ << endl;
//	cout << "Events passing HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30: "
//			<< TopSignal_TwoBtagsRegionCount_CaloIdVT_TrkIdT_ << endl;
}

bool HLTriggerQCDAnalyser::passesNonIsoWithoutBtagAndHLT(const EventPtr event) const {
	bool passes(false);
	passes = qcdNonIsoSelection_->passesEventCleaning(event);
	passes = passes && qcdNonIsoSelection_->hasExactlyOneIsolatedLepton(event);
	passes = passes && qcdNonIsoSelection_->passesLooseLeptonVeto(event);
	passes = passes && qcdNonIsoSelection_->passesDileptonVeto(event);
	passes = passes && qcdNonIsoSelection_->passesConversionVeto(event);
	passes = passes && qcdNonIsoSelection_->hasAtLeastNGoodJets(event,3);
	passes = passes && qcdNonIsoSelection_->hasAtLeastNGoodJets(event,4);

	return passes;
}

bool HLTriggerQCDAnalyser::passesAntiIDWithoutBtagAndHLT(const EventPtr event) const {
	bool passes(false);
	passes = qcdAntiIDSelection_->passesEventCleaning(event);
	passes = passes && qcdAntiIDSelection_->hasExactlyOneIsolatedLepton(event);
	passes = passes && qcdAntiIDSelection_->passesLooseLeptonVeto(event);
	passes = passes && qcdAntiIDSelection_->passesDileptonVeto(event);
	passes = passes && qcdAntiIDSelection_->passesConversionVeto(event);
	passes = passes && qcdAntiIDSelection_->hasAtLeastNGoodJets(event, 3);
	passes = passes && qcdAntiIDSelection_->hasAtLeastNGoodJets(event, 4);

	return passes;
}

bool HLTriggerQCDAnalyser::passesSignalSelectionWithoutBtagAndHLT(const EventPtr event) const {
	bool passes(false);
	passes = topSignalSelection_->passesEventCleaning(event);
	passes = passes && topSignalSelection_->hasExactlyOneIsolatedLepton(event);
	passes = passes && topSignalSelection_->passesLooseLeptonVeto(event);
	passes = passes && topSignalSelection_->passesDileptonVeto(event);
	passes = passes && topSignalSelection_->passesConversionVeto(event);
	passes = passes && topSignalSelection_->hasAtLeastNGoodJets(event, 3);
	passes = passes && topSignalSelection_->hasAtLeastNGoodJets(event, 4);

	return passes;
}

bool HLTriggerQCDAnalyser::passesTriggerAnalysisSelection(const EventPtr event) const {
	const ElectronCollection electrons(event->Electrons());
	const JetCollection jets(event->Jets());
	if (electrons.size() == 0 || jets.size() < 3)
		return false;

	unsigned int nElectrons(0);
	for (unsigned int index = 0; index < electrons.size(); ++index) {
		const ElectronPointer electron(electrons.at(index));
		if (fabs(electron->eta()) < 2.5 && electron->pt() > 20)
			++nElectrons;
		//if more than 2 electrons passing the selection of > 20GeV, reject event
	}
	const ElectronPointer mostEnergeticElectron(electrons.front());
	//clean jets against electron
	JetCollection cleanedJets;

	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet(jets.at(index));
		if (!jet->isWithinDeltaR(0.3, mostEnergeticElectron))
			cleanedJets.push_back(jet);
	}

	unsigned int nCleanedJetsAbove30GeV(0), nCleanedJetsAbove45GeV(0);
	for (unsigned int index = 0; index < cleanedJets.size(); ++index) {
		const JetPointer jet(cleanedJets.at(index));
		if (jet->pt() > 45.)
			++nCleanedJetsAbove45GeV;
		if (jet->pt() > 30.)
			++nCleanedJetsAbove30GeV;
	}

	return nElectrons == 1
			&& (nCleanedJetsAbove45GeV >= 3
					|| (nCleanedJetsAbove45GeV >= 2 && nCleanedJetsAbove30GeV >= 3 && event->runnumber() >= 194270));
}

} /* namespace BAT */
