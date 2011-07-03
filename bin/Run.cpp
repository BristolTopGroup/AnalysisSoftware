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
    Analysis::luminosity = 976.;
    Analysis::useCustomConversionTagger(false);
    Analysis::usePFIsolation(true);

    boost::scoped_ptr<Analysis> myAnalysis(new Analysis());
   //         myAnalysis->setMaximalNumberOfEvents(100000);
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

    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/data/ElectronHad/nTuple_v2b_Run2011-May10ReReco_GoldenJSON/db3f92ba514324d173b9a1664acdc31b/*.root");//203.815529928 pb-1
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/data/ElectronHad/nTuple_v2b_Run2011-PromptReco_GoldenJSON_exclRereco/db3f92ba514324d173b9a1664acdc31b/*.root");//294.450534905 pb-1
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/data/ElectronHad/nTuple_v2b_Run2011-PromptReco_GoldenJSON_10.06.11-17.06.11/c43a0fd1e74060a8f9608df5f5bafba0/*.root");//216.429549292 pb-1
    myAnalysis->addInputFile(
                "/storage/TopQuarkGroup/data/ElectronHad/nTuple_v2b_Run2011-PromptReco_GoldenJSON_17.06.11-24.06.11/4ee1203e97f9a00957561f563636708a/*.root");//166 pb-1
    myAnalysis->addInputFile(
                    "/storage/TopQuarkGroup/data/ElectronHad/nTuple_v2b_Run2011-PromptReco_GoldenJSON_24.06.11-01.07.11/4ee1203e97f9a00957561f563636708a/*.root");//94 pb-1

    //==========================Summer11 samples========================================

    myAnalysis->addInputFile("/storage/TopQuarkGroup/mc/TTJets_TuneZ2_7TeV-madgraph-tauola/nTuple_v2_Summer11-PU_S4_START42_V11-v1/7c548abbf04de779162e4a2cbdd09438/*.root");
    myAnalysis->addInputFile("/storage/TopQuarkGroup/mc/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/nTuple_v2c_Summer11-PU_S4_START42_V11-v1/3f9a3c05fc482d55208abea66464af16/*.root");
    myAnalysis->addInputFile("/storage/TopQuarkGroup/mc/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/nTuple_v2_Summer11-PU_S4_START42_V11-v1/3f9a3c05fc482d55208abea66464af16/*.root");

    //BCToE
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v2_Summer11-PU_S3_START42_V11-v2/ad7cf59d8abe76df5690594944b778cc/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/nTuple_v2_Summer11-PU_S4_START42_V11-v1/ad7cf59d8abe76df5690594944b778cc/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia/nTuple_v2_Summer11-PU_S4_START42_V11-v1/ad7cf59d8abe76df5690594944b778cc/*.root");

    //EMEnriched
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v2_Summer11-PU_S4_START42_V11-v1/ad7cf59d8abe76df5690594944b778cc/*.root");
    myAnalysis->addInputFile(
                "/storage/TopQuarkGroup/mc/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia/nTuple_v2_Summer11-PU_S4_START42_V11-v1/7c548abbf04de779162e4a2cbdd09438/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/nTuple_v2_Summer11-PU_S4_START42_V11-v1/ad7cf59d8abe76df5690594944b778cc/*.root");

//
//    //==========================Spring11 samples========================================
    //SM ttbar + jets
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/TTJets_TuneD6T_7TeV-madgraph-tauola/nTuple_v2b_Spring11-PU_S1_-START311_V1G1-v1/35b1f92254c19716429c19a0cca8c117/*.root");
//    // W+jets
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/WJetsToLNu_TuneD6T_7TeV-madgraph-tauola/nTuple_v2b_Spring11-PU_S1_-START311_V1G1-v1/6f7af515f09d5fa510155a389164eaef/*.root");
    //Z/gamma + jets
//    myAnalysis->addInputFile(
//                "/storage/TopQuarkGroup/mc/DYJetsToLL_TuneD6T_M-50_7TeV-madgraph-tauola/nTuple_v2c_Spring11-PU_S1_-START311_V1G1-v1/6f7af515f09d5fa510155a389164eaef/*.root");

    //SingleTop
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/TToBLNu_TuneZ2_s-channel_7TeV-madgraph/nTuple_v2b_Spring11-PU_S1_-START311_V1G1-v1/d2d68e81009e0568f462b5af5134933a/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/nTuple_v2c_Spring11-PU_S1_-START311_V1G1-v1/35b1f92254c19716429c19a0cca8c117/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph/nTuple_v2c_Spring11-PU_S1_-START311_V1G1-v1/35b1f92254c19716429c19a0cca8c117/*.root");
//
//    //Di-Boson
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/WWtoAnything_TuneZ2_7TeV-pythia6-tauola/nTuple_v2c_Spring11-PU_S1_-START311_V1G1-v1/35b1f92254c19716429c19a0cca8c117/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/nTuple_v2c_Spring11-PU_S1_-START311_V1G1-v1/35b1f92254c19716429c19a0cca8c117/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/ZZtoAnything_TuneZ2_7TeV-pythia6-tauola/nTuple_v2c_Spring11-PU_S1_-START311_V1G1-v1/35b1f92254c19716429c19a0cca8c117/*.root");
//
//    //Photon + jets
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/GJets_TuneD6T_HT-40To100_7TeV-madgraph/nTuple_v2c_Spring11-PU_S1_-START311_V1G1-v1/35b1f92254c19716429c19a0cca8c117/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/GJets_TuneD6T_HT-100To200_7TeV-madgraph/nTuple_v2c_Spring11-PU_S1_-START311_V1G1-v1/35b1f92254c19716429c19a0cca8c117/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/GJets_TuneD6T_HT-200_7TeV-madgraph/nTuple_v2c_Spring11-PU_S1_-START311_V1G1-v1/6f7af515f09d5fa510155a389164eaef/*.root");

    cout << "starting analysis" << endl;
    myAnalysis->analyze();
    watch.Stop();
    watch.Print();

    return 0;
}
