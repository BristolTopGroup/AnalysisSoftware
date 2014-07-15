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
#include <vector>

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
		allJets(), //
		genJets(), //
		allMuons(), //
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
//		PUWeight3D_(1.), //
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
		passesECALDeadCellTPFilter_(false), //
		passesTrackingFailureFilter_(false), //
		passesEEBadSCFilter_(false), //
		passesECALLaserCorrFilter_(false), //
		passesTrackingPOGFilters_(false) //
{
}

Event::~Event() {
}

bool Event::isRealData() const {
	return dataType == DataType::ElectronHad || dataType == DataType::MuHad || dataType == DataType::SingleElectron
			|| dataType == DataType::SingleMu;
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

}

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
}

void Event::setJets(JetCollection jets) {
	allJets.clear();

	allJets = jets;
}

void Event::setGenJets(JetCollection jets) {
	genJets.clear();
	genJets = jets;
}

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
}

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

const TrackCollection& Event::Tracks() const {
	return tracks;
}

const ElectronCollection& Event::Electrons() const {
	return allElectrons;
}

const JetCollection& Event::Jets() const {
	return allJets;
}

const JetCollection& Event::GenJets() const {
	return genJets;
}

const MuonCollection& Event::Muons() const {
	return allMuons;
}

const MCParticleCollection& Event::GenParticles() const {
	return genParticles;
}

const METPointer Event::MET() const {
	return MET(Globals::metAlgorithm);
}

const METPointer Event::GenMET() const {
	return MET(METAlgorithm::GenMET);
//	return genMet_;
}

const METPointer Event::MET(METAlgorithm::value type) const {
	unsigned int index(type);
	if (index >= mets_.size())
		throw "Trying to access non-available MET at " + std::to_string(index);

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

	cout << "number of electrons: " << allElectrons.size() << endl;
	EventContentPrinter::printElectrons(allElectrons);
}

bool Event::HLT(HLTriggers::value trigger) const {
	if (trigger >= HLTs->size())
		return false;

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

const std::vector<int> Event::GeneratedPileUpVertices() const {
	return genNumberOfPileUpVertices;
}

double Event::averageNumberOfVertices() const {
	double average(0);
	average = accumulate(genNumberOfPileUpVertices.begin(), genNumberOfPileUpVertices.end(), 0);
	average = average / genNumberOfPileUpVertices.size();
	return average;
}

double Event::inTimeOnlyNumberOfVertices() const {
	return genNumberOfPileUpVertices.at(1);
}

void Event::setHLTPrescales(const boost::shared_ptr<std::vector<int> > prescales) {
	HLTPrescales = prescales;
}

int Event::HLTPrescale(HLTriggers::value trigger) const {
	if (trigger >= HLTPrescales->size())
		return 0;
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

//void Event::setPUWeight3D(double weight) {
//	PUWeight3D_ = weight;
//}

//double Event::PUWeight3D() const {
//	return PUWeight3D_;
//}

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

//double Event::getBTagCorrectionFactor(const JetCollection jets) const {
//	double correction(1.);
//	if (!isRealData()) {
//
//	}
//
//	return correction;
//}

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

void Event::setECALDeadCellTPFilter(bool result) {
	passesECALDeadCellTPFilter_ = result;
}

void Event::setTrackingFailureFilter(bool result) {
	passesTrackingFailureFilter_ = result;
}

void Event::setEEBadSCFilter(bool result) {
	passesEEBadSCFilter_ = result;
}

void Event::setECALLaserCorrFilter(bool result) {
	passesECALLaserCorrFilter_ = result;
}

void Event::setTrackingPOGFilters(bool result) {
	passesTrackingPOGFilters_ = result;
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

bool Event::passesECALDeadCellTPFilter() const {
	return passesECALDeadCellTPFilter_;
}

bool Event::passesTrackingFailureFilter() const {
	return passesTrackingFailureFilter_;
}

bool Event::passesEEBadSCFilter() const {
	return passesEEBadSCFilter_;
}

bool Event::passesECALLaserCorrFilter() const {
	return passesECALLaserCorrFilter_;
}

bool Event::passesTrackingPOGFilters() const {
	return passesTrackingPOGFilters_;
}

double Event::HT(const JetCollection jets) {
	double ht(0);
	//Take ALL the jets!
	for (unsigned int index = 0; index < jets.size(); ++index) {
		if(jets.at(index)->pt() > 20)
			ht += jets.at(index)->pt();
	}
	return ht;
}

double Event::ST(const JetCollection jets, const ParticlePointer lepton, const METPointer met) {
	// ST = HT + MET + lepton pt
	double ht = Event::HT(jets);
	return ht + met->et() + lepton->pt();
}

double Event::MT(const ParticlePointer particle, const METPointer met) {
	double energySquared = pow(particle->et() + met->et(), 2);
	double momentumSquared = pow(particle->px() + met->px(), 2) + pow(particle->py() + met->py(), 2);
	double MTSquared = energySquared - momentumSquared;

	if (MTSquared > 0)
		return sqrt(MTSquared);
	else
		return -1;
}

double Event::WPT(const ParticlePointer particle, const METPointer met) {
	ParticlePointer W_boson;
	W_boson = ParticlePointer(new Particle(*met + *particle));

	return W_boson->pt();
}

double Event::M3(const JetCollection jets) {
	double m3(0), max_pt(0);
	if (jets.size() >= 3) {
		for (unsigned int index1 = 0; index1 < jets.size() - 2; ++index1) {
			for (unsigned int index2 = index1 + 1; index2 < jets.size() - 1;
					++index2) {
				for (unsigned int index3 = index2 + 1; index3 < jets.size();
						++index3) {
					FourVector m3Vector(
							jets.at(index1)->getFourVector()
									+ jets.at(index2)->getFourVector()
									+ jets.at(index3)->getFourVector());
					double currentPt = m3Vector.Pt();
					if (currentPt > max_pt) {
						max_pt = currentPt;
						m3 = m3Vector.M();
					}
				}
			}
		}
	}

	return m3;
}

double Event::M_bl(const JetCollection b_jets, const ParticlePointer lepton) {
	double m_bl(0.);
	if (b_jets.size() != 0) {
		// store the jets as particle pointers
		ParticleCollection particles;
		for (unsigned int i = 0; i < b_jets.size(); ++i)
			particles.push_back(b_jets.at(i));
		// find closest b_jet
		unsigned short closest_b_jet_index = lepton->getClosest(particles);
		JetPointer closest_b_jet = b_jets.at(closest_b_jet_index);
		ParticlePointer pseudo_particle(new Particle(*closest_b_jet + *lepton));
		m_bl = pseudo_particle->mass();
	}
	return m_bl;
}

double Event::angle_bl(const JetCollection b_jets,
		const ParticlePointer lepton) {
	double angle(-1);
	if (b_jets.size() != 0) {
		// store the jets as particle pointers
		ParticleCollection particles;
		for (unsigned int i = 0; i < b_jets.size(); ++i)
			particles.push_back(b_jets.at(i));
		// find closest b_jet
		unsigned short closest_b_jet_index = lepton->getClosest(particles);
		JetPointer closest_b_jet = b_jets.at(closest_b_jet_index);
		angle = lepton->angle(closest_b_jet);
	}
	return angle;
}


}
