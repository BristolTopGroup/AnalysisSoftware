/*
 * HitFitAnalyser.cpp
 *
 *  Created on: Jul 5, 2011
 *      Author: senkin
 */
#include "../../interface/Analysers/HitFitAnalyser.h"
#include "../../interface/LumiReWeighting.h"
#include "../../interface/EventWeightProvider.h"
#include "../../interface/PoissonMeanShifter.h"
#include "../../interface/GlobalVariables.h"
#include "../../interface/ReconstructionModules/ChiSquaredBasedTopPairReconstruction.h"
#include <boost/scoped_ptr.hpp>

//using namespace reweight;
using namespace BAT;

void HitFitAnalyser::analyse(const EventPtr event) {
}

BAT::TtbarHypothesis HitFitAnalyser::analyseAndReturn(const EventPtr event, const JetCollection jets, const JetCollection bjets, const LeptonPointer selectedLepton ) {

	weight_ = event->weight() * prescale_ * scale_;
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	treeMan_->setCurrentFolder(histogramFolder_);
	// const JetCollection jets(event->getCleanedJets( SelectionCriteria::ElectronPlusJetsReference ));
	// const JetCollection bJets(event->getCleanedBJets( SelectionCriteria::ElectronPlusJetsReference ));
	// const LeptonPointer selectedLepton = event->getSignalLepton( SelectionCriteria::ElectronPlusJetsReference );

	string metPrefix = METAlgorithm::names.at(0);
	const METPointer met(event->MET((METAlgorithm::value) 0));

	int lastTTBarJetPosition = positionOfLastTTBarJet( jets );
	if ( lastTTBarJetPosition != -1 ) treeMan_->Fill("PositionOfLastTTbarJet", lastTTBarJetPosition + 1 );
	else treeMan_->Fill("PositionOfLastTTbarJet", lastTTBarJetPosition );

	// Get cleaned jets that aren't b tagged
	JetCollection leadingLightJets;
	JetCollection leadingBJets;
	unsigned int maxNJet = std::min(5, int(jets.size()));
	for ( unsigned int jetIndex=0; jetIndex < maxNJet; ++jetIndex ) {
		bool isBJet = false;
		JetPointer thisJet = jets[jetIndex];
		for ( unsigned int bJetIndex=0; bJetIndex < bjets.size(); ++bJetIndex ) {
			JetPointer thisBJet = bjets[bJetIndex];
			if ( thisJet == thisBJet ) {
				isBJet = true;
				break;
			}
		}
		if ( !isBJet ) leadingLightJets.push_back( thisJet );
		else leadingBJets.push_back( thisJet );
	}

	histMan_->setCurrentHistogramFolder(histogramFolder_);

	//set MC matching flag
	if (event->getDataType() == DataType::TTJets)
		do_MC_matching = true;
	else
		do_MC_matching = false;

	//prepare the jets collection
	// Copy jets into an array
	JetCollection jetCopy;
	for (JetCollection::const_iterator j = leadingLightJets.begin(); j != leadingLightJets.end(); ++j) {
		jetCopy.push_back(*j);
	}

	JetCollection bJetCopy;
	for (JetCollection::const_iterator j = leadingBJets.begin(); j != leadingBJets.end(); ++j) {
		bJetCopy.push_back(*j);
	}

	std::sort(jetCopy.begin(), jetCopy.end(), jetPtComp);
	std::sort(bJetCopy.begin(), bJetCopy.end(), jetPtComp);

	jetsForFitting.clear();
	unsigned numJetsToFit = jetCopy.size();

	if (jetCopy.size() >= 2) {
		if (numJetsToFit > 5)
			numJetsToFit = 5;
		jetsForFitting.insert(jetsForFitting.begin(), jetCopy.begin(), jetCopy.begin() + numJetsToFit);
	}

	bJetsForFitting.clear();
	numJetsToFit = bJetCopy.size();
	if (bJetCopy.size() >= 2) {
		if (numJetsToFit > 5)
			numJetsToFit = 5;
		bJetsForFitting.insert(bJetsForFitting.begin(), bJetCopy.begin(), bJetCopy.begin() + numJetsToFit);
	}

	BatHitFit hhFitter(electronTranslator_, muonTranslator_, jetTranslator_, metTranslator_, hitfitDefault_,
			hitfitLepWMass_, hitfitHadWMass_, hitfitTopMass_);

	// Clear the internal state
	hhFitter.clear();

	// Add lepton into hitfit
	if ( isElectronChannel_ ) {
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(selectedLepton));
		//TODO: fix the fitter to accept lepton class OR particle!!
		hhFitter.AddLepton(*signalElectron);		
	}
	else {
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(selectedLepton));
		//TODO: fix the fitter to accept lepton class OR particle!!
		hhFitter.AddLepton(*signalMuon);
	}


	// Add jets into HitFit
	// Also check if jets matched to ttbar partons are in the jets passed to the fit
	bool quarkInCollection = false, quarkBarInCollection = false, lebBInCollection = false, hadBInCollection = false;
	for (size_t jet = 0; jet != jetsForFitting.size(); ++jet) {
		hhFitter.AddJet(*jetsForFitting.at(jet));
		// cout << "Adding light jet with csv : " << jetsForFitting.at(jet)->getBTagDiscriminator(BAT::BtagAlgorithm::value::CombinedSecondaryVertexV2) << endl;
		if ( jetsForFitting.at(jet)->ttbar_decay_parton() ) {
			int partonPdg = jetsForFitting.at(jet)->ttbar_decay_parton();
			if ( partonPdg == 3 ) quarkInCollection = true;
			else if ( partonPdg == 4 ) quarkBarInCollection = true;
			else if ( partonPdg == 5 ) lebBInCollection = true;
			else if ( partonPdg == 6 ) hadBInCollection = true;
		}
	}
	for (size_t jet = 0; jet != bJetsForFitting.size(); ++jet) {
		hhFitter.AddBJet(*bJetsForFitting.at(jet));
		// cout << "Adding b jet with csv : " << bJetsForFitting.at(jet)->getBTagDiscriminator(BAT::BtagAlgorithm::value::CombinedSecondaryVertexV2) << endl;
		if ( bJetsForFitting.at(jet)->ttbar_decay_parton() ) {
			int partonPdg = bJetsForFitting.at(jet)->ttbar_decay_parton();
			if ( partonPdg == 3 ) quarkInCollection = true;
			else if ( partonPdg == 4 ) quarkBarInCollection = true;
			else if ( partonPdg == 5 ) lebBInCollection = true;
			else if ( partonPdg == 6 ) hadBInCollection = true;
		}

	}

	// Check if jets matched to ttbar partons are in the jets passed to the fit
	if (do_MC_matching) {
		if ( quarkInCollection && quarkBarInCollection && lebBInCollection && hadBInCollection  ) {
			allTTBarJetsPassedToFit_ = true;
		}
	}

	// Add missing transverse energy into HitFit
	hhFitter.SetMet(*met);

	// Container for input of all jet permutation of the event
	std::vector<hitfit::Lepjets_Event> hitfitEventsInput;

	// Container for results of the fit for all jet permutation of the event
	std::vector<hitfit::Fit_Result> hitfitResult;
	//
	// R U N   H I T F I T
	//
	// Run the kinematic fit and get how many permutations is possible
	// in the fit

	size_t nHitFit = hhFitter.FitAllPermutation();

