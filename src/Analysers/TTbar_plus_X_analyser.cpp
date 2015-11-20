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

	if (event->PassesElectronTriggerAndSelectionNoB()){
		BTagEffAnalyserEPlusJetsRefSelection_->analyse(event);
		PileupAnalyserEPlusJetsRefSelection_->analyse(event);

		fillCommonTreesNoBSelection( event, SelectionCriteria::ElectronPlusJetsReference, histogramFolder_ + "/EPlusJets/Ref selection NoBSelection" );
	}

	if ( event->PassesElectronTriggerAndSelection() ) {

		// Fill branches that are common amongst all regions
		fillCommonTrees( event, SelectionCriteria::ElectronPlusJetsReference, histogramFolder_ + "/EPlusJets/Ref selection" );

		// Lepton
		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::ElectronPlusJetsReference );
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));
		// MET
		const METPointer MET_original(event->MET((METAlgorithm::value) 0));
		// Jets
		const JetCollection jets(event->CleanedJets());
		const JetCollection bJets(event->CleanedBJets());


		if ( Globals::useHitFit ) {
			BAT::TtbarHypothesis topHypothesis = hitFitAnalyserEPlusJetsRefSelection_->analyseAndReturn(event, jets, bJets, signalLepton );
			event->setTTbarHypothesis( topHypothesis );
			treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/Ref selection");
			if ( topHypothesis.isValid() && topHypothesis.isPhysical() ) {
				treeMan_->Fill("lepTopPt",topHypothesis.leptonicTop->pt());
				treeMan_->Fill("hadTopPt",topHypothesis.hadronicTop->pt());
				treeMan_->Fill("lepTopRap",topHypothesis.leptonicTop->rapidity());
				treeMan_->Fill("hadTopRap",topHypothesis.hadronicTop->rapidity());
				treeMan_->Fill("ttbarPt",topHypothesis.resonance->pt());
				treeMan_->Fill("ttbarM",topHypothesis.resonance->mass());
				treeMan_->Fill("ttbarRap",topHypothesis.resonance->rapidity());
			}
		}

		metAnalyserEPlusJetsRefSelection_->analyse(event, signalLepton, jets);
		electronAnalyserRefSelection_->analyse(event);
		electronAnalyserRefSelection_->analyseElectron(signalElectron, event->weight());
		vertexAnalyserEPlusJetsRefSelection_->analyse(event);
		jetAnalyserEPlusJetsRefSelection_->analyse(event);
		wAnalyserEPlusJetsRefSelection_->analyseHadronicW(event, jets, bJets);
		if ( ! event->isRealData() ) {
			wAnalyserEPlusJetsRefSelection_->analyseHadronicW_partons(event);			
		}

		likelihoodRecoAnalyserEPlusJetsRefSelection_->analyse(event, jets, bJets, signalLepton, MET_original);
	}
}

