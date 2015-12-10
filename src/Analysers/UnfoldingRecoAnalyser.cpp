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
		// cout << to_string(unc_i) << " " << weight/centralLHEWeight << endl;
		// string uncString = to_string(unc_i);
		treeMan_->Fill( "genWeight_" + to_string(unc_i), weight);
	}

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

	// Get lepton scale factor
	if ( event->PassesMuonTriggerAndSelection() ) {
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		double efficiencyCorrection = signalMuon->getEfficiencyCorrection( 0 );
		treeMan_->Fill("LeptonEfficiencyCorrection", efficiencyCorrection);
		treeMan_->Fill("LeptonEfficiencyCorrectionUp", signalMuon->getEfficiencyCorrection( 1 ));
		treeMan_->Fill("LeptonEfficiencyCorrectionDown", signalMuon->getEfficiencyCorrection( -1 ));
	}
	else if ( event->PassesElectronTriggerAndSelection() ) {
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));
		double efficiencyCorrection = signalElectron->getEfficiencyCorrection( 0 );	
		treeMan_->Fill("LeptonEfficiencyCorrection", efficiencyCorrection);
		treeMan_->Fill("LeptonEfficiencyCorrectionUp", signalElectron->getEfficiencyCorrection( 1 ));
		treeMan_->Fill("LeptonEfficiencyCorrectionDown", signalElectron->getEfficiencyCorrection( -1 ));
	}

	treeMan_->Fill("PUWeight", event->PileUpWeight() );
	treeMan_->Fill("PUWeight_up", event->PileUpWeight(1) );
	treeMan_->Fill("PUWeight_down", event->PileUpWeight(-1) );
	treeMan_->Fill("BJetWeight",event->BJetWeight());
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

	treeMan_->Fill("bPt", bjets[0]->pt() );
	treeMan_->Fill("bEta", bjets[0]->eta() );
	treeMan_->Fill("bPt", bjets[1]->pt() );
	treeMan_->Fill("bEta", bjets[1]->eta() );

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
	treeMan_->addBranch("passSelection", "F", "Unfolding" + Globals::treePrefix_);

	treeMan_->addBranch("MET", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("HT", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("ST", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("WPT", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("MT", "F", "Unfolding" + Globals::treePrefix_);

	treeMan_->addBranch("NJets", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("NBJets", "F", "Unfolding" + Globals::treePrefix_);

	treeMan_->addBranch("lepTopPt", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("hadTopPt", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("lepTopRap", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("hadTopRap", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("ttbarPt", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("ttbarM", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("ttbarRap", "F", "Unfolding" + Globals::treePrefix_);

	treeMan_->addBranch("leptonPt", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("leptonEta", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("LeptonEfficiencyCorrection", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("LeptonEfficiencyCorrectionUp", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("LeptonEfficiencyCorrectionDown", "F", "Unfolding" + Globals::treePrefix_);

	treeMan_->addBranch("MET_METUncertainties", "F", "Unfolding" + Globals::treePrefix_, false);
	treeMan_->addBranch("ST_METUncertainties", "F", "Unfolding" + Globals::treePrefix_, false);
	treeMan_->addBranch("WPT_METUncertainties", "F", "Unfolding" + Globals::treePrefix_, false);
	treeMan_->addBranch("BJetWeight", "F", "Unfolding" + Globals::treePrefix_ );
	treeMan_->addBranch("LightJetUpWeight", "F", "Unfolding" + Globals::treePrefix_ );
	treeMan_->addBranch("LightJetDownWeight", "F", "Unfolding" + Globals::treePrefix_ );
	treeMan_->addBranch("BJetUpWeight", "F", "Unfolding" + Globals::treePrefix_ );
	treeMan_->addBranch("BJetDownWeight", "F", "Unfolding" + Globals::treePrefix_ );

	treeMan_->addBranch("bPt", "F", "Unfolding" + Globals::treePrefix_, false);
	treeMan_->addBranch("bEta", "F", "Unfolding" + Globals::treePrefix_, false);

	for ( unsigned int i = 0; i < 250; ++i ) {
		treeMan_->addBranch("genWeight_" + to_string(i), "F", "GeneratorSystematicWeights" + Globals::treePrefix_);
	}
}

void UnfoldingRecoAnalyser::createHistograms() {

cout << "I shouldn't be here" << endl;

}

UnfoldingRecoAnalyser::UnfoldingRecoAnalyser(boost::shared_ptr<HistogramManager> histMan, TreeManagerPtr treeMan, std::string histogramFolder) :
		BasicAnalyser(histMan, treeMan, histogramFolder) {
}

UnfoldingRecoAnalyser::~UnfoldingRecoAnalyser() {
}

} /* namespace BAT */