//   // Get the number of jets
	// nHitFitJet      = hhFitter.GetEvent().njets();

	// Get the input events for all permutations
	hitfitEventsInput = hhFitter.GetUnfittedEvent();

	// Get the fit results for all permutations
	hitfitResult = hhFitter.GetFitAllPermutation();

	double bestChi2 = 999.;
	unsigned bestX2pos = nHitFit + 1;

//   // Loop over all permutations and extract the information
	for (size_t fit = 0; fit != nHitFit; ++fit) {

		// Get the event before the fit
		hitfit::Lepjets_Event unfittedEvent = hitfitEventsInput[fit];

		// Get the event after the fit
		hitfit::Fit_Result fitResult = hitfitResult[fit];

		if (hitfitResult[fit].chisq() > 0.0) {

			treeMan_->Fill("FitChiSquaredAllSolutions",fitResult.chisq());
		}
		// Is this the permutation with smallest chi2?
		if (fitResult.chisq() > 0.0 && fitResult.chisq() < bestChi2) {
			bestChi2 = fitResult.chisq();
			bestX2pos = fit;
		}

	}

	//
	// END PART WHICH EXTRACTS INFORMATION FROM HITFIT
	//

	if (bestX2pos < nHitFit + 1) {
		// histMan_->H1D("FittedTopMassBestSolution")->Fill(hitfitResult[bestX2pos].mt(), weight_);
		// histMan_->H1D("FitChiSquaredBestSolution")->Fill(hitfitResult[bestX2pos].chisq(), weight_);
		// histMan_->H1D("FitLogChiSqdBestSolution")->Fill(log(hitfitResult[bestX2pos].chisq()), weight_);

		treeMan_->Fill("FitChiSquaredBestSolutions",hitfitResult[bestX2pos].chisq());

		//pass hitfit event into BAT format
		lepton_charge = selectedLepton->charge();
		// cout << "Best chi2 : " << hitfitResult[bestX2pos].chisq() << endl;
		BAT::TtbarHypothesis newHyp = BatEvent(hitfitResult[bestX2pos].ev(), event);

		treeMan_->Fill("FittedLeptonicTopPtBestSolution", newHyp.leptonicTop->pt());
		treeMan_->Fill("FittedHadronicTopPtBestSolution", newHyp.hadronicTop->pt());
		treeMan_->Fill("FittedLeptonicTopRapidityBestSolution", newHyp.leptonicTop->rapidity());
		treeMan_->Fill("FittedHadronicTopRapidityBestSolution", newHyp.hadronicTop->rapidity());
		treeMan_->Fill("FittedTTbarMassBestSolution", newHyp.resonance->mass());
		treeMan_->Fill("FittedTTbarPtBestSolution", newHyp.resonance->pt());
		treeMan_->Fill("FittedTTbarRapidityBestSolution", newHyp.resonance->rapidity());

		// const hitfit::Column_Vector &px = hitfitResult[bestX2pos].pullx();
		// const hitfit::Column_Vector &py = hitfitResult[bestX2pos].pully();
		// double sumPx = 0.0;
		// for (int i = 0; i < px.num_row(); ++i) {
		// 	histMan_->H1D("PullDistAllVarsBestSolution")->Fill(px[i]);
		// 	histMan_->H2D("PullDistPerVarBestSolution")->Fill(px[i], i);
		// 	sumPx += px[i] * px[i];
		// }
		// histMan_->H1D("PullSumSquaredBestSolution")->Fill(sumPx);
		// for (int i = 0; i < py.num_row(); ++i) {
		// 	histMan_->H1D("PullDistYVarsBestSolution")->Fill(py[i]);
		// }

		// histMan_->H1D("FittedTopPtBestSolution")->Fill(newHyp.leptonicTop->pt(), weight_);
		// histMan_->H1D("FittedTopPtBestSolution")->Fill(newHyp.hadronicTop->pt(), weight_);
		// histMan_->H1D("FittedTopRapidityBestSolution")->Fill(newHyp.leptonicTop->rapidity(), weight_);
		// histMan_->H1D("FittedTopRapidityBestSolution")->Fill(newHyp.hadronicTop->rapidity(), weight_);

		// histMan_->H1D("FittedTTbarMassBestSolution")->Fill(newHyp.resonance->mass(), weight_);
		// histMan_->H1D("FittedTTbarPtBestSolution")->Fill(newHyp.resonance->pt(), weight_);
		// histMan_->H1D("FittedTTbarRapidityBestSolution")->Fill(newHyp.resonance->rapidity(), weight_);

		return newHyp;

	} else {
		// cout << "No HitFit solution found for this event" << endl;
		treeMan_->Fill("SolutionCategory", 0 );
	}

	return BAT::TtbarHypothesis();

}

