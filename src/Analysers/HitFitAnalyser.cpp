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
	weight_ = event->weight() * prescale_ * scale_;
	//fit only the events that pass full ttbar selection
	if (!topEplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event))
		;
	return;

	const JetCollection jets = topEplusJetsRefSelection_->cleanedJets(event);
	LeptonPointer selectedLepton = topEplusJetsRefSelection_->signalLepton(event);
	METPointer met = event->MET();

	histMan_->H1D("m3_diff")->Fill(fabs(truthMatchEvent.M3() - TtbarHypothesis::M3(jets)));

	boost::scoped_ptr<ChiSquaredBasedTopPairReconstruction> chi2Reco(
			new ChiSquaredBasedTopPairReconstruction(selectedLepton, met, jets));
	if (!chi2Reco->meetsInitialCriteria()) { //reports details on failure and skips event
		cout << chi2Reco->getDetailsOnFailure();
		return;
	}
	TtbarHypothesisPointer bestTopHypothesis = chi2Reco->getBestSolution();

	//set MC matching flag
	if (event->getDataType() == DataType::TTJets)
		do_MC_matching = true;
	else
		do_MC_matching = false;

	//initialise pile-up shifters
	PoissonMeanShifter PShiftUp(0.6); // PU-systematic
	PoissonMeanShifter PShiftDown(-0.6); // PU-systematic

	//temporary file for jet fit info (ugly but works)
	string tempFileName = "temp.txt";
	ofstream tempFile(tempFileName.c_str());

	histMan_->setCurrentHistogramFolder("hitfitStudy");

	if (Globals::produceFitterASCIIoutput) {
		//write the event info into ASCII file
		outFile << "------------------------------------------" << endl;
		outFile << "BeginEvent" << endl;
		outFile << "RunNr: " << event->runnumber() << endl;
		outFile << "LumiSection: " << event->lumiblock() << endl;
		outFile << "EventNr: " << event->eventnumber() << endl;
		outFile << "EventWeight: " << weight_ << "  ";
		double nVertices(0);
		if (!event->isRealData()) {
			float lumiWeight = event->PileUpWeight();

			if (Globals::pileUpReweightingMethod == PileUpReweightingMethod::averagePileUp
					|| Globals::pileUpReweightingMethod == PileUpReweightingMethod::threeDReweighting)
				nVertices = event->averageNumberOfVertices();
			if (Globals::pileUpReweightingMethod == PileUpReweightingMethod::inTimePileUpOnly)
				nVertices = event->inTimeOnlyNumberOfVertices();
			float lumiWeightUp = lumiWeight * PShiftUp.ShiftWeight(nVertices);
			float lumiWeightDown = lumiWeight * PShiftDown.ShiftWeight(nVertices);
			outFile << lumiWeight << "  " << lumiWeightUp << "  " << lumiWeightDown;

		} else
			outFile << "1  1  1";

		outFile << endl;

		outFile << jets.size() << "  " << event->Vertices().size() << "  ";
		if (!event->isRealData()) {
			outFile << nVertices;
		} else
			outFile << "9999";

		outFile << "  ";
		if ((event->getDataType() == DataType::TTJets) or (event->getDataType() == DataType::ttbar161)
				or (event->getDataType() == DataType::ttbar163) or (event->getDataType() == DataType::ttbar166)
				or (event->getDataType() == DataType::ttbar169) or (event->getDataType() == DataType::ttbar175)
				or (event->getDataType() == DataType::ttbar178) or (event->getDataType() == DataType::ttbar181)
				or (event->getDataType() == DataType::ttbar184)) {
			outFile << "9999  9999  9999" << endl; //needs to be fixed
		} else
			outFile << "9999  9999  9999" << endl;
	}

	//prepare the jets collection
	// Copy jets into an array
	JetCollection jetCopy;
	for (JetCollection::const_iterator j = jets.begin(); j != jets.end(); ++j) {
		jetCopy.push_back(*j);
	}

	std::sort(jetCopy.begin(), jetCopy.end(), jetPtComp);

	jetsForFitting.clear();
	unsigned numJetsToFit = jetCopy.size();

	if (jetCopy.size() >= 4) {
		if (numJetsToFit > 4)
			numJetsToFit = 4;
		jetsForFitting.insert(jetsForFitting.begin(), jetCopy.begin(), jetCopy.begin() + numJetsToFit);
	}

	BatHitFit hhFitter(electronTranslator_, muonTranslator_, jetTranslator_, metTranslator_, hitfitDefault_,
			hitfitLepWMass_, hitfitHadWMass_, hitfitTopMass_);

	// Clear the internal state
	hhFitter.clear();

	const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(selectedLepton));
	//TODO: fix the fitter to accept lepton class OR particle!!
	hhFitter.AddLepton(*signalElectron);

	// Add jets into HitFit
	for (size_t jet = 0; jet != jetsForFitting.size(); ++jet) {
		hhFitter.AddJet(*jetsForFitting.at(jet));
	}

	//calculate all jets pt
	double all_jets_pt = 0;
	for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
		all_jets_pt = all_jets_pt + jetsForFitting[i]->pt();
	}

	histMan_->H1D("AllJetsPt")->Fill(all_jets_pt, weight_);

	// Add missing transverse energy into HitFit
	hhFitter.SetMet(*met);

	// Results of the fit for all jet permutation of the event
	std::vector<hitfit::Lepjets_Event> hitfitEventsInput;

	// Results of the fit for all jet permutation of the event
	std::vector<hitfit::Fit_Result> hitfitResult;
	//
	// R U N   H I T F I T
	//
	// Run the kinematic fit and get how many permutations is possible
	// in the fit

	size_t nHitFit = hhFitter.FitAllPermutation();

