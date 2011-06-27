/*
 * CrossSections.cpp
 *
 *  Created on: 13 Aug 2010
 *      Author: kreczko
 */

#include "../interface/EventWeightProvider.h"
#include "TFile.h"
#include <boost/scoped_ptr.hpp>
#include <iostream>

namespace BAT {

boost::array<float, DataType::NUMBER_OF_DATA_TYPES> sevenTeV::getXSections() {
    boost::array<float, DataType::NUMBER_OF_DATA_TYPES> xsection;
    //from https://twiki.cern.ch/twiki/bin/view/CMS/CrossSections_3XSeries
    xsection[DataType::DATA] = 0;
    xsection[DataType::ttbar] = 157.5;
    xsection[DataType::Zjets] = 3048.;
    xsection[DataType::Wjets] = 31314.;
    xsection[DataType::WToENu] = 7899.;

    xsection[DataType::QCD_EMEnriched_Pt20to30] = 0.2355e9 * 0.0073;//xs 0.2355 mb (filter efficiency=0.0073)
    xsection[DataType::QCD_EMEnriched_Pt30to80] = 0.0593e9 * 0.059; //xs 0.0593 mb
    xsection[DataType::QCD_EMEnriched_Pt80to170] = 0.906e6 * 0.148; //xs 0.906e-3 mb

    xsection[DataType::QCD_BCtoE_Pt20to30] = 0.2355e9 * 0.00046; //xs 0.2355 mb (filter efficiency=0.00046)
    xsection[DataType::QCD_BCtoE_Pt30to80] = 0.0593e9 * 0.00234; //xs 0.0593 mb
    xsection[DataType::QCD_BCtoE_Pt80to170] = 0.906e6 * 0.0104; //xs 0.906e-3 mb

    xsection[DataType::PhotonJets_Pt40to100] = 23620.; //pb
    xsection[DataType::PhotonJets_Pt100to200] = 3476.; //pb
    xsection[DataType::PhotonJets_Pt200toInf] = 485.; //pb

    xsection[DataType::WWtoAnything] = 43.; //pb
    xsection[DataType::WZtoAnything] = 18.; //pb
    xsection[DataType::ZZtoAnything] = 5.9; //pb

    xsection[DataType::singleTop_And_W] = 10.6; //xs  11 pb (NLO MCFM) inclusive t,W decay
    xsection[DataType::singleTopTChannel] = 21.53;
    xsection[DataType::singleTopSChannel] = 1.40; //=4.21/3 15Jul

    xsection[DataType::VQQ] = 36.;
    xsection[DataType::Zprime_M500GeV_W5GeV] = 50;
    xsection[DataType::Zprime_M500GeV_W50GeV] = 50;
    xsection[DataType::Zprime_M750GeV_W7500MeV] = 50;
    xsection[DataType::Zprime_M1TeV_W10GeV] = 50;
    xsection[DataType::Zprime_M1TeV_W100GeV] = 50;
    xsection[DataType::Zprime_M1250GeV_W12500MeV] = 50;
    xsection[DataType::Zprime_M1500GeV_W15GeV] = 50;
    xsection[DataType::Zprime_M1500GeV_W150GeV] = 50;
    xsection[DataType::Zprime_M2TeV_W20GeV] = 50;
    xsection[DataType::Zprime_M2TeV_W200GeV] = 50;
    xsection[DataType::Zprime_M3TeV_W30GeV] = 50;
    xsection[DataType::Zprime_M3TeV_W300GeV] = 50;
    xsection[DataType::Zprime_M4TeV_W40GeV] = 50;
    xsection[DataType::Zprime_M4TeV_W400GeV] = 50;
    return xsection;
}

EventWeightProvider::EventWeightProvider(float lumiInInversePb, unsigned short tev, std::string pileUpEstimationFile) :
    lumiInInversePb(lumiInInversePb),
    tev(tev),
    useSkimEff(true),
    xsection(),
    numberOfProcessedEvents(),
//    numberOfSkimmedEvents(),
    estimatedPileUp(getPileUpHistogram(pileUpEstimationFile)),
    pileUpWeights(),
    numberOfEventsWithTooHighPileUp(0){
    generate_flat10_weights();
    if (tev == 7)
        xsection = sevenTeV::getXSections();
    defineNumberOfProducedEvents();
//    defineNumberOfSkimmedEvents();
}

void EventWeightProvider::defineNumberOfProducedEvents() {
    numberOfProcessedEvents[DataType::DATA] = 0;
    //Summer 11 !
//    numberOfProducedEvents[DataType::ttbar] = 1089625;


    //Spring 11
    numberOfProcessedEvents[DataType::ttbar] = 1286491;
    numberOfProcessedEvents[DataType::Zjets] = 1948290;
    //hotfix for missing stats!
    numberOfProcessedEvents[DataType::Wjets] = 14023626;//14805546;

    numberOfProcessedEvents[DataType::WToENu] = 5334220;

    numberOfProcessedEvents[DataType::QCD_EMEnriched_Pt20to30] = 37169939;
    numberOfProcessedEvents[DataType::QCD_EMEnriched_Pt30to80] = 32593127;
    numberOfProcessedEvents[DataType::QCD_EMEnriched_Pt80to170] = 5546413;

    numberOfProcessedEvents[DataType::QCD_BCtoE_Pt20to30] = 2243439;
    numberOfProcessedEvents[DataType::QCD_BCtoE_Pt30to80] = 1995502;
    numberOfProcessedEvents[DataType::QCD_BCtoE_Pt80to170] = 1043390;

    numberOfProcessedEvents[DataType::PhotonJets_Pt40to100] = 2217101;
    numberOfProcessedEvents[DataType::PhotonJets_Pt100to200] = 1065691;
    numberOfProcessedEvents[DataType::PhotonJets_Pt200toInf] = 1079950;

    numberOfProcessedEvents[DataType::WWtoAnything] = 2039440;
    numberOfProcessedEvents[DataType::WZtoAnything] = 2085696;
    numberOfProcessedEvents[DataType::ZZtoAnything] = 2108608;

    numberOfProcessedEvents[DataType::singleTop_And_W] = 489417;
    numberOfProcessedEvents[DataType::singleTopTChannel] = 484060;
    numberOfProcessedEvents[DataType::singleTopSChannel] = 494967;
    numberOfProcessedEvents[DataType::VQQ] = 720613;
    numberOfProcessedEvents[DataType::Zprime_M500GeV_W5GeV] = 227068;
    numberOfProcessedEvents[DataType::Zprime_M500GeV_W50GeV] = 238963;
    numberOfProcessedEvents[DataType::Zprime_M750GeV_W7500MeV] = 204819;
    numberOfProcessedEvents[DataType::Zprime_M1TeV_W10GeV] = 213384;
    numberOfProcessedEvents[DataType::Zprime_M1TeV_W100GeV] = 200387;
    numberOfProcessedEvents[DataType::Zprime_M1250GeV_W12500MeV] = 233361;
    numberOfProcessedEvents[DataType::Zprime_M1500GeV_W15GeV] = 193779;
    numberOfProcessedEvents[DataType::Zprime_M1500GeV_W150GeV] = 199121;
    numberOfProcessedEvents[DataType::Zprime_M2TeV_W20GeV] = 238752;
    numberOfProcessedEvents[DataType::Zprime_M2TeV_W200GeV] = 213363;
    numberOfProcessedEvents[DataType::Zprime_M3TeV_W30GeV] = 205270;
    numberOfProcessedEvents[DataType::Zprime_M3TeV_W300GeV] = 229034;
    numberOfProcessedEvents[DataType::Zprime_M4TeV_W40GeV] = 183920;
    numberOfProcessedEvents[DataType::Zprime_M4TeV_W400GeV] = 238142;
}

//void EventWeightProvider::defineNumberOfSkimmedEvents() {
//    numberOfSkimmedEvents[DataType::DATA] = 0;
//    numberOfSkimmedEvents[DataType::ttbar] = 642707.;
//    numberOfSkimmedEvents[DataType::Zjets] = 329061.;
//    numberOfSkimmedEvents[DataType::Wjets] = 1101679.;
//
//    numberOfSkimmedEvents[DataType::QCD_EMEnriched_Pt20to30] = 917274.;
//    numberOfSkimmedEvents[DataType::QCD_EMEnriched_Pt30to80] = 3639776.;
//    numberOfSkimmedEvents[DataType::QCD_EMEnriched_Pt80to170] = 1253958.;
//
//    numberOfSkimmedEvents[DataType::QCD_BCtoE_Pt20to30] = 86392.;
//    numberOfSkimmedEvents[DataType::QCD_BCtoE_Pt30to80] = 441537.;
//    numberOfSkimmedEvents[DataType::QCD_BCtoE_Pt80to170] = 477762.;
//
//    numberOfSkimmedEvents[DataType::PhotonJets_Pt40to100] = 311575;
//    numberOfSkimmedEvents[DataType::PhotonJets_Pt100to200] = 241590;
//    numberOfSkimmedEvents[DataType::PhotonJets_Pt200toInf] = 388407;
//
//    numberOfSkimmedEvents[DataType::singleTop_And_W] = 0;
//    numberOfSkimmedEvents[DataType::singleTopTChannel] = 0;
//    numberOfSkimmedEvents[DataType::singleTopSChannel] = 0;
//    numberOfSkimmedEvents[DataType::VQQ] = 36.;
//    numberOfSkimmedEvents[DataType::Zprime_M500GeV_W5GeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M500GeV_W50GeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M750GeV_W7500MeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M1TeV_W10GeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M1TeV_W100GeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M1250GeV_W12500MeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M1500GeV_W15GeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M1500GeV_W150GeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M2TeV_W20GeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M2TeV_W200GeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M3TeV_W30GeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M3TeV_W300GeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M4TeV_W40GeV] = 50;
//    numberOfSkimmedEvents[DataType::Zprime_M4TeV_W400GeV] = 50;
//}

EventWeightProvider::~EventWeightProvider() {

}

//void EventWeightProvider::useSkimEfficiency(bool use) {
//    useSkimEff = use;
//}

//float EventWeightProvider::getExpectedNumberOfEvents(DataType::value type) {
//    if (useSkimEff)
//        return xsection[type] * lumiInInversePb * numberOfSkimmedEvents[type] / numberOfProcessedEvents[type];
//    else
//        return xsection[type] * lumiInInversePb;
//}

float EventWeightProvider::getWeight(DataType::value type) {
    if (type == DataType::DATA)
        return 1.;
    else
        return xsection[type] * lumiInInversePb / numberOfProcessedEvents[type];
}

float EventWeightProvider::reweightPileUp(unsigned int numberOfVertices){
    if(numberOfVertices >= pileUpWeights.size()){
        ++numberOfEventsWithTooHighPileUp;
        return 1.;
    }

    return pileUpWeights.at(numberOfVertices);
}

boost::shared_ptr<TH1D> EventWeightProvider::getPileUpHistogram(std::string pileUpEstimationFile){
    std::cout << "Using pile-up estimation file " << pileUpEstimationFile << std::endl;
    boost::scoped_ptr<TFile> file(TFile::Open(pileUpEstimationFile.c_str()));
    boost::shared_ptr<TH1D> pileUp((TH1D*) file->Get("pileup")->Clone());
    file->Close();
    return pileUp;
}

void EventWeightProvider::generate_flat10_weights(){
    // see SimGeneral/MixingModule/python/mix_E7TeV_FlatDist10_2011EarlyData_inTimeOnly_cfi.py; copy and paste from there:
    const boost::array<double, 25> npu_probs = {{0.0698146584, 0.0698146584, 0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584 /* <-- 10*/,
           0.0630151648,0.0526654164,0.0402754482,0.0292988928,0.0194384503,0.0122016783,0.007207042,0.004003637,0.0020278322,
           0.0010739954,0.0004595759,0.0002229748,0.0001028162,4.58337152809607E-05 /* <-- 24 */}};
    double s = 0.0;
    for (unsigned int npu = 0; npu < npu_probs.size(); ++npu) {
        double npu_estimated = estimatedPileUp->GetBinContent(estimatedPileUp->GetXaxis()->FindBin(npu));
        pileUpWeights[npu] = npu_estimated / npu_probs[npu];
        s += npu_estimated;
    }
    // normalize weights such that the total sum of weights over thw whole sample is 1.0, i.e., sum_i  result[i] * npu_probs[i] should be 1.0 (!)
    for (unsigned int npu = 0; npu < pileUpWeights.size(); ++npu) {
        pileUpWeights[npu] /= s;
    }
}

unsigned long EventWeightProvider::getNumberOfEventsWithTooHighPileUp() const{
    return numberOfEventsWithTooHighPileUp;
}

} // namespace BAT
