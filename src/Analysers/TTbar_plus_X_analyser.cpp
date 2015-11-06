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
	}

	// if (topEplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
	if ( event->PassesElectronTriggerAndSelection() ) {
		const JetCollection jets(event->CleanedJets());
		unsigned int numberOfBjets = event->getNBJets( SelectionCriteria::ElectronPlusJetsReference );
		const JetCollection bJets(event->CleanedBJets());

		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::ElectronPlusJetsReference );

		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		double electronEfficiencyCorrection = 1;
		double electronEfficiencyCorrection_down = 1, electronEfficiencyCorrection_up = 1;
		if ( !event->isRealData() ) {
			electronEfficiencyCorrection = signalElectron->getEfficiencyCorrection( 0 );
			electronEfficiencyCorrection_down = signalElectron->getEfficiencyCorrection( -1 );
			electronEfficiencyCorrection_up = signalElectron->getEfficiencyCorrection( 1 );
		}

		// MET
		const METPointer MET_original(event->MET((METAlgorithm::value) 0));
		// const METPointer METNoHF(event->MET((METAlgorithm::value) 1));

		treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/Ref selection");
		treeMan_->Fill("EventWeight", event->weight());
		treeMan_->Fill("PUWeight", event->PileUpWeight());
		treeMan_->Fill("lepton_eta",signalElectron->eta());
		treeMan_->Fill("lepton_pt",signalElectron->pt());
		treeMan_->Fill("lepton_charge",signalElectron->charge());
		treeMan_->Fill("M3",Event::M3(jets));
		if ( numberOfBjets > 0 ) {
			treeMan_->Fill("M_bl",Event::M_bl(bJets, signalElectron));
			treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalElectron));
		}
		treeMan_->Fill("HT",Event::HT(jets));
		treeMan_->Fill("MET",MET_original->et());
		// treeMan_->Fill("METNoHF",METNoHF->et());
		treeMan_->Fill("ST",Event::ST(jets, signalElectron, MET_original));
		// treeMan_->Fill("STNoHF",Event::ST(jets, signalElectron, METNoHF));
		treeMan_->Fill("WPT",Event::WPT(signalElectron, MET_original));
		// treeMan_->Fill("WPTNoHF",Event::WPT(signalElectron, METNoHF));
		treeMan_->Fill("MT",Event::MT(signalElectron, MET_original));
		treeMan_->Fill("NJets",Event::NJets(jets));
		treeMan_->Fill("NBJets",Event::NJets(bJets));
		treeMan_->Fill("NVertices",	event->Vertices().size());

		treeMan_->Fill("ElectronEfficiencyCorrection",electronEfficiencyCorrection);
		treeMan_->Fill("ElectronUp",electronEfficiencyCorrection_up);
		treeMan_->Fill("ElectronDown",electronEfficiencyCorrection_down);
		treeMan_->Fill("MuonUp",1.0);
		treeMan_->Fill("MuonDown",1.0);

		treeMan_->Fill("BJetWeight",event->BJetWeight());
		treeMan_->Fill("BJetUpWeight",event->BJetUpWeight());
		treeMan_->Fill("BJetDownWeight",event->BJetDownWeight());

		treeMan_->Fill("isTightElectron",signalElectron->isTightConversionElectron());
		treeMan_->Fill("sigmaIetaIeta",signalElectron->sigmaIEtaIEta());

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

		// MET Uncertainties
		for ( unsigned int unc_i = 0; unc_i < MET_original->getAllMETUncertainties().size(); ++unc_i ) {
			METPointer METForUnc_i = MET_original->getMETForUncertainty( unc_i );
			treeMan_->Fill("MET_METUncertainties",METForUnc_i->et());
			treeMan_->Fill("ST_METUncertainties",Event::ST(jets, signalElectron, METForUnc_i));
			treeMan_->Fill("WPT_METUncertainties",Event::WPT(signalElectron, METForUnc_i));
		}

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
		if ( ! event->isRealData() ) {
			wAnalyserEPlusJetsRefSelection_->analyseHadronicW_partons(event);			
		}

		likelihoodRecoAnalyserEPlusJetsRefSelection_->analyse(event, jets, bJets, signalLepton, MET_original);

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

		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		// MET
		const METPointer MET_original(event->MET((METAlgorithm::value) 0));
		// const METPointer METNoHF(event->MET((METAlgorithm::value) 1));

		treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/QCD non iso e+jets");
		treeMan_->Fill("EventWeight", event->weight());
		treeMan_->Fill("PUWeight", event->PileUpWeight());
		treeMan_->Fill("lepton_eta",signalElectron->eta());
		treeMan_->Fill("lepton_pt",signalElectron->pt());
		treeMan_->Fill("lepton_charge",signalElectron->charge());
		treeMan_->Fill("M3",Event::M3(jets));
		treeMan_->Fill("HT",Event::HT(jets));
		treeMan_->Fill("MET",MET_original->et());
		// treeMan_->Fill("METNoHF",METNoHF->et());
		treeMan_->Fill("ST",Event::ST(jets, signalElectron, MET_original));
		// treeMan_->Fill("STNoHF",Event::ST(jets, signalElectron, METNoHF));
		treeMan_->Fill("WPT",Event::WPT(signalElectron, MET_original));
		// treeMan_->Fill("WPTNoHF",Event::WPT(signalElectron, METNoHF));
		treeMan_->Fill("MT",Event::MT(signalElectron, MET_original));
		treeMan_->Fill("NJets",Event::NJets(jets));
		treeMan_->Fill("NBJets",Event::NJets(bJets));
		treeMan_->Fill("NVertices",	event->Vertices().size());
		if ( numberOfBjets > 0 ) {
			treeMan_->Fill("M_bl",Event::M_bl(bJets, signalElectron));
			treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalElectron));
		}

		treeMan_->Fill("ElectronEfficiencyCorrection",1.0);
		treeMan_->Fill("ElectronUp",1.0);
		treeMan_->Fill("ElectronDown",1.0);
		treeMan_->Fill("MuonEfficiencyCorrection",1.0);
		treeMan_->Fill("MuonUp",1.0);
		treeMan_->Fill("MuonDown",1.0);
		treeMan_->Fill("BJetWeight",event->BJetWeight());
		treeMan_->Fill("BJetUpWeight",event->BJetUpWeight());
		treeMan_->Fill("BJetDownWeight",event->BJetDownWeight());

		treeMan_->Fill("isTightElectron",signalElectron->isTightConversionElectron());
		treeMan_->Fill("sigmaIetaIeta",signalElectron->sigmaIEtaIEta());

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

		// MET Uncertainties
		for ( unsigned int unc_i = 0; unc_i < MET_original->getAllMETUncertainties().size(); ++unc_i ) {
			METPointer METForUnc_i = MET_original->getMETForUncertainty( unc_i );
			treeMan_->Fill("MET_METUncertainties",METForUnc_i->et());
			treeMan_->Fill("ST_METUncertainties",Event::ST(jets, signalElectron, METForUnc_i));
			treeMan_->Fill("WPT_METUncertainties",Event::WPT(signalElectron, METForUnc_i));
		}

		// qcdNonIsoElectronAnalyser_->setScale(bjetWeight * efficiencyCorrection);
		// metAnalyserqcdNonIsoElectronSelection_->setScale(bjetWeight * efficiencyCorrection);

		qcdNonIsoElectronAnalyser_->analyse(event);
		qcdNonIsoElectronAnalyser_->analyseElectron(signalElectron, event->weight());

		metAnalyserqcdNonIsoElectronSelection_->analyse(event, signalLepton, jets);
		// qcd_noniso_binned_HT_analyser_electron_->setScale(bjetWeight * efficiencyCorrection);

		jetAnalyserEPlusJetsQCDNonIsoSelection_->analyse(event);

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

			// qcd_noniso_binned_MET_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_MET_analyser_electron_.at(metIndex)->analyse(met->et(),
					fit_variable_values, event->weight());

			// qcd_noniso_binned_ST_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_ST_analyser_electron_.at(metIndex)->analyse(Event::ST(jets, signalElectron, met),
					fit_variable_values, event->weight());

			// qcd_noniso_binned_MT_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_MT_analyser_electron_.at(metIndex)->analyse(Event::MT(signalElectron, met),
					fit_variable_values, event->weight());

			// qcd_noniso_binned_WPT_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_binned_WPT_analyser_electron_.at(metIndex)->analyse(Event::WPT(signalElectron, met),
					fit_variable_values, event->weight());
		}
	}

	if ( event->PassesElectronTriggerAndConversionSelection() ) {
		
		const JetCollection jets(event->CleanedJets());
		unsigned int numberOfBjets = event->getNBJets( SelectionCriteria::ElectronPlusJetsQCDConversion );
		const JetCollection bJets(event->CleanedBJets());
		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::ElectronPlusJetsQCDConversion );

		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));

		// MET
		const METPointer MET_original(event->MET((METAlgorithm::value) 0));
		// const METPointer METNoHF(event->MET((METAlgorithm::value) 1));

		treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/QCDConversions");
		treeMan_->Fill("EventWeight", event->weight() );
		treeMan_->Fill("PUWeight", event->PileUpWeight() );
		treeMan_->Fill("lepton_eta",signalElectron->eta());
		treeMan_->Fill("lepton_pt",signalElectron->pt());
		treeMan_->Fill("lepton_charge",signalElectron->charge());
		treeMan_->Fill("M3",Event::M3(jets));
		if ( numberOfBjets > 0 ) {
		treeMan_->Fill("M_bl",Event::M_bl(bJets, signalElectron));
		treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalElectron));
		}
		treeMan_->Fill("HT",Event::HT(jets));
		treeMan_->Fill("MET",MET_original->et());
		// treeMan_->Fill("METNoHF",METNoHF->et());
		treeMan_->Fill("ST",Event::ST(jets, signalElectron, MET_original));
		// treeMan_->Fill("STNoHF",Event::ST(jets, signalElectron, METNoHF));
		treeMan_->Fill("WPT",Event::WPT(signalElectron, MET_original));
		// treeMan_->Fill("WPTNoHF",Event::WPT(signalElectron, METNoHF));
		treeMan_->Fill("MT",Event::MT(signalElectron, MET_original));
		treeMan_->Fill("NJets",Event::NJets(jets));
		treeMan_->Fill("NBJets",Event::NJets(bJets));
		treeMan_->Fill("NVertices",	event->Vertices().size());

		treeMan_->Fill("ElectronEfficiencyCorrection",1.0);
		treeMan_->Fill("ElectronUp",1.0);
		treeMan_->Fill("ElectronDown",1.0);
		treeMan_->Fill("MuonEfficiencyCorrection",1.0);
		treeMan_->Fill("MuonUp",1.0);
		treeMan_->Fill("MuonDown",1.0);
		treeMan_->Fill("BJetWeight",event->BJetWeight());
		treeMan_->Fill("BJetUpWeight",event->BJetUpWeight());
		treeMan_->Fill("BJetDownWeight",event->BJetDownWeight());

		treeMan_->Fill("isTightElectron",signalElectron->isTightConversionElectron());
		treeMan_->Fill("sigmaIetaIeta",signalElectron->sigmaIEtaIEta());

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

		// MET Uncertainties
		for ( unsigned int unc_i = 0; unc_i < MET_original->getAllMETUncertainties().size(); ++unc_i ) {
			METPointer METForUnc_i = MET_original->getMETForUncertainty( unc_i );
			treeMan_->Fill("MET_METUncertainties",METForUnc_i->et());
			treeMan_->Fill("ST_METUncertainties",Event::ST(jets, signalElectron, METForUnc_i));
			treeMan_->Fill("WPT_METUncertainties",Event::WPT(signalElectron, METForUnc_i));
		}

		// qcdConversionsElectronAnalyser_->setScale(bjetWeight * efficiencyCorrection);
		// metAnalyserqcdConversionSelection_->setScale(bjetWeight * efficiencyCorrection);

		qcdConversionsElectronAnalyser_->analyse(event);
		qcdConversionsElectronAnalyser_->analyseElectron(signalElectron, event->weight());
		metAnalyserqcdConversionSelection_->analyse(event, signalLepton, jets);

		jetAnalyserEPlusJetsConversionSelection_->analyse(event);

		// qcd_conversion_binned_HT_analyser_electron_->setScale(bjetWeight * efficiencyCorrection);
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

			// qcd_conversion_binned_MET_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_conversion_binned_MET_analyser_electron_.at(metIndex)->analyse(met->et(),
					fit_variable_values, event->weight());

			// qcd_conversion_binned_ST_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_conversion_binned_ST_analyser_electron_.at(metIndex)->analyse(
					Event::ST(jets, signalElectron, met), fit_variable_values, event->weight());

			// qcd_conversion_binned_MT_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_conversion_binned_MT_analyser_electron_.at(metIndex)->analyse(Event::MT(signalElectron, met),
					fit_variable_values, event->weight());

			// qcd_conversion_binned_WPT_analyser_electron_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_conversion_binned_WPT_analyser_electron_.at(metIndex)->analyse(Event::WPT(signalElectron, met),
					fit_variable_values, event->weight());

		}
	}
}