void TTbar_plus_X_analyser::ePlusJetsQcdAnalysis(const EventPtr event) {
	if ( event->PassesElectronTriggerAndQCDSelection() ) {

		fillCommonTrees( event, SelectionCriteria::ElectronPlusJetsQCDNonIsolated, histogramFolder_ + "/EPlusJets/QCD non iso e+jets" );

		const JetCollection jets(event->CleanedJets());
		const JetCollection bJets(event->CleanedBJets());
		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::ElectronPlusJetsQCDNonIsolated );
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		// MET
		const METPointer MET_original(event->MET((METAlgorithm::value) 0));
		const METPointer METNoHF(event->MET((METAlgorithm::value) 1));

		if ( Globals::useHitFit ) {
			BAT::TtbarHypothesis topHypothesis = hitFitAnalyserEPlusJetsQCDSelection_->analyseAndReturn(event, jets, bJets, signalLepton );
			event->setTTbarHypothesis( topHypothesis );
			treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/QCD non iso e+jets");
			if ( topHypothesis.isValid() && topHypothesis.isPhysical() ) {
				treeMan_->Fill("lepTopPt",topHypothesis.leptonicTop->pt());
				treeMan_->Fill("hadTopPt",topHypothesis.hadronicTop->pt());
				treeMan_->Fill("lepTopRap",topHypothesis.leptonicTop->rapidity());
				treeMan_->Fill("hadTopRap",topHypothesis.hadronicTop->rapidity());
				treeMan_->Fill("ttbarPt",topHypothesis.resonance->pt());
				treeMan_->Fill("ttbarM",topHypothesis.resonance->mass());
				treeMan_->Fill("ttbarRap",topHypothesis.resonance->rapidity());
			}
		}

		qcdNonIsoElectronAnalyser_->analyse(event);
		qcdNonIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdNonIsoElectronSelection_->analyse(event, signalLepton, jets);
		jetAnalyserEPlusJetsQCDNonIsoSelection_->analyse(event);
	}

	if ( event->PassesElectronTriggerAndConversionSelection() ) {

		fillCommonTrees( event, SelectionCriteria::ElectronPlusJetsQCDConversion, histogramFolder_ + "/EPlusJets/QCDConversions" );

		const JetCollection jets(event->CleanedJets());
		const JetCollection bJets(event->CleanedBJets());
		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::ElectronPlusJetsQCDConversion );
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		// MET
		const METPointer MET_original(event->MET((METAlgorithm::value) 0));
		const METPointer METNoHF(event->MET((METAlgorithm::value) 1));

		if ( Globals::useHitFit ) {
			BAT::TtbarHypothesis topHypothesis = hitFitAnalyserEPlusJetsConversionSelection_->analyseAndReturn(event, jets, bJets, signalLepton );
			event->setTTbarHypothesis( topHypothesis );
			treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/QCDConversions");
			if ( topHypothesis.isValid() && topHypothesis.isPhysical() ) {
				treeMan_->Fill("lepTopPt",topHypothesis.leptonicTop->pt());
				treeMan_->Fill("hadTopPt",topHypothesis.hadronicTop->pt());
				treeMan_->Fill("lepTopRap",topHypothesis.leptonicTop->rapidity());
				treeMan_->Fill("hadTopRap",topHypothesis.hadronicTop->rapidity());
				treeMan_->Fill("ttbarPt",topHypothesis.resonance->pt());
				treeMan_->Fill("ttbarM",topHypothesis.resonance->mass());
				treeMan_->Fill("ttbarRap",topHypothesis.resonance->rapidity());
			}
		}
		qcdConversionsElectronAnalyser_->analyse(event);
		qcdConversionsElectronAnalyser_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdConversionSelection_->analyse(event, signalLepton, jets);
		jetAnalyserEPlusJetsConversionSelection_->analyse(event);
	}
}

void TTbar_plus_X_analyser::muPlusJetsSignalAnalysis(const EventPtr event) {

	if (event->PassesMuonTriggerAndSelectionNoB()){
		BTagEffAnalyserMuPlusJetsRefSelection_->analyse(event);
		PileupAnalyserMuPlusJetsRefSelection_->analyse(event);
	
		fillCommonTreesNoBSelection( event, SelectionCriteria::MuonPlusJetsReference, histogramFolder_ + "/MuPlusJets/Ref selection NoBSelection" );
	}

	// if (topMuplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
	if ( event->PassesMuonTriggerAndSelection() ) {

		fillCommonTrees( event, SelectionCriteria::MuonPlusJetsReference, histogramFolder_ + "/MuPlusJets/Ref selection" );

		const JetCollection jets(event->CleanedJets());
		const JetCollection bJets(event->CleanedBJets());
		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::MuonPlusJetsReference );
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		const METPointer MET_original(event->MET((METAlgorithm::value) 0));
		const METPointer METNoHF(event->MET((METAlgorithm::value) 1));

		if ( Globals::useHitFit ) {
			BAT::TtbarHypothesis topHypothesis = hitFitAnalyserMuPlusJetsRefSelection_->analyseAndReturn(event, jets, bJets, signalLepton);
			event->setTTbarHypothesis( topHypothesis );
			treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/Ref selection");
			if ( topHypothesis.isValid() && topHypothesis.isPhysical() ) {
				treeMan_->Fill("lepTopPt",topHypothesis.leptonicTop->pt());
				treeMan_->Fill("hadTopPt",topHypothesis.hadronicTop->pt());
				treeMan_->Fill("lepTopRap",topHypothesis.leptonicTop->rapidity());
				treeMan_->Fill("hadTopRap",topHypothesis.hadronicTop->rapidity());
				treeMan_->Fill("ttbarPt",topHypothesis.resonance->pt());
				treeMan_->Fill("ttbarM",topHypothesis.resonance->mass());
				treeMan_->Fill("ttbarRap",topHypothesis.resonance->rapidity());
			}
		}

		metAnalyserMuPlusJetsRefSelection_->analyse(event, signalLepton, jets);
		muonAnalyserRefSelection_->analyse(event);
		muonAnalyserRefSelection_->analyseMuon(signalMuon, event->weight());
		vertexAnalyserMuPlusJetsRefSelection_->analyse(event);
		jetAnalyserMuPlusJetsRefSelection_->analyse(event);
		wAnalyserMuPlusJetsRefSelection_->analyseHadronicW(event, jets, bJets);
		if ( ! event->isRealData() ) {
			wAnalyserMuPlusJetsRefSelection_->analyseHadronicW_partons(event);			
		}
		likelihoodRecoAnalyserMuPlusJetsRefSelection_->analyse(event, jets, bJets, signalLepton, MET_original);
	}
}

