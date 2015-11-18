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
/*
 * Summer11 PU_S3 and PU_S4, distribution obtained by only looking at the in-time crossing.
 * This is the "spike+smear" distribution, RECOMMENDED FOR REWEIGHTING.
 */

const boost::array<double, NWEIGHTSSIZE> PoissonOneXDist = { { 1.45346E-01, 6.42802E-02, 6.95255E-02, 6.96747E-02,
		6.92955E-02, 6.84997E-02, 6.69528E-02, 6.45515E-02, 6.09865E-02, 5.63323E-02, 5.07322E-02, 4.44681E-02,
		3.79205E-02, 3.15131E-02, 2.54220E-02, 2.00184E-02, 1.53776E-02, 1.15387E-02, 8.47608E-03, 6.08715E-03,
		4.28255E-03, 2.97185E-03, 2.01918E-03, 1.34490E-03, 8.81587E-04, 5.69954E-04, 3.61493E-04, 2.28692E-04,
		1.40791E-04, 8.44606E-05, 5.10204E-05, 3.07802E-05, 1.81401E-05, 1.00201E-05, 5.80004E-06, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } };

// Summer11 PU_S4, distribution obtained by averaging the number of interactions
// in each beam crossing to estimate the true mean.

const boost::array<double, NWEIGHTSSIZE> PoissonIntDist = { { 0.104109, 0.0703573, 0.0698445, 0.0698254, 0.0697054,
		0.0697907, 0.0696751, 0.0694486, 0.0680332, 0.0651044, 0.0598036, 0.0527395, 0.0439513, 0.0352202, 0.0266714,
		0.019411, 0.0133974, 0.00898536, 0.0057516, 0.00351493, 0.00212087, 0.00122891, 0.00070592, 0.000384744,
		0.000219377, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0 } };

// Distribution used for Fall2011 MC.

const boost::array<double, NWEIGHTSSIZE> Fall2011 = { { 0.003388501, 0.010357558, 0.024724258, 0.042348605, 0.058279812,
		0.068851751, 0.072914824, 0.071579609, 0.066811668, 0.060672356, 0.054528356, 0.04919354, 0.044886042,
		0.041341896, 0.0384679, 0.035871463, 0.03341952, 0.030915649, 0.028395374, 0.025798107, 0.023237445,
		0.020602754, 0.0180688, 0.015559693, 0.013211063, 0.010964293, 0.008920993, 0.007080504, 0.005499239,
		0.004187022, 0.003096474, 0.002237361, 0.001566428, 0.001074149, 0.000721755, 0.000470838, 0.00030268,
		0.000184665, 0.000112883, 6.74043E-05, 3.82178E-05, 2.22847E-05, 1.20933E-05, 6.96173E-06, 3.4689E-06,
		1.96172E-06, 8.49283E-07, 5.02393E-07, 2.15311E-07, 9.56938E-08 } };
}

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

const boost::array<double, NWEIGHTSSIZE> Summer2012 = { {
        2.560E-06,
        5.239E-06,
        1.420E-05,
        5.005E-05,
        1.001E-04,
        2.705E-04,
        1.999E-03,
        6.097E-03,
        1.046E-02,
        1.383E-02,
        1.685E-02,
        2.055E-02,
        2.572E-02,
        3.262E-02,
        4.121E-02,
        4.977E-02,
        5.539E-02,
        5.725E-02,
        5.607E-02,
        5.312E-02,
        5.008E-02,
        4.763E-02,
        4.558E-02,
        4.363E-02,
        4.159E-02,
        3.933E-02,
        3.681E-02,
        3.406E-02,
        3.116E-02,
        2.818E-02,
        2.519E-02,
        2.226E-02,
        1.946E-02,
        1.682E-02,
        1.437E-02,
        1.215E-02,
        1.016E-02,
        8.400E-03,
        6.873E-03,
        5.564E-03,
        4.457E-03,
        3.533E-03,
        2.772E-03,
        2.154E-03,
        1.656E-03,
        1.261E-03,
        9.513E-04,
        7.107E-04,
        5.259E-04,
        3.856E-04,
        2.801E-04,
        2.017E-04,
        1.439E-04,
        1.017E-04,
        7.126E-05,
        4.948E-05,
        3.405E-05,
        2.322E-05,
        1.570E-05,
        5.005E-06} };

const boost::array<double, NWEIGHTSSIZE> Summer11Leg = { {
		1.30976e-05,
		0.000148266,
		0.00226073,
		0.030543,
		0.0868303,
		0.120295,
		0.124687,
		0.110419,
		0.0945742,
		0.0837875,
		0.0774277,
		0.0740595,
		0.0676844,
		0.0551203,
		0.0378357,
		0.0210203,
		0.00918262,
		0.00309786,
		0.000808509,
		0.000168568,
		3.02344e-05,
		5.16455e-06,
		8.83185e-07,
		1.43975e-07,
		2.07228e-08,
		2.51393e-09,
		2.52072e-10,
		2.07328e-11,
		1.39369e-12,
		7.63843e-14,
		3.4069e-15,
		1.23492e-16,
		3.63059e-18,
		8.53277e-20,
		1.33668e-22,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0} };

#endif /* EVENTWEIGHTPROVIDER_H_ */
