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

	// if (topEplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
	if ( event->PassesElectronTriggerAndSelection() ) {
		const JetCollection jets(event->CleanedJets());
		unsigned int numberOfBjets = event->getNBJets( SelectionCriteria::ElectronPlusJetsReference );
		const JetCollection bJets(event->CleanedBJets());

		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::ElectronPlusJetsReference );

		double bjetWeight = 1;
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/Ref selection");

		// const LeptonPointer signalLepton = topEplusJetsRefSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		// double efficiencyCorrection = 1;//event->isRealData() ? 1. : signalElectron->getEfficiencyCorrection(false, Globals::ElectronScaleFactorSystematic, event->runnumber());
		double electronEfficiencyCorrection = 1, electronEfficiencyCorrection_down = 1, electronEfficiencyCorrection_up = 1;
		if ( !event->isRealData() ) {
			electronEfficiencyCorrection = signalElectron->getEfficiencyCorrection( 0 );
			electronEfficiencyCorrection_down = signalElectron->getEfficiencyCorrection( -1 );
			electronEfficiencyCorrection_up = signalElectron->getEfficiencyCorrection( 1 );
		}

		histMan_->setCurrentBJetBin(numberOfBjets);
		histMan_->H1D("BTagWeights")->Fill(bjetWeight);
		histMan_->H1D("N_BJets_reweighted")->Fill(numberOfBjets, event->weight() );
		histMan_->H1D("N_BJets")->Fill(numberOfBjets, event->weight());


		// MET
		const METPointer MET_main(event->MET((METAlgorithm::value) 0));

		treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/Ref selection");
		treeMan_->Fill("EventWeight", event->weight() * electronEfficiencyCorrection);
		treeMan_->Fill("absolute_eta",fabs(signalElectron->eta()));
		treeMan_->Fill("M3",Event::M3(jets));
		if ( numberOfBjets > 0 ) {
			treeMan_->Fill("M_bl",Event::M_bl(bJets, signalElectron));
			treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalElectron));
		}
		treeMan_->Fill("HT",Event::HT(jets));
		treeMan_->Fill("MET",MET_main->et());
		treeMan_->Fill("ST",Event::ST(jets, signalElectron, MET_main));
		treeMan_->Fill("WPT",Event::WPT(signalElectron, MET_main));
		treeMan_->Fill("MT",Event::MT(signalElectron, MET_main));

		treeMan_->Fill("ElectronUp",electronEfficiencyCorrection_up);
		treeMan_->Fill("ElectronDown",electronEfficiencyCorrection_down);

		// metAnalyserEPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		// electronAnalyserRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		// vertexAnalyserEPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		// jetAnalyserEPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);

		metAnalyserEPlusJetsRefSelection_->analyse(event, signalLepton, jets);

		electronAnalyserRefSelection_->analyse(event);
		electronAnalyserRefSelection_->analyseElectron(signalElectron, event->weight());

		vertexAnalyserEPlusJetsRefSelection_->analyse(event);
		jetAnalyserEPlusJetsRefSelection_->analyse(event);

		// wAnalyserEPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		wAnalyserEPlusJetsRefSelection_->analyseHadronicW(event, jets, bJets);
		wAnalyserEPlusJetsRefSelection_->analyseHadronicW_partons(event);

		// hitFitAnalyserEPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		// BAT::TtbarHypothesis topHypothesis = hitFitAnalyserEPlusJetsRefSelection_->analyseAndReturn(event, jets, bJets, signalLepton );
		// event->setTTbarHypothesis( topHypothesis );

		// ref_selection_binned_HT_analyser_electron_->setScale(bjetWeight * efficiencyCorrection);

		vector<double> fit_variable_values;
		fit_variable_values.push_back(fabs(signalElectron->eta()));
		fit_variable_values.push_back(Event::M3(jets));
		fit_variable_values.push_back(Event::M_bl(bJets, signalElectron));
		fit_variable_values.push_back(Event::angle_bl(bJets, signalElectron));
		ref_selection_binned_HT_analyser_electron_->analyse(Event::HT(jets), fit_variable_values, event->weight());


		// if ( topHypothesis.isValid() && topHypothesis.isPhysical() ) {
		// 	ref_selection_binned_mttbar_analyser_electron_->setScale( bjetWeight * efficiencyCorrection );
		// 	ref_selection_binned_mttbar_analyser_electron_->analyse( topHypothesis.resonance->mass(), fit_variable_values, event->weight() );

		// 	ref_selection_binned_Ptttbar_analyser_electron_->setScale( bjetWeight * efficiencyCorrection );
		// 	ref_selection_binned_Ptttbar_analyser_electron_->analyse( topHypothesis.resonance->pt(), fit_variable_values, event->weight() );

		// 	ref_selection_binned_yttbar_analyser_electron_->setScale( bjetWeight * efficiencyCorrection );
		// 	ref_selection_binned_yttbar_analyser_electron_->analyse( topHypothesis.resonance->rapidity(), fit_variable_values, event->weight() );

		// 	ref_selection_binned_Pttop_analyser_electron_->setScale( bjetWeight * efficiencyCorrection );
		// 	ref_selection_binned_Pttop_analyser_electron_->analyse( topHypothesis.leptonicTop->pt(), fit_variable_values, event->weight() );
		// 	ref_selection_binned_Pttop_analyser_electron_->analyse( topHypothesis.hadronicTop->pt(), fit_variable_values, event->weight() );

		// 	ref_selection_binned_yt_analyser_electron_->setScale( bjetWeight * efficiencyCorrection );
		// 	ref_selection_binned_yt_analyser_electron_->analyse( topHypothesis.leptonicTop->rapidity(), fit_variable_values, event->weight() );
		// 	ref_selection_binned_yt_analyser_electron_->analyse( topHypothesis.hadronicTop->rapidity(), fit_variable_values, event->weight() );
		// }

		for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
			// 	if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			// 		continue;
			// 	bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
			// 	//skip MC only MET entries
			// 	if (isMCOnlyMET && event->isRealData())
			// 		continue;
			string metPrefix = METAlgorithm::names.at(metIndex);


			const METPointer met(
					event->MET((METAlgorithm::value) metIndex));
			// ref_selection_binned_MET_analyser_electron_.at(metIndex)->setScale(
			// 		bjetWeight * efficiencyCorrection);
			ref_selection_binned_MET_analyser_electron_.at(metIndex)->analyse(
					met->et(), fit_variable_values, event->weight());

			// ref_selection_binned_ST_analyser_electron_.at(metIndex)->setScale(
			// 		bjetWeight * efficiencyCorrection);
			ref_selection_binned_ST_analyser_electron_.at(metIndex)->analyse(
					Event::ST(jets, signalElectron, met),
					fit_variable_values, event->weight());

			// ref_selection_binned_MT_analyser_electron_.at(metIndex)->setScale(
			// 		bjetWeight * efficiencyCorrection);
			ref_selection_binned_MT_analyser_electron_.at(metIndex)->analyse(
					Event::MT(signalElectron, met), fit_variable_values,
					event->weight());

			// ref_selection_binned_WPT_analyser_electron_.at(metIndex)->setScale(
			// 		bjetWeight * efficiencyCorrection);
			ref_selection_binned_WPT_analyser_electron_.at(metIndex)->analyse(
					Event::WPT(signalElectron, met), fit_variable_values,
					event->weight());
		}

		// //bbar analysis part
		// histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/Ref selection");
		// if (numberOfBjets > 1) {
		// 	unsigned int numberOfCombinations(1);
		// 	for (unsigned int i = 2; i < numberOfBjets; ++i)
		// 		numberOfCombinations += i;
		// 	for (unsigned int i = 0; i < numberOfBjets; ++i) {
		// 		for (unsigned int j = i + 1; j < numberOfBjets; ++j) {
		// 			double invMass = bJets.at(i)->invariantMass(bJets.at(j));
		// 			//conserve event weight by normalising the number of combinations
		// 			double weight = event->weight() * bjetWeight * efficiencyCorrection / numberOfCombinations;
		// 			histMan_->H1D_BJetBinned("bjet_invariant_mass")->Fill(invMass, weight);

		// 		}
		// 	}
		// }
	}
}

