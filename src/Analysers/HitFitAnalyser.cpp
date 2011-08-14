/*
 * HitFitAnalyser.cpp
 *
 *  Created on: Jul 5, 2011
 *      Author: phzss
 */
#include "../../interface/Analysers/HitFitAnalyser.h"

namespace BAT {

void HitFitAnalyser::analyse(const TopPairEventCandidate& ttbarEvent) {
    histMan->setCurrentCollection("hitfitStudy");
  	//prepare the jets collection
	// Copy jets into an array
	JetCollection jetCopy;
	for (JetCollection::const_iterator j=ttbarEvent.Jets().begin();
			j!=ttbarEvent.Jets().end(); ++j) {
	    jetCopy.push_back(*j);
	    //if ((*j)->getBTagDiscriminators().back() > 0.5) ++nBTags;
	}

	float massCut = 0.;
	float ptCut = 30.;
	float etaCut = 2.5;
	// Merge pairs of jets whose invariant mass is less than the cut
 	bool mergeJets = (massCut > 0.0);
 	while (mergeJets) {
 	  JetCollection::iterator j1=jetCopy.end();
 	  JetCollection::iterator j2=jetCopy.end();
 	  float lowMass=9999.;
 	  for (JetCollection::iterator j=jetCopy.begin(); j!=jetCopy.end(); ++j) {
 	      for (JetCollection::iterator k=j+1; k!=jetCopy.end(); ++k) {
 	    	  float m=(*j)->invariantMass(*k);
 	    	  if (m<lowMass) {
 	    		  j1 = j;
 	    		  j2 = k;
 	    		  lowMass = m;
 	    	  }
 	      }
 	  }
 	  // Merge the lowest mass pair of jets if its mass is small enough,
 	  // otherwise finish merging
 	  if (lowMass < massCut) {
 		  cout << "Number of jets " << jetCopy.size() << " lowest mass combination is " << lowMass
 	 	   << " between jets with pt " << (*j1)->pt() << " and " << (*j2)->pt() << endl;
 	      JetPointer newp(new Jet((**j1) + (**j2)));
 	      j1->swap(newp);
 	      jetCopy.erase(j2);
 	  } else {
 	    mergeJets = false;
 	  }
 	}

	  // Apply cuts on jets here
	for (JetCollection::iterator j=jetCopy.begin(); j != jetCopy.end(); ++j) {
		if (((*j)->pt() < ptCut) || ((etaCut > 0.0) && (abs((*j)->eta()) > etaCut))) {
	      // Remove this jet from the list
	      // Point the iterator at the previous jet so the loop works correctly
	      j = jetCopy.erase(j)-1;
	    }
	}

	std::sort(jetCopy.begin(), jetCopy.end(), jetPtComp);

	//cout << "Number of good jets in event: " << jetCopy.size() << "\n";

	jetsForFitting.clear();
	unsigned numJetsToFit = jetCopy.size();

	if (jetCopy.size()>=4) {
		if (numJetsToFit>5) numJetsToFit = 5;
		jetsForFitting.insert(jetsForFitting.begin(), jetCopy.begin(), jetCopy.begin()+numJetsToFit);
	}

  BatHitFit hhFitter(electronTranslator_,
		     muonTranslator_,
		     jetTranslator_,
		     metTranslator_,
		     hitfitDefault_,
		     hitfitLepWMass_,
		     hitfitHadWMass_,
		     hitfitTopMass_);

  // Clear the internal state
  hhFitter.clear();

  // Add the lepton into HitFit
  hhFitter.AddLepton(*ttbarEvent.getElectronFromWDecay());

  // Add jets into HitFit
  for (size_t jet = 0 ; jet != jetsForFitting.size() ; ++jet) {
    hhFitter.AddJet(*jetsForFitting.at(jet));
  }

//  size_t nJets = jetsForFitting.size();
//  if (nJets>0) {
//    std::vector<size_t> inv(nJets, 0);
//    std::vector<size_t> pos(nJets, 0);
//    for (size_t j1 = 0 ; j1<(nJets-1); ++j1) {
//      for (size_t j2 = (j1+1) ; j2<nJets; ++j2) {
//        if (truthMatchJetTypes.at(j1)<truthMatchJetTypes.at(j2)) {
//          inv.at(j2) = inv.at(j2) + 1;
//        } else {
//          inv.at(j1) = inv.at(j1) + 1;
//        }
//      }
//    }
//    for (size_t j = 0 ; j < nJets ; ++j) {
//      size_t k = inv.at(j);
//      pos.at(k) = j;
//    }
//    for (size_t jet = 0 ; jet != jetsForFitting.size() ; ++jet) {
//      size_t thisPos = pos.at(jet);
//      std::cout << "Adding jet of type " << truthMatchJetTypes.at(thisPos) << std::endl;
//      hhFitter.AddJet(*jetsForFitting.at(thisPos));
//    }
//  } else { std::cout << "No jets to add to HitFit for this event" << std::endl; }


  // Add missing transverse energy into HitFit
  hhFitter.SetMet(*ttbarEvent.MET());

  // Results of the fit for all jet permutation of the event
  std::vector<hitfit::Lepjets_Event>  hitfitEventsInput;

  // Results of the fit for all jet permutation of the event
  std::vector<hitfit::Fit_Result>     hitfitResult;
  //
  // R U N   H I T F I T
  //
  // Run the kinematic fit and get how many permutations is possible
  // in the fit

  size_t nHitFit         = hhFitter.FitAllPermutation();

//   // Get the number of jets
//   nHitFitJet      = hhFitter.GetEvent().njets();

  // Get the input events for all permutations
  hitfitEventsInput    = hhFitter.GetUnfittedEvent();

  // Get the fit results for all permutations
  hitfitResult    = hhFitter.GetFitAllPermutation();

  double bestChi2 = 999.;
  unsigned bestX2pos = nHitFit+1;
//  unsigned tMatchpos = nHitFit;
  //cout << "nHitFit = " << nHitFit << "\n";

//   // Loop over all permutation and extract the information
  for (size_t fit = 0 ; fit != nHitFit ; ++fit) {

    // Get the event before the fit
    hitfit::Lepjets_Event unfittedEvent  = hitfitEventsInput[fit];

    // Get the event after the fit
    hitfit::Fit_Result fitResult         = hitfitResult[fit];

    if (hitfitResult[fit].chisq()>0.0) {
      histMan->H1D("FittedTopMassAllSolutions")->Fill(fitResult.mt());
      histMan->H1D("FitChiSquaredAllSolutions")->Fill(fitResult.chisq());
      histMan->H1D("FitLogChiSqdAllSolutions")->Fill(log(fitResult.chisq()));
      const hitfit::Column_Vector &px = fitResult.pullx();
      const hitfit::Column_Vector &py = fitResult.pully();
      double sumPx = 0.0;
      for (int i=0; i<px.num_row(); ++i) {
        histMan->H1D("PullDistAllVarsAllSolutions")->Fill(px[i]);
        histMan->H2D("PullDistPerVarAllSolutions")->Fill(px[i],i);
        sumPx += px[i]*px[i];
      }
      histMan->H1D("PullSumSquaredAllSolutions")->Fill(sumPx);
      for (int i=0; i<py.num_row(); ++i) {
        histMan->H1D("PullDistYVarsAllSolutions")->Fill(py[i]);
      }

//      const hitfit::Lepjets_Event &hfev = fitResult.ev();
//
//      bool allJetsMatch = true;
//      for (unsigned i=0; i<hfev.njets(); ++i) {
//        bool Match = (hfev.jet(i).type() == truthMatchJetTypes.at(i));
//        Match |= ((hfev.jet(i).type() == hitfit::hadw1_label) && (truthMatchJetTypes.at(i) == hitfit::hadw2_label));
//        Match |= ((hfev.jet(i).type() == hitfit::hadw2_label) && (truthMatchJetTypes.at(i) == hitfit::hadw1_label));
//        allJetsMatch &= Match;
//      }
//    }
//    if (allJetsMatch) {
//      if (tMatchpos<nHitFit) {
//        // We've already found a solution where the jets match the truth solution
//        // See if this one gives a better chi-squared
//        if (fitResult.chisq()<hitfitResult[tMatchpos].chisq()) {
//          tMatchpos = fit;
//        }
//      } else {
//        // This is the first solution where the jets match the truth
//       tMatchpos = fit;
//      }
//    }

    }
    if (fitResult.chisq()>0.0 && fitResult.chisq()<bestChi2) {
      bestChi2 = fitResult.chisq();
      bestX2pos = fit;
    }

  }

  //
  // END PART WHICH EXTRACT INFORMATION FROM HITFIT
  //

  if (bestX2pos < nHitFit+1) {
    histMan->H1D("FittedTopMassBestSolution")->Fill(hitfitResult[bestX2pos].mt());
    histMan->H1D("FitChiSquaredBestSolution")->Fill(hitfitResult[bestX2pos].chisq());
    histMan->H1D("FitLogChiSqdBestSolution")->Fill(log(hitfitResult[bestX2pos].chisq()));

    const hitfit::Column_Vector &px = hitfitResult[bestX2pos].pullx();
    const hitfit::Column_Vector &py = hitfitResult[bestX2pos].pully();
    double sumPx = 0.0;
    for (int i=0; i<px.num_row(); ++i) {
      histMan->H1D("PullDistAllVarsBestSolution")->Fill(px[i]);
      histMan->H2D("PullDistPerVarBestSolution")->Fill(px[i],i);
      sumPx += px[i]*px[i];
    }
    histMan->H1D("PullSumSquaredBestSolution")->Fill(sumPx);
    for (int i=0; i<py.num_row(); ++i) {
      histMan->H1D("PullDistYVarsBestSolution")->Fill(py[i]);
    }
//
//    const hitfit::Lepjets_Event &hfev = hitfitResult[bestX2pos].ev();
//    bool allMatch = true;
//    unsigned nMatch = 0;
//    for (unsigned i=0; i<hfev.njets(); ++i) {
//      bool Match = (hfev.jet(i).type() == truthMatchJetTypes.at(i));
//      Match |= ((hfev.jet(i).type() == hitfit::hadw1_label) && (truthMatchJetTypes.at(i) == hitfit::hadw2_label));
//      Match |= ((hfev.jet(i).type() == hitfit::hadw2_label) && (truthMatchJetTypes.at(i) == hitfit::hadw1_label));
//      allMatch &= Match;
//      if (Match) ++nMatch;
//    }


    // TESTTEST
    BAT::TtbarHypothesis newHyp = BatEvent(hitfitResult[bestX2pos].ev());
  } else {
    std::cout << "No solutions found by HitFit for this event" << std::endl;
  }

//  if (tMatchpos < nHitFit) {
//    histMan->H1D("FittedTopMassTruthMatch")->Fill(hitfitResult[tMatchpos].mt());
//    histMan->H1D("FitChiSquaredTruthMatch")->Fill(hitfitResult[tMatchpos].chisq());
//    histMan->H1D("FitLogChiSqdTruthMatch")->Fill(log(hitfitResult[tMatchpos].chisq()));
//    const hitfit::Column_Vector &px = hitfitResult[tMatchpos].pullx();
//    const hitfit::Column_Vector &py = hitfitResult[tMatchpos].pully();
//    double sumPx = 0.0;
//    for (int i=0; i<px.num_row(); ++i) {
//      histMan->H1D("PullDistAllVarsTruthMatch")->Fill(px[i]);
//      histMan->H2D("PullDistPerVarTruthMatch")->Fill(px[i],i);
//      sumPx += px[i]*px[i];
//    }
//    histMan->H1D("PullSumSquaredTruthMatch")->Fill(sumPx);
//    for (int i=0; i<py.num_row(); ++i) {
//      histMan->H1D("PullDistYVarsTruthMatch")->Fill(py[i]);
//    }
//
//    if (bestX2pos < nHitFit) {
//      histMan->H2D("TopMassBestVsTruth")   -> Fill(hitfitResult[tMatchpos].mt(),hitfitResult[bestX2pos].mt());
//      histMan->H2D("LogChiSqdBestVsTruth") -> Fill(log(hitfitResult[tMatchpos].chisq()),log(hitfitResult[bestX2pos].chisq()));
//    }
//  } else {
//    std::cout << "No truth match solution found by HitFit for this event" << std::endl;
//  }


}

HitFitAnalyser::HitFitAnalyser(boost::shared_ptr<HistogramManager> histMan) :
    BasicAnalyser(histMan),
    // The following five initializers read the config parameters for the
    // ASCII text files which contains the physics object resolutions.
    CMSSWBase_               (""),
    hitfitDefault_           ("TopQuarkAnalysis/HitFit/data/defaults"),
    hitfitElectronResolution_("TopQuarkAnalysis/PatHitFit/data/exampleElectronResolution.txt"),
    hitfitMuonResolution_    ("TopQuarkAnalysis/PatHitFit/data/exampleMuonResolution.txt"),
    hitfitJetResolution_     ("TopQuarkAnalysis/PatHitFit/data/exampleJetResolution.txt"),
    hitfitMETResolution_     ("TopQuarkAnalysis/PatHitFit/data/exampleMETResolution.txt"),
    // The following three initializers read the config parameters for the
    // values to which the leptonic W, hadronic W, and top quark masses are to
    // be constrained to.
    hitfitLepWMass_(80.4),
    hitfitHadWMass_(80.4),
    hitfitTopMass_ ( 0.0),
    // The following three initializers instantiate the translator between PAT objects
    // and HitFit objects using the ASCII text files which contains the resolutions.
    electronTranslator_(CMSSWBase_+hitfitElectronResolution_),
    muonTranslator_    (CMSSWBase_+hitfitMuonResolution_),
  	jetTranslator_     (CMSSWBase_+hitfitJetResolution_,CMSSWBase_+hitfitJetResolution_),
   	metTranslator_     (CMSSWBase_+hitfitMETResolution_)

