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
cleanedBJetsIndices_() {
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
}