HitFitAnalyser::HitFitAnalyser(HistogramManagerPtr histMan, TreeManagerPtr treeMan, const bool isElectronChannel, std::string histogramFolder ) :
		BasicAnalyser(histMan, treeMan, histogramFolder), //
		// The following five initializers read the config parameters for the
		// ASCII text files which contains the physics object resolutions.
		FitterPath_(Globals::TQAFPath), //
		hitfitDefault_(FitterPath_ + "TopQuarkAnalysis/TopHitFit/data/setting/RunHitFitConfiguration.txt"), //
		hitfitElectronResolution_(
				FitterPath_ + "TopQuarkAnalysis/TopHitFit/data/resolution/tqafElectronResolution.txt"), //
		hitfitMuonResolution_(FitterPath_ + "TopQuarkAnalysis/TopHitFit/data/resolution/tqafMuonResolution.txt"), //
		hitfitUdscJetResolution_(FitterPath_ + "TopQuarkAnalysis/TopHitFit/data/resolution/tqafUdscJetResolution.txt"), //
		hitfitBJetResolution_(FitterPath_ + "TopQuarkAnalysis/TopHitFit/data/resolution/tqafBJetResolution.txt"), //
		hitfitMETResolution_(FitterPath_ + "TopQuarkAnalysis/TopHitFit/data/resolution/tqafKtResolution.txt"), //
		// The following three initializers read the config parameters for the
		// values to which the leptonic W, hadronic W, and top quark masses are to
		// be constrained to.
		hitfitLepWMass_(80.4), //
		hitfitHadWMass_(80.4), //
		hitfitTopMass_(172.5), //
		lepton_charge(0.0),
		do_MC_matching(false),
		allTTBarJetsPassedToFit_(false),
		// Tells hit fit whether event contains a signal electron or a signal muon
		isElectronChannel_(isElectronChannel), //
		// The following three initializers instantiate the translator between PAT objects
		// and HitFit objects using the ASCII text files which contains the resolutions.
		electronTranslator_(hitfitElectronResolution_), //
		muonTranslator_(hitfitMuonResolution_), //
		jetTranslator_(hitfitUdscJetResolution_, hitfitBJetResolution_), //
		metTranslator_(hitfitMETResolution_)
{

}