	{

}

BAT::TtbarHypothesis HitFitAnalyser::BatEvent(const hitfit::Lepjets_Event& ev) {

  bool evOk=true;

  // Do the electron
  //BAT::ElectronPointer newEle(new BAT::Electron(*truthMatchEvent.electronFromW));
  BAT::ElectronPointer newEle(new BAT::Electron());
  if (ev.nleps()>0) {
    newEle->setFourVector(fourVectorFromHitFit(ev.lep(0).p()));
  } else {
    evOk = false;
    std::cout << "No electron in HitFit event!" << std::endl;
  }

  // Do the neutrino
  //BAT::ParticlePointer newMet(new BAT::Particle(*truthMatchEvent.neutrinoFromW));
  BAT::ParticlePointer newMet(new BAT::Particle());
  newMet->setFourVector(fourVectorFromHitFit(ev.met()));

  // Do the jets
  BAT::JetPointer newLep(new BAT::Jet());
  BAT::JetPointer newHad(new BAT::Jet());
  BAT::JetPointer newWj1(new BAT::Jet());
  BAT::JetPointer newWj2(new BAT::Jet());

  // We need to match up the jets in the HitFit event against the input ones
  // Assume that HitFit has preserved the jet order
  std::vector<hitfit::Lepjets_Event_Jet>::size_type i = 0;
  for (BAT::JetCollection::const_iterator j = jetsForFitting.begin(); j != jetsForFitting.end(); ++i, ++j) {
    FourVector hfJet   = fourVectorFromHitFit(ev.jet(i).p());
    int        hfJType = ev.jet(i).type();
    if ((*j)->getFourVector().DeltaR(hfJet) < 0.005) {
      if (hfJType != hitfit::unknown_label) {
	BAT::Jet newJet(**j);
	newJet.setFourVector(hfJet);

	if (hfJType == hitfit::lepb_label) *newLep = newJet;
	if (hfJType == hitfit::hadb_label) *newHad = newJet;
	if (hfJType == hitfit::hadw1_label) *newWj1 = newJet;
	if (hfJType == hitfit::hadw2_label) *newWj2 = newJet;
      }
    } else {
      std::cout << "Distance to corresponding jet " << (*j)->getFourVector().DeltaR(hfJet) << " too large; not matching" << std::endl;
      if (hfJType != hitfit::unknown_label) evOk = false;
    }
  }

  if (!evOk) std::cout << "Solution is not Ok" << std::endl;

  BAT::TtbarHypothesis hyp(newEle, newMet,
			   newLep, newHad,
			   newWj1, newWj2);
  return hyp;
}

FourVector HitFitAnalyser::fourVectorFromHitFit(const hitfit::Fourvec& v) {
  FourVector result(v.x(), v.y(), v.z(), v.t());
  return result;
}

HitFitAnalyser::~HitFitAnalyser() {
}

void HitFitAnalyser::createHistograms() {
    histMan->setCurrentCollection("hitfitStudy");
    histMan->addH1D("FittedTopMassAllSolutions", "Fitted top mass all solutions",   100,  0., 400.);
    histMan->addH1D("FitChiSquaredAllSolutions", "Fit chi-squared all solutions",   100,  0., 20.);
    histMan->addH1D("FitLogChiSqdAllSolutions",  "Fit log(chi-sqd) all solutions",  100, -5., 5.);
    histMan->addH1D("FittedTopMassBestSolution", "Fitted top mass best solution",   100,  0., 400.);
    histMan->addH1D("FitChiSquaredBestSolution", "Fit chi-squared best solution",   100,  0., 20.);
    histMan->addH1D("FitLogChiSqdBestSolution",  "Fit log(chi-sqd) best solutions", 100, -5., 5.);

    histMan->addH1D("PullDistAllVarsAllSolutions", "Pulls well measured all solutions",   100, -10.,  10.);
    histMan->addH1D("PullSumSquaredAllSolutions",  "Sum-squared pulls all solutions",     100,   0,  500.);
    histMan->addH2D("PullDistPerVarAllSolutions",  "Pulls well measured vs varno all",    100, -10.,  10.,   25, 0., 25.);
    histMan->addH1D("PullDistYVarsAllSolutions",   "Pulls poorly measured all",           100, -10.,  10.);
    histMan->addH1D("PullDistAllVarsBestSolution", "Pulls well measured best solution",   100, -10.,  10.);
    histMan->addH1D("PullSumSquaredBestSolution",  "Sum-squared pulls best solution",     100,   0,  500.);
    histMan->addH2D("PullDistPerVarBestSolution",  "Pulls well measured vs varno best",   100, -10.,  10.,   25, 0., 25.);
    histMan->addH1D("PullDistYVarsBestSolution",   "Pulls poorly measured best",          100, -10.,  10.);

}

}
