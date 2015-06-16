/*
 * LikelihoodInputAnalyser.cpp
 *
 *  Created on: May 5, 2015
 *      Author: ejclemen
 */
#include "../../interface/Analysers/LikelihoodInputAnalyser.h"
#include "../../interface/GlobalVariables.h"
#include <boost/scoped_ptr.hpp>

using namespace BAT;


void LikelihoodInputAnalyser::analyse(const EventPtr event) {

	weight_ = event->weight();
	treeMan_->setCurrentFolder(histogramFolder_);
	unsigned int parton = 0;
	double CSV = -1;

	const JetCollection AllJets(event->Jets());
	const JetCollection jets(event->CleanedJets());
	const JetCollection bjets(event->CleanedBJets());
	// const JetCollection genJets( event->GenJets() );

	METAlgorithm::value metType = (METAlgorithm::value) 0;
	const METPointer met(event->MET(metType));

	if (event->isSemiLeptonicElectron() || event->isSemiLeptonicMuon()){
		for ( unsigned int AllJetIndex=0; AllJetIndex < AllJets.size(); ++AllJetIndex ) {

			JetPointer AllJet = AllJets[AllJetIndex];

			if (AllJet->matched_parton() == 0) continue;//if null pointer

			parton = abs(AllJet->matched_parton()->pdgId());
			CSV = AllJet->getBTagDiscriminator(BAT::BtagAlgorithm::value::CombinedSecondaryVertexV2);

			if ( abs(CSV) >= 11 ){
				// cout << "CSV : " << CSV << " Run Number : " << event->runnumber() << " Event Number : " << event->eventnumber() << " Lumi Number : " << event->lumiblock() << endl;
				// event->inspect(); //For more info on events failing CSV
				continue;
				}

			if ( (parton >= 1 && parton <= 4) || (parton == 21) ){
				treeMan_->Fill("LightJets", CSV);
				// cout << "LightJet CSV : " << CSV << endl;
			}

			else if ( parton == 5 ){
				treeMan_->Fill("BJets", CSV);
				// cout << "BJet CSV : " << CSV << endl;
			}

			else{
				// cout << "Everything is Rubbish" << endl;
			}
		}
	}


	// Require one of the event selections to have been satisfied, and we have a genuine ttbar event
	int selectionCriteria = -1;
	if ( event->PassesElectronTriggerAndSelection() && event->isSemiLeptonicElectron() ) selectionCriteria = SelectionCriteria::ElectronPlusJetsReference;
	else if ( event->PassesMuonTriggerAndSelection() && event->isSemiLeptonicMuon() ) selectionCriteria = SelectionCriteria::MuonPlusJetsReference;
	if ( selectionCriteria < 0 ) return;

	// Get objects
	const LeptonPointer signalLepton = event->getSignalLepton( selectionCriteria );

	JetCollection jetsWithoutBs;
	FourVector WLep, TopLep, WHad, TopHad;

	// Get cleaned jets that aren't b tagged
	for ( unsigned int jetIndex=0; jetIndex < jets.size(); ++jetIndex ) {
		bool isBJet = false;
		JetPointer thisJet = jets[jetIndex];
		for ( unsigned int bJetIndex=0; bJetIndex < bjets.size(); ++bJetIndex ) {
			JetPointer thisBJet = bjets[bJetIndex];
			if ( thisJet == thisBJet ) {
				isBJet = true;
				break;
			}
		}
		if ( !isBJet ) jetsWithoutBs.push_back( thisJet );
	}

	if ( jetsWithoutBs.size() < 2 ) return;

	// cout << endl << "*****************************************" << endl;
	// cout << "Number of Jets : " << jets.size() << endl;
	// cout << "Number of bJets : " << bjets.size() << endl;
	// cout << "Number of Light Jets : " << jets.size() - bjets.size() << endl;
	// cout << "-----------------------------------------" << endl;

	for ( unsigned int bjetIndex=0; bjetIndex < bjets.size(); ++bjetIndex ) {

		JetPointer bjet = bjets[bjetIndex];
		bool leptonicB = false;
		bool hadronicB = false;
		bool mistaggedB = false;
		double CorrectEvent = 0;

		if (bjet->ttbar_decay_parton() == 5){
			leptonicB = true;
			// cout << "B is Actually Leptonic: " << endl;
		}
		if (bjet->ttbar_decay_parton() == 6){
			hadronicB = true;
			mistaggedB = true;
			// cout << "B is Actually Hadronic: " << endl;
		}
		if (bjet->ttbar_decay_parton() < 5){
			mistaggedB = true;
			// cout << "B is Mis-Tagged: " << endl;
		}

		//For particular b jet and signal lepton - return the best solution for the neutrino given the t/W mass constraints
		NeutrinoSolver neutrinoSolver( &(signalLepton->getFourVector()), &(bjet->getFourVector()), 80, 173 );
		double test = -1;
		FourVector neutrino = neutrinoSolver.GetBest(met->getFourVector().Px(), met->getFourVector().Py(), 25., 25., 0., test );
		// cout << "Neutrino ChiSq : " << test << endl;

		// Calculate the leptonic W and hence leptonic top four vectors.
		WLep = neutrino + signalLepton->getFourVector();
		TopLep = WLep + bjet->getFourVector();

		// cout << "-----------------------------------------" << endl;
		// cout << "Mass of Leptonic W : " << WLep.M() << "GeV" << endl;
		// cout << "Mass of Leptonic Top : " << TopLep.M() << "GeV" << endl;

		//Store the outcome of the Chisq test for leptonic/hadronic and incorrect b jets (inc hadronic)
		if (leptonicB == true){
			treeMan_->Fill("Leptonic_B", test);
		}

		if (hadronicB == true){
			treeMan_->Fill("Hadronic_B", test);
		}

		if (mistaggedB == true){
			treeMan_->Fill("False_B", test);
		}

		// Reconstruct the hadronic W and hence top.
		for ( unsigned int jet1Index=0; jet1Index < (jetsWithoutBs.size()-1); ++jet1Index ) {
			for ( unsigned int jet2Index=jet1Index+1; jet2Index < jetsWithoutBs.size(); ++jet2Index ) {
				JetPointer jet1 = jetsWithoutBs[jet1Index];
				JetPointer jet2 = jetsWithoutBs[jet2Index];

				WHad = jet1->getFourVector() + jet2->getFourVector();
				// cout << "-----------------------------------------" << endl;
				// cout << "Mass of Hadronic W : " << WHad.M() << "GeV" << endl;

				for ( unsigned int AltbjetIndex=0; AltbjetIndex < bjets.size(); ++AltbjetIndex ) {
					if (AltbjetIndex == bjetIndex) continue;
					JetPointer AltBjet = bjets[AltbjetIndex];

					TopHad = WHad + AltBjet->getFourVector();
					// cout << "Mass of Hadronic Top : " << TopHad.M() << "GeV" << endl;

					if (AltBjet->ttbar_decay_parton() == 6 && leptonicB == true){
						CorrectEvent = 1;
					}

					treeMan_->Fill("LeptonicTop_Pt",TopLep.Pt() );
					treeMan_->Fill("LeptonicTop_Energy",TopLep.Energy() );
					treeMan_->Fill("LeptonicTop_Mass",TopLep.M() );

					treeMan_->Fill("LeptonicW_Pt",WLep.Pt());
					treeMan_->Fill("LeptonicW_Energy",WLep.Energy());
					treeMan_->Fill("LeptonicW_Mass",WLep.M() );

					treeMan_->Fill("HadronicTop_Pt",TopHad.Pt());
					treeMan_->Fill("HadronicTop_Energy",TopHad.Energy());
					treeMan_->Fill("HadronicTop_Mass",TopHad.M() );

					treeMan_->Fill("HadronicW_Pt",WHad.Pt());
					treeMan_->Fill("HadronicW_Energy",WHad.Energy());
					treeMan_->Fill("HadronicW_Mass",WHad.M() );

					treeMan_->Fill("CorrectReconstruction", CorrectEvent);
				}
			}
		}
	}
}

