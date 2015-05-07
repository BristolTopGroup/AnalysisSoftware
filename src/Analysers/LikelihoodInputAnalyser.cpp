/*
 * LikelihoodInputAnalyser.cpp
 *
 *  Created on: May 5, 2015
 *      Author: ejclemen
 */
#include "../../interface/Analysers/LikelihoodInputAnalyser.h"
#include "../../interface/GlobalVariables.h"
#include <boost/scoped_ptr.hpp>
#include "../Rochester/NeutrinoSolver.cc"

using namespace BAT;


void LikelihoodInputAnalyser::analyse(const EventPtr event) {

	weight_ = event->weight();
	treeMan_->setCurrentFolder(histogramFolder_);
	unsigned int parton = 0;
	double CSV;

	const JetCollection AllJets(event->Jets());
	const JetCollection jets(event->CleanedJets());
	const JetCollection bjets(event->CleanedBJets());
	const JetCollection genJets( event->GenJets() );

	METAlgorithm::value metType = (METAlgorithm::value) 0;
	const METPointer met(event->MET(metType));

	if (event->isSemiLeptonicElectron() || event->isSemiLeptonicMuon()){
		for ( unsigned int AllJetIndex=0; AllJetIndex < AllJets.size(); ++AllJetIndex ) {

			JetPointer AllJet = AllJets[AllJetIndex];
			parton = AllJet->ttbar_decay_parton();
			CSV = AllJet->getBTagDiscriminator(BAT::BtagAlgorithm::value::CombinedSecondaryVertexV2);


			// // Look at matched generator jets <----------- something im working on (lookiong at differences between jet pt and gen jet pt etc)
			// const ParticlePointer genJet = AllJet->matched_generated_jet();
			// if ( genJet != 0 ) {
			// 	treeMan_->Fill("genJetPt",genJet->pt());
			// 	treeMan_->Fill("genJetEta",genJet->eta());
			// 	treeMan_->Fill("JetPt", AllJet->getFourVector().Pt());
			// 	treeMan_->Fill("JetEta", AllJet->getFourVector().Eta());
			// }



			if ( abs(CSV) >= 11 ){
				cout << "CSV : " << CSV << " Run Number : " << event->runnumber() << " Event Number : " << event->eventnumber() << " Lumi Number : " << event->lumiblock() << endl;
				// event->inspect(); //For more info on events failing
				continue;
				}

			if ( parton == 3 || parton == 4 ){
				treeMan_->Fill("LightJets", CSV);
				// cout << "LightJet CSV : " << CSV << endl;
				if ( CSV == -10 ){
					treeMan_->Fill("LightJetPt", AllJet->getFourVector().Pt());
					treeMan_->Fill("LightJetEta", AllJet->getFourVector().Eta());
				}
			}

			else if ( parton == 5 || parton == 6 ){
				treeMan_->Fill("BJets", CSV);
				// cout << "BJet CSV : " << CSV << endl;
				if ( CSV == -10 ){
					treeMan_->Fill("BJetPt", AllJet->getFourVector().Pt());
					treeMan_->Fill("BJetEta", AllJet->getFourVector().Eta());
				}
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
	FourVector TopHad, TopLep, WHad, WLep;
	unsigned int BestLepbjetIndex, BestHadbjetIndex, BestLightjet1Index, BestLightjet2Index;
	BestLightjet2Index = BestLightjet1Index = BestHadbjetIndex = BestLepbjetIndex = 99;
	double TopMassDiff, CurrentTopMassDiff;
	CurrentTopMassDiff = 9999;

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
					TopMassDiff = TopLep.M()-TopHad.M();

					// cout << "Mass of Hadronic Top : " << TopHad.M() << "GeV" << endl;
					// cout << "Top Mass Difference : " << abs(TopMassDiff) << endl;
					// cout << "-----------------------------------------" << endl;
					if (abs(TopMassDiff) < CurrentTopMassDiff){
						CurrentTopMassDiff = TopMassDiff;
						BestLightjet1Index = jet1Index;
						BestLightjet2Index = jet2Index;
						BestHadbjetIndex = AltbjetIndex;
						BestLepbjetIndex = bjetIndex;
						// cout << " Best Indices : " << BestLightjet1Index << BestLightjet2Index << BestLepbjetIndex << BestHadbjetIndex << endl;
					}

				}

			}

		}

		WHad = jetsWithoutBs[BestLightjet1Index]->getFourVector() + jetsWithoutBs[BestLightjet2Index]->getFourVector();
		TopHad = WHad + bjets[BestHadbjetIndex]->getFourVector() ;

		if (leptonicB == true){

			treeMan_->Fill("TrueLeptonicTop_Pt",TopLep.Pt() );
			treeMan_->Fill("TrueLeptonicTop_Energy",TopLep.Energy() );
			treeMan_->Fill("TrueLeptonicTop_Mass",TopLep.M() );

			treeMan_->Fill("TrueLeptonicW_Pt",WLep.Pt());
			treeMan_->Fill("TrueLeptonicW_Energy",WLep.Energy());
			treeMan_->Fill("TrueLeptonicW_Mass",WLep.M() );

			treeMan_->Fill("TrueHadronicTop_Pt",TopHad.Pt());
			treeMan_->Fill("TrueHadronicTop_Energy",TopHad.Energy());
			treeMan_->Fill("TrueHadronicTop_Mass",TopHad.M() );

			treeMan_->Fill("TrueHadronicW_Pt",WHad.Pt());
			treeMan_->Fill("TrueHadronicW_Energy",WHad.Energy());
			treeMan_->Fill("TrueHadronicW_Mass",WHad.M() );
		}

		else {

			treeMan_->Fill("FalseLeptonicTop_Pt",TopLep.Pt());
			treeMan_->Fill("FalseLeptonicTop_Energy",TopLep.Energy());
			treeMan_->Fill("FalseLeptonicTop_Mass",TopLep.M());

			treeMan_->Fill("FalseLeptonicW_Pt",WLep.Pt());
			treeMan_->Fill("FalseLeptonicW_Energy",WLep.Energy());
			treeMan_->Fill("FalseLeptonicW_Mass",WLep.M());

			treeMan_->Fill("FalseHadronicTop_Pt",TopHad.Pt());
			treeMan_->Fill("FalseHadronicTop_Energy",TopHad.Energy());
			treeMan_->Fill("FalseHadronicTop_Mass",TopHad.M() );

			treeMan_->Fill("FalseHadronicW_Pt",WHad.Pt());
			treeMan_->Fill("FalseHadronicW_Energy",WHad.Energy());
			treeMan_->Fill("FalseHadronicW_Mass",WHad.M() );
		
		}
	}
}