void TTbar_plus_X_analyser::muPlusJetsQcdAnalysis(const EventPtr event) {
	//selection with respect to reference selection
	if ( event->PassesMuonTriggerAndQCDSelection() ) {

		fillCommonTrees( event, SelectionCriteria::MuonPlusJetsQCDNonIsolated, histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets" );

		const JetCollection jets(event->CleanedJets());
		const JetCollection bJets(event->CleanedBJets());
		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::MuonPlusJetsQCDNonIsolated );
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));

		const METPointer MET_original(event->MET((METAlgorithm::value) 0));
		const METPointer METNoHF(event->MET((METAlgorithm::value) 1));

		if ( Globals::useHitFit ) {
			BAT::TtbarHypothesis topHypothesis = hitFitAnalyserMuPlusJetsQCDSelection_->analyseAndReturn(event, jets, bJets, signalLepton);
			event->setTTbarHypothesis( topHypothesis );
			treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets");
			if ( topHypothesis.isValid() && topHypothesis.isPhysical() ) {
				treeMan_->Fill("lepTopPt",topHypothesis.leptonicTop->pt());
				treeMan_->Fill("hadTopPt",topHypothesis.hadronicTop->pt());
				treeMan_->Fill("lepTopRap",topHypothesis.leptonicTop->rapidity());
				treeMan_->Fill("hadTopRap",topHypothesis.hadronicTop->rapidity());
				treeMan_->Fill("ttbarPt",topHypothesis.resonance->pt());
				treeMan_->Fill("ttbarM",topHypothesis.resonance->mass());
				treeMan_->Fill("ttbarRap",topHypothesis.resonance->rapidity());
			}
		}

		qcdNonIsoMuonAnalyser_->analyse(event);
		qcdNonIsoMuonAnalyser_->analyseMuon(signalMuon, event->weight());
		metAnalyserqcdNonIsoMuonSelection_->analyse(event, signalLepton, jets);
		jetAnalyserMuPlusJetsQCDNonIsoSelection_->analyse(event);
	}
}

