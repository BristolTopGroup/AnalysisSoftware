/**
 * CrossSections.h
 *
 *  Created on: 29 Jul 2010
 *      Author: kreczko
 *      Adaptation of
 *      PhysicsTools/Utilities/interface/LumiReWeighting.h
 *      PhysicsTools/Utilities/interface/Lumi3DReWeighting.h
 *
 */

#ifndef EVENTWEIGHTPROVIDER_H_
#define EVENTWEIGHTPROVIDER_H_
#include "DataTypes.h"
#include "Python/DatasetInformation.h"

#include <vector>

#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

#include "TH1D.h"

#define NWEIGHTSSIZE 60

namespace BAT {

namespace PileUpReweightingMethod {
enum value {
	averagePileUp, //
	inTimePileUpOnly, //
	threeDReweighting
};
}
}
#include "Event.h"
namespace BAT{


class EventWeightProvider {
private:
	DatasetInformation datasetInfo_;
//	float lumiInInversePb;
//	unsigned short tev;
//	bool useSkimEff;
	boost::array<double, DataType::NUMBER_OF_DATA_TYPES> xsection;
	boost::array<unsigned long, DataType::NUMBER_OF_DATA_TYPES> numberOfProcessedEvents;
	boost::shared_ptr<TH1D> estimatedPileUp, estimatedPileUp_up, estimatedPileUp_down;
	boost::array<double, NWEIGHTSSIZE> DATAdistribution;
	boost::array<double, NWEIGHTSSIZE> pileUpWeights, pileUpWeights_up, pileUpWeights_down;
	unsigned long numberOfEventsWithTooHighPileUp;
	void defineNumberOfProducedEvents();
public:

	EventWeightProvider(std::string datasetInformationFile);
	EventWeightProvider(std::string datasetInformationFile, boost::shared_ptr<TH1D>,boost::shared_ptr<TH1D>);
	EventWeightProvider(std::string datasetInformationFile, std::vector<double>, std::vector<double>);
	~EventWeightProvider();
	double getWeight(const EventPtr) const;
	double getPUWeight(const EventPtr) const;