void TTbar_plus_X_analyser::ePlusJetsQcdAnalysis(const EventPtr event) {
	if ( event->PassesElectronTriggerAndQCDSelection() ) {

		const JetCollection jets(event->CleanedJets());
		unsigned int numberOfBjets = event->getNBJets( SelectionCriteria::ElectronPlusJetsQCDNonIsolated );
		const JetCollection bJets(event->CleanedBJets());
		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::ElectronPlusJetsQCDNonIsolated );

		double bjetWeight = 1;
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);


		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));
		double efficiencyCorrection = 1;//event->isRealData() ? 1. : signalElectron->getEfficiencyCorrection(false, Globals::ElectronScaleFactorSystematic, event->runnumber()) * hadronTriggerLegCorrection;
		double electronEfficiencyCorrection = 1, electronEfficiencyCorrection_down = 1, electronEfficiencyCorrection_up = 1;
		if ( !event->isRealData() ) {
			electronEfficiencyCorrection = signalElectron->getEfficiencyCorrection( 0 );
			electronEfficiencyCorrection_down = signalElectron->getEfficiencyCorrection( -1 );
			electronEfficiencyCorrection_up = signalElectron->getEfficiencyCorrection( 1 );
		}

		// MET
		const METPointer MET_main(event->MET((METAlgorithm::value) 0));

		treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/QCD non iso e+jets");
		treeMan_->Fill("EventWeight", event->weight() * electronEfficiencyCorrection);
		treeMan_->Fill("absolute_eta",fabs(signalElectron->eta()));
		treeMan_->Fill("M3",Event::M3(jets));
		treeMan_->Fill("HT",Event::HT(jets));
		treeMan_->Fill("MET",MET_main->et());
		treeMan_->Fill("ST",Event::ST(jets, signalElectron, MET_main));
		treeMan_->Fill("WPT",Event::WPT(signalElectron, MET_main));
		treeMan_->Fill("MT",Event::MT(signalElectron, MET_main));
		if ( numberOfBjets > 0 ) {
			treeMan_->Fill("M_bl",Event::M_bl(bJets, signalElectron));
			treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalElectron));
		}

		treeMan_->Fill("ElectronUp",electronEfficiencyCorrection_up);
		treeMan_->Fill("ElectronDown",electronEfficiencyCorrection_down);

		qcdNonIsoElectronAnalyser_->setScale(bjetWeight * efficiencyCorrection);
		metAnalyserqcdNonIsoElectronSelection_->setScale(bjetWeight * efficiencyCorrection);

		qcdNonIsoElectronAnalyser_->analyse(event);
		qcdNonIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());

		metAnalyserqcdNonIsoElectronSelection_->analyse(event, signalLepton, jets);
		qcd_noniso_binned_HT_analyser_electron_->setScale(bjetWeight * efficiencyCorrection);

		vector<double> fit_variable_values;
		fit_variable_values.push_back(fabs(signalElectron->eta()));
		fit_variable_values.push_back(Event::M3(jets));
		fit_variable_values.push_back(Event::M_bl(bJets, signalElectron));
		fit_variable_values.push_back(Event::angle_bl(bJets, signalElectron));

		qcd_noniso_binned_HT_analyser_electron_->analyse(Event::HT(jets), fit_variable_values, event->weight());

		for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
			// if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			// 	continue;
			// bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
			// //skip MC only MET entries
			// if (isMCOnlyMET && event->isRealData())
			// 	continue;
			string metPrefix = METAlgorithm::names.at(metIndex);
			const METPointer met(event->MET((METAlgorithm::value) metIndex));

			qcd_noniso_binned_MET_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_MET_analyser_electron_.at(metIndex)->analyse(met->et(),
					fit_variable_values, event->weight());

			qcd_noniso_binned_ST_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_ST_analyser_electron_.at(metIndex)->analyse(Event::ST(jets, signalElectron, met),
					fit_variable_values, event->weight());

			qcd_noniso_binned_MT_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_MT_analyser_electron_.at(metIndex)->analyse(Event::MT(signalElectron, met),
					fit_variable_values, event->weight());

			qcd_noniso_binned_WPT_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_WPT_analyser_electron_.at(metIndex)->analyse(Event::WPT(signalElectron, met),
					fit_variable_values, event->weight());
		}
	}

	if ( event->PassesElectronTriggerAndConversionSelection() ) {
		
		const JetCollection jets(event->CleanedJets());
		unsigned int numberOfBjets = event->getNBJets( SelectionCriteria::ElectronPlusJetsQCDConversion );
		const JetCollection bJets(event->CleanedBJets());
		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::ElectronPlusJetsQCDConversion );

		double bjetWeight = 1;
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);

		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));
		double efficiencyCorrection = 1;//event->isRealData() ? 1. : signalElectron->getEfficiencyCorrection(false, Globals::ElectronScaleFactorSystematic, event->runnumber()) * hadronTriggerLegCorrection;
		double electronEfficiencyCorrection = 1, electronEfficiencyCorrection_down = 1, electronEfficiencyCorrection_up = 1;
		if ( !event->isRealData() ) {
			electronEfficiencyCorrection = signalElectron->getEfficiencyCorrection( 0 );
			electronEfficiencyCorrection_down = signalElectron->getEfficiencyCorrection( -1 );
			electronEfficiencyCorrection_up = signalElectron->getEfficiencyCorrection( 1 );
		}

		// MET
		const METPointer MET_main(event->MET((METAlgorithm::value) 0));

		treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/QCDConversions");
		treeMan_->Fill("EventWeight", event->weight() * electronEfficiencyCorrection);
		treeMan_->Fill("absolute_eta",fabs(signalElectron->eta()));
		treeMan_->Fill("M3",Event::M3(jets));
		if ( numberOfBjets > 0 ) {
		treeMan_->Fill("M_bl",Event::M_bl(bJets, signalElectron));
		treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalElectron));
		}
		treeMan_->Fill("HT",Event::HT(jets));
		treeMan_->Fill("MET",MET_main->et());
		treeMan_->Fill("ST",Event::ST(jets, signalElectron, MET_main));
		treeMan_->Fill("WPT",Event::WPT(signalElectron, MET_main));
		treeMan_->Fill("MT",Event::MT(signalElectron, MET_main));

		treeMan_->Fill("ElectronUp",electronEfficiencyCorrection_up);
		treeMan_->Fill("ElectronDown",electronEfficiencyCorrection_down);

		qcdConversionsElectronAnalyser_->setScale(bjetWeight * efficiencyCorrection);
		metAnalyserqcdConversionSelection_->setScale(bjetWeight * efficiencyCorrection);

		qcdConversionsElectronAnalyser_->analyse(event);
		qcdConversionsElectronAnalyser_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdConversionSelection_->analyse(event, signalLepton, jets);

		qcd_conversion_binned_HT_analyser_electron_->setScale(bjetWeight * efficiencyCorrection);
		vector<double> fit_variable_values;
		fit_variable_values.push_back(fabs(signalElectron->eta()));
		fit_variable_values.push_back(Event::M3(jets));
		fit_variable_values.push_back(Event::M_bl(bJets, signalElectron));
		fit_variable_values.push_back(Event::angle_bl(bJets, signalElectron));
		qcd_conversion_binned_HT_analyser_electron_->analyse(Event::HT(jets), fit_variable_values, event->weight());

		for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
			// if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			// 	continue;
			// bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
			// //skip MC only MET entries
			// if (isMCOnlyMET && event->isRealData())
			// 	continue;
			string metPrefix = METAlgorithm::names.at(metIndex);
			const METPointer met(event->MET((METAlgorithm::value) metIndex));

			qcd_conversion_binned_MET_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_conversion_binned_MET_analyser_electron_.at(metIndex)->analyse(met->et(),
					fit_variable_values, event->weight());

			qcd_conversion_binned_ST_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_conversion_binned_ST_analyser_electron_.at(metIndex)->analyse(
					Event::ST(jets, signalElectron, met), fit_variable_values, event->weight());

			qcd_conversion_binned_MT_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_conversion_binned_MT_analyser_electron_.at(metIndex)->analyse(Event::MT(signalElectron, met),
					fit_variable_values, event->weight());

			qcd_conversion_binned_WPT_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_conversion_binned_WPT_analyser_electron_.at(metIndex)->analyse(Event::WPT(signalElectron, met),
					fit_variable_values, event->weight());

		}
	}
}

