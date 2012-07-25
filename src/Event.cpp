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
bool Event::usePFIsolation = true;

Event::Event() : //
		HLTs(new std::vector<int>()), //
		HLTPrescales(new std::vector<int>()), //
		vertices(), //
		goodVertices(), //
		tracks(), //
		allElectrons(), //
//		goodElectrons(), //
//		goodIsolatedElectrons(), //
//		goodPFIsolatedElectrons(), //
//		goodPFNonIsolatedElectrons(), //
//		looseElectrons(), //
//		qcdElectrons(), //
		allJets(), //
		genJets(), //
//		goodJets(), //
//		goodBJets(), //
//		goodElectronCleanedJets(), //
//		goodElectronCleanedBJets(), //
//		goodMuonCleanedJets(), //
//		goodMuonCleanedBJets(), //
		allMuons(), //
//		goodMuons(), //
//		goodIsolatedMuons(), //
//		goodPFIsolatedMuons(), //
//		looseMuons(), //
		genParticles(), //
		mets_(), //
		genMet_(), //
		dataType(DataType::ElectronHad), //
		runNumber(0), //
		eventNumber(0), //
		localEventNumber(0), //
		lumiBlock(0), //
		eventWeight(1.), //
		pileUpWeight_(1.), //
		PUWeightInTimeOnly_(1.), //
		PUWeight3BX_(1.), //
		PUWeight3D_(1.), //
		PUWeightShiftUp_(1.), //
		PUWeightShiftDown_(1.), //
		numberOfHighPurityTracks_(0), //
		isBeamScraping_(true), //
		genNumberOfPileUpVertices(0), //
		trueNumberOfPileUpVertices_(0), //
		pdfWeights(), //
		ptdensityRho(0.), //
		file_(""), //
		passesCSCTightBeamHaloFilter_(false), //
		passesHBHENoiseFilter_(false), //
		passesHCALLaserFilter_(false), //
		passesECALDeadCellFilter_(false), //
		passesTrackingFailureFilter_(false), //
		passesNoisySCFilter_(false) {
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

//	selectVerticesByQuality();
}

//void Event::selectVerticesByQuality() {
//	goodVertices.clear();
//
//	for (unsigned int i = 0; i < vertices.size(); ++i) {
//		if (vertices.at(i)->isGood())
//			goodVertices.push_back(vertices.at(i));
//	}
//}

void Event::setTracks(TrackCollection tracks) {
	this->tracks.clear();
	this->tracks = tracks;
	numberOfHighPurityTracks_ = 0;
	for (unsigned int index = 0; index < tracks.size(); ++index) {
		if (tracks.at(index)->isHighPurity())
			numberOfHighPurityTracks_++;
	}
}

void Event::setGenParticles(MCParticleCollection genParticles) {
	this->genParticles = genParticles;
}

void Event::setElectrons(ElectronCollection electrons) {
	allElectrons.clear();
	allElectrons = electrons;
//	selectElectronsByQuality();
}

//void Event::selectElectronsByQuality() {
//	goodElectrons.clear();
//	goodIsolatedElectrons.clear();
//	goodPFIsolatedElectrons.clear();
//	goodPFNonIsolatedElectrons.clear();
//	for (unsigned int index = 0; index < allElectrons.size(); ++index) {
//		ElectronPointer electron = allElectrons.at(index);
//
//		bool isGood(electron->isGood((short) Globals::electronID));
//		bool isIsolated = electron->relativeIsolation() < Globals::maxElectronRelativeIsolation;
//		bool isPFIsolated = electron->isPFLepton()
//				&& electron->pfRelativeIsolation(Globals::electronIsolationCone) < Globals::maxElectronPFIsolation;
//		bool isPFNonIsolated = electron->isPFLepton()
//				&& electron->pfRelativeIsolation(Globals::electronIsolationCone) > Globals::maxElectronLoosePFIsolation;
//
//		if (isGood)
//			goodElectrons.push_back(electron);
//
//		if (isGood && isIsolated)
//			goodIsolatedElectrons.push_back(electron);
//
//		if (isGood && isPFIsolated)
//			goodPFIsolatedElectrons.push_back(electron);
//
//		if (isGood && isPFNonIsolated)
//			goodPFNonIsolatedElectrons.push_back(electron);
//
//		if (electron->isLoose())
//			looseElectrons.push_back(electron);
//	}
//}

