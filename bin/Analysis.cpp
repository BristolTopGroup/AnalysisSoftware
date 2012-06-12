/*
 * Analysis.cpp
 *
 *  Created on: 12 Jul 2010
 *      Author: kreczko
 */

#include "Analysis.h"
#include "TROOT.h"
#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <boost/array.hpp>
#include "../interface/EventCounter.h"
#include <cmath>
#include <math.h>
#include "../interface/Printers/EventTablePrinter.h"
#include "../interface/ReconstructionModules/ChiSquaredBasedTopPairReconstruction.h"
#include "../interface/LumiReWeighting.h"
#include "../interface/GlobalVariables.h"

using namespace reweight;
using namespace BAT;
using namespace std;
//float Analysis::luminosity = 882.;

void Analysis::analyse() {
	createHistograms();
	cout << "detected samples:" << endl;
	for (unsigned int sample = 0; sample < DataType::NUMBER_OF_DATA_TYPES; ++sample) {
		if (eventReader->getSeenDatatypes()[sample])
			cout << DataType::names[sample] << endl;
	}
	while (eventReader->hasNextEvent()) {
		initiateEvent();
		printNumberOfProccessedEventsEvery(Globals::printEveryXEvents);
		inspectEvents();

//		selectionCommissioning_->analyse(currentEvent);

		abcdMethodAnalyser_->analyse(currentEvent);
		bjetAnalyser->analyse(currentEvent);
		diElectronAnalyser->analyse(currentEvent);
		electronAnalyser->analyse(currentEvent);
		eventcountAnalyser->analyse(currentEvent);

//		hltriggerAnalyser->analyse(currentEvent);
		hltriggerQCDAnalyserInclusive_->analyse(currentEvent);
		hltriggerQCDAnalyserExclusive_->analyse(currentEvent);
		jetAnalyser->analyse(currentEvent);
		if (currentEvent->getDataType() == DataType::TTJets) {
			mcAnalyser->analyse(currentEvent);
			const TtbarHypothesis& mcEvent = mcAnalyser->GetMCTTbarHypothesis();
			const TtbarHypothesisPointer mcEventPtr(new TtbarHypothesis(mcEvent));
			hitfitAnalyser->setMCTTbarHypothesis(mcEvent);
			neutrinoRecoAnalyser->setMCTTbarHypothesis(mcEventPtr);
		}
		//hitfit analyser has to be after mcAnalyser as it depends on it
		if (Globals::useHitFit) {
			hitfitAnalyser->analyse(currentEvent);
		}
//		metAnalyser->analyse(currentEvent);
		mttbarAnalyser->analyse(currentEvent);
		muonAnalyser->analyse(currentEvent);
//		mvAnalyser->analyse(currentEvent);
//		neutrinoRecoAnalyser->analyse(currentEvent);
//		qcdAnalyser->analyse(currentEvent);
		ttbarPlusMETAnalyser_->analyse(currentEvent);
		vertexAnalyser->analyse(currentEvent);

//TODO: move to TTbarAnalysis
//		doTTbarCutFlow();
//        if(currentEvent->getDataType() == DataType::DATA)
//            eventCheck[currentEvent->runnumber()].push_back(currentEvent->eventnumber());
//        checkForBrokenEvents();
	}
//    checkForDuplicatedEvents();
//    printInterestingEvents();
}

void Analysis::printNumberOfProccessedEventsEvery(unsigned long printEvery) {
	unsigned long eventIndex = eventReader->getNumberOfProccessedEvents();
	if (eventIndex % printEvery == 0 || eventIndex == 1) {
		cout << "Analysing event no " << eventIndex << ", sample: " << DataType::names[currentEvent->getDataType()]
				<< endl;
		cout << "File: " << eventReader->getCurrentFile() << endl;
	}

}

