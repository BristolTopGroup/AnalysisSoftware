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
	AllEventsReader(), 
	AtLeastFourGoodJetsReader(), 
	AtLeastThreeGoodJetsReader(), 
	AtLeastTwoGoodJetsReader(), 
	AtLeastOneGoodJetReader(), 
	AtLeastOneBTagReader(), 
	AtLeastTwoBTagsReader(), 
	ConversionVetoReader(), 
	LooseElectronVetoReader(), 
	LooseMuonVetoReader(), 
	
	selectionOutputInfo() {

}

SelectionOutputReader::SelectionOutputReader(TChainPointer input, unsigned int selectionCriteria ) :
	numberJetsReader(),
	numberBJetsReader(),
	signalLeptonIndexReader(),
	cleanedJetsIndexReader(),
	cleanedBJetsIndexReader(),
	AllEventsReader(), 
	AtLeastFourGoodJetsReader(), 
	AtLeastThreeGoodJetsReader(), 
	AtLeastTwoGoodJetsReader(), 
	AtLeastOneGoodJetReader(), 
	AtLeastOneBTagReader(), 
	AtLeastTwoBTagsReader(), 
	ConversionVetoReader(), 
	LooseElectronVetoReader(), 
	LooseMuonVetoReader(), 
	
	selectionOutputInfo() {

	SelectionCriteria::selection selection = SelectionCriteria::selection(selectionCriteria);

	if ( selection == SelectionCriteria::ElectronPlusJetsReference ) {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsSelection.NumberOfBtags");
		signalLeptonIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsSelection.signalElectronIndices");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsSelection.cleanedJetIndex");
		cleanedBJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsSelection.cleanedBJetIndex");
		AllEventsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsSelection.AllEvents");
		AtLeastFourGoodJetsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsSelection.AtLeastFourGoodJets");
		AtLeastThreeGoodJetsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsSelection.AtLeastThreeGoodJets");
		AtLeastTwoGoodJetsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsSelection.AtLeastTwoGoodJets");
		AtLeastOneGoodJetReader = VariableReader<bool>(input, "TopPairElectronPlusJetsSelection.AtLeastOneGoodJet");
		AtLeastOneBTagReader = VariableReader<bool>(input, "TopPairElectronPlusJetsSelection.AtLeastOneBtag");
		AtLeastTwoBTagsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsSelection.AtLeastTwoBtags");
		ConversionVetoReader = VariableReader<bool>(input, "TopPairElectronPlusJetsSelection.ConversionVeto");
		LooseElectronVetoReader = VariableReader<bool>(input, "TopPairElectronPlusJetsSelection.LooseElectronVeto");
		LooseMuonVetoReader = VariableReader<bool>(input, "TopPairElectronPlusJetsSelection.LooseMuonVeto");

	}
	else if ( selection == SelectionCriteria::MuonPlusJetsReference ) {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsSelection.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsSelection.NumberOfBtags");
		signalLeptonIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsSelection.signalMuonIndices");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsSelection.cleanedJetIndex");		
		cleanedBJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsSelection.cleanedBJetIndex");
		AllEventsReader = VariableReader<bool>(input, "TopPairMuonPlusJetsSelection.AllEvents");
		AtLeastFourGoodJetsReader = VariableReader<bool>(input, "TopPairMuonPlusJetsSelection.AtLeastFourGoodJets");
		AtLeastThreeGoodJetsReader = VariableReader<bool>(input, "TopPairMuonPlusJetsSelection.AtLeastThreeGoodJets");
		AtLeastTwoGoodJetsReader = VariableReader<bool>(input, "TopPairMuonPlusJetsSelection.AtLeastTwoGoodJets");
		AtLeastOneGoodJetReader = VariableReader<bool>(input, "TopPairMuonPlusJetsSelection.AtLeastOneGoodJet");
		AtLeastOneBTagReader = VariableReader<bool>(input, "TopPairMuonPlusJetsSelection.AtLeastOneBtag");
		AtLeastTwoBTagsReader = VariableReader<bool>(input, "TopPairMuonPlusJetsSelection.AtLeastTwoBtags");
		ConversionVetoReader = VariableReader<bool>(input, "tmp");
		LooseElectronVetoReader = VariableReader<bool>(input, "TopPairMuonPlusJetsSelection.LooseElectronVeto");
		LooseMuonVetoReader = VariableReader<bool>(input, "TopPairMuonPlusJetsSelection.LooseMuonVeto");
		}
	else if ( selection == SelectionCriteria::ElectronPlusJetsQCDNonIsolated ) {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsQCDSelection.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsQCDSelection.NumberOfBtags");
		signalLeptonIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsQCDSelection.signalElectronIndices");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsQCDSelection.cleanedJetIndex");
		cleanedBJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsQCDSelection.cleanedBJetIndex");
		AllEventsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsQCDSelection.AllEvents");
		AtLeastFourGoodJetsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsQCDSelection.AtLeastFourGoodJets");
		AtLeastThreeGoodJetsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsQCDSelection.AtLeastThreeGoodJets");
		AtLeastTwoGoodJetsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsQCDSelection.AtLeastTwoGoodJets");
		AtLeastOneGoodJetReader = VariableReader<bool>(input, "TopPairElectronPlusJetsQCDSelection.AtLeastOneGoodJet");
		AtLeastOneBTagReader = VariableReader<bool>(input, "TopPairElectronPlusJetsQCDSelection.AtLeastOneBtag");
		AtLeastTwoBTagsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsQCDSelection.AtLeastTwoBtags");
		ConversionVetoReader = VariableReader<bool>(input, "TopPairElectronPlusJetsQCDSelection.ConversionVeto");
		LooseElectronVetoReader = VariableReader<bool>(input, "TopPairElectronPlusJetsQCDSelection.LooseElectronVeto");
		LooseMuonVetoReader = VariableReader<bool>(input, "TopPairElectronPlusJetsQCDSelection.LooseMuonVeto");
		}
	else if ( selection == SelectionCriteria::ElectronPlusJetsQCDConversion ) {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsConversionSelection.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairElectronPlusJetsConversionSelection.NumberOfBtags");
		signalLeptonIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsConversionSelection.signalElectronIndices");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsConversionSelection.cleanedJetIndex");
		cleanedBJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairElectronPlusJetsConversionSelection.cleanedBJetIndex");
		AllEventsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsConversionSelection.AllEvents");
		AtLeastFourGoodJetsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsConversionSelection.AtLeastFourGoodJets");
		AtLeastThreeGoodJetsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsConversionSelection.AtLeastThreeGoodJets");
		AtLeastTwoGoodJetsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsConversionSelection.AtLeastTwoGoodJets");
		AtLeastOneGoodJetReader = VariableReader<bool>(input, "TopPairElectronPlusJetsConversionSelection.AtLeastOneGoodJet");
		AtLeastOneBTagReader = VariableReader<bool>(input, "TopPairElectronPlusJetsConversionSelection.AtLeastOneBtag");
		AtLeastTwoBTagsReader = VariableReader<bool>(input, "TopPairElectronPlusJetsConversionSelection.AtLeastTwoBtags");
		ConversionVetoReader = VariableReader<bool>(input, "TopPairElectronPlusJetsConversionSelection.ConversionVeto");
		LooseElectronVetoReader = VariableReader<bool>(input, "TopPairElectronPlusJetsConversionSelection.LooseElectronVeto");
		LooseMuonVetoReader = VariableReader<bool>(input, "TopPairElectronPlusJetsConversionSelection.LooseMuonVeto");
		}
	else if ( selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated1p5to3 ) {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsQCDSelection1p5to3.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsQCDSelection1p5to3.NumberOfBtags");

		signalLeptonIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsQCDSelection1p5to3.signalMuonIndices");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsQCDSelection1p5to3.cleanedJetIndex");		
		cleanedBJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsQCDSelection1p5to3.cleanedBJetIndex");

		AllEventsReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection1p5to3.AllEvents");
		AtLeastFourGoodJetsReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection1p5to3.AtLeastFourGoodJets");
		AtLeastThreeGoodJetsReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection1p5to3.AtLeastThreeGoodJets");
		AtLeastTwoGoodJetsReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection1p5to3.AtLeastTwoGoodJets");
		AtLeastOneGoodJetReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection1p5to3.AtLeastOneGoodJet");
		AtLeastOneBTagReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection1p5to3.AtLeastOneBtag");
		AtLeastTwoBTagsReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection1p5to3.AtLeastTwoBtags");
		ConversionVetoReader = VariableReader<bool>(input,  "tmp");
		LooseElectronVetoReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection1p5to3.LooseElectronVeto");
		LooseMuonVetoReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection1p5to3.LooseMuonVeto");
		}
	else if ( selection == SelectionCriteria::MuonPlusJetsQCDNonIsolated3toInf ) {
		numberJetsReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsQCDSelection3toInf.NumberOfJets");
		numberBJetsReader = VariableReader<unsigned int>(input, "TopPairMuonPlusJetsQCDSelection3toInf.NumberOfBtags");

		signalLeptonIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsQCDSelection3toInf.signalMuonIndices");
		cleanedJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsQCDSelection3toInf.cleanedJetIndex");		
		cleanedBJetsIndexReader = VariableReader<MultiUIntPointer>(input, "TopPairMuonPlusJetsQCDSelection3toInf.cleanedBJetIndex");
		
		AllEventsReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection3toInf.AllEvents");
		AtLeastFourGoodJetsReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection3toInf.AtLeastFourGoodJets");
		AtLeastThreeGoodJetsReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection3toInf.AtLeastThreeGoodJets");
		AtLeastTwoGoodJetsReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection3toInf.AtLeastTwoGoodJets");
		AtLeastOneGoodJetReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection3toInf.AtLeastOneGoodJet");
		AtLeastOneBTagReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection3toInf.AtLeastOneBtag");
		AtLeastTwoBTagsReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection3toInf.AtLeastTwoBtags");
		ConversionVetoReader = VariableReader<bool>(input,  "tmp");
		LooseElectronVetoReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection3toInf.LooseElectronVeto");
		LooseMuonVetoReader = VariableReader<bool>(input,  "TopPairMuonPlusJetsQCDSelection3toInf.LooseMuonVeto");
		}
}

