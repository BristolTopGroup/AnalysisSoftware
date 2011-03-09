/*
 * CrossSections.cpp
 *
 *  Created on: 13 Aug 2010
 *      Author: kreczko
 */

#include "../interface/CrossSections.h"

namespace BAT {
boost::array<float, DataType::NUMBER_OF_DATA_TYPES> sevenTeV::getXSections() {
    boost::array<float, DataType::NUMBER_OF_DATA_TYPES> xsection;
    xsection[DataType::DATA] = 0;
    xsection[DataType::ttbar] = 157.5;
    xsection[DataType::Zjets] = 3048.;
    xsection[DataType::Wjets] = 31314.;

    xsection[DataType::QCD_EMEnriched_Pt20to30] = 0.2355e9 * 0.0073;//xs 0.2355 mb (filter efficiency=0.0073)
    xsection[DataType::QCD_EMEnriched_Pt30to80] = 0.0593e9 * 0.059; //xs 0.0593 mb
    xsection[DataType::QCD_EMEnriched_Pt80to170] = 0.906e6 * 0.148; //xs 0.906e-3 mb

    xsection[DataType::QCD_BCtoE_Pt20to30] = 0.2355e9 * 0.00046; //xs 0.2355 mb (filter efficiency=0.00046)
    xsection[DataType::QCD_BCtoE_Pt30to80] = 0.0593e9 * 0.00234; //xs 0.0593 mb
    xsection[DataType::QCD_BCtoE_Pt80to170] = 0.906e6 * 0.0104; //xs 0.906e-3 mb

    xsection[DataType::PhotonJets_Pt40to100] = 23620.; //pb
    xsection[DataType::PhotonJets_Pt100to200] = 3476.; //pb
    xsection[DataType::PhotonJets_Pt200toInf] = 485.; //pb

    xsection[DataType::singleTop_And_W] = 10.6; //xs  11 pb (NLO MCFM) inclusive t,W decay
    xsection[DataType::singleTopTChannel] = 21.53; //=64.6/3 15Jul
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

boost::array<float, DataType::NUMBER_OF_DATA_TYPES> tenTeV::getXSections() {
    boost::array<float, DataType::NUMBER_OF_DATA_TYPES> xsection;
    xsection[DataType::DATA] = 0;
    xsection[DataType::ttbar] = 0;
    return xsection;
}

CrossSectionProvider::CrossSectionProvider(float lumiInInversePb, unsigned short tev) :
    lumiInInversePb(lumiInInversePb), tev(tev), useSkimEff(true), xsection(), numberOfProducedEvents(),
            numberOfSkimmedEvents() {
    if (tev == 7)
        xsection = sevenTeV::getXSections();
    else if (tev == 10)
        xsection = tenTeV::getXSections();
    defineNumberOfProducedEvents();
    defineNumberOfSkimmedEvents();
}

void CrossSectionProvider::defineNumberOfProducedEvents() {
    numberOfProducedEvents[DataType::DATA] = 0;
    numberOfProducedEvents[DataType::ttbar] = 1306182;
    numberOfProducedEvents[DataType::Zjets] = 2543727;
    numberOfProducedEvents[DataType::Wjets] = 14805546;

    numberOfProducedEvents[DataType::QCD_EMEnriched_Pt20to30] = 37169939;
    numberOfProducedEvents[DataType::QCD_EMEnriched_Pt30to80] = 71845473;
    numberOfProducedEvents[DataType::QCD_EMEnriched_Pt80to170] = 5546413;

    numberOfProducedEvents[DataType::QCD_BCtoE_Pt20to30] = 2243439;
    numberOfProducedEvents[DataType::QCD_BCtoE_Pt30to80] = 1995502;
    numberOfProducedEvents[DataType::QCD_BCtoE_Pt80to170] = 1043390;

    numberOfProducedEvents[DataType::PhotonJets_Pt40to100] = 2217101;
    numberOfProducedEvents[DataType::PhotonJets_Pt100to200] = 1065691;
    numberOfProducedEvents[DataType::PhotonJets_Pt200toInf] = 1142171;

    numberOfProducedEvents[DataType::singleTop_And_W] = 494961;
    numberOfProducedEvents[DataType::singleTopTChannel] = 484060;
    numberOfProducedEvents[DataType::singleTopSChannel] = 494967;
    numberOfProducedEvents[DataType::VQQ] = 720613;
    numberOfProducedEvents[DataType::Zprime_M500GeV_W5GeV] = 227068;
    numberOfProducedEvents[DataType::Zprime_M500GeV_W50GeV] = 238963;
    numberOfProducedEvents[DataType::Zprime_M750GeV_W7500MeV] = 204819;
    numberOfProducedEvents[DataType::Zprime_M1TeV_W10GeV] = 213384;
    numberOfProducedEvents[DataType::Zprime_M1TeV_W100GeV] = 200387;
    numberOfProducedEvents[DataType::Zprime_M1250GeV_W12500MeV] = 233361;
    numberOfProducedEvents[DataType::Zprime_M1500GeV_W15GeV] = 193779;
    numberOfProducedEvents[DataType::Zprime_M1500GeV_W150GeV] = 199121;
    numberOfProducedEvents[DataType::Zprime_M2TeV_W20GeV] = 238752;
    numberOfProducedEvents[DataType::Zprime_M2TeV_W200GeV] = 213363;
    numberOfProducedEvents[DataType::Zprime_M3TeV_W30GeV] = 205270;
    numberOfProducedEvents[DataType::Zprime_M3TeV_W300GeV] = 229034;
    numberOfProducedEvents[DataType::Zprime_M4TeV_W40GeV] = 183920;
    numberOfProducedEvents[DataType::Zprime_M4TeV_W400GeV] = 238142;
}

void CrossSectionProvider::defineNumberOfSkimmedEvents() {
    numberOfSkimmedEvents[DataType::DATA] = 0;
    numberOfSkimmedEvents[DataType::ttbar] = 642707.;
    numberOfSkimmedEvents[DataType::Zjets] = 329061.;
    numberOfSkimmedEvents[DataType::Wjets] = 1101679.;

    numberOfSkimmedEvents[DataType::QCD_EMEnriched_Pt20to30] = 917274.;
    numberOfSkimmedEvents[DataType::QCD_EMEnriched_Pt30to80] = 3639776.;
    numberOfSkimmedEvents[DataType::QCD_EMEnriched_Pt80to170] = 1253958.;

    numberOfSkimmedEvents[DataType::QCD_BCtoE_Pt20to30] = 86392.;
    numberOfSkimmedEvents[DataType::QCD_BCtoE_Pt30to80] = 441537.;
    numberOfSkimmedEvents[DataType::QCD_BCtoE_Pt80to170] = 477762.;

    numberOfSkimmedEvents[DataType::PhotonJets_Pt40to100] = 311575;
    numberOfSkimmedEvents[DataType::PhotonJets_Pt100to200] = 241590;
    numberOfSkimmedEvents[DataType::PhotonJets_Pt200toInf] = 388407;

    numberOfSkimmedEvents[DataType::singleTop_And_W] = 0;
    numberOfSkimmedEvents[DataType::singleTopTChannel] = 0;
    numberOfSkimmedEvents[DataType::singleTopSChannel] = 0;
    numberOfSkimmedEvents[DataType::VQQ] = 36.;
    numberOfSkimmedEvents[DataType::Zprime_M500GeV_W5GeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M500GeV_W50GeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M750GeV_W7500MeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M1TeV_W10GeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M1TeV_W100GeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M1250GeV_W12500MeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M1500GeV_W15GeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M1500GeV_W150GeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M2TeV_W20GeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M2TeV_W200GeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M3TeV_W30GeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M3TeV_W300GeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M4TeV_W40GeV] = 50;
    numberOfSkimmedEvents[DataType::Zprime_M4TeV_W400GeV] = 50;
    //    pj1: 311575. / 2255228.
    //  pj2: 241590. / 1071393.
    //pj3: 338407. / 960198.


}

CrossSectionProvider::~CrossSectionProvider() {

}

void CrossSectionProvider::useSkimEfficiency(bool use) {
    useSkimEff = use;
}

float CrossSectionProvider::getExpectedNumberOfEvents(DataType::value type) {
    if (useSkimEff)
        return xsection[type] * lumiInInversePb * numberOfSkimmedEvents[type] / numberOfProducedEvents[type];
    else
        return xsection[type] * lumiInInversePb;
}

float CrossSectionProvider::getWeight(DataType::value type) {
    if (type == DataType::DATA)
        return 1.;
    else
        return xsection[type] * lumiInInversePb / numberOfProducedEvents[type];
}

} // namespace BAT