void Event::setJets(JetCollection jets) {
	allJets.clear();
//	goodJets.clear();
//	goodElectronCleanedJets.clear();
//	goodMuonCleanedJets.clear();

//	goodBJets.clear();
//	goodElectronCleanedBJets.clear();
//	goodMuonCleanedBJets.clear();

	allJets = jets;
//	selectGoodJets();
}

void Event::setGenJets(JetCollection jets) {
	genJets.clear();
	genJets = jets;
}

//void Event::selectGoodJets() {
//
//	for (unsigned int index = 0; index < allJets.size(); ++index) {
//		const JetPointer jet = allJets.at(index);
//		if (jet->isGood()) {
//			goodJets.push_back(jet);
//		}
//	}
//
//	goodBJets = GetBJetCollection(goodJets, Globals::btagAlgorithm, Globals::btagWorkingPoint);
//
//	cleanGoodJets();
//
//	goodElectronCleanedBJets = GetBJetCollection(goodElectronCleanedJets, Globals::btagAlgorithm,
//			Globals::btagWorkingPoint);
//	goodMuonCleanedBJets = GetBJetCollection(goodMuonCleanedJets, Globals::btagAlgorithm, Globals::btagWorkingPoint);
//
//}

JetCollection Event::GetBJetCollection(const JetCollection& jets, BtagAlgorithm::value btagAlgorithm,
		BtagAlgorithm::workingPoint WP) const {
	JetCollection bjets;
	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet = jets.at(index);
		if (jet->isBJet(btagAlgorithm, WP))
			bjets.push_back(jet);
	}

	return bjets;
}

//void Event::cleanGoodJets() {
//
//	if (goodJets.size() > 0) {
//		if (goodIsolatedElectrons.size() > 0 || goodPFIsolatedElectrons.size() > 0) {
//			if (Event::usePFIsolation)
//				goodElectronCleanedJets = cleanGoodJetsAgainstIsolatedElectrons(goodPFIsolatedElectrons);
//			else
//				goodElectronCleanedJets = cleanGoodJetsAgainstIsolatedElectrons(goodIsolatedElectrons);
//		} else if (allElectrons.size() > 0)
//			goodElectronCleanedJets = cleanGoodJetsAgainstMostIsolatedLepton(
//					MostIsolatedElectron(allElectrons, Event::usePFIsolation));
//
//		if (goodIsolatedMuons.size() > 0 || goodPFIsolatedMuons.size() > 0) {
//			if (Event::usePFIsolation)
//				goodMuonCleanedJets = cleanGoodJetsAgainstIsolatedMuons(goodPFIsolatedMuons);
//			else
//				goodMuonCleanedJets = cleanGoodJetsAgainstIsolatedMuons(goodIsolatedMuons);
//		} else if (allMuons.size() > 0) {
//			goodMuonCleanedJets = cleanGoodJetsAgainstMostIsolatedLepton(
//					MostIsolatedMuon(allMuons, Event::usePFIsolation));
//		}
//
//	}
//
//}
//
//JetCollection Event::cleanGoodJetsAgainstIsolatedElectrons(const ElectronCollection& electrons) const {
//	JetCollection cleanedJets;
//	set<unsigned int> cleanedJetsIndices;
//
//	for (unsigned int jetIndex = 0; jetIndex < goodJets.size(); ++jetIndex) {
//		for (unsigned int electronIndex = 0; electronIndex < electrons.size(); ++electronIndex) {
//			if (!goodJets.at(jetIndex)->isWithinDeltaR(0.3, electrons.at(electronIndex))) {
//				cleanedJetsIndices.insert(jetIndex);
//			}
//		}
//	}
//
//	for (set<unsigned int>::iterator jetIndex = cleanedJetsIndices.begin(); jetIndex != cleanedJetsIndices.end();
//			++jetIndex) {
//		cleanedJets.push_back(goodJets.at(*jetIndex));
//	}
//
//	return cleanedJets;
//}
//
//JetCollection Event::cleanGoodJetsAgainstIsolatedMuons(const MuonCollection& muons) const {
//	JetCollection cleanedJets;
//	set<unsigned int> cleanedJetsIndices;
//
//	for (unsigned int jetIndex = 0; jetIndex < goodJets.size(); ++jetIndex) {
//		for (unsigned int muonIndex = 0; muonIndex < muons.size(); ++muonIndex) {
//			if (!goodJets.at(jetIndex)->isWithinDeltaR(0.3, muons.at(muonIndex))) {
//				cleanedJetsIndices.insert(jetIndex);
//			}
//		}
//	}
//
//	for (set<unsigned int>::iterator jetIndex = cleanedJetsIndices.begin(); jetIndex != cleanedJetsIndices.end();
//			++jetIndex) {
//		cleanedJets.push_back(goodJets.at(*jetIndex));
//	}
//
//	return cleanedJets;
//}
//
//JetCollection Event::cleanGoodJetsAgainstMostIsolatedLepton(const LeptonPointer mostIsolatedLepton) const {
//	JetCollection cleanedJets;
//
//	for (unsigned int jetIndex = 0; jetIndex < goodJets.size(); ++jetIndex) {
//		if (!goodJets.at(jetIndex)->isWithinDeltaR(0.3, mostIsolatedLepton)) {
//			cleanedJets.push_back(goodJets.at(jetIndex));
//		}
//	}
//	return cleanedJets;
//}