void TTbar_plus_X_analyser::muPlusJetsSignalAnalysis(const EventPtr event) {


	if (event->PassesMuonTriggerAndSelectionNoB()){
		BTagEffAnalyserMuPlusJetsRefSelection_->analyse(event);
		PileupAnalyserMuPlusJetsRefSelection_->analyse(event);
	}

	if (event->PassesMuonTriggerAndSelection() ){
	}
	// if (topMuplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
	if ( event->PassesMuonTriggerAndSelection() ) {

		const JetCollection jets(event->CleanedJets());
		unsigned int numberOfBjets = event->getNBJets( SelectionCriteria::MuonPlusJetsReference );
		const JetCollection bJets(event->CleanedBJets());

		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::MuonPlusJetsReference );
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));

		double muonEfficiencyCorrection = 1;
		double muonEfficiencyCorrection_down = 1, muonEfficiencyCorrection_up = 1;
		if ( !event->isRealData() ) {
			muonEfficiencyCorrection = signalMuon->getEfficiencyCorrection( 0 );
			muonEfficiencyCorrection_down = signalMuon->getEfficiencyCorrection( -1 );
			muonEfficiencyCorrection_up = signalMuon->getEfficiencyCorrection( 1 );
		}

		// histMan_->setCurrentBJetBin(numberOfBjets);
		// histMan_->H1D("BTagWeights")->Fill(bjetWeight);
		// histMan_->H1D("N_BJets_reweighted")->Fill(numberOfBjets, event->weight() * bjetWeight * efficiencyCorrection);
		// histMan_->H1D("N_BJets")->Fill(numberOfBjets, event->weight() * efficiencyCorrection);


		const METPointer MET_original(event->MET((METAlgorithm::value) 0));
		// const METPointer METNoHF(event->MET((METAlgorithm::value) 1));

		treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/Ref selection");
		treeMan_->Fill("EventWeight", event->weight() );
		treeMan_->Fill("PUWeight", event->PileUpWeight() );
		treeMan_->Fill("lepton_eta",signalMuon->eta());
		treeMan_->Fill("lepton_pt",signalMuon->pt());
		treeMan_->Fill("lepton_charge",signalMuon->charge());
		treeMan_->Fill("M3",Event::M3(jets));
		if ( numberOfBjets > 0 ) {
			treeMan_->Fill("M_bl",Event::M_bl(bJets, signalMuon));
			treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalMuon));
		}
		treeMan_->Fill("HT",Event::HT(jets));
		treeMan_->Fill("MET",MET_original->et());
		// treeMan_->Fill("METNoHF",METNoHF->et());
		treeMan_->Fill("ST",Event::ST(jets, signalMuon, MET_original));
		// treeMan_->Fill("STNoHF",Event::ST(jets, signalMuon, METNoHF));
		treeMan_->Fill("WPT",Event::WPT(signalMuon, MET_original));
		// treeMan_->Fill("WPTNoHF",Event::WPT(signalMuon, METNoHF));
		treeMan_->Fill("MT",Event::MT(signalMuon, MET_original));
		treeMan_->Fill("NJets",Event::NJets(jets));
		treeMan_->Fill("NBJets",Event::NJets(bJets));
		treeMan_->Fill("NVertices",	event->Vertices().size());

		treeMan_->Fill("MuonEfficiencyCorrection",muonEfficiencyCorrection);
		treeMan_->Fill("MuonUp",muonEfficiencyCorrection_up);
		treeMan_->Fill("MuonDown",muonEfficiencyCorrection_down);
		treeMan_->Fill("ElectronUp",1.0);
		treeMan_->Fill("ElectronDown",1.0);
		treeMan_->Fill("BJetWeight",event->BJetWeight());
		treeMan_->Fill("BJetUpWeight",event->BJetUpWeight());
		treeMan_->Fill("BJetDownWeight",event->BJetDownWeight());

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

		// MET Uncertainties
		for ( unsigned int unc_i = 0; unc_i < MET_original->getAllMETUncertainties().size(); ++unc_i ) {
			METPointer METForUnc_i = MET_original->getMETForUncertainty( unc_i );
			treeMan_->Fill("MET_METUncertainties",METForUnc_i->et());
			treeMan_->Fill("ST_METUncertainties",Event::ST(jets, signalMuon, METForUnc_i));
			treeMan_->Fill("WPT_METUncertainties",Event::WPT(signalMuon, METForUnc_i));
		}

		// metAnalyserMuPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		// muonAnalyserRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		// vertexAnalyserMuPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		// jetAnalyserMuPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);

		metAnalyserMuPlusJetsRefSelection_->analyse(event, signalLepton, jets);

		muonAnalyserRefSelection_->analyse(event);
		muonAnalyserRefSelection_->analyseMuon(signalMuon, event->weight());

		vertexAnalyserMuPlusJetsRefSelection_->analyse(event);
		jetAnalyserMuPlusJetsRefSelection_->analyse(event);

		// wAnalyserMuPlusJetsRefSelection_->setScale(bjetWeight * efficiencyCorrection);
		wAnalyserMuPlusJetsRefSelection_->analyseHadronicW(event, jets, bJets);
		if ( ! event->isRealData() ) {
			wAnalyserMuPlusJetsRefSelection_->analyseHadronicW_partons(event);			
		}

		likelihoodRecoAnalyserMuPlusJetsRefSelection_->analyse(event, jets, bJets, signalLepton, MET_original);

		// ref_selection_binned_HT_analyser_muon_->setScale(bjetWeight * efficiencyCorrection);
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
			// ref_selection_binned_MET_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			ref_selection_binned_MET_analyser_muon_.at(metIndex)->analyse(met->et(), fit_variable_values,
					event->weight());

			// ref_selection_binned_ST_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			ref_selection_binned_ST_analyser_muon_.at(metIndex)->analyse(Event::ST(jets, signalMuon, met),
					fit_variable_values, event->weight());

			// ref_selection_binned_MT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			ref_selection_binned_MT_analyser_muon_.at(metIndex)->analyse(Event::MT(signalMuon, met),
					fit_variable_values, event->weight());

			// ref_selection_binned_WPT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			ref_selection_binned_WPT_analyser_muon_.at(metIndex)->analyse(Event::WPT(signalMuon, met),
					fit_variable_values, event->weight());
		}
	}
}