	double reweightTopPt(const EventPtr);
	//@deprecated
	double getWeight(DataType::value type);
	//@deprecated
	double reweightPileUp(unsigned int nVertices, int systematic = 0);
//	boost::shared_ptr<TH1D> getPileUpHistogram(std::string pileUpEstimationFile);
	void generate_weights();
	boost::array<double, NWEIGHTSSIZE> generateWeights(const boost::array<double, NWEIGHTSSIZE>, const boost::shared_ptr<TH1D>);
	unsigned long getNumberOfEventsWithTooHighPileUp() const;

};

typedef boost::shared_ptr<EventWeightProvider> EventWeightProviderPtr;

const boost::array<double, NWEIGHTSSIZE> Spring2016 = { {
		0.000829312873542,
 		0.00124276120498,
 		0.00339329181587,
 		0.00408224735376,
 		0.00383036590008,
		0.00659159288946,
 		0.00816022734493,
 		0.00943640833116,
 		0.0137777376066,
 		0.017059392038,
 		0.0213193035468,
 		0.0247343174676,
 		0.0280848773878,
 		0.0323308476564,
 		0.0370394341409,
 		0.0456917721191,
 		0.0558762890594,
 		0.0576956187107,
 		0.0625325287017,
 		0.0591603758776,
 		0.0656650815128,
 		0.0678329011676,
 		0.0625142146389,
 		0.0548068448797,
 		0.0503893295063,
 		0.040209818868,
 		0.0374446988111,
 		0.0299661572042,
 		0.0272024759921,
 		0.0219328403791,
 		0.0179586571619,
 		0.0142926728247,
 		0.00839941654725,
 		0.00522366397213,
 		0.00224457976761,
 		0.000779274977993,
 		0.000197066585944,
 		7.16031761328e-05,
 		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
 		0.0,
 		0.0,
		0.0
} };

const boost::array<double, NWEIGHTSSIZE> Fall2015_25ns = { {
    0.000108643,
    0.000388957,
    0.000332882,
    0.00038397,
    0.000549167,
    0.00105412,
    0.00459007,
    0.0210314,
    0.0573688,
    0.103986,
    0.142369,
    0.157729,
    0.147685,
    0.121027,
    0.08855,
    0.0582866,
    0.0348526,
    0.019457,
    0.0107907,
    0.00654313,
    0.00463195,
    0.00370927,
    0.0031137,
    0.00261141,
    0.00215499,
    0.00174491,
    0.00138268,
    0.00106731,
    0.000798828,
    0.00057785,
    0.00040336,
    0.00027161,
    0.000176535,
    0.00011092,
    6.75502e-05,
    4.00323e-05,
    2.32123e-05,
    1.32585e-05,
    7.51611e-06,
    4.25902e-06,
    2.42513e-06,
    1.39077e-06,
    8.02452e-07,
    4.64159e-07,
    2.67845e-07,
    1.5344e-07,
    8.68966e-08,
    4.84931e-08,
    2.6606e-08,
    1.433e-08
} };


const boost::array<double, NWEIGHTSSIZE> Startup2015_25ns = { {
	4.8551E-07,
    1.74806E-06,
    3.30868E-06,
    1.62972E-05,
    4.95667E-05,
    0.000606966,
    0.003307249,
    0.010340741,
    0.022852296,
    0.041948781,
    0.058609363,
    0.067475755,
    0.072817826,
    0.075931405,
    0.076782504,
    0.076202319,
    0.074502547,
    0.072355135,
    0.069642102,
    0.064920999,
    0.05725576,
    0.047289348,
    0.036528446,
    0.026376131,
    0.017806872,
    0.011249422,
    0.006643385,
    0.003662904,
    0.001899681,
    0.00095614,
    0.00050028,
    0.000297353,
    0.000208717,
    0.000165856,
    0.000139974,
    0.000120481,
    0.000103826,
    8.88868E-05,
    7.53323E-05,
    6.30863E-05,
    5.21356E-05,
    4.24754E-05,
    3.40876E-05,
    2.69282E-05,
    2.09267E-05,
    1.5989E-05,
    4.8551E-06,
    2.42755E-06,
    4.8551E-07,
    2.42755E-07,
    1.21378E-07,
    4.8551E-08
} };

// This array is generated by python get_PU_profiles.py
const boost::array<double, NWEIGHTSSIZE> Spring2015_50ns = { {
	1.1501310837e-05,
	0.00010256110545,
	0.000396904331865,
	0.00113204447553,
	0.00299984728917,
	0.00622522830963,
	0.0113030029461,
	0.0184802301228,
	0.0275372415781,
	0.0379406400025,
	0.0481193512678,
	0.0579271838069,
	0.0656016692519,
	0.0707077309489,
	0.0739287137985,
	0.0738715305924,
	0.0713002681732,
	0.0661827325821,
	0.0604616813362,
	0.0540139526129,
	0.0472621172667,
	0.0397371724248,
	0.0338714048266,
	0.0279997400939,
	0.0227178633213,
	0.0183092225343,
	0.0146730374545,
	0.0115087591112,
	0.00909056607634,
	0.00690457597375,
	0.00522021949291,
	0.00404522847384,
	0.00303545501083,
	0.00219072820619,
	0.00153157731984,
	0.00110369920731,
	0.000822269066703,
	0.000543183821719,
	0.000367429282051,
	0.000262808520347,
	0.000185305529158,
	0.000100168937934,
	9.50939866016e-05,
	6.29806090728e-05,
	3.32664749294e-05,
	3.12346382998e-05,
	2.53321613854e-05,
	7.05499496689e-06,
	4.29647388955e-06,
	1.36816959184e-06,
	2.7877924822e-06,
	1.48733374772e-06,
	4.3370214371e-06,
	2.84449697574e-06,
	1.36816959184e-06,
	0.0,
	0.0,
	0.0,
	0.0,
	0.0} };
}

#endif /* EVENTWEIGHTPROVIDER_H_ */