const ElectronPointer Event::MostIsolatedElectron(const ElectronCollection& electrons, bool usePFIso) const {
	float bestIsolation = 999999999;
	unsigned int bestIsolatedLepton = 990;
	for (unsigned int index = 0; index < electrons.size(); ++index) {
		float currentIsolation = 999999999;
		if (usePFIso)
			currentIsolation = electrons.at(index)->pfRelativeIsolation();
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
		if (usePFIso)
			currentIsolation = muons.at(index)->pfRelativeIsolation();
		else
			currentIsolation = muons.at(index)->relativeIsolation();

		if (currentIsolation < bestIsolation) {
			bestIsolation = currentIsolation;
			bestIsolatedLepton = index;
		}
	}
	return muons.at(bestIsolatedLepton);
}

const ElectronPointer Event::MostIsolatedElectron(const ElectronCollection& electrons) const {
	return MostIsolatedElectron(electrons, false);
}

const ElectronPointer Event::MostPFIsolatedElectron(const ElectronCollection& electrons) const {
	return MostIsolatedElectron(electrons, true);
}

const MuonPointer Event::MostIsolatedMuon(const MuonCollection& muons) const {
	return MostIsolatedMuon(muons, false);
}

const MuonPointer Event::MostPFIsolatedMuon(const MuonCollection& muons) const {
	return MostIsolatedMuon(muons, true);
}

void Event::setMuons(MuonCollection muons) {
	allMuons.clear();
	allMuons = muons;
//	selectMuonsByQuality();
}

//void Event::selectMuonsByQuality() {
//	goodMuons.clear();
//	goodIsolatedMuons.clear();
//	goodPFIsolatedMuons.clear();
//	looseMuons.clear();
//	for (unsigned int index = 0; index < allMuons.size(); ++index) {
//		MuonPointer muon = allMuons.at(index);
//
//		bool isGood(muon->isGood());
//		bool isPFIsolated = muon->isPFLepton() && muon->pfRelativeIsolation() < Globals::maxMuonPFIsolation;
//		if (isGood)
//			goodMuons.push_back(muon);
//
//		if (isGood && muon->relativeIsolation() < Globals::maxMuonRelativeIsolation)
//			goodIsolatedMuons.push_back(muon);
//
//		if (isGood && isPFIsolated)
//			goodPFIsolatedMuons.push_back(muon);
//
//		if (muon->isLoose())
//			looseMuons.push_back(muon);
//
//	}
//}