void SelectionOutputReader::initialise() {
	numberJetsReader.initialise();
	numberBJetsReader.initialise();
	signalLeptonIndexReader.initialise();
	cleanedJetsIndexReader.initialise();
	cleanedBJetsIndexReader.initialise();
	AllEventsReader.initialiseBlindly();
	AtLeastFourGoodJetsReader.initialiseBlindly();
	AtLeastThreeGoodJetsReader.initialiseBlindly();
	AtLeastTwoGoodJetsReader.initialiseBlindly();
	AtLeastOneGoodJetReader.initialiseBlindly();
	AtLeastOneBTagReader.initialiseBlindly();
	AtLeastTwoBTagsReader.initialiseBlindly();
	ConversionVetoReader.initialiseBlindly();
	LooseElectronVetoReader.initialiseBlindly();
	LooseMuonVetoReader.initialiseBlindly();
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

	if ( signalLeptonIndexReader.size() == 0 ) selectionOutputInfo.setSignalLeptonIndex( 999999999 );
	else selectionOutputInfo.setSignalLeptonIndex( signalLeptonIndexReader.getUIntVariableAt(0) );
	selectionOutputInfo.setAllEvents( AllEventsReader.getVariable() );
	selectionOutputInfo.setAtLeastFourGoodJets( AtLeastFourGoodJetsReader.getVariable() );
	selectionOutputInfo.setAtLeastThreeGoodJets( AtLeastThreeGoodJetsReader.getVariable() );
	selectionOutputInfo.setAtLeastTwoGoodJets( AtLeastTwoGoodJetsReader.getVariable() );
	selectionOutputInfo.setAtLeastOneGoodJet( AtLeastOneGoodJetReader.getVariable() );
	selectionOutputInfo.setAtLeastOneBTag( AtLeastOneBTagReader.getVariable() );
	selectionOutputInfo.setAtLeastTwoBTags( AtLeastTwoBTagsReader.getVariable() );
	selectionOutputInfo.setConversionVeto( ConversionVetoReader.getVariable() );
	selectionOutputInfo.setLooseElectronVeto( LooseElectronVetoReader.getVariable() );
	selectionOutputInfo.setLooseMuonVeto( LooseMuonVetoReader.getVariable() );
}

SelectionOutputReader::~SelectionOutputReader() {
}

}
