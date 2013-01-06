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
#include "../../interface/HighLevelTriggers.h"

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
//		genMetReader(new GenMETReader(input)), //
		metReaders(), //
		metCorrReaders(), //
		runNumberReader(new VariableReader<unsigned int>(input, "Event.Run")), //
		eventNumberReader(new VariableReader<unsigned int>(input, "Event.Number")), //
		lumiBlockReader(new VariableReader<unsigned int>(input, "Event.LumiSection")), //
		PDFWeightsReader(new VariableReader<MultiDoublePointer>(input, "Event.PDFWeights")), //
		PileupInfoReader(new VariableReader<MultiIntPointer>(input, "Event.PileUpInteractions")), //
		TruePileupInfoReader(new VariableReader<MultiIntPointer>(input, "Event.NumberOfTrueInteractions")), //
		PUWeightInTimeOnly_(new VariableReader<double>(input, "Event.PUWeightInTimeOnly")), //
		PUWeight3BX_(new VariableReader<double>(input, "Event.PUWeight3BX")), //
//		PUWeight3D_(new VariableReader<double>(input, "Event.PUWeight3D")), //
		PUWeightShiftUp_(new VariableReader<double>(input, "Event.PUWeightShiftUp")), //
		PUWeightShiftDown_(new VariableReader<double>(input, "Event.PUWeightShiftDown")), //
		sumETReader_(new VariableReader<double>(input, "Event.SumET")), //
		HCALLaserFilter(new VariableReader<bool>(input, "Event.HCALLaserFilter")), //
		ECALDeadCellFilter(new VariableReader<bool>(input, "Event.ECALDeadCellFilter")), //
		ECALDeadCellTPFilter(new VariableReader<bool>(input, "Event.ECALDeadCellTriggerPrimitiveFilter")), //
		TrackingFailureFilter(new VariableReader<bool>(input, "Event.TrackingFailureFilter")), //
		CSCTightHaloId(new VariableReader<bool>(input, "Event.CSCTightHaloId")), //
		EEBadSCFilter(new VariableReader<bool>(input, "Event.EEBadSCFilter")), //
		ECALLaserCorrFilter(new VariableReader<bool>(input, "Event.ECALLaserCorrFilter")), //
		TrackingPOGFilters(new VariableReader<bool>(input, "Event.TrackingPOGFilters")), //
		areReadersSet(false), //
		areDatatypesKnown(false), //
		currentEvent(), //
		seenDataTypes() {
	metReaders.resize(METAlgorithm::NUMBER_OF_METALGORITHMS);
	metCorrReaders.resize(METCorrections::NUMBER_OF_METCORRECTIONS);

	for (unsigned int index = 0; index < METCorrections::NUMBER_OF_METCORRECTIONS; ++ index) {
		metCorrReaders.at(index) = boost::shared_ptr<METCorrReader>(new METCorrReader(input, (METCorrections::value) index));
	}

	for (unsigned int index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, index))
			continue;
		metReaders.at(index) = boost::shared_ptr<METReader>(new METReader(input, (METAlgorithm::value) index));
	}

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

	while (triggers->size() < HLTriggers::NUMBER_OF_TRIGGERS) {
		triggers->push_back(0);
		triggerPrescales->push_back(0);
	}

	currentEvent->setDataType(getDataType(getCurrentFile()));
	currentEvent->setFile(getCurrentFile());
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
			currentEvent->setPUWeightShiftUp(PUWeightShiftUp_->getVariable());
			currentEvent->setPUWeightShiftDown(PUWeightShiftDown_->getVariable());
		}

	}

	currentEvent->setJets(jetReader->getJets());

	double sysShiftMetCorrectionX = 0;
	double sysShiftMetCorrectionY = 0;
	double type0MetCorrectionX = 0;
	double type0MetCorrectionY = 0;
	double type1MetCorrectionX = 0;
	double type1MetCorrectionY = 0;

	if (Globals::NTupleVersion > 8) {
		if (Globals::applySysShiftMetCorrection) {
			metCorrReaders.at(METCorrections::pfMetSysShiftCorrections)->readMETCorrections();
			sysShiftMetCorrectionX = metCorrReaders.at(METCorrections::pfMetSysShiftCorrections)->getXcorrection();
			sysShiftMetCorrectionY = metCorrReaders.at(METCorrections::pfMetSysShiftCorrections)->getYcorrection();
		}
		if (Globals::applyType0MetCorrection) {
			metCorrReaders.at(METCorrections::pfMetType0Corrections)->readMETCorrections();
			type0MetCorrectionX = metCorrReaders.at(METCorrections::pfMetType0Corrections)->getXcorrection();
			type0MetCorrectionY = metCorrReaders.at(METCorrections::pfMetType0Corrections)->getYcorrection();
		}
		if (Globals::applyType1MetCorrection) {
			metCorrReaders.at(METCorrections::pfMetType1Corrections)->readMETCorrections();
			type1MetCorrectionX = metCorrReaders.at(METCorrections::pfMetType1Corrections)->getXcorrection();
			type1MetCorrectionY = metCorrReaders.at(METCorrections::pfMetType1Corrections)->getYcorrection();
		}
	}

	double totalMetCorrectionX = sysShiftMetCorrectionX + type0MetCorrectionX + type1MetCorrectionX;
	double totalMetCorrectionY = sysShiftMetCorrectionY + type0MetCorrectionY + type1MetCorrectionY;

	METCollection mets;
	mets.resize(METAlgorithm::NUMBER_OF_METALGORITHMS);
	for (unsigned int index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, index))
			continue;
		bool isMCOnlyMET = MET::isMCOnlyMETType(index);
		if (isMCOnlyMET && currentEvent->isRealData())
			continue;
		const METPointer met(metReaders.at(index)->getMET(totalMetCorrectionX, totalMetCorrectionY));
		if (Globals::NTupleVersion >= 7){
			met->setSumET(sumETReader_->getVariable());
			currentEvent->setHCALLaserFilter(HCALLaserFilter->getVariable());
			currentEvent->setECALDeadCellFilter(ECALDeadCellFilter->getVariable());
			currentEvent->setTrackingFailureFilter(TrackingFailureFilter->getVariable());
			currentEvent->setCSCTightBeamHaloFilter(!CSCTightHaloId->getVariable());
			currentEvent->setHBHENoiseFilter(true);//we filter on this for the skim
		}
		if (Globals::NTupleVersion >= 9){
			currentEvent->setECALDeadCellTPFilter(ECALDeadCellTPFilter->getVariable());
		}
		if (Globals::NTupleVersion >= 10){
			currentEvent->setEEBadSCFilter(EEBadSCFilter->getVariable());
			currentEvent->setECALLaserCorrFilter(ECALLaserCorrFilter->getVariable());
			currentEvent->setTrackingPOGFilters(TrackingPOGFilters->getVariable());
		}
		mets.at(index) = met;
	}

	currentEvent->setMETs(mets);
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
		runNumberReader->initialise();
		eventNumberReader->initialise();
		lumiBlockReader->initialise();
		if (Globals::NTupleVersion >= 6) { //MC only info!
			PDFWeightsReader->initialiseBlindly();
			PileupInfoReader->initialiseBlindly();
			TruePileupInfoReader->initialiseBlindly();
			PUWeightInTimeOnly_->initialiseBlindly();
			PUWeight3BX_->initialiseBlindly();
			PUWeightShiftUp_->initialiseBlindly();
			PUWeightShiftDown_->initialiseBlindly();
		}
		if (Globals::NTupleVersion >= 7) {
			sumETReader_->initialise();
			HCALLaserFilter->initialise();
			ECALDeadCellFilter->initialise();
			TrackingFailureFilter->initialise();
			CSCTightHaloId->initialise();
		}
		if (Globals::NTupleVersion >= 9) {
			ECALDeadCellTPFilter->initialise();
		}
		if (Globals::NTupleVersion >= 10) {
			EEBadSCFilter->initialise();
			ECALLaserCorrFilter->initialise();
			TrackingPOGFilters->initialise();
		}

		if (Globals::NTupleVersion > 8) {
			for (unsigned int index = 0; index < METCorrections::NUMBER_OF_METCORRECTIONS; ++index) {
				metCorrReaders.at(index)->initialise();
			}
		}

		for (unsigned int index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
			if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, index))
				continue;
			bool isMCOnlyMET = MET::isMCOnlyMETType(index);
			if (isMCOnlyMET)
				metReaders.at(index)->initialiseBlindly();
			else
				metReaders.at(index)->initialise();

		}
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
