/*
 * TTbarPlusMETAnalyser.cpp
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Analysers/TTbarPlusMETAnalyser.h"
//#include "../../interface/TopPairEventCandidate.h"
//signal selections
#include "../../interface/Selections/TopPairEplusJetsRefAsymJetsSelection.h"
#include "../../interface/Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../../interface/Selections/TopPairEPlusJetsRefAsymJetsMETSelection.h"
//QCD selections w.r.t signal ref selection
#include "../../interface/Selections/QCDNonIsolatedElectronSelection.h"
#include "../../interface/Selections/QCDConversionsSelection.h"
#include "../../interface/Selections/QCDPFRelIsoEPlusJetsSelection.h"
//QCD selections w.r.t signal ref selection + asymmetric jets
#include "../../interface/Selections/QCDNonIsolatedElectronAsymJetsSelection.h"
#include "../../interface/Selections/QCDConversionsAsymJetsSelection.h"
#include "../../interface/Selections/QCDPFRelIsoEPlusAsymJetsSelection.h"
//QCD selections w.r.t signal ref selection + asymmetric jets + MET
#include "../../interface/Selections/QCDNonIsolatedElectronAsymJetsMETSelection.h"
#include "../../interface/Selections/QCDConversionsAsymJetsMETSelection.h"
#include "../../interface/Selections/QCDPFRelIsoEPlusAsymJetsMET.h"

namespace BAT {

void TTbarPlusMETAnalyser::analyse(const EventPtr event) {
	signalAnalysis(event);
	qcdAnalysis(event);
	qcdAnalysisAsymJets(event);
	qcdAnalysisAsymJetsMET(event);
}

void TTbarPlusMETAnalyser::signalAnalysis(const EventPtr event) {

	if (topEplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const LeptonPointer signalLepton = topEplusJetsRefSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		metAnalyserRefSelection_->analyse(event);
		metAnalyserRefSelection_->analyseTransverseMass(event->MET(), signalLepton, event->weight());

		electronAnalyserRefSelection_->analyse(event);
		electronAnalyserRefSelection_->analyseElectron(signalElectron, event->weight());
	}

	if (topEplusAsymJetsMETSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const LeptonPointer signalLepton = topEplusAsymJetsMETSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		metAnalyserRefAsymJetsMETSelection_->analyse(event);
		metAnalyserRefAsymJetsMETSelection_->analyseTransverseMass(event->MET(), signalLepton, event->weight());

		electronAnalyserRefAsymJetsMETSelection_->analyse(event);
		electronAnalyserRefAsymJetsMETSelection_->analyseElectron(signalElectron, event->weight());
	}

	if (topEplusAsymJetsSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const LeptonPointer signalLepton = topEplusAsymJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		metAnalyserRefAsymJetsSelection_->analyse(event);
		metAnalyserRefAsymJetsSelection_->analyseTransverseMass(event->MET(), signalLepton, event->weight());

		electronAnalyserRefAsymJetsSelection_->analyse(event);
		electronAnalyserRefAsymJetsSelection_->analyseElectron(signalElectron, event->weight());
	}

}

void TTbarPlusMETAnalyser::qcdAnalysis(const EventPtr event) {
	//selection with respect to reference selection
	if (qcdNonIsoElectronSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoSelection_->setPrescale(prescale);

		qcdNonIsoElectronAnalyser_->analyse(event);
		qcdNonIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoSelection_->analyse(event);
		metAnalyserqcdNonIsoSelection_->analyseTransverseMass(event->MET(), signalLepton, event->weight());
	}

	if (qcdNonIsoElectronNonIsoTriggerSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronNonIsoTriggerSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronNonIsoTriggerSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoNonIsoTriggerElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdNonIsoNonIsoTriggerSelection_->setPrescale(prescale);

		qcdNonIsoNonIsoTriggerElectronAnalyser_->analyse(event);
		qcdNonIsoNonIsoTriggerElectronAnalyser_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoNonIsoTriggerSelection_->analyse(event);
		metAnalyserqcdNonIsoNonIsoTriggerSelection_->analyseTransverseMass(event->MET(), signalLepton, event->weight());
	}

	if (qcdConversionSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		unsigned int prescale(qcdConversionSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdConversionSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdConversionsElectronAnalyser_->setPrescale(prescale);
		metAnalyserqcdConversionSelection_->setPrescale(prescale);

		qcdConversionsElectronAnalyser_->analyse(event);
		qcdConversionsElectronAnalyser_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdConversionSelection_->analyse(event);
		metAnalyserqcdConversionSelection_->analyseTransverseMass(event->MET(), signalLepton, event->weight());
	}

	if (qcdPFRelIsoSelection_->passesSelectionUpToStep(event, TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		unsigned int prescale(qcdPFRelIsoSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoElectronAnalyser_->setPrescale(prescale);

		qcdEPlusjetsPFRelIsoElectronAnalyser_->analyse(event);
		qcdEPlusjetsPFRelIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());
	}

	if (qcdPFRelIsoNonIsoTriggerSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets)) {
		unsigned int prescale(qcdPFRelIsoNonIsoTriggerSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoNonIsoTriggerSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->setPrescale(prescale);

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->analyse(event);
		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->analyseElectron(signalElectron, event->weight());
	}

	//selection with respect to reference selection + asymmetric jet cuts

}

void TTbarPlusMETAnalyser::qcdAnalysisAsymJets(const EventPtr event) {
	//selection with respect to reference selection + asym. jet cuts
	if (qcdNonIsoElectronAsymJetsSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronAsymJetsSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronAsymJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoElectronAnalyserAsymJets_->setPrescale(prescale);
		metAnalyserqcdNonIsoAsymJetsSelection_->setPrescale(prescale);

		qcdNonIsoElectronAnalyserAsymJets_->analyse(event);
		qcdNonIsoElectronAnalyserAsymJets_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoAsymJetsSelection_->analyse(event);
		metAnalyserqcdNonIsoAsymJetsSelection_->analyseTransverseMass(event->MET(), signalLepton, event->weight());
	}

	if (qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJets_->setPrescale(prescale);
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsSelection_->setPrescale(prescale);

		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJets_->analyse(event);
		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJets_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsSelection_->analyse(event);
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsSelection_->analyseTransverseMass(event->MET(), signalLepton,
				event->weight());
	}

	if (qcdConversionAsymJetsSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		unsigned int prescale(qcdConversionAsymJetsSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdConversionAsymJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdConversionsElectronAnalyserAsymJets_->setPrescale(prescale);
		metAnalyserqcdConversionAsymJetsSelection_->setPrescale(prescale);

		qcdConversionsElectronAnalyserAsymJets_->analyse(event);
		qcdConversionsElectronAnalyserAsymJets_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdConversionAsymJetsSelection_->analyse(event);
		metAnalyserqcdConversionAsymJetsSelection_->analyseTransverseMass(event->MET(), signalLepton, event->weight());
	}

	if (qcdPFRelIsoAsymJetsSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		unsigned int prescale(qcdPFRelIsoAsymJetsSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoAsymJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJets_->setPrescale(prescale);

		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJets_->analyse(event);
		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJets_->analyseElectron(signalElectron, event->weight());
	}

	if (qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		unsigned int prescale(qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJets_->setPrescale(prescale);

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJets_->analyse(event);
		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJets_->analyseElectron(signalElectron, event->weight());
	}

}

void TTbarPlusMETAnalyser::qcdAnalysisAsymJetsMET(const EventPtr event) {
	//selection with respect to reference selection + asym. jet cuts
	if (qcdNonIsoElectronAsymJetsMETSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronAsymJetsMETSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronAsymJetsMETSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoElectronAnalyserAsymJetsMET_->setPrescale(prescale);
		metAnalyserqcdNonIsoAsymJetsMETSelection_->setPrescale(prescale);

		qcdNonIsoElectronAnalyserAsymJetsMET_->analyse(event);
		qcdNonIsoElectronAnalyserAsymJetsMET_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoAsymJetsMETSelection_->analyse(event);
		metAnalyserqcdNonIsoAsymJetsMETSelection_->analyseTransverseMass(event->MET(), signalLepton, event->weight());
	}

	if (qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		//in case of prescaled triggers
		unsigned int prescale(qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->setPrescale(prescale);
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsMETSelection_->setPrescale(prescale);

		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->analyse(event);
		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsMETSelection_->analyse(event);
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsMETSelection_->analyseTransverseMass(event->MET(), signalLepton,
				event->weight());
	}

	if (qcdConversionAsymJetsMETSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		unsigned int prescale(qcdConversionAsymJetsMETSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdConversionAsymJetsMETSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdConversionsElectronAnalyserAsymJetsMET_->setPrescale(prescale);
		metAnalyserqcdConversionAsymJetsMETSelection_->setPrescale(prescale);

		qcdConversionsElectronAnalyserAsymJetsMET_->analyse(event);
		qcdConversionsElectronAnalyserAsymJetsMET_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdConversionAsymJetsMETSelection_->analyse(event);
		metAnalyserqcdConversionAsymJetsMETSelection_->analyseTransverseMass(event->MET(), signalLepton,
				event->weight());
	}

	if (qcdPFRelIsoAsymJetsMETSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		unsigned int prescale(qcdPFRelIsoAsymJetsMETSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoAsymJetsMETSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJetsMET_->setPrescale(prescale);

		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJetsMET_->analyse(event);
		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJetsMET_->analyseElectron(signalElectron, event->weight());
	}

	if (qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		unsigned int prescale(qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->prescale(event));
		const LeptonPointer signalLepton = qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->setPrescale(prescale);

		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->analyse(event);
		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->analyseElectron(signalElectron, event->weight());
	}

}

void TTbarPlusMETAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	//signal
	metAnalyserRefSelection_->createHistograms();
	metAnalyserRefAsymJetsMETSelection_->createHistograms();
	metAnalyserRefAsymJetsSelection_->createHistograms();
	electronAnalyserRefSelection_->createHistograms();
	electronAnalyserRefAsymJetsSelection_->createHistograms();
	electronAnalyserRefAsymJetsMETSelection_->createHistograms();
	//QCD region
	qcdNonIsoElectronAnalyser_->createHistograms();
	qcdNonIsoNonIsoTriggerElectronAnalyser_->createHistograms();
	metAnalyserqcdNonIsoSelection_->createHistograms();
	metAnalyserqcdNonIsoNonIsoTriggerSelection_->createHistograms();

	qcdConversionsElectronAnalyser_->createHistograms();
	metAnalyserqcdConversionSelection_->createHistograms();

	qcdEPlusjetsPFRelIsoElectronAnalyser_->createHistograms();
	qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_->createHistograms();
	//QCD region + asym. jet cuts
	qcdNonIsoElectronAnalyserAsymJets_->createHistograms();
	qcdNonIsoNonIsoTriggerElectronAnalyserAsymJets_->createHistograms();
	metAnalyserqcdNonIsoAsymJetsSelection_->createHistograms();
	metAnalyserqcdNonIsoNonIsoTriggerAsymJetsSelection_->createHistograms();

	qcdConversionsElectronAnalyserAsymJets_->createHistograms();
	metAnalyserqcdConversionAsymJetsSelection_->createHistograms();

	qcdEPlusjetsPFRelIsoElectronAnalyserAsymJets_->createHistograms();
	qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJets_->createHistograms();
	//QCD region + asym. jet cuts
	qcdNonIsoElectronAnalyserAsymJetsMET_->createHistograms();
	qcdNonIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->createHistograms();
	metAnalyserqcdNonIsoAsymJetsMETSelection_->createHistograms();
	metAnalyserqcdNonIsoNonIsoTriggerAsymJetsMETSelection_->createHistograms();

	qcdConversionsElectronAnalyserAsymJetsMET_->createHistograms();
	metAnalyserqcdConversionAsymJetsMETSelection_->createHistograms();

	qcdEPlusjetsPFRelIsoElectronAnalyserAsymJetsMET_->createHistograms();
	qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJetsMET_->createHistograms();

}

TTbarPlusMETAnalyser::TTbarPlusMETAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		topEplusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection()), //
		topEplusAsymJetsSelection_(new TopPairEplusJetsRefAsymJetsSelection()), //
		topEplusAsymJetsMETSelection_(new TopPairEPlusJetsRefAsymJetsMETSelection()), //
//QCD selections with respect to reference selection
		qcdNonIsoElectronSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdNonIsoElectronNonIsoTriggerSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdConversionSelection_(new QCDConversionsSelection()), //
		qcdPFRelIsoSelection_(new QCDPFRelIsoEPlusJetsSelection()), //
		qcdPFRelIsoNonIsoTriggerSelection_(new QCDPFRelIsoEPlusJetsSelection()), //
//QCD selections with respect to reference selection + asymmetric jet cuts
		qcdNonIsoElectronAsymJetsSelection_(new QCDNonIsolatedElectronAsymJetsSelection()), //
		qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_(new QCDNonIsolatedElectronAsymJetsSelection()), //
		qcdConversionAsymJetsSelection_(new QCDConversionsAsymJetsSelection()), //
		qcdPFRelIsoAsymJetsSelection_(new QCDPFRelIsoEPlusAsymJetsSelection()), //
		qcdPFRelIsoNonIsoTriggerAsymJetsSelection_(new QCDPFRelIsoEPlusAsymJetsSelection()), //
//QCD selections with respect to reference selection + MET + asymmetric jet cuts
		qcdNonIsoElectronAsymJetsMETSelection_(new QCDNonIsolatedElectronAsymJetsMETSelection()), //
		qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_(new QCDNonIsolatedElectronAsymJetsMETSelection()), //
		qcdConversionAsymJetsMETSelection_(new QCDConversionsAsymJetsMETSelection()), //
		qcdPFRelIsoAsymJetsMETSelection_(new QCDPFRelIsoEPlusAsymJetsMET()), //
		qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_(new QCDPFRelIsoEPlusAsymJetsMET()), //
//analysers
//signal regions
		metAnalyserRefSelection_(new METAnalyser(histMan, histogramFolder + "/Ref selection/MET")), //
		metAnalyserRefAsymJetsMETSelection_(
				new METAnalyser(histMan, histogramFolder + "/Ref + AsymJets + MET selection/MET")), //
		metAnalyserRefAsymJetsSelection_(new METAnalyser(histMan, histogramFolder + "/Ref + AsymJets selection/MET")), //
		electronAnalyserRefSelection_(new ElectronAnalyser(histMan, histogramFolder + "/Ref selection/Electron")), //
		electronAnalyserRefAsymJetsMETSelection_(
				new ElectronAnalyser(histMan, histogramFolder + "/Ref + AsymJets + MET selection/Electron")), //
		electronAnalyserRefAsymJetsSelection_(
				new ElectronAnalyser(histMan, histogramFolder + "/Ref + AsymJets selection/Electron")), //
//QCD region
		metAnalyserqcdNonIsoSelection_(new METAnalyser(histMan, histogramFolder + "/Ref selection/QCD non iso e+jets/MET")), //
		metAnalyserqcdNonIsoNonIsoTriggerSelection_(
				new METAnalyser(histMan, histogramFolder + "/Ref selection/QCD non iso e+jets, non iso trigger/MET")), //
		qcdNonIsoElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/Ref selection/QCD non iso e+jets/Electron")), //
		qcdNonIsoNonIsoTriggerElectronAnalyser_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/Ref selection/QCD non iso e+jets, non iso trigger/Electron")), //
		metAnalyserqcdConversionSelection_(new METAnalyser(histMan, histogramFolder + "/Ref selection/QCDConversions/MET")), //
		qcdConversionsElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/Ref selection/QCDConversions/Electron")), //
		qcdEPlusjetsPFRelIsoElectronAnalyser_(
				new ElectronAnalyser(histMan, histogramFolder + "/Ref selection/QCD e+jets PFRelIso/Electron")), //
		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyser_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/Ref selection/QCD e+jets PFRelIso, non iso trigger/Electron")), //
//QCD region with asym. jets
		metAnalyserqcdNonIsoAsymJetsSelection_(
				new METAnalyser(histMan, histogramFolder + "/Ref + AsymJets selection/QCD non iso e+jets/MET")), //
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsSelection_(
				new METAnalyser(histMan,
						histogramFolder + "/Ref + AsymJets selection/QCD non iso e+jets, non iso trigger/MET")), //
		qcdNonIsoElectronAnalyserAsymJets_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/Ref + AsymJets selection/QCD non iso e+jets/Electron")), //
		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJets_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/Ref + AsymJets selection/QCD non iso e+jets, non iso trigger/Electron")), //
		metAnalyserqcdConversionAsymJetsSelection_(
				new METAnalyser(histMan, histogramFolder + "/Ref + AsymJets selection/QCDConversions/MET")), //
		qcdConversionsElectronAnalyserAsymJets_(
				new ElectronAnalyser(histMan, histogramFolder + "/Ref + AsymJets selection/QCDConversions/Electron")), //
		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJets_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/Ref + AsymJets selection/QCD e+jets PFRelIso/Electron")), //
		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJets_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/Ref + AsymJets selection/QCD e+jets PFRelIso, non iso trigger/Electron")), //
//QCD region with asym. jets + MET
		metAnalyserqcdNonIsoAsymJetsMETSelection_(
				new METAnalyser(histMan, histogramFolder + "/Ref + AsymJets + MET selection/QCD non iso e+jets/MET")), //
		metAnalyserqcdNonIsoNonIsoTriggerAsymJetsMETSelection_(
				new METAnalyser(histMan,
						histogramFolder + "/Ref + AsymJets + MET selection/QCD non iso e+jets, non iso trigger/MET")), //
		qcdNonIsoElectronAnalyserAsymJetsMET_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/Ref + AsymJets + MET selection/QCD non iso e+jets/Electron")), //
		qcdNonIsoNonIsoTriggerElectronAnalyserAsymJetsMET_(
				new ElectronAnalyser(histMan,
						histogramFolder
								+ "/Ref + AsymJets + MET selection/QCD non iso e+jets, non iso trigger/Electron")), //
		metAnalyserqcdConversionAsymJetsMETSelection_(
				new METAnalyser(histMan, histogramFolder + "/Ref + AsymJets + MET selection/QCDConversions/MET")), //
		qcdConversionsElectronAnalyserAsymJetsMET_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/Ref + AsymJets + MET selection/QCDConversions/Electron")), //
		qcdEPlusjetsPFRelIsoElectronAnalyserAsymJetsMET_(
				new ElectronAnalyser(histMan,
						histogramFolder + "/Ref + AsymJets + MET selection/QCD e+jets PFRelIso/Electron")), //
		qcdEPlusjetsPFRelIsoNonIsoTriggerElectronAnalyserAsymJetsMET_(
				new ElectronAnalyser(histMan,
						histogramFolder
								+ "/Ref + AsymJets + MET selection/QCD e+jets PFRelIso, non iso trigger/Electron")) {
	qcdNonIsoElectronNonIsoTriggerSelection_->useNonIsoTrigger(true);
	qcdPFRelIsoNonIsoTriggerSelection_->useNonIsoTrigger(true);
	qcdNonIsoElectronNonIsoTriggerAsymJetsSelection_->useNonIsoTrigger(true);
	qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->useNonIsoTrigger(true);
	qcdNonIsoElectronNonIsoTriggerAsymJetsMETSelection_->useNonIsoTrigger(true);
	qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->useNonIsoTrigger(true);

}

TTbarPlusMETAnalyser::~TTbarPlusMETAnalyser() {
}

} /* namespace BAT */