void Analysis::initiateEvent() {
	currentEvent = eventReader->getNextEvent();
//	ttbarCandidate = TopPairEventCandidatePtr(new TopPairEventCandidate(*currentEvent.get()));
	weight = weights->getWeight(currentEvent->getDataType());
	if (!currentEvent->isRealData()) {
		//TODO: fix this dirty little thing
		if (Globals::NTupleVersion >= 6)
			pileUpWeight = weights->reweightPileUp(currentEvent->getTrueNumberOfVertices().at(1));
		else
			pileUpWeight = 1.;
		weight *= pileUpWeight;
	}

	currentEvent->setEventWeight(weight);
	currentEvent->setPileUpWeight(pileUpWeight);
//	ttbarCandidate->setEventWeight(weight);
//	ttbarCandidate->setPileUpWeight(pileUpWeight);

	histMan->setCurrentDataType(currentEvent->getDataType());
	histMan->setCurrentJetBin(currentEvent->GoodElectronCleanedJets().size());
	histMan->setCurrentBJetBin(currentEvent->GoodElectronCleanedBJets().size());
}

void Analysis::inspectEvents() {
	std::vector<InterestingEvent> eventsToInspect;

	for (unsigned int index = 0; index < eventsToInspect.size(); ++index) {
		if ((currentEvent->runnumber() == eventsToInspect.at(index).candidate->runnumber()
				&& currentEvent->eventnumber() == eventsToInspect.at(index).candidate->eventnumber())) {
			cout << "file: " << eventReader->getCurrentFile() << endl;
			currentEvent->inspect();
		}
	}

}

//void Analysis::doSynchExercise() {
//	if (ttbarCandidate->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::ConversionFinder)) {
//		cout << ttbarCandidate->runnumber() << ":" << ttbarCandidate->eventnumber() << ":" << endl; //electron->et() << endl;
//		if (ttbarCandidate->eventnumber() == 450622) {
//			ttbarCandidate->inspect();
//		}
//	}
//}

//void Analysis::doTTbarCutFlow() {
//	for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
//		if (ttbarCandidate->passesEPlusJetsSelectionStep((TTbarEPlusJetsSelection::Step) cut)) {
//			++ePlusJetsSingleCuts[cut];
//			ePlusJetsSingleCutsPerFile[eventReader->getCurrentFile()][cut]++;
//		}
//
//		if (ttbarCandidate->passesEPlusJetsSelectionStepUpTo((TTbarEPlusJetsSelection::Step) cut)) {
//			ePlusJetsCutflow[cut] += 1;
//			ePlusJetsCutflowPerFile[eventReader->getCurrentFile()][cut]++;
//			unsigned int njet = ttbarCandidate->GoodElectronCleanedJets().size();
//			if (njet >= JetBin::NUMBER_OF_JET_BINS)
//				njet = JetBin::NUMBER_OF_JET_BINS - 1;
//			ePlusJetsCutflowPerSample.increase(ttbarCandidate->getDataType(), cut, njet, weight);
//		}
//	}
//
//	for (unsigned int cut = 0; cut < TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
//		if (ttbarCandidate->passesMuPlusJetsSelectionStep((TTbarMuPlusJetsSelection::Step) cut)) {
//			++muPlusJetsSingleCuts[cut];
////			ePlusJetsSingleCutsPerFile[eventReader->getCurrentFile()][cut]++;
//		}
//
//		if (ttbarCandidate->passesMuPlusJetsSelectionStepUpTo((TTbarMuPlusJetsSelection::Step) cut)) {
//			muPlusJetsCutFlow[cut] += 1;
////			ePlusJetsCutflowPerFile[eventReader->getCurrentFile()][cut]++;
//			unsigned int njet = ttbarCandidate->GoodMuonCleanedJets().size();
//			if (njet >= JetBin::NUMBER_OF_JET_BINS)
//				njet = JetBin::NUMBER_OF_JET_BINS - 1;
//			muPlusJetsCutflowPerSample.increase(ttbarCandidate->getDataType(), cut, njet, weight);
//		}
//	}
//}

void Analysis::printInterestingEvents() {
	cout << "Interesting events:" << endl;
	for (unsigned int index = 0; index < interestingEvents.size(); ++index) {
		interestingEvents.at(index).print();
	}
}

