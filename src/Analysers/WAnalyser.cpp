/*
 * WAnalyser.cpp
 *
 *  Created on: 12 Jan 2015
 *      Author: ejclemen
 */

#include "../../interface/Analysers/WAnalyser.h"

#include <boost/scoped_ptr.hpp>

namespace BAT {

void WAnalyser::analyse(const EventPtr event) {
}

void WAnalyser::analyseHadronicW(const EventPtr event, const JetCollection jets, const JetCollection bjets, std::string folder) {

	treeMan_->setCurrentFolder(folder);

	// Get cleaned jets that aren't b tagged
	JetCollection jetsWithoutBs;
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

	// Get each jet pair combination and form a W candidate
	ParticlePointer bestWCand = 0;
	JetPointer bestJet1 = 0;
	JetPointer bestJet2 = 0;
	for ( unsigned int jet1Index=0; jet1Index < jetsWithoutBs.size()-1; ++jet1Index ) {
		for ( unsigned int jet2Index=jet1Index+1; jet2Index < jetsWithoutBs.size(); ++jet2Index ) {
			JetPointer jet1 = jetsWithoutBs[jet1Index];
			JetPointer jet2 = jetsWithoutBs[jet2Index];

			if (jet1->pt()<=30 || jet2->pt()<=30 ) continue;

			ParticlePointer hadronicW( new Particle(*jet1 + *jet2) );

			if ( bestWCand == 0 ) {
				bestWCand = hadronicW;
				bestJet1 = jet1;
				bestJet2 = jet2;
			}
			else if ( fabs( bestWCand->mass() - 80.4 ) > fabs( hadronicW->mass() - 80.4 ) ) {
				bestWCand = hadronicW;
				bestJet1 = jet1;
				bestJet2 = jet2;
			}
		}
	}

	treeMan_->Fill("mjj",bestWCand->mass());

	bool correctWReco = ( ( bestJet1->ttbar_decay_parton() == 3 && bestJet2->ttbar_decay_parton() == 4 ) ||  ( bestJet1->ttbar_decay_parton() == 4 && bestJet2->ttbar_decay_parton() == 3 ));
	treeMan_->Fill("mjj_correctlyRecod",correctWReco);

	treeMan_->Fill("mjj_jet1Pt",bestJet1->pt());
	treeMan_->Fill("mjj_jet2Pt",bestJet2->pt());
	treeMan_->Fill("mjj_jet1Eta",bestJet1->eta());
	treeMan_->Fill("mjj_jet2Eta",bestJet2->eta());

}

void WAnalyser::createTrees( std::string folder ) {
	treeMan_->setCurrentFolder(folder);

	treeMan_->addBranch("mjj", "F", "AnalysisVariables");
	treeMan_->addBranch("mjj_correctlyRecod", "F", "AnalysisVariables");
	treeMan_->addBranch("mjj_jet1Pt", "F", "AnalysisVariables");
	treeMan_->addBranch("mjj_jet2Pt", "F", "AnalysisVariables");
	treeMan_->addBranch("mjj_jet1Eta", "F", "AnalysisVariables");
	treeMan_->addBranch("mjj_jet2Eta", "F", "AnalysisVariables");
}

WAnalyser::WAnalyser(boost::shared_ptr<TreeManager> treeMan, std::string histogramFolder) :
		BasicAnalyser(treeMan, histogramFolder) {
}

WAnalyser::~WAnalyser() {
}

} /* namespace BAT */