void TTbar_plus_X_analyser::muPlusJetsSignalAnalysis(const EventPtr event) {

	// if (topMuplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
	if ( event->PassesMuonTriggerAndSelection() ) {
		const JetCollection jets(event->CleanedJets());
		unsigned int numberOfBjets = event->getNBJets( SelectionCriteria::MuonPlusJetsReference );
		const JetCollection bJets(event->CleanedBJets());

		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::MuonPlusJetsReference );

		double bjetWeight = 1;
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);
		histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/Ref selection");

		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection = 1;//event->isRealData() ? 1. : signalElectron->getEfficiencyCorrection(false, Globals::ElectronScaleFactorSystematic, event->runnumber());

		double muonEfficiencyCorrection = 1, muonEfficiencyCorrection_down = 1, muonEfficiencyCorrection_up = 1;
		if ( !event->isRealData() ) {
			muonEfficiencyCorrection = signalMuon->getEfficiencyCorrection( 0 );
			muonEfficiencyCorrection_down = signalMuon->getEfficiencyCorrection( -1 );
			muonEfficiencyCorrection_up = signalMuon->getEfficiencyCorrection( 1 );
		}

		histMan_->setCurrentBJetBin(numberOfBjets);
		histMan_->H1D("BTagWeights")->Fill(bjetWeight);
		histMan_->H1D("N_BJets_reweighted")->Fill(numberOfBjets, event->weight() * bjetWeight * efficiencyCorrection);
		histMan_->H1D("N_BJets")->Fill(numberOfBjets, event->weight() * efficiencyCorrection);


		const METPointer MET_main(event->MET((METAlgorithm::value) 0));

		treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/Ref selection");
		treeMan_->Fill("EventWeight", event->weight() * muonEfficiencyCorrection);
		treeMan_->Fill("absolute_eta",fabs(signalMuon->eta()));
		treeMan_->Fill("M3",Event::M3(jets));
		if ( numberOfBjets > 0 ) {
			treeMan_->Fill("M_bl",Event::M_bl(bJets, signalMuon));
			treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalMuon));
		}
		treeMan_->Fill("HT",Event::HT(jets));
		treeMan_->Fill("MET",MET_main->et());
		treeMan_->Fill("ST",Event::ST(jets, signalMuon, MET_main));
		treeMan_->Fill("WPT",Event::WPT(signalMuon, MET_main));
		treeMan_->Fill("MT",Event::MT(signalMuon, MET_main));

		treeMan_->Fill("MuonUp",muonEfficiencyCorrection_up);
		treeMan_->Fill("MuonDown",muonEfficiencyCorrection_down);

		metAnalyserMuPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		muonAnalyserRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		vertexAnalyserMuPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		jetAnalyserMuPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);

		metAnalyserMuPlusJetsRefSelection_->analyse(event, signalLepton, jets);

		muonAnalyserRefSelection_->analyse(event);
		muonAnalyserRefSelection_->analyseMuon(signalMuon, event->weight());

		vertexAnalyserMuPlusJetsRefSelection_->analyse(event);
		jetAnalyserMuPlusJetsRefSelection_->analyse(event);

		wAnalyserMuPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		wAnalyserMuPlusJetsRefSelection_->analyseHadronicW(event, jets, bJets);
		wAnalyserMuPlusJetsRefSelection_->analyseHadronicW_partons(event);

		// hitFitAnalyserMuPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		// BAT::TtbarHypothesis topHypothesis = hitFitAnalyserMuPlusJetsRefSelection_->analyseAndReturn(event, jets, bJets, signalLepton);
		// event->setTTbarHypothesis( topHypothesis );

		ref_selection_binned_HT_analyser_muon_->setScale(bjetWeight * efficiencyCorrection);
		vector<double> fit_variable_values;
		fit_variable_values.push_back(fabs(signalMuon->eta()));
		fit_variable_values.push_back(Event::M3(jets));
		fit_variable_values.push_back(Event::M_bl(bJets, signalMuon));
		fit_variable_values.push_back(Event::angle_bl(bJets, signalMuon));
		ref_selection_binned_HT_analyser_muon_->analyse(Event::HT(jets), fit_variable_values, event->weight());

		// if ( topHypothesis.isValid() && topHypothesis.isPhysical() ) {
		// 	ref_selection_binned_mttbar_analyser_muon_->setScale( bjetWeight * efficiencyCorrection );
		// 	ref_selection_binned_mttbar_analyser_muon_->analyse( topHypothesis.resonance->mass(), fit_variable_values, event->weight() );

		// 	ref_selection_binned_Ptttbar_analyser_muon_->setScale( bjetWeight * efficiencyCorrection );
		// 	ref_selection_binned_Ptttbar_analyser_muon_->analyse( topHypothesis.resonance->pt(), fit_variable_values, event->weight() );

		// 	ref_selection_binned_yttbar_analyser_muon_->setScale( bjetWeight * efficiencyCorrection );
		// 	ref_selection_binned_yttbar_analyser_muon_->analyse( topHypothesis.resonance->rapidity(), fit_variable_values, event->weight() );

		// 	ref_selection_binned_Pttop_analyser_muon_->setScale( bjetWeight * efficiencyCorrection );
		// 	ref_selection_binned_Pttop_analyser_muon_->analyse( topHypothesis.leptonicTop->pt(), fit_variable_values, event->weight() );
		// 	ref_selection_binned_Pttop_analyser_muon_->analyse( topHypothesis.hadronicTop->pt(), fit_variable_values, event->weight() );

		// 	ref_selection_binned_yt_analyser_muon_->setScale( bjetWeight * efficiencyCorrection );
		// 	ref_selection_binned_yt_analyser_muon_->analyse( topHypothesis.leptonicTop->rapidity(), fit_variable_values, event->weight() );
		// 	ref_selection_binned_yt_analyser_muon_->analyse( topHypothesis.hadronicTop->rapidity(), fit_variable_values, event->weight() );
		// }
		
		for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
			
			string metPrefix = METAlgorithm::names.at(metIndex);
			const METPointer met(event->MET((METAlgorithm::value) metIndex));
			ref_selection_binned_MET_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			ref_selection_binned_MET_analyser_muon_.at(metIndex)->analyse(met->et(), fit_variable_values,
					event->weight());

			ref_selection_binned_ST_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			ref_selection_binned_ST_analyser_muon_.at(metIndex)->analyse(Event::ST(jets, signalMuon, met),
					fit_variable_values, event->weight());

			ref_selection_binned_MT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			ref_selection_binned_MT_analyser_muon_.at(metIndex)->analyse(Event::MT(signalMuon, met),
					fit_variable_values, event->weight());

			ref_selection_binned_WPT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			ref_selection_binned_WPT_analyser_muon_.at(metIndex)->analyse(Event::WPT(signalMuon, met),
					fit_variable_values, event->weight());
		}
	}
}

