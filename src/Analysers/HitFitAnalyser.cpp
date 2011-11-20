/*
 * HitFitAnalyser.cpp
 *
 *  Created on: Jul 5, 2011
 *      Author: senkin
 */
#include "../../interface/Analysers/HitFitAnalyser.h"
#include "../../interface/LumiReWeighting.h"
#include "../../interface/GlobalVariables.h"

using namespace reweight;
using namespace BAT;

void HitFitAnalyser::analyse(const TopPairEventCandidate& ttbarEvent) {
	//fit only the events that pass full ttbar selection
	if (!ttbarEvent.passesFullTTbarEPlusJetSelection()) return;

	//initialise pile-up shifters
	PoissonMeanShifter PShiftUp = PoissonMeanShifter(0.6); // PU-systematic
	PoissonMeanShifter PShiftDown = PoissonMeanShifter(-0.6); // PU-systematic

	//temporary file for jet fit info (ugly but works)
	string tempFileName = "temp.txt";
	ofstream tempFile(tempFileName.c_str());

    histMan->setCurrentCollection("hitfitStudy");

    if (Globals::produceFitterASCIIoutput) {
        //write the event info into ASCII file
    	outFile << "------------------------------------------" << endl;
    	outFile << "BeginEvent" << endl;
    	outFile << "RunNr: " << ttbarEvent.runnumber()  << endl;
    	outFile << "LumiSection: " << ttbarEvent.lumiblock() << endl;
    	outFile << "EventNr: " << ttbarEvent.eventnumber() << endl;
    	outFile << "EventWeight: " << ttbarEvent.weight() << "  ";
        if(!ttbarEvent.isRealData()){
        	float lumiWeight = ttbarEvent.PileUpWeight();
            float lumiWeightUp = lumiWeight * PShiftUp.ShiftWeight( (float) ttbarEvent.numberOfGeneratedPileUpVertices() );
            float lumiWeightDown = lumiWeight * PShiftDown.ShiftWeight( (float) ttbarEvent.numberOfGeneratedPileUpVertices() );
            outFile << lumiWeight << "  " << lumiWeightUp << "  " << lumiWeightDown;

        } else outFile << "1  1  1";

        outFile << endl;

        outFile << ttbarEvent.GoodJets().size() << "  " << ttbarEvent.Vertices().size() << "  ";
        if(!ttbarEvent.isRealData()){
        	outFile << ttbarEvent.numberOfGeneratedPileUpVertices();
        } else outFile << "9999";

        outFile << "  ";
        if ((ttbarEvent.getDataType() == DataType::ttbar) or (ttbarEvent.getDataType() == DataType::ttbar161) or
        		(ttbarEvent.getDataType() == DataType::ttbar163) or (ttbarEvent.getDataType() == DataType::ttbar166) or
        		(ttbarEvent.getDataType() == DataType::ttbar169) or (ttbarEvent.getDataType() == DataType::ttbar175) or
        		(ttbarEvent.getDataType() == DataType::ttbar178) or (ttbarEvent.getDataType() == DataType::ttbar181) or
        		(ttbarEvent.getDataType() == DataType::ttbar184)) {
        	outFile << "9999  9999  9999" << endl; //needs to be fixed
//        	outFile << ttbarEvent.MCtruthAntiTopMass() << "  " << ttbarEvent.MCtruthAntiTopMass() << "  "
//        			<< ttbarEvent.topDecayedLeptonically() << endl;
        } else outFile << "9999  9999  9999" << endl;
    }

  	//prepare the jets collection
	// Copy jets into an array
	JetCollection jetCopy;
	for (JetCollection::const_iterator j=ttbarEvent.GoodElectronCleanedJets().begin();
			j!=ttbarEvent.GoodElectronCleanedJets().end(); ++j) {
	    jetCopy.push_back(*j);
	    //if ((*j)->getBTagDiscriminators().back() > 0.5) ++nBTags;
	}

	std::sort(jetCopy.begin(), jetCopy.end(), jetPtComp);

	//cout << "Number of good jets in event: " << jetCopy.size() << "\n";

	jetsForFitting.clear();
	unsigned numJetsToFit = jetCopy.size();

	if (jetCopy.size()>=4) {
		if (numJetsToFit>4) numJetsToFit = 4;
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

  //calculate all jets pt
  double all_jets_pt = 0;
  for (unsigned int i = 0 ; i != jetsForFitting.size() ; ++i) {
    all_jets_pt = all_jets_pt+ jetsForFitting[i]->pt();
  }

  histMan->H1D("AllJetsPt")->Fill(all_jets_pt, ttbarEvent.weight());

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

//   // Loop over all permutations and extract the information
  for (size_t fit = 0 ; fit != nHitFit ; ++fit) {

    // Get the event before the fit
    hitfit::Lepjets_Event unfittedEvent  = hitfitEventsInput[fit];

    // Get the event after the fit
    hitfit::Fit_Result fitResult         = hitfitResult[fit];

    if (hitfitResult[fit].chisq()>0.0) {
      histMan->H1D("FittedTopMassAllSolutions")->Fill(fitResult.mt(), ttbarEvent.weight());
      histMan->H1D("FitChiSquaredAllSolutions")->Fill(fitResult.chisq(), ttbarEvent.weight());
      histMan->H1D("FitLogChiSqdAllSolutions")->Fill(log(fitResult.chisq()), ttbarEvent.weight());
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

      if (Globals::produceFitterASCIIoutput) {
    	  if (fitResult.chisq()<20) {
    		  tempFile << fitResult.mt() << "  " << fitResult.sigmt() << "  " << fitResult.chisq() << "  ";
    		  for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
    			  if (fitResult.jet_types()[i] == 13) tempFile << i;
    		  }
    		  tempFile << "  ";
    		  for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
    			  if (fitResult.jet_types()[i] == 14) tempFile << i;
    		  }
    		  tempFile << "  ";
    		  for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
    			  if (fitResult.jet_types()[i] == 12) tempFile << i;
    		  }
    		  tempFile << endl;
    	  }
	}

    }
    if (fitResult.chisq()>0.0 && fitResult.chisq()<bestChi2) {
      bestChi2 = fitResult.chisq();
      bestX2pos = fit;
    }

  }

  //
  // END PART WHICH EXTRACTS INFORMATION FROM HITFIT
  //

  if (bestX2pos < nHitFit+1) {
    histMan->H1D("FittedTopMassBestSolution")->Fill(hitfitResult[bestX2pos].mt(), ttbarEvent.weight());
    histMan->H1D("FitChiSquaredBestSolution")->Fill(hitfitResult[bestX2pos].chisq(), ttbarEvent.weight());
    histMan->H1D("FitLogChiSqdBestSolution")->Fill(log(hitfitResult[bestX2pos].chisq()), ttbarEvent.weight());

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

    //pass hitfit event into BAT format
    lepton_charge = ttbarEvent.getElectronFromWDecay()->charge();
    BAT::TtbarHypothesis newHyp = BatEvent(hitfitResult[bestX2pos].ev());

    if (Globals::produceFitterASCIIoutput) {
        //outFile << "goodCombi: " << hitfitResult[bestX2pos].mt() << "  ";
    	outFile << "goodCombi: ";

        for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
      		if (hitfitResult[bestX2pos].jet_types()[i] == 13) outFile << i;
      	}
        outFile << "  ";
      	for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
      		if (hitfitResult[bestX2pos].jet_types()[i] == 14) outFile << i;
      	}
      	outFile << "  ";
    	for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
	        if (hitfitResult[bestX2pos].jet_types()[i] == 12) outFile << i;
    	}
    	outFile << "  index: " << bestX2pos << endl;

    	//copy temp file to outfile
    	ifstream tempCopy (tempFileName.c_str());
        if (tempCopy.is_open())
         {
           char c;
           while ( tempCopy.get(c).good() ) outFile << c;
           tempCopy.close();
         }

    	outFile << "FinishedEvent" << endl;
    	outFile << "------------------------------------------" << endl;
    }
    tempFile.close();

  } else {
	  if (Globals::produceFitterASCIIoutput) outFile << "No solutions found by HitFit for this event" << std::endl;
  }

}

