/*
 * SelectionOutputInfo.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Emyr Clement
 */

#include "../../interface/RecoObjects/SelectionOutputInfo.h"
#include "../../interface/GlobalVariables.h"
#include "../../interface/RecoObjects/Jet.h"
#include "../../interface/Event.h"

#include <iostream>

namespace BAT {

SelectionOutputInfo::SelectionOutputInfo() :
nJets_(),
nBJets_(),
signalLeptonIndex_(),
cleanedJetsIndices_(),
cleanedBJetsIndices_(),
AllEvents_(),
AtLeastFourGoodJets_(),
AtLeastThreeGoodJets_(),
AtLeastTwoGoodJets_(),
AtLeastOneGoodJet_(),
AtLeastOneBTag_(),
AtLeastTwoBTags_(),
ConversionVeto_(),
LooseElectronVeto_(),
LooseMuonVeto_() {
}

SelectionOutputInfo::~SelectionOutputInfo() {
}

unsigned int SelectionOutputInfo::getNumberOfJets() const {
	return nJets_;
}

unsigned int SelectionOutputInfo::getNumberOfBJets() const {
	return nBJets_;
}

unsigned int SelectionOutputInfo::getSignalLeptonIndex() const {
	return signalLeptonIndex_;
}

std::vector<unsigned int> SelectionOutputInfo::getCleanedJetIndex() const {
	return cleanedJetsIndices_;
}

std::vector<unsigned int> SelectionOutputInfo::getCleanedBJetIndex() const {
	return cleanedBJetsIndices_;
}

bool SelectionOutputInfo::getAllEvents() const {
	return AllEvents_;
}

bool SelectionOutputInfo::getAtLeastFourGoodJets() const {
	return AtLeastFourGoodJets_;
}

bool SelectionOutputInfo::getAtLeastThreeGoodJets() const {
	return AtLeastThreeGoodJets_;
}

bool SelectionOutputInfo::getAtLeastTwoGoodJets() const {
	return AtLeastTwoGoodJets_;
}

bool SelectionOutputInfo::getAtLeastOneGoodJet() const {
	return AtLeastOneGoodJet_;
}

bool SelectionOutputInfo::getAtLeastOneBTag() const {
	return AtLeastOneBTag_;
}

bool SelectionOutputInfo::getAtLeastTwoBTags() const {
	return AtLeastTwoBTags_;
}

bool SelectionOutputInfo::getConversionVeto() const {
	return ConversionVeto_;
}

bool SelectionOutputInfo::getLooseElectronVeto() const {
	return LooseElectronVeto_;
}

bool SelectionOutputInfo::getLooseMuonVeto() const {
	return LooseMuonVeto_;
}



void SelectionOutputInfo::setNumberOfJets( unsigned int nJets ) {
	nJets_ = nJets;
}

void SelectionOutputInfo::setNumberOfBJets( unsigned int nBJets ) {
	nBJets_ = nBJets;
}

void SelectionOutputInfo::setSignalLeptonIndex( unsigned int signalLeptonIndex ) {
	signalLeptonIndex_ = signalLeptonIndex;
}

void SelectionOutputInfo::setCleanedJetIndex( std::vector<unsigned int> cleanedJetsIndices ) {
	cleanedJetsIndices_ = cleanedJetsIndices;
}

void SelectionOutputInfo::setCleanedBJetIndex( std::vector<unsigned int> cleanedBJetsIndices ) {
	cleanedBJetsIndices_ = cleanedBJetsIndices;
}

void SelectionOutputInfo::setAllEvents( bool AllEvents){
	AllEvents_ = AllEvents;
}

void SelectionOutputInfo::setAtLeastFourGoodJets( bool AtLeastFourGoodJets){
	AtLeastFourGoodJets_ = AtLeastFourGoodJets;
}

void SelectionOutputInfo::setAtLeastThreeGoodJets( bool AtLeastThreeGoodJets){
	AtLeastThreeGoodJets_ = AtLeastThreeGoodJets;
}

void SelectionOutputInfo::setAtLeastTwoGoodJets( bool AtLeastTwoGoodJets){
	AtLeastTwoGoodJets_ = AtLeastTwoGoodJets;
}

void SelectionOutputInfo::setAtLeastOneGoodJet( bool AtLeastOneGoodJet){
	AtLeastOneGoodJet_ = AtLeastOneGoodJet;
}

void SelectionOutputInfo::setAtLeastOneBTag( bool AtLeastOneBTag){
	AtLeastOneBTag_ = AtLeastOneBTag;
}

void SelectionOutputInfo::setAtLeastTwoBTags( bool AtLeastTwoBTags){
	AtLeastTwoBTags_ = AtLeastTwoBTags;
}

void SelectionOutputInfo::setConversionVeto( bool ConversionVeto){
	ConversionVeto_ = ConversionVeto;
}

void SelectionOutputInfo::setLooseElectronVeto( bool LooseElectronVeto){
	LooseElectronVeto_ = LooseElectronVeto;
}

void SelectionOutputInfo::setLooseMuonVeto( bool LooseMuonVeto){
	LooseMuonVeto_ = LooseMuonVeto;
}
}