BAT::TtbarHypothesis HitFitAnalyser::BatEvent(const hitfit::Lepjets_Event& ev, const EventPtr event ) {
	// Do the electron
	BAT::LeptonPointer newLepton(new BAT::Electron());
	if ( !isElectronChannel_ ) {
		newLepton = BAT::LeptonPointer(new BAT::Muon());
	}

	if (ev.nleps() > 0) {
		newLepton->setFourVector(fourVectorFromHitFit(ev.lep(0).p()));
	} else {
		std::cout << "No lepton in HitFit event!" << std::endl;
	}

	// Do the neutrino
	//BAT::ParticlePointer newMet(new BAT::Particle(*truthMatchEvent.neutrinoFromW));
	BAT::ParticlePointer newMet(new BAT::Particle());
	newMet->setFourVector(fourVectorFromHitFit(ev.met()));

	// Do the jets
	BAT::JetPointer newLepB(new BAT::Jet());
	BAT::JetPointer newHadB(new BAT::Jet());
	BAT::JetPointer newWj1(new BAT::Jet());
	BAT::JetPointer newWj2(new BAT::Jet());

	// We need to match up the jets in the HitFit event against the input ones
	// Assume that HitFit has preserved the jet order
	std::vector<hitfit::Lepjets_Event_Jet>::size_type i = 0;
	for (BAT::JetCollection::const_iterator j = jetsForFitting.begin(); j != jetsForFitting.end(); ++i, ++j) {
		FourVector hfJet = fourVectorFromHitFit(ev.jet(i).p());
		int hfJType = ev.jet(i).type();
//    if ((*j)->getFourVector().DeltaR(hfJet) < 0.005) {
		if (hfJType != hitfit::unknown_label) {
			BAT::Jet newJet(**j);
			newJet.setFourVector(hfJet);

			if (hfJType == hitfit::lepb_label)
				*newLepB = newJet;
			if (hfJType == hitfit::hadb_label)
				*newHadB = newJet;
			if (hfJType == hitfit::hadw1_label)
				*newWj1 = newJet;
			if (hfJType == hitfit::hadw2_label)
				*newWj2 = newJet;
		}
	}

	for (BAT::JetCollection::const_iterator j = bJetsForFitting.begin(); j != bJetsForFitting.end(); ++i, ++j) {
		FourVector hfJet = fourVectorFromHitFit(ev.jet(i).p());
		int hfJType = ev.jet(i).type();
//    if ((*j)->getFourVector().DeltaR(hfJet) < 0.005) {
		if (hfJType != hitfit::unknown_label) {
			BAT::Jet newJet(**j);
			newJet.setFourVector(hfJet);
			if (hfJType == hitfit::lepb_label)
				*newLepB = newJet;
			if (hfJType == hitfit::hadb_label)
				*newHadB = newJet;
			if (hfJType == hitfit::hadw1_label)
				*newWj1 = newJet;
			if (hfJType == hitfit::hadw2_label)
				*newWj2 = newJet;
		}
	}

	BAT::TtbarHypothesis hyp(newLepton, newMet, newLepB, newHadB, newWj1, newWj2);
	hyp.combineReconstructedObjects();

	// do MC matching study
	if (do_MC_matching) {


		// Check if jets are in correct position
		if ( ( newWj1->ttbar_decay_parton() == 3 || newWj1->ttbar_decay_parton() == 4 ) &&
			 ( newWj2->ttbar_decay_parton() == 3 || newWj2->ttbar_decay_parton() == 4 ) && 
			 ( newLepB->ttbar_decay_parton() == 5 ) &&
			 ( newHadB->ttbar_decay_parton() == 6 ) ) {
			// Correct
			treeMan_->Fill("SolutionCategory", 1 );
		}
		else if ( !( event->isSemiLeptonicElectron() || event->isSemiLeptonicMuon() ) ) {
			// Not a genuine semi leptonic event
			treeMan_->Fill("SolutionCategory", 2 );
		}
		else if ( !allTTBarJetsPassedToFit_ ) {
			// Not all jets from ttbar were passed to fit
			treeMan_->Fill("SolutionCategory", 3 );
		}
		else if ( allTTBarJetsPassedToFit_ && ( newWj1->ttbar_decay_parton() == 0 || newWj2->ttbar_decay_parton() == 0 || newLepB->ttbar_decay_parton() == 0 || newHadB->ttbar_decay_parton() != 0 ) ) {
			// All ttbar jets were available, but at least one incorrect jet used
			treeMan_->Fill("SolutionCategory", 4 );
		}
		else if ( ( newWj1->ttbar_decay_parton() == 3 || newWj1->ttbar_decay_parton() == 4 ) &&
			 ( newWj2->ttbar_decay_parton() == 3 || newWj2->ttbar_decay_parton() == 4 ) && 
			 ( newLepB->ttbar_decay_parton() == 6 ) &&
			 ( newHadB->ttbar_decay_parton() == 5 ) ) {
			// B jets swapped, but W's correct
			treeMan_->Fill("SolutionCategory", 5 );
		}
		else if ( ( ( newWj1->ttbar_decay_parton() == 5 || newWj1->ttbar_decay_parton() == 6 ) ||
			 ( newWj2->ttbar_decay_parton() == 5 || newWj2->ttbar_decay_parton() == 6 ) ) || 
			 ( ( newLepB->ttbar_decay_parton() == 3 || newLepB->ttbar_decay_parton() == 4 ) ||
			 ( newHadB->ttbar_decay_parton() == 3 || newHadB->ttbar_decay_parton() == 4 ) ) ) {
			// Light jet from W assigned as one of b's
			// Or B jet from top assigend as light jet
			treeMan_->Fill("SolutionCategory", 6 );
		}
		else {
			// Just plain wrong
			treeMan_->Fill("SolutionCategory", 7 );
		}
	// 	cout << "Doing MC matching" << endl;
	// 	//Particle Pointers for best fitted hypothesis
	// 	BAT::ParticlePointer hadronicTop, leptonicTop, leptonicW, hadronicW;
	// 	leptonicW = ParticlePointer(new Particle(*newMet + *newEle));
	// 	if (newWj1 != newWj2)
	// 		hadronicW = ParticlePointer(new Particle(*newWj1 + *newWj2));
	// 	else
	// 		hadronicW = newWj1;
	// 	leptonicTop = ParticlePointer(new Particle(*newLep + *leptonicW));
	// 	hadronicTop = ParticlePointer(new Particle(*newHad + *hadronicW));

	// 	double deltaRelectron, deltaRhadronicBjet, deltaRleptonicBjet, deltaRWjet1, deltaRWjet2, sumDeltaR;
	// 	deltaRelectron = truthMatchEvent.leptonFromW->deltaR(hyp.leptonFromW);
	// 	deltaRhadronicBjet = truthMatchEvent.hadronicBJet->deltaR(hyp.hadronicBJet);
	// 	deltaRleptonicBjet = truthMatchEvent.leptonicBjet->deltaR(hyp.leptonicBjet);

	// 	histMan_->H1D("deltaPtElectron")->Fill(
	// 			(truthMatchEvent.leptonFromW->pt() - hyp.leptonFromW->pt()) / truthMatchEvent.leptonFromW->pt());
	// 	histMan_->H1D("deltaRelectron")->Fill(truthMatchEvent.leptonFromW->deltaR(hyp.leptonFromW));
	// 	histMan_->H1D("deltaPtHadronicBjet")->Fill(
	// 			(truthMatchEvent.hadronicBJet->pt() - hyp.hadronicBJet->pt()) / truthMatchEvent.hadronicBJet->pt());
	// 	histMan_->H1D("deltaRhadronicBjet")->Fill(truthMatchEvent.hadronicBJet->deltaR(hyp.hadronicBJet));
	// 	histMan_->H1D("deltaPtLeptonicBjet")->Fill(
	// 			(truthMatchEvent.leptonicBjet->pt() - hyp.leptonicBjet->pt()) / truthMatchEvent.leptonicBjet->pt());
	// 	histMan_->H1D("deltaRleptonicBjet")->Fill(truthMatchEvent.leptonicBjet->deltaR(hyp.leptonicBjet));
	// 	if (truthMatchEvent.jet1FromW->deltaR(hyp.jet1FromW) < truthMatchEvent.jet1FromW->deltaR(hyp.jet2FromW)) {
	// 		deltaRWjet1 = truthMatchEvent.jet1FromW->deltaR(hyp.jet1FromW);
	// 		deltaRWjet2 = truthMatchEvent.jet2FromW->deltaR(hyp.jet2FromW);
	// 		histMan_->H1D("deltaPtWjet1")->Fill(
	// 				(truthMatchEvent.jet1FromW->pt() - hyp.jet1FromW->pt()) / truthMatchEvent.jet1FromW->pt());
	// 		histMan_->H1D("deltaRWjet1")->Fill(truthMatchEvent.jet1FromW->deltaR(hyp.jet1FromW));
	// 		histMan_->H1D("deltaPtWjet2")->Fill(
	// 				(truthMatchEvent.jet2FromW->pt() - hyp.jet2FromW->pt()) / truthMatchEvent.jet2FromW->pt());
	// 		histMan_->H1D("deltaRWjet2")->Fill(truthMatchEvent.jet2FromW->deltaR(hyp.jet2FromW));
	// 	} else {
	// 		deltaRWjet1 = truthMatchEvent.jet1FromW->deltaR(hyp.jet2FromW);
	// 		deltaRWjet2 = truthMatchEvent.jet2FromW->deltaR(hyp.jet1FromW);
	// 		histMan_->H1D("deltaPtWjet1")->Fill(
	// 				(truthMatchEvent.jet1FromW->pt() - hyp.jet2FromW->pt()) / truthMatchEvent.jet1FromW->pt());
	// 		histMan_->H1D("deltaRWjet1")->Fill(truthMatchEvent.jet1FromW->deltaR(hyp.jet2FromW));
	// 		histMan_->H1D("deltaPtWjet2")->Fill(
	// 				(truthMatchEvent.jet2FromW->pt() - hyp.jet1FromW->pt()) / truthMatchEvent.jet2FromW->pt());
	// 		histMan_->H1D("deltaRWjet2")->Fill(truthMatchEvent.jet2FromW->deltaR(hyp.jet1FromW));
	// 	}

	// 	sumDeltaR = deltaRelectron + deltaRhadronicBjet + deltaRleptonicBjet + deltaRWjet1 + deltaRWjet2;
	// 	histMan_->H1D("SumDeltaR")->Fill(sumDeltaR);

	// 	histMan_->H1D("deltaLeptonicTopMass")->Fill(
	// 			(truthMatchEvent.leptonicTop->mass() - leptonicTop->mass()) / truthMatchEvent.leptonicTop->mass());
	// 	histMan_->H1D("deltaHadronicTopMass")->Fill(
	// 			(truthMatchEvent.hadronicTop->mass() - hadronicTop->mass()) / truthMatchEvent.hadronicTop->mass());

	// 	if (sumDeltaR < 0.4) {
	// 		if (truthMatchEvent.jet1FromW->deltaR(hyp.jet1FromW) < truthMatchEvent.jet1FromW->deltaR(hyp.jet2FromW)) {
	// 			histMan_->H1D("deltaPtWjet1Best")->Fill(
	// 					(truthMatchEvent.jet1FromW->pt() - hyp.jet1FromW->pt()) / truthMatchEvent.jet1FromW->pt());
	// 			histMan_->H1D("deltaPtWjet2Best")->Fill(
	// 					(truthMatchEvent.jet2FromW->pt() - hyp.jet2FromW->pt()) / truthMatchEvent.jet2FromW->pt());
	// 		} else {
	// 			histMan_->H1D("deltaPtWjet1")->Fill(
	// 					(truthMatchEvent.jet1FromW->pt() - hyp.jet2FromW->pt()) / truthMatchEvent.jet1FromW->pt());
	// 			histMan_->H1D("deltaPtWjet2")->Fill(
	// 					(truthMatchEvent.jet2FromW->pt() - hyp.jet1FromW->pt()) / truthMatchEvent.jet2FromW->pt());
	// 		}
	// 		histMan_->H1D("deltaPtElectronBest")->Fill(
	// 				(truthMatchEvent.leptonFromW->pt() - hyp.leptonFromW->pt()) / truthMatchEvent.leptonFromW->pt());
	// 		histMan_->H1D("deltaPtHadronicBjetBest")->Fill(
	// 				(truthMatchEvent.hadronicBJet->pt() - hyp.hadronicBJet->pt()) / truthMatchEvent.hadronicBJet->pt());
	// 		histMan_->H1D("deltaPtLeptonicBjetBest")->Fill(
	// 				(truthMatchEvent.leptonicBjet->pt() - hyp.leptonicBjet->pt()) / truthMatchEvent.leptonicBjet->pt());
	// 		histMan_->H1D("deltaLeptonicTopMassBest")->Fill(
	// 				(truthMatchEvent.leptonicTop->mass() - leptonicTop->mass()) / truthMatchEvent.leptonicTop->mass());
	// 		histMan_->H1D("deltaHadronicTopMassBest")->Fill(
	// 				(truthMatchEvent.hadronicTop->mass() - hadronicTop->mass()) / truthMatchEvent.hadronicTop->mass());
	// 	}
	}

	return hyp;
}

