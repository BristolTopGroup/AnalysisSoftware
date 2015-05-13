/*
 * LikelihoodRecoAnalyser.cpp
 *
 *  Created on: Mey 13, 2015
 *      Author: ejclemen
 */
#include "../../interface/Analysers/LikelihoodRecoAnalyser.h"
#include "../../interface/LumiReWeighting.h"
#include "../../interface/EventWeightProvider.h"
#include "../../interface/PoissonMeanShifter.h"
#include "../../interface/GlobalVariables.h"
#include <boost/scoped_ptr.hpp>

//using namespace reweight;
using namespace BAT;

void LikelihoodRecoAnalyser::analyse(const EventPtr event) {
}

void LikelihoodRecoAnalyser::analyse(const EventPtr event, const JetCollection jets, const JetCollection bjets, const LeptonPointer selectedLepton, const METPointer met ) {

	weight_ = event->weight();
	treeMan_->setCurrentFolder(histogramFolder_);
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	int ttbarpartonsfound = 0;
	const JetCollection AllJets(event->Jets());
	int TypeofSolution = 0;//1 = correct, 2=wrong cobination, 3 = not reconstructible, 4 = not semileptonic
	int EventCorrect = 0;//0=False, 1=True

	LikelihoodBasedTopPairReconstruction likelihoodReco( selectedLepton, met, jets, bjets );

	likelihoodReco.reconstruct();

	TtbarHypothesisCollection allSolutions = likelihoodReco.getAllSolutions();


	if (allSolutions.size() != 0){

		for (unsigned int index = 0; index<allSolutions.size(); ++index){
			TtbarHypothesisPointer solution = allSolutions[index];
			double Discrim = solution->discriminator;
			treeMan_->Fill( "likelihoodRatioDiscrimAllJetCombinations", Discrim );
		}

		TtbarHypothesisPointer bestSolution = likelihoodReco.getBestSolution();

		ParticlePointer TopHad = bestSolution->hadronicTop;
		ParticlePointer TopLep = bestSolution->leptonicTop;
		ParticlePointer W = bestSolution->hadronicW;
		// double Nu = bestSolution->neutrinoChi2;

		treeMan_->Fill("LeptonicTop_Pt",TopLep->pt() );
		treeMan_->Fill("LeptonicTop_Eta",TopLep->eta() );
		treeMan_->Fill("LeptonicTop_Energy",TopLep->energy() );
		treeMan_->Fill("LeptonicTop_Mass",TopLep->mass() );

		treeMan_->Fill("HadronicTop_Pt",TopHad->pt());
		treeMan_->Fill("HadronicTop_Eta",TopHad->eta() );
		treeMan_->Fill("HadronicTop_Energy",TopHad->energy());
		treeMan_->Fill("HadronicTop_Mass",TopHad->mass() );

		// Require one of the event selections to have been satisfied, and we have a genuine ttbar event
		int selectionCriteria = -1;
		if ( event->PassesElectronTriggerAndSelection() && event->isSemiLeptonicElectron() ) selectionCriteria = SelectionCriteria::ElectronPlusJetsReference;
		else if ( event->PassesMuonTriggerAndSelection() && event->isSemiLeptonicMuon() ) selectionCriteria = SelectionCriteria::MuonPlusJetsReference;
		if ( selectionCriteria < 0 ) {
			TypeofSolution = 4;
		}


		for ( unsigned int AllJetIndex=0; AllJetIndex < AllJets.size(); ++AllJetIndex ) {

			JetPointer AllJet = AllJets[AllJetIndex];

			if ( AllJet->matched_parton() == 0 ) continue;//if null pointer

			if ( AllJet->ttbar_decay_parton() == 3 ){
				// cout << "Found qbar decay parton" << endl;
				++ttbarpartonsfound;
			}
			if ( AllJet->ttbar_decay_parton() == 4 ){
				// cout << "Found q decay parton" << endl;
				++ttbarpartonsfound;
			}		
			if ( AllJet->ttbar_decay_parton() == 5 ){
				// cout << "Found leptonicb decay parton" << endl;
				++ttbarpartonsfound;
			}		
			if ( AllJet->ttbar_decay_parton() == 6 ){
				// cout << "Found hadronicb decay parton" << endl;
				++ttbarpartonsfound;
			}
		}
		// cout << "-------------------" << endl;
		if ( ttbarpartonsfound != 4 && TypeofSolution == 0){
			TypeofSolution = 3;
		}

		double Discrim = bestSolution->discriminator;
		double CSVDiscrim = bestSolution->CSVDiscriminator;
		double MassDiscrim = bestSolution->MassDiscriminator;
		double NuChi2Discrim = bestSolution->NuChi2Discriminator;

		treeMan_->Fill( "likelihoodRatioDiscrimBest", Discrim );
		treeMan_->Fill( "CSVDiscrimBest", CSVDiscrim );
		treeMan_->Fill( "NuChi2DiscrimBest", NuChi2Discrim );
		treeMan_->Fill( "MassDiscrimBest", MassDiscrim );



		if (bestSolution->isCorrect() && TypeofSolution == 0){
			TypeofSolution = 1;
			EventCorrect = 1;
		}

		else{
			if (TypeofSolution == 0){
				TypeofSolution = 2;
			}
		}
		treeMan_->Fill("EventReconstruction", EventCorrect);
		treeMan_->Fill("TypeofSolution", TypeofSolution);



		// cout << "**********************************" << endl;
		// cout << "Got " << allSolutions.size() << " solutions" << endl;
		// cout << "Is the solution correct : " << bestSolution->isCorrect() << endl;
		// cout << "Got Best Solution : " << Discrim << ", Mass T : " << TopHad->mass() << ", Mass W : " << W->mass() << ", NuChi2 : " << Nu << endl;
		// cout << "NuChi2Discrim : " << NuChi2Discrim << ", MassDiscrim : " << MassDiscrim << ", CSVDiscrim" << CSVDiscrim << endl;
		// cout << "**********************************" << endl;
	}
}