//   // Get the number of jets
//   nHitFitJet      = hhFitter.GetEvent().njets();

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
			histMan_->H1D("FittedTopMassAllSolutions")->Fill(fitResult.mt(), weight_);
			histMan_->H1D("FitChiSquaredAllSolutions")->Fill(fitResult.chisq(), weight_);
			histMan_->H1D("FitLogChiSqdAllSolutions")->Fill(log(fitResult.chisq()), weight_);
			const hitfit::Column_Vector &px = fitResult.pullx();
			const hitfit::Column_Vector &py = fitResult.pully();
			double sumPx = 0.0;
			for (int i = 0; i < px.num_row(); ++i) {
				histMan_->H1D("PullDistAllVarsAllSolutions")->Fill(px[i]);
				histMan_->H2D("PullDistPerVarAllSolutions")->Fill(px[i], i);
				sumPx += px[i] * px[i];
			}
			histMan_->H1D("PullSumSquaredAllSolutions")->Fill(sumPx);
			for (int i = 0; i < py.num_row(); ++i) {
				histMan_->H1D("PullDistYVarsAllSolutions")->Fill(py[i]);
			}

			if (Globals::produceFitterASCIIoutput) {
				if (fitResult.chisq() < 20) {
					tempFile << fitResult.mt() << "  " << fitResult.sigmt() << "  " << fitResult.chisq() << "  ";
					for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
						if (fitResult.jet_types()[i] == 13)
							tempFile << i;
					}
					tempFile << "  ";
					for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
						if (fitResult.jet_types()[i] == 14)
							tempFile << i;
					}
					tempFile << "  ";
					for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
						if (fitResult.jet_types()[i] == 12)
							tempFile << i;
					}
					tempFile << endl;
				}
			}

		}
		if (fitResult.chisq() > 0.0 && fitResult.chisq() < bestChi2) {
			bestChi2 = fitResult.chisq();
			bestX2pos = fit;
		}

	}

	//
	// END PART WHICH EXTRACTS INFORMATION FROM HITFIT
	//

	if (bestX2pos < nHitFit + 1) {
		histMan_->H1D("FittedTopMassBestSolution")->Fill(hitfitResult[bestX2pos].mt(), weight_);
		histMan_->H1D("FitChiSquaredBestSolution")->Fill(hitfitResult[bestX2pos].chisq(), weight_);
		histMan_->H1D("FitLogChiSqdBestSolution")->Fill(log(hitfitResult[bestX2pos].chisq()), weight_);

		const hitfit::Column_Vector &px = hitfitResult[bestX2pos].pullx();
		const hitfit::Column_Vector &py = hitfitResult[bestX2pos].pully();
		double sumPx = 0.0;
		for (int i = 0; i < px.num_row(); ++i) {
			histMan_->H1D("PullDistAllVarsBestSolution")->Fill(px[i]);
			histMan_->H2D("PullDistPerVarBestSolution")->Fill(px[i], i);
			sumPx += px[i] * px[i];
		}
		histMan_->H1D("PullSumSquaredBestSolution")->Fill(sumPx);
		for (int i = 0; i < py.num_row(); ++i) {
			histMan_->H1D("PullDistYVarsBestSolution")->Fill(py[i]);
		}

		//pass hitfit event into BAT format
		lepton_charge = selectedLepton->charge();
		BAT::TtbarHypothesis newHyp = BatEvent(hitfitResult[bestX2pos].ev());

		if (Globals::produceFitterASCIIoutput) {
			//outFile << "goodCombi: " << hitfitResult[bestX2pos].mt() << "  ";
			outFile << "goodCombi: ";

			for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
				if (hitfitResult[bestX2pos].jet_types()[i] == 13)
					outFile << i;
			}
			outFile << "  ";
			for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
				if (hitfitResult[bestX2pos].jet_types()[i] == 14)
					outFile << i;
			}
			outFile << "  ";
			for (unsigned int i = 0; i != jetsForFitting.size(); ++i) {
				if (hitfitResult[bestX2pos].jet_types()[i] == 12)
					outFile << i;
			}
			outFile << "  index: " << bestX2pos << endl;

			//copy temp file to outfile
			ifstream tempCopy(tempFileName.c_str());
			if (tempCopy.is_open()) {
				char c;
				while (tempCopy.get(c).good())
					outFile << c;
				tempCopy.close();
			}

			outFile << "FinishedEvent" << endl;
			outFile << "------------------------------------------" << endl;
		}
		tempFile.close();

	} else {
		if (Globals::produceFitterASCIIoutput)
			outFile << "No solutions found by HitFit for this event" << std::endl;
	}

}