void TTbar_plus_X_analyser::muPlusJetsQcdAnalysis(const EventPtr event) {
	
	//selection with respect to reference selection
	if ( event->PassesMuonTriggerAndQCDSelection1p5to3() ) {

		const JetCollection jets(event->CleanedJets());
		unsigned int numberOfBjets = event->getNBJets( SelectionCriteria::MuonPlusJetsQCDNonIsolated1p5to3 );
		const JetCollection bJets(event->CleanedBJets());

		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::MuonPlusJetsQCDNonIsolated1p5to3 );

		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));

		const METPointer MET_original(event->MET((METAlgorithm::value) 0));
		// const METPointer METNoHF(event->MET((METAlgorithm::value) 1));

		treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 1p5to3");
		treeMan_->Fill("EventWeight", event->weight() );
		treeMan_->Fill("PUWeight", event->PileUpWeight() );
		treeMan_->Fill("lepton_eta",signalMuon->eta());
		treeMan_->Fill("lepton_pt",signalMuon->pt());
		treeMan_->Fill("lepton_charge",signalMuon->charge());
		treeMan_->Fill("M3",Event::M3(jets));
		if ( numberOfBjets > 0 ) {
			treeMan_->Fill("M_bl",Event::M_bl(bJets, signalMuon));
			treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalMuon));
		}
		treeMan_->Fill("HT",Event::HT(jets));
		treeMan_->Fill("MET",MET_original->et());
		// treeMan_->Fill("METNoHF",METNoHF->et());
		treeMan_->Fill("ST",Event::ST(jets, signalMuon, MET_original));
		// treeMan_->Fill("STNoHF",Event::ST(jets, signalMuon, METNoHF));
		treeMan_->Fill("WPT",Event::WPT(signalMuon, MET_original));
		// treeMan_->Fill("WPTNoHF",Event::WPT(signalMuon, METNoHF));
		treeMan_->Fill("MT",Event::MT(signalMuon, MET_original));
		treeMan_->Fill("NJets",Event::NJets(jets));
		treeMan_->Fill("NBJets",Event::NJets(bJets));
		treeMan_->Fill("NVertices",	event->Vertices().size());

		treeMan_->Fill("ElectronEfficiencyCorrection",1.0);
		treeMan_->Fill("ElectronUp",1.0);
		treeMan_->Fill("ElectronDown",1.0);
		treeMan_->Fill("MuonEfficiencyCorrection",1.0);
		treeMan_->Fill("MuonUp",1.0);
		treeMan_->Fill("MuonDown",1.0);
		treeMan_->Fill("BJetWeight",event->BJetWeight());
		treeMan_->Fill("BJetUpWeight",event->BJetUpWeight());
		treeMan_->Fill("BJetDownWeight",event->BJetDownWeight());

		if ( Globals::useHitFit ) {
			BAT::TtbarHypothesis topHypothesis = hitFitAnalyserMuPlusJetsQCDSelection1p5to3_->analyseAndReturn(event, jets, bJets, signalLepton);
			event->setTTbarHypothesis( topHypothesis );
			treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 1p5to3");
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

		// MET Uncertainties
		for ( unsigned int unc_i = 0; unc_i < MET_original->getAllMETUncertainties().size(); ++unc_i ) {
			METPointer METForUnc_i = MET_original->getMETForUncertainty( unc_i );
			treeMan_->Fill("MET_METUncertainties",METForUnc_i->et());
			treeMan_->Fill("ST_METUncertainties",Event::ST(jets, signalMuon, METForUnc_i));
			treeMan_->Fill("WPT_METUncertainties",Event::WPT(signalMuon, METForUnc_i));
		}

		// qcdNonIsoMuonAnalyser_->setScale(bjetWeight * efficiencyCorrection);
		// metAnalyserqcdNonIsoMuonSelection_->setScale(bjetWeight * efficiencyCorrection);

		qcdNonIsoMuonAnalyser1p5to3_->analyse(event);
		qcdNonIsoMuonAnalyser1p5to3_->analyseMuon(signalMuon, event->weight());
		metAnalyserqcdNonIsoMuonSelection1p5to3_->analyse(event, signalLepton, jets);

		jetAnalyserMuPlusJetsQCDNonIsoSelection1p5to3_->analyse(event);

		// qcd_noniso_binned_HT_analyser_muon_->setScale(bjetWeight * efficiencyCorrection);
		vector<double> fit_variable_values;
		fit_variable_values.push_back(fabs(signalMuon->eta()));
		fit_variable_values.push_back(Event::M3(jets));
		fit_variable_values.push_back(Event::M_bl(bJets, signalMuon));
		fit_variable_values.push_back(Event::angle_bl(bJets, signalMuon));
		qcd_noniso_1p5to3_binned_HT_analyser_muon_->analyse(Event::HT(jets), fit_variable_values, event->weight());

		for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
			// if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			// 	continue;
			// bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
			// //skip MC only MET entries
			// if (isMCOnlyMET && event->isRealData())
			// 	continue;
			string metPrefix = METAlgorithm::names.at(metIndex);

			const METPointer met(event->MET((METAlgorithm::value) metIndex));
			// qcd_noniso_binned_MET_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_1p5to3_binned_MET_analyser_muon_.at(metIndex)->analyse(met->et(), fit_variable_values,
					event->weight());

			// qcd_noniso_binned_ST_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_1p5to3_binned_ST_analyser_muon_.at(metIndex)->analyse(Event::ST(jets, signalMuon, met),
					fit_variable_values, event->weight());

			// qcd_noniso_binned_MT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_1p5to3_binned_MT_analyser_muon_.at(metIndex)->analyse(Event::MT(signalMuon, met),
					fit_variable_values, event->weight());

			// qcd_noniso_binned_WPT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_1p5to3_binned_WPT_analyser_muon_.at(metIndex)->analyse(Event::WPT(signalMuon, met),
					fit_variable_values, event->weight());

		}

	}



