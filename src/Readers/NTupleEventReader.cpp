/*
 * NTupleEventReader.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/Readers/NTupleEventReader.h"
#include "TFile.h"
#include "TChainElement.h"
#include <iostream>
#include <boost/shared_ptr.hpp>
using namespace std;
namespace BAT {

const char * NTupleEventReader::EVENT_CHAIN = "rootTupleTree/tree";

JetAlgorithm::value NTupleEventReader::jetAlgorithm = JetAlgorithm::Calo_AntiKT_Cone05;
ElectronAlgorithm::value NTupleEventReader::electronAlgorithm = ElectronAlgorithm::Calo;
METAlgorithm::value NTupleEventReader::metAlgorithm = METAlgorithm::Calo;
MuonAlgorithm::value NTupleEventReader::muonAlgorithm = MuonAlgorithm::Default;
bool NTupleEventReader::loadTracks = false;

NTupleEventReader::NTupleEventReader() :
    processedEvents(0),
    maximalNumberOfEvents(999999999),
    currentEventEntry(0),
    numberOfFiles(0),
    input(new TChain(NTupleEventReader::EVENT_CHAIN)),
    hltReader(new VariableReader<MultiIntPointer>(input, "HLTResults")),
    primaryReader(new PrimaryVertexReader(input)),
    trackReader(new TrackReader(input)),
    electronReader(new ElectronReader(input, NTupleEventReader::electronAlgorithm)),
    genParticleReader(new GenParticleReader(input)),
    jetReader(new JetReader(input, NTupleEventReader::jetAlgorithm)),
    genJetReader(new GenJetReader(input)),
    muonReader(new MuonReader(input, NTupleEventReader::muonAlgorithm)),
    metReader(new METReader(input, NTupleEventReader::metAlgorithm)),
    runNumberReader(new VariableReader<unsigned int> (input, "run")),
    eventNumberReader(new VariableReader<unsigned int> (input, "event")),
    lumiBlockReader(new VariableReader<unsigned int> (input, "ls")),
    beamScrapingReader(new VariableReader<bool> (input, "isBeamScraping")),
    areReadersSet(false),
    areDatatypesKnown(false),
    currentEvent(),
    seenDataTypes() {
}

NTupleEventReader::~NTupleEventReader() {
}

void NTupleEventReader::addInputFile(const char * fileName) {
    unsigned long filesAdded = input->Add(fileName, -1);//-1 == number of events is not read!
    if (filesAdded <= 0)
        throw NoFileFoundException("No file found in '" + TString(fileName) + "'");
    numberOfFiles += filesAdded;
}

void NTupleEventReader::addInputFileWithoutCheck(const char * fileName) {
    numberOfFiles += input->Add(fileName);
}

const Event& NTupleEventReader::getNextEvent() {
    currentEvent = Event();
    selectNextNtupleEvent();

    boost::shared_ptr<std::vector<int> > triggers(new std::vector<int>());

    for(unsigned int i = 0; i < hltReader->size(); i++){
        triggers->push_back(hltReader->getIntVariableAt(i));
    }

    currentEvent.setDataType(getDataType(getCurrentFile()));
    currentEvent.setHLTs(triggers);
    currentEvent.setPrimaryVertex(primaryReader->getVertex());

    if(NTupleEventReader::loadTracks)
        currentEvent.setTracks(trackReader->getTracks());
    currentEvent.setElectrons(electronReader->getElectrons());

    if(!currentEvent.isRealData()) {
    	currentEvent.setGenParticles(genParticleReader->getGenParticles());
    	currentEvent.setGenJets(genJetReader->getGenJets());
    }

    currentEvent.setJets(jetReader->getJets());
    currentEvent.setMuons(muonReader->getMuons());
    currentEvent.setMET(metReader->getMET());
    currentEvent.setRunNumber(runNumberReader->getVariable());
    currentEvent.setEventNumber(eventNumberReader->getVariable());
    currentEvent.setLocalEventNumber(currentEventEntry);
    currentEvent.setLumiBlock(lumiBlockReader->getVariable());
    currentEvent.setBeamScrapingVeto(beamScrapingReader->getVariable());

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
    bool isWithinEventRange = currentEventEntry < maximalNumberOfEvents;
    return hasNextInNTuple && isWithinEventRange;
}

void NTupleEventReader::initiateReadersIfNotSet() {
    if (areReadersSet == false) {
        input->SetBranchStatus("*", 0);
        hltReader->initialise();
        primaryReader->initialise();
        if(NTupleEventReader::loadTracks)
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
        beamScrapingReader->initialise();
        areReadersSet = true;
    }

}

DataType::value NTupleEventReader::getDataType(const std::string filename) {
    DataType::value filetype = DataType::DATA;

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

void NTupleEventReader::setMaximumNumberOfEvents(unsigned long maxNumberOfEvents) {
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
