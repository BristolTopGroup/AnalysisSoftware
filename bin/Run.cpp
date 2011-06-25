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

using namespace ROOT;
using namespace std;
using namespace BAT;

void setUpOnce() {
    //needed to proper link vector<float> etc.
    gROOT->ProcessLine("#include <vector>");
    //prevent automatic ownership of ROOT objects
    TH1F::AddDirectory(false);
    //ignore ROOT errors (temporaly due to different nTuple content)
    gROOT->ProcessLine("gErrorIgnoreLevel = 3001;");
}

int main(int argc, char **argv) {
    setUpOnce();
    TStopwatch watch;
    watch.Start();
    Analysis::useJetAlgorithm(JetAlgorithm::PF2PAT);
    Analysis::useElectronAlgorithm(ElectronAlgorithm::ParticleFlow);
    Analysis::useMuonAlgorithm(MuonAlgorithm::ParticleFlow);
    Analysis::useMETAlgorithm(METAlgorithm::ParticleFlowMET);
    Analysis::luminosity = 715.09;
    Analysis::useCustomConversionTagger(false);
    Analysis::usePFIsolation(true);

    boost::scoped_ptr<Analysis> myAnalysis(new Analysis());
    //        myAnalysis->setMaximalNumberOfEvents(10000);
    myAnalysis->setUsedNeutrinoSelectionForTopPairReconstruction(NeutrinoSelectionCriterion::chi2);

    //Test samples
    //        myAnalysis->addInputFile("/storage/TopQuarkGroup/TTJet_nTuple_41x_mc.root");

    //==========================DATA 2010========================================

    //    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/Run2010A-Dec22ReReco_v1/*.root");//3.06 pb-1
    //    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/Run2010B-Dec22ReReco_v1/*.root");//32.96 pb-1

    //==========================DATA 2011========================================

    //    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/SingleElectron/nTuple_v1_Run2011-May10ReReco_GoldenJSON_v2/65f10056ea7ba245c10a9a8e7971901a/*.root");//204.21 pb-1
    //    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/SingleElectron/nTuple_v1_Run2011-PromptReco_GoldenJSON_exclRereco_v2/65f10056ea7ba245c10a9a8e7971901a/*.root");//60.09 pb-1

    //==========================ElectronHad PD ========================================

myAnalysis->addInputFile("/gpfs_phys/storm/cms/user/kreczko/ElectronHad/nTuple_v2b_Run2011-PromptReco_GoldenJSON_17.06.11-24.06.11/4ee1203e97f9a00957561f563636708a/*.root");

    //==========================Summer11 samples========================================


    //EMEnriched
    
myAnalysis->addInputFile("/gpfs_phys/storm/cms/user/kreczko/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia/nTuple_v2_Summer11-PU_S4_START42_V11-v1/7c548abbf04de779162e4a2cbdd09438/*.root");

    //==========================Spring11 samples========================================

    cout << "starting analysis" << endl;
    myAnalysis->analyze();
    watch.Stop();
    watch.Print();

    return 0;
}