void Event::setHLTs(const boost::shared_ptr<std::vector<int> > triggers) {
	HLTs = triggers;
}

void Event::setMETs(const std::vector<METPointer> mets) {
	mets_ = mets;
}

void Event::setGenMET(const METPointer met) {
	genMet_ = met;
}

void Event::setFile(string file) {
	file_ = file;
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
	pileUpWeight_ = weight;
}

void Event::setBeamScrapingVeto(bool isScraping) {
	isBeamScraping_ = isScraping;
}

const VertexPointer Event::PrimaryVertex() const {
	return goodVertices.front();
}

const VertexCollection& Event::Vertices() const {
	return vertices;
}

//const VertexCollection& Event::GoodVertices() const {
//	return goodVertices;
//}

const TrackCollection& Event::Tracks() const {
	return tracks;
}

const ElectronCollection& Event::Electrons() const {
	return allElectrons;
}

//const ElectronCollection& Event::GoodElectrons() const {
//	return goodElectrons;
//}
//
//const ElectronCollection& Event::GoodIsolatedElectrons() const {
//	return goodIsolatedElectrons;
//}
//
//const ElectronCollection& Event::GoodPFIsolatedElectrons() const {
//	return goodPFIsolatedElectrons;
//}
//
//const ElectronCollection& Event::GoodPFNonIsolatedElectrons() const {
//	return goodPFNonIsolatedElectrons;
//}
//
//const ElectronCollection& Event::QCDElectrons() const {
//	return qcdElectrons;
//}

const JetCollection& Event::Jets() const {
	return allJets;
}

//const JetCollection& Event::GoodJets() const {
//	return goodJets;
//}

const JetCollection& Event::GenJets() const {
	return genJets;
}

//const JetCollection& Event::GoodElectronCleanedJets() const {
//	return goodElectronCleanedJets;
//}
//
//const JetCollection& Event::GoodMuonCleanedJets() const {
//	return goodMuonCleanedJets;
//}
//
//const JetCollection& Event::GoodBJets() const {
//	return goodBJets;
//}
//
//const JetCollection& Event::GoodElectronCleanedBJets() const {
//	return goodElectronCleanedBJets;
//}
//
//const JetCollection& Event::GoodMuonCleanedBJets() const {
//	return goodMuonCleanedBJets;
//}

const MuonCollection& Event::Muons() const {
	return allMuons;
}

//const MuonCollection& Event::GoodMuons() const {
//	return goodMuons;
//}
//
//const MuonCollection& Event::GoodIsolatedMuons() const {
//	return goodIsolatedMuons;
//}
//
//const MuonCollection& Event::GoodPFIsolatedMuons() const {
//	return goodPFIsolatedMuons;
//}

const MCParticleCollection& Event::GenParticles() const {
	return genParticles;
}

const METPointer Event::MET() const {
	return MET(Globals::metAlgorithm);
}

const METPointer Event::GenMET() const {
	return genMet_;
}

const METPointer Event::MET(METAlgorithm::value type) const {
	unsigned int index(type);
	if (index >= mets_.size())
		throw "Trying to access non-available MET at " + index;

	return mets_.at(index);
}

std::string Event::file() const {
	return file_;
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
	return pileUpWeight_;
}

void Event::inspect() const {
	cout << "run " << runNumber << ", event number " << eventNumber << ", lumi section " << lumiBlock << endl;

	cout << "number of tracks: " << tracks.size() << endl;
	cout << "number of high purity tracks: " << numberOfHighPurityTracks_ << endl;

	cout << "number of jets: " << allJets.size() << endl;
	EventContentPrinter::printJets(allJets);
//	cout << "number of good jets: " << goodJets.size() << endl;
//	EventContentPrinter::printJets(goodJets);
//
//	cout << "number of good isolated electrons: " << goodIsolatedElectrons.size() << endl;
//	EventContentPrinter::printElectrons(goodIsolatedElectrons);
//
//	cout << "number of good electrons: " << goodElectrons.size() << endl;
//	EventContentPrinter::printElectrons(goodElectrons);

	cout << "number of electrons: " << allElectrons.size() << endl;
	EventContentPrinter::printElectrons(allElectrons);
}

