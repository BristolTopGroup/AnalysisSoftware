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
		bjetAnalyser->analyse(ttbarCandidate);
		diElectronAnalyser->analyse(ttbarCandidate);
		electronAnalyser->analyse(ttbarCandidate);
		if (Globals::useHitFit) {
			if (currentEvent.getDataType() == DataType::ttbar)
				hitfitAnalyser->setMCTTbarHypothesis(mcAnalyser->GetMCTTbarHypothesis());
			hitfitAnalyser->analyse(ttbarCandidate);
		}
		hltriggerAnalyser->analyse(ttbarCandidate);
		jetAnalyser->analyse(ttbarCandidate);
		if (currentEvent.getDataType() == DataType::ttbar)
			mcAnalyser->analyse(ttbarCandidate);
		metAnalyser->analyse(ttbarCandidate);
		mttbarAnalyser->analyse(ttbarCandidate);
		muonAnalyser->analyse(ttbarCandidate);
		qcdAnalyser->analyse(ttbarCandidate);
		topRecoAnalyser->analyse(ttbarCandidate);
		vertexAnalyser->analyse(ttbarCandidate);

		//TODO: move to TTbarAnalysis
		doTTbarCutFlow();
//TODO: to be tidied up
		//                doSynchExercise();
//        if(currentEvent.getDataType() == DataType::DATA)
//            eventCheck[currentEvent.runnumber()].push_back(currentEvent.eventnumber());
//        checkForBrokenEvents();
	}
//    checkForDuplicatedEvents();
//    printInterestingEvents();
	printSummary();
}

void Analysis::printNumberOfProccessedEventsEvery(unsigned long printEvery) {
	unsigned long eventIndex = eventReader->getNumberOfProccessedEvents();
	if (eventIndex % printEvery == 0 || eventIndex == 1) {
		cout << "Analysing event no " << eventIndex << ", sample: " << DataType::names[currentEvent.getDataType()]
				<< endl;
		cout << "File: " << eventReader->getCurrentFile() << endl;
	}

}

void Analysis::initiateEvent() {
	currentEvent = eventReader->getNextEvent();
	ttbarCandidate = TopPairEventCandidate(currentEvent);
	weight = weights->getWeight(currentEvent.getDataType());
	if (!currentEvent.isRealData()) {
		pileUpWeight = weights->reweightPileUp(
				currentEvent.numberOfGeneratedPileUpVertices(Globals::pileUpReweightingMethod));
		weight *= pileUpWeight;
	}

	currentEvent.setEventWeight(weight);
	currentEvent.setPileUpWeight(pileUpWeight);
	ttbarCandidate.setEventWeight(weight);
	ttbarCandidate.setPileUpWeight(pileUpWeight);

	histMan->setCurrentDataType(ttbarCandidate.getDataType());
	histMan->setCurrentJetBin(currentEvent.GoodElectronCleanedJets().size());
	histMan->setCurrentBJetBin(currentEvent.GoodElectronCleanedBJets().size());
}

void Analysis::inspectEvents() {
	std::vector<InterestingEvent> eventsToInspect;

	for (unsigned int index = 0; index < eventsToInspect.size(); ++index) {
		if ((ttbarCandidate.runnumber() == eventsToInspect.at(index).runNumber
				&& ttbarCandidate.eventnumber() == eventsToInspect.at(index).eventNumber)) {
			cout << "file: " << eventReader->getCurrentFile() << endl;
			ttbarCandidate.inspect();
		}
	}

}

void Analysis::doSynchExercise() {
	if (ttbarCandidate.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::ConversionFinder)) {
		cout << ttbarCandidate.runnumber() << ":" << ttbarCandidate.eventnumber() << ":" << endl; //electron->et() << endl;
		if (ttbarCandidate.eventnumber() == 450622) {
			ttbarCandidate.inspect();
		}
	}
}

void Analysis::doTTbarCutFlow() {
	for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
		if (ttbarCandidate.passesEPlusJetsSelectionStep((TTbarEPlusJetsSelection::Step) cut)) {
			++ePlusJetsSingleCuts[cut];
			ePlusJetsSingleCutsPerFile[eventReader->getCurrentFile()][cut]++;
		}

		if (ttbarCandidate.passesEPlusJetsSelectionStepUpTo((TTbarEPlusJetsSelection::Step) cut)) {
			ePlusJetsCutflow[cut] += 1;
			ePlusJetsCutflowPerFile[eventReader->getCurrentFile()][cut]++;
			unsigned int njet = ttbarCandidate.GoodElectronCleanedJets().size();
			if (njet >= JetBin::NUMBER_OF_JET_BINS)
				njet = JetBin::NUMBER_OF_JET_BINS - 1;
			ePlusJetsCutflowPerSample.increase(ttbarCandidate.getDataType(), cut, njet, weight);
		}
	}

	for (unsigned int cut = 0; cut < TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
		if (ttbarCandidate.passesMuPlusJetsSelectionStep((TTbarMuPlusJetsSelection::Step) cut)) {
			++muPlusJetsSingleCuts[cut];
//			ePlusJetsSingleCutsPerFile[eventReader->getCurrentFile()][cut]++;
		}

		if (ttbarCandidate.passesMuPlusJetsSelectionStepUpTo((TTbarMuPlusJetsSelection::Step) cut)) {
			muPlusJetsCutFlow[cut] += 1;
//			ePlusJetsCutflowPerFile[eventReader->getCurrentFile()][cut]++;
			unsigned int njet = ttbarCandidate.GoodMuonCleanedJets().size();
			if (njet >= JetBin::NUMBER_OF_JET_BINS)
				njet = JetBin::NUMBER_OF_JET_BINS - 1;
			muPlusJetsCutflowPerSample.increase(ttbarCandidate.getDataType(), cut, njet, weight);
		}
	}
}