LikelihoodRecoAnalyser::LikelihoodRecoAnalyser(HistogramManagerPtr histMan, TreeManagerPtr treeMan, const bool isElectronChannel, std::string histogramFolder ) :
		BasicAnalyser(histMan, treeMan, histogramFolder) //
{

}

LikelihoodRecoAnalyser::~LikelihoodRecoAnalyser() {
}

void LikelihoodRecoAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

}

void LikelihoodRecoAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);

	treeMan_->addBranch("likelihoodRatioDiscrimBest", "F", "Discriminator" + Globals::treePrefix_);
	treeMan_->addBranch("CSVDiscrimBest", "F", "Discriminator" + Globals::treePrefix_);
	treeMan_->addBranch("MassDiscrimBest", "F", "Discriminator" + Globals::treePrefix_);
	treeMan_->addBranch("NuChi2DiscrimBest", "F", "Discriminator" + Globals::treePrefix_);

	treeMan_->addBranch("likelihoodRatioDiscrimAllJetCombinations", "F", "Discriminator" + Globals::treePrefix_);
	treeMan_->addBranch("EventReconstruction", "F", "Discriminator" + Globals::treePrefix_);

	treeMan_->addBranch("LeptonicTop_Pt", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("LeptonicTop_Eta", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("LeptonicTop_Energy", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("LeptonicTop_Mass", "F", "TopReco" + Globals::treePrefix_);

	treeMan_->addBranch("HadronicTop_Pt", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("HadronicTop_Eta", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("HadronicTop_Energy", "F", "TopReco" + Globals::treePrefix_);
	treeMan_->addBranch("HadronicTop_Mass", "F", "TopReco" + Globals::treePrefix_);

	treeMan_->addBranch("TypeofSolution", "F", "TopReco" + Globals::treePrefix_);

}