void TTbar_plus_X_analyser::fillCommonTrees(const EventPtr event,  const unsigned int selectionCriteria, std::string folder ) {
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
	const METPointer METNoHF(event->MET((METAlgorithm::value) 1));

	treeMan_->setCurrentFolder(folder);
	treeMan_->Fill("EventWeight", event->weight());
	treeMan_->Fill("PUWeight", event->PileUpWeight());
	treeMan_->Fill("PUWeight_up", event->PileUpWeight(1));
	treeMan_->Fill("PUWeight_down", event->PileUpWeight(-1));
	treeMan_->Fill("lepton_eta",signalLepton->eta());
	treeMan_->Fill("lepton_pt",signalLepton->pt());
	treeMan_->Fill("lepton_charge",signalLepton->charge());
	treeMan_->Fill("M3",Event::M3(jets));
	if ( numberOfBjets > 0 ) {
		treeMan_->Fill("M_bl",Event::M_bl(bJets, signalLepton));
		treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalLepton));
	}
	treeMan_->Fill("HT",Event::HT(jets));
	treeMan_->Fill("MET",MET_original->et());
	treeMan_->Fill("METNoHF",METNoHF->et());
	treeMan_->Fill("ST",Event::ST(jets, signalLepton, MET_original));
	treeMan_->Fill("STNoHF",Event::ST(jets, signalLepton, METNoHF));
	treeMan_->Fill("WPT",Event::WPT(signalLepton, MET_original));
	treeMan_->Fill("WPTNoHF",Event::WPT(signalLepton, METNoHF));
	treeMan_->Fill("MT",Event::MT(signalLepton, MET_original));
	treeMan_->Fill("NJets",Event::NJets(jets));
	treeMan_->Fill("NBJets",Event::NJets(bJets));
	treeMan_->Fill("NVertices",	event->Vertices().size());

	treeMan_->Fill("BJetWeight",event->BJetWeight());
	treeMan_->Fill("BJetUpWeight",event->BJetUpWeight());
	treeMan_->Fill("BJetDownWeight",event->BJetDownWeight());

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

	treeMan_->setCurrentFolder(folder);
	treeMan_->Fill("EventWeight", event->weight());
	treeMan_->Fill("PUWeight", event->PileUpWeight());
	treeMan_->Fill("PUWeight_up", event->PileUpWeight(1));
	treeMan_->Fill("PUWeight_down", event->PileUpWeight(-1));

	treeMan_->Fill("NJets",Event::NJets(jets));
	treeMan_->Fill("NBJets",Event::NJets(bJets));
	treeMan_->Fill("NVertices",	event->Vertices().size());

	treeMan_->Fill("BJetWeight",event->BJetWeight());
	treeMan_->Fill("BJetUpWeight",event->BJetUpWeight());
	treeMan_->Fill("BJetDownWeight",event->BJetDownWeight());

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
	treeMan_->addBranch("METNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("STNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("WPTNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("WPT_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("MT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_eta", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_pt", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("lepton_charge", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M3", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("NJets", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("NBJets", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("NVertices", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("angle_bl", "F", "FitVariables" + Globals::treePrefix_);
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
}

void TTbar_plus_X_analyser::createTrees() {
	createCommonTrees(histogramFolder_ + "/EPlusJets/Ref selection");
	createCommonTrees(histogramFolder_ + "/EPlusJets/QCD non iso e+jets");
	createCommonTrees(histogramFolder_ + "/EPlusJets/QCDConversions");

	createCommonTrees(histogramFolder_ + "/MuPlusJets/Ref selection");
	createCommonTrees(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets");

	createCommonNoBSelectionTrees(histogramFolder_ + "/EPlusJets/Ref selection NoBSelection");
	createCommonNoBSelectionTrees(histogramFolder_ + "/MuPlusJets/Ref selection NoBSelection");

	//signal
	metAnalyserEPlusJetsRefSelection_->createHistograms();
	metAnalyserEPlusJetsRefSelection_->createTrees();
	electronAnalyserRefSelection_->createHistograms();
	electronAnalyserRefSelection_->createTrees();
	metAnalyserMuPlusJetsRefSelection_->createHistograms();
	metAnalyserMuPlusJetsRefSelection_->createTrees();
	muonAnalyserRefSelection_->createHistograms();
	muonAnalyserRefSelection_->createTrees();

	vertexAnalyserEPlusJetsRefSelection_->createHistograms();
	vertexAnalyserMuPlusJetsRefSelection_->createHistograms();
	//QCD region
	qcdNonIsoElectronAnalyser_->createHistograms();
	qcdNonIsoElectronAnalyser_->createTrees();
	metAnalyserqcdNonIsoElectronSelection_->createHistograms();
	metAnalyserqcdNonIsoElectronSelection_->createTrees();
	qcdNonIsoMuonAnalyser_->createHistograms();
	qcdNonIsoMuonAnalyser_->createTrees();
	metAnalyserqcdNonIsoMuonSelection_->createHistograms();
	metAnalyserqcdNonIsoMuonSelection_->createTrees();
	
	qcdConversionsElectronAnalyser_->createHistograms();
	qcdConversionsElectronAnalyser_->createTrees();
	metAnalyserqcdConversionSelection_->createHistograms();
	metAnalyserqcdConversionSelection_->createTrees();

	jetAnalyserEPlusJetsRefSelection_->createHistograms();
	jetAnalyserMuPlusJetsRefSelection_->createHistograms();
	jetAnalyserEPlusJetsQCDNonIsoSelection_->createHistograms();
	jetAnalyserEPlusJetsConversionSelection_->createHistograms();
	jetAnalyserMuPlusJetsQCDNonIsoSelection_->createHistograms();

	jetAnalyserEPlusJetsRefSelection_->createTrees();
	jetAnalyserMuPlusJetsRefSelection_->createTrees();
	jetAnalyserEPlusJetsQCDNonIsoSelection_->createTrees();
	jetAnalyserEPlusJetsConversionSelection_->createTrees();
	jetAnalyserMuPlusJetsQCDNonIsoSelection_->createTrees();

	// W boson simple reconstruction
	wAnalyserEPlusJetsRefSelection_->createHistograms();
	wAnalyserMuPlusJetsRefSelection_->createHistograms();
	wAnalyserEPlusJetsRefSelection_->createTrees();
	wAnalyserMuPlusJetsRefSelection_->createTrees();

	hitFitAnalyserEPlusJetsRefSelection_->createHistograms();
	hitFitAnalyserMuPlusJetsRefSelection_->createHistograms();
	hitFitAnalyserEPlusJetsQCDSelection_->createHistograms();
	hitFitAnalyserEPlusJetsConversionSelection_->createHistograms();
	hitFitAnalyserMuPlusJetsQCDSelection_->createHistograms();

	hitFitAnalyserEPlusJetsRefSelection_->createTrees();
	hitFitAnalyserMuPlusJetsRefSelection_->createTrees();
	hitFitAnalyserEPlusJetsQCDSelection_->createTrees();
	hitFitAnalyserEPlusJetsConversionSelection_->createTrees();
	hitFitAnalyserMuPlusJetsQCDSelection_->createTrees();

	likelihoodRecoAnalyserEPlusJetsRefSelection_->createHistograms();
	likelihoodRecoAnalyserMuPlusJetsRefSelection_->createHistograms();
	likelihoodRecoAnalyserEPlusJetsRefSelection_->createTrees();
	likelihoodRecoAnalyserMuPlusJetsRefSelection_->createTrees();

	BTagEffAnalyserEPlusJetsRefSelection_->createTrees();
	BTagEffAnalyserMuPlusJetsRefSelection_->createTrees();

	PileupAnalyserEPlusJetsRefSelection_->createTrees();
	PileupAnalyserMuPlusJetsRefSelection_->createTrees();
}

TTbar_plus_X_analyser::TTbar_plus_X_analyser(HistogramManagerPtr histMan, TreeManagerPtr treeMan, std::string histogramFolder) :
		BasicAnalyser(histMan, treeMan, histogramFolder), //

		//analysers
		//signal regions
		metAnalyserEPlusJetsRefSelection_(new METAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/Ref selection/MET")), //
		metAnalyserMuPlusJetsRefSelection_(new METAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/Ref selection/MET")), //
		electronAnalyserRefSelection_(
				new ElectronAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/Ref selection/Electron", true)), //
		muonAnalyserRefSelection_(new MuonAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/Ref selection/Muon", true)), //
		vertexAnalyserEPlusJetsRefSelection_(
				new VertexAnalyser(histMan, histogramFolder + "/EPlusJets/Ref selection/Vertices")), //
		vertexAnalyserMuPlusJetsRefSelection_(
				new VertexAnalyser(histMan, histogramFolder + "/MuPlusJets/Ref selection/Vertices")), //
		//QCD region
		metAnalyserqcdNonIsoElectronSelection_(
				new METAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/QCD non iso e+jets/MET")), //
		metAnalyserqcdNonIsoMuonSelection_(
				new METAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets/MET")), //
		qcdNonIsoElectronAnalyser_(
				new ElectronAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/QCD non iso e+jets/Electron", true)), //
		qcdNonIsoMuonAnalyser_(
				new MuonAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets/Muon", true)), //
		metAnalyserqcdConversionSelection_(new METAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/QCDConversions/MET")), //
		qcdConversionsElectronAnalyser_(
				new ElectronAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/QCDConversions/Electron", true)), //

		jetAnalyserEPlusJetsRefSelection_(new JetAnalyser(histMan, treeMan, SelectionCriteria::ElectronPlusJetsReference, histogramFolder + "/EPlusJets/Ref selection/Jets")), //
		jetAnalyserMuPlusJetsRefSelection_(new JetAnalyser(histMan, treeMan, SelectionCriteria::MuonPlusJetsReference, histogramFolder + "/MuPlusJets/Ref selection/Jets")), //
		jetAnalyserEPlusJetsQCDNonIsoSelection_(new JetAnalyser(histMan, treeMan, SelectionCriteria::ElectronPlusJetsQCDNonIsolated, histogramFolder + "/EPlusJets/QCD non iso e+jets/Jets")), //
		jetAnalyserEPlusJetsConversionSelection_(new JetAnalyser(histMan, treeMan, SelectionCriteria::ElectronPlusJetsQCDConversion, histogramFolder + "/EPlusJets/QCDConversions/Jets")), //
		jetAnalyserMuPlusJetsQCDNonIsoSelection_( new JetAnalyser(histMan, treeMan, SelectionCriteria::MuonPlusJetsQCDNonIsolated, histogramFolder + "/MuPlusJets/QCD non iso mu+jets/Jets")), //
		wAnalyserEPlusJetsRefSelection_(new WAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/Ref selection/W Bosons")), //
		wAnalyserMuPlusJetsRefSelection_(new WAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/Ref selection/W Bosons")), //
		hitFitAnalyserEPlusJetsRefSelection_(new HitFitAnalyser(histMan, treeMan, true, histogramFolder + "/EPlusJets/Ref selection/HitFit")), //
		hitFitAnalyserMuPlusJetsRefSelection_(new HitFitAnalyser(histMan, treeMan, false, histogramFolder + "/MuPlusJets/Ref selection/HitFit")), //
		hitFitAnalyserEPlusJetsQCDSelection_(new HitFitAnalyser(histMan, treeMan, true, histogramFolder + "/EPlusJets/QCD non iso e+jets/HitFit")), //
		hitFitAnalyserEPlusJetsConversionSelection_(new HitFitAnalyser(histMan, treeMan, true, histogramFolder + "/EPlusJets/QCDConversions/HitFit")), //
		hitFitAnalyserMuPlusJetsQCDSelection_(new HitFitAnalyser(histMan, treeMan, false, histogramFolder + "/MuPlusJets/QCD non iso mu+jets/HitFit")), //		
		likelihoodRecoAnalyserEPlusJetsRefSelection_(new LikelihoodRecoAnalyser(histMan, treeMan, true, histogramFolder + "/EPlusJets/Ref selection/LikelihoodReco")), //
		likelihoodRecoAnalyserMuPlusJetsRefSelection_(new LikelihoodRecoAnalyser(histMan, treeMan, false, histogramFolder + "/MuPlusJets/Ref selection/LikelihoodReco")), //
		BTagEffAnalyserEPlusJetsRefSelection_(new BTagEff(histMan, treeMan, histogramFolder + "/EPlusJets/Ref selection/BTagEfficiencies")), //
		BTagEffAnalyserMuPlusJetsRefSelection_(new BTagEff(histMan, treeMan, histogramFolder + "/MuPlusJets/Ref selection/BTagEfficiencies")), //
		PileupAnalyserEPlusJetsRefSelection_(new PileupAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/Ref selection/Pileup")), //
		PileupAnalyserMuPlusJetsRefSelection_(new PileupAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/Ref selection/Pileup")) //
		{
}

TTbar_plus_X_analyser::~TTbar_plus_X_analyser() {
}

} /* namespace BAT */