void Analysis::printSummary() {
	cout << "total number of processed events: " << eventReader->getNumberOfProccessedEvents() << endl;
	cout << "number of events without electrons: " << brokenEvents.size() << endl;
	cout << "number of events with too high pileup: " << weights->getNumberOfEventsWithTooHighPileUp() << endl;
}

void Analysis::createHistograms() {
	histMan->prepareForSeenDataTypes(eventReader->getSeenDatatypes());
	unsigned int numberOfHistograms(0), lastNumberOfHistograms(0);

	abcdMethodAnalyser_->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by abcdMethodAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

	bjetAnalyser->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by bjetAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

	diElectronAnalyser->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by diElectronAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

	electronAnalyser->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by electronAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

	eventcountAnalyser->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by eventcountAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

	hitfitAnalyser->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by hitfitAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

//	hltriggerAnalyser->createHistograms();
//	numberOfHistograms = histMan->size();
//	cout << "Number of histograms added by hltriggerAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
//	lastNumberOfHistograms = numberOfHistograms;

	hltriggerQCDAnalyserInclusive_->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by hltriggerQCDAnalyserInclusive_: "
			<< numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

	hltriggerQCDAnalyserExclusive_->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by hltriggerQCDAnalyserExclusive_: "
			<< numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

	jetAnalyser->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by jetAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

	mcAnalyser->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by mcAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

//	metAnalyser->createHistograms();
//	numberOfHistograms = histMan->size();
//	cout << "Number of histograms added by metAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
//	lastNumberOfHistograms = numberOfHistograms;

	mttbarAnalyser->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by mttbarAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

	muonAnalyser->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by muonAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

//	mvAnalyser->createHistograms();
//	numberOfHistograms = histMan->size();
//	cout << "Number of histograms added by mvAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
//	lastNumberOfHistograms = numberOfHistograms;

//	neutrinoRecoAnalyser->createHistograms();
//	numberOfHistograms = histMan->size();
//	cout << "Number of histograms added by neutrinoRecoAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
//	lastNumberOfHistograms = numberOfHistograms;

//	qcdAnalyser->createHistograms();
//	numberOfHistograms = histMan->size();
//	cout << "Number of histograms added by qcdAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
//	lastNumberOfHistograms = numberOfHistograms;

	ttbarPlusMETAnalyser_->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by ttbarPlusMETAnalyser: " << numberOfHistograms - lastNumberOfHistograms
			<< endl;
	lastNumberOfHistograms = numberOfHistograms;

	vertexAnalyser->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by vertexAnalyser: " << numberOfHistograms - lastNumberOfHistograms << endl;
	lastNumberOfHistograms = numberOfHistograms;

	cout << "Total number of histograms: " << histMan->size() << endl;
}

Analysis::Analysis(std::string datasetInfoFile) : //
		eventReader(new NTupleEventReader()), //
		currentEvent(), //
