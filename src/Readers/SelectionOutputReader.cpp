/*
 * SelectionOutputReader.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Emyr Clement
 */

#include "../../interface/Readers/SelectionOutputReader.h"
#include "../../interface/GlobalVariables.h"
#include "../../interface/Event.h"
#include <iostream>

using namespace std;

namespace BAT {

SelectionOutputReader::SelectionOutputReader() :
	numberJetsReader(),
	numberBJetsReader(),
	signalLeptonIndexReader(),
	cleanedJetsIndexReader(),
	cleanedBJetsIndexReader(),
	selectionOutputInfo() {

}

SelectionOutputReader::SelectionOutputReader(TChainPointer input, unsigned int selectionCriteria ) :
	numberJetsReader(),
	numberBJetsReader(),
	signalLeptonIndexReader(),
	cleanedJetsIndexReader(),
	cleanedBJetsIndexReader(),
	selectionOutputInfo() {

	SelectionCriteria::selection selection = SelectionCriteria::selection(selectionCriteria);


	if ( selection == SelectionCriteria::ElectronPlusJetsReference ) {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.NumberOfBtags");
		signalLeptonIndexReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.signalElectronIndex");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsSelection.cleanedJetIndex");
		cleanedBJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsSelection.cleanedBJetIndex");
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsReference ) {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsSelection.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsSelection.NumberOfBtags");
		signalLeptonIndexReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsSelection.signalMuonIndex");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsSelection.cleanedJetIndex");		
		cleanedBJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsSelection.cleanedBJetIndex");
	}
	else if ( selection == SelectionCriteria::ElectronPlusJetsQCDNonIsolated ) {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.NumberOfBtags");
		signalLeptonIndexReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.signalElectronIndex");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsSelection.cleanedJetIndex");
		cleanedBJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsSelection.cleanedBJetIndex");
	}
	else if ( selection == SelectionCriteria::ElectronPlusJetsQCDConversion ) {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.NumberOfBtags");
		signalLeptonIndexReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.signalElectronIndex");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsSelection.cleanedJetIndex");
		cleanedBJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsSelection.cleanedBJetIndex");
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated ) {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsQCDSelection.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsQCDSelection.NumberOfBtags");
		signalLeptonIndexReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsQCDSelection.signalMuonIndex");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsQCDSelection.cleanedJetIndex");		
		cleanedBJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsQCDSelection.cleanedBJetIndex");
	}
}

void SelectionOutputReader::initialise() {
	numberJetsReader.initialise();
	numberBJetsReader.initialise();
	signalLeptonIndexReader.initialise();
	cleanedJetsIndexReader.initialise();
	cleanedBJetsIndexReader.initialise();
}

const SelectionOutputInfo& SelectionOutputReader::getSelectionOutputInfo() {
	readSelectionOutputInfo();
	return selectionOutputInfo;
}

void SelectionOutputReader::readSelectionOutputInfo() {
	selectionOutputInfo.setNumberOfJets( numberJetsReader.getVariable() );
	selectionOutputInfo.setNumberOfBJets( numberBJetsReader.getVariable() );

	std::vector<unsigned int> cleanedJetIndices;
	for (unsigned int index = 0; index < cleanedJetsIndexReader.size(); index++) {
		cleanedJetIndices.push_back( cleanedJetsIndexReader.getUIntVariableAt(index) );
	}
	selectionOutputInfo.setCleanedJetIndex( cleanedJetIndices );

	std::vector<unsigned int> cleanedBJetIndices;
	for (unsigned int index = 0; index < cleanedBJetsIndexReader.size(); index++) {
		cleanedBJetIndices.push_back( cleanedBJetsIndexReader.getUIntVariableAt(index) );
	}
	selectionOutputInfo.setCleanedBJetIndex( cleanedBJetIndices );

	selectionOutputInfo.setSignalLeptonIndex( signalLeptonIndexReader.getVariable() );
}
SelectionOutputReader::~SelectionOutputReader() {
}

}
