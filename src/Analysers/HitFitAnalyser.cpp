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
	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->Fill("EventWeight", weight_ );

	// const JetCollection jets(event->getCleanedJets( SelectionCriteria::ElectronPlusJetsReference ));
	// const JetCollection bJets(event->getCleanedBJets( SelectionCriteria::ElectronPlusJetsReference ));
	// const LeptonPointer selectedLepton = event->getSignalLepton( SelectionCriteria::ElectronPlusJetsReference );

	string metPrefix = METAlgorithm::names.at(0);
	const METPointer met(event->MET((METAlgorithm::value) 0));

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

	//set MC matching flag
	if (event->getDataType() == DataType::TTJets_amcatnloFXFX)
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
		int lastTTBarJetPosition = positionOfLastTTBarJet( jets );
		if ( lastTTBarJetPosition != -1 ) {
			allTTBarJetsPassSelection_ = true;
			treeMan_->Fill("PositionOfLastTTbarJet", lastTTBarJetPosition + 1 );
		}
		else treeMan_->Fill("PositionOfLastTTbarJet", lastTTBarJetPosition );
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
		treeMan_->Fill("FitChiSquaredBestSolutions",hitfitResult[bestX2pos].chisq());
		treeMan_->Fill("FitChiSquaredProbabilityBestSolutions",TMath::Prob(hitfitResult[bestX2pos].chisq(),1));

		//pass hitfit event into BAT format
		lepton_charge = selectedLepton->charge();
		BAT::TtbarHypothesis newHyp = BatEvent(hitfitResult[bestX2pos].ev(), event, "SolutionCategory");

		treeMan_->Fill("FittedLeptonicTopPtBestSolution", newHyp.leptonicTop->pt());
		treeMan_->Fill("FittedHadronicTopPtBestSolution", newHyp.hadronicTop->pt());
		treeMan_->Fill("FittedLeptonicTopRapidityBestSolution", newHyp.leptonicTop->rapidity());
		treeMan_->Fill("FittedHadronicTopRapidityBestSolution", newHyp.hadronicTop->rapidity());
		treeMan_->Fill("FittedTTbarMassBestSolution", newHyp.resonance->mass());
		treeMan_->Fill("FittedTTbarPtBestSolution", newHyp.resonance->pt());
		treeMan_->Fill("FittedTTbarRapidityBestSolution", newHyp.resonance->rapidity());

		// Get the event before the fit
		hitfit::Lepjets_Event unfittedEvent = hitfitEventsInput[bestX2pos];

		// Now need to perform second kinematic fit
		hitfit::Fit_Result secondFitResult = performSecondKinematicFit( unfittedEvent, event);
		BAT::TtbarHypothesis newHyp_afterSecondFit = BatEvent(secondFitResult.ev(), event, "SolutionCategory_second");

		treeMan_->Fill("FitChiSquaredBestSolutions_second",secondFitResult.chisq());
		treeMan_->Fill("FitChiSquaredProbabilityBestSolutions_second",TMath::Prob(secondFitResult.chisq(),2));

		treeMan_->Fill("FittedLeptonicTopPtBestSolution_second", newHyp_afterSecondFit.leptonicTop->pt());
		treeMan_->Fill("FittedHadronicTopPtBestSolution_second", newHyp_afterSecondFit.hadronicTop->pt());
		treeMan_->Fill("FittedLeptonicTopRapidityBestSolution_second", newHyp_afterSecondFit.leptonicTop->rapidity());
		treeMan_->Fill("FittedHadronicTopRapidityBestSolution_second", newHyp_afterSecondFit.hadronicTop->rapidity());
		treeMan_->Fill("FittedTTbarMassBestSolution_second", newHyp_afterSecondFit.resonance->mass());
		treeMan_->Fill("FittedTTbarPtBestSolution_second", newHyp_afterSecondFit.resonance->pt());
		treeMan_->Fill("FittedTTbarRapidityBestSolution_second", newHyp_afterSecondFit.resonance->rapidity());

		return newHyp_afterSecondFit;

	} else {
		// cout << "No HitFit solution found for this event" << endl;
		treeMan_->Fill("SolutionCategory", 0 );
		treeMan_->Fill("SolutionCategory_second", 0);
	}

	return BAT::TtbarHypothesis();

}