int HitFitAnalyser::positionOfLastTTBarJet(const JetCollection jets) {
	// Loop over jets and find position of last jet that comes from ttbar decay
	bool foundHadB = false;
	bool foundLepB = false;
	bool foundQ = false;
	bool foundQBar = false;
	for ( unsigned int jetIndex=0; jetIndex < jets.size(); ++jetIndex ) {
		JetPointer jet = jets[jetIndex];

		if ( jet->ttbar_decay_parton() == 3 ) foundQ = true;
		else if ( jet->ttbar_decay_parton() == 4 ) foundQBar = true;
		else if ( jet->ttbar_decay_parton() == 5 ) foundLepB = true;
		else if ( jet->ttbar_decay_parton() == 6 ) foundHadB = true;

		if ( foundQ && foundQBar && foundLepB && foundHadB ) return jetIndex;
	}
	return -1;

}

FourVector HitFitAnalyser::fourVectorFromHitFit(const hitfit::Fourvec& v) {
	FourVector result(v.x(), v.y(), v.z(), v.t());
	return result;
}

void HitFitAnalyser::setMCTTbarHypothesis(const TtbarHypothesis& mcEvent) {
	cout << "Setting truth event" << endl;
	truthMatchEvent = mcEvent;
}

HitFitAnalyser::~HitFitAnalyser() {
}

