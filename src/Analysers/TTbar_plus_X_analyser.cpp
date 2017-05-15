/*
 * TTbar_plus_X_analyser.cpp
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Analysers/TTbar_plus_X_analyser.h"
#include "../../interface/BTagWeight.h"
#include "../../interface/GlobalVariables.h"
#include "../../interface/Event.h"

namespace BAT {

void TTbar_plus_X_analyser::analyse(const EventPtr event) {
	ePlusJetsSignalAnalysis(event);
	ePlusJetsQcdAnalysis(event);
	muPlusJetsSignalAnalysis(event);
	muPlusJetsQcdAnalysis(event);
}

void TTbar_plus_X_analyser::ePlusJetsSignalAnalysis(const EventPtr event) {
	if ( event->PassesElectronTriggerAndSelectionNoB() ){
		BTagEffAnalyserEPlusJetsRefSelection_->analyse(event);
		fillCommonTreesNoBSelection( event, SelectionCriteria::ElectronPlusJetsReference, histogramFolder_ + "/EPlusJets/Ref selection NoBSelection" );
	}

	if ( event->PassesElectronTriggerAndSelection() ) {
		// Fill branches that are common amongst all regions
		fillCommonTrees( event, SelectionCriteria::ElectronPlusJetsReference, histogramFolder_ + "/EPlusJets/Ref selection" );
	}
}

void TTbar_plus_X_analyser::ePlusJetsQcdAnalysis(const EventPtr event) {
	if ( event->PassesElectronTriggerAndQCDSelection() ) {
		fillCommonTrees( event, SelectionCriteria::ElectronPlusJetsQCDNonIsolated, histogramFolder_ + "/EPlusJets/QCD non iso e+jets" );
	}

	if ( event->PassesElectronTriggerAndConversionSelection() ) {
		fillCommonTrees( event, SelectionCriteria::ElectronPlusJetsQCDConversion, histogramFolder_ + "/EPlusJets/QCDConversions" );
	}
}

void TTbar_plus_X_analyser::muPlusJetsSignalAnalysis(const EventPtr event) {
	if ( event->PassesMuonTriggerAndSelectionNoB() ){
		BTagEffAnalyserMuPlusJetsRefSelection_->analyse(event);
		fillCommonTreesNoBSelection( event, SelectionCriteria::MuonPlusJetsReference, histogramFolder_ + "/MuPlusJets/Ref selection NoBSelection" );
	}

	if ( event->PassesMuonTriggerAndSelection() ) {
		fillCommonTrees( event, SelectionCriteria::MuonPlusJetsReference, histogramFolder_ + "/MuPlusJets/Ref selection" );
	}
}

void TTbar_plus_X_analyser::muPlusJetsQcdAnalysis(const EventPtr event) {
	//selection with respect to reference selection
	if ( event->PassesMuonTriggerAndQCDSelection1p5to3() ) {
		fillCommonTrees( event, SelectionCriteria::MuonPlusJetsQCDNonIsolated1p5to3, histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 1p5to3" );
	}

	if ( event->PassesMuonTriggerAndQCDSelection3toInf() ) {
		fillCommonTrees( event, SelectionCriteria::MuonPlusJetsQCDNonIsolated3toInf, histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 3toInf" );
	}
}

void TTbar_plus_X_analyser::fillCommonTrees(const EventPtr event, const unsigned int selectionCriteria, std::string folder ) {
	SelectionCriteria::selection selection = SelectionCriteria::selection(selectionCriteria);

	// Jets
	const JetCollection jets(event->CleanedJets());
	// B Jets
	unsigned int numberOfBjets = event->getNBJets( selection );

	const JetCollection bJets(event->CleanedBJets());
	// Lepton
	const LeptonPointer signalLepton = event->getSignalLepton( selection );

	// MET
	const METPointer MET_original(event->MET((METAlgorithm::value) 0));

	treeMan_->setCurrentFolder(folder);
	treeMan_->Fill("EventWeight", event->weight());
	treeMan_->Fill("PUWeight", event->PileUpWeight());
	treeMan_->Fill("PUWeight_up", event->PileUpWeight(1));
	treeMan_->Fill("PUWeight_down", event->PileUpWeight(-1));
	treeMan_->Fill("lepton_eta",signalLepton->eta());
	treeMan_->Fill("lepton_pt",signalLepton->pt());
	treeMan_->Fill("lepton_charge",signalLepton->charge());
	if (selection == SelectionCriteria::selection(SelectionCriteria::ElectronPlusJetsReference) ||
		selection == SelectionCriteria::selection(SelectionCriteria::ElectronPlusJetsQCDNonIsolated) ||
		selection == SelectionCriteria::selection(SelectionCriteria::ElectronPlusJetsQCDConversion)){
		treeMan_->Fill("lepton_isolation", signalLepton->PFRelIsoWithEA());

		treeMan_->Fill("lepton_hltECALisolation", signalLepton->hltECALIso());
		treeMan_->Fill("lepton_hltHCALisolation", signalLepton->hltHCALIso());
		treeMan_->Fill("lepton_hltTrackerisolation", signalLepton->hltTrackerIso());
		if ( signalLepton->pt() > 0 ) {
			treeMan_->Fill("lepton_hltTrackerisolation_overPt", signalLepton->hltTrackerIso() / signalLepton->pt() );
		}
	}
	else if (selection == SelectionCriteria::selection(SelectionCriteria::MuonPlusJetsReference) ||
			 selection == SelectionCriteria::selection(SelectionCriteria::MuonPlusJetsQCDNonIsolated1p5to3) ||
			 selection == SelectionCriteria::selection(SelectionCriteria::MuonPlusJetsQCDNonIsolated3toInf)){
		treeMan_->Fill("lepton_isolation", signalLepton->PFRelIso04DeltaBeta());
	}
	treeMan_->Fill("M3",Event::M3(jets));
	if ( numberOfBjets > 0 ) {
		treeMan_->Fill("M_bl",Event::M_bl(bJets, signalLepton));
		treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalLepton));

// 		if ( numberOfBjets >= 2 ) {
// 			unsigned int highestCSVJetIndex = 0;
// 			unsigned int secondHighestCSVJetIndex = 0;
// 			Event::getTopTwoCSVJets( bJets, highestCSVJetIndex, secondHighestCSVJetIndex );
// 			JetPointer highestCSVJet(bJets.at(highestCSVJetIndex));
// 			JetPointer secondHighestCSVJet(bJets.at(secondHighestCSVJetIndex));
// 			treeMan_->Fill("deltaPhi_bb", fabs( Event::deltaPhi_bb(highestCSVJet, secondHighestCSVJet) ) ) ;
// 			treeMan_->Fill("deltaEta_bb", fabs( Event::deltaEta_bb(highestCSVJet, secondHighestCSVJet) ) ) ;
// 			treeMan_->Fill("angle_bb", Event::angle_bb(highestCSVJet, secondHighestCSVJet));
// 		}
	}
	for (unsigned int index = 0; index < jets.size(); ++index) {
		treeMan_->Fill("jet_pt", jets.at(index)->pt() );
		treeMan_->Fill("jet_eta", jets.at(index)->eta() );
		treeMan_->Fill("jet_csv", jets.at(index)->getBTagDiscriminator(BtagAlgorithm::CombinedSecondaryVertexV2) );
	}	
	treeMan_->Fill("HT",Event::HT(jets));
	treeMan_->Fill("MET",MET_original->et());
	treeMan_->Fill("MET_phi",MET_original->phi());
	treeMan_->Fill("ST",Event::ST(jets, signalLepton, MET_original));
	treeMan_->Fill("WPT",Event::WPT(signalLepton, MET_original));
	treeMan_->Fill("MT",Event::MT(signalLepton, MET_original));

	treeMan_->Fill("NJets",Event::NJets(jets));
	treeMan_->Fill("NBJets",Event::NJets(bJets));
	treeMan_->Fill("NVertices",	event->Vertices().size());

	treeMan_->Fill("BJetWeight",event->BJetWeight());
	treeMan_->Fill("BJetUpWeight",event->BJetUpWeight());
	treeMan_->Fill("BJetDownWeight",event->BJetDownWeight());
	treeMan_->Fill("LightJetUpWeight",event->LightJetUpWeight());
	treeMan_->Fill("LightJetDownWeight",event->LightJetDownWeight());

	// MET Uncertainties		
	for ( unsigned int unc_i = 0; unc_i < MET_original->getAllMETUncertainties().size(); ++unc_i ) {		
		METPointer METForUnc_i = MET_original->getMETForUncertainty( unc_i );		
		treeMan_->Fill("MET_METUncertainties",METForUnc_i->et());		
		treeMan_->Fill("ST_METUncertainties",Event::ST(jets, signalLepton, METForUnc_i));		
		treeMan_->Fill("WPT_METUncertainties",Event::WPT(signalLepton, METForUnc_i));		
	}

	fillLeptonEfficiencyCorrectionBranches( event, selectionCriteria, signalLepton );	
}

void TTbar_plus_X_analyser::fillCommonTreesNoBSelection(const EventPtr event,  const unsigned int selectionCriteria, std::string folder ) {
	SelectionCriteria::selection selection = SelectionCriteria::selection(selectionCriteria);

	// Jets
	const JetCollection jets(event->CleanedJets());
	// B Jets
	const JetCollection bJets(event->CleanedBJets());
	// Lepton
	const LeptonPointer signalLepton = event->getSignalLepton( selection );
	// MET
	const METPointer MET_original(event->MET((METAlgorithm::value) 0));


	treeMan_->setCurrentFolder(folder);
	treeMan_->Fill("EventWeight", event->weight());
	treeMan_->Fill("PUWeight", event->PileUpWeight());
	treeMan_->Fill("PUWeight_up", event->PileUpWeight(1));
	treeMan_->Fill("PUWeight_down", event->PileUpWeight(-1));

	treeMan_->Fill("M3",Event::M3(jets));

	if ( Event::NJets(bJets) > 0 ) {
		treeMan_->Fill("M_bl",Event::M_bl(bJets, signalLepton));
		treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalLepton));
	}

	treeMan_->Fill("HT",Event::HT(jets));
	treeMan_->Fill("MET",MET_original->et());
	treeMan_->Fill("MET_phi",MET_original->phi());
	treeMan_->Fill("ST",Event::ST(jets, signalLepton, MET_original));
	treeMan_->Fill("WPT",Event::WPT(signalLepton, MET_original));
	treeMan_->Fill("MT",Event::MT(signalLepton, MET_original));

	treeMan_->Fill("NJets",Event::NJets(jets));
	treeMan_->Fill("NBJets",Event::NJets(bJets));
	treeMan_->Fill("NVertices",	event->Vertices().size());

	treeMan_->Fill("BJetWeight",event->BJetWeight());
	treeMan_->Fill("BJetUpWeight",event->BJetUpWeight());
	treeMan_->Fill("BJetDownWeight",event->BJetDownWeight());
	treeMan_->Fill("LightJetUpWeight",event->LightJetUpWeight());
	treeMan_->Fill("LightJetDownWeight",event->LightJetDownWeight());

	if (selection == SelectionCriteria::selection(SelectionCriteria::ElectronPlusJetsReference)){
		treeMan_->Fill("lepton_isolation", signalLepton->PFRelIso03DeltaBeta());
	}
	else if (selection == SelectionCriteria::selection(SelectionCriteria::MuonPlusJetsReference)){
		treeMan_->Fill("lepton_isolation", signalLepton->PFRelIso04DeltaBeta());
	}

	for (unsigned int index = 0; index < jets.size(); ++index) {
		treeMan_->Fill("jet_csv", jets.at(index)->getBTagDiscriminator(BtagAlgorithm::CombinedSecondaryVertexV2) );
	}	
	fillLeptonEfficiencyCorrectionBranches( event, selectionCriteria, signalLepton );	
}

void TTbar_plus_X_analyser::fillLeptonEfficiencyCorrectionBranches( const EventPtr event, const unsigned int selectionCriteria, const LeptonPointer signalLepton ) {
	SelectionCriteria::selection selection = SelectionCriteria::selection(selectionCriteria);
	if ( selection == SelectionCriteria::ElectronPlusJetsReference ) {
		double electronEfficiencyCorrection = 1, electronEfficiencyCorrection_down = 1, electronEfficiencyCorrection_up = 1;
		if ( !event->isRealData() ) {
			const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));
			electronEfficiencyCorrection = signalElectron->getEfficiencyCorrection( 0 );
			electronEfficiencyCorrection_down = signalElectron->getEfficiencyCorrection( -1 );
			electronEfficiencyCorrection_up = signalElectron->getEfficiencyCorrection( 1 );
		}
		treeMan_->Fill("ElectronEfficiencyCorrection",electronEfficiencyCorrection);
		treeMan_->Fill("ElectronUp",electronEfficiencyCorrection_up);
		treeMan_->Fill("ElectronDown",electronEfficiencyCorrection_down);
	}
	else if ( selection == SelectionCriteria::MuonPlusJetsReference ) {
		double muonEfficiencyCorrection = 1, muonEfficiencyCorrection_down = 1, muonEfficiencyCorrection_up = 1;
		if ( !event->isRealData() ) {
			const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
			muonEfficiencyCorrection = signalMuon->getEfficiencyCorrection( 0 );
			muonEfficiencyCorrection_down = signalMuon->getEfficiencyCorrection( -1 );
			muonEfficiencyCorrection_up = signalMuon->getEfficiencyCorrection( 1 );
		}
		treeMan_->Fill("MuonEfficiencyCorrection",muonEfficiencyCorrection);
		treeMan_->Fill("MuonUp",muonEfficiencyCorrection_up);
		treeMan_->Fill("MuonDown",muonEfficiencyCorrection_down);
	}
}

void TTbar_plus_X_analyser::createCommonTrees( std::string folder) {
	treeMan_->setCurrentFolder(folder);
	treeMan_->addBranch("HT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET_phi", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("WPT_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("MT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_eta", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_pt", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_charge", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_isolation", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_hltECALisolation", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_hltHCALisolation", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_hltTrackerisolation", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_hltTrackerisolation_overPt", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M3", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("NJets", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("NBJets", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("NVertices", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("jet_pt", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("jet_eta", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("jet_csv", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("angle_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("deltaPhi_bb", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("deltaEta_bb", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("angle_bb", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepTopPt", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("hadTopPt", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepTopRap", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("hadTopRap", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ttbarPt", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ttbarM", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ttbarRap", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronEfficiencyCorrection", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronUp", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronDown", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MuonEfficiencyCorrection", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MuonUp", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MuonDown", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("BJetWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("BJetUpWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("BJetDownWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("LightJetUpWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("LightJetDownWeight", "F", "FitVariables" + Globals::treePrefix_);
}

void TTbar_plus_X_analyser::createCommonNoBSelectionTrees( std::string folder) {
	treeMan_->setCurrentFolder(folder);

	treeMan_->addBranch("NJets", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("NBJets", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("NVertices", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronEfficiencyCorrection", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronUp", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronDown", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MuonEfficiencyCorrection", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MuonUp", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MuonDown", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("BJetWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("BJetUpWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("BJetDownWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("LightJetUpWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("LightJetDownWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_isolation", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("jet_csv", "F", "FitVariables" + Globals::treePrefix_, false);

	treeMan_->addBranch("HT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET_phi", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_eta", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_pt", "F", "FitVariables" + Globals::treePrefix_);

	treeMan_->addBranch("M_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M3", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("angle_bl", "F", "FitVariables" + Globals::treePrefix_);

}

void TTbar_plus_X_analyser::createTrees() {
	createCommonTrees(histogramFolder_ + "/EPlusJets/Ref selection");
	createCommonTrees(histogramFolder_ + "/EPlusJets/QCD non iso e+jets");
	createCommonTrees(histogramFolder_ + "/EPlusJets/QCDConversions");

	createCommonTrees(histogramFolder_ + "/MuPlusJets/Ref selection");
	createCommonTrees(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 1p5to3");
	createCommonTrees(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 3toInf");

	createCommonNoBSelectionTrees(histogramFolder_ + "/EPlusJets/Ref selection NoBSelection");
	createCommonNoBSelectionTrees(histogramFolder_ + "/MuPlusJets/Ref selection NoBSelection");

	BTagEffAnalyserEPlusJetsRefSelection_->createTrees();
	BTagEffAnalyserMuPlusJetsRefSelection_->createTrees();
	PileupAnalyserEPlusJetsRefSelection_->createTrees();
	PileupAnalyserMuPlusJetsRefSelection_->createTrees();
}

TTbar_plus_X_analyser::TTbar_plus_X_analyser(TreeManagerPtr treeMan, std::string histogramFolder) :
		BasicAnalyser(treeMan, histogramFolder),
		BTagEffAnalyserEPlusJetsRefSelection_(new BTagEff(treeMan, histogramFolder + "/EPlusJets/Ref selection NoBSelection/BTagEfficiencies")),
		BTagEffAnalyserMuPlusJetsRefSelection_(new BTagEff(treeMan, histogramFolder + "/MuPlusJets/Ref selection NoBSelection/BTagEfficiencies")){
}

TTbar_plus_X_analyser::~TTbar_plus_X_analyser() {
}

} /* namespace BAT */