bool Event::HLT(HLTriggers::value trigger) const {
	return HLTs->at(trigger) > 0;
}

void Event::setGenNumberOfPileUpVertices(std::vector<int> pileup) {
	genNumberOfPileUpVertices = pileup;
}

void Event::setPDFWeights(std::vector<double> pdfWeights) {
	this->pdfWeights = pdfWeights;
}

const std::vector<double> Event::PDFWeights() const {
	return pdfWeights;
}

//double Event::averageNumberOfGeneratedPileUpVertices() const {
//	double average = std::accumulate(genNumberOfPileUpVertices.begin(), genNumberOfPileUpVertices.end(), 0)/genNumberOfPileUpVertices.size();
//    return average;
//}

const std::vector<int> Event::GeneratedPileUpVertices() const {
	return genNumberOfPileUpVertices;
}

double Event::averageNumberOfVertices() const {
	double average(0);
	average = accumulate(genNumberOfPileUpVertices.begin(), genNumberOfPileUpVertices.end(), 0);
	average = average / genNumberOfPileUpVertices.size();
	return average;
}

double Event::inTimeOnlyNUmberOfVertices() const {
	return genNumberOfPileUpVertices.at(1);
}
//double Event::numberOfGeneratedPileUpVertices(PileUpReweightingMethod::value method) const {
//	using namespace std;
//
//	double numberOfVertices(0);
//	double average(0);
//
//	switch (method) {
//	case PileUpReweightingMethod::averagePileUp:
//		average = accumulate(genNumberOfPileUpVertices.begin(), genNumberOfPileUpVertices.end(), 0);
//		average = average / genNumberOfPileUpVertices.size();
//		numberOfVertices = average;
//		break;
//	case PileUpReweightingMethod::inTimePileUpOnly:
//		numberOfVertices = genNumberOfPileUpVertices.at(1);
//		break;
//	case PileUpReweightingMethod::threeDReweighting:
//		cout << "Pile-up reweighting method '3D-reweighting' not implemented" << endl;
//		return 0;
//	}
//
//	return numberOfVertices;
//}