//		ttbarCandidate(), //
		histMan(new BAT::HistogramManager()), //
		ePlusJetsCutflow(), //
		ePlusJetsSingleCuts(), //
		ePlusJetsCutflowPerFile(), //
		ePlusJetsSingleCutsPerFile(), //
		muPlusJetsCutFlow(), //
		muPlusJetsSingleCuts(), //
		interestingEvents(), //
		brokenEvents(), //
		eventCheck(), //
		weights(new EventWeightProvider(datasetInfoFile)), //
		weight(0), //
		pileUpWeight(1), //
		ePlusJetsCutflowPerSample(DataType::NUMBER_OF_DATA_TYPES, TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS,
				JetBin::NUMBER_OF_JET_BINS), //
		muPlusJetsCutflowPerSample(DataType::NUMBER_OF_DATA_TYPES, TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS,
				JetBin::NUMBER_OF_JET_BINS), //
		abcdMethodAnalyser_(new ABCDMethodAnalyser(histMan)), //
		bjetAnalyser(new BJetAnalyser(histMan)), //
		diElectronAnalyser(new DiElectronAnalyser(histMan)), //
		electronAnalyser(new ElectronAnalyser(histMan)), //
		eventcountAnalyser(new EventCountAnalyser(histMan)), //
		hitfitAnalyser(new HitFitAnalyser(histMan)), //
		hltriggerAnalyser(new HLTriggerTurnOnAnalyser(histMan)), //
		hltriggerQCDAnalyserInclusive_(new HLTriggerQCDAnalyser(histMan, "HLTQCDAnalyser_inclusive", false)), //
		hltriggerQCDAnalyserExclusive_(new HLTriggerQCDAnalyser(histMan, "HLTQCDAnalyser_exclusive", true)), //
		jetAnalyser(new JetAnalyser(histMan)), //
		mcAnalyser(new MCAnalyser(histMan)), //
		metAnalyser(new METAnalyser(histMan)), //
		mttbarAnalyser(new MTtbarAnalyser(histMan)), //
		muonAnalyser(new MuonAnalyser(histMan)), //
		mvAnalyser(new MVAnalyser(histMan)), //
		neutrinoRecoAnalyser(new NeutrinoReconstructionAnalyser(histMan)), //
		qcdAnalyser(new QCDAnalyser(histMan)), //
		ttbarPlusMETAnalyser_(new TTbarPlusMETAnalyser(histMan)), //
		vertexAnalyser(new VertexAnalyser(histMan)), //
		selectionCommissioning_(new Selections(histMan)) {
	for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
		ePlusJetsCutflow[cut] = 0;
		ePlusJetsSingleCuts[cut] = 0;
	}

	for (unsigned int cut = 0; cut < TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
		muPlusJetsCutFlow[cut] = 0;
		muPlusJetsSingleCuts[cut] = 0;
	}
	histMan->enableDebugMode(true);
}

Analysis::~Analysis() {

}

void Analysis::finishAnalysis() {
	printSummary();
	histMan->writeToDisk();
}

void Analysis::addInputFile(const char* fileName) {
	eventReader->addInputFile(fileName);
}

void Analysis::setMaximalNumberOfEvents(long maxEvents) {
	if (maxEvents > 0) {
		eventReader->setMaximumNumberOfEvents(maxEvents);
	}
}

void Analysis::setUsedNeutrinoSelectionForTopPairReconstruction(NeutrinoSelectionCriterion::value selection) {
	TopPairEventCandidate::usedNeutrinoSelection = selection;
}

void Analysis::setUsedTTbarReconstructionCriterion(TTbarReconstructionCriterion::value selection) {
	TopPairEventCandidate::usedTTbarReconstruction = selection;
}

void Analysis::checkForDuplicatedEvents() {
	map<unsigned long, std::vector<unsigned long> >::const_iterator iter;
	std::vector<pair<unsigned long, unsigned long> > duplicateEvents;

	for (iter = eventCheck.begin(); iter != eventCheck.end(); ++iter) {
		std::vector<unsigned long> events = (*iter).second;
		std::sort(events.begin(), events.end());
		for (unsigned long ev = 0; ev < events.size() - 1; ++ev) {
			if (events.at(ev) == events.at(ev + 1)) {
				duplicateEvents.push_back(make_pair((*iter).first, events.at(ev)));
			}
		}
	}

	if (duplicateEvents.size() > 0) {
		cout << "found duplicate events" << endl;
		for (unsigned long ev = 0; ev < duplicateEvents.size() - 1; ++ev) {
			cout << "run: " << duplicateEvents.at(ev).first << " event: " << duplicateEvents.at(ev).second << endl;
		}
	}
}

//void Analysis::checkForBrokenEvents() {
//	if (ttbarCandidate->Electrons().size() == 0) {
//		brokenEvents.push_back(InterestingEvent(currentEvent, eventReader->getCurrentFile()));
//	}
//
//	if (currentEvent->eventnumber() == 1019245) {
//		cout << "broken event" << endl;
//		ttbarCandidate->inspect();
//	}
//}

unsigned long Analysis::getNumberOfProccessedEvents() const {
	return eventReader->getNumberOfProccessedEvents();
}