HitFitAnalyser::HitFitAnalyser(TreeManagerPtr treeMan, const bool isElectronChannel, std::string histogramFolder ) :
		BasicAnalyser(treeMan, histogramFolder), //
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
		allTTBarJetsPassSelection_(false),
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

BAT::TtbarHypothesis HitFitAnalyser::BatEvent(const hitfit::Lepjets_Event& ev, const EventPtr event, const string SolutionCategoryHist ) {
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

	// Loop over jets in event
	for ( size_t fit_i = 0; fit_i < ev.njets(); ++fit_i ) {
		FourVector jetFV = fourVectorFromHitFit(ev.jet(fit_i).p());
		int jetType = ev.jet(fit_i).type();

		if ( jetType == hitfit::lepb_label ) {
			newLepB->setFourVector( jetFV );
		}
		else if ( jetType == hitfit::hadb_label ) {
			newHadB->setFourVector( jetFV );
		}
		else if ( jetType == hitfit::hadw1_label ) {
			newWj1->setFourVector( jetFV );
		}
		else if ( jetType == hitfit::hadw2_label ) {
			newWj2->setFourVector( jetFV );
		}
	}

	BAT::TtbarHypothesis hyp(newLepton, newMet, newLepB, newHadB, newWj1, newWj2);
	hyp.combineReconstructedObjects();

	// do MC matching study
	if (do_MC_matching && SolutionCategoryHist != "") {


		// Check if jets are in correct position
		if ( ( newWj1->ttbar_decay_parton() == 3 || newWj1->ttbar_decay_parton() == 4 ) &&
			 ( newWj2->ttbar_decay_parton() == 3 || newWj2->ttbar_decay_parton() == 4 ) && 
			 ( newLepB->ttbar_decay_parton() == 5 ) &&
			 ( newHadB->ttbar_decay_parton() == 6 ) ) {
			// Correct
			treeMan_->Fill(SolutionCategoryHist, 1 );
		}
		else if ( !( event->isSemiLeptonicElectron() || event->isSemiLeptonicMuon() ) ) {
			// Not a genuine semi leptonic event
			treeMan_->Fill(SolutionCategoryHist, 2 );
		}
		else if ( !allTTBarJetsPassSelection_ ) {
			// Not all jets from ttbar passed jet selection
			treeMan_->Fill(SolutionCategoryHist, 3 );			
		}
		else if ( !allTTBarJetsPassedToFit_ ) {
			// Not all jets from ttbar were passed to fit
			treeMan_->Fill(SolutionCategoryHist, 4 );
		}
		else if ( allTTBarJetsPassedToFit_ && ( newWj1->ttbar_decay_parton() == 0 || newWj2->ttbar_decay_parton() == 0 || newLepB->ttbar_decay_parton() == 0 || newHadB->ttbar_decay_parton() != 0 ) ) {
			// All ttbar jets were available, but at least one incorrect jet used
			treeMan_->Fill(SolutionCategoryHist, 5 );
		}
		else if ( ( newWj1->ttbar_decay_parton() == 3 || newWj1->ttbar_decay_parton() == 4 ) &&
			 ( newWj2->ttbar_decay_parton() == 3 || newWj2->ttbar_decay_parton() == 4 ) && 
			 ( newLepB->ttbar_decay_parton() == 6 ) &&
			 ( newHadB->ttbar_decay_parton() == 5 ) ) {
			// B jets swapped, but W's correct
			treeMan_->Fill(SolutionCategoryHist, 6 );
		}
		else if ( ( ( newWj1->ttbar_decay_parton() == 5 || newWj1->ttbar_decay_parton() == 6 ) ||
			 ( newWj2->ttbar_decay_parton() == 5 || newWj2->ttbar_decay_parton() == 6 ) ) || 
			 ( ( newLepB->ttbar_decay_parton() == 3 || newLepB->ttbar_decay_parton() == 4 ) ||
			 ( newHadB->ttbar_decay_parton() == 3 || newHadB->ttbar_decay_parton() == 4 ) ) ) {
			// Light jet from W assigned as one of b's
			// Or B jet from top assigend as light jet
			treeMan_->Fill(SolutionCategoryHist, 7 );
		}
		else {
			// Just plain wrong
			treeMan_->Fill(SolutionCategoryHist, 8 );
		}
	}
	return hyp;
}