LikelihoodInputAnalyser::LikelihoodInputAnalyser(HistogramManagerPtr histMan, boost::shared_ptr<TreeManager> treeMan, std::string histogramFolder ) :
		BasicAnalyser(histMan, treeMan, histogramFolder){

}

LikelihoodInputAnalyser::~LikelihoodInputAnalyser() {
}

void LikelihoodInputAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
}

void LikelihoodInputAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);

	treeMan_->addBranch("BRANCH", "F", "Likelihood" + Globals::treePrefix_);
	treeMan_->addBranch("Leptonic_B", "F", "Neutrino" + Globals::treePrefix_);
	treeMan_->addBranch("Hadronic_B", "F", "Neutrino" + Globals::treePrefix_);
	treeMan_->addBranch("False_B", "F", "Neutrino" + Globals::treePrefix_);

	treeMan_->addBranch("BJets", "F", "CSV" + Globals::treePrefix_);
	treeMan_->addBranch("LightJets", "F", "CSV" + Globals::treePrefix_);

	treeMan_->addBranch("LeptonicTop_Pt", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("LeptonicTop_Energy", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("LeptonicTop_Mass", "F", "TopReco" + Globals::treePrefix_);

	treeMan_->addBranch("HadronicTop_Pt", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("HadronicTop_Energy", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("HadronicTop_Mass", "F", "TopReco" + Globals::treePrefix_);

	treeMan_->addBranch("LeptonicW_Pt", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("LeptonicW_Energy", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("LeptonicW_Mass", "F", "TopReco" + Globals::treePrefix_);

	treeMan_->addBranch("HadronicW_Pt", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("HadronicW_Energy", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("HadronicW_Mass", "F", "TopReco" + Globals::treePrefix_);
	
	treeMan_->addBranch("CorrectReconstruction", "F", "TopReco"+ Globals::treePrefix_);
}
