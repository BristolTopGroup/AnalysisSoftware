/*
 * Event.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../interface/Event.h"
#include "../interface/GlobalVariables.h"
#include <iostream>
#include <numeric>
#include <set>

using namespace std;

namespace BAT {
bool Event::useCustomConversionTagger = false;
bool Event::usePFIsolation = false;
//bool Event::useCiCElectronID = false;
Event::Event() :
    HLTs(new std::vector<int>()),
    HLTPrescales(new std::vector<int>()),
    vertices(),
    goodVertices(),
    tracks(),
    allElectrons(),
    goodElectrons(),
    goodIsolatedElectrons(),
    goodPFIsolatedElectrons(),
    looseElectrons(),
    qcdElectrons(),
    allJets(),
    goodJets(),
    goodBJets(),
    goodElectronCleanedJets(),
    goodElectronCleanedBJets(),
    goodMuonCleanedJets(),
    goodMuonCleanedBJets(),
    allMuons(),
    goodMuons(),
    goodIsolatedMuons(),
    goodPFIsolatedMuons(),
    looseMuons(),
    genParticles(),
    met(),
    dataType(DataType::ElectronHad),
    runNumber(0),
    eventNumber(0),
    lumiBlock(0),
    eventWeight(1.),
    numberOfHighPurityTracks(0),
    isBeamScraping(true),
    genNumberOfPileUpVertices(0),
    ptdensityRho(0.){
}

Event::~Event() {
}

bool Event::isRealData() const {
    return dataType == DataType::ElectronHad || dataType == DataType::MuHad;
}

const DataType::value Event::getDataType() const {
    return dataType;
}

void Event::setDataType(DataType::value type) {
    dataType = type;
}

void Event::setVertices(VertexCollection vertices) {
    this->vertices.clear();
    this->vertices = vertices;

    selectVerticesByQuality();
}

void Event::selectVerticesByQuality() {
    goodVertices.clear();

    for (unsigned int i = 0; i < vertices.size(); ++i) {
        if (vertices.at(i)->isGood())
            goodVertices.push_back(vertices.at(i));
    }
}

void Event::setTracks(TrackCollection tracks) {
    this->tracks.clear();
    this->tracks = tracks;
    numberOfHighPurityTracks = 0;
    for (unsigned int index = 0; index < tracks.size(); ++index) {
        if (tracks.at(index)->isHighPurity())
            numberOfHighPurityTracks++;
    }
}

void Event::setGenParticles(MCParticleCollection genParticles) {
	this->genParticles = genParticles;
}

void Event::setElectrons(ElectronCollection electrons) {
    allElectrons.clear();
    allElectrons = electrons;
    selectElectronsByQuality();
}

void Event::selectElectronsByQuality() {
    goodElectrons.clear();
    goodIsolatedElectrons.clear();
    goodPFIsolatedElectrons.clear();
    for (unsigned int index = 0; index < allElectrons.size(); ++index) {
        ElectronPointer electron = allElectrons.at(index);

        bool isGood(electron->isGood((short) Globals::electronID));
        bool isIsolated = electron->relativeIsolation() < Globals::maxElectronRelativeIsolation;
        bool isPFIsolated = electron->isPFLepton() && electron->pfIsolation() < Globals::maxElectronPFIsolation;

        if (isGood)
            goodElectrons.push_back(electron);



        if (isGood && isIsolated)
            goodIsolatedElectrons.push_back(electron);

            if(isGood && isPFIsolated)
                goodPFIsolatedElectrons.push_back(electron);

        if (electron->isLoose())
            looseElectrons.push_back(electron);
    }
}

void Event::setJets(JetCollection jets) {
    allJets.clear();
    goodJets.clear();
    goodElectronCleanedJets.clear();
    goodMuonCleanedJets.clear();

    goodBJets.clear();
    goodElectronCleanedBJets.clear();
    goodMuonCleanedBJets.clear();

    allJets = jets;
    selectGoodJets();
}

void Event::setGenJets(JetCollection jets) {
    genJets.clear();
    genJets = jets;
}


void Event::selectGoodJets() {

	for (unsigned int index = 0; index < allJets.size(); ++index) {
		const JetPointer jet = allJets.at(index);
		if (jet->isGood()) {
			goodJets.push_back(jet);
		}
	}

	goodBJets = GetBJetCollection(goodJets, Globals::btagAlgorithm, Globals::btagWorkingPoint);

	cleanGoodJets();

	goodElectronCleanedBJets = GetBJetCollection(goodElectronCleanedJets, Globals::btagAlgorithm,
			Globals::btagWorkingPoint);
	goodMuonCleanedBJets = GetBJetCollection(goodMuonCleanedJets, Globals::btagAlgorithm, Globals::btagWorkingPoint);

}

JetCollection Event::GetBJetCollection(const JetCollection& jets, BtagAlgorithm::value btagAlgorithm,
		BtagAlgorithm::workingPoint WP) const{
	JetCollection bjets;
	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet = jets.at(index);
		if (jet->isBJet(btagAlgorithm, WP))
			bjets.push_back(jet);
	}

	return bjets;
}

void Event::cleanGoodJets() {

	if(goodJets.size() > 0){
		if(goodIsolatedElectrons.size() > 0 || goodPFIsolatedElectrons.size() > 0){
			if (Event::usePFIsolation)
				goodElectronCleanedJets = cleanGoodJetsAgainstIsolatedElectrons(goodPFIsolatedElectrons);
			else
				goodElectronCleanedJets = cleanGoodJetsAgainstIsolatedElectrons(goodIsolatedElectrons);
		}
		else if(allElectrons.size() > 0)
			goodElectronCleanedJets = cleanGoodJetsAgainstMostIsolatedLepton(MostIsolatedElectron(allElectrons, Event::usePFIsolation));

		if (goodIsolatedMuons.size() > 0 || goodPFIsolatedMuons.size() > 0) {
			if (Event::usePFIsolation)
				goodMuonCleanedJets = cleanGoodJetsAgainstIsolatedMuons(goodPFIsolatedMuons);
			else
				goodMuonCleanedJets = cleanGoodJetsAgainstIsolatedMuons(goodIsolatedMuons);
		}
		else if(allMuons.size() > 0){
			goodMuonCleanedJets = cleanGoodJetsAgainstMostIsolatedLepton(MostIsolatedMuon(allMuons, Event::usePFIsolation));
		}

	}

}

JetCollection Event::cleanGoodJetsAgainstIsolatedElectrons(const ElectronCollection& electrons) const {
	JetCollection cleanedJets;
	set<unsigned int> cleanedJetsIndices;

	for (unsigned int jetIndex = 0; jetIndex < goodJets.size(); ++jetIndex) {
		for (unsigned int electronIndex = 0; electronIndex < electrons.size(); ++electronIndex) {
			if (!goodJets.at(jetIndex)->isWithinDeltaR(0.3, electrons.at(electronIndex))) {
				cleanedJetsIndices.insert(jetIndex);
			}
		}
	}

	for (set<unsigned int>::iterator jetIndex = cleanedJetsIndices.begin(); jetIndex != cleanedJetsIndices.end();
			++jetIndex) {
		cleanedJets.push_back(goodJets.at(*jetIndex));
	}

	return cleanedJets;
}

JetCollection Event::cleanGoodJetsAgainstIsolatedMuons(const MuonCollection& muons) const {
	JetCollection cleanedJets;
	set<unsigned int> cleanedJetsIndices;

	for (unsigned int jetIndex = 0; jetIndex < goodJets.size(); ++jetIndex) {
		for (unsigned int muonIndex = 0; muonIndex < muons.size(); ++muonIndex) {
			if (!goodJets.at(jetIndex)->isWithinDeltaR(0.3, muons.at(muonIndex))) {
				cleanedJetsIndices.insert(jetIndex);
			}
		}
	}

	for (set<unsigned int>::iterator jetIndex = cleanedJetsIndices.begin(); jetIndex != cleanedJetsIndices.end();
			++jetIndex) {
		cleanedJets.push_back(goodJets.at(*jetIndex));
	}

	return cleanedJets;
}

JetCollection Event::cleanGoodJetsAgainstMostIsolatedLepton(const LeptonPointer mostIsolatedLepton) const {
	JetCollection cleanedJets;

    for (unsigned int jetIndex = 0; jetIndex < goodJets.size(); ++jetIndex) {
        if (!goodJets.at(jetIndex)->isWithinDeltaR(0.3, mostIsolatedLepton)) {
        	cleanedJets.push_back(goodJets.at(jetIndex));
        }
    }
    return cleanedJets;
}

const ElectronPointer Event::MostIsolatedElectron(const ElectronCollection& electrons, bool usePFIso) const {
    float bestIsolation = 999999999;
    unsigned int bestIsolatedLepton = 990;
    for (unsigned int index = 0; index < electrons.size(); ++index) {
        float currentIsolation = 999999999;
        if(usePFIso)
            currentIsolation = electrons.at(index)->pfIsolation();
        else
            currentIsolation = electrons.at(index)->relativeIsolation();

        if (currentIsolation < bestIsolation) {
            bestIsolation = currentIsolation;
            bestIsolatedLepton = index;
        }
    }
    return electrons.at(bestIsolatedLepton);
}

const MuonPointer Event::MostIsolatedMuon(const MuonCollection& muons, bool usePFIso) const {
    float bestIsolation = 999999999;
    unsigned int bestIsolatedLepton = 990;
    for (unsigned int index = 0; index < muons.size(); ++index) {
        float currentIsolation = 999999999;
        if(usePFIso)
            currentIsolation = muons.at(index)->pfIsolation();
        else
            currentIsolation = muons.at(index)->relativeIsolation();

        if (currentIsolation < bestIsolation) {
            bestIsolation = currentIsolation;
            bestIsolatedLepton = index;
        }
    }
    return muons.at(bestIsolatedLepton);
}

const ElectronPointer Event::MostIsolatedElectron(const ElectronCollection& electrons) const{
    return MostIsolatedElectron(electrons, false);
}

const ElectronPointer Event::MostPFIsolatedElectron(const ElectronCollection& electrons) const{
    return MostIsolatedElectron(electrons, true);
}

const MuonPointer Event::MostIsolatedMuon(const MuonCollection& muons) const{
    return MostIsolatedMuon(muons, false);
}

const MuonPointer Event::MostPFIsolatedMuon(const MuonCollection& muons) const{
    return MostIsolatedMuon(muons, true);
}


void Event::setMuons(MuonCollection muons) {
    allMuons.clear();
    allMuons = muons;
    selectMuonsByQuality();
}

void Event::selectMuonsByQuality() {
    goodMuons.clear();
    goodIsolatedMuons.clear();
    goodPFIsolatedMuons.clear();
    looseMuons.clear();
    for (unsigned int index = 0; index < allMuons.size(); ++index) {
		MuonPointer muon = allMuons.at(index);

		bool isGood(muon->isGood());
		bool isPFIsolated = muon->isPFLepton() && muon->pfIsolation() < Globals::maxMuonPFIsolation;
		if (isGood)
			goodMuons.push_back(muon);

		if (isGood && muon->relativeIsolation() < Globals::maxMuonRelativeIsolation)
			goodIsolatedMuons.push_back(muon);

		if (isGood && isPFIsolated)
			goodPFIsolatedMuons.push_back(muon);

		if (muon->isLoose())
			looseMuons.push_back(muon);

	}
}

void Event::setHLTs(const boost::shared_ptr<std::vector<int> > triggers){
    HLTs = triggers;
}

void Event::setMET(const METPointer met) {
    this->met = met;
}

void Event::setRunNumber(unsigned long number) {
    runNumber = number;
}

void Event::setEventNumber(unsigned long number) {
    eventNumber = number;
}

void Event::setLocalEventNumber(unsigned long number) {
    localEventNumber = number;
}

void Event::setLumiBlock(unsigned long block) {
    lumiBlock = block;
}

void Event::setEventWeight(double weight) {
    eventWeight = weight;
}

void Event::setPileUpWeight(double weight) {
	pileUpWeight = weight;
}

void Event::setBeamScrapingVeto(bool isScraping){
    isBeamScraping = isScraping;
}

const VertexPointer Event::PrimaryVertex() const {
    return goodVertices.front();
}

const VertexCollection& Event::Vertices() const {
    return vertices;
}

const VertexCollection& Event::GoodVertices() const {
    return goodVertices;
}

const TrackCollection& Event::Tracks() const {
    return tracks;
}

const ElectronCollection& Event::Electrons() const {
    return allElectrons;
}

const ElectronCollection& Event::GoodElectrons() const {
    return goodElectrons;
}

const ElectronCollection& Event::GoodIsolatedElectrons() const {
    return goodIsolatedElectrons;
}

const ElectronCollection& Event::GoodPFIsolatedElectrons() const {
    return goodPFIsolatedElectrons;
}

const ElectronCollection& Event::QCDElectrons() const{
    return qcdElectrons;
}

const JetCollection& Event::Jets() const {
    return allJets;
}

const JetCollection& Event::GoodJets() const {
    return goodJets;
}

const JetCollection& Event::GenJets() const {
    return genJets;
}

const JetCollection& Event::GoodElectronCleanedJets() const {
	return goodElectronCleanedJets;
}

const JetCollection& Event::GoodMuonCleanedJets() const {
	return goodMuonCleanedJets;
}

const JetCollection& Event::GoodBJets() const {
    return goodBJets;
}

const JetCollection& Event::GoodElectronCleanedBJets() const {
	return goodElectronCleanedBJets;
}

const JetCollection& Event::GoodMuonCleanedBJets() const {
	return goodMuonCleanedBJets;
}

const MuonCollection& Event::Muons() const {
    return allMuons;
}

const MuonCollection& Event::GoodMuons() const {
    return goodMuons;
}

const MuonCollection& Event::GoodIsolatedMuons() const {
    return goodIsolatedMuons;
}

const MuonCollection& Event::GoodPFIsolatedMuons() const {
    return goodPFIsolatedMuons;
}

const MCParticleCollection& Event::GenParticles() const {
	return genParticles;
}

const METPointer Event::MET() const {
    return met;
}

unsigned long Event::runnumber() const {
    return runNumber;
}

unsigned long Event::eventnumber() const {
    return eventNumber;
}

unsigned long Event::localnumber() const {
    return localEventNumber;
}

unsigned long Event::lumiblock() const {
    return lumiBlock;
}

double Event::weight() const {
    return eventWeight;
}

double Event::PileUpWeight() const {
	return pileUpWeight;
}

void Event::inspect() const {
    cout << "run " << runNumber << ", event number " << eventNumber << ", lumi section " << lumiBlock << endl;

    cout << "number of tracks: " << tracks.size() << endl;
    cout << "number of high purity tracks: " << numberOfHighPurityTracks << endl;

    cout << "number of jets: " << allJets.size() << endl;
    EventContentPrinter::printJets(allJets);
    cout << "number of good jets: " << goodJets.size() << endl;
    EventContentPrinter::printJets(goodJets);

    cout << "number of good isolated electrons: " << goodIsolatedElectrons.size() << endl;
    EventContentPrinter::printElectrons(goodIsolatedElectrons);

    cout << "number of good electrons: " << goodElectrons.size() << endl;
    EventContentPrinter::printElectrons(goodElectrons);

    cout << "number of electrons: " << allElectrons.size() << endl;
    EventContentPrinter::printElectrons(allElectrons);
}

bool Event::HLT(HLTriggers::value trigger) const{
    return HLTs->at(trigger) > 0;
}

void Event::setGenNumberOfPileUpVertices(std::vector<int> pileup){
    genNumberOfPileUpVertices = pileup;
}

//double Event::averageNumberOfGeneratedPileUpVertices() const {
//	double average = std::accumulate(genNumberOfPileUpVertices.begin(), genNumberOfPileUpVertices.end(), 0)/genNumberOfPileUpVertices.size();
//    return average;
//}

const std::vector<int> Event::GeneratedPileUpVertices() const {
	return genNumberOfPileUpVertices;
}

double Event::numberOfGeneratedPileUpVertices(PileUpReweightingMethod::value method) const {
	using namespace std;

	double numberOfVertices(0);
	double average(0);

	switch (method) {
	case PileUpReweightingMethod::averagePileUp:
		average = accumulate(genNumberOfPileUpVertices.begin(), genNumberOfPileUpVertices.end(), 0);
		average = average / genNumberOfPileUpVertices.size();
		numberOfVertices = average;
		break;
	case PileUpReweightingMethod::inTimePileUpOnly:
		numberOfVertices = genNumberOfPileUpVertices.at(1);
		break;
	case PileUpReweightingMethod::threeDReweighting:
		cout << "Pile-up reweighting method '3D-reweighting' not implemented" << endl;
		return 0;
	}

	return numberOfVertices;
}
void Event::setHLTPrescales(const boost::shared_ptr<std::vector<int> > prescales){
	HLTPrescales = prescales;
}

int Event::HLTPrescale(HLTriggers::value trigger) const {
	return HLTPrescales->at(trigger);
}

void Event::setPtDensityRho(double rho) {
    ptdensityRho = rho;
}

double Event::rho() const {
    return ptdensityRho;
}

}
