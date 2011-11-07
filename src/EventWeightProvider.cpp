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
    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
    xsection[DataType::ElectronHad] = 0;
    xsection[DataType::MuHad] = 0;
    xsection[DataType::ttbar] = 157.5;
    xsection[DataType::Zjets] = 3048.;//m(ll)>50GeV
    xsection[DataType::Wjets] = 31314.;
//    xsection[DataType::WToENu] = 7899.;

    xsection[DataType::QCD_EMEnriched_Pt20to30] = 0.2355e9 * 0.0073;//xs 0.2355 mb (filter efficiency=0.0073)
    xsection[DataType::QCD_EMEnriched_Pt30to80] = 0.0593e9 * 0.059; //xs 0.0593 mb
    xsection[DataType::QCD_EMEnriched_Pt80to170] = 0.906e6 * 0.148; //xs 0.906e-3 mb

    xsection[DataType::QCD_BCtoE_Pt20to30] = 0.2355e9 * 0.00046; //xs 0.2355 mb (filter efficiency=0.00046)
    xsection[DataType::QCD_BCtoE_Pt30to80] = 0.0593e9 * 0.00234; //xs 0.0593 mb
    xsection[DataType::QCD_BCtoE_Pt80to170] = 0.906e6 * 0.0104; //xs 0.906e-3 mb

    xsection[DataType::PhotonJets_Pt40to100] = 23620.; //pb
    xsection[DataType::PhotonJets_Pt100to200] = 3476.; //pb
    xsection[DataType::PhotonJets_Pt200toInf] = 485.; //pb

    xsection[DataType::WWtoAnything] = 43.; //pb +-1.5
    xsection[DataType::WZtoAnything] = 18.2; //pb +-0.7pb
    xsection[DataType::ZZtoAnything] = 5.9; //pb +- 0.15pb

    xsection[DataType::singleTop_And_W] = 5.3; //pb
    xsection[DataType::singleTopTChannel] = 42.6; //pb
    xsection[DataType::singleTopSChannel] = 2.72; //pb

    xsection[DataType::singleAntiTop_And_W] = 5.3; //pb
	xsection[DataType::singleAntiTopTChannel] = 22.0; //pb
	xsection[DataType::singleAntiTopSChannel] = 1.49; //pb

    xsection[DataType::VQQ] = 35.3;
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
    numberOfProcessedEvents[DataType::ElectronHad] = 0;
    numberOfProcessedEvents[DataType::MuHad] = 0;

    numberOfProcessedEvents[DataType::ttbar] = 3673321;//3673321;
    numberOfProcessedEvents[DataType::Zjets] = 33645364;//36277961;
    numberOfProcessedEvents[DataType::Wjets] = 78982439;

    numberOfProcessedEvents[DataType::QCD_EMEnriched_Pt20to30] = 35729669;
    numberOfProcessedEvents[DataType::QCD_EMEnriched_Pt30to80] = 70392060;
    numberOfProcessedEvents[DataType::QCD_EMEnriched_Pt80to170] = 8090132;

    numberOfProcessedEvents[DataType::QCD_BCtoE_Pt20to30] = 2081560;
    numberOfProcessedEvents[DataType::QCD_BCtoE_Pt30to80] = 2030033;
    numberOfProcessedEvents[DataType::QCD_BCtoE_Pt80to170] = 1082691;

    numberOfProcessedEvents[DataType::PhotonJets_Pt40to100] = 2217101;
    numberOfProcessedEvents[DataType::PhotonJets_Pt100to200] = 1508384;
    numberOfProcessedEvents[DataType::PhotonJets_Pt200toInf] = 8339186;

    numberOfProcessedEvents[DataType::WWtoAnything] = 3490064;
    numberOfProcessedEvents[DataType::WZtoAnything] = 2368019;
    numberOfProcessedEvents[DataType::ZZtoAnything] = 3145780;

    numberOfProcessedEvents[DataType::singleTop_And_W] = 556519;
	numberOfProcessedEvents[DataType::singleTopTChannel] = 2867419;
	numberOfProcessedEvents[DataType::singleTopSChannel] = 259971;

	numberOfProcessedEvents[DataType::singleAntiTop_And_W] = 375192;
	numberOfProcessedEvents[DataType::singleAntiTopTChannel] = 1944826;
	numberOfProcessedEvents[DataType::singleAntiTopSChannel] = 137980;

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

EventWeightProvider::~EventWeightProvider() {

}

float EventWeightProvider::getWeight(DataType::value type) {
    if (type == DataType::ElectronHad || type == DataType::MuHad)
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

void EventWeightProvider::generate_flat10_weights() {
//	/https://twiki.cern.ch/twiki/bin/view/CMS/PileupInformation
	//https://twiki.cern.ch/twiki/bin/view/CMS/PileupMCReweightingUtilities
	// Flat10+Tail distribution taken directly from MixingModule input:  (Can be used for Spring11 and Summer11 if you don't worry about small shifts in the mean)
//    const boost::array<double, 25> npu_probs = {{0.0698146584, 0.0698146584, 0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584 /* <-- 10*/,
//           0.0630151648,0.0526654164,0.0402754482,0.0292988928,0.0194384503,0.0122016783,0.007207042,0.004003637,0.0020278322,
//           0.0010739954,0.0004595759,0.0002229748,0.0001028162,4.58337152809607E-05 /* <-- 24 */}};

	// Summer11 PU_S4, distribution obtained by averaging the number of interactions
	// in each beam crossing to estimate the true mean.  THIS IS THE RECOMMENDED ONE for reweighting.
	const boost::array<double, 25> npu_probs =
			{ { 0.104109, 0.0703573, 0.0698445, 0.0698254, 0.0697054, 0.0697907, 0.0696751, 0.0694486, 0.0680332,
					0.0651044, 0.0598036, 0.0527395, 0.0439513, 0.0352202, 0.0266714, 0.019411, 0.0133974, 0.00898536,
					0.0057516, 0.00351493, 0.00212087, 0.00122891, 0.00070592, 0.000384744, 0.000219377 } };
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
