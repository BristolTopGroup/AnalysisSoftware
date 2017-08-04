/*
 * UnfoldingRecoAnalyser.cpp
 *
 *  Created on: 24 Feb 2015
 *      Author: ejclemen
 */

#include "../../interface/Analysers/UnfoldingRecoAnalyser.h"

#include <boost/scoped_ptr.hpp>
#include <iostream>

using namespace std;

namespace BAT {

void UnfoldingRecoAnalyser::analyse(const EventPtr event) {
	weight_ = event->weight();

	treeMan_->setCurrentFolder(histogramFolder_);

	treeMan_->Fill("EventWeight", event->weight());

	double centralLHEWeight = event->centralLHEWeight();
	for ( unsigned int unc_i = 0; unc_i < event->generatorSystematicWeights().size(); ++unc_i ) {
		double weight = event->generatorSystematicWeights().at( unc_i ) / centralLHEWeight;
		int weightID = event->generatorSystematicWeightsID().at( unc_i );

		// cout << to_string(unc_i) << " " << weightID << " " << weight/centralLHEWeight << endl;
		// string uncString = to_string(unc_i);
		if ( weightID >= 1001 && weightID <= 1009 ) {
			treeMan_->Fill( "muFmuRWeight_" + to_string(weightID-1001), weight);
		}
		else if ( weightID >= 2001 && weightID <= 2100 ) {
			treeMan_->Fill( "pdfWeight_" + to_string(weightID-2001), weight);
		}
		else if ( weightID == 2101 || weightID == 2102 ) {
			treeMan_->Fill( "alphaSWeight_" + to_string(weightID-2101), weight);
		}
		else if ( weightID >= 3001 && weightID <= 3055 ) {
			treeMan_->Fill( "CT14Weight_" + to_string(weightID-3001), weight);
		}
		else if ( weightID >= 4001 && weightID <= 4056 ) {
			treeMan_->Fill( "MMHT14Weight_" + to_string(weightID-4001), weight);
		}
		else if ( weightID >= 5001 && weightID <= 5027 ) {
			treeMan_->Fill( "matchingWeight_" + to_string(weightID-5001), weight);
		}
	}

	treeMan_->Fill("semilepbrUp", event->semiLepBrUpWeight());
	treeMan_->Fill("semilepbrDown", event->semiLepBrDownWeight());
	treeMan_->Fill("petersonFrag", event->petersonFragWeight());
	treeMan_->Fill("centralFrag", event->centralFragWeight());
	treeMan_->Fill("upFrag", event->upFragWeight());
	treeMan_->Fill("downFrag", event->downFragWeight());


	treeMan_->setCurrentFolder(histogramFolder_);

	int selectionCriteria = -1;

	if ( event->PassesElectronTriggerAndSelection() ) selectionCriteria = SelectionCriteria::ElectronPlusJetsReference;
	else if ( event->PassesMuonTriggerAndSelection() ) selectionCriteria = SelectionCriteria::MuonPlusJetsReference;


	if ( event->PassesElectronTriggerAndSelection() && event->PassesMuonTriggerAndSelection() ) {
		cout << "-------------> This should not happen <-------------" << endl;
	}
	// Store selection flags first
	treeMan_->Fill("passSelection",selectionCriteria);

	if ( !( event->PassesMuonTriggerAndSelection() || event->PassesElectronTriggerAndSelection() )) return;

	const JetCollection jets(event->CleanedJets());
	const JetCollection bjets(event->CleanedBJets());
	const LeptonPointer signalLepton = event->getSignalLepton( selectionCriteria );
	METAlgorithm::value metType = (METAlgorithm::value) 0;
	const METPointer met(event->MET(metType));

	treeMan_->Fill("MET",met->et());
	treeMan_->Fill("HT",Event::HT(jets));
	treeMan_->Fill("ST",Event::ST(jets,signalLepton,met));
	treeMan_->Fill("WPT",Event::WPT(signalLepton,met));
	treeMan_->Fill("MT",Event::MT(signalLepton,met));

	treeMan_->Fill("leptonPt", signalLepton->pt() );
	treeMan_->Fill("leptonEta", signalLepton->eta() );
	treeMan_->Fill("leptonCharge", signalLepton->charge() );

	// Get lepton scale factor
	if ( event->PassesMuonTriggerAndSelection() ) {
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection_etaBins = 1;
		double efficiencyCorrection_down_etaBins = 1;
		double efficiencyCorrection_up_etaBins = 1;

		double efficiencyCorrection = signalMuon->getEfficiencyCorrection( 0, efficiencyCorrection_etaBins );
		double efficiencyCorrection_down = signalMuon->getEfficiencyCorrection( -1, efficiencyCorrection_down_etaBins );
		double efficiencyCorrection_up = signalMuon->getEfficiencyCorrection( 1, efficiencyCorrection_up_etaBins );

		treeMan_->Fill("LeptonEfficiencyCorrection", efficiencyCorrection);
		treeMan_->Fill("LeptonEfficiencyCorrectionDown", efficiencyCorrection_down );
		treeMan_->Fill("LeptonEfficiencyCorrectionUp", efficiencyCorrection_up );

		treeMan_->Fill("LeptonEfficiencyCorrection_etaBins", efficiencyCorrection_etaBins);
		treeMan_->Fill("LeptonEfficiencyCorrectionDown_etaBins", efficiencyCorrection_down_etaBins );
		treeMan_->Fill("LeptonEfficiencyCorrectionUp_etaBins", efficiencyCorrection_up_etaBins );
	}
	else if ( event->PassesElectronTriggerAndSelection() ) {
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));
		double efficiencyCorrection = signalElectron->getEfficiencyCorrection( 0 );	
		double efficiencyCorrection_down = signalElectron->getEfficiencyCorrection( -1 );	
		double efficiencyCorrection_up = signalElectron->getEfficiencyCorrection( 1 );	

