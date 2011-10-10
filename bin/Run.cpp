/*
 * Run.cc
 *
 *  Created on: Mar 11, 2010
 *      Author: lkreczko
 */
//#include "cms_bristol_ana_v3.hh"
#include "TSystem.h"
#include "TStopwatch.h"
#include "TROOT.h"
#include "Analysis.h"
#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>

using namespace ROOT;
using namespace std;
using namespace BAT;
using namespace boost::program_options;

void setUpOnce();
variables_map getInputVariables(int argc, char **argv);

int main(int argc, char **argv) {
	unsigned long maxEvents(0);

	variables_map inputVariables = getInputVariables(argc, argv);

	setUpOnce();
	TStopwatch watch;
	watch.Start();
	Analysis::useJetAlgorithm(JetAlgorithm::PF2PAT);
	Analysis::useElectronAlgorithm(ElectronAlgorithm::ParticleFlow);
	Analysis::useMuonAlgorithm(MuonAlgorithm::ParticleFlow);
	Analysis::useMETAlgorithm(METAlgorithm::ParticleFlowMET);
	Analysis::luminosity = 1959.75;
	Analysis::useCustomConversionTagger(false);
	Analysis::usePFIsolation(true);
	Analysis::useCiCElectronID(true);

	std::string pileUpFile = "Pileup_2011_to_172802_LP_LumiScale.root";
	if (inputVariables.count("PUfile")) {
		pileUpFile = inputVariables["PUfile"].as<std::string>();
	}

	boost::scoped_ptr<Analysis> myAnalysis(new Analysis(pileUpFile));

	if (inputVariables.count("maxEvents")) {
		maxEvents = inputVariables["maxEvents"].as<unsigned long>();
	}

	if (maxEvents > 0){
		myAnalysis->setMaximalNumberOfEvents(maxEvents);
		cout << "Maximal number of events to be processed: " << maxEvents << ".\n";
	}
	else
		cout << "Maximal number of events to be processed: " << "all available" << ".\n";

	if (inputVariables.count("fitter")) {
		myAnalysis->useHitFit = inputVariables["fitter"].as<bool>();
	}

	myAnalysis->setUsedNeutrinoSelectionForTopPairReconstruction(NeutrinoSelectionCriterion::chi2);

	//Test samples
//	myAnalysis->addInputFile("/storage/TopQuarkGroup/mc/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/QCD_Pt-30to80_EMEnriched_nTuple_42x_mc_1_1_5x9.root");
	//==========================DATA 2010========================================

	//    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/Run2010A-Dec22ReReco_v1/*.root");//3.06 pb-1
	//    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/Run2010B-Dec22ReReco_v1/*.root");//32.96 pb-1

	//==========================DATA 2011========================================

	//    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/SingleElectron/nTuple_v1_Run2011-May10ReReco_GoldenJSON_v2/65f10056ea7ba245c10a9a8e7971901a/*.root");//204.21 pb-1
	//    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/SingleElectron/nTuple_v1_Run2011-PromptReco_GoldenJSON_exclRereco_v2/65f10056ea7ba245c10a9a8e7971901a/*.root");//60.09 pb-1

	//========================== ElectronHad PD 2011 with latest JEC ========================================
	myAnalysis->addInputFile(
			"/storage/TopQuarkGroup/data/ElectronHad/nTuple_v3_Run2011-May10ReReco_GoldenJSON/f5ee4d99da7f83721abc451fc8fada55/*.root"); //215 pb-1
	myAnalysis->addInputFile(
			"/storage/TopQuarkGroup/data/ElectronHad/nTuple_v3_Run2011-PromptReco_v4_GoldenJSON_exclRereco/f5ee4d99da7f83721abc451fc8fada55/*.root"); //926 pb-1
	myAnalysis->addInputFile(
			"/storage/TopQuarkGroup/data/ElectronHad/nTuple_v3_Run2011-PromptReco_v5_GoldenJSON_exclRereco/f5ee4d99da7f83721abc451fc8fada55/*.root"); //291 pb-1
	myAnalysis->addInputFile(
			"/storage/TopQuarkGroup/data/ElectronHad/nTuple_v3_Run2011-PromptReco_v5_GoldenJSON_05.08.11-10.08.11/f5ee4d99da7f83721abc451fc8fada55/*.root"); //77 pb-1
	myAnalysis->addInputFile(
			"/storage/TopQuarkGroup/data/ElectronHad/nTuple_v3_Run2011-PromptReco_v6_GoldenJSON_05.08.11-10.08.11/f5ee4d99da7f83721abc451fc8fada55/*.root"); //103 pb-1
	myAnalysis->addInputFile(
			"/storage/TopQuarkGroup/data/ElectronHad/nTuple_v3_Run2011-PromptReco_v6_GoldenJSON_10.08.11-19.08.11/f5ee4d99da7f83721abc451fc8fada55/*.root"); //347 pb-1

	//
	//    //==========================Summer11 samples========================================
	//
	myAnalysis->addInputFile(
			"/storage/TopQuarkGroup/mc/TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");
	myAnalysis->addInputFile(
			"/storage/TopQuarkGroup/mc/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");
	myAnalysis->addInputFile(
			"/storage/TopQuarkGroup/mc/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");

	    //BCToE
	    myAnalysis->addInputFile(
	            "/storage/TopQuarkGroup/mc/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v3_Summer11-PU_S3_START42_V11-v2/0b63e6090f671316354e220e2be2a651/*.root");
	    myAnalysis->addInputFile(
	            "/storage/TopQuarkGroup/mc/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");
	    myAnalysis->addInputFile(
	            "/storage/TopQuarkGroup/mc/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");

	    //EMEnriched
	    myAnalysis->addInputFile(
	            "/storage/TopQuarkGroup/mc/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");
	    myAnalysis->addInputFile(
	                "/storage/TopQuarkGroup/mc/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");
	    myAnalysis->addInputFile(
	            "/storage/TopQuarkGroup/mc/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");

	// Photon + jets
//	    myAnalysis->addInputFile(
//	            "/storage/TopQuarkGroup/mc/GJets_TuneZ2_100_HT_200_7TeV-madgraph/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");

	//SingleTop
	    myAnalysis->addInputFile(
				"/storage/TopQuarkGroup/mc/T_TuneZ2_s-channel_7TeV-powheg-tauola/nTuple_v3b_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");
		myAnalysis->addInputFile(
				"/storage/TopQuarkGroup/mc/T_TuneZ2_t-channel_7TeV-powheg-tauola/nTuple_v3b_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");
		myAnalysis->addInputFile(
				"/storage/TopQuarkGroup/mc/T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/nTuple_v3b_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");

	//SingleAntiTop
		myAnalysis->addInputFile(
				"/storage/TopQuarkGroup/mc/Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/nTuple_v3b_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");
		myAnalysis->addInputFile(
				"/storage/TopQuarkGroup/mc/Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/nTuple_v3b_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");
		myAnalysis->addInputFile(
				"/storage/TopQuarkGroup/mc/Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/nTuple_v3b_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");

	//di-boson
	myAnalysis->addInputFile(
			"/storage/TopQuarkGroup/mc/WW_TuneZ2_7TeV_pythia6_tauola/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");
	myAnalysis->addInputFile(
			"/storage/TopQuarkGroup/mc/WZ_TuneZ2_7TeV_pythia6_tauola/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");
	myAnalysis->addInputFile(
			"/storage/TopQuarkGroup/mc/ZZ_TuneZ2_7TeV_pythia6_tauola/nTuple_v3_Summer11-PU_S4_START42_V11-v1/0b63e6090f671316354e220e2be2a651/*.root");

	    //==========================Spring11 samples========================================
	    //Photon + jets
	    myAnalysis->addInputFile(
	            "/storage/TopQuarkGroup/mc/GJets_TuneD6T_HT-40To100_7TeV-madgraph/nTuple_v3_Spring11-PU_S1_-START311_V1G1-v1/31d11c15b464cd479887c73ed35f2bf2/*.root");

	    myAnalysis->addInputFile(
	            "/storage/TopQuarkGroup/mc/GJets_TuneD6T_HT-200_7TeV-madgraph/nTuple_v3_Spring11-PU_S1_-START311_V1G1-v1/31d11c15b464cd479887c73ed35f2bf2/*.root");

	//Systematic Samples

	cout << "starting analysis" << endl;
	myAnalysis->analyze();
	watch.Stop();
	cout << "Number of events processed per minute: "
			<< myAnalysis->getNumberOfProccessedEvents() / (watch.CpuTime() / 60) << endl;
	watch.Print();

	return 0;
}

void setUpOnce() {
	//needed to proper link vector<float> etc.
	gROOT->ProcessLine("#include <vector>");
	//prevent automatic ownership of ROOT objects
	TH1F::AddDirectory(false);
	//ignore ROOT errors (temporaly due to different nTuple content)
	gROOT->ProcessLine("gErrorIgnoreLevel = 3001;");
}

variables_map getInputVariables(int argc, char **argv) {
	// Declare the supported options.
	options_description desc("Allowed options");
	desc.add_options()("help,h", "produce help message")("maxEvents", value<unsigned long>()->default_value(0),
			"set maximal number of events to be processed");

	desc.add_options()("PUfile", value<std::string>(), "set input PU file for PU re-weighting");

	desc.add_options()("fitter", bool_switch()->default_value(false), "turn on the fitter (HitFit)");

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);
	if (vm.count("help")) {
		cout << desc << "\n";
		exit(0);
		return 0;
	}

	return vm;
}