hitfit::Fit_Result HitFitAnalyser::performSecondKinematicFit(const hitfit::Lepjets_Event& unfittedEvent, const EventPtr event ) {

	// Same as first, but without top mass constraint
	// However recosntructed tops still constrained to have same mass
	BatHitFit hhFitter(electronTranslator_, muonTranslator_, jetTranslator_, metTranslator_, hitfitDefault_,
				hitfitLepWMass_, hitfitHadWMass_, 0);

	// Clear the internal state
	hhFitter.clear();

	BAT::TtbarHypothesis unfittedBatEvent = BatEvent(unfittedEvent, event, "");


	// Add the lepton to the fit
	if ( isElectronChannel_ ) {
		BAT::ElectronPointer newLepton(new BAT::Electron());
		newLepton->setFourVector(unfittedBatEvent.leptonFromW->getFourVector());
		hhFitter.AddLepton( *newLepton );
	}
	else {
		BAT::MuonPointer newLepton(new BAT::Muon());
		newLepton->setFourVector(unfittedBatEvent.leptonFromW->getFourVector());
		hhFitter.AddLepton( *newLepton );
	}

	// Add the MET to the fit
	// Add missing transverse energy into HitFit
	hhFitter.SetMet(*unfittedBatEvent.met);

	// Add jets to the fit
	hhFitter.AddJet( *unfittedBatEvent.jet1FromW );
	hhFitter.AddJet( *unfittedBatEvent.jet2FromW );

	// Add b jets to the fit
	hhFitter.AddBJet( *unfittedBatEvent.leptonicBjet );
	hhFitter.AddBJet( *unfittedBatEvent.hadronicBJet );

	// Perform the fit
	size_t nHitFit = hhFitter.FitAllPermutation();

	// Get fit results
	std::vector<hitfit::Fit_Result> hitfitResult = hhFitter.GetFitAllPermutation();

	unsigned int bestChi2 = 9999;
	unsigned int bestSolution = 0;
	// Loop over solutions, and get the permutation that matches the intended permutation
	for (size_t fit = 0; fit != nHitFit; ++fit) {
		// Get the event after the fit
		hitfit::Fit_Result fitResult = hitfitResult[fit];
		if ( fitResult.ev().jet_permutation() == "WWbB" ) {
			double chi2 = fitResult.chisq();
			if ( chi2 < bestChi2 ) {
				bestChi2 = chi2;
				bestSolution = fit;
			}
		}
	}

	hitfit::Fit_Result bestFitResult = hitfitResult[bestSolution];

	return bestFitResult;


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

void HitFitAnalyser::createTrees() {
	treeMan_->setCurrentFolder(histogramFolder_);
	treeMan_->addBranch("FitChiSquaredAllSolutions", "F", "HitFit" + Globals::treePrefix_, false);
	treeMan_->addBranch("FitChiSquaredBestSolutions", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FitChiSquaredProbabilityBestSolutions", "F", "HitFit" + Globals::treePrefix_);

	treeMan_->addBranch("FitChiSquaredBestSolutions_second", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FitChiSquaredProbabilityBestSolutions_second", "F", "HitFit" + Globals::treePrefix_);

	treeMan_->addBranch("FittedLeptonicTopPtBestSolution", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedHadronicTopPtBestSolution", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedLeptonicTopRapidityBestSolution", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedHadronicTopRapidityBestSolution", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedTTbarMassBestSolution", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedTTbarPtBestSolution", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedTTbarRapidityBestSolution", "F", "HitFit" + Globals::treePrefix_);

	treeMan_->addBranch("FittedLeptonicTopPtBestSolution_second", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedHadronicTopPtBestSolution_second", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedLeptonicTopRapidityBestSolution_second", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedHadronicTopRapidityBestSolution_second", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedTTbarMassBestSolution_second", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedTTbarPtBestSolution_second", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("FittedTTbarRapidityBestSolution_second", "F", "HitFit" + Globals::treePrefix_);

	treeMan_->addBranch("SolutionCategory", "F", "HitFit" + Globals::treePrefix_);
	treeMan_->addBranch("SolutionCategory_second", "F", "HitFit" + Globals::treePrefix_);

	treeMan_->addBranch("PositionOfLastTTbarJet", "F", "HitFit" + Globals::treePrefix_);
}