HitFitAnalyser::HitFitAnalyser(boost::shared_ptr<HistogramManager> histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		outFileName("FitResults.txt"), //
		outFile(outFileName.c_str()), //
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
		hitfitTopMass_(0.0), //
		// The following three initializers instantiate the translator between PAT objects
		// and HitFit objects using the ASCII text files which contains the resolutions.
		electronTranslator_(hitfitElectronResolution_), //
		muonTranslator_(hitfitMuonResolution_), //
		jetTranslator_(hitfitUdscJetResolution_, hitfitBJetResolution_), //
		metTranslator_(hitfitMETResolution_), //
		topEplusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection())

{

}

BAT::TtbarHypothesis HitFitAnalyser::BatEvent(const hitfit::Lepjets_Event& ev) {
	// Do the electron
	//BAT::ElectronPointer newEle(new BAT::Electron(*truthMatchEvent.electronFromW));
	BAT::ElectronPointer newEle(new BAT::Electron());
	if (ev.nleps() > 0) {
		newEle->setFourVector(fourVectorFromHitFit(ev.lep(0).p()));
	} else {
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
		FourVector hfJet = fourVectorFromHitFit(ev.jet(i).p());
		int hfJType = ev.jet(i).type();
//    if ((*j)->getFourVector().DeltaR(hfJet) < 0.005) {
		if (hfJType != hitfit::unknown_label) {
			BAT::Jet newJet(**j);
			newJet.setFourVector(hfJet);

			if (hfJType == hitfit::lepb_label)
				*newLep = newJet;
			if (hfJType == hitfit::hadb_label)
				*newHad = newJet;
			if (hfJType == hitfit::hadw1_label)
				*newWj1 = newJet;
			if (hfJType == hitfit::hadw2_label)
				*newWj2 = newJet;
		}
	}

	BAT::TtbarHypothesis hyp(newEle, newMet, newLep, newHad, newWj1, newWj2);

	// do MC matching study
	if (do_MC_matching) {
		//Particle Pointers for best fitted hypothesis
		BAT::ParticlePointer hadronicTop, leptonicTop, leptonicW, hadronicW;
		leptonicW = ParticlePointer(new Particle(*newMet + *newEle));
		if (newWj1 != newWj2)
			hadronicW = ParticlePointer(new Particle(*newWj1 + *newWj2));
		else
			hadronicW = newWj1;
		leptonicTop = ParticlePointer(new Particle(*newLep + *leptonicW));
		hadronicTop = ParticlePointer(new Particle(*newHad + *hadronicW));

		double deltaRelectron, deltaRhadronicBjet, deltaRleptonicBjet, deltaRWjet1, deltaRWjet2, sumDeltaR;
		deltaRelectron = truthMatchEvent.leptonFromW->deltaR(hyp.leptonFromW);
		deltaRhadronicBjet = truthMatchEvent.hadronicBJet->deltaR(hyp.hadronicBJet);
		deltaRleptonicBjet = truthMatchEvent.leptonicBjet->deltaR(hyp.leptonicBjet);

		histMan_->H1D("deltaPtElectron")->Fill(
				(truthMatchEvent.leptonFromW->pt() - hyp.leptonFromW->pt()) / truthMatchEvent.leptonFromW->pt());
		histMan_->H1D("deltaRelectron")->Fill(truthMatchEvent.leptonFromW->deltaR(hyp.leptonFromW));
		histMan_->H1D("deltaPtHadronicBjet")->Fill(
				(truthMatchEvent.hadronicBJet->pt() - hyp.hadronicBJet->pt()) / truthMatchEvent.hadronicBJet->pt());
		histMan_->H1D("deltaRhadronicBjet")->Fill(truthMatchEvent.hadronicBJet->deltaR(hyp.hadronicBJet));
		histMan_->H1D("deltaPtLeptonicBjet")->Fill(
				(truthMatchEvent.leptonicBjet->pt() - hyp.leptonicBjet->pt()) / truthMatchEvent.leptonicBjet->pt());
		histMan_->H1D("deltaRleptonicBjet")->Fill(truthMatchEvent.leptonicBjet->deltaR(hyp.leptonicBjet));
		if (truthMatchEvent.jet1FromW->deltaR(hyp.jet1FromW) < truthMatchEvent.jet1FromW->deltaR(hyp.jet2FromW)) {
			deltaRWjet1 = truthMatchEvent.jet1FromW->deltaR(hyp.jet1FromW);
			deltaRWjet2 = truthMatchEvent.jet2FromW->deltaR(hyp.jet2FromW);
			histMan_->H1D("deltaPtWjet1")->Fill(
					(truthMatchEvent.jet1FromW->pt() - hyp.jet1FromW->pt()) / truthMatchEvent.jet1FromW->pt());
			histMan_->H1D("deltaRWjet1")->Fill(truthMatchEvent.jet1FromW->deltaR(hyp.jet1FromW));
			histMan_->H1D("deltaPtWjet2")->Fill(
					(truthMatchEvent.jet2FromW->pt() - hyp.jet2FromW->pt()) / truthMatchEvent.jet2FromW->pt());
			histMan_->H1D("deltaRWjet2")->Fill(truthMatchEvent.jet2FromW->deltaR(hyp.jet2FromW));
		} else {
			deltaRWjet1 = truthMatchEvent.jet1FromW->deltaR(hyp.jet2FromW);
			deltaRWjet2 = truthMatchEvent.jet2FromW->deltaR(hyp.jet1FromW);
			histMan_->H1D("deltaPtWjet1")->Fill(
					(truthMatchEvent.jet1FromW->pt() - hyp.jet2FromW->pt()) / truthMatchEvent.jet1FromW->pt());
			histMan_->H1D("deltaRWjet1")->Fill(truthMatchEvent.jet1FromW->deltaR(hyp.jet2FromW));
			histMan_->H1D("deltaPtWjet2")->Fill(
					(truthMatchEvent.jet2FromW->pt() - hyp.jet1FromW->pt()) / truthMatchEvent.jet2FromW->pt());
			histMan_->H1D("deltaRWjet2")->Fill(truthMatchEvent.jet2FromW->deltaR(hyp.jet1FromW));
		}

		sumDeltaR = deltaRelectron + deltaRhadronicBjet + deltaRleptonicBjet + deltaRWjet1 + deltaRWjet2;
		histMan_->H1D("SumDeltaR")->Fill(sumDeltaR);

		histMan_->H1D("deltaLeptonicTopMass")->Fill(
				(truthMatchEvent.leptonicTop->mass() - leptonicTop->mass()) / truthMatchEvent.leptonicTop->mass());
		histMan_->H1D("deltaHadronicTopMass")->Fill(
				(truthMatchEvent.hadronicTop->mass() - hadronicTop->mass()) / truthMatchEvent.hadronicTop->mass());

		if (sumDeltaR < 0.4) {
			if (truthMatchEvent.jet1FromW->deltaR(hyp.jet1FromW) < truthMatchEvent.jet1FromW->deltaR(hyp.jet2FromW)) {
				histMan_->H1D("deltaPtWjet1Best")->Fill(
						(truthMatchEvent.jet1FromW->pt() - hyp.jet1FromW->pt()) / truthMatchEvent.jet1FromW->pt());
				histMan_->H1D("deltaPtWjet2Best")->Fill(
						(truthMatchEvent.jet2FromW->pt() - hyp.jet2FromW->pt()) / truthMatchEvent.jet2FromW->pt());
			} else {
				histMan_->H1D("deltaPtWjet1")->Fill(
						(truthMatchEvent.jet1FromW->pt() - hyp.jet2FromW->pt()) / truthMatchEvent.jet1FromW->pt());
				histMan_->H1D("deltaPtWjet2")->Fill(
						(truthMatchEvent.jet2FromW->pt() - hyp.jet1FromW->pt()) / truthMatchEvent.jet2FromW->pt());
			}
			histMan_->H1D("deltaPtElectronBest")->Fill(
					(truthMatchEvent.leptonFromW->pt() - hyp.leptonFromW->pt()) / truthMatchEvent.leptonFromW->pt());
			histMan_->H1D("deltaPtHadronicBjetBest")->Fill(
					(truthMatchEvent.hadronicBJet->pt() - hyp.hadronicBJet->pt()) / truthMatchEvent.hadronicBJet->pt());
			histMan_->H1D("deltaPtLeptonicBjetBest")->Fill(
					(truthMatchEvent.leptonicBjet->pt() - hyp.leptonicBjet->pt()) / truthMatchEvent.leptonicBjet->pt());
			histMan_->H1D("deltaLeptonicTopMassBest")->Fill(
					(truthMatchEvent.leptonicTop->mass() - leptonicTop->mass()) / truthMatchEvent.leptonicTop->mass());
			histMan_->H1D("deltaHadronicTopMassBest")->Fill(
					(truthMatchEvent.hadronicTop->mass() - hadronicTop->mass()) / truthMatchEvent.hadronicTop->mass());
		}
	}

	if (Globals::produceFitterASCIIoutput) {
		outFile << newEle->phi() << "  " << newEle->eta() << "  " << newEle->pt() << "  " << lepton_charge << "  "
				<< newMet->px() << "  " << newMet->py() << endl;
		outFile << jetsForFitting[0]->pt() << "  " << jetsForFitting[0]->eta() << "  " << jetsForFitting[0]->phi()
				<< "  " << jetsForFitting[0]->btagSSVHE() << "  ";
		outFile << jetsForFitting[1]->pt() << "  " << jetsForFitting[1]->eta() << "  " << jetsForFitting[1]->phi()
				<< "  " << jetsForFitting[1]->btagSSVHE() << "  ";
		outFile << jetsForFitting[2]->pt() << "  " << jetsForFitting[2]->eta() << "  " << jetsForFitting[2]->phi()
				<< "  " << jetsForFitting[2]->btagSSVHE() << "  ";
		outFile << jetsForFitting[3]->pt() << "  " << jetsForFitting[3]->eta() << "  " << jetsForFitting[3]->phi()
				<< "  " << jetsForFitting[3]->btagSSVHE() << endl;
	}
	return hyp;
}

