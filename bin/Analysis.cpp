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
#include "../interface/GlobalVariables.h"

using namespace BAT;
using namespace std;

void Analysis::analyse() {
	cout << "detected samples:" << endl;
	for (unsigned int sample = 0; sample < DataType::NUMBER_OF_DATA_TYPES; ++sample) {
		if (eventReader->getSeenDatatypes()[sample])
			cout << DataType::names[sample] << endl;
	}

	createHistograms();


	while (eventReader->hasNextEvent()) {

		initiateEvent();
		printNumberOfProccessedEventsEvery(Globals::printEveryXEvents);
		inspectEvents();

		// Check if MET fitlers are satisfied
		// cout << currentEvent->passesMETFilters() << endl;
		// if ( currentEvent->isRealData() && !currentEvent->passesMETFilters() ) continue;

		ttbar_plus_X_analyser_->analyse(currentEvent);
		if ( currentEvent->isTTJet(currentEvent->getDataType()) ) {
			pseudoTopAnalyser_->analyse(currentEvent);
			unfoldingRecoAnalyser_->analyse(currentEvent);
			if ( Globals::treePrefix_ == "" ) {
				partonAnalyser_->analyse(currentEvent);
			}
			// likelihoodInputAnalyser_->analyse(currentEvent);
		}
		treeMan->FillTrees();
	}
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
	weight = 1.;
	histMan->setCurrentDataType(currentEvent->getDataType());
	histMan->setCurrentJetBin(currentEvent->Jets().size());
	histMan->setCurrentBJetBin(0);

	treeMan->setCurrentDataType(currentEvent->getDataType());

	double pileUpWeight_up = 1;
	double pileUpWeight_down = 1;
	if (!currentEvent->isRealData()) {
		weight = weights->getWeight(currentEvent->getDataType());
		pileUpWeight = weights->reweightPileUp(currentEvent->getTrueNumberOfVertices().at(0));
		pileUpWeight_up = weights->reweightPileUp(currentEvent->getTrueNumberOfVertices().at(0), 1);
		pileUpWeight_down = weights->reweightPileUp(currentEvent->getTrueNumberOfVertices().at(0), -1);
	}

	// include generator weight
	// 1, except for amcatnlo samples (so far?)
	weight *= currentEvent->generatorWeight() / fabs( currentEvent->generatorWeight() );

	currentEvent->setEventWeight(weight);
	currentEvent->setPileUpWeight(pileUpWeight);
	currentEvent->setPileUpWeight(pileUpWeight_up, 1);
	currentEvent->setPileUpWeight(pileUpWeight_down, -1);
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

void Analysis::printInterestingEvents() {
	cout << "Interesting events:" << endl;
	for (unsigned int index = 0; index < interestingEvents.size(); ++index) {
		interestingEvents.at(index).print();
	}
}

void Analysis::printSummary() {
	cout << "Total number of processed events: " << eventReader->getNumberOfProccessedEvents() << endl;
}

void Analysis::createHistograms() {
	histMan->prepareForSeenDataTypes(eventReader->getSeenDatatypes());
	treeMan->prepareForSeenDataTypes(eventReader->getSeenDatatypes());

	ttbar_plus_X_analyser_->createTrees();

	pseudoTopAnalyser_->createTrees();
	unfoldingRecoAnalyser_->createTrees();

	if ( Globals::treePrefix_ == "" ) {
		partonAnalyser_->createTrees();
		likelihoodInputAnalyser_->createTrees();
	}
}

Analysis::Analysis(std::string datasetInfoFile) : //
		eventReader(new NTupleEventReader()), //
		currentEvent(), //
		histMan(new BAT::HistogramManager()), //
		treeMan(new BAT::TreeManager()), //
		interestingEvents(), //
		eventCheck(), //
		weights(new EventWeightProvider(datasetInfoFile)), //
		weight(0), //
		pileUpWeight(1), //
		ttbar_plus_X_analyser_(new TTbar_plus_X_analyser(histMan, treeMan)), //
		pseudoTopAnalyser_(new PseudoTopAnalyser(histMan, treeMan)),
		unfoldingRecoAnalyser_(new UnfoldingRecoAnalyser(histMan, treeMan)),
		partonAnalyser_(new PartonAnalyser(histMan, treeMan)),
		likelihoodInputAnalyser_(new LikelihoodInputAnalyser(histMan, treeMan)) {
	histMan->enableDebugMode(true);
}

Analysis::~Analysis() {

}

void Analysis::finishAnalysis() {
	printSummary();
	histMan->writeToDisk();
	treeMan->writeToDisk();
}

void Analysis::addInputFile(const char* fileName) {
	eventReader->addInputFile(fileName);
}

void Analysis::setMaximalNumberOfEvents(long maxEvents) {
	if (maxEvents > 0) {
		eventReader->setMaximumNumberOfEvents(maxEvents);
	}
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

unsigned long Analysis::getNumberOfProccessedEvents() const {
	return eventReader->getNumberOfProccessedEvents();
}

