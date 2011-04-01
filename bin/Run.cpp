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
    Analysis::luminosity = 36.145;

    Analysis::useCustomConversionTagger(false);
    Analysis::usePFIsolation(true);

    boost::scoped_ptr<Analysis> myAnalysis(new Analysis());
//        myAnalysis->setMaximalNumberOfEvents(100);
    myAnalysis->setUsedNeutrinoSelectionForTopPairReconstruction(NeutrinoSelectionCriterion::chi2);
    myAnalysis->setUsedTTbarReconstructionCriterion(TTbarReconstructionCriterion::TopMassDifference);

    //Test samples
    //    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/test*.root");
    //DATA
    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/Nov4ReReco_JEC_Spring_V8_36.145pb_e25skim/Run2010A/*.root");
    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/Nov4ReReco_JEC_Spring_V8_36.145pb_e25skim/Run2010B/*.root");

    //FALL 10 samples
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/TTJets_TuneD6T_7TeV-madgraph-tauola_Fall10-START38_V12-v2/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/TToBLNu_TuneZ2_t-channel_7TeV-madgraph-tauola_Fall10-START38_V12-v2/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph-tauola_Fall10-START38_V12-v2/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/WJetsToLNu_TuneD6T_7TeV-madgraph-tauola_Fall10-START38_V12-v1/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/DYJetsToLL_TuneD6T_M-50_7TeV-madgraph-tauola_Fall10-START38_V12-v2/*.root");
    //QCD BC to E
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1/*.root");
//    //QCD EM Enriched
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1/*.root");

//    //QCD Photon + Jets
        myAnalysis->addInputFile(
                "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/GJets_TuneD6T_HT-40To100_7TeV-madgraph_Fall10-START38_V12-v1/*.root");
        myAnalysis->addInputFile(
                "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/GJets_TuneD6T_HT-100To200_7TeV-madgraph_Fall10-START38_V12-v1/*.root");
        myAnalysis->addInputFile(
                "/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/GJets_TuneD6T_HT-200_7TeV-madgraph_Fall10-START38_V12-v1/*.root");
    cout << "starting analysis" << endl;
    myAnalysis->analyze();
    watch.Stop();
    watch.Print();

    return 0;
}
