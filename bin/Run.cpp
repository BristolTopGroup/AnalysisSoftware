/*
 * Run.cc
 *
 *  Created on: Mar 11, 2010
 *      Author: lkreczko
 */
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
void setConfiguration(const ConfigFile);

int main(int argc, char **argv) {
	setUpOnce();
	TStopwatch watch;

	watch.Start();
	ConfigFile config(argc, argv);
	setConfiguration(config);
	watch.Stop();

	cout << "Time to read configuration: " << watch.CpuTime() << "s" << endl;
	watch.Reset();

	watch.Start();
	Analysis::useCustomConversionTagger(false);
	Analysis::usePFIsolation(true);

	boost::scoped_ptr<Analysis> myAnalysis(new Analysis(config.PUFile()));

	myAnalysis->setMaximalNumberOfEvents(Globals::maxEvents);

	//@DEPRECATED
//	myAnalysis->useHitFit = Globals::useHitFit;

	//@DEPRECATED
	myAnalysis->setUsedNeutrinoSelectionForTopPairReconstruction(NeutrinoSelectionCriterion::chi2);

	vector<string> inputFiles = config.inputFiles();

	for (unsigned int index = 0; index < inputFiles.size(); ++index) {
		myAnalysis->addInputFile(inputFiles.at(index).c_str());
	}

	watch.Stop();
	cout << "Time to prepare analysis: " << watch.CpuTime() << "s" << endl;
	watch.Reset();

	cout << "starting analysis" << endl;
	watch.Start();
	//	ProfilerStart("BATProfile");
	myAnalysis->analyse();
	//	ProfilerStop();
	watch.Stop();
	double eventsPerMinute = myAnalysis->getNumberOfProccessedEvents() / (watch.CpuTime() / 60);
	cout << "Number of events processed per minute: " << eventsPerMinute << endl;
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

void setConfiguration(ConfigFile config) {
	cout << "Loading configuration..." << endl;
	cout << "Using config-file '" << config.configPath() << endl;
	cout << "Using L7 jet energy corrections: " << config.bJetResoFile() << ", ";
	cout << config.lightJetResoFile() << endl;
	long maxEvents(config.maxEvents());
	cout << "Maximal number of events to be processed: ";
	if (maxEvents > 0)
		cout << maxEvents << ".\n";
	else
		cout << "all available" << ".\n";

	if (config.useHitFit())
		cout << "Using HitFit.\n";

	config.loadIntoMemory();
	
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

//	//Loading l7 JEC
//	config.LoadJetL7Resolutions(bJetResoFile, lightJetResoFile);

//	cout << "From Config file: " << pileUpFile << endl;
//	cout << "Using L7 jet energy corrections: " << bJetResoFile << ", " << lightJetResoFile << endl;
}