		treeMan_->Fill("LeptonEfficiencyCorrection", efficiencyCorrection );
		treeMan_->Fill("LeptonEfficiencyCorrectionUp", efficiencyCorrection_up );
		treeMan_->Fill("LeptonEfficiencyCorrectionDown", efficiencyCorrection_down );

		treeMan_->Fill("LeptonEfficiencyCorrection_etaBins", efficiencyCorrection);
		treeMan_->Fill("LeptonEfficiencyCorrectionUp_etaBins", efficiencyCorrection_up );
		treeMan_->Fill("LeptonEfficiencyCorrectionDown_etaBins", efficiencyCorrection_down );
	}

	treeMan_->Fill("PUWeight", event->PileUpWeight() );
	treeMan_->Fill("PUWeight_up", event->PileUpWeight(1) );
	treeMan_->Fill("PUWeight_down", event->PileUpWeight(-1) );
	treeMan_->Fill("BJetWeight",event->BJetWeight());
	treeMan_->Fill("BJetEfficiencyCorrectionWeight",event->BJetEfficiencyCorrectionWeight());
	treeMan_->Fill("BJetAlternativeWeight",event->BJetAlternativeWeight());

	treeMan_->Fill("LightJetUpWeight",event->LightJetUpWeight());
	treeMan_->Fill("LightJetDownWeight",event->LightJetDownWeight());
	treeMan_->Fill("BJetUpWeight",event->BJetUpWeight());
	treeMan_->Fill("BJetDownWeight",event->BJetDownWeight());

	// MET Uncertainties
	for ( unsigned int unc_i = 0; unc_i < met->getAllMETUncertainties().size(); ++unc_i ) {
		METPointer METForUnc_i = met->getMETForUncertainty( unc_i );
		treeMan_->Fill("MET_METUncertainties",METForUnc_i->et());
		treeMan_->Fill("ST_METUncertainties",Event::ST(jets, signalLepton, METForUnc_i));
		treeMan_->Fill("WPT_METUncertainties",Event::WPT(signalLepton, METForUnc_i));
	}

	for (unsigned int index = 0; index < bjets.size(); ++index) {
		treeMan_->Fill("bPt", bjets.at(index)->pt() );
		treeMan_->Fill("bEta", bjets.at(index)->eta() );
	}


	if ( bjets.size() >= 1 ) {
		treeMan_->Fill("angle_bl",Event::angle_bl( bjets, signalLepton));
		if ( bjets.size() >= 2 ) {
			unsigned int highestCSVJetIndex = 0;
			unsigned int secondHighestCSVJetIndex = 0;
			Event::getTopTwoCSVJets( bjets, highestCSVJetIndex, secondHighestCSVJetIndex );
			JetPointer highestCSVJet(bjets.at(highestCSVJetIndex));
			JetPointer secondHighestCSVJet(bjets.at(secondHighestCSVJetIndex));
			treeMan_->Fill("deltaPhi_bb", fabs( Event::deltaPhi_bb(highestCSVJet, secondHighestCSVJet) ) ) ;
			treeMan_->Fill("deltaEta_bb", fabs( Event::deltaEta_bb(highestCSVJet, secondHighestCSVJet) ) ) ;
			treeMan_->Fill("angle_bb", Event::angle_bb(highestCSVJet, secondHighestCSVJet));
		}
	}

	for (unsigned int index = 0; index < jets.size(); ++index) {
		treeMan_->Fill("jetPt", jets.at(index)->pt() );
		treeMan_->Fill("jetEta", jets.at(index)->eta() );
		treeMan_->Fill("jetParton", jets.at(index)->ttbar_decay_parton() );
	}

	BAT::TtbarHypothesis topHypothesis = event->ttbarHypothesis();
	if ( topHypothesis.isValid() && topHypothesis.isPhysical() ) {
		treeMan_->Fill("lepTopPt",topHypothesis.leptonicTop->pt());
		treeMan_->Fill("hadTopPt",topHypothesis.hadronicTop->pt());
		treeMan_->Fill("lepTopRap",topHypothesis.leptonicTop->rapidity());
		treeMan_->Fill("hadTopRap",topHypothesis.hadronicTop->rapidity());
		treeMan_->Fill("ttbarPt",topHypothesis.resonance->pt());
		treeMan_->Fill("ttbarM",topHypothesis.resonance->mass());
		treeMan_->Fill("ttbarRap",topHypothesis.resonance->rapidity());
	}

	unsigned int numberOfJets(event->NJets(jets));
	unsigned int numberOfBJets(event->NJets(bjets));
	treeMan_->Fill("NJets", numberOfJets );
	treeMan_->Fill("NBJets", numberOfBJets );
}


void UnfoldingRecoAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->addBranch("passSelection", "F", "Unfolding");

	treeMan_->addBranch("MET", "F", "Unfolding");
	treeMan_->addBranch("HT", "F", "Unfolding");
	treeMan_->addBranch("ST", "F", "Unfolding");
	treeMan_->addBranch("WPT", "F", "Unfolding");
	treeMan_->addBranch("MT", "F", "Unfolding");

	treeMan_->addBranch("NJets", "F", "Unfolding");
	treeMan_->addBranch("NBJets", "F", "Unfolding");

	treeMan_->addBranch("angle_bl", "F", "Unfolding");
	treeMan_->addBranch("deltaPhi_bb", "F", "Unfolding");
	treeMan_->addBranch("deltaEta_bb", "F", "Unfolding");
	treeMan_->addBranch("angle_bb", "F", "Unfolding");

	treeMan_->addBranch("lepTopPt", "F", "Unfolding");
	treeMan_->addBranch("hadTopPt", "F", "Unfolding");
	treeMan_->addBranch("lepTopRap", "F", "Unfolding");
	treeMan_->addBranch("hadTopRap", "F", "Unfolding");
	treeMan_->addBranch("ttbarPt", "F", "Unfolding");
	treeMan_->addBranch("ttbarM", "F", "Unfolding");
	treeMan_->addBranch("ttbarRap", "F", "Unfolding");

	treeMan_->addBranch("leptonPt", "F", "Unfolding");
	treeMan_->addBranch("leptonEta", "F", "Unfolding");
	treeMan_->addBranch("leptonCharge", "F", "Unfolding");
	treeMan_->addBranch("LeptonEfficiencyCorrection", "F", "Unfolding");
	treeMan_->addBranch("LeptonEfficiencyCorrectionUp", "F", "Unfolding");
	treeMan_->addBranch("LeptonEfficiencyCorrectionDown", "F", "Unfolding");

	treeMan_->addBranch("LeptonEfficiencyCorrection_etaBins", "F", "Unfolding");
	treeMan_->addBranch("LeptonEfficiencyCorrectionUp_etaBins", "F", "Unfolding");
	treeMan_->addBranch("LeptonEfficiencyCorrectionDown_etaBins", "F", "Unfolding");

	treeMan_->addBranch("MET_METUncertainties", "F", "Unfolding", false);
	treeMan_->addBranch("ST_METUncertainties", "F", "Unfolding", false);
	treeMan_->addBranch("WPT_METUncertainties", "F", "Unfolding", false);
	treeMan_->addBranch("BJetWeight", "F", "Unfolding" );
	treeMan_->addBranch("BJetEfficiencyCorrectionWeight", "F", "Unfolding" );
	treeMan_->addBranch("BJetAlternativeWeight", "F", "Unfolding" );

	treeMan_->addBranch("LightJetUpWeight", "F", "Unfolding" );
	treeMan_->addBranch("LightJetDownWeight", "F", "Unfolding" );
	treeMan_->addBranch("BJetUpWeight", "F", "Unfolding" );
	treeMan_->addBranch("BJetDownWeight", "F", "Unfolding" );

	treeMan_->addBranch("bPt", "F", "Unfolding", false);
	treeMan_->addBranch("bEta", "F", "Unfolding", false);

	treeMan_->addBranch("jetPt", "F", "Unfolding", false);
	treeMan_->addBranch("jetEta", "F", "Unfolding", false);
	treeMan_->addBranch("jetParton", "F", "Unfolding", false);

	for ( unsigned int i = 0; i < 9; ++i ) {
		treeMan_->addBranch("muFmuRWeight_" + to_string(i), "F", "Unfolding");
	}

	for ( unsigned int i = 0; i < 100; ++i ) {
		treeMan_->addBranch("pdfWeight_" + to_string(i), "F", "Unfolding");
	}

	for ( unsigned int i = 0; i < 2; ++i ) {
		treeMan_->addBranch("alphaSWeight_" + to_string(i), "F", "Unfolding");
	}

	for ( unsigned int i = 0; i < 27; ++i ) {
		treeMan_->addBranch("matchingWeight_" + to_string(i), "F", "Unfolding");
	}

	for ( unsigned int i = 0; i < 55; ++i ) {
		treeMan_->addBranch("CT14Weight_" + to_string(i), "F", "Unfolding");
	}
	for ( unsigned int i = 0; i < 56; ++i ) {
		treeMan_->addBranch("MMHT14Weight_" + to_string(i), "F", "Unfolding");
	}

	treeMan_->addBranch("semilepbrUp", "F", "Unfolding");
	treeMan_->addBranch("semilepbrDown", "F", "Unfolding");

	treeMan_->addBranch("petersonFrag", "F", "Unfolding");
	treeMan_->addBranch("centralFrag", "F", "Unfolding");
	treeMan_->addBranch("upFrag", "F", "Unfolding");
	treeMan_->addBranch("downFrag", "F", "Unfolding");
}


UnfoldingRecoAnalyser::UnfoldingRecoAnalyser(TreeManagerPtr treeMan, std::string histogramFolder) :
		BasicAnalyser(treeMan, histogramFolder) {
}

UnfoldingRecoAnalyser::~UnfoldingRecoAnalyser() {
}

} /* namespace BAT */