void Event::setHLTPrescales(const boost::shared_ptr<std::vector<int> > prescales) {
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

unsigned int Event::numberOfHighPurityTracks() const {
	return numberOfHighPurityTracks_;
}

bool Event::isBeamScraping() const {
	return isBeamScraping_;
}

void Event::setTrueNumberOfPileUpVertices(vector<int> pileup) {
	trueNumberOfPileUpVertices_ = pileup;
}

const vector<int>& Event::getTrueNumberOfVertices() const {
	return trueNumberOfPileUpVertices_;
}

void Event::setPUWeightInTimeOnly(double puweight) {
	PUWeightInTimeOnly_ = puweight;
}

double Event::PUWeightInTimeOnly() const {
	return PUWeightInTimeOnly_;
}

void Event::setPUWeight3BX(double puweight) {
	PUWeight3BX_ = puweight;
}

double Event::PUWeight3BX() const {
	return PUWeight3BX_;
}

void Event::setPUWeight3D(double weight) {
	PUWeight3D_ = weight;
}

double Event::PUWeight3D() const {
	return PUWeight3D_;
}

void Event::setPUWeightShiftUp(double weight) {
	PUWeightShiftUp_ = weight;
}

double Event::PUWeightShiftUp() const {
	return PUWeightShiftUp_;
}

void Event::setPUWeightShiftDown(double weight) {
	PUWeightShiftDown_ = weight;
}

double Event::PUWeightShiftDown() const {
	return PUWeightShiftDown_;
}

double Event::getBTagCorrectionFactor(const JetCollection jets) const {
	double correction(1.);
	if (!isRealData()) {

	}

	return correction;
}

void Event::setCSCTightBeamHaloFilter(bool result) {
	passesCSCTightBeamHaloFilter_ = result;
}

void Event::setHBHENoiseFilter(bool result) {
	passesHBHENoiseFilter_ = result;
}

void Event::setHCALLaserFilter(bool result) {
	passesHCALLaserFilter_ = result;
}

void Event::setECALDeadCellFilter(bool result) {
	passesECALDeadCellFilter_ = result;
}

void Event::setTrackingFailureFilter(bool result) {
	passesTrackingFailureFilter_ = result;
}

void Event::setNoisySCFilter(bool result) {
	passesNoisySCFilter_ = result;
}

bool Event::passesCSCTightBeamHaloFilter() const {
	return passesCSCTightBeamHaloFilter_;
}

bool Event::passesHBHENoiseFilter() const {
	return passesHBHENoiseFilter_;
}

bool Event::passesHCALLaserFilter() const {
	return passesHCALLaserFilter_;
}

bool Event::passesECALDeadCellFilter() const {
	return passesECALDeadCellFilter_;
}

bool Event::passesTrackingFailureFilter() const {
	return passesTrackingFailureFilter_;
}

bool Event::passesNoisySCFilter() const {
	return passesNoisySCFilter_;
}

std::vector<double> Event::BjetWeights(const JetCollection bjets) const {
	std::vector<double> bjetWeights;
	unsigned int numberOfBjets(bjets.size());
	for (unsigned int index = 0; index < numberOfBjets; ++index) {
		bjetWeights.push_back(0);
	}

	std::vector<double> SF;

	const boost::array<double, 14> SFb_error = { { 0.0295675, 0.0295095, 0.0210867, 0.0219349, 0.0227033, 0.0204062,
			0.0185857, 0.0256242, 0.0383341, 0.0409675, 0.0420284, 0.0541299, 0.0578761, 0.0655432 } };

	const boost::array<double, 15> ptbins = { { 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500 } };


	for (unsigned int index = 0; index < numberOfBjets; ++index) {
		const JetPointer jet(bjets.at(index));
//		if (jet->isBJet(bjetAlgo, WP) && fabs(jet->eta()) < 2.4) {
		double SFb(0);
		double sf_error(0);
		//these numbers are for CSVM only
		double pt = jet->pt();

		if (pt < 30) {
			SFb = 0.6981 * ((1. + (0.414063 * 30)) / (1. + (0.300155 * 30)));
			sf_error = 0.12;
		} else if (pt > 670) {
			SFb = 0.6981 * ((1. + (0.414063 * 670)) / (1. + (0.300155 * 670)));
			sf_error = 2 * SFb_error[SFb_error.size() - 1];
		} else {
			SFb = 0.6981 * ((1. + (0.414063 * pt)) / (1. + (0.300155 * pt)));
			unsigned int ptbin(0);
			for (unsigned int bin = 0; bin < ptbins.size() + 1; ++bin) {
				double upperCut = index < ptbins.size() ? ptbins.at(index) : 670.;
				double lowerCut = index == 0 ? 0. : ptbins.at(index - 1);

				if (pt > lowerCut && pt < upperCut) {
					ptbin = bin;
					break;
				}
			}
			sf_error = SFb_error.at(ptbin);
		}
		SFb += sf_error * Globals::BJetSystematic;
		SF.push_back(SFb);
//		}
	}

	for (unsigned int index = 0; index < numberOfBjets; ++index) {
		double sf = SF.at(index);
		bjetWeights[0] = index == 0 ? (1 - sf) : bjetWeights[0] * (1 - sf);

		if (index > 0) {
			bjetWeights[1] = index == 1 ? sf : bjetWeights[1] * (1 - sf);
		}
		if (index > 1) {
			bjetWeights[2] = index == 2 ? sf * sf : bjetWeights[2] * (1 - sf);
		}
		if (index > 2) {
			bjetWeights[3] = index == 3 ? sf * sf * sf : bjetWeights[3] * (1 - sf);
		}
		if (index > 3) {
			bjetWeights[4] = index == 4 ? sf * sf * sf * sf : bjetWeights[4] * (1 - sf);
		}
	}
	if(numberOfBjets == 0)
		bjetWeights[0] = 1.;

	return bjetWeights;

}

}
