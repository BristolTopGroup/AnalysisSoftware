

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

double const Event::minJetPt_ = 30;
double const Event::maxJetAbsEta_ = 2.4;
unsigned int const Event::minNJets_ = 4;
unsigned int const Event::minNBJets_ = 2;

double const Event::minSignalMuonPt_ = 26;
double const Event::minSignalMuonEta_ = 2.4;
double const Event::minSignalElectronPt_ = 34;
double const Event::minSignalElectronEta_ = 2.1; 

Event::Event() : //
		HLTs(new std::vector<int>()), //
		HLTPrescales(new std::vector<int>()), //
		vertices(), //
		goodVertices(), //
		tracks(), //
		allElectrons(), //
		allJets(), //
		cleanedJets_(), //
		cleanedBJets_(),
		genJets(), //
		allMuons(), //
		passesElectronSelection_(false),
		passesElectronQCDSelection_(false),
		passesElectronConversionSelection_(false),
		passesMuonSelection_(false),
		passesMuonQCDSelection1p5to3_(false),
		passesMuonQCDSelection3toInf_(false),
		passesElectronSelectionNoB_(false),
		passesElectronQCDSelectionNoB_(false),
		passesElectronConversionSelectionNoB_(false),
		passesMuonSelectionNoB_(false),
		passesMuonQCDSelection1p5to3NoB_(false),
		passesMuonQCDSelection3toInfNoB_(false),
		passesMETFilters_(true),
		isSemiLeptonicElectron_(false),
		isSemiLeptonicMuon_(false),
		selectionOutputInfo_electron(),
		selectionOutputInfo_muon(),
		genParticles(), //
		mets_(), //
		genMet_(), //
		ttbarHypothesis_(), //
		dataType(DataType::ElectronHad), //
		runNumber(0), //
		eventNumber(0), //
		localEventNumber(0), //
		lumiBlock(0), //
		eventWeight(1.), //
		pileUpWeight_(1.), //
		pileUpWeight_up_(1.), //
		pileUpWeight_down_(1.), //
		bJetWeight_(1.), //
		lightJetUpWeight_(1.), //
		lightJetDownWeight_(1.), //
		bJetUpWeight_(1.), //
		bJetDownWeight_(1.), //
		PUWeightInTimeOnly_(1.), //
		PUWeight3BX_(1.), //
//		PUWeight3D_(1.), //
		PUWeightShiftUp_(1.), //
		PUWeightShiftDown_(1.), //
		numberOfHighPurityTracks_(0), //
		isBeamScraping_(true), //
		genNumberOfPileUpVertices(0), //
		trueNumberOfPileUpVertices_(0), //
		generatorWeight_(1), //
		generatorSystematicWeights_(), //
		generatorSystematicWeightsID_(), //
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
	return  dataType == DataType::SingleElectron_RunB
	|| dataType == DataType::SingleElectron_RunC
	|| dataType == DataType::SingleElectron_RunD
	|| dataType == DataType::SingleElectron_RunE
	|| dataType == DataType::SingleElectron_RunF
	|| dataType == DataType::SingleElectron_RunG
	|| dataType == DataType::SingleElectron_RunH_Prompt_v2
	|| dataType == DataType::SingleElectron_RunH_Prompt_v3
	|| dataType == DataType::SingleMuon_RunB
	|| dataType == DataType::SingleMuon_RunC
	|| dataType == DataType::SingleMuon_RunD
	|| dataType == DataType::SingleMuon_RunE
	|| dataType == DataType::SingleMuon_RunF
	|| dataType == DataType::SingleMuon_RunG
	|| dataType == DataType::SingleMuon_RunH_Prompt_v2
	|| dataType == DataType::SingleMuon_RunH_Prompt_v3;
}

const DataType::value Event::getDataType() const {
	return dataType;
}