if ( event->PassesMuonTriggerAndQCDSelection3toInf() ) {

		const JetCollection jets(event->CleanedJets());
		unsigned int numberOfBjets = event->getNBJets( SelectionCriteria::MuonPlusJetsQCDNonIsolated3toInf );
		const JetCollection bJets(event->CleanedBJets());

		const LeptonPointer signalLepton = event->getSignalLepton( SelectionCriteria::MuonPlusJetsQCDNonIsolated3toInf );

		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));

		const METPointer MET_original(event->MET((METAlgorithm::value) 0));
		// const METPointer METNoHF(event->MET((METAlgorithm::value) 1));

		treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 3toInf");
		treeMan_->Fill("EventWeight", event->weight() );
		treeMan_->Fill("PUWeight", event->PileUpWeight() );
		treeMan_->Fill("lepton_eta",signalMuon->eta());
		treeMan_->Fill("lepton_pt",signalMuon->pt());
		treeMan_->Fill("lepton_charge",signalMuon->charge());
		treeMan_->Fill("M3",Event::M3(jets));
		if ( numberOfBjets > 0 ) {
			treeMan_->Fill("M_bl",Event::M_bl(bJets, signalMuon));
			treeMan_->Fill("angle_bl",Event::angle_bl(bJets, signalMuon));
		}
		treeMan_->Fill("HT",Event::HT(jets));
		treeMan_->Fill("MET",MET_original->et());
		// treeMan_->Fill("METNoHF",METNoHF->et());
		treeMan_->Fill("ST",Event::ST(jets, signalMuon, MET_original));
		// treeMan_->Fill("STNoHF",Event::ST(jets, signalMuon, METNoHF));
		treeMan_->Fill("WPT",Event::WPT(signalMuon, MET_original));
		// treeMan_->Fill("WPTNoHF",Event::WPT(signalMuon, METNoHF));
		treeMan_->Fill("MT",Event::MT(signalMuon, MET_original));
		treeMan_->Fill("NJets",Event::NJets(jets));
		treeMan_->Fill("NBJets",Event::NJets(bJets));
		treeMan_->Fill("NVertices",	event->Vertices().size());

		treeMan_->Fill("ElectronEfficiencyCorrection",1.0);
		treeMan_->Fill("ElectronUp",1.0);
		treeMan_->Fill("ElectronDown",1.0);
		treeMan_->Fill("MuonEfficiencyCorrection",1.0);
		treeMan_->Fill("MuonUp",1.0);
		treeMan_->Fill("MuonDown",1.0);
		treeMan_->Fill("BJetWeight",event->BJetWeight());
		treeMan_->Fill("BJetUpWeight",event->BJetUpWeight());
		treeMan_->Fill("BJetDownWeight",event->BJetDownWeight());

		if ( Globals::useHitFit ) {
			BAT::TtbarHypothesis topHypothesis = hitFitAnalyserMuPlusJetsQCDSelection3toInf_->analyseAndReturn(event, jets, bJets, signalLepton);
			event->setTTbarHypothesis( topHypothesis );
			treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 3toInf");
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

		// MET Uncertainties
		for ( unsigned int unc_i = 0; unc_i < MET_original->getAllMETUncertainties().size(); ++unc_i ) {
			METPointer METForUnc_i = MET_original->getMETForUncertainty( unc_i );
			treeMan_->Fill("MET_METUncertainties",METForUnc_i->et());
			treeMan_->Fill("ST_METUncertainties",Event::ST(jets, signalMuon, METForUnc_i));
			treeMan_->Fill("WPT_METUncertainties",Event::WPT(signalMuon, METForUnc_i));
		}

		// qcdNonIsoMuonAnalyser_->setScale(bjetWeight * efficiencyCorrection);
		// metAnalyserqcdNonIsoMuonSelection_->setScale(bjetWeight * efficiencyCorrection);

		qcdNonIsoMuonAnalyser3toInf_->analyse(event);
		qcdNonIsoMuonAnalyser3toInf_->analyseMuon(signalMuon, event->weight());
		metAnalyserqcdNonIsoMuonSelection3toInf_->analyse(event, signalLepton, jets);

		jetAnalyserMuPlusJetsQCDNonIsoSelection3toInf_->analyse(event);

		// qcd_noniso_binned_HT_analyser_muon_->setScale(bjetWeight * efficiencyCorrection);
		vector<double> fit_variable_values;
		fit_variable_values.push_back(fabs(signalMuon->eta()));
		fit_variable_values.push_back(Event::M3(jets));
		fit_variable_values.push_back(Event::M_bl(bJets, signalMuon));
		fit_variable_values.push_back(Event::angle_bl(bJets, signalMuon));
		qcd_noniso_3toInf_binned_HT_analyser_muon_->analyse(Event::HT(jets), fit_variable_values, event->weight());

		for (unsigned int metIndex = 0; metIndex < METAlgorithm::NUMBER_OF_METALGORITHMS; ++metIndex) {
			// if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, metIndex))
			// 	continue;
			// bool isMCOnlyMET = MET::isMCOnlyMETType(metIndex);
			// //skip MC only MET entries
			// if (isMCOnlyMET && event->isRealData())
			// 	continue;
			string metPrefix = METAlgorithm::names.at(metIndex);

			const METPointer met(event->MET((METAlgorithm::value) metIndex));
			// qcd_noniso_binned_MET_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_3toInf_binned_MET_analyser_muon_.at(metIndex)->analyse(met->et(), fit_variable_values,
					event->weight());

			// qcd_noniso_binned_ST_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_3toInf_binned_ST_analyser_muon_.at(metIndex)->analyse(Event::ST(jets, signalMuon, met),
					fit_variable_values, event->weight());

			// qcd_noniso_binned_MT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_3toInf_binned_MT_analyser_muon_.at(metIndex)->analyse(Event::MT(signalMuon, met),
					fit_variable_values, event->weight());

			// qcd_noniso_binned_WPT_analyser_muon_.at(metIndex)->setScale(bjetWeight * efficiencyCorrection);
			qcd_noniso_3toInf_binned_WPT_analyser_muon_.at(metIndex)->analyse(Event::WPT(signalMuon, met),
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
	// treeMan_->addBranch("METNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("STNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("WPTNoHF", "F", "FitVariables" + Globals::treePrefix_);
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
	treeMan_->addBranch("MuonUp", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MuonDown", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("BJetWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("BJetUpWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("BJetDownWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("isTightElectron", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("sigmaIetaIeta", "F", "FitVariables" + Globals::treePrefix_);

	treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/QCD non iso e+jets");
	treeMan_->addBranch("HT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("METNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("STNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("WPTNoHF", "F", "FitVariables" + Globals::treePrefix_);
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
	treeMan_->addBranch("isTightElectron", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("sigmaIetaIeta", "F", "FitVariables" + Globals::treePrefix_);

	treeMan_->setCurrentFolder(histogramFolder_ + "/EPlusJets/QCDConversions");
	treeMan_->addBranch("HT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("METNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("STNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("WPTNoHF", "F", "FitVariables" + Globals::treePrefix_);
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
	treeMan_->addBranch("isTightElectron", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("sigmaIetaIeta", "F", "FitVariables" + Globals::treePrefix_);

	treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/Ref selection");
	treeMan_->addBranch("HT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("METNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("STNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("WPTNoHF", "F", "FitVariables" + Globals::treePrefix_);
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
	treeMan_->addBranch("MuonEfficiencyCorrection", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MuonUp", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MuonDown", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronUp", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ElectronDown", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("BJetWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("BJetUpWeight", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("BJetDownWeight", "F", "FitVariables" + Globals::treePrefix_);

	treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 1p5to3");
	treeMan_->addBranch("HT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("METNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("STNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("WPTNoHF", "F", "FitVariables" + Globals::treePrefix_);
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

	treeMan_->setCurrentFolder(histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 3toInf");
	treeMan_->addBranch("HT", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("METNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("MET_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("ST", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("STNoHF", "F", "FitVariables" + Globals::treePrefix_);
	treeMan_->addBranch("ST_METUncertainties", "F", "FitVariables" + Globals::treePrefix_, false);
	treeMan_->addBranch("WPT", "F", "FitVariables" + Globals::treePrefix_);
	// treeMan_->addBranch("WPTNoHF", "F", "FitVariables" + Globals::treePrefix_);
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
	qcdConversionsElectronAnalyser_->createHistograms();
	qcdConversionsElectronAnalyser_->createTrees();
	metAnalyserqcdConversionSelection_->createHistograms();
	metAnalyserqcdConversionSelection_->createTrees();

	qcdNonIsoElectronAnalyser_->createHistograms();
	qcdNonIsoElectronAnalyser_->createTrees();
	metAnalyserqcdNonIsoElectronSelection_->createHistograms();
	metAnalyserqcdNonIsoElectronSelection_->createTrees();

	qcdNonIsoMuonAnalyser1p5to3_->createHistograms();
	qcdNonIsoMuonAnalyser1p5to3_->createTrees();
	metAnalyserqcdNonIsoMuonSelection1p5to3_->createHistograms();
	metAnalyserqcdNonIsoMuonSelection1p5to3_->createTrees();

	qcdNonIsoMuonAnalyser3toInf_->createHistograms();
	qcdNonIsoMuonAnalyser3toInf_->createTrees();
	metAnalyserqcdNonIsoMuonSelection3toInf_->createHistograms();
	metAnalyserqcdNonIsoMuonSelection3toInf_->createTrees();

	jetAnalyserEPlusJetsRefSelection_->createHistograms();
	jetAnalyserMuPlusJetsRefSelection_->createHistograms();
	jetAnalyserEPlusJetsQCDNonIsoSelection_->createHistograms();
	jetAnalyserEPlusJetsConversionSelection_->createHistograms();
	jetAnalyserMuPlusJetsQCDNonIsoSelection1p5to3_->createHistograms();
	jetAnalyserMuPlusJetsQCDNonIsoSelection3toInf_->createHistograms();

	jetAnalyserEPlusJetsRefSelection_->createTrees();
	jetAnalyserMuPlusJetsRefSelection_->createTrees();
	jetAnalyserEPlusJetsQCDNonIsoSelection_->createTrees();
	jetAnalyserEPlusJetsConversionSelection_->createTrees();
	jetAnalyserMuPlusJetsQCDNonIsoSelection1p5to3_->createTrees();
	jetAnalyserMuPlusJetsQCDNonIsoSelection3toInf_->createTrees();

	// W boson simple reconstruction
	wAnalyserEPlusJetsRefSelection_->createHistograms();
	wAnalyserMuPlusJetsRefSelection_->createHistograms();
	wAnalyserEPlusJetsRefSelection_->createTrees();
	wAnalyserMuPlusJetsRefSelection_->createTrees();

	hitFitAnalyserEPlusJetsRefSelection_->createHistograms();
	hitFitAnalyserMuPlusJetsRefSelection_->createHistograms();
	hitFitAnalyserEPlusJetsQCDSelection_->createHistograms();
	hitFitAnalyserEPlusJetsConversionSelection_->createHistograms();
	hitFitAnalyserMuPlusJetsQCDSelection1p5to3_->createHistograms();
	hitFitAnalyserMuPlusJetsQCDSelection3toInf_->createHistograms();

	hitFitAnalyserEPlusJetsRefSelection_->createTrees();
	hitFitAnalyserMuPlusJetsRefSelection_->createTrees();
	hitFitAnalyserEPlusJetsQCDSelection_->createTrees();
	hitFitAnalyserEPlusJetsConversionSelection_->createTrees();
	hitFitAnalyserMuPlusJetsQCDSelection1p5to3_->createTrees();
	hitFitAnalyserMuPlusJetsQCDSelection3toInf_->createTrees();

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
		metAnalyserqcdConversionSelection_(
				new METAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/QCDConversions/MET")), //
		metAnalyserqcdNonIsoMuonSelection1p5to3_(
				new METAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets 1p5to3/MET")), //
		metAnalyserqcdNonIsoMuonSelection3toInf_(
				new METAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets 3toInf/MET")), //
		qcdNonIsoElectronAnalyser_(
				new ElectronAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/QCD non iso e+jets/Electron", true)), //
		qcdConversionsElectronAnalyser_(
				new ElectronAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/QCDConversions/Electron", true)), //
		qcdNonIsoMuonAnalyser1p5to3_(
				new MuonAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets 1p5to3/Muon", true)), //
		qcdNonIsoMuonAnalyser3toInf_(
				new MuonAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/QCD non iso mu+jets 3toInf/Muon", true)), //

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
		qcd_noniso_1p5to3_binned_MET_analyser_muon_(), //
		qcd_noniso_3toInf_binned_MET_analyser_muon_(), //

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
		qcd_noniso_1p5to3_binned_HT_analyser_muon_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 1p5to3/Binned_HT_Analysis")), //
		qcd_noniso_3toInf_binned_HT_analyser_muon_(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 3toInf/Binned_HT_Analysis")), //
		//ST
		ref_selection_binned_ST_analyser_electron_(), //
		qcd_conversion_binned_ST_analyser_electron_(), //
		qcd_noniso_binned_ST_analyser_electron_(), //
		//ST
		ref_selection_binned_ST_analyser_muon_(), //
		qcd_noniso_1p5to3_binned_ST_analyser_muon_(), //
		qcd_noniso_3toInf_binned_ST_analyser_muon_(), //
		//MT electrons
		ref_selection_binned_MT_analyser_electron_(), //
		qcd_conversion_binned_MT_analyser_electron_(), //
		qcd_noniso_binned_MT_analyser_electron_(), //
		//MT muons
		ref_selection_binned_MT_analyser_muon_(), //
		qcd_noniso_1p5to3_binned_MT_analyser_muon_(), //
		qcd_noniso_3toInf_binned_MT_analyser_muon_(), //
		//WPT electrons
		ref_selection_binned_WPT_analyser_electron_(), //
		qcd_conversion_binned_WPT_analyser_electron_(), //
		qcd_noniso_binned_WPT_analyser_electron_(), //
		//WPT muons
		ref_selection_binned_WPT_analyser_muon_(), //
		qcd_noniso_1p5to3_binned_WPT_analyser_muon_(), //
		qcd_noniso_3toInf_binned_WPT_analyser_muon_(), //

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
		jetAnalyserMuPlusJetsRefSelection_(new JetAnalyser(histMan, treeMan, SelectionCriteria::MuonPlusJetsReference, histogramFolder + "/MuPlusJets/Ref selection/Jets")), //
		jetAnalyserEPlusJetsQCDNonIsoSelection_(new JetAnalyser(histMan, treeMan, SelectionCriteria::ElectronPlusJetsQCDNonIsolated, histogramFolder + "/EPlusJets/QCD non iso e+jets/Jets")), //
		jetAnalyserEPlusJetsConversionSelection_(new JetAnalyser(histMan, treeMan, SelectionCriteria::ElectronPlusJetsQCDConversion, histogramFolder + "/EPlusJets/QCDConversions/Jets")), //
		jetAnalyserMuPlusJetsQCDNonIsoSelection1p5to3_( new JetAnalyser(histMan, treeMan, SelectionCriteria::MuonPlusJetsQCDNonIsolated1p5to3, histogramFolder + "/MuPlusJets/QCD non iso mu+jets 1p5to3/Jets")), //
		jetAnalyserMuPlusJetsQCDNonIsoSelection3toInf_( new JetAnalyser(histMan, treeMan, SelectionCriteria::MuonPlusJetsQCDNonIsolated3toInf, histogramFolder + "/MuPlusJets/QCD non iso mu+jets 3toInf/Jets")), //
		wAnalyserEPlusJetsRefSelection_(new WAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/Ref selection/W Bosons")), //
		wAnalyserMuPlusJetsRefSelection_(new WAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/Ref selection/W Bosons")), //
		hitFitAnalyserEPlusJetsRefSelection_(new HitFitAnalyser(histMan, treeMan, true, histogramFolder + "/EPlusJets/Ref selection/HitFit")), //
		hitFitAnalyserMuPlusJetsRefSelection_(new HitFitAnalyser(histMan, treeMan, false, histogramFolder + "/MuPlusJets/Ref selection/HitFit")), //
		hitFitAnalyserEPlusJetsQCDSelection_(new HitFitAnalyser(histMan, treeMan, true, histogramFolder + "/EPlusJets/QCD non iso e+jets/HitFit")), //
		hitFitAnalyserEPlusJetsConversionSelection_(new HitFitAnalyser(histMan, treeMan, true, histogramFolder + "/EPlusJets/QCDConversions/HitFit")), //
		hitFitAnalyserMuPlusJetsQCDSelection1p5to3_(new HitFitAnalyser(histMan, treeMan, false, histogramFolder + "/MuPlusJets/QCD non iso mu+jets 1p5to3/HitFit")), //		
		hitFitAnalyserMuPlusJetsQCDSelection3toInf_(new HitFitAnalyser(histMan, treeMan, false, histogramFolder + "/MuPlusJets/QCD non iso mu+jets 3toInf/HitFit")), //		
		likelihoodRecoAnalyserEPlusJetsRefSelection_(new LikelihoodRecoAnalyser(histMan, treeMan, true, histogramFolder + "/EPlusJets/Ref selection/LikelihoodReco")), //
		likelihoodRecoAnalyserMuPlusJetsRefSelection_(new LikelihoodRecoAnalyser(histMan, treeMan, false, histogramFolder + "/MuPlusJets/Ref selection/LikelihoodReco")), //
		BTagEffAnalyserEPlusJetsRefSelection_(new BTagEff(histMan, treeMan, histogramFolder + "/EPlusJets/Ref selection/BTagEfficiencies")), //
		BTagEffAnalyserMuPlusJetsRefSelection_(new BTagEff(histMan, treeMan, histogramFolder + "/MuPlusJets/Ref selection/BTagEfficiencies")), //
		PileupAnalyserEPlusJetsRefSelection_(new PileupAnalyser(histMan, treeMan, histogramFolder + "/EPlusJets/Ref selection/Pileup")), //
		PileupAnalyserMuPlusJetsRefSelection_(new PileupAnalyser(histMan, treeMan, histogramFolder + "/MuPlusJets/Ref selection/Pileup")), //
		electron_variables_(), //
		muon_variables_(){

	//bins: start is always assumed to be 0 and the last is read as > X
	//MET bins:
	// old: [0., 27., 52., 87., 129., 171., inf]
	metBins_.push_back(0.);
	metBins_.push_back(27.);
	metBins_.push_back(52.);
	metBins_.push_back(87.);
	metBins_.push_back(130.);
	metBins_.push_back(172.);

	//HT:
	// old: [0., 187., 217., 250., 287., 327., 369., 414., 414., 464., 517., 575., 634., 696., 772., inf]
	ht_bins_.push_back(120.0);
	ht_bins_.push_back(185.0);
	ht_bins_.push_back(215.0);
	ht_bins_.push_back(247.0);
	ht_bins_.push_back(283.0);
	ht_bins_.push_back(323.0);
	ht_bins_.push_back(365.0);
	ht_bins_.push_back(409.0);
	ht_bins_.push_back(458.0);
	ht_bins_.push_back(512.0);
	ht_bins_.push_back(570.0);
	ht_bins_.push_back(629.0);
	ht_bins_.push_back(691.0);
	ht_bins_.push_back(769.0);

	//ST:
	// old: [0.0, 281.0, 324.0, 367.0, 415.0, 466.0, 521.0, 581.0, 645.0, 714.0, 783.0, 861.0, 946.0, inf]
	st_bins_.push_back(146.0);
	st_bins_.push_back(277.0);
	st_bins_.push_back(319.0);
	st_bins_.push_back(361.0);
	st_bins_.push_back(408.0);
	st_bins_.push_back(459.0);
	st_bins_.push_back(514.0);
	st_bins_.push_back(573.0);
	st_bins_.push_back(637.0);
	st_bins_.push_back(705.0);
	st_bins_.push_back(774.0);
	st_bins_.push_back(854.0);
	st_bins_.push_back(940.0);

	//MT:
	// old: [0., 23., 58., inf]
	mt_bins_.push_back(0.);
	mt_bins_.push_back(23.0);
	mt_bins_.push_back(58.0);

	//WPT:
	// old: [0., 27., 52., 78., 105., 134., 165., 198., 235., inf]
	wpt_bins_.push_back(0.);
	wpt_bins_.push_back(27.0);
	wpt_bins_.push_back(52.0);
	wpt_bins_.push_back(78.0);
	wpt_bins_.push_back(105.0);
	wpt_bins_.push_back(134.0);
	wpt_bins_.push_back(166.0);
	wpt_bins_.push_back(200.0);
	wpt_bins_.push_back(237.0);

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
		Binned_Variable_analyser_ptr qcd_noniso_1p5to3_binned_met_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 1p5to3/Binned_MET_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_3toInf_binned_met_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 3toInf/Binned_MET_Analysis"));

		ref_selection_binned_met_muon_analyser->set_variables(MET, muon_variables_);
		qcd_noniso_1p5to3_binned_met_muon_analyser->set_variables(MET, muon_variables_);
		qcd_noniso_3toInf_binned_met_muon_analyser->set_variables(MET, muon_variables_);

		ref_selection_binned_MET_analyser_muon_.push_back(ref_selection_binned_met_muon_analyser);
		qcd_noniso_1p5to3_binned_MET_analyser_muon_.push_back(qcd_noniso_1p5to3_binned_met_muon_analyser);
		qcd_noniso_3toInf_binned_MET_analyser_muon_.push_back(qcd_noniso_3toInf_binned_met_muon_analyser);

	}
}

void TTbar_plus_X_analyser::make_binned_HT_analysers() {
	// multi-variable analysers
	Variable HT = Variable("HT", ht_bins_);
	ref_selection_binned_HT_analyser_electron_->set_variables(HT, electron_variables_);
	ref_selection_binned_HT_analyser_muon_->set_variables(HT, muon_variables_);

	qcd_noniso_binned_HT_analyser_electron_->set_variables(HT, electron_variables_);
	qcd_noniso_1p5to3_binned_HT_analyser_muon_->set_variables(HT, muon_variables_);
	qcd_noniso_3toInf_binned_HT_analyser_muon_->set_variables(HT, muon_variables_);

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
		Binned_Variable_analyser_ptr qcd_noniso_1p5to3_binned_ST_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 1p5to3/Binned_ST_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_3toInf_binned_ST_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 3toInf/Binned_ST_Analysis"));

		ref_selection_binned_ST_muon_analyser->set_variables(ST, muon_variables_);
		qcd_noniso_1p5to3_binned_ST_muon_analyser->set_variables(ST, muon_variables_);
		qcd_noniso_3toInf_binned_ST_muon_analyser->set_variables(ST, muon_variables_);

		ref_selection_binned_ST_analyser_muon_.push_back(ref_selection_binned_ST_muon_analyser);
		qcd_noniso_1p5to3_binned_ST_analyser_muon_.push_back(qcd_noniso_1p5to3_binned_ST_muon_analyser);
		qcd_noniso_3toInf_binned_ST_analyser_muon_.push_back(qcd_noniso_3toInf_binned_ST_muon_analyser);

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
		Binned_Variable_analyser_ptr qcd_noniso_1p5to3_binned_MT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 1p5to3/Binned_MT_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_3toInf_binned_MT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 3toInf/Binned_MT_Analysis"));

		ref_selection_binned_MT_muon_analyser->set_variables(MT, muon_variables_);
		qcd_noniso_1p5to3_binned_MT_muon_analyser->set_variables(MT, muon_variables_);
		qcd_noniso_3toInf_binned_MT_muon_analyser->set_variables(MT, muon_variables_);

		ref_selection_binned_MT_analyser_muon_.push_back(ref_selection_binned_MT_muon_analyser);
		qcd_noniso_1p5to3_binned_MT_analyser_muon_.push_back(qcd_noniso_1p5to3_binned_MT_muon_analyser);
		qcd_noniso_3toInf_binned_MT_analyser_muon_.push_back(qcd_noniso_3toInf_binned_MT_muon_analyser);

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
		Binned_Variable_analyser_ptr qcd_noniso_1p5to3_binned_WPT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 1p5to3/Binned_WPT_Analysis"));
		Binned_Variable_analyser_ptr qcd_noniso_3toInf_binned_WPT_muon_analyser(
				new Binned_variable_analyser(histMan_,
						histogramFolder_ + "/MuPlusJets/QCD non iso mu+jets 3toInf/Binned_WPT_Analysis"));

		no_selection_binned_WPT_muon_analyser->set_variables(WPT, muon_variables_);
		ref_selection_binned_WPT_muon_analyser->set_variables(WPT, muon_variables_);
		qcd_noniso_1p5to3_binned_WPT_muon_analyser->set_variables(WPT, muon_variables_);
		qcd_noniso_3toInf_binned_WPT_muon_analyser->set_variables(WPT, muon_variables_);

		ref_selection_binned_WPT_analyser_muon_.push_back(ref_selection_binned_WPT_muon_analyser);
		qcd_noniso_1p5to3_binned_WPT_analyser_muon_.push_back(qcd_noniso_1p5to3_binned_WPT_muon_analyser);
		qcd_noniso_3toInf_binned_WPT_analyser_muon_.push_back(qcd_noniso_3toInf_binned_WPT_muon_analyser);

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
