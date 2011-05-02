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
    Analysis::luminosity = 78.436067238;

    Analysis::useCustomConversionTagger(false);
    Analysis::usePFIsolation(true);

    boost::scoped_ptr<Analysis> myAnalysis(new Analysis());
//        myAnalysis->setMaximalNumberOfEvents(300000);
    myAnalysis->setUsedNeutrinoSelectionForTopPairReconstruction(NeutrinoSelectionCriterion::chi2);

    //Test samples
    //    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/test*.root");
    //DATA
    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/Run2010A-Dec22ReReco_v1/*.root");//3.06 pb-1
    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/Run2010B-Dec22ReReco_v1/*.root");//32.96 pb-1
    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/Run2011A-PromptReco-v1/*.root");//4.88 pb-1
    myAnalysis->addInputFile("/storage/TopQuarkGroup/data/Run2011A-PromptReco-v2/*.root");//37.53 pb-1

    //FALL 10 samples
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/Spring11/TTJets_TuneD6T_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/*.root");

    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/Spring11/TToBLNu_TuneZ2_t-channel_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/*.root");
    myAnalysis->addInputFile(
                "/storage/TopQuarkGroup/mc/Spring11/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/*.root");
    myAnalysis->addInputFile(
                "/storage/TopQuarkGroup/mc/Spring11/TToBLNu_TuneZ2_s-channel_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/*.root");

    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/Spring11/WJetsToLNu_TuneD6T_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/Spring11/DYJetsToLL_TuneD6T_M-50_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/*.root");
    //QCD BC to E
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/Spring11/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6_Spring11-PU_S1_START311_V1G1-v1/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/Spring11/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6_Spring11-PU_S1_START311_V1G1-v1/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/Spring11/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6_Spring11-PU_S1_START311_V1G1-v1/*.root");
//    //QCD EM Enriched
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/Spring11/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6_Spring11-PU_S1_START311_V1G1-v1/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/Spring11/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6_Spring11-PU_S1_START311_V1G1-v1/*.root");
    myAnalysis->addInputFile(
            "/storage/TopQuarkGroup/mc/Spring11/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6_Spring11-PU_S1_START311_V1G1-v1/*.root");

//    //QCD Photon + Jets
        myAnalysis->addInputFile(
                "/storage/TopQuarkGroup/mc/Spring11/GJets_TuneD6T_HT-40To100_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/*.root");
        myAnalysis->addInputFile(
                "/storage/TopQuarkGroup/mc/Spring11/GJets_TuneD6T_HT-100To200_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/*.root");
        myAnalysis->addInputFile(
                "/storage/TopQuarkGroup/mc/Spring11/GJets_TuneD6T_HT-200_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/*.root");
//
//    // Z' -> ttbar samples
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M500GeV_W5GeV-madgraph_Fall10-START38_V12-v1/*.root");
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M500GeV_W50GeV-madgraph_Fall10-START38_V12-v1/*.root");
//
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M750GeV_W7500MeV-madgraph_Fall10-START38_V12-v1/*.root");
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M1250GeV_W1250MeV-madgraph_Fall10-START38_V12-v1/*.root");
//
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M1500GeV_W15GeV-madgraph_Fall10-START38_V12-v1/*.root");
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M1500GeV_W150GeV-madgraph_Fall10-START38_V12-v1/*.root");
//
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M1000GeV_W10GeV-madgraph_Fall10-START38_V12-v1/*.root");
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M1000GeV_W100GeV-madgraph_Fall10-START38_V12-v1/*.root");
//
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M2000GeV_W20GeV-madgraph_Fall10-START38_V12-v1/*.root");
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M2000GeV_W200GeV-madgraph_Fall10-START38_V12-v1/*.root");
//
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M3000GeV_W30GeV-madgraph_Fall10-START38_V12-v1/*.root");
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M3000GeV_W300GeV-madgraph_Fall10-START38_V12-v1/*.root");
//
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M4000GeV_W40GeV-madgraph_Fall10-START38_V12-v1/*.root");
//    myAnalysis->addInputFile(
//            "/storage/TopQuarkGroup/mc/Spring11/Zprime_M4000GeV_W400GeV-madgraph_Fall10-START38_V12-v1/*.root");

    cout << "starting analysis" << endl;
    myAnalysis->analyze();
    watch.Stop();
    watch.Print();

    return 0;
}