bool Event::isTTJet( DataType::value type) const {
	if (
		type == DataType::TTJets_amcatnloFXFX ||
		type == DataType::TTJets_madgraphMLM ||
    	type == DataType::TTJets_PowhegPythia8 ||
    	type == DataType::TTJets_PowhegPythia8_fsrdown ||
    	type == DataType::TTJets_PowhegPythia8_fsrup ||
    	type == DataType::TTJets_PowhegPythia8_isrdown ||
    	type == DataType::TTJets_PowhegPythia8_isrup ||
    	type == DataType::TTJets_PowhegPythia8_down ||
    	type == DataType::TTJets_PowhegPythia8_up ||
    	type == DataType::TTJets_PowhegPythia8_mtop1695 ||
    	type == DataType::TTJets_PowhegPythia8_mtop1755 ||
    	type == DataType::TTJets_madgraphMLM ||
		type == DataType::TTJets_PowhegHerwigpp ||
		type == DataType::TTJets_amcatnloHerwigpp
		// type == DataType::TTJets_synch
		)
		return true;
	else
		return false;
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

void Event::setJetTTBarPartons() {
	// Loop over all jets, and set the parton if it matches to one from ttbar decay
	const TTGenInfoPointer ttGen( this->TTGenInfo() );
	ParticlePointer quark = ttGen->getQuark();
	ParticlePointer quarkBar = ttGen->getQuarkBar();
	ParticlePointer hadronicB = ttGen->gethadronicB();
	ParticlePointer leptonicB = ttGen->getleptonicB();

	// Only consider if there are partons from top decay
	if ( quark == 0 || quarkBar == 0 ) return;

	for ( unsigned int jetIndex = 0; jetIndex < allJets.size(); ++jetIndex ) {
		JetPointer jet = allJets[jetIndex];

		// Skip if jet doesn't have a matched parton
		if ( jet->matched_parton() == 0 ) continue;

		FourVector partonFV = jet->matched_parton()->getFourVector();

		if ( partonFV == quark->getFourVector() ) jet->set_ttbar_decay_parton( TTPartons::partonType::Quark );
		else if ( partonFV == quarkBar->getFourVector() ) jet->set_ttbar_decay_parton( TTPartons::partonType::QuarkBar );
		else if ( partonFV == hadronicB->getFourVector() ) jet->set_ttbar_decay_parton( TTPartons::partonType::HadB );
		else if ( partonFV == leptonicB->getFourVector() ) jet->set_ttbar_decay_parton( TTPartons::partonType::LepB );
	}
}

void Event::setCleanedJets(JetCollection jets) {
	cleanedJets_.clear();

	cleanedJets_ = jets;
}

void Event::setCleanedBJets(JetCollection bjets) {
	cleanedBJets_.clear();

	cleanedBJets_ = bjets;
}

void Event::setGenJets(JetCollection jets) {
	genJets.clear();
	genJets = jets;
}

const bool Event::PassesElectronChannelTrigger() const {
	if ( passesElectronChannelTrigger_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonChannelTrigger() const {
	if ( passesMuonChannelTrigger_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesTkMuonChannelTrigger() const {
	if ( passesTkMuonChannelTrigger_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesElectronSelection() const {
	if ( passesElectronSelection_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesElectronQCDSelection() const {
	if ( passesElectronQCDSelection_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesElectronConversionSelection() const {
	if ( passesElectronConversionSelection_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonSelection() const {
	if ( passesMuonSelection_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonQCDSelection1p5to3() const {
	if ( passesMuonQCDSelection1p5to3_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonQCDSelection3toInf() const {
	if ( passesMuonQCDSelection3toInf_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesElectronTriggerAndSelection() const {
	if ( passesElectronSelection_ && passesElectronChannelTrigger_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesElectronTriggerAndQCDSelection() const {
	if ( passesElectronQCDSelection_ && passesElectronChannelQCDTrigger_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesElectronTriggerAndConversionSelection() const {
	if ( passesElectronConversionSelection_ && passesElectronChannelQCDTrigger_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonTriggerAndSelection() const {
	if ( passesMuonSelection_ && (passesMuonChannelTrigger_ || passesTkMuonChannelTrigger_) ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonTriggerAndQCDSelection1p5to3() const {
	if ( passesMuonQCDSelection1p5to3_ && passesMuonChannelQCDTrigger_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonTriggerAndQCDSelection3toInf() const {
	if ( passesMuonQCDSelection3toInf_ && passesMuonChannelQCDTrigger_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesElectronSelectionNoB() const {
	if ( passesElectronSelectionNoB_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesElectronQCDSelectionNoB() const {
	if ( passesElectronQCDSelectionNoB_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesElectronConversionSelectionNoB() const {
	if ( passesElectronConversionSelectionNoB_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonSelectionNoB() const {
	if ( passesMuonSelectionNoB_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonQCDSelection1p5to3NoB() const {
	if ( passesMuonQCDSelection1p5to3NoB_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonQCDSelection3toInfNoB() const {
	if ( passesMuonQCDSelection3toInfNoB_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesElectronTriggerAndSelectionNoB() const {
	if ( passesElectronSelectionNoB_ && passesElectronChannelTrigger_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesElectronTriggerAndQCDSelectionNoB() const {
	if ( passesElectronQCDSelectionNoB_ && passesElectronChannelTrigger_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesElectronTriggerAndConversionSelectionNoB() const {
	if ( passesElectronConversionSelectionNoB_ && passesElectronChannelTrigger_ ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonTriggerAndSelectionNoB() const {
	if ( passesMuonSelectionNoB_ && (passesMuonChannelTrigger_ || passesTkMuonChannelTrigger_) ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonTriggerAndQCDSelection1p5to3NoB() const {
	if ( passesMuonQCDSelection1p5to3NoB_ && (passesMuonChannelTrigger_ || passesTkMuonChannelTrigger_) ) {
		return true;
	}

	return false;
}

const bool Event::PassesMuonTriggerAndQCDSelection3toInfNoB() const {
	if ( passesMuonQCDSelection3toInfNoB_ && (passesMuonChannelTrigger_ || passesTkMuonChannelTrigger_) ) {
		return true;
	}

	return false;
}

const bool Event::isSemiLeptonicElectron() const {
	if ( isSemiLeptonicElectron_ ) {
		return true;
	}

	return false;
}

const bool Event::isSemiLeptonicMuon() const {
	if ( isSemiLeptonicMuon_ ) {
		return true;
	}

	return false;
}

const TTGenInfoPointer Event::TTGenInfo() const {
	return ttGenInfo_;
}

const PseudoTopParticlesPointer Event::PseudoTopParticles() const {
	return pseudoTopParticles_;
}

const LeptonPointer Event::getSignalLepton( unsigned int selectionCriteria ) const {
	SelectionCriteria::selection selection = SelectionCriteria::selection(selectionCriteria);
	if ( selection == SelectionCriteria::ElectronPlusJetsReference ) {
		unsigned int signalLeptonIndex = selectionOutputInfo_electron.getSignalLeptonIndex();
		return allElectrons[signalLeptonIndex];
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsReference ) {
		unsigned int signalLeptonIndex = selectionOutputInfo_muon.getSignalLeptonIndex();
		return allMuons[signalLeptonIndex];
	}
	else if ( selection == SelectionCriteria::ElectronPlusJetsQCDNonIsolated ) {
		unsigned int signalLeptonIndex = selectionOutputInfo_electronQCDNonisolated.getSignalLeptonIndex();
		return allElectrons[signalLeptonIndex];
	}
	else if ( selection == SelectionCriteria::ElectronPlusJetsQCDConversion ) {
		unsigned int signalLeptonIndex = selectionOutputInfo_electronQCDConversion.getSignalLeptonIndex();
		return allElectrons[signalLeptonIndex];
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated1p5to3 ) {
		unsigned int signalLeptonIndex = selectionOutputInfo_muonQCDNonisolated1p5to3.getSignalLeptonIndex();
		return allMuons[signalLeptonIndex];
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated3toInf ) {
		unsigned int signalLeptonIndex = selectionOutputInfo_muonQCDNonisolated3toInf.getSignalLeptonIndex();
		return allMuons[signalLeptonIndex];
	}

	return LeptonPointer();
}

const JetCollection Event::getCleanedJets( unsigned int selectionCriteria ) const{
	SelectionCriteria::selection selection = SelectionCriteria::selection(selectionCriteria);

	std::vector<unsigned int> cleanedJetIndices;

	if ( selection == SelectionCriteria::ElectronPlusJetsReference ) {
		cleanedJetIndices = selectionOutputInfo_electron.getCleanedJetIndex();
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsReference ) {
		cleanedJetIndices = selectionOutputInfo_muon.getCleanedJetIndex();
	}
	else if ( selection == SelectionCriteria::ElectronPlusJetsQCDNonIsolated ) {
		cleanedJetIndices = selectionOutputInfo_electronQCDNonisolated.getCleanedJetIndex();
	}
	else if ( selection == SelectionCriteria::ElectronPlusJetsQCDConversion ) {
		cleanedJetIndices = selectionOutputInfo_electronQCDConversion.getCleanedJetIndex();
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated1p5to3 ) {
		cleanedJetIndices = selectionOutputInfo_muonQCDNonisolated1p5to3.getCleanedJetIndex();
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated3toInf ) {
		cleanedJetIndices = selectionOutputInfo_muonQCDNonisolated3toInf.getCleanedJetIndex();
	}

	JetCollection cleanedJets;
	for ( unsigned int cleanedJetIndex = 0; cleanedJetIndex < cleanedJetIndices.size(); ++cleanedJetIndex ) {
		unsigned int jetIndex = cleanedJetIndices[ cleanedJetIndex ];
		if ( allJets[jetIndex]->pt() >= minJetPt_ && fabs(allJets[jetIndex]->eta()) <= maxJetAbsEta_ ) {
			cleanedJets.push_back( allJets[jetIndex] );
		}
	}
	return cleanedJets;
}

const JetCollection Event::getCleanedBJets( unsigned int selectionCriteria ) const{

	SelectionCriteria::selection selection = SelectionCriteria::selection(selectionCriteria);

	// Note these are indices in the cleaned jets collection
	// not from all jets
	std::vector<unsigned int> cleanedBJetIndices;
	std::vector<unsigned int> cleanedJetIndices;

	if ( selection == SelectionCriteria::ElectronPlusJetsReference ) {
		cleanedBJetIndices = selectionOutputInfo_electron.getCleanedBJetIndex();
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsReference ) {
		cleanedBJetIndices = selectionOutputInfo_muon.getCleanedBJetIndex();
	}
	else if ( selection == SelectionCriteria::ElectronPlusJetsQCDNonIsolated ) {
		cleanedBJetIndices = selectionOutputInfo_electronQCDNonisolated.getCleanedBJetIndex();
	}
	else if ( selection == SelectionCriteria::ElectronPlusJetsQCDConversion ) {
		cleanedBJetIndices = selectionOutputInfo_electronQCDConversion.getCleanedBJetIndex();
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated1p5to3 ) {
		cleanedBJetIndices = selectionOutputInfo_muonQCDNonisolated1p5to3.getCleanedBJetIndex();
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated3toInf ) {
		cleanedBJetIndices = selectionOutputInfo_muonQCDNonisolated3toInf.getCleanedBJetIndex();
	}

	JetCollection cleanedBJets;
	for ( unsigned int cleanedBJetIndex = 0; cleanedBJetIndex < cleanedBJetIndices.size(); ++cleanedBJetIndex ) {
		double jetIndex = cleanedBJetIndices[cleanedBJetIndex];
		if ( allJets[jetIndex]->pt() >= minJetPt_ && fabs(allJets[jetIndex]->eta()) <= maxJetAbsEta_ ) {
			cleanedBJets.push_back( allJets[jetIndex] );
		}
	}
	return cleanedBJets;
}

const unsigned int Event::getNBJets( unsigned int selectionCriteria ) const {

	SelectionCriteria::selection selection = SelectionCriteria::selection(selectionCriteria);

	if ( selection == SelectionCriteria::ElectronPlusJetsReference ) {
		return selectionOutputInfo_electron.getNumberOfBJets();
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsReference ) {
		return selectionOutputInfo_muon.getNumberOfBJets();
	}
	else if ( selection == SelectionCriteria::ElectronPlusJetsQCDNonIsolated ) {
		return selectionOutputInfo_electronQCDNonisolated.getNumberOfBJets();
	}
	else if ( selection == SelectionCriteria::ElectronPlusJetsQCDConversion ) {
		return selectionOutputInfo_electronQCDConversion.getNumberOfBJets();
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated1p5to3 ) {
		return selectionOutputInfo_muonQCDNonisolated1p5to3.getNumberOfBJets();
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated3toInf ) {
		return selectionOutputInfo_muonQCDNonisolated3toInf.getNumberOfBJets();
	}
	return 0;
}


const std::vector<bool> Event::getCuts( unsigned int selectionCriteria ) const {

	std::vector<bool> CutSequence;
	SelectionCriteria::selection selection = SelectionCriteria::selection(selectionCriteria);
	if ( selection == SelectionCriteria::ElectronPlusJetsReference ) {
		CutSequence.push_back(selectionOutputInfo_electron.getAllEvents());
		CutSequence.push_back(selectionOutputInfo_electron.getLooseElectronVeto());
		CutSequence.push_back(selectionOutputInfo_electron.getLooseMuonVeto());
		CutSequence.push_back(selectionOutputInfo_electron.getConversionVeto());
		CutSequence.push_back(selectionOutputInfo_electron.getAtLeastOneGoodJet());
		CutSequence.push_back(selectionOutputInfo_electron.getAtLeastTwoGoodJets());
		CutSequence.push_back(selectionOutputInfo_electron.getAtLeastThreeGoodJets());
		CutSequence.push_back(selectionOutputInfo_electron.getAtLeastFourGoodJets());
		CutSequence.push_back(selectionOutputInfo_electron.getAtLeastOneBTag());
		CutSequence.push_back(selectionOutputInfo_electron.getAtLeastTwoBTags());

	}

	else if ( selection == SelectionCriteria::MuonPlusJetsReference ) {
		CutSequence.push_back(selectionOutputInfo_muon.getAllEvents());
		CutSequence.push_back(selectionOutputInfo_muon.getLooseElectronVeto());
		CutSequence.push_back(selectionOutputInfo_muon.getLooseMuonVeto());
		CutSequence.push_back(0);
		CutSequence.push_back(selectionOutputInfo_muon.getAtLeastOneGoodJet());
		CutSequence.push_back(selectionOutputInfo_muon.getAtLeastTwoGoodJets());
		CutSequence.push_back(selectionOutputInfo_muon.getAtLeastThreeGoodJets());
		CutSequence.push_back(selectionOutputInfo_muon.getAtLeastFourGoodJets());
		CutSequence.push_back(selectionOutputInfo_muon.getAtLeastOneBTag());
		CutSequence.push_back(selectionOutputInfo_muon.getAtLeastTwoBTags());
	}
	return CutSequence;
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

void Event::setMuons(MuonCollection muons) {
	allMuons.clear();
	allMuons = muons;
}

void Event::setPassesElectronChannelTrigger( bool passesTrigger ) {
	passesElectronChannelTrigger_ = passesTrigger;
}

void Event::setPassesElectronChannelQCDTrigger( bool passesTrigger ) {
	passesElectronChannelQCDTrigger_ = passesTrigger;
}

void Event::setPassesMuonChannelTrigger( bool passesTrigger ) {
	passesMuonChannelTrigger_ = passesTrigger;
}

void Event::setPassesTkMuonChannelTrigger( bool passesTrigger ) {
	passesTkMuonChannelTrigger_ = passesTrigger;
}

void Event::setPassesMuonChannelQCDTrigger( bool passesTrigger ) {
	passesMuonChannelQCDTrigger_ = passesTrigger;
}

void Event::setPassesElectronSelection( bool passesElectronSelection ) {
	passesElectronSelection_ = passesElectronSelection;
}

void Event::setPassesElectronQCDSelection( bool passesElectronQCDSelection ) {
	passesElectronQCDSelection_ = passesElectronQCDSelection;
}

void Event::setPassesElectronConversionSelection( bool passesElectronConversionSelection ) {
	passesElectronConversionSelection_ = passesElectronConversionSelection;
}

void Event::setPassesMuonSelection( bool passesMuonSelection ) {
	passesMuonSelection_ = passesMuonSelection;
}

void Event::setPassesMuonQCDSelection1p5to3( bool passesMuonQCDSelection1p5to3 ) {
	passesMuonQCDSelection1p5to3_ = passesMuonQCDSelection1p5to3;
}

void Event::setPassesMuonQCDSelection3toInf( bool passesMuonQCDSelection3toInf ) {
	passesMuonQCDSelection3toInf_ = passesMuonQCDSelection3toInf;
}

void Event::setPassesElectronSelectionNoB( bool passesElectronSelectionNoB ) {
	passesElectronSelectionNoB_ = passesElectronSelectionNoB;
}

void Event::setPassesElectronQCDSelectionNoB( bool passesElectronQCDSelectionNoB ) {
	passesElectronQCDSelectionNoB_ = passesElectronQCDSelectionNoB;
}

void Event::setPassesElectronConversionSelectionNoB( bool passesElectronConversionSelectionNoB ) {
	passesElectronConversionSelectionNoB_ = passesElectronConversionSelectionNoB;
}

void Event::setPassesMuonSelectionNoB( bool passesMuonSelectionNoB ) {
	passesMuonSelectionNoB_ = passesMuonSelectionNoB;
}

void Event::setPassesMuonQCDSelection1p5to3NoB( bool passesMuonQCDSelection1p5to3NoB ) {
	passesMuonQCDSelection1p5to3NoB_ = passesMuonQCDSelection1p5to3NoB;
}

void Event::setPassesMuonQCDSelection3toInfNoB( bool passesMuonQCDSelection3toInfNoB ) {
	passesMuonQCDSelection3toInfNoB_ = passesMuonQCDSelection3toInfNoB;
}

void Event::setPassOfflineSelectionInfo( std::vector<unsigned int> passSelections ) {
	// if ( passSelections.size() > 1 ) {
	// 	for ( unsigned int selection = 0; selection < passSelections.size(); ++selection ) {
	// 		if ( passSelections[selection] != 2 && passSelections[selection] != 4 )
	// 			cout << selection << " " << passSelections[selection] << endl;
	// 	}
	// }

	for ( unsigned int selection = 0; selection < passSelections.size(); ++selection ) {
		SelectionCriteria::selection selectionCriteria = SelectionCriteria::selection(passSelections[selection]);

		if ( passSelections[selection] == 1 && passesJetSelection( selectionCriteria ) && passesSignalLeptonSelection( selectionCriteria ) ){
			setPassesMuonSelectionNoB( true );
			if ( passesBJetSelection( selectionCriteria ) ) setPassesMuonSelection( true );
		}
		if ( passSelections[selection] == 2 && passesJetSelection( selectionCriteria ) && passesSignalLeptonSelection( selectionCriteria ) ){
			setPassesElectronSelectionNoB( true );
			if ( passesBJetSelection( selectionCriteria ) ) setPassesElectronSelection( true );
		}
		if ( passSelections[selection] == 3 && passesJetSelection( selectionCriteria ) && passesSignalLeptonSelection( selectionCriteria ) ){
			setPassesMuonQCDSelection1p5to3NoB( true );
			if ( passesBJetSelection( selectionCriteria ) ) setPassesMuonQCDSelection1p5to3( true );
		}
		if ( passSelections[selection] == 4 && passesJetSelection( selectionCriteria ) && passesSignalLeptonSelection( selectionCriteria ) ){
			setPassesMuonQCDSelection3toInfNoB( true );
			if ( passesBJetSelection( selectionCriteria ) ) setPassesMuonQCDSelection3toInf( true );
		}
		if ( passSelections[selection] == 5 && passesJetSelection( selectionCriteria ) && passesSignalLeptonSelection( selectionCriteria ) ){
			setPassesElectronQCDSelectionNoB( true );
			if ( passesBJetSelection( selectionCriteria ) ) setPassesElectronQCDSelection( true );
		}
		if ( passSelections[selection] == 6 && passesJetSelection( selectionCriteria ) && passesSignalLeptonSelection( selectionCriteria ) ){
			setPassesElectronConversionSelectionNoB( true );
			if ( passesBJetSelection( selectionCriteria ) ) setPassesElectronConversionSelection( true );
		}
	}
}

const bool Event::passesJetSelection( const unsigned int selection ) { 

	const JetCollection jets = getCleanedJets( selection );
	unsigned int nJetPass = 0;
	for ( unsigned int jetIndex = 0; jetIndex < jets.size(); ++jetIndex ) {
		const JetPointer jet = jets.at(jetIndex);
		if ( jet->pt() >= minJetPt_ ) ++nJetPass;
	}

		if ( nJetPass < minNJets_ ) return false;

	return true;
}

const bool Event::passesBJetSelection( const unsigned int selection ) { 

	SelectionCriteria::selection selectionCriteria = SelectionCriteria::selection(selection);

	const JetCollection bjets = getCleanedBJets( selection );
	unsigned int nBJetPass = 0;
	for ( unsigned int bJetIndex = 0; bJetIndex < bjets.size(); ++bJetIndex ) {
		const JetPointer bjet = bjets.at(bJetIndex);
		if ( bjet->pt() >= minJetPt_ ) ++nBJetPass;
	}

	if ( selectionCriteria == SelectionCriteria::ElectronPlusJetsReference ||
		 selectionCriteria == SelectionCriteria::MuonPlusJetsReference ) {
		if ( nBJetPass < minNBJets_ ) return false;		
	}
	else {
		if ( nBJetPass != 0 ) return false;
	}
	return true;

}

const bool Event::passesSignalLeptonSelection( const unsigned int selectionCriteria ) {

	SelectionCriteria::selection selection = SelectionCriteria::selection(selectionCriteria);

	LeptonPointer signalLepton = getSignalLepton( selectionCriteria );
	if ( signalLepton == 0 ) return false;
	double ptThreshold = 99999999;
	double etaThreshold = 99999999;

	if ( selection == SelectionCriteria::ElectronPlusJetsReference ||
			selection == SelectionCriteria::ElectronPlusJetsQCDNonIsolated ||
			selection == SelectionCriteria::ElectronPlusJetsQCDConversion
	 ) {
		ptThreshold = minSignalElectronPt_;
		etaThreshold = minSignalElectronEta_;

		// Selection on electron d0 and dz not in VID selection, so apply here.
		if ( signalLepton->isInBarrelRegion() ) {
			if ( signalLepton->d0() > 0.05 ) return false;
		}
		else {
			if ( signalLepton->d0() > 0.1 ) return false;
		}
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsReference ||
		 		selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated1p5to3 ||
		 		selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated3toInf		 		
	) {
		ptThreshold = minSignalMuonPt_;
		etaThreshold = minSignalMuonEta_;
	}

	if ( (signalLepton->pt() > ptThreshold) && ( abs(signalLepton->eta()) < etaThreshold) ) return true;
	else return false;
}

void Event::setIsSemiLeptonicElectron( bool isSemiLeptonicElectron ) {
	isSemiLeptonicElectron_ = isSemiLeptonicElectron;
}

void Event::setIsSemiLeptonicMuon( bool isSemiLeptonicMuon ) {
	isSemiLeptonicMuon_ = isSemiLeptonicMuon;
}

void Event::setPassGenSelectionInfo( std::vector<unsigned int> passSelections ) {
	for ( unsigned int selection = 0; selection < passSelections.size(); ++selection ) {
		if ( passSelections[selection] == 1 ) setIsSemiLeptonicMuon( true );
		if ( passSelections[selection] == 2 ) setIsSemiLeptonicElectron( true );
	}
}

void Event::setElectronSelectionOutputInfo(SelectionOutputInfo newSelectionOutputInfo) {
	selectionOutputInfo_electron = newSelectionOutputInfo;
}

void Event::setMuonSelectionOutputInfo(SelectionOutputInfo newSelectionOutputInfo) {
	selectionOutputInfo_muon = newSelectionOutputInfo;
}

void Event::setElectronQCDNonisolatedSelectionOutputInfo(SelectionOutputInfo newSelectionOutputInfo) {
	selectionOutputInfo_electronQCDNonisolated = newSelectionOutputInfo;
}

void Event::setElectronConversionSelectionOutputInfo(SelectionOutputInfo newSelectionOutputInfo) {
	selectionOutputInfo_electronQCDConversion = newSelectionOutputInfo;
}

void Event::setMuonQCDNonisolatedSelection1p5to3OutputInfo(SelectionOutputInfo newSelectionOutputInfo) {
	selectionOutputInfo_muonQCDNonisolated1p5to3 = newSelectionOutputInfo;
}

void Event::setMuonQCDNonisolatedSelection3toInfOutputInfo(SelectionOutputInfo newSelectionOutputInfo) {
	selectionOutputInfo_muonQCDNonisolated3toInf = newSelectionOutputInfo;
}

void Event::setTTGenInfo( TTGenInfoPointer ttGenInfo ){
	ttGenInfo_ = ttGenInfo;
}

void Event::setPseudoTopParticles( PseudoTopParticlesPointer newPseudoParticles  ){
	pseudoTopParticles_ = newPseudoParticles;
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

void Event::setTTbarHypothesis(const TtbarHypothesis newHypo) {
	ttbarHypothesis_ = newHypo;
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

void Event::setPileUpWeight(double weight, int systematic ) {
	if ( systematic == 0 ) {
		pileUpWeight_ = weight;
	}
	else if ( systematic == -1 ) {
		pileUpWeight_down_ = weight;
	}
	else if ( systematic == 1 ) {
		pileUpWeight_up_ = weight;
	}
}

void Event::setBJetWeight(double weight) {
	bJetWeight_ = weight;
}

void Event::setLightJetUpWeight(double weight) {
	lightJetUpWeight_ = weight;
}

void Event::setLightJetDownWeight(double weight) {
	lightJetDownWeight_ = weight;
}

void Event::setBJetUpWeight(double weight) {
	bJetUpWeight_ = weight;
}

void Event::setBJetDownWeight(double weight) {
	bJetDownWeight_ = weight;
}

void Event::setTightBJetWeight(double weight) {
	tightbJetWeight_ = weight;
}

void Event::setTightLightJetUpWeight(double weight) {
	tightlightJetUpWeight_ = weight;
}

void Event::setTightLightJetDownWeight(double weight) {
	tightlightJetDownWeight_ = weight;
}

void Event::setTightBJetUpWeight(double weight) {
	tightbJetUpWeight_ = weight;
}

void Event::setTightBJetDownWeight(double weight) {
	tightbJetDownWeight_ = weight;
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

const JetCollection& Event::CleanedJets() const {
	return cleanedJets_;
}

const JetCollection& Event::CleanedBJets() const {
	return cleanedBJets_;
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
	return MET(METAlgorithm::MET);  // FIXME when genMet is available from miniAOD ntuples
//	return genMet_;
}

const TtbarHypothesis Event::ttbarHypothesis() const {
	return ttbarHypothesis_;
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

double Event::PileUpWeight( int systematic ) const {
	if ( systematic == 0 ) {
		return pileUpWeight_;
	}
	else if ( systematic == 1 ) {
		return pileUpWeight_up_;
	}
	else if ( systematic == -1 ) {
		return pileUpWeight_down_;
	}

	return 1.;
}

double Event::BJetWeight() const {
	return bJetWeight_;
}

double Event::LightJetUpWeight() const {
	return lightJetUpWeight_;
}

double Event::LightJetDownWeight() const {
	return lightJetDownWeight_;
}

double Event::BJetUpWeight() const {
	return bJetUpWeight_;
}

double Event::BJetDownWeight() const {
	return bJetDownWeight_;
}

double Event::TightBJetWeight() const {
	return tightbJetWeight_;
}

double Event::TightLightJetUpWeight() const {
	return tightlightJetUpWeight_;
}

double Event::TightLightJetDownWeight() const {
	return tightlightJetDownWeight_;
}

double Event::TightBJetUpWeight() const {
	return tightbJetUpWeight_;
}

double Event::TightBJetDownWeight() const {
	return tightbJetDownWeight_;
}
void Event::inspect() const {
	cout << "run " << runNumber << ", event number " << eventNumber << ", lumi section " << lumiBlock << endl;

	cout << "number of tracks: " << tracks.size() << endl;
	cout << "number of high purity tracks: " << numberOfHighPurityTracks_ << endl;

	cout << "number of jets: " << allJets.size() << endl;
	EventContentPrinter::printJets(allJets);

	cout << "number of electrons: " << allElectrons.size() << endl;
	EventContentPrinter::printElectrons(allElectrons);

	cout << "number of muons: " << allMuons.size() << endl;
	EventContentPrinter::printMuons(allMuons);
}

bool Event::HLT(HLTriggers::value trigger) const {
	if (trigger >= HLTs->size())
		return false;

	return HLTs->at(trigger) > 0;
}

void Event::setGenNumberOfPileUpVertices(std::vector<int> pileup) {
	genNumberOfPileUpVertices = pileup;
}

void Event::setGeneratorWeight( double generatorWeight ) {
	generatorWeight_ = generatorWeight;
}

const double Event::generatorWeight() const {
	return generatorWeight_;
}

void Event::setCentralLHEWeight( double centralLHEWeight ) {
	centralLHEWeight_ = centralLHEWeight;
}

const double Event::centralLHEWeight() const {
	return centralLHEWeight_;
}

void Event::setGeneratorSystematicWeights(std::vector<double> generatorSystematicWeights) {
	generatorSystematicWeights_ = generatorSystematicWeights;
}

const std::vector<double> Event::generatorSystematicWeights() const {
	return generatorSystematicWeights_;
}

void Event::setGeneratorSystematicWeightsID(std::vector<int> generatorSystematicWeightsID) {
	generatorSystematicWeightsID_ = generatorSystematicWeightsID;
}

const std::vector<int> Event::generatorSystematicWeightsID() const {
	return generatorSystematicWeightsID_;
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

bool Event::passesMETFilters() const {
	return passesMETFilters_;	
}

void Event::setPassesMETFilters(bool pass) {
	passesMETFilters_ = pass;
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
		if(jets.at(index)->pt() > minJetPt_)
			ht += jets.at(index)->pt();
	}
	return ht;
}

double Event::ST(const JetCollection jets, const ParticlePointer lepton, const METPointer met) {
	// ST = HT + MET + lepton pt
	double ht = Event::HT(jets);
	double MET = met == 0 ? 0 : met->et();
	double lpt = lepton == 0 ? 0 : lepton->pt();

	return ht + MET + lpt;
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
					if ( jets.at(index1)->pt() <= minJetPt_ ||
						 jets.at(index2)->pt() <= minJetPt_ ||
						 jets.at(index3)->pt() <= minJetPt_ )
						continue;
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
		for (unsigned int i = 0; i < b_jets.size(); ++i) {
			if ( b_jets.at(i)->pt() <= minJetPt_ ) continue;
			particles.push_back(b_jets.at(i));
		}

		if ( particles.size() > 0 ) {

			// find closest b_jet
			unsigned short closest_b_jet_index = lepton->getClosest(particles);
			JetPointer closest_b_jet = b_jets.at(closest_b_jet_index);
			ParticlePointer pseudo_particle(new Particle(*closest_b_jet + *lepton));
			m_bl = pseudo_particle->mass();
		}
	}
	return m_bl;
}

double Event::angle_bl(const JetCollection b_jets,
		const ParticlePointer lepton) {
	double angle(-1);
	if (b_jets.size() != 0) {
		// store the jets as particle pointers
		ParticleCollection particles;
		for (unsigned int i = 0; i < b_jets.size(); ++i) {
			if ( b_jets.at(i)->pt() <= minJetPt_ ) continue;
			particles.push_back(b_jets.at(i));
		}

		if ( particles.size() > 0 ) {

			// find closest b_jet
			unsigned short closest_b_jet_index = lepton->getClosest(particles);
			JetPointer closest_b_jet = b_jets.at(closest_b_jet_index);
			angle = lepton->angle(closest_b_jet);
		}
	}
	return angle;
}

unsigned int Event::NJets(const JetCollection jets) {
	unsigned int nJets(0);
	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet(jets.at(index));

		if ( jet->pt() < minJetPt_ ) continue;

		++nJets;
	}
	return nJets;
}


}