void TTbar_plus_X_analyser::muPlusJetsQcdAnalysis(const EventPtr event) {
	//selection with respect to reference selection
	if ( event->PassesMuonTriggerAndQCDSelection() ) {

		const JetCollection jets(event->CleanedJets());
		unsigned int numberOfBjets = event->getNBJets( SelectionCriteria::MuonPlusJetsQCDNonIsolated );
		const JetCollection bJets(event->CleanedBJets());

		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::MuonPlusJetsQCDNonIsolated );

		double bjetWeight = 1;
		histMan_->setCurrentJetBin(jets.size());
		histMan_->setCurrentBJetBin(numberOfBjets);

		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection = 1.;//event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(true, Globals::MuonScaleFactorSystematic, event->runnumber());

		double muonEfficiencyCorrection = 1, muonEfficiencyCorrection_down = 1, muonEfficiencyCorrection_up = 1;
		if ( !event->isRealData() ) {
			muonEfficiencyCorrection = signalMuon->getEfficiencyCorrection( 0 );
			muonEfficiencyCorrection_down = signalMuon->getEfficiencyCorrection( -1 );
			muonEfficiencyCorrection_up = signalMuon->getEfficiencyCorrection( 1 );
		}

		const METPointer MET_main(event->MET((METAlgorithm::value) 0));

		treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets");
		treeMan_->Fill("EventWeight", event->weight() * muonEfficiencyCorrection);
		treeMan_->Fill("absolute_eta",fabs(signalMuon->eta()));
		treeMan_->Fill("M3",Event::M3(jets));
		if ( numberOfBjets > 0 ) {
			treeMan_->Fill("M_bl",Event::M_bl(bJets, signalMuon));
			treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalMuon));
		}
		treeMan_->Fill("HT",Event::HT(jets));
		treeMan_->Fill("MET",MET_main->et());
		treeMan_->Fill("ST",Event::ST(jets, signalMuon, MET_main));
		treeMan_->Fill("WPT",Event::WPT(signalMuon, MET_main));
		treeMan_->Fill("MT",Event::MT(signalMuon, MET_main));

		treeMan_->Fill("MuonUp",muonEfficiencyCorrection_up);
		treeMan_->Fill("MuonDown",muonEfficiencyCorrection_down);

		qcdNonIsoMuonAnalyser_->setScale(bjetWeight * efficiencyCorrection);
		metAnalyserqcdNonIsoMuonSelection_->setScale(bjetWeight * efficiencyCorrection);

		qcdNonIsoMuonAnalyser_->analyse(event);
		qcdNonIsoMuonAnalyser_->analyseMuon(signalMuon, event->weight());
		metAnalyserqcdNonIsoMuonSelection_->analyse(event, signalLepton, jets);

		qcd_noniso_binned_HT_analyser_muon_->setScale(bjetWeight * efficiencyCorrection);
		vector<double> fit_variable_values;
		fit_variable_values.push_back(fabs(signalMuon->eta()));
		fit_variable_values.push_back(Event::M3(jets));
		fit_variable_values.push_back(Event::M_bl(bJets, signalMuon));
		fit_variable_values.push_back(Event::angle_bl(bJets, signalMuon));
		qcd_noniso_binned_HT_analyser_muon_->analyse(Event::HT(jets), fit_variable_values, event->weight());

		for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
			// if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			// 	continue;
			// bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
			// //skip MC only MET entries
			// if (isMCOnlyMET && event->isRealData())
			// 	continue;
			string metPrefix = METAlgorithm::names.at(metIndex);

			const METPointer met(event->MET((METAlgorithm::value) metIndex));
			qcd_noniso_binned_MET_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_MET_analyser_muon_.at(metIndex)->analyse(met->et(), fit_variable_values,
					event->weight());

			qcd_noniso_binned_ST_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_ST_analyser_muon_.at(metIndex)->analyse(Event::ST(jets, signalMuon, met),
					fit_variable_values, event->weight());

			qcd_noniso_binned_MT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_MT_analyser_muon_.at(metIndex)->analyse(Event::MT(signalMuon, met),
					fit_variable_values, event->weight());

			qcd_noniso_binned_WPT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_WPT_analyser_muon_.at(metIndex)->analyse(Event::WPT(signalMuon, met),
					fit_variable_values, event->weight());

		}

	}
}

