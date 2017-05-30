/*
 * PseudoTopAnalyser.cpp
 *
 *  Created on: 24 Feb 2015
 *      Author: ejclemen
 */

#include "../../interface/Analysers/PseudoTopAnalyser.h"

#include <boost/scoped_ptr.hpp>
#include <iostream>

using namespace std;

namespace BAT {

double const PseudoTopAnalyser::minLeptonPt_ = 26;
double const PseudoTopAnalyser::maxLeptonAbsEta_ = 2.4;
double const PseudoTopAnalyser::minVetoLeptonPt_ = 15;
double const PseudoTopAnalyser::maxVetoLeptonAbsEta_ = 2.4;
double const PseudoTopAnalyser::minNeutrinoSumPt_ = 0;
double const PseudoTopAnalyser::minWMt_ = 0;
unsigned int const PseudoTopAnalyser::minNJets_ = 4;
unsigned int const PseudoTopAnalyser::minNBJets_ = 2;
double const PseudoTopAnalyser::minJetPt_ = 30;
double const PseudoTopAnalyser::maxJetAbsEta_ = 2.4;

void PseudoTopAnalyser::analyse(const EventPtr event) {
	weight_ = event->weight();

	treeMan_->setCurrentFolder(histogramFolder_);

	// Store gen selection criteria
	treeMan_->Fill("isSemiLeptonicElectron", event->isSemiLeptonicElectron());
	treeMan_->Fill("isSemiLeptonicMuon", event->isSemiLeptonicMuon());


	const PseudoTopParticlesPointer pseudoTopParticles = event->PseudoTopParticles();
	const MCParticleCollection pseudoTops = pseudoTopParticles->getPseudoTops();
	const MCParticlePointer pseudoLeptonicW = pseudoTopParticles->getPseudoLeptonicW();
	const MCParticlePointer pseudoLepton = pseudoTopParticles->getPseudoLepton();
	const MCParticleCollection allPseudoLeptons = pseudoTopParticles->getAllPseudoLeptons();
	const MCParticleCollection pseudoBs = pseudoTopParticles->getPseudoBs();
	const ParticlePointer pseudoMET = pseudoTopParticles->getPseudoMET();
	const JetCollection pseudoJets = pseudoTopParticles->getPseudoJets();

	const ParticlePointer genMET = event->GenMET();


	ParticleCollection pseudoTopsForTTbar;

	// // Only consider events with two pseudo tops
	// if ( pseudoTops.size() != 2 ) return;

	// // Also only consider events that are semi leptonic at the pseudo top level
	// if ( !pseudoTopParticles->isSemiLeptonic() ) return;

	// Check if event passes event selection (at pseudo top level)
	bool passesGenSelection = passesEventSelection( pseudoLepton, pseudoJets, pseudoBs, allPseudoLeptons, minNJets_, minNBJets_ );
	if ( passesGenSelection ) {
		++nEventsInPS_;
		treeMan_->Fill("passesGenEventSelection",1);
	}
	else {
		treeMan_->Fill("passesGenEventSelection",0);
	}

	bool passesOfflineSelection = event->PassesElectronTriggerAndSelection() || event->PassesMuonTriggerAndSelection();
	if ( passesOfflineSelection ) ++nEventsOffline_;
	if ( passesOfflineSelection && !passesGenSelection ){
		++nEventsOfflineButNotInPS_;
	}

	if ( passesEventSelection( pseudoLepton, pseudoJets, pseudoBs, allPseudoLeptons, 4, 1 ) ) {
		++nEventsInPS_4j1b_;
	}
	else if ( passesOfflineSelection ){
		++nEventsOfflineButNotInPS_4j1b_;
	}

	if ( passesEventSelection( pseudoLepton, pseudoJets, pseudoBs, allPseudoLeptons, 4, 0 ) ) {
		++nEventsInPS_4j0b_;
	}
	else if ( passesOfflineSelection ){
		++nEventsOfflineButNotInPS_4j0b_;
	}

	if ( passesEventSelection( pseudoLepton, pseudoJets, pseudoBs, allPseudoLeptons, 3, 2 ) ) {
		++nEventsInPS_3j2b_;
	}
	else if ( passesOfflineSelection ){
		++nEventsOfflineButNotInPS_3j2b_;
	}

	if ( passesEventSelection( pseudoLepton, pseudoJets, pseudoBs, allPseudoLeptons, 3, 1 ) ) {
		++nEventsInPS_3j1b_;
	}
	else if ( passesOfflineSelection ){
		++nEventsOfflineButNotInPS_3j1b_;
	}

	if ( passesEventSelection( pseudoLepton, pseudoJets, pseudoBs, allPseudoLeptons, 3, 0 ) ) {
		++nEventsInPS_3j0b_;
	}
	else if ( passesOfflineSelection ){
		++nEventsOfflineButNotInPS_3j0b_;
	}

	//
	// TOP VARIABLES
	// Top reco at particle level performed
	//

	// Store info on top
	for ( unsigned int ptIndex = 0; ptIndex < pseudoTops.size(); ++ ptIndex ) {
		// cout << "Getting pseudo top number : " << ptIndex << endl;
		const ParticlePointer pseudoTop = pseudoTops[ptIndex];
		pseudoTopsForTTbar.push_back( pseudoTop );
		treeMan_->Fill("pseudoTop_pT", pseudoTop->pt() );
		treeMan_->Fill("pseudoTop_y", pseudoTop->rapidity() );
	}

	if( pseudoTopsForTTbar.size() == 2 ) {
		// Store info on ttbar
		ParticlePointer pseudoTTbar( new Particle( *pseudoTopsForTTbar[0] + *pseudoTopsForTTbar[1] ) );
		treeMan_->Fill("pseudoTTbar_pT", pseudoTTbar->pt() );
		treeMan_->Fill("pseudoTTbar_y", pseudoTTbar->rapidity() );
		treeMan_->Fill("pseudoTTbar_m", pseudoTTbar->mass() );
	}


	// Store info on Bs
	if ( pseudoBs.size() == 2 ) {
		unsigned int leadingPsuedoBIndex = 0;
		if ( pseudoBs[1]->pt() > pseudoBs[0]->pt() ) {
			leadingPsuedoBIndex = 1;
		}
		unsigned int subleadingPsuedoBIndex = ( leadingPsuedoBIndex == 0 ) ? 1 : 0;
		treeMan_->Fill("pseudoB_pT", pseudoBs[leadingPsuedoBIndex]->pt() );
		treeMan_->Fill("pseudoB_eta", pseudoBs[leadingPsuedoBIndex]->eta() );
		treeMan_->Fill("pseudoB_pT", pseudoBs[subleadingPsuedoBIndex]->pt() );
		treeMan_->Fill("pseudoB_eta", pseudoBs[subleadingPsuedoBIndex]->eta() );
		// for ( unsigned int pbIndex = 0; pbIndex < pseudoBs.size(); ++pbIndex ) {
		// 	treeMan_->Fill("pseudoB_pT", pseudoBs[pbIndex]->pt() );
		// 	treeMan_->Fill("pseudoB_eta", pseudoBs[pbIndex]->eta() );
		// }
	}

	// std::cout << pseudoBs.size() << " " << allPseudoLeptons.size() << std::endl;
	if ( pseudoBs.size() >= 1 && allPseudoLeptons.size() > 0 ) {
		treeMan_->Fill("angle_bl",Event::pseudo_angle_bl( pseudoBs, allPseudoLeptons[0]));
	}

	if ( pseudoBs.size() >= 2 ) {
		treeMan_->Fill("pseudo_angle_bb",Event::angle_bb( pseudoBs[0], pseudoBs[1]));
		treeMan_->Fill("pseudo_deltaPhi_bb",fabs( Event::deltaPhi_bb( pseudoBs[0], pseudoBs[1]) ) );
		treeMan_->Fill("pseudo_deltaEta_bb",fabs( Event::deltaEta_bb( pseudoBs[0], pseudoBs[1]) ) );
	}
	//
	// GLOBAL VARIABLES
	// No top reco at particle level
	//

	if ( allPseudoLeptons.size() > 0 ) {
		// Store info on lepton
		treeMan_->Fill("pseudoLepton_pT", allPseudoLeptons[0]->pt() );
		treeMan_->Fill("pseudoLepton_eta", allPseudoLeptons[0]->eta() );		

		treeMan_->Fill("pseudoLepton_pdgId", fabs(allPseudoLeptons[0]->pdgId()));
	}
	else {
		treeMan_->Fill("pseudoLepton_pT", -50 );
		treeMan_->Fill("pseudoLepton_eta", -50 );
	}

	// Store pseudo MET
	if ( pseudoMET != 0 ) {
		treeMan_->Fill("pseudoMET", pseudoMET->et() );
		treeMan_->Fill("pseudoMET_mass", pseudoMET->mass() );
	}
	else {
		treeMan_->Fill("pseudoMET", 0 );
	}

	// Store gen MET
	if ( genMET != 0 ) {
		treeMan_->Fill("genMET", genMET->et() );
	}
	else {
		treeMan_->Fill("genMET", 0 );
	}


	// Store pseudo HT
	treeMan_->Fill("pseudoHT", event->HT( pseudoJets ) );

	for (unsigned int index = 0; index < pseudoJets.size(); ++index) {
		treeMan_->Fill("pseudoJet_pT", pseudoJets.at(index)->pt() );
		treeMan_->Fill("pseudoJet_eta", pseudoJets.at(index)->eta() );
	}

	// Store pseudo ST
	if ( allPseudoLeptons.size() > 0 ) {
		treeMan_->Fill("pseudoST", event->ST( pseudoJets, allPseudoLeptons[0], METPointer( new MET( pseudoMET->px(), pseudoMET->py() )) ) );
	}
	else {
		treeMan_->Fill("pseudoST", 0 );
	}

	// Store pseudo MT and WPT
	// These are from the W reconstructed from the pseudo particles
	// i.e. use the neutrino assocaited with the W rather than the more "global" MET
	if ( pseudoLeptonicW != 0 ) {
		treeMan_->Fill("pseudoWPT_reco", pseudoLeptonicW->pt() );
	}
	else {
		treeMan_->Fill("pseudoWPT_reco", 0 );
	}

	if ( pseudoMET != 0 && allPseudoLeptons.size() > 0 ) {
		double WPT = event->WPT( allPseudoLeptons[0], METPointer( new MET( pseudoMET->px(), pseudoMET->py() ))  );
		treeMan_->Fill("pseudoWPT", WPT );

		double MT = event->MT( allPseudoLeptons[0], METPointer( new MET( pseudoMET->px(), pseudoMET->py() )) );
		treeMan_->Fill("pseudoMT", MT );
	}
	else {
		treeMan_->Fill("pseudoWPT", 0 );
		treeMan_->Fill("pseudoMT", 0 );
	}

	// NJets && NBJets
	unsigned int numberOfBJets(0);
	unsigned int numberOfJets(0);
	for (unsigned int index = 0; index < pseudoJets.size(); ++index) {
		const JetPointer jet(pseudoJets.at(index));
		if (jet->pt() < 30 ) continue;
		++numberOfJets;
	}
	for (unsigned int index = 0; index < pseudoBs.size(); ++index) {
		const MCParticlePointer bJet(pseudoBs.at(index));
		if ( bJet->pt() < 30 ) continue;
		++numberOfBJets;
	}

	treeMan_->Fill("NPseudoJets", numberOfJets );
	treeMan_->Fill("NPseudoBJets", numberOfBJets );

}

void PseudoTopAnalyser::createTrees() {

	treeMan_->setCurrentFolder(histogramFolder_);
	// Generator level selecton
	treeMan_->addBranch("isSemiLeptonicElectron", "F","Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("isSemiLeptonicMuon", "F","Unfolding" + Globals::treePrefix_);

	treeMan_->addBranch("passesGenEventSelection","F","Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("pseudoLepton_pdgId", "F", "Unfolding" + Globals::treePrefix_);

	// Branches for top
	treeMan_->addBranch("pseudoTop_pT", "F", "Unfolding" + Globals::treePrefix_, false);
	treeMan_->addBranch("pseudoTop_y", "F", "Unfolding" + Globals::treePrefix_, false);
	// Branches for ttbar
	treeMan_->addBranch("pseudoTTbar_pT", "F","Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("pseudoTTbar_y", "F","Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("pseudoTTbar_m", "F","Unfolding" + Globals::treePrefix_);
	// Branches for lepton
	treeMan_->addBranch("pseudoLepton_pT", "F","Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("pseudoLepton_eta", "F","Unfolding" + Globals::treePrefix_);
	// Branches for b jets
	treeMan_->addBranch("pseudoB_pT", "F", "Unfolding" + Globals::treePrefix_, false);
	treeMan_->addBranch("pseudoB_eta", "F", "Unfolding" + Globals::treePrefix_, false);
	treeMan_->addBranch("pseudo_angle_bl", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("pseudo_deltaPhi_bb", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("pseudo_deltaEta_bb", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("pseudo_angle_bb", "F", "Unfolding" + Globals::treePrefix_);
	// Branches for jets
	treeMan_->addBranch("pseudoJet_pT", "F", "Unfolding" + Globals::treePrefix_, false);
	treeMan_->addBranch("pseudoJet_eta", "F", "Unfolding" + Globals::treePrefix_, false);
	// Branch for pseudo MET
	treeMan_->addBranch("pseudoMET", "F","Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("pseudoMET_mass", "F","Unfolding" + Globals::treePrefix_);
	// Branch for gen MET
	treeMan_->addBranch("genMET", "F","Unfolding" + Globals::treePrefix_);
	// Branches for other pseudo global variables
	treeMan_->addBranch("pseudoHT", "F","Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("pseudoST", "F","Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("pseudoWPT_reco", "F","Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("pseudoWPT", "F","Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("pseudoMT", "F","Unfolding" + Globals::treePrefix_);

	// Number of pseudo jets
	treeMan_->addBranch("NPseudoJets", "F", "Unfolding" + Globals::treePrefix_);
	treeMan_->addBranch("NPseudoBJets", "F", "Unfolding" + Globals::treePrefix_);
}

bool PseudoTopAnalyser::passesEventSelection( const MCParticlePointer pseudoLepton, const JetCollection pseudoJets, const MCParticleCollection pseudoBs, const MCParticleCollection allPseudoLeptons ) {

	// Event selection taken from here : https://twiki.cern.ch/twiki/bin/view/LHCPhysics/ParticleLevelTopDefinitions
	unsigned int numberGoodLeptons = 0;
	unsigned int numberVetoLeptons = 0;
	ParticlePointer leadingLepton;

	// Lepton selection
	for ( unsigned int leptonIndex = 0; leptonIndex < allPseudoLeptons.size(); ++ leptonIndex ) {
		const MCParticlePointer lepton = allPseudoLeptons.at(leptonIndex);

		// Check if this is a good signal type lepton
		if ( lepton->pt() > minLeptonPt_ && fabs(lepton->eta()) < maxLeptonAbsEta_ ) {
			++numberGoodLeptons;
			if ( leadingLepton == 0 ) leadingLepton = lepton;
		}
		
		// Check if this is a veto lepton
		if ( lepton->pt() > minVetoLeptonPt_ && fabs(lepton->eta()) < maxVetoLeptonAbsEta_ ) {
			++numberVetoLeptons;
		}
	}

	// Jets
	unsigned int numberGoodJets = 0;
	unsigned int numberGoodBJets = 0;

	unsigned int numberGoodJets_lowerPt = 0;
	unsigned int numberGoodBJets_lowerPt = 0;

	for ( unsigned int jetIndex = 0; jetIndex < pseudoJets.size(); ++ jetIndex ) {
		const JetPointer jet = pseudoJets.at(jetIndex);

		// Check if this is a good jet
		if ( jet->pt() > 20 && fabs(jet->eta()) < maxJetAbsEta_ ) {
			++numberGoodJets_lowerPt;
			if ( fabs( jet->partonFlavour() ) == 5 ) {
				++numberGoodBJets_lowerPt;
			}
		}

		if ( jet->pt() > minJetPt_ && fabs(jet->eta()) < maxJetAbsEta_ ) {
			++numberGoodJets;

			// Check if this is also a good b jet
			if ( fabs( jet->partonFlavour() ) == 5 ) {
				++numberGoodBJets;
			}
		}
	}
	
	// if ( numberGoodLeptons == 1 && numberVetoLeptons <= 1 && numberGoodJets >= minNJets && numberGoodBJets >= minNBJets ) {
	if ( numberGoodLeptons == 1 && numberVetoLeptons <= 1 && numberGoodJets >= 3 && numberGoodBJets >= 1 && numberGoodJets_lowerPt >= 4 && numberGoodBJets_lowerPt >= 2 ) {
		return true;
	}
	else return false;

}

PseudoTopAnalyser::PseudoTopAnalyser(TreeManagerPtr treeMan, std::string histogramFolder) :
		BasicAnalyser(treeMan, histogramFolder),
		nEventsInPS_(0),
		nEventsOfflineButNotInPS_(0),
		nEventsInPS_4j0b_(0),
		nEventsOfflineButNotInPS_4j0b_(0),
		nEventsInPS_4j1b_(0),
		nEventsOfflineButNotInPS_4j1b_(0),
		nEventsInPS_3j2b_(0),
		nEventsOfflineButNotInPS_3j2b_(0),
		nEventsInPS_3j0b_(0),
		nEventsOfflineButNotInPS_3j0b_(0),
		nEventsInPS_3j1b_(0),
		nEventsOfflineButNotInPS_3j1b_(0),
		nEventsOffline_(0)
		 {
}

PseudoTopAnalyser::~PseudoTopAnalyser() {
	std::cout << "PseudoTopAnalyser : Phase Space Info" << std::endl;
	std::cout << "N event offline : " << nEventsOffline_ << std::endl;

	std::cout << "N jets : " << minNJets_ << std::endl;
	std::cout << "N b jets : " << minNBJets_ << std::endl;
	std::cout << "Number events in PS : " << nEventsInPS_ << std::endl;
	std::cout << "Size wrt default : " << float( nEventsInPS_ ) / float( nEventsInPS_ ) * 100 << std::endl;
	std::cout << "Number events offline but not in PS : " << nEventsOfflineButNotInPS_ << std::endl;
	std::cout << "Fraction offline but not in PS : " << float( nEventsOfflineButNotInPS_ ) / float( nEventsOffline_ ) * 100 << std::endl;

	std::cout << "N jets : 4" << std::endl;
	std::cout << "N b jets : 1" << std::endl;
	std::cout << "Number events in PS : " << nEventsInPS_4j1b_ << std::endl;
	std::cout << "Size wrt default : " << float( nEventsInPS_4j1b_ ) / float( nEventsInPS_ ) * 100 << std::endl;
	std::cout << "Number events offline but not in PS : " << nEventsOfflineButNotInPS_4j1b_ << std::endl;
	std::cout << "Fraction offline but not in PS : " << float( nEventsOfflineButNotInPS_4j1b_) / float( nEventsOffline_ ) * 100 << std::endl;
	std::cout << std::endl;

	std::cout << "N jets : 4" << std::endl;
	std::cout << "N b jets : 0" << std::endl;
	std::cout << "Number events in PS : " << nEventsInPS_4j0b_ << std::endl;
	std::cout << "Size wrt default : " << float( nEventsInPS_4j0b_ ) / float( nEventsInPS_ ) * 100 << std::endl;
	std::cout << "Number events offline but not in PS : " << nEventsOfflineButNotInPS_4j0b_ << std::endl;
	std::cout << "Fraction offline but not in PS : " << float( nEventsOfflineButNotInPS_4j0b_) / float( nEventsOffline_ ) * 100 << std::endl;
	std::cout << std::endl;

	std::cout << "N jets : 3" << std::endl;
	std::cout << "N b jets : 2" << std::endl;
	std::cout << "Number events in PS : " << nEventsInPS_3j2b_ << std::endl;
	std::cout << "Size wrt default : " << float( nEventsInPS_3j2b_ ) / float( nEventsInPS_ ) * 100 << std::endl;
	std::cout << "Number events offline but not in PS : " << nEventsOfflineButNotInPS_3j2b_ << std::endl;
	std::cout << "Fraction offline but not in PS : " << float( nEventsOfflineButNotInPS_3j2b_) / float( nEventsOffline_ ) * 100 << std::endl;
	std::cout << std::endl;

	std::cout << "N jets : 3" << std::endl;
	std::cout << "N b jets : 1" << std::endl;
	std::cout << "Number events in PS : " << nEventsInPS_3j1b_ << std::endl;
	std::cout << "Size wrt default : " << float( nEventsInPS_3j1b_ ) / float( nEventsInPS_ ) * 100 << std::endl;
	std::cout << "Number events offline but not in PS : " << nEventsOfflineButNotInPS_3j1b_ << std::endl;
	std::cout << "Fraction offline but not in PS : " << float( nEventsOfflineButNotInPS_3j1b_) / float( nEventsOffline_ ) * 100 << std::endl;
	std::cout << std::endl;

	std::cout << "N jets : 3" << std::endl;
	std::cout << "N b jets : 0" << std::endl;
	std::cout << "Number events in PS : " << nEventsInPS_3j0b_ << std::endl;
	std::cout << "Size wrt default : " << float( nEventsInPS_3j0b_ ) / float( nEventsInPS_ ) * 100 << std::endl;
	std::cout << "Number events offline but not in PS : " << nEventsOfflineButNotInPS_3j0b_ << std::endl;
	std::cout << "Fraction offline but not in PS : " << float( nEventsOfflineButNotInPS_3j0b_) / float( nEventsOffline_ ) * 100 << std::endl;
	std::cout << std::endl;

}

} /* namespace BAT */
