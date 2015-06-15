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
#include "../interface/ReconstructionModules/ChiSquaredBasedTopPairReconstruction.h"
#include "../interface/LumiReWeighting.h"
#include "../interface/GlobalVariables.h"
#include "../interface/BTagWeight.h"

using namespace reweight;
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
		const JetCollection jets(currentEvent->Jets());
		unsigned int numberOfJets(jets.size());
		unsigned int numberOfBJets(0);
		for (unsigned int index = 0; index < numberOfJets; ++index) {
			const JetPointer jet(currentEvent->Jets().at(index));
			if (jet->isBJet(BtagAlgorithm::CombinedSecondaryVertexV2, BtagAlgorithm::MEDIUM))
				++numberOfBJets;
		}
		histMan->setCurrentBJetBin(numberOfBJets);
		histMan->setCurrentJetBin(numberOfJets);

		vector<double> bjetWeights;
		if (currentEvent->isRealData()) {
			for (unsigned int index = 0; index <= numberOfBJets; ++index) {
				if (index == numberOfBJets)
					bjetWeights.push_back(1.);
				else
					bjetWeights.push_back(0);
			}
		} else
			bjetWeights = BjetWeights(jets, numberOfBJets);

		ttbar_plus_X_analyser_->analyse(currentEvent);
		if ( ( currentEvent->getDataType() == DataType::TTJets_amcatnloFXFX || currentEvent->getDataType() == DataType::TTJets_madgraphMLM ) && Globals::treePrefix_ == "" ) {
			pseudoTopAnalyser_->analyse(currentEvent);
			unfoldingRecoAnalyser_->analyse(currentEvent);
			partonAnalyser_->analyse(currentEvent);
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

	// Ignore PU and PDF weights for now
	if (!currentEvent->isRealData()) {
		weight = weights->getWeight(currentEvent->getDataType());
	// 	//TODO: fix this dirty little thing
	// 	std::cout << "Getting PU weight" << std::endl;
	// 	pileUpWeight = weights->reweightPileUp(currentEvent->getTrueNumberOfVertices().at(1));
	// 	weight *= pileUpWeight;
	// 	if (Globals::pdfWeightNumber != 0) {
	// 		try {
	// 			double pdf_weight(currentEvent->PDFWeights().at(Globals::pdfWeightNumber) / currentEvent->PDFWeights().at(0));
	// 			weight *= pdf_weight;

	// 			histMan->setCurrentHistogramFolder("");
	// 			histMan->H1D("PDFweights")->Fill(pdf_weight);
	// 		} catch (exception& e) {
	// 			cout << "PDF weight assigning exception: " << e.what() << endl;
	// 		}
	// 	}
	}

	//top pt weight
	if(Globals::applyTopPtReweighting == true && currentEvent->getDataType() == DataType::TTJets_amcatnloFXFX){

	double topPtweight = 1.;
	topPtweight = weights->reweightTopPt(currentEvent);

	weight *= topPtweight;
	}

	currentEvent->setEventWeight(weight);
	currentEvent->setPileUpWeight(pileUpWeight);

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
	cout << "total number of processed events: " << eventReader->getNumberOfProccessedEvents() << endl;
	cout << "number of events without electrons: " << brokenEvents.size() << endl;
	cout << "number of events with too high pileup: " << weights->getNumberOfEventsWithTooHighPileUp() << endl;
}

void Analysis::createHistograms() {
	histMan->prepareForSeenDataTypes(eventReader->getSeenDatatypes());
	treeMan->prepareForSeenDataTypes(eventReader->getSeenDatatypes());

	unsigned int numberOfHistograms(0), lastNumberOfHistograms(0);

	ttbar_plus_X_analyser_->createHistograms();
	numberOfHistograms = histMan->size();
	cout << "Number of histograms added by ttbar_plus_X_analyser: " << numberOfHistograms - lastNumberOfHistograms
			<< endl;
	lastNumberOfHistograms = numberOfHistograms;

	if ( ( eventReader->getSeenDatatypes()[DataType::TTJets_amcatnloFXFX] || eventReader->getSeenDatatypes()[DataType::TTJets_madgraphMLM] )
		&& Globals::treePrefix_ == "" ) {
		pseudoTopAnalyser_->createTrees();
		unfoldingRecoAnalyser_->createTrees();
		partonAnalyser_->createTrees();
		likelihoodInputAnalyser_->createTrees();
	}

	histMan->setCurrentHistogramFolder("");
	histMan->addH1D("PDFweights", "PDF weights", 1000, 0.8, 1.2);

	cout << "Total number of histograms: " << histMan->size() << endl;
}

Analysis::Analysis(std::string datasetInfoFile) : //
		eventReader(new NTupleEventReader()), //
		currentEvent(), //
		histMan(new BAT::HistogramManager()), //
		treeMan(new BAT::TreeManager()), //
		interestingEvents(), //
		brokenEvents(), //
		eventCheck(), //
		weights(new EventWeightProvider(datasetInfoFile)), //
		weight(0), //
		pileUpWeight(1), //
		metAnalyser(new METAnalyser(histMan, treeMan)), //
		ttbar_plus_X_analyser_(new TTbar_plus_X_analyser(histMan, treeMan)), //
		vertexAnalyser(new VertexAnalyser(histMan)),
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