HitFitAnalyser::HitFitAnalyser(boost::shared_ptr<HistogramManager> histMan) :
    BasicAnalyser(histMan),
    outFileName("FitResults.txt"),
    outFile(outFileName.c_str()),
    // The following five initializers read the config parameters for the
    // ASCII text files which contains the physics object resolutions.
    FitterPath_              (Globals::TQAFPath),
    hitfitDefault_           (FitterPath_+"TopQuarkAnalysis/TopHitFit/data/setting/RunHitFitConfiguration.txt"),
    hitfitElectronResolution_(FitterPath_+"TopQuarkAnalysis/TopHitFit/data/resolution/tqafElectronResolution.txt"),
    hitfitMuonResolution_    (FitterPath_+"TopQuarkAnalysis/TopHitFit/data/resolution/tqafMuonResolution.txt"),
    hitfitUdscJetResolution_ (FitterPath_+"TopQuarkAnalysis/TopHitFit/data/resolution/tqafUdscJetResolution.txt"),
    hitfitBJetResolution_    (FitterPath_+"TopQuarkAnalysis/TopHitFit/data/resolution/tqafBJetResolution.txt"),
    hitfitMETResolution_     (FitterPath_+"TopQuarkAnalysis/TopHitFit/data/resolution/tqafKtResolution.txt"),
    // The following three initializers read the config parameters for the
    // values to which the leptonic W, hadronic W, and top quark masses are to
    // be constrained to.
    hitfitLepWMass_(80.4),
    hitfitHadWMass_(80.4),
    hitfitTopMass_ ( 0.0),
    // The following three initializers instantiate the translator between PAT objects
    // and HitFit objects using the ASCII text files which contains the resolutions.
    electronTranslator_(hitfitElectronResolution_),
    muonTranslator_    (hitfitMuonResolution_),
  	jetTranslator_     (hitfitUdscJetResolution_,hitfitBJetResolution_),
   	metTranslator_     (hitfitMETResolution_)

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
//      std::cout << "Distance to corresponding jet " << (*j)->getFourVector().DeltaR(hfJet) << " too large; not matching" << std::endl;
      if (hfJType != hitfit::unknown_label) evOk = false;
    }
  }

  //  if (!evOk) std::cout << "Solution is not Ok" << std::endl;

  BAT::TtbarHypothesis hyp(newEle, newMet,
			   newLep, newHad,
			   newWj1, newWj2);

  if (Globals::produceFitterASCIIoutput) {
	  outFile << newEle->phi() << "  " << newEle->eta() << "  " << newEle->pt() << "  " << lepton_charge << "  " << newMet->px() << "  " << newMet->py() << endl;
	  outFile << jetsForFitting[0]->pt() << "  " << jetsForFitting[0]->eta() << "  " << jetsForFitting[0]->phi() << "  " << jetsForFitting[0]->btagSSVHE() << "  ";
	  outFile << jetsForFitting[1]->pt() << "  " << jetsForFitting[1]->eta() << "  " << jetsForFitting[1]->phi() << "  " << jetsForFitting[1]->btagSSVHE() << "  ";
	  outFile << jetsForFitting[2]->pt() << "  " << jetsForFitting[2]->eta() << "  " << jetsForFitting[2]->phi() << "  " << jetsForFitting[2]->btagSSVHE() << "  ";
	  outFile << jetsForFitting[3]->pt() << "  " << jetsForFitting[3]->eta() << "  " << jetsForFitting[3]->phi() << "  " << jetsForFitting[3]->btagSSVHE() << endl;
  }
  return hyp;
}

FourVector HitFitAnalyser::fourVectorFromHitFit(const hitfit::Fourvec& v) {
  FourVector result(v.x(), v.y(), v.z(), v.t());
  return result;
}

void HitFitAnalyser::printFile(const string filename) {
    ifstream fin;                   // Initialise filestream object.
    char c;

    fin.open(filename.c_str(), ios::in);    // Open an input filestream.

    // Check if file opened.
    // fin.fail() returns 1 if there is a fail in the filestream.
    if(fin.fail())
    {
      cout << "Error: Unable to open " << filename << ".\n";
      exit(1);
    }

    fin.get(c);                    // Get first character for kicks.

    // While the stream hasn't failed or reached the end of file, read and display.
    while(!fin.fail() && !fin.eof())
    {
      cout << c;                 // Display character.
      fin.get(c);                 // Get the next character from the stream.
    }

    fin.close();                  // Always close the stream once done.
}

HitFitAnalyser::~HitFitAnalyser() {
    outFile.close();
}

void HitFitAnalyser::createHistograms() {
    histMan->setCurrentCollection("hitfitStudy");
    histMan->addH1D("AllJetsPt", "All jets Pt", 100, 0., 600.);
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