void TTbar_plus_X_analyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/Ref selection");
	histMan_->addH1D("N_BJets", "# of b-Jets; # of b-Jet; Events", 11, -0.5, 10.5);
	histMan_->addH1D("N_BJets_reweighted", "# of b-Jets; # of b-Jet; Events", 11, -0.5, 10.5);
	histMan_->addH1D("BTagWeights", "BTag weights; BTag weight; Events", 300, -1, 2);

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/Ref selection");
	histMan_->addH1D("N_BJets", "# of b-Jets; # of b-Jet; Events", 11, -0.5, 10.5);
	histMan_->addH1D("N_BJets_reweighted", "# of b-Jets; # of b-Jet; Events", 11, -0.5, 10.5);
	histMan_->addH1D("BTagWeights", "BTag weights; BTag weight; Events", 300, -1, 2);

	treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/Ref selection");
	treeMan_->addBranch("HT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("absolute_eta", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M3", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("angle_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronUp", "F", "SystematicWeights" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronDown", "F", "SystematicWeights" + Globals::treePrefix_);

	treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/QCD non iso e+jets");
	treeMan_->addBranch("HT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("absolute_eta", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M3", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("angle_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronUp", "F", "SystematicWeights" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronDown", "F", "SystematicWeights" + Globals::treePrefix_);

	treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/QCDConversions");
	treeMan_->addBranch("HT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("absolute_eta", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M3", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("angle_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronUp", "F", "SystematicWeights" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronDown", "F", "SystematicWeights" + Globals::treePrefix_);

	treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/Ref selection");
	treeMan_->addBranch("HT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("absolute_eta", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M3", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("angle_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MuonUp", "F", "SystematicWeights" + Globals::treePrefix_);
	treeMan_->addBranch("MuonDown", "F", "SystematicWeights" + Globals::treePrefix_);

	treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets");
	treeMan_->addBranch("HT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("absolute_eta", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M3", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("M_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("angle_bl", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MuonUp", "F", "SystematicWeights" + Globals::treePrefix_);
	treeMan_->addBranch("MuonDown", "F", "SystematicWeights" + Globals::treePrefix_);

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
	jetAnalyserEPlusJetsRefSelection_->createTrees();
	jetAnalyserMuPlusJetsRefSelection_->createTrees();

	// W boson simple reconstruction
	wAnalyserEPlusJetsRefSelection_->createHistograms();
	wAnalyserMuPlusJetsRefSelection_->createHistograms();
	wAnalyserEPlusJetsRefSelection_->createTrees();
	wAnalyserMuPlusJetsRefSelection_->createTrees();

	hitFitAnalyserEPlusJetsRefSelection_->createHistograms();
	hitFitAnalyserMuPlusJetsRefSelection_->createHistograms();
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
		metBins_(), //
		ht_bins_(), //
		st_bins_(), //
		mt_bins_(), //
		wpt_bins_(), //
		//MET
		ref_selection_binned_MET_analyser_electron_(), //
		qcd_conversion_binned_MET_analyser_electron_(), //
		qcd_noniso_binned_MET_analyser_electron_(), //
		ref_selection_binned_MET_analyser_muon_(), //
		qcd_noniso_binned_MET_analyser_muon_(), //

		//HT
		ref_selection_binned_HT_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_HT_Analysis")), //
		qcd_conversion_binned_HT_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_HT_Analysis")), //
		qcd_noniso_binned_HT_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_HT_Analysis")), //
		//HT muons
		ref_selection_binned_HT_analyser_muon_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_HT_Analysis")), //
		qcd_noniso_binned_HT_analyser_muon_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets/Binned_HT_Analysis")), //
		//ST
		ref_selection_binned_ST_analyser_electron_(), //
		qcd_conversion_binned_ST_analyser_electron_(), //
		qcd_noniso_binned_ST_analyser_electron_(), //
		//ST
		ref_selection_binned_ST_analyser_muon_(), //
		qcd_noniso_binned_ST_analyser_muon_(), //
		//MT electrons
		ref_selection_binned_MT_analyser_electron_(), //
		qcd_conversion_binned_MT_analyser_electron_(), //
		qcd_noniso_binned_MT_analyser_electron_(), //
		//MT muons
		ref_selection_binned_MT_analyser_muon_(), //
		qcd_noniso_binned_MT_analyser_muon_(), //
		//WPT electrons
		ref_selection_binned_WPT_analyser_electron_(), //
		qcd_conversion_binned_WPT_analyser_electron_(), //
		qcd_noniso_binned_WPT_analyser_electron_(), //
		//WPT muons
		ref_selection_binned_WPT_analyser_muon_(), //
		qcd_noniso_binned_WPT_analyser_muon_(), //

		// mttbar
		ref_selection_binned_mttbar_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_mttbar_Analysis")), //
		qcd_conversion_binned_mttbar_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_mttbar_Analysis")), //
		qcd_noniso_binned_mttbar_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_mttbar_Analysis")), //
		ref_selection_binned_mttbar_analyser_muon_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_mttbar_Analysis")), //

		// Yttbar
		ref_selection_binned_yttbar_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_yttbar_Analysis")), //
		qcd_conversion_binned_yttbar_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_yttbar_Analysis")), //
		qcd_noniso_binned_yttbar_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_yttbar_Analysis")), //
		ref_selection_binned_yttbar_analyser_muon_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_yttbar_Analysis")), //

		// Pttbar
		ref_selection_binned_Ptttbar_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_Ptttbar_Analysis")), //
		qcd_conversion_binned_Ptttbar_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_Ptttbar_Analysis")), //
		qcd_noniso_binned_Ptttbar_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_Ptttbar_Analysis")), //
		ref_selection_binned_Ptttbar_analyser_muon_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_Ptttbar_Analysis")), //

		// Pttop
		ref_selection_binned_Pttop_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_Pttop_Analysis")), //
		qcd_conversion_binned_Pttop_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_Pttop_Analysis")), //
		qcd_noniso_binned_Pttop_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_Pttop_Analysis")), //
		ref_selection_binned_Pttop_analyser_muon_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_Pttop_Analysis")), //

		// yt
		ref_selection_binned_yt_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_yt_Analysis")), //
		qcd_conversion_binned_yt_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_yt_Analysis")), //
		qcd_noniso_binned_yt_analyser_electron_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_yt_Analysis")), //
		ref_selection_binned_yt_analyser_muon_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_yt_Analysis")), //

		jetAnalyserEPlusJetsRefSelection_(new JetAnalyser(histMan, treeMan, SelectionCriteria::ElectronPlusJetsReference, histogramFolder + "/EPlusJets/Ref selection/Jets")), //
		jetAnalyserMuPlusJetsRefSelection_(
				new JetAnalyser(histMan, treeMan, SelectionCriteria::MuonPlusJetsReference, histogramFolder + "/MuPlusJets/Ref selection/Jets")), //
		wAnalyserEPlusJetsRefSelection_(new WAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/Ref selection/W Bosons")), //
		wAnalyserMuPlusJetsRefSelection_(new WAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/Ref selection/W Bosons")), //
		hitFitAnalyserEPlusJetsRefSelection_(new HitFitAnalyser(histMan, true, histogramFolder + "/EPlusJets/Ref selection/HitFit")), //
		hitFitAnalyserMuPlusJetsRefSelection_(new HitFitAnalyser(histMan, false, histogramFolder + "/MuPlusJets/Ref selection/HitFit")), //
		electron_variables_(), //
		muon_variables_() {

	//bins: start is always assumed to be 0 and the last is read as > X
	//MET bins:
	// very old: 25, 45, 70, 100, 150, inf
	// old: [0.0, 31.0, 58.0, 96.0, 142.0, 191.0, inf]
	// new: [0.0, 27.0, 52.0, 87.0, 129.0, 171.0, inf]
	metBins_.push_back(27.);
	metBins_.push_back(52.);
	metBins_.push_back(87.);
	metBins_.push_back(129.);
	metBins_.push_back(171.);
	//HT:
	// very old: 50, 150, 250, 350, 450, 650, 1100, inf
	// old: [0.0, 190.0, 225.0, 262.0, 302.0, 345.0, 392.0, 445.0, 501.0, 562.0, 623.0, 689.0, 766.0, inf]
	// new: [0.0, 187.0, 217.0, 250.0, 287.0, 327.0, 369.0, 414.0, 464.0, 517.0, 575.0, 634.0, 696.0, 772.0, inf]
	ht_bins_.push_back(187.0);
	ht_bins_.push_back(217.0);
	ht_bins_.push_back(250.0);
	ht_bins_.push_back(287.0);
	ht_bins_.push_back(327.0);
	ht_bins_.push_back(369.0);
	ht_bins_.push_back(414.0);
	ht_bins_.push_back(464.0);
	ht_bins_.push_back(517.0);
	ht_bins_.push_back(575.0);
	ht_bins_.push_back(634.0);
	ht_bins_.push_back(696.0);
	ht_bins_.push_back(772.0);

	//ST:
	// very old: 150, 250, 350, 450, 550, 750, 1250, inf new: 350 & 400 & 450 & 500 & 580 & 700 & inf
	// old: [0.0, 285.0, 329.0, 376.0, 428.0, 484.0, 544.0, 609.0, 678.0, 751.0, 830.0, 911.0, 1028.0, inf]
	// new: [0.0, 281.0, 324.0, 367.0, 415.0, 466.0, 521.0, 581.0, 645.0, 714.0, 783.0, 861.0, 946.0, inf]
	st_bins_.push_back(281.0);
	st_bins_.push_back(324.0);
	st_bins_.push_back(367.0);
	st_bins_.push_back(415.0);
	st_bins_.push_back(466.0);
	st_bins_.push_back(521.0);
	st_bins_.push_back(581.0);
	st_bins_.push_back(645.0);
	st_bins_.push_back(714.0);
	st_bins_.push_back(783.0);
	st_bins_.push_back(861.0);
	st_bins_.push_back(946.0);

	//MT:
	// very old: 0, 40, 65, 85, 150, inf
	// old: [0.0, 28.0, 66.0, inf]
	// new: [0.0, 23.0, 58.0, inf]
	mt_bins_.push_back(23.0);
	mt_bins_.push_back(58.0);

	//WPT:
	// very old: 0, 40, 70, 100, 130, 170 inf
	// old: [0.0, 31.0, 59.0, 88.0, 118.0, 151.0, 187.0, 227.0, 267.0, inf]
	// new: [0.0, 27.0, 52.0, 78.0, 105.0, 134.0, 165.0, 198.0, 235.0, inf]
	wpt_bins_.push_back(27.0);
	wpt_bins_.push_back(52.0);
	wpt_bins_.push_back(78.0);
	wpt_bins_.push_back(105.0);
	wpt_bins_.push_back(134.0);
	wpt_bins_.push_back(165.0);
	wpt_bins_.push_back(198.0);
	wpt_bins_.push_back(235.0);

	// mttbar
	// Estimated from previous papers
	mttbar_bins_.push_back(350.);
	mttbar_bins_.push_back(400.);
	mttbar_bins_.push_back(450.);
	mttbar_bins_.push_back(530.);
	mttbar_bins_.push_back(650.);
	mttbar_bins_.push_back(800.);
	mttbar_bins_.push_back(1100.);
	mttbar_bins_.push_back(1600.);

	// pt ttbar
	pttbar_bins_.push_back(25.);
	pttbar_bins_.push_back(50.);
	pttbar_bins_.push_back(75.);
	pttbar_bins_.push_back(125.);
	pttbar_bins_.push_back(180.);
	pttbar_bins_.push_back(300.);

	// yttbar
	yttbar_bins_.push_back(-2.5);
	yttbar_bins_.push_back(-1.3);
	yttbar_bins_.push_back(-0.9);
	yttbar_bins_.push_back(-0.5);
	yttbar_bins_.push_back(-0.25);
	yttbar_bins_.push_back(0.);
	yttbar_bins_.push_back(0.25);
	yttbar_bins_.push_back(0.5);
	yttbar_bins_.push_back(0.9);
	yttbar_bins_.push_back(1.3);
	yttbar_bins_.push_back(2.5);

	// pt top
	pttop_bins_.push_back(60.);
	pttop_bins_.push_back(100.);
	pttop_bins_.push_back(140.);
	pttop_bins_.push_back(200.);
	pttop_bins_.push_back(260.);
	pttop_bins_.push_back(325.);
	pttop_bins_.push_back(400.);

	// yt
	yt_bins_.push_back(-2.5);
	yt_bins_.push_back(-1.3);
	yt_bins_.push_back(-0.9);
	yt_bins_.push_back(-0.5);
	yt_bins_.push_back(-0.25);
	yt_bins_.push_back(0.);
	yt_bins_.push_back(0.25);
	yt_bins_.push_back(0.5);
	yt_bins_.push_back(0.9);
	yt_bins_.push_back(1.3);
	yt_bins_.push_back(2.5);

	Variable electron_absolute_eta = Variable("electron_absolute_eta", 30, 0.0, 3.0);
	Variable muon_absolute_eta = Variable("muon_absolute_eta", 30, 0.0, 3.0);
	Variable M3 = Variable("M3", 200, 0.0, 1000.);
	Variable M_bl = Variable("M_bl", 200, 0.0, 1000.);
	Variable angle_bl = Variable("angle_bl", 40, 0, 4.);
	electron_variables_.push_back(electron_absolute_eta);
	muon_variables_.push_back(muon_absolute_eta);
	electron_variables_.push_back(M3);
	muon_variables_.push_back(M3);
	electron_variables_.push_back(M_bl);
	muon_variables_.push_back(M_bl);
	electron_variables_.push_back(angle_bl);
	muon_variables_.push_back(angle_bl);

	make_binned_MET_analysers();
	make_binned_HT_analysers();
	make_binned_ST_analysers();
	make_binned_MT_analysers();
	make_binned_WPT_analysers();

	make_binned_mttbar_analysers();
	make_binned_yttbar_analysers();
	make_binned_ptttbar_analysers();
	make_binned_pttop_analysers();
	make_binned_yt_analysers();
}

TTbar_plus_X_analyser::~TTbar_plus_X_analyser() {
}

void TTbar_plus_X_analyser::make_binned_MET_analysers() {
	//for all MET types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			continue;
		string metPrefix = METAlgorithm::names.at(metIndex);

		Binned_Variable_analyser_ptr ref_selection_binned_met_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_conversion_binned_met_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_met_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_MET_Analysis"));

		Variable MET = Variable(metPrefix, metBins_);
		ref_selection_binned_met_electron_analyser->set_variables(MET, electron_variables_);
		qcd_conversion_binned_met_electron_analyser->set_variables(MET, electron_variables_);
		qcd_noniso_binned_met_electron_analyser->set_variables(MET, electron_variables_);

		ref_selection_binned_MET_analyser_electron_.push_back(ref_selection_binned_met_electron_analyser);
		qcd_conversion_binned_MET_analyser_electron_.push_back(qcd_conversion_binned_met_electron_analyser);
		qcd_noniso_binned_MET_analyser_electron_.push_back(qcd_noniso_binned_met_electron_analyser);

		//binned MET muons
		Binned_Variable_analyser_ptr ref_selection_binned_met_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_met_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets/Binned_MET_Analysis"));

		ref_selection_binned_met_muon_analyser->set_variables(MET, muon_variables_);
		qcd_noniso_binned_met_muon_analyser->set_variables(MET, muon_variables_);

		ref_selection_binned_MET_analyser_muon_.push_back(ref_selection_binned_met_muon_analyser);
		qcd_noniso_binned_MET_analyser_muon_.push_back(qcd_noniso_binned_met_muon_analyser);

	}
}

void TTbar_plus_X_analyser::make_binned_HT_analysers() {
	// multi-variable analysers
	Variable HT = Variable("HT", ht_bins_);
	ref_selection_binned_HT_analyser_electron_->set_variables(HT, electron_variables_);
	ref_selection_binned_HT_analyser_muon_->set_variables(HT, muon_variables_);

	qcd_noniso_binned_HT_analyser_electron_->set_variables(HT, electron_variables_);
	qcd_noniso_binned_HT_analyser_muon_->set_variables(HT, muon_variables_);

	qcd_conversion_binned_HT_analyser_electron_->set_variables(HT, electron_variables_);
}

void TTbar_plus_X_analyser::make_binned_ST_analysers() {
	//for all ST types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			continue;
		string metPrefix = METAlgorithm::names.at(metIndex);
		Variable ST = Variable("ST_with_" + metPrefix, st_bins_);
		Binned_Variable_analyser_ptr ref_selection_binned_ST_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_conversion_binned_ST_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_ST_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_ST_Analysis"));
		ref_selection_binned_ST_electron_analyser->set_variables(ST, electron_variables_);

		qcd_conversion_binned_ST_electron_analyser->set_variables(ST, electron_variables_);
		qcd_noniso_binned_ST_electron_analyser->set_variables(ST, electron_variables_);
		ref_selection_binned_ST_analyser_electron_.push_back(ref_selection_binned_ST_electron_analyser);
		qcd_conversion_binned_ST_analyser_electron_.push_back(qcd_conversion_binned_ST_electron_analyser);
		qcd_noniso_binned_ST_analyser_electron_.push_back(qcd_noniso_binned_ST_electron_analyser);

		//binned ST muons
		Binned_Variable_analyser_ptr ref_selection_binned_ST_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_ST_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets/Binned_ST_Analysis"));

		ref_selection_binned_ST_muon_analyser->set_variables(ST, muon_variables_);
		qcd_noniso_binned_ST_muon_analyser->set_variables(ST, muon_variables_);

		ref_selection_binned_ST_analyser_muon_.push_back(ref_selection_binned_ST_muon_analyser);
		qcd_noniso_binned_ST_analyser_muon_.push_back(qcd_noniso_binned_ST_muon_analyser);

	}
}

void TTbar_plus_X_analyser::make_binned_MT_analysers() {
	//for all MT types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			continue;
		string metPrefix = METAlgorithm::names.at(metIndex);
		Variable MT = Variable("MT_with_" + metPrefix, mt_bins_);
		Binned_Variable_analyser_ptr ref_selection_binned_MT_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_conversion_binned_MT_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_MT_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_MT_Analysis"));
		ref_selection_binned_MT_electron_analyser->set_variables(MT, electron_variables_);
		qcd_conversion_binned_MT_electron_analyser->set_variables(MT, electron_variables_);
		qcd_noniso_binned_MT_electron_analyser->set_variables(MT, electron_variables_);

		ref_selection_binned_MT_analyser_electron_.push_back(ref_selection_binned_MT_electron_analyser);
		qcd_conversion_binned_MT_analyser_electron_.push_back(qcd_conversion_binned_MT_electron_analyser);
		qcd_noniso_binned_MT_analyser_electron_.push_back(qcd_noniso_binned_MT_electron_analyser);

		//binned MT muons
		Binned_Variable_analyser_ptr ref_selection_binned_MT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_MT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets/Binned_MT_Analysis"));

		ref_selection_binned_MT_muon_analyser->set_variables(MT, muon_variables_);
		qcd_noniso_binned_MT_muon_analyser->set_variables(MT, muon_variables_);

		ref_selection_binned_MT_analyser_muon_.push_back(ref_selection_binned_MT_muon_analyser);
		qcd_noniso_binned_MT_analyser_muon_.push_back(qcd_noniso_binned_MT_muon_analyser);

	}
}

void TTbar_plus_X_analyser::make_binned_WPT_analysers() {
	//for all WPT types!!
	for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
		if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			continue;
		string metPrefix = METAlgorithm::names.at(metIndex);
		Variable WPT = Variable("WPT_with_" + metPrefix, wpt_bins_);
		Binned_Variable_analyser_ptr ref_selection_binned_WPT_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/Ref selection/Binned_WPT_Analysis"));
		Binned_Variable_analyser_ptr qcd_conversion_binned_WPT_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCDConversions/Binned_WPT_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_WPT_electron_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/EPlusJets/QCD non iso e+jets/Binned_WPT_Analysis"));
		ref_selection_binned_WPT_electron_analyser->set_variables(WPT, electron_variables_);
		qcd_conversion_binned_WPT_electron_analyser->set_variables(WPT, electron_variables_);
		qcd_noniso_binned_WPT_electron_analyser->set_variables(WPT, electron_variables_);
		ref_selection_binned_WPT_analyser_electron_.push_back(ref_selection_binned_WPT_electron_analyser);
		qcd_conversion_binned_WPT_analyser_electron_.push_back(qcd_conversion_binned_WPT_electron_analyser);
		qcd_noniso_binned_WPT_analyser_electron_.push_back(qcd_noniso_binned_WPT_electron_analyser);

		//binned WPT muons
		Binned_Variable_analyser_ptr no_selection_binned_WPT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/No selection/Binned_WPT_Analysis"));
		Binned_Variable_analyser_ptr ref_selection_binned_WPT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/Ref selection/Binned_WPT_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_binned_WPT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets/Binned_WPT_Analysis"));

		no_selection_binned_WPT_muon_analyser->set_variables(WPT, muon_variables_);
		ref_selection_binned_WPT_muon_analyser->set_variables(WPT, muon_variables_);
		qcd_noniso_binned_WPT_muon_analyser->set_variables(WPT, muon_variables_);

		ref_selection_binned_WPT_analyser_muon_.push_back(ref_selection_binned_WPT_muon_analyser);
		qcd_noniso_binned_WPT_analyser_muon_.push_back(qcd_noniso_binned_WPT_muon_analyser);

	}
}

void TTbar_plus_X_analyser::make_binned_mttbar_analysers() {
	// multi-variable analysers
	Variable mttbar = Variable("mttbar", mttbar_bins_);
	ref_selection_binned_mttbar_analyser_electron_->set_variables(mttbar, electron_variables_);

	qcd_noniso_binned_mttbar_analyser_electron_->set_variables(mttbar, electron_variables_);

	qcd_conversion_binned_mttbar_analyser_electron_->set_variables(mttbar, electron_variables_);

	ref_selection_binned_mttbar_analyser_muon_->set_variables(mttbar, muon_variables_);
}

void TTbar_plus_X_analyser::make_binned_yttbar_analysers() {
	// multi-variable analysers
	Variable yttbar = Variable("yttbar", yttbar_bins_);
	ref_selection_binned_yttbar_analyser_electron_->set_variables(yttbar, electron_variables_);

	qcd_noniso_binned_yttbar_analyser_electron_->set_variables(yttbar, electron_variables_);

	qcd_conversion_binned_yttbar_analyser_electron_->set_variables(yttbar, electron_variables_);

	ref_selection_binned_yttbar_analyser_muon_->set_variables(yttbar, muon_variables_);
}

void TTbar_plus_X_analyser::make_binned_ptttbar_analysers() {
	// multi-variable analysers
	Variable ptttbar = Variable("ptttbar", pttbar_bins_);
	ref_selection_binned_Ptttbar_analyser_electron_->set_variables(ptttbar, electron_variables_);

	qcd_noniso_binned_Ptttbar_analyser_electron_->set_variables(ptttbar, electron_variables_);

	qcd_conversion_binned_Ptttbar_analyser_electron_->set_variables(ptttbar, electron_variables_);

	ref_selection_binned_Ptttbar_analyser_muon_->set_variables(ptttbar, muon_variables_);
}

void TTbar_plus_X_analyser::make_binned_pttop_analysers() {
	// multi-variable analysers
	Variable pttop = Variable("pttop", mttbar_bins_);
	ref_selection_binned_Pttop_analyser_electron_->set_variables(pttop, electron_variables_);

	qcd_noniso_binned_Pttop_analyser_electron_->set_variables(pttop, electron_variables_);

	qcd_conversion_binned_Pttop_analyser_electron_->set_variables(pttop, electron_variables_);

	ref_selection_binned_Pttop_analyser_muon_->set_variables(pttop, muon_variables_);
}

void TTbar_plus_X_analyser::make_binned_yt_analysers() {
	// multi-variable analysers
	Variable yt = Variable("yt", yt_bins_);
	ref_selection_binned_yt_analyser_electron_->set_variables(yt, electron_variables_);

	qcd_noniso_binned_yt_analyser_electron_->set_variables(yt, electron_variables_);

	qcd_conversion_binned_yt_analyser_electron_->set_variables(yt, electron_variables_);

	ref_selection_binned_yt_analyser_muon_->set_variables(yt, muon_variables_);
}

} /* namespace BAT */