void HitFitAnalyser::createHistograms() {
	// histMan_->setCurrentHistogramFolder(histogramFolder_);
	// histMan_->addH1D("AllJetsPt", "All jets Pt", 100, 0., 600.);
	// histMan_->addH1D("FittedTopMassAllSolutions", "Fitted top mass all solutions", 100, 0., 400.);
	// histMan_->addH1D("FittedTopPtAllSolutions", "Fitted top pt all solutions", 100, 0., 400.);

	// histMan_->addH1D("FittedTTbarMassAllSolutions", "Fitted ttbar mass all solutions", 100, 0., 1600.);
	// histMan_->addH1D("FittedTTbarPtAllSolutions", "Fitted ttbar pt all solutions", 100, 0., 1600.);


	// histMan_->addH1D("FitChiSquaredAllSolutions", "Fit chi-squared all solutions", 100, 0., 20.);
	// histMan_->addH1D("FitLogChiSqdAllSolutions", "Fit log(chi-sqd) all solutions", 100, -5., 5.);
	// histMan_->addH1D("FittedTopMassBestSolution", "Fitted top mass best solution", 100, 0., 400.);
	// histMan_->addH1D("FittedTopPtBestSolution", "Fitted top pt best solution", 20, 0., 400.);
	// histMan_->addH1D("FittedTopRapidityBestSolution", "Fitted top pt best solution", 50, -2.5, 2.5);

	// histMan_->addH1D("FittedTTbarMassBestSolution", "Fitted ttbar mass best solution", 80, 0., 1600.);
	// histMan_->addH1D("FittedTTbarPtBestSolution", "Fitted ttbar pt best solution", 15, 0., 300.);
	// histMan_->addH1D("FittedTTbarRapidityBestSolution", "Fitted ttbar pt best solution", 50, -2.5, 2.5);

	// histMan_->addH1D("FitChiSquaredBestSolution", "Fit chi-squared best solution", 100, 0., 20.);
	// histMan_->addH1D("FitLogChiSqdBestSolution", "Fit log(chi-sqd) best solutions", 100, -5., 5.);

	// histMan_->addH1D("PullDistAllVarsAllSolutions", "Pulls well measured all solutions", 100, -10., 10.);
	// histMan_->addH1D("PullSumSquaredAllSolutions", "Sum-squared pulls all solutions", 100, 0, 500.);
	// histMan_->addH2D("PullDistPerVarAllSolutions", "Pulls well measured vs varno all", 100, -10., 10., 25, 0., 25.);
	// histMan_->addH1D("PullDistYVarsAllSolutions", "Pulls poorly measured all", 100, -10., 10.);
	// histMan_->addH1D("PullDistAllVarsBestSolution", "Pulls well measured best solution", 100, -10., 10.);
	// histMan_->addH1D("PullSumSquaredBestSolution", "Sum-squared pulls best solution", 100, 0, 500.);
	// histMan_->addH2D("PullDistPerVarBestSolution", "Pulls well measured vs varno best", 100, -10., 10., 25, 0., 25.);
	// histMan_->addH1D("PullDistYVarsBestSolution", "Pulls poorly measured best", 100, -10., 10.);

	// // MC matching study histograms
	// histMan_->addH1D("deltaPtElectron", "Pt difference between truth and fitted electrons", 100, -1.5, 1.5);
	// histMan_->addH1D("deltaRelectron", "DeltaR between truth and fitted electrons", 100, 0., 2.);
	// histMan_->addH1D("deltaPtHadronicBjet", "Pt difference between truth and fitted b-jets from hadronic top", 100,
	// 		-1.5, 1.5);
	// histMan_->addH1D("deltaRhadronicBjet", "DeltaR between truth and fitted b-jets from hadronic top", 100, 0., 5.);
	// histMan_->addH1D("deltaPtLeptonicBjet", "Pt difference between truth and fitted b-jets from leptonic top", 100,
	// 		-1.5, 1.5);
	// histMan_->addH1D("deltaRleptonicBjet", "DeltaR between truth and fitted b-jets from leptonic top", 100, 0., 5.);
	// histMan_->addH1D("deltaPtWjet1", "Pt difference between truth and fitted W jets 1", 100, -1.5, 1.5);
	// histMan_->addH1D("deltaRWjet1", "DeltaR between truth and fitted W jets 1", 100, 0., 5.);
	// histMan_->addH1D("deltaPtWjet2", "Pt difference between truth and fitted W jets 2", 100, -1.5, 1.5);
	// histMan_->addH1D("deltaRWjet2", "DeltaR between truth and fitted W jets 2", 100, 0., 5.);
	// histMan_->addH1D("SumDeltaR", "Summarised deltaR of all e+jets", 100, 0., 25.);
	// histMan_->addH1D("deltaPtElectronBest", "Pt difference between truth and fitted electrons for matched solutions",
	// 		100, -1.5, 1.5);
	// histMan_->addH1D("deltaPtHadronicBjetBest",
	// 		"Pt difference between truth and fitted b-jets from hadronic top for matched solutions", 100, -1.5, 1.5);
	// histMan_->addH1D("deltaPtLeptonicBjetBest",
	// 		"Pt difference between truth and fitted b-jets from leptonic top for matched solutions", 100, -1.5, 1.5);
	// histMan_->addH1D("deltaPtWjet1Best", "Pt difference between truth and fitted W jets 1 for matched solutions", 100,
	// 		-1.5, 1.5);
	// histMan_->addH1D("deltaPtWjet2Best", "Pt difference between truth and fitted W jets 2 for matched solutions", 100,
	// 		-1.5, 1.5);
	// histMan_->addH1D("deltaMET", "Delta MET between truth and fitted solutions", 100, -50., 50.);
	// histMan_->addH1D("deltaMETbest", "Delta MET between truth and fitted matched solutions", 100, -50., 50.);
	// histMan_->addH1D("deltaLeptonicTopMass", "Difference between truth and fitted leptonic top masses", 100, -1.5, 1.5);
	// histMan_->addH1D("deltaHadronicTopMass", "Difference between truth and fitted hadronic top masses", 100, -1.5, 1.5);
	// histMan_->addH1D("deltaLeptonicTopMassBest",
	// 		"Difference between truth and fitted leptonic top masses for matched solutions", 100, -1.5, 1.5);
	// histMan_->addH1D("deltaHadronicTopMassBest",
	// 		"Difference between truth and fitted hadronic top masses for matched solutions", 100, -1.5, 1.5);

}

void HitFitAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->addBranch("FitChiSquaredAllSolutions", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FitChiSquaredBestSolutions", "F", "HitFit" + Globals::treePrefix_);

	treeMan_->addBranch("FittedLeptonicTopPtBestSolution", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedHadronicTopPtBestSolution", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedLeptonicTopRapidityBestSolution", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedHadronicTopRapidityBestSolution", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedTTbarMassBestSolution", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedTTbarPtBestSolution", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedTTbarRapidityBestSolution", "F", "HitFit" + Globals::treePrefix_);

	treeMan_->addBranch("SolutionCategory", "F", "HitFit" + Globals::treePrefix_);

	treeMan_->addBranch("PositionOfLastTTbarJet", "F", "HitFit" + Globals::treePrefix_);
}