void Analysis::printInterestingEvents() {
	cout << "Interesting events:" << endl;
	for (unsigned int index = 0; index < interestingEvents.size(); ++index) {
		interestingEvents.at(index).print();
	}
}

void Analysis::printSummary() {
	EventTablePrinter::printCutFlowLatexTable(ePlusJetsCutflowPerSample, TTbarEPlusJetsSelection::StringSteps,
			Globals::luminosity);
	EventTablePrinter::printUnweightedCutFlowLatexTable(ePlusJetsCutflowPerSample, TTbarEPlusJetsSelection::StringSteps,
			Globals::luminosity);

	cout << "Muon + jets selection" << endl;
	EventTablePrinter::printCutFlowLatexTable(muPlusJetsCutflowPerSample, TTbarMuPlusJetsSelection::StringSteps,
			Globals::luminosity);
	EventTablePrinter::printUnweightedCutFlowLatexTable(muPlusJetsCutflowPerSample,
			TTbarMuPlusJetsSelection::StringSteps, Globals::luminosity);

	cout << "total number of processed events: " << eventReader->getNumberOfProccessedEvents() << endl;
	cout << endl;
	cout << "Electron + jets selection" << endl;
	for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
		cout << "Selection step '" << TTbarEPlusJetsSelection::StringSteps[cut] << "'" << endl;
		cout << "passed events (single cut): " << ePlusJetsSingleCuts.at(cut) << endl;
		if (cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS)
			cout << "passed events (up to this cut):" << ePlusJetsCutflow.at(cut) << endl;
		else
			cout << "passed events (full selection):" << ePlusJetsCutflow.at(cut) << endl;
		cout << endl;
	}

	cout << "Muon + jets selection" << endl;
	for (unsigned int cut = 0; cut < TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
		cout << "Selection step '" << TTbarMuPlusJetsSelection::StringSteps[cut] << "'" << endl;
		cout << "passed events (single cut): " << muPlusJetsSingleCuts.at(cut) << endl;

		if (cut < TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS)
			cout << "passed events (up to this cut):" << muPlusJetsCutFlow.at(cut) << endl;
		else
			cout << "passed events (full selection):" << muPlusJetsCutFlow.at(cut) << endl;
		cout << endl;
	}

	cout << "number of events without electrons: " << brokenEvents.size() << endl;
	cout << "number of events with too high pileup: " << weights->getNumberOfEventsWithTooHighPileUp() << endl;
}

void Analysis::createHistograms() {
	histMan->prepareForSeenDataTypes(eventReader->getSeenDatatypes());

	bjetAnalyser->createHistograms();
	diElectronAnalyser->createHistograms();
	electronAnalyser->createHistograms();
	hitfitAnalyser->createHistograms();
	hltriggerAnalyser->createHistograms();
	jetAnalyser->createHistograms();
	mcAnalyser->createHistograms();
	metAnalyser->createHistograms();
	mttbarAnalyser->createHistograms();
	muonAnalyser->createHistograms();
	qcdAnalyser->createHistograms();
	topRecoAnalyser->createHistograms();
	vertexAnalyser->createHistograms();

}

Analysis::Analysis(std::string fileForPileUpReweighting) : //
		eventReader(new NTupleEventReader()), //
		currentEvent(), //
		ttbarCandidate(), //
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
		weights(new EventWeightProvider(Globals::luminosity/*current lumi*/, 7, fileForPileUpReweighting)), //
		weight(0), //
		pileUpWeight(1), //
		ePlusJetsCutflowPerSample(DataType::NUMBER_OF_DATA_TYPES, TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS,
				JetBin::NUMBER_OF_JET_BINS), //
		muPlusJetsCutflowPerSample(DataType::NUMBER_OF_DATA_TYPES, TTbarMuPlusJetsSelection::NUMBER_OF_SELECTION_STEPS,
				JetBin::NUMBER_OF_JET_BINS), //
		bjetAnalyser(new BJetAnalyser(histMan)), //
		diElectronAnalyser(new DiElectronAnalyser(histMan)), //
		electronAnalyser(new ElectronAnalyser(histMan)), //
		hitfitAnalyser(new HitFitAnalyser(histMan)), //
		hltriggerAnalyser(new HLTriggerAnalyser(histMan)), //
		jetAnalyser(new JetAnalyser(histMan)), //
		mcAnalyser(new MCAnalyser(histMan)), //
		metAnalyser(new METAnalyser(histMan)), //
		mttbarAnalyser(new MTtbarAnalyser(histMan)), //
		muonAnalyser(new MuonAnalyser(histMan)), //
		qcdAnalyser(new QCDAnalyser(histMan)), //
		topRecoAnalyser(new TopReconstructionAnalyser(histMan)), //
		vertexAnalyser(new VertexAnalyser(histMan, weights)) //
{
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

void Analysis::checkForBrokenEvents() {
	if (ttbarCandidate.Electrons().size() == 0) {
		brokenEvents.push_back(InterestingEvent(ttbarCandidate, eventReader->getCurrentFile()));
	}

	if (ttbarCandidate.eventnumber() == 1019245) {
		cout << "broken event" << endl;
		ttbarCandidate.inspect();
	}
}

unsigned long Analysis::getNumberOfProccessedEvents() const {
	return eventReader->getNumberOfProccessedEvents();
}