FourVector HitFitAnalyser::fourVectorFromHitFit(const hitfit::Fourvec& v) {
	FourVector result(v.x(), v.y(), v.z(), v.t());
	return result;
}

void HitFitAnalyser::setMCTTbarHypothesis(const TtbarHypothesis& mcEvent) {
	truthMatchEvent = mcEvent;
}

void HitFitAnalyser::printFile(const string filename) {
	ifstream fin; // Initialise filestream object.
	char c;

	fin.open(filename.c_str(), ios::in); // Open an input filestream.

	// Check if file opened.
	// fin.fail() returns 1 if there is a fail in the filestream.
	if (fin.fail()) {
		cout << "Error: Unable to open " << filename << ".\n";
		exit(1);
	}

	fin.get(c); // Get first character for kicks.

	// While the stream hasn't failed or reached the end of file, read and display.
	while (!fin.fail() && !fin.eof()) {
		cout << c; // Display character.
		fin.get(c); // Get the next character from the stream.
	}

	fin.close(); // Always close the stream once done.
}

HitFitAnalyser::~HitFitAnalyser() {
	outFile.close();
}

void HitFitAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder("hitfitStudy");
	histMan_->addH1D("AllJetsPt", "All jets Pt", 100, 0., 600.);
	histMan_->addH1D("FittedTopMassAllSolutions", "Fitted top mass all solutions", 100, 0., 400.);
	histMan_->addH1D("FitChiSquaredAllSolutions", "Fit chi-squared all solutions", 100, 0., 20.);
	histMan_->addH1D("FitLogChiSqdAllSolutions", "Fit log(chi-sqd) all solutions", 100, -5., 5.);
	histMan_->addH1D("FittedTopMassBestSolution", "Fitted top mass best solution", 100, 0., 400.);
	histMan_->addH1D("FitChiSquaredBestSolution", "Fit chi-squared best solution", 100, 0., 20.);
	histMan_->addH1D("FitLogChiSqdBestSolution", "Fit log(chi-sqd) best solutions", 100, -5., 5.);

	histMan_->addH1D("PullDistAllVarsAllSolutions", "Pulls well measured all solutions", 100, -10., 10.);
	histMan_->addH1D("PullSumSquaredAllSolutions", "Sum-squared pulls all solutions", 100, 0, 500.);
	histMan_->addH2D("PullDistPerVarAllSolutions", "Pulls well measured vs varno all", 100, -10., 10., 25, 0., 25.);
	histMan_->addH1D("PullDistYVarsAllSolutions", "Pulls poorly measured all", 100, -10., 10.);
	histMan_->addH1D("PullDistAllVarsBestSolution", "Pulls well measured best solution", 100, -10., 10.);
	histMan_->addH1D("PullSumSquaredBestSolution", "Sum-squared pulls best solution", 100, 0, 500.);
	histMan_->addH2D("PullDistPerVarBestSolution", "Pulls well measured vs varno best", 100, -10., 10., 25, 0., 25.);
	histMan_->addH1D("PullDistYVarsBestSolution", "Pulls poorly measured best", 100, -10., 10.);

	// MC matching study histograms
	histMan_->addH1D("deltaPtElectron", "Pt difference between truth and fitted electrons", 100, -1.5, 1.5);
	histMan_->addH1D("deltaRelectron", "DeltaR between truth and fitted electrons", 100, 0., 2.);
	histMan_->addH1D("deltaPtHadronicBjet", "Pt difference between truth and fitted b-jets from hadronic top", 100,
			-1.5, 1.5);
	histMan_->addH1D("deltaRhadronicBjet", "DeltaR between truth and fitted b-jets from hadronic top", 100, 0., 5.);
	histMan_->addH1D("deltaPtLeptonicBjet", "Pt difference between truth and fitted b-jets from leptonic top", 100,
			-1.5, 1.5);
	histMan_->addH1D("deltaRleptonicBjet", "DeltaR between truth and fitted b-jets from leptonic top", 100, 0., 5.);
	histMan_->addH1D("deltaPtWjet1", "Pt difference between truth and fitted W jets 1", 100, -1.5, 1.5);
	histMan_->addH1D("deltaRWjet1", "DeltaR between truth and fitted W jets 1", 100, 0., 5.);
	histMan_->addH1D("deltaPtWjet2", "Pt difference between truth and fitted W jets 2", 100, -1.5, 1.5);
	histMan_->addH1D("deltaRWjet2", "DeltaR between truth and fitted W jets 2", 100, 0., 5.);
	histMan_->addH1D("SumDeltaR", "Summarised deltaR of all e+jets", 100, 0., 25.);
	histMan_->addH1D("deltaPtElectronBest", "Pt difference between truth and fitted electrons for matched solutions",
			100, -1.5, 1.5);
	histMan_->addH1D("deltaPtHadronicBjetBest",
			"Pt difference between truth and fitted b-jets from hadronic top for matched solutions", 100, -1.5, 1.5);
	histMan_->addH1D("deltaPtLeptonicBjetBest",
			"Pt difference between truth and fitted b-jets from leptonic top for matched solutions", 100, -1.5, 1.5);
	histMan_->addH1D("deltaPtWjet1Best", "Pt difference between truth and fitted W jets 1 for matched solutions", 100,
			-1.5, 1.5);
	histMan_->addH1D("deltaPtWjet2Best", "Pt difference between truth and fitted W jets 2 for matched solutions", 100,
			-1.5, 1.5);
	histMan_->addH1D("deltaMET", "Delta MET between truth and fitted solutions", 100, -50., 50.);
	histMan_->addH1D("deltaMETbest", "Delta MET between truth and fitted matched solutions", 100, -50., 50.);
	histMan_->addH1D("deltaLeptonicTopMass", "Difference between truth and fitted leptonic top masses", 100, -1.5, 1.5);
	histMan_->addH1D("deltaHadronicTopMass", "Difference between truth and fitted hadronic top masses", 100, -1.5, 1.5);
	histMan_->addH1D("deltaLeptonicTopMassBest",
			"Difference between truth and fitted leptonic top masses for matched solutions", 100, -1.5, 1.5);
	histMan_->addH1D("deltaHadronicTopMassBest",
			"Difference between truth and fitted hadronic top masses for matched solutions", 100, -1.5, 1.5);

}
