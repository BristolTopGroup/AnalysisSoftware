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
//#include <google/profiler.h>
#include "../interface/ConfigFile.h"
#include "../interface/GlobalVariables.h"

using namespace ROOT;
using namespace std;
using namespace BAT;
using namespace boost::program_options;

void setUpOnce();

int main(int argc, char **argv) {

	ConfigFile config(argc, argv);
	cout << "Using config-file '" << config.configPath() << endl;
	unsigned long maxEvents(config.maxEvents());
	std::string pileUpFile = config.PUFile();
    std::string bJetResoFile = config.bJetResoFile();
    std::string lightJetResoFile = config.lightJetResoFile();
	setUpOnce();
	TStopwatch watch;
	watch.Start();

	//general
	Globals::TQAFPath = config.TQAFPath();
	Globals::luminosity = config.lumi();
	Globals::useHitFit =  config.useHitFit();
	Globals::produceFitterASCIIoutput = config.fitterOutputFlag();
	//jets
	Globals::jetAlgorithm = JetAlgorithm::PF2PAT;
	Globals::minJetPt = 30.;
	Globals::maxAbsoluteJetEta = 2.4;
	//electrons
	Globals::electronAlgorithm = ElectronAlgorithm::ParticleFlow;
	Globals::electronID = ElectronID::CiCHyperTight1MC;
	Globals::minElectronET = 30.;
	Globals::MaxAbsoluteElectronEta = 2.5;
	//muons
	Globals::muonAlgorithm = MuonAlgorithm::ParticleFlow;
	Globals::maxAbsoluteMuonEta = 2.1;
	Globals::minMuonPt = 20.;
	//mets
	Globals::metAlgorithm = METAlgorithm::ParticleFlowMET;
	Globals::METCut = 20.;

	//Loading l7 JEC
	config.LoadJetL7Resolutions(bJetResoFile, lightJetResoFile);

	Analysis::useCustomConversionTagger(false);
	Analysis::usePFIsolation(true);
//	Analysis::useCiCElectronID(true);

	cout << "From Config file: " << config.PUFile() << endl;
	cout << "Using L7 jet energy corrections: " << config.bJetResoFile() << ", " << config.lightJetResoFile() << endl;

	boost::scoped_ptr<Analysis> myAnalysis(new Analysis(pileUpFile));

	if (maxEvents > 0){
		myAnalysis->setMaximalNumberOfEvents(maxEvents);
		cout << "Maximal number of events to be processed: " << maxEvents << ".\n";
	}
	else
		cout << "Maximal number of events to be processed: " << "all available" << ".\n";

    if (config.useHitFit()) cout << "Using HitFit.\n";

	myAnalysis->setUsedNeutrinoSelectionForTopPairReconstruction(NeutrinoSelectionCriterion::chi2);

	vector<string> inputFiles = config.inputFiles();

	for(unsigned int index = 0; index < inputFiles.size(); ++ index){
		myAnalysis->addInputFile(inputFiles.at(index).c_str());
	}


//	ProfilerStart("BATProfile");
	cout << "starting analysis" << endl;
	myAnalysis->analyze();
//	ProfilerStop();
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
