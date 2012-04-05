/*
 * NTupleEventReader.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/Readers/NTupleEventReader.h"
#include "../../interface/GlobalVariables.h"
#include "TFile.h"
#include "TChainElement.h"
#include <iostream>
#include <boost/shared_ptr.hpp>
using namespace std;
namespace BAT {

const char * NTupleEventReader::EVENT_CHAIN = "rootTupleTree/tree";

bool NTupleEventReader::loadTracks = false;

NTupleEventReader::NTupleEventReader() :
		processedEvents(0), //
		maximalNumberOfEvents(999999999999), //
		currentEventEntry(0), //
		numberOfFiles(0), //
		input(new TChain(NTupleEventReader::EVENT_CHAIN)), //
		hltReader(new VariableReader<MultiIntPointer>(input, "Trigger.HLTResults")), //
		hltPrescaleReader(new VariableReader<MultiIntPointer>(input, "Trigger.HLTPrescales")), //
		vertexReader(new VertexReader(input)), //
		trackReader(new TrackReader(input)), //
		electronReader(new ElectronReader(input, Globals::electronAlgorithm)), //
		genParticleReader(new GenParticleReader(input)), //
		jetReader(new JetReader(input, Globals::jetAlgorithm)), //
		genJetReader(new GenJetReader(input)), //
		muonReader(new MuonReader(input, Globals::muonAlgorithm)), //
		metReader(new METReader(input, Globals::metAlgorithm)), //
		runNumberReader(new VariableReader<unsigned int>(input, "Event.Run")), //
		eventNumberReader(new VariableReader<unsigned int>(input, "Event.Number")), //
		lumiBlockReader(new VariableReader<unsigned int>(input, "Event.LumiSection")), //
		PDFWeightsReader(new VariableReader<MultiDoublePointer>(input, "Event.PDFWeights")), //
		PileupInfoReader(new VariableReader<MultiIntPointer>(input, "Event.PileUpInteractions")), //
		TruePileupInfoReader(new VariableReader<MultiIntPointer>(input, "Event.NumberOfTrueInteractions")), //
		PUWeightInTimeOnly_(new VariableReader<double>(input, "Event.PUWeightInTimeOnly")), //
		PUWeight3BX_(new VariableReader<double>(input, "Event.PUWeight3BX")), //
		PUWeight3D_(new VariableReader<double>(input, "Event.PUWeight3D")), //
		PUWeightShiftUp_(new VariableReader<double>(input, "Event.PUWeightShiftUp")), //
		PUWeightShiftDown_(new VariableReader<double>(input, "Event.PUWeightShiftDown")), //
		areReadersSet(false), //
		areDatatypesKnown(false), //
		currentEvent(), //
		seenDataTypes() {
}

NTupleEventReader::~NTupleEventReader() {
}

void NTupleEventReader::addInputFile(const TString fileName) {
	unsigned long filesAdded = input->Add(fileName, -1); //-1 == number of events is not read!
	if (filesAdded <= 0)
		throw NoFileFoundException("No file found in '" + TString(fileName) + "'");
	numberOfFiles += filesAdded;
}

void NTupleEventReader::addInputFileWithoutCheck(const char * fileName) {
	numberOfFiles += input->Add(fileName);
}

const EventPtr NTupleEventReader::getNextEvent() {
	currentEvent = EventPtr(new Event());
	selectNextNtupleEvent();

	boost::shared_ptr<std::vector<int> > triggers(new std::vector<int>());
	boost::shared_ptr<std::vector<int> > triggerPrescales(new std::vector<int>());

//    assert(hltReader->size() == HLTriggers::NUMBER_OF_TRIGGERS);
	for (unsigned int i = 0; i < hltReader->size(); i++) {
		triggers->push_back(hltReader->getIntVariableAt(i));
		triggerPrescales->push_back(hltPrescaleReader->getIntVariableAt(i));
	}

	currentEvent->setDataType(getDataType(getCurrentFile()));
	currentEvent->setHLTs(triggers);
	currentEvent->setHLTPrescales(triggerPrescales);
	currentEvent->setVertices(vertexReader->getVertices());

	if (NTupleEventReader::loadTracks)
		currentEvent->setTracks(trackReader->getTracks());
	//fill leptons BEFORE jets for jet cleaning
	currentEvent->setElectrons(electronReader->getElectrons());
	currentEvent->setMuons(muonReader->getMuons());

	if (!currentEvent->isRealData()) {
		currentEvent->setGenParticles(genParticleReader->getGenParticles());
		currentEvent->setGenJets(genJetReader->getGenJets());
		currentEvent->setGenNumberOfPileUpVertices(*PileupInfoReader->getVariable());
		currentEvent->setPDFWeights(*PDFWeightsReader->getVariable());

		if (Globals::NTupleVersion >= 6) {
			currentEvent->setTrueNumberOfPileUpVertices(*TruePileupInfoReader->getVariable());
			currentEvent->setPUWeightInTimeOnly(PUWeightInTimeOnly_->getVariable());
			currentEvent->setPUWeight3BX(PUWeight3BX_->getVariable());
			currentEvent->setPUWeight3D(PUWeight3D_->getVariable());
			currentEvent->setPUWeightShiftUp(PUWeightShiftUp_->getVariable());
			currentEvent->setPUWeightShiftDown(PUWeightShiftDown_->getVariable());
		}

	}

	currentEvent->setJets(jetReader->getJets());
	currentEvent->setMET(metReader->getMET());
	currentEvent->setRunNumber(runNumberReader->getVariable());
	currentEvent->setEventNumber(eventNumberReader->getVariable());
	currentEvent->setLocalEventNumber(currentEventEntry);
	currentEvent->setLumiBlock(lumiBlockReader->getVariable());
	currentEvent->setBeamScrapingVeto(false);

	return currentEvent;
}

void NTupleEventReader::selectNextNtupleEvent() {
	if (hasNextEvent()) {
		initiateReadersIfNotSet();
		input->GetEntry(currentEventEntry);
		currentEventEntry++;
		processedEvents++;
	}
}

bool NTupleEventReader::hasNextEvent() {
	if (numberOfFiles == 0)
		throw NoFileFoundException("No input file found!");
	bool hasNextInNTuple = input->LoadTree(currentEventEntry) >= 0;
	bool isWithinEventRange = currentEventEntry < maximalNumberOfEvents || maximalNumberOfEvents <= 0;
	return hasNextInNTuple && isWithinEventRange;
}

void NTupleEventReader::initiateReadersIfNotSet() {
	if (areReadersSet == false) {
		input->SetBranchStatus("*", 0);
		hltReader->initialise();
		hltPrescaleReader->initialise();
		vertexReader->initialise();
		if (NTupleEventReader::loadTracks)
			trackReader->initialise();
		electronReader->initialise();
		genParticleReader->initialise();
		jetReader->initialise();
		genJetReader->initialise();
		muonReader->initialise();
		metReader->initialise();
		runNumberReader->initialise();
		eventNumberReader->initialise();
		lumiBlockReader->initialise();
		PDFWeightsReader->initialiseBlindly();
		PileupInfoReader->initialiseBlindly();
		TruePileupInfoReader->initialiseBlindly();
		PUWeightInTimeOnly_->initialiseBlindly();
		PUWeight3BX_->initialiseBlindly();
		PUWeight3D_->initialiseBlindly();
		PUWeightShiftUp_->initialiseBlindly();
		PUWeightShiftDown_->initialiseBlindly();
		areReadersSet = true;
	}

}

DataType::value NTupleEventReader::getDataType(const std::string filename) {
	DataType::value filetype = DataType::ElectronHad;

	for (unsigned int index = 0; index < DataType::names.size(); ++index) {
		const std::string searchString(DataType::names.at(index));

		if (filename.find(searchString) != std::string::npos) {
			filetype = (DataType::value) index;
		}
	}
	return filetype;
}

unsigned long NTupleEventReader::getNumberOfProccessedEvents() const {
	return processedEvents;
}

unsigned long NTupleEventReader::getCurrentLocalEventNumber() const {
	return currentEventEntry;
}

void NTupleEventReader::skipNumberOfEvents(unsigned long skipNextNEvents) {
	currentEventEntry += skipNextNEvents;
}

void NTupleEventReader::setMaximumNumberOfEvents(long maxNumberOfEvents) {
	maximalNumberOfEvents = maxNumberOfEvents;
}

const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& NTupleEventReader::getSeenDatatypes() {
	if (areDatatypesKnown == false) {
		readDataTypes();
		areDatatypesKnown = true;
	}
	return seenDataTypes;
}

void NTupleEventReader::readDataTypes() {
	TIter nextFile(input->GetListOfFiles());
	TChainElement* file = 0;
	while ((file = (TChainElement*) nextFile()) != 0) {
		string fileName = file->GetTitle();
		DataType::value type = getDataType(fileName);
		seenDataTypes.at(type) = true;
	}
}

const char* NTupleEventReader::getCurrentFile() const {
	if (input->GetCurrentFile() == 0)
		throw NoFileFoundException("Tried to access non-existent file.");
	return input->GetCurrentFile()->GetName();
}
}