LikelihoodInputAnalyser::LikelihoodInputAnalyser(HistogramManagerPtr histMan, boost::shared_ptr<TreeManager> treeMan, std::string histogramFolder ) :
		BasicAnalyser(histMan, treeMan, histogramFolder)
{

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
	treeMan_->addBranch("LightJetPt", "F", "CSV" + Globals::treePrefix_);
	treeMan_->addBranch("LightJetEta", "F", "CSV" + Globals::treePrefix_);
	treeMan_->addBranch("BJetPt", "F", "CSV" + Globals::treePrefix_);
	treeMan_->addBranch("BJetEta", "F", "CSV" + Globals::treePrefix_);

	// treeMan_->addBranch("genJetPt", "F", "CSV" + Globals::treePrefix_);
	// treeMan_->addBranch("genJetEta", "F", "CSV" + Globals::treePrefix_);
	// treeMan_->addBranch("JetPt", "F", "CSV" + Globals::treePrefix_);
	// treeMan_->addBranch("JetEta", "F", "CSV" + Globals::treePrefix_);


	treeMan_->addBranch("TrueLeptonicTop_Pt", "F", "LeptonicTop" + Globals::treePrefix_);
	treeMan_->addBranch("TrueLeptonicTop_Energy", "F", "LeptonicTop" + Globals::treePrefix_);
	treeMan_->addBranch("TrueLeptonicTop_Mass", "F", "LeptonicTop" + Globals::treePrefix_);

	treeMan_->addBranch("TrueHadronicTop_Pt", "F", "HadronicTop" + Globals::treePrefix_);
	treeMan_->addBranch("TrueHadronicTop_Energy", "F", "HadronicTop" + Globals::treePrefix_);
	treeMan_->addBranch("TrueHadronicTop_Mass", "F", "HadronicTop" + Globals::treePrefix_);

	treeMan_->addBranch("TrueLeptonicW_Pt", "F", "LeptonicW" + Globals::treePrefix_);
	treeMan_->addBranch("TrueLeptonicW_Energy", "F", "LeptonicW" + Globals::treePrefix_);
	treeMan_->addBranch("TrueLeptonicW_Mass", "F", "LeptonicW" + Globals::treePrefix_);

	treeMan_->addBranch("TrueHadronicW_Pt", "F", "HadronicW" + Globals::treePrefix_);
	treeMan_->addBranch("TrueHadronicW_Energy", "F", "HadronicW" + Globals::treePrefix_);
	treeMan_->addBranch("TrueHadronicW_Mass", "F", "HadronicW" + Globals::treePrefix_);


	treeMan_->addBranch("FalseLeptonicTop_Pt", "F", "LeptonicTop" + Globals::treePrefix_);
	treeMan_->addBranch("FalseLeptonicTop_Energy", "F", "LeptonicTop" + Globals::treePrefix_);
	treeMan_->addBranch("FalseLeptonicTop_Mass", "F", "LeptonicTop" + Globals::treePrefix_);

	treeMan_->addBranch("FalseHadronicTop_Pt", "F", "HadronicTop" + Globals::treePrefix_);
	treeMan_->addBranch("FalseHadronicTop_Energy", "F", "HadronicTop" + Globals::treePrefix_);
	treeMan_->addBranch("FalseHadronicTop_Mass", "F", "HadronicTop" + Globals::treePrefix_);

	treeMan_->addBranch("FalseLeptonicW_Pt", "F", "LeptonicW" + Globals::treePrefix_);
	treeMan_->addBranch("FalseLeptonicW_Energy", "F", "LeptonicW" + Globals::treePrefix_);
	treeMan_->addBranch("FalseLeptonicW_Mass", "F", "LeptonicW" + Globals::treePrefix_);

	treeMan_->addBranch("FalseHadronicW_Pt", "F", "HadronicW" + Globals::treePrefix_);
	treeMan_->addBranch("FalseHadronicW_Energy", "F", "HadronicW" + Globals::treePrefix_);
	treeMan_->addBranch("FalseHadronicW_Mass", "F", "HadronicW" + Globals::treePrefix_);
}
