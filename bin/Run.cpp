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
#include "../interface/Python/ConfigFile.h"
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

	boost::scoped_ptr<Analysis> myAnalysis(new Analysis(config.datasetInfoFile()));

	myAnalysis->setMaximalNumberOfEvents(Globals::maxEvents);

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
	watch.Reset();
	cout << "Finishing up analysis and writing histograms do disk (wait for \"Analysis finished\")" << endl;
	watch.Start();
	myAnalysis->finishAnalysis();
	watch.Stop();
	watch.Print();
	cout << "==========================================================" << endl;
	cout << "\t \t Analysis finished" << endl;
	cout << "==========================================================" << endl;
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
	cout << "Using set-up for nTuple version " << config.nTupleVersion() << endl;
	cout << "Using set-up for center of mass energy = " << config.centerOfMassEnergy() << endl;
	cout << "Loading configuration..." << endl;
	cout << "Using config-file '" << config.configPath() << endl;
	cout << "Using Dataset info file:  '" << config.datasetInfoFile() << endl;
	cout << "Using pileup-file '" << config.PUFile() << endl;
	cout << "Using L7 jet energy corrections: " << config.bJetResoFile() << ", ";
	cout << config.lightJetResoFile() << endl;
	if (config.jesSystematic()!=0)
		cout << "Varying all the jets by " << config.jesSystematic() << " sigma(s) (JEC uncertainty)\n";
	long maxEvents(config.maxEvents());
	cout << "Maximal number of events to be processed: ";
	if (maxEvents > 0)
		cout << maxEvents << ".\n";
	else
		cout << "all available" << ".\n";
	cout << "Scaling to luminosity " << config.lumi() << "pb-1" << endl;
	if (config.useHitFit())
		cout << "Using HitFit.\n";

	config.loadIntoMemory();


	//@Deprecated: move to selections
	//jets
	Globals::jetAlgorithm = JetAlgorithm::PF2PAT;
	//electrons
	Globals::electronAlgorithm = ElectronAlgorithm::ParticleFlow;
	//muons
	Globals::muonAlgorithm = MuonAlgorithm::ParticleFlow;
	//mets
	Globals::metAlgorithm = METAlgorithm::patMETsPFlow;
}
