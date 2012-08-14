#include "cute/cute.h"
#include "cute/ide_listener.h"
#include "cute/cute_runner.h"

#include "TestBtagWeight.h"
#include "TestDatasetInformation.h"
#include "TestEventWeightProvider.h"
#include "TestElectron.h"
#include "TestElectronReader.h"
#include "TestEvent.h"
#include "TestEventCounter.h"
#include "TestGenJetReader.h"
#include "TestHistogram.h"
#include "TestHistogramManager.h"
#include "TestJet.h"
#include "TestJetReader.h"
#include "TestLepton.h"
#include "TestMET.h"
#include "TestMETReader.h"
#include "TestMuon.h"
#include "TestMuonReader.h"
#include "TestNTupleEventReader.h"
#include "TestParticle.h"
#include "TestROOT.h"
#include "TestStringOperations.h"
#include "TestTHCollection.h"
#include "TestTrack.h"
#include "TestTrackReader.h"
#include "TestTTbarSolution.h"
#include "TestVariableReader.h"
#include "TestVertex.h"
#include "TestVertexReader.h"

#include "TROOT.h"
#include "../interface/Python/ConfigFile.h"
#include "../interface/GlobalVariables.h"
#include <iostream>

using namespace std;

void setUpOnce() {
	//needed to proper link vector<float> etc.
	gROOT->ProcessLine("#include <vector>");
	//prevent automatic ownership of ROOT objects
	TH1F::AddDirectory(false);
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

void runSuite() {
	cute::suite s = make_suite_TestBtagWeight();
//    s += make_suite_TestDatasetInformation();
//    s += make_suite_TestEventWeightProvider();
//    s += make_suite_TestElectron();
//    s += make_suite_TestElectronReader();
//    s += make_suite_TestEvent();
//    s += make_suite_TestEventCounter();
//    s += make_suite_TestGenJetReader();
//    s += make_suite_TestHistogramManager();
//    s += make_suite_TestHistogram();
//    s += make_suite_TestJet();
//    s += make_suite_TestJetReader();
//    s += make_suite_TestLepton();
//    s += make_suite_TestMET();
//    s += make_suite_TestMETReader();
	s += make_suite_TestMuon();
	s += make_suite_TestMuonReader();
//    s += make_suite_TestParticle();
//    s += make_suite_ROOTLearnTests();
//    s += make_suite_TestStringOperations();
//    s += make_suite_TestTHCollection();
//    s += make_suite_TestTrack();
//    s += make_suite_TestTTbarSolution();
//    s += make_suite_TestVariableReader();
//    s += make_suite_TestVertex();
//    s += make_suite_TestVertexReader();
//    s += make_suite_TestNTupleEventReader();
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "Testing BristolAnalysis Tools");

}

int main(int argc, char **argv) {
	setUpOnce();
	ConfigFile config(argc, argv);
	setConfiguration(config);
	runSuite();
	return 0;
}

